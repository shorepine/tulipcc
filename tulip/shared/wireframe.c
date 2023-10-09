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


mp_obj_t load_obj_file_into_ram(const char *fn) {
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
    uint32_t total_entries = 2 + v_count * 3 + f_count * 3;
    //fprintf(stderr, "f_count %d v_count %d max %f ram is %d\n", f_count, v_count, max_position, total_entries);

    // Header
    uint16_t ram[total_entries];
    uint32_t offset = 0;
    ram[offset++] = v_count;
    ram[offset++] = f_count;

    fp = tulip_fopen(fn, "r");

    while((n_read = tulip_getline(line, &len, fp)) != 0){
        if(line[0] == '#') continue;
        if(sscanf(line, "v %f %f %f", &x, &y, &z) == 3){
            // convert points into uint16ts 0-65536 with max
            uint16_t ux,uy,uz;
            ux = (uint16_t) (((x/max_position)*32767.0) + 32767);
            uy = (uint16_t) (((y/max_position)*32767.0) + 32767);
            uz = (uint16_t) (((z/max_position)*32767.0) + 32767);
            ram[offset++] = ux;
            ram[offset++] = uy;
            ram[offset++] = uz;
        }

        if(sscanf(line, "f %d %d %d", &a, &b, &c) == 3){
            a = a - 1;
            b = b - 1;
            c = c - 1;
            ram[offset++] = a;
            ram[offset++] = b;
            ram[offset++] = c;
        }
        for(uint8_t j=0;j<255;j++) line[j] = 0;
    }
    free_caps(line);
    tulip_fclose(fp);
    return mp_obj_new_bytes((uint8_t*)ram, total_entries*2);
}

int compare_line_y0(const void *a, const void *b) {
    uint16_t y0a = ((uint16_t *) a)[1];
    uint16_t y0b = ((uint16_t *) b)[1];
    if (y0a < y0b) return -1;
    else if (y0a > y0b) return 1;
    return 0;
}

void project_draw(uint16_t x0, uint16_t y0, uint16_t z0,  uint16_t x1, uint16_t y1, uint16_t z1, float fa,float fb,float fc, float fd, 
                    uint16_t x, uint16_t y, float scale, uint16_t*lines, uint32_t w_offset , uint8_t color) {

    float fx, fy, fz;
    
    fx = (x0 - 32767.0) / 32767.0;
    fy = (y0 - 32767.0) / 32767.0;
    fz = (z0 - 32767.0) / 32767.0;
    int16_t draw_x0 = (uint16_t)((fa*fx + fb*fz)*scale) + x;
    int16_t draw_y0 = (uint16_t)((fc*fy + fd*fz)*scale) + y;

    fx = (x1 - 32767.0) / 32767.0;
    fy = (y1 - 32767.0) / 32767.0;
    fz = (z1 - 32767.0) / 32767.0;
    int16_t draw_x1 = (uint16_t)((fa*fx + fb*fz)*scale) + x;
    int16_t draw_y1 = (uint16_t)((fc*fy + fd*fz)*scale) + y;
    
    if(draw_x0 > H_RES) draw_x0 = H_RES;
    if(draw_x0 < 0) draw_x0 = 0;
    if(draw_x1 > H_RES) draw_x1 = H_RES;
    if(draw_x1 < 0) draw_x1 = 0;

    if(draw_y0 > V_RES) draw_y0 = V_RES;
    if(draw_y0 < 0) draw_y0 = 0;
    if(draw_y1 > V_RES) draw_y1 = V_RES;
    if(draw_y1 < 0) draw_y1 = 0;


    // Ensure that y0 < y1
    if(draw_y0 > draw_y1) {
        draw_x1 = draw_x1 | ((color & 0xF0) << 8);
        draw_x0 = draw_x0 | ((color & 0x0F) << 12);
        lines[w_offset+0] = (uint16_t)draw_x1;
        lines[w_offset+1] = (uint16_t)draw_y1;
        lines[w_offset+2] = (uint16_t)draw_x0;
        lines[w_offset+3] = (uint16_t)draw_y0;
    } else {
        draw_x0 = draw_x0 | ((color & 0xF0) << 8);
        draw_x1 = draw_x1 | ((color & 0x0F) << 12);
        lines[w_offset+0] = (uint16_t)draw_x0;
        lines[w_offset+1] = (uint16_t)draw_y0;
        lines[w_offset+2] = (uint16_t)draw_x1;
        lines[w_offset+3] = (uint16_t)draw_y1;
    }
}


// TODO , go back to casting this as uint16

mp_obj_t render_wire_to_lines(uint8_t *buf, uint16_t x, uint16_t y, uint16_t scale, uint16_t theta, uint8_t color){
    float theta_f = (float)theta * (M_PI/100.0);
    float scale_f = (float)scale;
    float fa,fb,fc,fd;
    fa = sinf(theta_f);
    fb = cosf(theta_f);
    fc = cosf(theta_f);
    fd = -sinf(theta_f);

    // this is now the drawing INTO offset
    uint32_t w_offset = 0;
    uint16_t *buf0 = (uint16_t*) buf;

    uint16_t v_count = buf0[0];
    uint16_t f_count = buf0[1];

    uint32_t f_offset = 2 + (v_count*3);

    uint32_t total_lines = f_count * 3 + 1; // one extra for end
    uint16_t lines[total_lines * 4];

    for(uint16_t f=0;f<f_count;f++) {
        uint16_t a = buf0[f_offset + f*3 + 0]; 
        uint16_t b = buf0[f_offset + f*3 + 1]; 
        uint16_t c = buf0[f_offset + f*3 + 2]; 

        uint32_t v_a_offset = 2 + (a*3);
        uint16_t ax = buf0[v_a_offset+0];
        uint16_t ay = buf0[v_a_offset+1]; 
        uint16_t az = buf0[v_a_offset+2]; 

        uint32_t v_b_offset = 2 + (b*3);
        uint16_t bx = buf0[v_b_offset + 0]; 
        uint16_t by = buf0[v_b_offset + 1]; 
        uint16_t bz = buf0[v_b_offset + 2]; 

        uint32_t v_c_offset = 2 + (c*3);
        uint16_t cx = buf0[v_c_offset+0]; 
        uint16_t cy = buf0[v_c_offset+1];
        uint16_t cz = buf0[v_c_offset+2];

        // drawline between a->b, b->c, c->a
        project_draw(ax,ay,az,bx,by,bz,fa,fb,fc,fd,x,y,scale_f, lines, w_offset, color);
        project_draw(bx,by,bz,cx,cy,cz,fa,fb,fc,fd,x,y,scale_f, lines, w_offset+4, color);
        project_draw(cx,cy,cz,ax,ay,az,fa,fb,fc,fd,x,y,scale_f, lines, w_offset+8, color);
        w_offset += 12;
    }
    // Last line has to be all 0xffff 
    lines[w_offset+0] = 65535;
    lines[w_offset+1] = 65535;
    lines[w_offset+2] = 65535;
    lines[w_offset+3] = 65535;

    // Gotta sort the lines
    qsort(lines, total_lines, 8, compare_line_y0);
    //for(uint16_t i=0;i<total_lines;i++) {
    //    fprintf(stderr, "line %d: %d %d -> %d %d\n", i, lines[i*4 + 0], lines[i*4 + 1], lines[i*4 + 2], lines[i*4 + 3]);
    //}
    return mp_obj_new_bytes((uint8_t*)lines, total_lines * 2 * 4);
}



