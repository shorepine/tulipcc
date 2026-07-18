// Virtual MIDI endpoint for AMYboard VCV.
//
// Publishes a virtual MIDI in/out pair named "AMYboard VCV" so amyboard.com's
// editor (Chrome WebMIDI, "control" mode) sees the Rack module exactly like a
// hardware AMYboard: sketch transfer (zT), run-python (zP), dumps
// (zD), ping (zI) all flow over sysex.
//
// Backends: CoreMIDI virtual source/destination on macOS, an ALSA sequencer
// client/port on Linux. (Windows has no OS-level virtual MIDI — TODO: route
// via Rack midi::Output + a loopMIDI pair instead.)
//
// Inbound routing (shared across backends, handle_bytes/handle_sysex_frame):
//  - channel-voice MIDI -> AMY's byte-stream parser (notes/CCs, as UART)
//  - shorepine sysex (F0 00 03 45 ...): zI answered here in C (like
//    parse_sysex does on hardware); everything else is queued to the
//    MicroPython thread (mp_embed.c), which processes it with
//    amy_add_message_from_sysex() and then emits the AK — the same
//    process-on-MP-thread-then-ack design as AMYBOARD's scheduled
//    tulip_amy_send_sysex, without needing AMY's ring slots.
//  - other sysex -> amy_event_midi_message_received() so Python
//    midi.add_callback() sees it, as on hardware.
//
// Outbound: AMY and the firmware emit through the global midi_out()
// (zI OK replies, zT AKs, zD dump frames, Python tulip.midi_out()). The
// archive is built with -DAMY_HOST_MIDI (see amy_midi.h) so amy_midi.c
// skips its own device layer and this file owns the midi_out symbol.

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

extern void convert_midi_bytes_to_messages(uint8_t *data, size_t len, uint8_t usb);
extern void amy_event_midi_message_received(uint8_t *data, uint32_t len, uint8_t sysex, uint32_t time);
extern uint32_t amy_sysclock(void);
void midi_out(uint8_t *bytes, uint16_t len);
// mp_embed.c: queue a shorepine control payload for the MP thread (which
// processes it and sends the AK)
extern void amyboard_vcv_sysex_push(const uint8_t *payload, int len);
extern void amyboard_vcv_exec(const char *code);

#define VCV_SYSEX_MAX 16384
static uint8_t frame_buf[VCV_SYSEX_MAX];
static int frame_len = -1;  // -1 = not inside a sysex frame

static void handle_sysex_frame(uint8_t *frame, int len) {
    // frame[0]==0xF0 ... frame[len-1]==0xF7
    if (len >= 7 && frame[1] == 0x00 && frame[2] == 0x03 && frame[3] == 0x45) {
        uint8_t *payload = frame + 4;
        int plen = len - 5;
        if (plen >= 2 && payload[0] == 'z' && payload[1] == 'I') {
            // ping: reply OK immediately, no ack (mirrors parse_sysex)
            uint8_t ok[] = {0xF0, 0x00, 0x03, 0x45, 'O', 'K', 0xF7};
            midi_out(ok, sizeof(ok));
            return;
        }
        if (plen >= 2 && payload[0] == 'z' && payload[1] == 'B') {
            // "reboot": no MCU to reset — mode 0 means wait-for-commands,
            // anything else means run the sketch again. No ack, like hardware.
            if (plen >= 3 && payload[2] == '0')
                amyboard_vcv_exec("import amyboard; amyboard.stop_sketch()");
            else
                amyboard_vcv_exec("import amyboard; amyboard.restart_sketch()");
            return;
        }
        // Everything else (zT/zD/zP/zY, base64 transfer chunks): process
        // on the MP thread, which acks after handling.
        amyboard_vcv_sysex_push(payload, plen);
        return;
    }
    // Non-shorepine sysex: hand to the firmware's Python midi callback path.
    amy_event_midi_message_received(frame, (uint32_t)len, 1, amy_sysclock());
}

// Feed raw MIDI 1.0 bytes (possibly containing partial sysex across calls).
// Only ever called from the single platform MIDI-input thread, so the frame
// accumulator needs no locking; the Rack MIDI-menu path in AmyBoard.cpp uses
// amy_event_midi_message_received and never touches this state.
// ── Outbound queue for Rack's midi::Output ─────────────────────────────────
// Every midi_out() frame is also queued here; AmyModule::process() drains it
// into the module's MIDI output device (loopMIDI on Windows — the only
// outbound route there; an optional extra route on mac/linux alongside the
// virtual port). Heap copies, mutex-guarded, bounded.
#define VCV_MIDI_OUT_SLOTS 64
static struct { uint8_t *data; int len; } vcv_out_q[VCV_MIDI_OUT_SLOTS];
static int vcv_out_head = 0, vcv_out_tail = 0;
static pthread_mutex_t vcv_out_lock = PTHREAD_MUTEX_INITIALIZER;

