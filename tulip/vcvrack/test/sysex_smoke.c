// Simulates amyboard.com's control mode against the VCV firmware build:
// zT sketch transfer (header + base64 chunks), zP restart, zD dump.
// midi_out is OUR capturing version (vcv_midi.c is not linked here).
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#include "amy.h"

void run_amy(void);
void tsequencer_init(void);
int amyboard_vcv_mp_start(void);
void amyboard_vcv_mp_stop(void);
int amyboard_vcv_mp_booted(void);
void amyboard_vcv_sysex_push(const uint8_t *payload, int len);

// --- captured outbound frames ---
#define MAX_FRAMES 256
static uint8_t out_frames[MAX_FRAMES][20000];
static int out_lens[MAX_FRAMES];
static volatile int out_count = 0;

void midi_out(uint8_t *bytes, uint16_t len) {
    if (out_count < MAX_FRAMES) {
        memcpy(out_frames[out_count], bytes, len);
        out_lens[out_count] = len;
        out_count++;
    }
}

static void render_ms(int ms) {
    long blocks = (long)ms * AMY_SAMPLE_RATE / 1000 / AMY_BLOCK_SIZE + 1;
    for (long b = 0; b < blocks; b++) {
        amy_simple_fill_buffer();
        usleep(1000L * AMY_BLOCK_SIZE / AMY_SAMPLE_RATE * 1000 / 2);
    }
}

// wait for a new outbound frame; return its index or -1
static int wait_frame(int prev_count, int timeout_ms) {
    int waited = 0;
    while (out_count <= prev_count && waited < timeout_ms) {
        render_ms(5);
        waited += 5;
    }
    return out_count > prev_count ? prev_count : -1;
}

static int is_ack(int idx) {
    return out_lens[idx] == 7 && out_frames[idx][4] == 'A' && out_frames[idx][5] == 'K';
}

static const char b64tab[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static int b64enc(const uint8_t *in, int len, char *out) {
    int o = 0;
    for (int i = 0; i < len; i += 3) {
        int n = in[i] << 16 | (i + 1 < len ? in[i + 1] : 0) << 8 | (i + 2 < len ? in[i + 2] : 0);
        out[o++] = b64tab[(n >> 18) & 63];
        out[o++] = b64tab[(n >> 12) & 63];
        out[o++] = i + 1 < len ? b64tab[(n >> 6) & 63] : '=';
        out[o++] = i + 2 < len ? b64tab[n & 63] : '=';
    }
    out[o] = 0;
    return o;
}

static int push_and_expect_ack(const char *payload, const char *what) {
    int prev = out_count;
    amyboard_vcv_sysex_push((const uint8_t *)payload, strlen(payload));
    int idx = wait_frame(prev, 5000);
    if (idx < 0) { printf("== %-28s NO RESPONSE\n", what); return 0; }
    printf("== %-28s -> %s (frame %db)\n", what,
           is_ack(idx) ? "AK" : "unexpected", out_lens[idx]);
    return is_ack(idx);
}

static const char *SKETCH =
    "import amy\n"
    "def loop(step):\n"
    "    pass\n"
    "# VCV_SYSEX_TEST_MARKER\n";

int main(void) {
    run_amy();
    tsequencer_init();
    amyboard_vcv_mp_start();
    int waited = 0;
    while (!amyboard_vcv_mp_booted() && waited < 30000) { render_ms(3); waited += 3; }
    printf("== booted\n");
    render_ms(500);
    out_count = 0;

    // --- zT sketch transfer, exactly like spss.js write_sketch_to_amyboard ---
    int size = (int)strlen(SKETCH);
    char header[128];
    snprintf(header, sizeof(header), "zT/user/current/sketch.py,%dZ", size);
    int ok = push_and_expect_ack(header, "zT header");

    for (int off = 0; off < size && ok; off += 188) {
        int n = size - off > 188 ? 188 : size - off;
        char chunk[300];
        b64enc((const uint8_t *)SKETCH + off, n, chunk);
        ok &= push_and_expect_ack(chunk, "zT b64 chunk");
    }

    ok &= push_and_expect_ack("zPimport amyboard; amyboard.environment_transfer_done()Z", "zP transfer done");
    render_ms(1000);

    // verify the file landed at the VCV root
    char path[512];
    const char *home = getenv("HOME");
    if (!home) home = getenv("USERPROFILE");
    snprintf(path, sizeof(path), "%s/Documents/AMYboard/user/current/sketch.py", home);
    FILE *f = fopen(path, "rb");
    int file_ok = 0;
    if (f) {
        char buf[4096];
        size_t n = fread(buf, 1, sizeof(buf) - 1, f);
        buf[n] = 0;
        fclose(f);
        file_ok = strstr(buf, "VCV_SYSEX_TEST_MARKER") != NULL;
        printf("== sketch.py on disk: %d bytes, marker %s\n", (int)n, file_ok ? "FOUND" : "MISSING");
    } else {
        printf("== sketch.py NOT FOUND at %s\n", path);
    }

    // --- zD state dump: expect one or more 0/C/E frames then the AK ---
    int prev = out_count;
    amyboard_vcv_sysex_push((const uint8_t *)"zDZ", 3);
    render_ms(1500);
    int dump_frames = 0, got_ack = 0;
    for (int i = prev; i < out_count; i++) {
        uint8_t tag = out_frames[i][4];
        if (tag == '0' || tag == 'C' || tag == 'E') dump_frames++;
        if (is_ack(i)) got_ack = 1;
    }
    printf("== zD dump: %d data frame(s), ack=%d\n", dump_frames, got_ack);

    printf("== overall: %s\n", (ok && file_ok && dump_frames > 0) ? "PASS" : "FAIL");
    amyboard_vcv_mp_stop();
    amy_stop();
    printf("== clean exit\n");
    return 0;
}
