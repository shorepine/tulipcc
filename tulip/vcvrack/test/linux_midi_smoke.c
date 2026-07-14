// Linux end-to-end smoke: boots the full AMYboard firmware, publishes the
// "AMYboard VCV" ALSA virtual port, then acts as an external client (like
// Chrome WebMIDI would): finds the port, subscribes both directions, pings
// with zI sysex, expects the OK reply, and plays a note-on through the port.
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <alsa/asoundlib.h>

#include "amy.h"

void run_amy(void);
void tsequencer_init(void);
int amyboard_vcv_mp_start(void);
void amyboard_vcv_mp_stop(void);
int amyboard_vcv_mp_booted(void);
int amyboard_vcv_midi_start(void);
void amyboard_vcv_midi_stop(void);

static double render_rms(float secs) {
    long blocks = (long)(secs * AMY_SAMPLE_RATE / AMY_BLOCK_SIZE);
    double e = 0;
    for (long b = 0; b < blocks; b++) {
        int16_t *buf = amy_simple_fill_buffer();
        for (int i = 0; i < AMY_BLOCK_SIZE * AMY_NCHANS; i++)
            e += (double)buf[i] * buf[i];
        usleep(1000L * AMY_BLOCK_SIZE / AMY_SAMPLE_RATE * 1000 / 2);
    }
    return sqrt(e / (blocks * AMY_BLOCK_SIZE * AMY_NCHANS));
}

int main(void) {
    run_amy();
    tsequencer_init();
    amyboard_vcv_mp_start();
    amyboard_vcv_midi_start();

    int waited = 0;
    while (!amyboard_vcv_mp_booted() && waited < 30000) {
        amy_simple_fill_buffer();
        usleep(2900);
        waited += 3;
    }
    printf("== booted (%d ms)\n", waited);
    render_rms(1.0f);  // bleep

    // --- external test client, like the browser ---
    snd_seq_t *cli;
    if (snd_seq_open(&cli, "default", SND_SEQ_OPEN_DUPLEX, 0) < 0) {
        printf("== FAIL: test client snd_seq_open\n");
        return 1;
    }
    snd_seq_set_client_name(cli, "smoke-test");
    int cli_port = snd_seq_create_simple_port(cli, "smoke",
        SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ |
        SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE,
        SND_SEQ_PORT_TYPE_MIDI_GENERIC | SND_SEQ_PORT_TYPE_APPLICATION);

    // find the "AMYboard VCV" client
    int board_client = -1;
    snd_seq_client_info_t *cinfo;
    snd_seq_client_info_alloca(&cinfo);
    snd_seq_client_info_set_client(cinfo, -1);
    while (snd_seq_query_next_client(cli, cinfo) >= 0) {
        if (strcmp(snd_seq_client_info_get_name(cinfo), "AMYboard VCV") == 0) {
            board_client = snd_seq_client_info_get_client(cinfo);
            break;
        }
    }
    printf("== ALSA client 'AMYboard VCV': %s (client %d)\n",
           board_client >= 0 ? "FOUND" : "MISSING", board_client);
    if (board_client < 0) return 1;

    snd_seq_connect_to(cli, cli_port, board_client, 0);    // us -> board
    snd_seq_connect_from(cli, cli_port, board_client, 0);  // board -> us

    // zI ping through the kernel sequencer
    uint8_t ping[] = {0xF0, 0x00, 0x03, 0x45, 'z', 'I', 'Z', 0xF7};
    snd_seq_event_t ev;
    snd_seq_ev_clear(&ev);
    snd_seq_ev_set_source(&ev, cli_port);
    snd_seq_ev_set_subs(&ev);
    snd_seq_ev_set_direct(&ev);
    snd_seq_ev_set_sysex(&ev, sizeof(ping), ping);
    snd_seq_event_output(cli, &ev);
    snd_seq_drain_output(cli);

    // wait for the OK reply while rendering (replies come from the seq thread)
    int got_ok = 0;
    for (int tries = 0; tries < 200 && !got_ok; tries++) {
        amy_simple_fill_buffer();
        usleep(5000);
        snd_seq_event_t *in;
        while (snd_seq_event_input_pending(cli, 1) > 0) {
            if (snd_seq_event_input(cli, &in) < 0 || !in) break;
            if (in->type == SND_SEQ_EVENT_SYSEX) {
                uint8_t *d = (uint8_t *)in->data.ext.ptr;
                unsigned n = in->data.ext.len;
                for (unsigned k = 0; k + 1 < n; k++)
                    if (d[k] == 'O' && d[k + 1] == 'K') got_ok = 1;
            }
        }
    }
    printf("== zI ping over ALSA -> %s\n", got_ok ? "OK reply PASS" : "no reply FAIL");

    // note-on ch1 through the virtual port -> Juno should sound
    snd_seq_ev_clear(&ev);
    snd_seq_ev_set_source(&ev, cli_port);
    snd_seq_ev_set_subs(&ev);
    snd_seq_ev_set_direct(&ev);
    snd_seq_ev_set_noteon(&ev, 0, 60, 100);
    snd_seq_event_output(cli, &ev);
    snd_seq_drain_output(cli);
    usleep(20000);
    double rms = render_rms(1.0f);
    printf("== note-on via ALSA rms=%.1f %s\n", rms, rms > 10.0 ? "AUDIBLE PASS" : "silent FAIL");

    snd_seq_close(cli);
    amyboard_vcv_midi_stop();
    amyboard_vcv_mp_stop();
    amy_stop();
    printf("== overall: %s\n", (got_ok && rms > 10.0) ? "PASS" : "FAIL");
    return 0;
}