static void vcv_midi_out_enqueue(const uint8_t *bytes, uint16_t len) {
    if (!len) return;
    uint8_t *copy = malloc(len);
    if (!copy) return;
    memcpy(copy, bytes, len);
    pthread_mutex_lock(&vcv_out_lock);
    int next = (vcv_out_tail + 1) % VCV_MIDI_OUT_SLOTS;
    if (next != vcv_out_head) {
        vcv_out_q[vcv_out_tail].data = copy;
        vcv_out_q[vcv_out_tail].len = len;
        vcv_out_tail = next;
    } else {
        free(copy);  // full: drop (editor retries on timeout)
    }
    pthread_mutex_unlock(&vcv_out_lock);
}

// Module side: pop one frame (returns length, 0 if empty; caller buffer).
int amyboard_vcv_midi_out_pop(uint8_t *buf, int maxlen) {
    int n = 0;
    pthread_mutex_lock(&vcv_out_lock);
    if (vcv_out_head != vcv_out_tail) {
        n = vcv_out_q[vcv_out_head].len;
        if (n <= maxlen)
            memcpy(buf, vcv_out_q[vcv_out_head].data, n);
        else
            n = 0;  // caller buffer too small: drop
        free(vcv_out_q[vcv_out_head].data);
        vcv_out_q[vcv_out_head].data = NULL;
        vcv_out_head = (vcv_out_head + 1) % VCV_MIDI_OUT_SLOTS;
    }
    pthread_mutex_unlock(&vcv_out_lock);
    return n;
}

// Rack MIDI-menu path (AmyBoard.cpp): hand a complete sysex frame over.
// Returns 1 if it was a shorepine control frame (consumed), 0 otherwise.
int amyboard_vcv_shorepine_frame(const uint8_t *bytes, int len) {
    if (len >= 7 && bytes[0] == 0xF0 && bytes[1] == 0x00 &&
        bytes[2] == 0x03 && bytes[3] == 0x45) {
        handle_sysex_frame((uint8_t *)bytes, len);
        return 1;
    }
    return 0;
}

static void handle_bytes(const uint8_t *data, int len) {
    int span_start = -1;
    for (int i = 0; i < len; i++) {
        uint8_t b = data[i];
        if (frame_len >= 0) {
            // inside a sysex frame
            if (b == 0xF7) {
                if (frame_len < VCV_SYSEX_MAX)
                    frame_buf[frame_len++] = b;
                handle_sysex_frame(frame_buf, frame_len);
                frame_len = -1;
            }
            else if (b >= 0x80 && b < 0xF8) {
                // non-realtime status byte aborts the frame
                frame_len = -1;
                span_start = i;
            }
            else if (b < 0x80 && frame_len < VCV_SYSEX_MAX) {
                frame_buf[frame_len++] = b;
            }
            continue;
        }
        if (b == 0xF0) {
            if (span_start >= 0) {
                convert_midi_bytes_to_messages((uint8_t *)data + span_start, i - span_start, 0);
                span_start = -1;
            }
            frame_len = 0;
            frame_buf[frame_len++] = b;
        }
        else if (span_start < 0) {
            span_start = i;
        }
    }
    if (span_start >= 0 && frame_len < 0)
        convert_midi_bytes_to_messages((uint8_t *)data + span_start, len - span_start, 0);
}

// ───────────────────────────── macOS: CoreMIDI ─────────────────────────────
#ifdef __APPLE__

#include <CoreMIDI/CoreMIDI.h>

static MIDIClientRef vcv_midi_client = 0;
static MIDIEndpointRef vcv_virt_src = 0;   // "AMYboard VCV": input from the browser's view
static MIDIEndpointRef vcv_virt_dst = 0;   // ...and the output it sends control frames to
static pthread_mutex_t vcv_midi_out_lock = PTHREAD_MUTEX_INITIALIZER;

