# amyboard.py
import tulip, midi, amy, time, struct, time, sys, os
from tulip import wifi, upgrade

i2c = None
display = None # Display instance (unified interface for all display types)

# Web encoder emulation state (used when running on AMYBOARD_WEB)
_web_encoder_pos = 0      # cumulative encoder position
_web_encoder_button = False  # current button state

def web():
    return (tulip.board()=="AMYBOARD_WEB")


class Display:
    """Unified display interface for ssd1327, sh1107, and web framebuffer.

    Drawing methods (text, fill_rect, line, fill, pixel, rect, scroll) are
    forwarded to the underlying framebuf.  ``show()`` does the right thing
    for each backend so callers never need to know which hardware is present.
    """
    WIDTH = 128
    HEIGHT = 128
    LINEHEIGHT = 12

    def __init__(self):
        self._hw = None        # hardware driver (ssd1327 or sh1107), None for web
        self._fb = None        # framebuf used for drawing
        self._buf = None       # raw byte buffer backing the framebuf
        self._is_web = web()

        if self._is_web:
            import framebuf
            self._buf = bytearray(self.WIDTH * self.HEIGHT // 2)
            self._fb = framebuf.FrameBuffer(self._buf, self.WIDTH, self.HEIGHT, framebuf.GS4_HMSB)
        else:
            # Try ssd1327 first, then sh1107
            try:
                hw = ssd1327_oled()
                self._hw = hw
                self._buf = hw.buffer
                self._fb = hw.framebuf
            except Exception:
                try:
                    hw = sh1107_oled()
                    self._hw = hw
                    self._buf = hw.displaybuf
                    self._fb = hw  # sh1107 *is* a FrameBuffer subclass
                except Exception:
                    pass  # no physical display

    @property
    def buffer(self):
        return self._buf

    @property
    def available(self):
        return self._fb is not None

    # -- drawing primitives (forwarded to framebuf) --

    def text(self, string, x, y, col=255):
        if self._fb is not None:
            self._fb.text(string, x, y, col)

    def fill(self, col):
        if self._fb is not None:
            self._fb.fill(col)

    def fill_rect(self, x, y, w, h, col):
        if self._fb is not None:
            self._fb.fill_rect(x, y, w, h, col)

    def line(self, x1, y1, x2, y2, col):
        if self._fb is not None:
            self._fb.line(x1, y1, x2, y2, col)

    def pixel(self, x, y, col=None):
        if self._fb is None:
            return None
        if col is None:
            return self._fb.pixel(x, y)
        self._fb.pixel(x, y, col)

    def rect(self, x, y, w, h, col):
        if self._fb is not None:
            self._fb.rect(x, y, w, h, col)

    def scroll(self, dx, dy):
        if self._fb is not None:
            self._fb.scroll(dx, dy)

    # Next-layer-up API
    def clear(self):
        self.fill(0)
        self.show()

    def message(self, message, row=0, inverse=False):
        """Display some text on a row on the display."""
        top_y = self.LINEHEIGHT * row
        bw = (255, 0) if inverse else (0, 255)
        self.fill_rect(0, top_y, self.WIDTH, self.LINEHEIGHT, bw[0])
        self.text(message, 0, top_y, bw[1])
        self.show()

    # -- refresh --

    def show(self):
        """Push the framebuffer to the display hardware (or web bridge)."""
        if self._fb is None:
            return
        if self._is_web:
            tulip.framebuf_web_update(self._buf)
        elif self._hw is not None:
            self._hw.show()

    # convenience alias so old code calling display_refresh() on a Display works
    refresh = show

DEFAULT_SKETCH_SOURCE = """\
# AMYboard Sketch
# Code put here runs first, then loop() is called every 32nd note.
import amyboard, amy

def loop():
    pass

# Do not edit. Set automatically by the knobs on AMYboard Online.
_auto_generated_knobs = \"\"\"
\"\"\"
"""

def _path_exists(path):
    import os
    try:
        os.stat(path)
        return True
    except OSError:
        return False

def _remove_tree(path):
    import os
    try:
        mode = os.stat(path)[0]
    except OSError:
        return
    if mode & 0x4000:  # directory
        try:
            entries = os.ilistdir(path)
        except OSError:
            entries = []
        for entry in entries:
            name = entry[0]
            if name == "." or name == "..":
                continue
            _remove_tree(path.rstrip("/") + "/" + name)
        try:
            os.rmdir(path)
        except OSError:
            pass
    else:
        try:
            os.remove(path)
        except OSError:
            pass

def _clear_directory(path):
    import os
    try:
        entries = os.ilistdir(path)
    except OSError:
        return
    for entry in entries:
        name = entry[0]
        if name == "." or name == "..":
            continue
        _remove_tree(path.rstrip("/") + "/" + name)


def load_patch_file(filename, synth=1, num_voices=6):
    """Load a .patch file (list of AMY wire commands) into a given synth."""

    def _num_oscs_for_patch(patch):
        """Scan for how many oscs are used in a patch."""
        top_osc = 0
        for line in patch.split('\n'):
            if not line:
                continue
            if line[0] == 'v':
                osc_num = int(line[1])  # Cannot see osc nums > 9.
            if osc_num > top_osc:
                top_osc = osc_num
        return top_osc + 1

    try:
        with open(filename, 'rb') as f:
            patch = f.read().decode('latin-1')
    except OSError:
        print("error reading", filename)
        return

    # Reset global FX.
    amy.send_raw("V1x0,0,0M0,500,,0,0k0,320,0.5,0.5h0,0.85,0.5,3000")
    # Reset the synth.
    oscs_per_voice = _num_oscs_for_patch(patch)
    amy.send_raw('i%div%din%d' % (synth, num_voices, oscs_per_voice))
    # Execute the patch.
    for line in patch.split('\n'):
        if line and not line.startswith('#'):
            amy.send_raw('i%d%s' % (synth, line))


def save_patch_file(filename, synth=1):
    """Write the current state of a synth out to a patch file."""
    patch_commands = amy.get_synth_commands(synth)
    with open(filename, 'w') as f:
        f.write(patch_commands)


def ensure_user_environment():
    import os
    user_base = tulip.root_dir() + "user"
    current_base = user_base + "/current"
    for path in (user_base, current_base):
        try:
            os.mkdir(path)
        except OSError:
            pass

    current_sketch_file = current_base + "/sketch.py"
    try:
        open(current_sketch_file, "r").close()
    except OSError:
        w = open(current_sketch_file, "w")
        w.write(DEFAULT_SKETCH_SOURCE)
        w.close()

    return current_base

def _ensure_current_env_layout():
    return ensure_user_environment()


# -- _auto_generated_knobs helpers --

_KNOBS_MARKER = '_auto_generated_knobs = """'
_KNOBS_END = '"""'
_KNOBS_MARKER_B = _KNOBS_MARKER.encode("utf-8")
_KNOBS_END_B = _KNOBS_END.encode("utf-8")

def generate_knobs_text():
    """Return the full AMY state text (same as zW output) using the C helper."""
    return tulip.amy_dump_state()

def _strip_all_knobs_blocks(data):
    """Remove every _auto_generated_knobs = \"\"\"...\"\"\" block from data (bytes).
    Also strips the preceding comment line on each removed block so we don't
    leave orphaned comments behind. Returns the cleaned bytes. Idempotent.

    Self-heals files that accidentally ended up with multiple knobs blocks
    (e.g. from a prior corrupted save that appended instead of replacing)."""
    comment_b = b"# Do not edit. Set automatically by the knobs on AMYboard Online."
    while True:
        start = data.find(_KNOBS_MARKER_B)
        if start < 0:
            break
        end = data.find(_KNOBS_END_B, start + len(_KNOBS_MARKER_B))
        if end < 0:
            # Unterminated block — chop from the marker onward.
            data = data[:start]
            break
        # Also absorb the preceding comment line if present, so repeated
        # strip+re-add doesn't accumulate stray comment lines.
        cut_from = start
        comment_idx = data.rfind(comment_b, 0, start)
        if comment_idx >= 0:
            # Back up to the newline before the comment (if any) so we remove
            # the full comment line plus its trailing newline.
            line_start = data.rfind(b"\n", 0, comment_idx)
            if line_start < 0:
                line_start = 0
            else:
                line_start += 1  # keep the \n terminating the previous line
            # Only absorb the comment if the only thing between it and the
            # marker is whitespace.
            gap = data[comment_idx + len(comment_b):start]
            if gap.strip() == b"":
                cut_from = line_start
        # Also absorb the closing """ + trailing whitespace up to the next \n.
        cut_to = end + len(_KNOBS_END_B)
        # Absorb any trailing whitespace / single newline after the closing quote.
        while cut_to < len(data) and data[cut_to:cut_to + 1] in (b" ", b"\t"):
            cut_to += 1
        if cut_to < len(data) and data[cut_to:cut_to + 1] == b"\n":
            cut_to += 1
        data = data[:cut_from] + data[cut_to:]
    return data


def update_sketch_knobs(sketch_path=None):
    """Read sketch.py, replace _auto_generated_knobs section with current AMY state, write back.

    Works at the byte level so we never trip over non-UTF-8 bytes that may
    have ended up in the file (e.g. from a previous partial/binary transfer).

    If the file already contains multiple knobs blocks (or fragments from a
    prior corrupted save), all of them are stripped and a single fresh block
    is appended. This is self-healing.

    This function is called synchronously from the C-side zA handler via
    mp_call_function_1, so any uncaught exception would unwind the MicroPython
    NLR stack and abort the sysex parser mid-message. We wrap the whole thing
    in a top-level try/except that only emits a stderr log on failure."""
    try:
        if sketch_path is None:
            sketch_path = tulip.root_dir() + "user/current/sketch.py"
        tulip.stderr_write("update_sketch_knobs: path=%s" % sketch_path)
        try:
            data = open(sketch_path, "rb").read()
        except OSError:
            tulip.stderr_write("update_sketch_knobs: file missing, creating default")
            _ensure_current_env_layout()
            try:
                data = open(sketch_path, "rb").read()
            except OSError:
                tulip.stderr_write("update_sketch_knobs: still can't read, giving up")
                return
        try:
            knobs = generate_knobs_text()
        except Exception as e:
            tulip.stderr_write("update_sketch_knobs: generate_knobs_text failed: %s" % e)
            return
        tulip.stderr_write("update_sketch_knobs: knobs=%d bytes" % len(knobs))
        knobs_b = knobs.encode("utf-8") if isinstance(knobs, str) else knobs
        # Strip every existing knobs block (defensive — self-heal if the
        # file picked up duplicates from a prior bad save), then append one
        # fresh block at the end.
        data = _strip_all_knobs_blocks(data)
        # Ensure exactly one trailing newline before we append the fresh block.
        while data.endswith(b"\n\n"):
            data = data[:-1]
        if not data.endswith(b"\n"):
            data = data + b"\n"
        data = (
            data
            + b"\n# Do not edit. Set automatically by the knobs on AMYboard Online.\n"
            + _KNOBS_MARKER_B + b"\n"
            + knobs_b
            + (b"" if knobs_b.endswith(b"\n") else b"\n")
            + _KNOBS_END_B + b"\n"
        )
        with open(sketch_path, "wb") as f:
            f.write(data)
    except Exception as e:
        tulip.stderr_write("update_sketch_knobs: unexpected error: %s: %s" % (type(e).__name__, e))

def _extract_knobs_from_file(filepath):
    """Read sketch.py as bytes and extract the _auto_generated_knobs content without importing.

    Reads as bytes so non-UTF-8 bytes elsewhere in the file don't break us;
    the knobs section itself is always ASCII (AMY wire protocol)."""
    try:
        data = open(filepath, "rb").read()
    except OSError:
        return ''
    start = data.find(_KNOBS_MARKER_B)
    if start < 0:
        return ''
    body_start = start + len(_KNOBS_MARKER_B)
    end = data.find(_KNOBS_END_B, body_start)
    if end < 0:
        return ''
    body = data[body_start:end]
    try:
        return body.decode("utf-8").strip()
    except UnicodeError:
        # Knobs section had bad bytes somehow — fall back to ASCII-only decode.
        try:
            return "".join(chr(b) for b in body if b < 128).strip()
        except Exception:
            return ''

def _apply_knobs_text(knobs_text):
    """Send each line of knobs text to AMY."""
    if not knobs_text:
        # First run / factory reset: clear any previously configured synths
        # (drums on ch10, etc.) and set up channel 1 with the default Juno patch.
        amy.send_raw("S%dZ" % amy.RESET_SYNTHS)
        amy.send_raw("i1K257iv6Z")
        amy.send_raw("i1ic255Z")
        return
    # Find all active channels and their oscillator counts, then init each
    # with the right voice/osc structure before applying the saved wire code.
    lines = knobs_text.strip().split('\n')
    channel_oscs = {}  # channel → max osc index seen
    for line in lines:
        line = line.strip()
        if not line or not line.startswith('i'):
            continue
        # Extract channel number (i1..., i16...)
        num = ''
        rest = ''
        for idx, c in enumerate(line[1:], 1):
            if c.isdigit():
                num += c
            else:
                rest = line[idx:]
                break
        if not num:
            continue
        ch = int(num)
        if ch not in channel_oscs:
            channel_oscs[ch] = 0
        # Scan for vN (osc number) in this channel's wire code
        for part in rest.split('v'):
            if part and part[0].isdigit():
                osc_num = int(part[0])
                if osc_num > channel_oscs[ch]:
                    channel_oscs[ch] = osc_num
    for ch in sorted(channel_oscs):
        oscs_per_voice = channel_oscs[ch] + 1
        amy.send_raw("i%dic255Z" % ch)
        amy.send_raw("i%div6in%dZ" % (ch, oscs_per_voice))
    for line in knobs_text.strip().split('\n'):
        line = line.strip()
        if line and not line.startswith('#'):
            amy.send_raw(line)


def factory_reset(*_args):
    """Reset AMYboard to defaults: clear current/ folder and restart sketch."""
    import os
    tulip.stderr_write("factory reset — clearing current/ and writing default sketch.py")
    user_base = tulip.root_dir() + "user"
    current_base = user_base + "/current"
    try:
        for f in os.listdir(current_base):
            try:
                os.remove(current_base + "/" + f)
            except OSError:
                pass
    except OSError:
        pass
    env_dir = _ensure_current_env_layout()
    restart_sketch()

def environment_transfer_done(*_args):
    """Called after sketch.py is received via zT file transfer.
    The transferred file already has the correct _auto_generated_knobs,
    so we just restart the sketch without re-injecting the (stale) AMY state."""
    tulip.stderr_write("sketch.py transfer done — restarting sketch")
    # zT stops the sequencer during transfer — restart it before running the sketch.
    tulip.sequencer_start()
    restart_sketch()


def mount_sd():
    # mount the SD card if given
    import machine, uos
    try:
        sd = machine.SDCard(sck=12, miso=13, mosi=11, cs=10,slot=2)
        uos.mount(uos.VfsFat(sd), '/sd')
        print("SD card mounted as /sd")
    except OSError:
        pass # it's ok!

def init_midi(type='A'):
    from machine import Pin
    # set up MIDI, default to type A
    # only have to set up MIDI OUT. MIDI IN will work either way
    if(type=='A'):
        # Type A: GPIO15 held high, MIDI OUT pin is 14
        pin = Pin(15, Pin.OUT)
        pin.value(1)
        return 14
    else:
        # Type B: GPIO14 held high, MIDI OUT pin is 15
        pin = Pin(14, Pin.OUT)  
        pin.value(1)
        return 15


def start_amy():
    init_pcm9211()
    midi_out_pin = init_midi()
    tulip.amyboard_start(midi_out_pin)
    _env_dir = _ensure_current_env_layout()

    if tulip.bootloader_mode():
        tulip.stderr_write("bootloader mode — sketch skipped, waiting for commands")
        return

    from upysh import cd
    try:
        from machine import Pin
        button = Pin(0, Pin.IN, Pin.PULL_UP)
        if button.value() != 0:  # Skip sketch if boot button held
            run_sketch()
    except Exception as e:
        print("Environment start failed:")
        sys.print_exception(e)


# -- sketch.py support (top-level init + loop) --

_sketch_seq = None  # Keep reference to prevent GC

def run_sketch():
    """Apply knobs from sketch.py, then import it (top-level code runs), start loop()."""
    _env_dir = _ensure_current_env_layout()
    from upysh import cd
    tulip.stderr_write("cding to %s" % (_env_dir))
    cd(_env_dir)

    # Restore AMY synth state BEFORE importing sketch.py so user code
    # can rely on the synth being fully configured.
    knobs_text = _extract_knobs_from_file(_env_dir + "/sketch.py")
    _apply_knobs_text(knobs_text)

    # Clear cached sketch module so we always load fresh from disk
    # (important after soft_reset which preserves sys.modules).
    if 'sketch' in sys.modules:
        del sys.modules['sketch']
    tulip.stderr_write("import sketch")
    if _env_dir not in sys.path:
        sys.path.insert(0, _env_dir)
    try:
        import sketch
    except Exception as e:
        # Route through stderr_write so the web console sees it alongside the
        # other diagnostics (stdout may not be wired up in some hosts).
        tulip.stderr_write("sketch.py load failed: %s: %s" % (type(e).__name__, e))
        try:
            sys.print_exception(e)
        except Exception:
            pass
        # Self-heal: if sketch.py is corrupted (SyntaxError from stray bytes,
        # etc.), overwrite it with the default and try once more so the board
        # doesn't end up stuck in a bad state across saves/syncs.
        try:
            tulip.stderr_write("sketch.py corrupted — restoring default")
            sketch_path = _env_dir + "/sketch.py"
            with open(sketch_path, "wb") as f:
                f.write(DEFAULT_SKETCH_SOURCE.encode("utf-8"))
            if 'sketch' in sys.modules:
                del sys.modules['sketch']
            import sketch
        except Exception as e2:
            tulip.stderr_write("default sketch.py load also failed: %s: %s" % (type(e2).__name__, e2))
            try:
                sys.print_exception(e2)
            except Exception:
                pass
            cd(tulip.root_dir() + "user")
            return

    if hasattr(sketch, 'loop'):
        _start_sketch_loop(sketch.loop)

    cd(tulip.root_dir() + "user")


def _start_sketch_loop(loop_fn):
    """Schedule loop_fn via TulipSequence (every 32nd note, ~60ms)."""
    import sequencer
    global _sketch_seq
    _loop_running = False

    def _guarded_loop(tick):
        nonlocal _loop_running
        if _loop_running:
            return
        _loop_running = True
        try:
            loop_fn()
        except Exception as e:
            print("sketch.loop() error:")
            sys.print_exception(e)
        finally:
            _loop_running = False

    _sketch_seq = sequencer.TulipSequence(32, _guarded_loop)


def stop_sketch():
    """Stop the sketch loop."""
    global _sketch_seq
    if _sketch_seq is not None:
        _sketch_seq.clear()
        _sketch_seq = None


def restart_sketch():
    """Reload and restart sketch.py."""
    stop_sketch()
    if 'sketch' in sys.modules:
        del sys.modules['sketch']
    # Ensure the sequencer is running so TulipSequence callbacks fire.
    tulip.sequencer_start()
    run_sketch()


def get_i2c():
    global i2c
    if(i2c is None):
        from machine import I2C
        i2c = I2C(0, freq=400000)
    return i2c

def ssd1327_oled():
    import ssd1327
    d = ssd1327.SSD1327_I2C(128,128,get_i2c(),addr=0x3d)
    d.fill(0)
    return d

def sh1107_oled():
    import sh1107
    d = sh1107.SH1107_I2C(128, 128, get_i2c(), address=0x3c)
    d.sleep(False)
    return d

def display_refresh():
    """Legacy helper — calls display.show() if a display exists."""
    if display is not None:
        display.show()

def display_startup():
    display.text("AMYboard!!!!", 0, 0, 255)
    display.text(tulip.version(), 0, 24, 255)
    display.show()

def init_display():
    global display
    display = Display()
    if display.available:
        display_startup()

def adc1115_raw(channel=0):
    import adc1115
    adc = adc1115.ADS1115(get_i2c())
    raw = float(adc.read(channel1=channel))
    return raw

# Calibrate the ADC1115 against the GP8413. Generates a csv file
def cv_cal(channel=0):
    vs = [-10,-9,-8,-7,-6,-5,-4,-3,-2,-1.5,-1,-0.5,0,0.5,1,1.5,2,3,4,5,6,7,8,9,10]
    print("Calibrating...")
    out = open("cal.csv", "w")
    for v in vs:
        cv_out(v, channel=channel)
        time.sleep(1)
        raw = adc1115(channel)
        out.write("%f,%d\n" % (v, raw))
    out.close()
    print("Done. Written to cal.csv")

def read_register(addr, reg):
    get_i2c().writeto(addr, bytes([reg]))
    b = get_i2c().readfrom(addr, 1)
    if(b):
        return int.from_bytes(b, "big")
    return None

def write_register(addr, reg, val):
    get_i2c().writeto(addr, bytes([reg, val]))

def init_pcm9211(addr=0x40):
    registers = [
        [ 0x40, 0x33 ], # Power down ADC, power down DIR, power down DIT, power down OSC
        [ 0x40, 0xc0 ], # Normal operation for all
        [ 0x34, 0x00 ], # Initialize DIR - both biphase amps on, input from RXIN0
        [ 0x26, 0x01 ], # Main Out is DIR/ADC if no DIR sync (these match power-on default, repeated for clarity).
        [ 0x6B, 0x00 ], # Main output pins are DIR/ADC AUTO
        [ 0x30, 0x04 ], # PLL sends 512fs as SCK
        [ 0x31, 0x0A ], # XTI SCK as 512fs too
        [ 0x60, 0x44 ], # Initialize PCM9211 DIT to send SPDIF from AUXIN1 through MPO0 (pin15).  MPO1 (pin16) is VOUT (Valid)
        [ 0x61, 0x20 ], # DIT SCK ratio = 512fs (must match PLL config in 0x30/0x31)
        [ 0x78, 0x3d ], # MPO0 = 0b1101 = TXOUT, MPO1 = 0b0011 = VOUT
        [ 0x6F, 0x40 ]  # MPIO_A = CLKST etc / MPIO_B = AUXIN2 / MPIO_C = AUXIN1
    ]
    for (reg, val) in registers:
        write_register(addr, reg, val)
        r = read_register(addr, reg)
        if(r==val): 
            print("Write 0x%02x to 0x%02x OK" % (val, reg))
        else:
            print("Write 0x%02x to 0x%02x returned 0x%02x" % (val, reg, r))

def set_cv_out(channel=0, synth=1):
    """Route a synth's audio output to a CV channel instead of speakers.

    channel: 0 for CV1, 1 for CV2
    synth: AMY synth number whose audio will be sent to this CV output

    The synth's oscillators will be silenced from the audio mix and their
    waveform sent to the DAC instead. Use amy.send(synth=N, ...) to control
    the CV waveform shape, frequency, etc.

    Call set_cv_out(channel, synth=0) to clear the mapping.
    """
    tulip.set_cv_synth(synth, channel + 1 if synth > 0 else 0)

def edit(filename=None):
    """Open the pye text editor. Pass a filename to edit, or None for a new file."""
    from pye import pye
    if filename is not None:
        pye(filename)
    else:
        pye()

def cv_out(volts, channel=0):
    """Output -10.0v to +10.0v (nominal) on CV1 (channel=0) or 2 (channel=1)"""
    addr = 88 # GP8413
    # With rev1 scaling, 0x0000 -> -10v, 0x7fff -> +10v
    val = int(((volts + 10)/20.0) * 0x8000)
    if(val < 0):
        val = 0
    if(val > 0x7fff):
        val = 0x7fff
    b1 = (val & 0xff00) >> 8
    b0 = (val & 0x00ff)
    ch = 0x02
    if(channel == 1):
        ch = 0x04
    get_i2c().writeto_mem(addr, ch, bytes([b0,b1]))

def cv_in(channel=0):
    return tulip.cv_in(channel)

# --- Web encoder emulation helpers (called from JS) ---

def _web_encoder_turn(delta):
    """Called from JS when the web encoder UI is turned."""
    global _web_encoder_pos
    _web_encoder_pos += delta

def _web_encoder_press(state):
    """Called from JS when the web encoder push button is pressed/released."""
    global _web_encoder_button
    _web_encoder_button = state


# Adafruit I2C Quad Rotary Encoder Breakout
# https://www.adafruit.com/product/5752
# Four rotary encoders with built-in push buttons.
#  read_encoder(encoder=0)  # reads the current value of the encoder
#  init_buttons()  # must be called to configure pullup of encoder buttons
#  read_buttons()  # returns a boolean list of the state of the 4 buttons.
# Code based on abstracting
# https://github.com/adafruit/Adafruit_CircuitPython_seesaw/blob/main/adafruit_seesaw/seesaw.py.

def read_encoder(encoder=0, seesaw_dev=0x49, delay=0.008):
    """Read the cumulated value of encoder 0..3."""
    if web():
        return _web_encoder_pos
    i2c = get_i2c()
    result = bytearray(4)
    ENCODER_BASE = 0x11
    ENCODER_POSITION = 0x30
    i2c.writeto(seesaw_dev, bytes([ENCODER_BASE, ENCODER_POSITION + encoder]))
    time.sleep(delay)
    i2c.readfrom_into(seesaw_dev, result)
    return struct.unpack(">i", result)[0]

def init_buttons(pins=(12, 14, 17, 9), seesaw_dev=0x49):
    """Setup the seesaw quad encoder button pins to input_pullup."""
    if web():
        return
    mask = 0
    for p in pins:
        mask |= (1 << p)
    mask_bytes = struct.pack('>I', mask)
    i2c = get_i2c()
    GPIO_BASE = 0x01
    GPIO_DIRCLR_BULK = 0x03
    GPIO_PULLENSET = 0x0B
    GPIO_BULK_SET = 0x05
    i2c.writeto(seesaw_dev, bytes([GPIO_BASE, GPIO_DIRCLR_BULK]) + mask_bytes)
    i2c.writeto(seesaw_dev, bytes([GPIO_BASE, GPIO_PULLENSET]) + mask_bytes)
    i2c.writeto(seesaw_dev, bytes([GPIO_BASE, GPIO_BULK_SET]) + mask_bytes)

def read_buttons(pins=(12, 14, 17, 9), seesaw_dev=0x49, delay=0.008):
    """Read the 4 seesaw encoder push buttons."""
    if web():
        return [_web_encoder_button] * len(pins)
    i2c = get_i2c()
    GPIO_BASE = 0x01
    GPIO_BULK = 0x04
    i2c.writeto(seesaw_dev, bytes([GPIO_BASE, GPIO_BULK]))
    time.sleep(delay)
    buffer = bytearray(4)
    i2c.readfrom_into(seesaw_dev, buffer)
    mask = struct.unpack('>I', buffer)[0]
    result = []
    for p in pins:
        state = True
        if (mask & (1 << p)):
            # bit set means button not pressed.
            state = False
        result.append(state)
    return result

def monitor_encoders():
    """Show status of encoders on display."""
    # You can run this in a loop to get continuous display of encoders
    # e.g.
    # >>> amyboard.init_buttons()
    # >>> for _ in range(100):
    # ...     amyboard.monitor_encoders()
    # ...
    bar_top = 20
    bar_height = 8
    bar_space = 12
    bar_center = 64
    buttons = read_buttons()
    for encoder in range(4):
        width = read_encoder(encoder)
        top = bar_top + encoder * bar_space
        # Clear existing bar
        display.fill_rect(0, top, 128, bar_space, 0)
        # Draw new bar
        if width < 0:
            center = bar_center + width
            width = -width
        else:
            center = bar_center
        display.fill_rect(center, top, width, bar_height, 255)
        if buttons[encoder]:
            # Small box at the left.
            display.fill_rect(2, top + 2, 2, 2, 255)
    display.show()

# To see when MIDI CCs are being changed, add to your sketch.py:
# amyboard.show_midi_ccs()
class ShowMidiCcs:
    """Class to manage state of deferred-display MIDI CC change tracker."""
    def __init__(self, ticks_to_display=20, top_row=4, bar_height=4):
        self.channel = None
        self.code = None
        self.value = None
        self.ticks_to_display = ticks_to_display
        self.ticks_to_live = 0
        # Where on display to place result
        self.top_row = top_row
        self.bar_height = bar_height

    def midi_hook(self, midi):
        """Attached to midi callback."""
        if midi and (midi[0] & 0xF0) == 0xB0:
            # We have a control code.
            self.channel = 1 + (midi[0] & 0x0F)  # MIDI channels are numbered 1..16.
            self.code = midi[1]
            self.value = midi[2]

    def update(self):
        if self.channel is not None:
            display.message("CH %02d CC %03d" % (self.channel, self.code), self.top_row)
            display.fill_rect(0, (self.top_row + 1) * display.LINEHEIGHT + 1, self.value, self.bar_height, 1)
            display.fill_rect(self.value, (self.top_row + 1) * display.LINEHEIGHT + 1, display.WIDTH - self.value, self.bar_height, 0)
            display.show()
            self.channel = None  # Mark update as handled
            self.ticks_to_live = self.ticks_to_display  # Reset blanking counter
        elif self.ticks_to_live:
            self.ticks_to_live -= 1
            if self.ticks_to_live == 0:
                # Clear the old display.
                display.fill_rect(0, self.top_row * display.LINEHEIGHT, display.WIDTH, 2 * display.LINEHEIGHT, 0)
                display.show()

def show_midi_ccs(ticks_to_display=20, top_row=4, bar_height=4):
    """Setup hook to show last MIDI CC on the display."""
    import midi
    import sequencer
    show_midi_obj = ShowMidiCcs(ticks_to_display, top_row, bar_height)
    midi.add_callback(show_midi_obj.midi_hook)
    seq = sequencer.TulipSequence(32, lambda x: show_midi_obj.update())
    return seq, show_midi_obj
    

WAVEFORM_MAX = 32767.0

def draw_waveform():
    """Plot some of the output waveform on the oled."""
    # e.g.
    # >>> for _ in range(100):
    # ...     amyboard.draw_waveform()
    waveform = struct.unpack('<' + (512 * 'h'), tulip.amy_get_output_buffer())
    waveform_top = 0
    waveform_left = 0
    waveform_height = 64
    waveform_width = 128
    display.fill_rect(waveform_left, waveform_top,
                      waveform_width, waveform_height, 0)
    # center the largest sample
    max_value = 0
    max_value_pos = 0
    for x in range(len(waveform) // 2 - waveform_width):
        pos = waveform_width // 2 + x
        value = (waveform[2 * pos] + waveform[2 * pos + 1])
        if value > max_value:
            max_value = value
            max_value_pos = pos
    # Pos marks middle; shift it to the left edge
    pos = max_value_pos - waveform_width // 2
    for x in range(waveform_width):
        value = (waveform[2 * (pos + x)] + waveform[2 * (pos + x) + 1]) // 2
        y = waveform_top + int((waveform_height // 2)
                               * (1.0 - value / WAVEFORM_MAX))
        if x > 0:
            display.line(waveform_left + last_x, last_y,
                         waveform_left + x, y, 15)
        last_x = x
        last_y = y
    display.show()


# Patch Selector -
# If you have a display and a rotary encoder on I2C
# this function will let you scroll through your available patches
# and load each one by clicking the rotary encoder.
#
# You can launch it from sketch.py like this:
#
#   import amyboard
#   amyboard.patch_selector()
#
# or, for the 4-knob Adafruit rotary encoder:
#
#   amyboard.patch_selector(seesaw_dev=0x49, button_pin=12)

class PatchSelector:

    def __init__(
            self, synth=1, seesaw_dev=0x36, encoder=0, button_pin=24,
            patch_dir='/user/current', extension='.patch',
            hold_max=30, exit_callback=lambda: True
    ):
        """Endless loop scrolling through patch files and installing on click.  Long click rewrites the file."""
        self.new_file_prefix = '* '
        self.synth = synth
        self.seesaw_dev = seesaw_dev
        self.encoder = encoder
        self.button_pin = button_pin
        self.patch_dir = patch_dir
        self.extension = extension
        self.hold_max = hold_max  # Count before "long press" action in ~ deciseconds
        self.exit_callback = exit_callback  # Not currently used.
        # State
        self.patches = self._list_patches()
        self.index = 0  # within patches
        self.button_state = False
        self.hold_count = 0
        # Make sure we always start at the top of the list.
        self.last_encoder_value = 0
        self.encoder_offset = -self.read_encoder()
        # Initialize
        display.clear()
        display.message("PATCH SELECTOR", 0)
        if self.patches:
            display.message(self.patches[0], 2)
        init_buttons(pins=(self.button_pin,), seesaw_dev=self.seesaw_dev)

    def _list_patches(self):
        """List of the patch files."""
        try:
            patches = [f[:-len(self.extension)] for f in os.listdir(self.patch_dir) if f.endswith(self.extension)]
        except OSError:
            patches = []
        for new_patch_num in range(100):
            new_patch_name = '~NEW PATCH %02d' % new_patch_num
            if new_patch_name not in patches:
                patches.append(self.new_file_prefix + new_patch_name)
                break
        return patches

    def patch_name(self, index, remove_star=False):
        """Return the index'th patch name from the stored list, removing "* " prefix if needed."""
        name = self.patches[index % len(self.patches)]
        if remove_star and name and name[:len(self.new_file_prefix)] == self.new_file_prefix:
            name = name[len(self.new_file_prefix):]
        return name

    def read_encoder(self):
        value = self.last_encoder_value
        try:
            value = read_encoder(self.encoder, seesaw_dev=self.seesaw_dev)
        except OSError:
            # Ignore I2C errors
            pass
        self.last_encoder_vale = value
        return value
    
    def read_button(self):
        value = False
        try:
            value = read_buttons(pins=(self.button_pin,), seesaw_dev=self.seesaw_dev)[0]
        except OSError:
            # Ignore I2C errors
            pass
        return value

    def update(self):
        index = (self.read_encoder() + self.encoder_offset) % len(self.patches)
        button_state = self.read_button()
        if (index, button_state) != (self.index, self.button_state):
            # Only rewrite display if it's out of sync.
            display.message(self.patch_name(index), 2, inverse=button_state)
        if button_state:
            # Button is still down.
            self.hold_count += 1
            if self.hold_count == self.hold_max:  # Only act once, at the exact count.
                # Long hold - perform action (save)
                name = self.patch_name(index, remove_star=True)
                save_patch_file(self.patch_dir + '/' + name + self.extension, synth=self.synth)
                # Flash non-inverse
                display.message(name, 2, inverse=False)
                time.sleep(0.1)
                display.message(name, 2, inverse=True)
                # Rescan the files (and add a new NEW PATCH if we just materialized one)
                self.patches = self._list_patches()
                # Update the offset so that we're still on NEW PATCH if we just wrote it.
                self.encoder_offset = self.patches.index(name) - self.read_encoder()
        else:
            # Button is released.
            if self.button_state and self.hold_count < self.hold_max:
                # The button was just released *and* it wasn't a long-hold - perform laod.
                load_patch_file(self.patch_dir + '/' + self.patch_name(index) + self.extension, synth=self.synth)
            # Ensure count is reset
            self.hold_count = 0
        self.index = index
        self.button_state = button_state


def patch_selector(synth=1, duration=30, seesaw_dev=0x36, encoder=0, button_pin=24,
                   patch_dir=None, extension='.patch'):
    """Run the PATCH SELECTOR app using Tulip sequencer."""
    import sequencer
    if not web():
        if display is None or not display.available:
            print("You need a rotary encoder and OLED display connected")
            return
        try:
            read_encoder(encoder, seesaw_dev=seesaw_dev)
        except OSError:
            print("You need a rotary encoder and OLED display connected")
            return
    if patch_dir is None:
        patch_dir = tulip.root_dir() + 'user/current'
    seq = None

    def exit_callback():
        # Terminate the sequencing
        if seq:
            # Stop sequencer
            seq.clear()
            # Clear screen
            display.clear()

    # Initialize the patch selector
    patchsel_obj = PatchSelector(synth=synth, seesaw_dev=seesaw_dev, encoder=encoder, button_pin=button_pin,
                                 patch_dir=patch_dir, extension=extension, exit_callback=exit_callback)
    # Arrange for it to be called every 32nd note
    seq = sequencer.TulipSequence(32, lambda x: patchsel_obj.update())
    return seq, patchsel_obj
