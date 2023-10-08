// wireframe.c
// per scanline wireframe mesh drawing tools, loading OBJ files
// heavily borrowed from https://github.com/ryomuk/gu3000/blob/main/src/examples/showwire/showwire.cpp

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bresenham.h"
#include "tulip_helpers.h"
#include <ctype.h>

// Wire frame models are available
// from https://people.sc.fsu.edu/~jburkardt/data/obj/obj.html

// make a struct for lines that's LL and sorted on insert like our event queues

// you really want to put this queue in sprite ram 
// and you don't need a queue! The wire drawing can do the sort ! 
// so yes, like wire = tulip.load_wire("teapot.obj") --> parses obj into bytes for python
// tulip.wire_register(wire, 0, scale, theta) --> sorts and scales and writes line points into sprite ram, with existing header
// 
// 

#define LINE_STORAGE_FIFO_LEN 1000
uint16_t line_qsize = 0;

// Delta holds the individual changes from an event, it's sorted in order of playback time 
// this is more efficient in memory than storing entire events per message 
struct line_storage {
    uint16_t x0; 
    uint16_t y0;
    uint16_t x1; 
    uint16_t y1;
    // color can be part of them maybe on the unused bits?
    struct line_storage * next; // the next event, in time 
};

struct line_storage * line_head;
// like

line_head = // use sprite_ram, right ? or ... (struct line_storage*) malloc(sizeof(struct line_storage));
line_head.x0 = 65535;
line_head.y0 = 65535;
line_head.x1 = 65535;
line_head.y1 = 65535;
line_head.next = NULL;


void add_line_to_queue(struct line_storage l) {
    if(line_qsize < LINE_STORAGE_FIFO_LEN) {
        int16_t found = -1;
        while(found<0) {

            if(events[write_location].time == UINT32_MAX) found = write_location;
            write_location = (write_location + 1) % AMY_EVENT_FIFO_LEN;
        }
        // found a mem location. copy the data in and update the write pointers.
        events[found].time = d.time;
        events[found].osc = d.osc;
        events[found].param = d.param;
        events[found].data = d.data;
        global.next_event_write = write_location;
        global.event_qsize++;

        // now insert it into the sorted list for fast playback
        // first, see if it's eariler than the first item, special case
        if(d.time < global.event_start->time) {
            events[found].next = global.event_start;
            global.event_start = &events[found];
        } else {
            // or it's got to be found somewhere
            struct delta* ptr = global.event_start; 
            int8_t inserted = -1;
            while(inserted<0) {
                if(d.time < ptr->next->time) { 
                    // next should point to me, and my next should point to old next
                    events[found].next = ptr->next;
                    ptr->next = &events[found];
                    inserted = 1;
                }
                ptr = ptr->next;
            }
        }
        event_counter++;

    } else {
        // if there's no room in the queue, just skip the message
        // todo -- report this somehow? 
    }
#ifdef ESP_PLATFORM
    xSemaphoreGive( xQueueSemaphore );
#endif
}



// we pack points in u16s
uint8_t u0(uint16_t a) { return (uint8_t)(a & 0x00FF); }
uint8_t u1(uint16_t a) { return (uint8_t)((a & 0xFF00) >> 8); }
uint16_t u16fromu8(uint8_t u0, uint8_t u1) { 
    uint16_t ret = 0;
    ret = ret | (u1 << 8);
    ret = ret | u0;
    return ret;
}

uint32_t load_obj_file_into_sprite_ram(const char *fn, uint32_t ram_start) {
    uint32_t offset = ram_start;
    mp_obj_t fp;

    char * line;
    uint32_t len;
    int n_read;
    
    float x, y, z;
    int a,b,c;
    fp = tulip_fopen(fn, "r");
    float max_position = 0;

    uint16_t v_count = 0;
    uint16_t f_count = 0;

    line = malloc_caps(255, MALLOC_CAP_SPIRAM);

    // go through everything once, count how many vs and fs , get max, etc
    while((n_read = tulip_getline(line, &len, fp)) != 0){
        if(line[0] == '#') continue;
        if(sscanf(line, "v %f %f %f", &x, &y, &z) == 3){
            v_count++;
            max_position=fmax(max_position, fmax(fmax(fabsf(x), fabsf(y)), fabsf(z)));
        }
        if(sscanf(line, "f %d %d %d", &a, &b, &c) == 3){
            f_count++;
        }
        for(uint8_t j=0;j<255;j++) line[j] = 0;
    }
    tulip_fclose(fp);
    fprintf(stderr, "f_count %d v_count %d max %f\n", f_count, v_count, max_position);

    // Header
    sprite_ram[offset++] = u0(v_count);
    sprite_ram[offset++] = u1(v_count);
    sprite_ram[offset++] = u0(f_count);
    sprite_ram[offset++] = u1(f_count);

    fp = tulip_fopen(fn, "r");

    while((n_read = tulip_getline(line, &len, fp)) != 0){
        if(line[0] == '#') continue;
        if(sscanf(line, "v %f %f %f", &x, &y, &z) == 3){
            // convert points into uint16ts 0-65536 with max
            uint16_t ux,uy,uz;
            ux = (uint16_t) (((x/max_position)*32767.0) + 32767);
            uy = (uint16_t) (((y/max_position)*32767.0) + 32767);
            uz = (uint16_t) (((z/max_position)*32767.0) + 32767);
            sprite_ram[offset++] = u0(ux);
            sprite_ram[offset++] = u1(ux);
            sprite_ram[offset++] = u0(uy);
            sprite_ram[offset++] = u1(uy);
            sprite_ram[offset++] = u0(uz);
            sprite_ram[offset++] = u1(uz);
        }

        if(sscanf(line, "f %d %d %d", &a, &b, &c) == 3){
            a = a - 1;
            b = b - 1;
            c = c - 1;
            sprite_ram[offset++] = u0(a);
            sprite_ram[offset++] = u1(a);
            sprite_ram[offset++] = u0(b);
            sprite_ram[offset++] = u1(b);
            sprite_ram[offset++] = u0(c);
            sprite_ram[offset++] = u1(c);
        }
        for(uint8_t j=0;j<255;j++) line[j] = 0;
    }
    free_caps(line);
    tulip_fclose(fp);
    return offset-ram_start;
}