// CoreMIDI read proc (CoreMIDI's own thread).
static void vcv_midi_read_proc(const MIDIPacketList *pktlist, void *refCon, void *srcConnRefCon) {
    (void)refCon; (void)srcConnRefCon;
    const MIDIPacket *pkt = &pktlist->packet[0];
    for (UInt32 i = 0; i < pktlist->numPackets; i++) {
        if (pkt->length)
            handle_bytes(pkt->data, pkt->length);
        pkt = MIDIPacketNext(pkt);
    }
}

int amyboard_vcv_midi_start(void) {
    if (vcv_midi_client)
        return 0;
    if (MIDIClientCreate(CFSTR("AMYboard VCV"), NULL, NULL, &vcv_midi_client) != noErr) {
        fprintf(stderr, "AMYboard VCV: MIDIClientCreate failed\n");
        return -1;
    }
    if (MIDISourceCreate(vcv_midi_client, CFSTR("AMYboard VCV"), &vcv_virt_src) != noErr)
        fprintf(stderr, "AMYboard VCV: MIDISourceCreate failed\n");
    if (MIDIDestinationCreate(vcv_midi_client, CFSTR("AMYboard VCV"),
                              vcv_midi_read_proc, NULL, &vcv_virt_dst) != noErr)
        fprintf(stderr, "AMYboard VCV: MIDIDestinationCreate failed\n");
    return 0;
}

void amyboard_vcv_midi_stop(void) {
    if (vcv_virt_dst) { MIDIEndpointDispose(vcv_virt_dst); vcv_virt_dst = 0; }
    if (vcv_virt_src) { MIDIEndpointDispose(vcv_virt_src); vcv_virt_src = 0; }
    if (vcv_midi_client) { MIDIClientDispose(vcv_midi_client); vcv_midi_client = 0; }
}

// AMY's MIDI OUT: zI OK replies, zT AKs (mp_embed.c), zD dumps (transfer.c),
// Python tulip.midi_out(). May be called from the audio thread, the MIDI
// thread, or the MP thread — hence the lock.
void midi_out(uint8_t *bytes, uint16_t len) {
    if (!vcv_virt_src || !len)
        return;
    pthread_mutex_lock(&vcv_midi_out_lock);
    // Big enough for a full zD dump frame in one packet list.
    static uint8_t plbuf[65536];
    MIDIPacketList *pl = (MIDIPacketList *)plbuf;
    MIDIPacket *p = MIDIPacketListInit(pl);
    uint16_t off = 0;
    while (off < len && p) {
        uint16_t n = (uint16_t)(len - off > 256 ? 256 : len - off);
        p = MIDIPacketListAdd(pl, sizeof(plbuf), p, 0, n, bytes + off);
        off += n;
    }
    MIDIReceived(vcv_virt_src, pl);
    pthread_mutex_unlock(&vcv_midi_out_lock);
    vcv_midi_out_enqueue(bytes, len);
}

// ─────────────────────────── Linux: ALSA sequencer ─────────────────────────
#elif defined(__linux__)

#include <alsa/asoundlib.h>
#include <poll.h>
#include <stdlib.h>

static snd_seq_t *vcv_seq = NULL;
static int vcv_seq_port = -1;
static pthread_t vcv_seq_thread;
static volatile int vcv_seq_running = 0;
static snd_midi_event_t *vcv_seq_dec = NULL;  // seq event -> raw bytes (input)
static snd_midi_event_t *vcv_seq_enc = NULL;  // raw bytes -> seq event (short msgs out)
static pthread_mutex_t vcv_midi_out_lock = PTHREAD_MUTEX_INITIALIZER;

static void *vcv_seq_thread_body(void *arg) {
    (void)arg;
    int npfd = snd_seq_poll_descriptors_count(vcv_seq, POLLIN);
    struct pollfd *pfd = calloc(npfd, sizeof(struct pollfd));
    snd_seq_poll_descriptors(vcv_seq, pfd, npfd, POLLIN);
    uint8_t buf[1024];
    while (vcv_seq_running) {
        if (poll(pfd, npfd, 200) <= 0)
            continue;
        snd_seq_event_t *ev = NULL;
        while (vcv_seq_running && snd_seq_event_input_pending(vcv_seq, 1) > 0) {
            if (snd_seq_event_input(vcv_seq, &ev) < 0 || !ev)
                break;
            if (ev->type == SND_SEQ_EVENT_SYSEX) {
                // A chunk of a sysex message (first starts with F0, last ends
                // with F7); handle_bytes accumulates across chunks.
                handle_bytes((const uint8_t *)ev->data.ext.ptr, (int)ev->data.ext.len);
            }
            else {
                long n = snd_midi_event_decode(vcv_seq_dec, buf, sizeof(buf), ev);
                if (n > 0)
                    handle_bytes(buf, (int)n);
                snd_midi_event_reset_decode(vcv_seq_dec);
            }
        }
    }
    free(pfd);
    return NULL;
}

