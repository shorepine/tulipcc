// user_c_dsp_web.js
// Web plumbing for runtime-installed user C DSP (tulip.install_c_process /
// install_c_osc). The web builds append this file to AMY's amy.js, so it runs
// in BOTH scopes that evaluate that script:
//
//   - the page, where micropython's user_c_dsp.c (xcc700w backend) hands us
//     freshly compiled wasm module bytes via tulip_udsp_install() below
//   - AMY's AudioWorkletGlobalScope, where amy.js is re-evaluated by
//     emscripten's audio worklet bootstrap and AMY's render loop calls the
//     amy_*_js_hook functions we define here
//
// The two scopes share only AMY's wasm linear memory (a SharedArrayBuffer:
// the AMY build uses -sWASM_WORKERS). The page allocates a control block in
// that memory and registers it with AMY's exported
// _amy_set_external_hook_context(); the worklet side finds it through
// _amy_get_external_hook_context() on the Module object AMY passes to its js
// hooks. Installs travel through per-slot mailboxes in the block; the worklet
// instantiates the user module importing AMY's memory (so buffer pointers may
// be used directly) and AMY exports like cos_lut (wasm-to-wasm), and runs it
// from the hooks. Bus masks, osc bindings and call counts also live in the
// block, so the page can flip them and read liveness without messaging.
//
// Control block layout (i32 words; TU_ = this file's shared constants):
//   [0]                  magic
//   [1 + slot*8 .. +7]   mailbox: pending_seq, done_seq, kind (0 effect,
//                        1 osc, -1 uninstall), wasm_ptr, wasm_len, bss_base,
//                        rodata_base, stack_top
//   [65 + slot*2 .. +1]  state: bus_mask, calls
//   [81 + osc]           osc_map: osc -> slot+1 (0 = unbound), 256 oscs
//   [337]                worklet error count (page reads it for debugging —
//   [338..401]           an uncaught worklet exception would otherwise kill
//                        the audio thread with nothing in the page console);
//                        338.. is the latest message, NUL-terminated ASCII
(() => {
  'use strict';
  const TU_SLOTS = 8;
  const TU_MAGIC = 0x7D5B0001;
  const TU_MB = (s) => 1 + s * 8;      // mailbox base word for slot s
  const TU_ST = (s) => 65 + s * 2;     // state base word for slot s
  const TU_OM = 81;                    // osc_map base word
  const TU_OSCS = 256;
  const TU_BEACON = TU_OM + TU_OSCS;   // worklet hook heartbeat (debug)
  const TU_ERR = TU_BEACON + 1;        // error count word
  const TU_ERRMSG_BYTES = 256;
  const TU_WORDS = TU_ERR + 1 + TU_ERRMSG_BYTES / 4;  // total block words
  const TU_STACK_BYTES = 16384;        // shadow stack per user module
  const align16 = (n) => (n + 15) & ~15;

  if (globalThis.AudioWorkletGlobalScope) {
    // ------------------------------------------------------- worklet side --
    // Runs on the audio thread. State here: the live instances per slot.
    const live = new Array(TU_SLOTS).fill(null);      // {fn, kind}
    let ctx = 0;         // control block byte address (0 until page registers)
    let i32 = null;      // Int32Array view of AMY memory (refreshed on growth)

    const views = (mod) => {
      const buf = mod.wasmMemory.buffer;
      if (i32 === null || i32.buffer !== buf || i32.byteLength !== buf.byteLength) {
        i32 = new Int32Array(buf);
      }
      return i32;
    };

    // An exception escaping a hook would kill the audio thread with nothing
    // in the page console, so record it in the control block (the page reads
    // it back via tulip_udsp_debug()) and also try console.error.
    const record = (mod, e) => {
      try { console.error('user C DSP (worklet):', e); } catch (_) { /* no console here */ }
      try {
        if (!ctx) return;
        const buf = mod.wasmMemory ? mod.wasmMemory.buffer : mod.HEAPU8.buffer;
        const msg = String((e && e.stack) || e).slice(0, TU_ERRMSG_BYTES - 1);
        const u8 = new Uint8Array(buf, ctx + (TU_ERR + 1) * 4, TU_ERRMSG_BYTES);
        for (let i = 0; i < msg.length; i++) u8[i] = msg.charCodeAt(i) & 0x7f;
        u8[msg.length] = 0;
        new Int32Array(buf)[(ctx >> 2) + TU_ERR]++;
      } catch (_) { /* leave only the console trace */ }
    };

    // Check the per-slot mailboxes for new installs/uninstalls. Called once
    // per block from the bus hook (which AMY fires for bus 0 every block).
    // Instantiation is synchronous: user modules are sub-2KB (sync compile
    // has no size cap off the main thread), and the AudioWorklet scope never
    // dispatches the task that would complete an async WebAssembly.compile —
    // its promise just hangs. A fresh install is live a block after
    // install_c_process() returns; c_process_calls() shows liveness.
    const poll = (mod) => {
      if (!ctx) {
        const found = mod._amy_get_external_hook_context();
        if (!found) return;
        ctx = found;  // set before validating so record() can write
        if (views(mod)[ctx >> 2] !== TU_MAGIC) {
          record(mod, 'control block at ' + found + ' has bad magic ' + views(mod)[ctx >> 2]);
          ctx = 0;
          return;
        }
      }
      const m = views(mod);
      m[(ctx >> 2) + TU_BEACON]++;  // page-visible heartbeat: poll is running
      const c = ctx >> 2;
      for (let s = 0; s < TU_SLOTS; s++) {
        const mb = c + TU_MB(s);
        const pending = Atomics.load(m, mb);
        if (pending === Atomics.load(m, mb + 1)) continue;
        const kind = m[mb + 2];
        if (kind < 0) {  // uninstall
          live[s] = null;
          Atomics.store(m, mb + 1, pending);
          continue;
        }
        try {
          // Copy the module bytes out of shared memory (compile wants a
          // non-shared buffer, and the page may free/reuse them once acked).
          const bytes = new Uint8Array(mod.wasmMemory.buffer, m[mb + 3], m[mb + 4]).slice();
          const module = new WebAssembly.Module(bytes);
          // Undefined functions in the user's C are env.* imports; wire
          // them straight to AMY's exports (e.g. cos_lut) so the call
          // stays wasm-to-wasm. The three base globals place statics.
          const env = {
            memory: mod.wasmMemory,
            __stack_base: m[mb + 7],
            __bss_base: m[mb + 5],
            __rodata_base: m[mb + 6],
          };
          for (const imp of WebAssembly.Module.imports(module)) {
            if (imp.module !== 'env' || imp.kind !== 'function' || imp.name in env) continue;
            const fn = mod['_' + imp.name];
            if (typeof fn !== 'function') throw new Error("no AMY export for '" + imp.name + "'");
            env[imp.name] = fn;
          }
          const instance = new WebAssembly.Instance(module, { env });
          live[s] = { fn: instance.exports[kind === 1 ? 'render' : 'process'], kind };
        } catch (e) {
          record(mod, e);
          live[s] = null;
        }
        Atomics.store(m, mb + 1, pending);
      }
    };

    // End of a bus' FX chain, every block (see docs/api.md in AMY). buf is
    // nchans sequential channel blocks of len S8.23 samples, processed in
    // place by every enabled effect slot.
    globalThis.amy_bus_postprocess_js_hook = (bus, buf, len, nchans, mod) => {
      try {
        if (bus === 0) poll(mod);
        if (!ctx) return;
        const m = views(mod);
        const c = ctx >> 2;
        for (let s = 0; s < TU_SLOTS; s++) {
          const slot = live[s];
          if (slot === null || slot.kind !== 0) continue;
          if (!(m[c + TU_ST(s)] & (1 << bus))) continue;
          slot.fn(buf, len, nchans);
          m[c + TU_ST(s) + 1]++;
        }
      } catch (e) {
        record(mod, e);
      }
    };

    // Per audible osc, after AMY renders it. If bound to a user oscillator,
    // replace the waveform in buf (mono) and return 0 so AMY still pans and
    // mixes it — same contract as the native tulip_user_render_hook.
    globalThis.amy_render_js_hook = (osc, buf, len, phaseInc, amp, mod) => {
      try {
        if (!ctx) return 0;
        const m = views(mod);
        const c = ctx >> 2;
        const bound = (osc >= 0 && osc < TU_OSCS) ? m[c + TU_OM + osc] : 0;
        if (!bound) return 0;
        const slot = live[bound - 1];
        if (slot === null || slot.kind !== 1) return 0;
        m.fill(0, buf >> 2, (buf >> 2) + len);  // replace AMY's carrier entirely
        if (amp < 0) amp = 0;
        if (amp > 1) amp = 1;
        slot.fn(buf, len, osc, (phaseInc * 65536) | 0, (amp * (1 << 23)) | 0);
        m[c + TU_ST(bound - 1) + 1]++;
      } catch (e) {
        record(mod, e);
      }
      return 0;
    };
    return;
  }

  // ---------------------------------------------------------- page side --
  // Called from micropython's user_c_dsp.c via EM_ASM. The AMY module object
  // is the page global amy_module (set by spss.js once amyModule() resolves).
  // Return codes mirror user_c_dsp.h: -1 no such name / bad args, -2 AMY not
  // ready, -3 slots full, -4 name exists as the other kind.
  const names = new Map();  // name -> {slot, kind}
  let ctx = 0;
  // Allocations a slot's *previous* install may still be reading; freed once
  // that slot's done_seq catches up.
  const pendingFree = [];   // {slot, seq, ptrs: [..]}
  const owned = new Array(TU_SLOTS).fill(null);  // current {ptrs} per slot

  const am = () => (globalThis.amy_module && globalThis.amy_module.wasmMemory) ? globalThis.amy_module : null;
  const heap32 = (a) => new Int32Array(a.wasmMemory.buffer);
  const heapU8 = (a) => new Uint8Array(a.wasmMemory.buffer);

  const ensureCtx = (a) => {
    if (ctx) return ctx;
    ctx = a._malloc(TU_WORDS * 4);
    heapU8(a).fill(0, ctx, ctx + TU_WORDS * 4);
    heap32(a)[ctx >> 2] = TU_MAGIC;
    a._amy_set_external_hook_context(ctx);
    return ctx;
  };

  const sweepFrees = (a) => {
    const m = heap32(a);
    for (let i = pendingFree.length - 1; i >= 0; i--) {
      const pf = pendingFree[i];
      if (Atomics.load(m, (ctx >> 2) + TU_MB(pf.slot) + 1) >= pf.seq) {
        for (const p of pf.ptrs) a._free(p);
        pendingFree.splice(i, 1);
      }
    }
  };

  // Post a mailbox command for slot: write the fields, then bump pending_seq
  // (release) — the worklet reads pending_seq first (acquire).
  const post = (a, slot, kind, wasmPtr, wasmLen, bssBase, rodataBase, stackTop) => {
    const m = heap32(a);
    const mb = (ctx >> 2) + TU_MB(slot);
    m[mb + 2] = kind;
    m[mb + 3] = wasmPtr;
    m[mb + 4] = wasmLen;
    m[mb + 5] = bssBase;
    m[mb + 6] = rodataBase;
    m[mb + 7] = stackTop;
    const seq = Atomics.load(m, mb) + 1;
    Atomics.store(m, mb, seq);
    return seq;
  };

  globalThis.tulip_udsp_install = (name, kind, bytes, bssSize, rodataSize) => {
    const a = am();
    if (!a) return -2;
    ensureCtx(a);
    sweepFrees(a);
    let entry = names.get(name);
    if (entry && entry.kind !== kind) return -4;
    let slot;
    if (entry) {
      slot = entry.slot;
    } else {
      const used = new Set([...names.values()].map((e) => e.slot));
      slot = -1;
      for (let s = 0; s < TU_SLOTS; s++) if (!used.has(s)) { slot = s; break; }
      if (slot < 0) return -3;
    }
    const m = heap32(a);
    const u8 = heapU8(a);
    // One allocation for the module's data: [bss][rodata][shadow stack].
    // rodata is written by instantiation itself (the module carries it as an
    // active data segment based at env.__rodata_base); bss we zero here.
    const bssA = align16(bssSize), rodataA = align16(rodataSize);
    const wasmPtr = a._malloc(bytes.length);
    const dataPtr = a._malloc(bssA + rodataA + TU_STACK_BYTES);
    u8.set(bytes, wasmPtr);
    u8.fill(0, dataPtr, dataPtr + bssA);
    const stackTop = (dataPtr + bssA + rodataA + TU_STACK_BYTES) & ~15;
    if (!entry) {
      // Fresh slot: no bus enables / osc bindings yet. (Reinstalls keep
      // both, so the dev loop is edit + reinstall.)
      m[(ctx >> 2) + TU_ST(slot)] = 0;
    }
    m[(ctx >> 2) + TU_ST(slot) + 1] = 0;  // calls
    // The previous install's memory may still be executing until the worklet
    // acks this seq; free it lazily.
    const seq = post(a, slot, kind, wasmPtr, bytes.length, dataPtr, dataPtr + bssA, stackTop);
    if (owned[slot]) pendingFree.push({ slot, seq, ptrs: owned[slot].ptrs });
    owned[slot] = { ptrs: [wasmPtr, dataPtr] };
    names.set(name, { slot, kind });
    return slot;
  };

  globalThis.tulip_udsp_set = (name, bus, on) => {
    const a = am();
    if (!a || !ctx) return -1;
    if (bus < 0 || bus >= 4) return -2;
    const entry = names.get(name);
    if (!entry) return -1;
    if (entry.kind !== 0) return -3;
    const m = heap32(a);
    const w = (ctx >> 2) + TU_ST(entry.slot);
    if (on) m[w] |= (1 << bus);
    else m[w] &= ~(1 << bus);
    return 0;
  };

  globalThis.tulip_udsp_bind_osc = (name, osc, on) => {
    const a = am();
    if (!a || !ctx) return -1;
    if (osc < 0 || osc >= TU_OSCS) return -2;
    const entry = names.get(name);
    if (!entry) return -1;
    if (entry.kind !== 1) return -3;
    heap32(a)[(ctx >> 2) + TU_OM + osc] = on ? entry.slot + 1 : 0;
    return 0;
  };

  globalThis.tulip_udsp_calls = (name) => {
    const a = am();
    if (!a || !ctx) return -1;
    const entry = names.get(name);
    if (!entry) return -1;
    return heap32(a)[(ctx >> 2) + TU_ST(entry.slot) + 1];
  };

  // Debug: worklet-side heartbeat (poll invocations) plus how many exceptions
  // the worklet has recorded and the last one's message. Not Python API.
  globalThis.tulip_udsp_debug = () => {
    const a = am();
    if (!a || !ctx) return null;
    const m = heap32(a);
    const count = m[(ctx >> 2) + TU_ERR];
    const u8 = heapU8(a);
    let msg = '';
    for (let i = ctx + (TU_ERR + 1) * 4; count && u8[i] !== 0 && msg.length < TU_ERRMSG_BYTES; i++) {
      msg += String.fromCharCode(u8[i]);
    }
    return { beacon: m[(ctx >> 2) + TU_BEACON], errors: count, msg };
  };

  globalThis.tulip_udsp_uninstall = (name) => {
    const a = am();
    if (!a || !ctx) return -1;
    const entry = names.get(name);
    if (!entry) return -1;
    const m = heap32(a);
    m[(ctx >> 2) + TU_ST(entry.slot)] = 0;
    for (let o = 0; o < TU_OSCS; o++) {
      const w = (ctx >> 2) + TU_OM + o;
      if (m[w] === entry.slot + 1) m[w] = 0;
    }
    const seq = post(a, entry.slot, -1, 0, 0, 0, 0, 0);
    if (owned[entry.slot]) pendingFree.push({ slot: entry.slot, seq, ptrs: owned[entry.slot].ptrs });
    owned[entry.slot] = null;
    names.delete(name);
    m[(ctx >> 2) + TU_ST(entry.slot) + 1] = 0;
    sweepFrees(a);
    return 0;
  };
})();
