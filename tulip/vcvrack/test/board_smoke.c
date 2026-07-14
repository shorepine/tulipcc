// Phase-2 offline smoke test: boots the full AMYboard firmware (MicroPython +
// AMY in-process) exactly the way the Rack module does, renders audio on this
// thread (standing in for Rack's audio thread), and checks the seams.
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#include "amy.h"

void run_amy(void);
void tsequencer_init(void);
int amyboard_vcv_mp_start(void);
void amyboard_vcv_mp_stop(void);
int amyboard_vcv_mp_booted(void);
void amyboard_vcv_exec(const char *code);
extern float amyboard_vcv_cv_in[2];
extern float amyboard_vcv_cv_out[2];
extern int32_t amyboard_vcv_encoder_pos[4];
extern volatile uint32_t amyboard_vcv_framebuf_gen;

static void render_seconds(const char *label, float secs) {
    long blocks = (long)(secs * AMY_SAMPLE_RATE / AMY_BLOCK_SIZE);
    double energy = 0;
    int16_t peak = 0;
    for (long b = 0; b < blocks; b++) {
        int16_t *buf = amy_simple_fill_buffer();
        for (int i = 0; i < AMY_BLOCK_SIZE * AMY_NCHANS; i++) {
            int16_t s = buf[i];
            energy += (double)s * s;
            if (s > peak) peak = s;
            if (-s > peak) peak = (int16_t)-s;
        }
        // real-time-ish pacing so the MP thread gets scheduled like in Rack
        usleep(1000000L * AMY_BLOCK_SIZE / AMY_SAMPLE_RATE / 2);
    }
    double rms = sqrt(energy / (blocks * AMY_BLOCK_SIZE * AMY_NCHANS));
    printf("== %-24s peak=%6d rms=%8.1f %s\n", label, peak, rms,
           rms > 10.0 ? "AUDIBLE" : "(silent)");
    fflush(stdout);
}

int main(void) {
    printf("== starting AMY + firmware\n");
    run_amy();
    tsequencer_init();
    amyboard_vcv_mp_start();

    // Render while the firmware boots (sequencer time only advances as we render).
    int waited = 0;
    while (!amyboard_vcv_mp_booted() && waited < 30000) {
        amy_simple_fill_buffer();
        usleep(2900);
        waited += 3;
    }
    printf("== booted after ~%d ms of rendering\n", waited);

    render_seconds("boot + default sketch", 2.0f);

    uint8_t note_on[3] = {0x90, 60, 100};
    amy_event_midi_message_received(note_on, 3, 0, amy_sysclock());
    render_seconds("juno note on ch1", 1.0f);
    uint8_t note_off[3] = {0x80, 60, 0};
    amy_event_midi_message_received(note_off, 3, 0, amy_sysclock());

    // CV in visible from Python?
    amyboard_vcv_cv_in[0] = 3.25f;
    amyboard_vcv_exec("import amyboard; print('PY cv_in ->', amyboard.cv_in(0))");
    // CV out from Python?
    amyboard_vcv_exec("amyboard.cv_out(-4.5, 1)");
    // Encoder seam
    amyboard_vcv_encoder_pos[0] = 7;
    amyboard_vcv_exec("print('PY encoder ->', amyboard.read_encoder(0))");
    // Display seam: draw + show, then we check the framebuf generation
    amyboard_vcv_exec("amyboard.display.text('hello rack', 0, 0); amyboard.display.show()");
    render_seconds("after exec batch", 1.0f);

    printf("== cv_out[1] = %f (expect -4.5)\n", amyboard_vcv_cv_out[1]);
    printf("== framebuf generation = %u (expect > 0)\n", (unsigned)amyboard_vcv_framebuf_gen);

    amyboard_vcv_exec("import amyboard; amyboard.restart_sketch()");
    render_seconds("after restart_sketch", 1.5f);

    printf("== stopping\n");
    amyboard_vcv_mp_stop();
    amy_stop();
    printf("== clean exit\n");
    return 0;
}
