// amy_connector.c
// all the stuff that connects Tulip to AMY
// like MIDI queue -> python
// like external CV
// like t-sequencer?
// like alles / wifi stuff?


#include "polyfills.h"
#include "py/mphal.h"
#include "py/runtime.h"

uint8_t * external_map;


// A queue to store the AMY midi messages coming IN
uint8_t last_midi[MIDI_QUEUE_DEPTH][MAX_MIDI_BYTES_PER_MESSAGE];
uint8_t last_midi_len[MIDI_QUEUE_DEPTH];
extern mp_obj_t midi_callback;

int16_t midi_queue_head = 0;
int16_t midi_queue_tail = 0;


void midi_local(uint8_t * bytes, uint16_t len) {
    // send these bytes to AMY
}


void amy_midi_message_received(uint8_t *data, size_t len) {
    //fprintf(stderr, "adding midi message of %d bytes to queue: ", len);
    for(uint32_t i = 0; i < (uint32_t)len; i++) {
        if(i < MAX_MIDI_BYTES_PER_MESSAGE) {
            //fprintf(stderr, "%02x ", data[i]);
            last_midi[midi_queue_tail][i] = data[i];
        }
    }
    //fprintf(stderr, "\n");
    last_midi_len[midi_queue_tail] = (uint16_t)len;
    midi_queue_tail = (midi_queue_tail + 1) % MIDI_QUEUE_DEPTH;
    if (midi_queue_tail == midi_queue_head) {
        // Queue wrap, drop oldest item.
        midi_queue_head = (midi_queue_head + 1) % MIDI_QUEUE_DEPTH;
        //fprintf(stderr, "dropped midi message\n");
    }

    // We tell Python that a MIDI message has been received
    if(midi_callback!=NULL) mp_sched_schedule(midi_callback, mp_const_false);
}




// Parse extra stuff out of an AMY message. Used for alles

void tulip_parse_amy_message(char *message, uint16_t length) {
    uint8_t mode = 0;
    int16_t client = -1;
    int32_t sync = -1;
    int8_t sync_index = -1;
    uint8_t ipv4 = 0;
    uint16_t start = 0;
    uint16_t c = 0;
    uint8_t sync_response = 0;

    // Parse the AMY stuff out of the message first
    struct event e = amy_parse_message(message);
    if(e.status == EVENT_TRANSFER_DATA) {
        // transfer data already dealt with. we skip this followon check.
        length = 0;
    } else {
        //fprintf(stderr, "message is %s len is %d\n", message, length);
        // Then pull out any alles-specific modes in this message 
        while(c < length+1) {
            uint8_t b = message[c];
            if(b == '_' && c==0) sync_response = 1;
            if( ((b >= 'a' && b <= 'z') || (b >= 'A' && b <= 'Z')) || b == 0) {  // new mode or end
                if(mode=='g') client = atoi(message + start); 
                if(mode=='U') sync = atol(message + start); 
                if(mode=='W') external_map[e.osc] = atoi(message+start);
                if(sync_response) if(mode=='r') ipv4=atoi(message + start);
                if(mode=='i') sync_index = atoi(message + start);
                mode = b;
                start = c + 1;
            } 
            c++;
        }
    }
    if(sync_response) {
        // If this is a sync response, let's update our local map of who is booted
        //fprintf(stderr, "sync response message was %s\n", message);
        //update_map(client, ipv4, sync);
        length = 0; // don't need to do the rest
    } else {
        // Note, we DO NOT do anything with computed_delta in Tulip. Tulip cannot receive alles mesh messages for playback.
    }
    // Only do this if we got some data
    if(length >0) {
        // TODO -- not that it matters, but the below could probably be one or two lines long instead
        // Don't add sync messages to the event queue
        if(sync >= 0 && sync_index >= 0) {
            //handle_sync(sync, sync_index);
        } else {
            // Don't parse events other than sync messages if i'm in mesh mode. 
            //if(!mesh_flag) {
                amy_add_event(e);
            //}
        }
    }
}