void project_draw(uint16_t x0, uint16_t y0, uint16_t z0,  uint16_t x1, uint16_t y1, uint16_t z1, float fa,float fb,float fc, float fd, 
                    uint16_t x, uint16_t y, float scale, uint16_t color) {

  float fx, fy, fz;
  
  fx = (x0 - 32767.0) / 32767.0;
  fy = (y0 - 32767.0) / 32767.0;
  fz = (z0 - 32767.0) / 32767.0;
  uint16_t draw_x0 = (uint16_t)((fa*fx + fb*fz)*scale) + x;
  uint16_t draw_y0 = (uint16_t)((fc*fy + fd*fz)*scale) + y;

  fx = (x1 - 32767.0) / 32767.0;
  fy = (y1 - 32767.0) / 32767.0;
  fz = (z1 - 32767.0) / 32767.0;
  uint16_t draw_x1 = (uint16_t)((fa*fx + fb*fz)*scale) + x;
  uint16_t draw_y1 = (uint16_t)((fc*fy + fd*fz)*scale) + y;

  drawLine(draw_x0,draw_y0,draw_x1,draw_y1,color);
}

void draw_sprite_wire(uint16_t sprite_no){
    float theta = (float)sprite_h_px[sprite_no] * (M_PI/100.0);
    float scale = (float)sprite_w_px[sprite_no];
    uint16_t color = 1024; 
    uint16_t tx = sprite_x_px[sprite_no];
    uint16_t ty = sprite_y_px[sprite_no];
    float fa,fb,fc,fd;
    fa = sinf(theta);
    fb = cosf(theta);
    fc = cosf(theta);
    fd = -sinf(theta);

    uint32_t offset = sprite_mem[sprite_no];

    uint16_t v_count = u16fromu8(sprite_ram[offset], sprite_ram[offset+1]);
    offset += 2;
    uint16_t f_count = u16fromu8(sprite_ram[offset], sprite_ram[offset+1]);
    offset += 2;

    // clear line buffer before drawing
    for(uint32_t i=0;i<V_RES*LINE_BUFFERS_PER_ROW;i++) line_buffer[i] = 65535;

    uint32_t f_offset = offset + (v_count*6);
    for(uint16_t f=0;f<f_count;f++) {
        uint16_t a = u16fromu8 (sprite_ram[f_offset + (f*6) + 0], sprite_ram[f_offset + (f*6) + 1]);
        uint16_t b = u16fromu8 (sprite_ram[f_offset + (f*6) + 2], sprite_ram[f_offset + (f*6) + 3]);
        uint16_t c = u16fromu8 (sprite_ram[f_offset + (f*6) + 4], sprite_ram[f_offset + (f*6) + 5]);

        uint32_t v_a_offset = sprite_mem[sprite_no] + 2 + (a*6);
        uint16_t ax = u16fromu8(sprite_ram[v_a_offset+0], sprite_ram[v_a_offset+1]);
        uint16_t ay = u16fromu8(sprite_ram[v_a_offset+2], sprite_ram[v_a_offset+3]);
        uint16_t az = u16fromu8(sprite_ram[v_a_offset+4], sprite_ram[v_a_offset+5]);

        uint32_t v_b_offset = sprite_mem[sprite_no] + 2 + (b*6);
        uint16_t bx = u16fromu8(sprite_ram[v_b_offset+0], sprite_ram[v_b_offset+1]);
        uint16_t by = u16fromu8(sprite_ram[v_b_offset+2], sprite_ram[v_b_offset+3]);
        uint16_t bz = u16fromu8(sprite_ram[v_b_offset+4], sprite_ram[v_b_offset+5]);

        uint32_t v_c_offset = sprite_mem[sprite_no] + 2 + (c*6);
        uint16_t cx = u16fromu8(sprite_ram[v_c_offset+0], sprite_ram[v_c_offset+1]);
        uint16_t cy = u16fromu8(sprite_ram[v_c_offset+2], sprite_ram[v_c_offset+3]);
        uint16_t cz = u16fromu8(sprite_ram[v_c_offset+4], sprite_ram[v_c_offset+5]);

        // drawline between a->b, b->c, c->a
        project_draw(ax,ay,az,bx,by,bz,fa,fb,fc,fd,tx,ty,scale,color);
        project_draw(bx,by,bz,cx,cy,cz,fa,fb,fc,fd,tx,ty,scale,color);
        project_draw(cx,cy,cz,ax,ay,az,fa,fb,fc,fd,tx,ty,scale,color);
    }
}
