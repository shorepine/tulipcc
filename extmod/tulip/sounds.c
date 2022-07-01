// sounds.c
// various little "make a sound in firmware" methods
#include "alles_tulip.h"

void note_on(int8_t osc, int64_t time) {
    struct event e = default_event();
    e.osc = osc;
    e.time = time;
    e.velocity = 1;
    add_event(e);
}


// Play a pew -- upgdrading

void upgrade_tone() {
    struct event e = default_event();
    int64_t sysclock = get_sysclock();
    e.osc = 0;
    e.time = sysclock;
    e.wave = SINE;
    e.freq = 220;
    parse_breakpoint(&e, "0,0,10,1,500,0,0,0",0);
    e.breakpoint_target[0] = TARGET_AMP;
    e.breakpoint_target[1] = TARGET_FREQ;
    add_event(e);
    e.osc = 1;
    e.freq = 420;
    add_event(e);

    note_on(0, e.time+1);
    note_on(1, e.time+1);
}

// Play a sonar ping -- searching for wifi
void wifi_tone() {
    struct event e = default_event();
    int64_t sysclock = get_sysclock();
    e.osc = 0;
    e.time = sysclock;
    e.wave = SINE;
    e.freq = 440;
    parse_breakpoint(&e, "0,0,10,1,500,0,0,0",0);
    e.breakpoint_target[0] = TARGET_AMP;
    add_event(e);
    e.osc = 1;
    e.freq = 840;
    add_event(e);

    note_on(0, e.time+1);
    note_on(1, e.time+1);
}

// Schedule a bleep now
void bleep() {
    struct event e = default_event();
    int64_t sysclock = get_sysclock();
    e.time = sysclock;
    e.wave = SINE;
    e.freq = 220;
    add_event(e);
    e.velocity = 1;
    add_event(e);
    e.time = sysclock + 150;
    e.freq = 440;
    add_event(e);
    e.time = sysclock + 300;
    e.velocity = 0;
    e.amp = 0;
    e.freq = 0;
    add_event(e);
}

void debleep() {
    struct event e = default_event();
    int64_t sysclock = get_sysclock();
    e.time = sysclock;
    e.wave = SINE;
    e.freq = 440;
    e.velocity = 1;
    add_event(e);
    e.time = sysclock + 150;
    e.freq = 220;
    add_event(e);
    e.time = sysclock + 300;
    e.velocity = 0;
    e.freq = 0;
    add_event(e);
}


// Plays a short scale 
void scale(uint8_t wave) {
    struct event e = default_event();
    int64_t sysclock = get_sysclock();
    for(uint8_t i=0;i<12;i++) {
        e.time = sysclock + (i*250);
        e.wave = wave;
        e.midi_note = 48+i;
        e.velocity = 1;
        add_event(e);
    }
}