int amyboard_vcv_midi_start(void) {
    if (vcv_seq)
        return 0;
    if (snd_seq_open(&vcv_seq, "default", SND_SEQ_OPEN_DUPLEX, 0) < 0) {
        fprintf(stderr, "AMYboard VCV: snd_seq_open failed\n");
        vcv_seq = NULL;
        return -1;
    }
    snd_seq_set_client_name(vcv_seq, "AMYboard VCV");
    vcv_seq_port = snd_seq_create_simple_port(vcv_seq, "AMYboard VCV",
        SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ |
        SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE,
        SND_SEQ_PORT_TYPE_MIDI_GENERIC | SND_SEQ_PORT_TYPE_APPLICATION);
    if (vcv_seq_port < 0)
        fprintf(stderr, "AMYboard VCV: create_simple_port failed\n");
    snd_midi_event_new(1024, &vcv_seq_dec);
    snd_midi_event_no_status(vcv_seq_dec, 1);
    snd_midi_event_new(1024, &vcv_seq_enc);
    vcv_seq_running = 1;
    if (pthread_create(&vcv_seq_thread, NULL, vcv_seq_thread_body, NULL)) {
        vcv_seq_running = 0;
        fprintf(stderr, "AMYboard VCV: seq thread create failed\n");
    }
    return 0;
}

void amyboard_vcv_midi_stop(void) {
    if (!vcv_seq)
        return;
    vcv_seq_running = 0;
    pthread_join(vcv_seq_thread, NULL);
    if (vcv_seq_dec) { snd_midi_event_free(vcv_seq_dec); vcv_seq_dec = NULL; }
    if (vcv_seq_enc) { snd_midi_event_free(vcv_seq_enc); vcv_seq_enc = NULL; }
    if (vcv_seq_port >= 0) { snd_seq_delete_simple_port(vcv_seq, vcv_seq_port); vcv_seq_port = -1; }
    snd_seq_close(vcv_seq);
    vcv_seq = NULL;
}

void midi_out(uint8_t *bytes, uint16_t len) {
    if (!vcv_seq || !len)
        return;
    pthread_mutex_lock(&vcv_midi_out_lock);
    snd_seq_event_t ev;
    if (bytes[0] == 0xF0) {
        snd_seq_ev_clear(&ev);
        snd_seq_ev_set_source(&ev, vcv_seq_port);
        snd_seq_ev_set_subs(&ev);
        snd_seq_ev_set_direct(&ev);
        snd_seq_ev_set_sysex(&ev, len, bytes);
        snd_seq_event_output(vcv_seq, &ev);
        snd_seq_drain_output(vcv_seq);
    }
    else if (vcv_seq_enc) {
        // Short channel/system messages (tulip.midi_out from sketches).
        snd_midi_event_reset_encode(vcv_seq_enc);
        for (uint16_t i = 0; i < len; i++) {
            snd_seq_ev_clear(&ev);
            if (snd_midi_event_encode_byte(vcv_seq_enc, bytes[i], &ev) == 1) {
                snd_seq_ev_set_source(&ev, vcv_seq_port);
                snd_seq_ev_set_subs(&ev);
                snd_seq_ev_set_direct(&ev);
                snd_seq_event_output(vcv_seq, &ev);
            }
        }
        snd_seq_drain_output(vcv_seq);
    }
    pthread_mutex_unlock(&vcv_midi_out_lock);
    vcv_midi_out_enqueue(bytes, len);
}

// ─────────────────── other platforms (Windows): no virtual port ────────────
#else

// No OS-level virtual MIDI here: the module's Rack MIDI menus carry the
// editor traffic instead (user creates a loopMIDI pair; Chrome/amyboard.com
// takes one side, the module the other). All outbound frames go through
// the Rack midi::Output drain.
int amyboard_vcv_midi_start(void) { return -1; }
void amyboard_vcv_midi_stop(void) {}
void midi_out(uint8_t *bytes, uint16_t len) { vcv_midi_out_enqueue(bytes, len); }

#endif
