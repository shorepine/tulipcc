/* 
 * Tiny PNG Output (C)
 * 
 * Copyright (c) 2017 Project Nayuki
 * https://www.nayuki.io/page/tiny-png-output-c
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program (see COPYING.txt and COPYING.LESSER.txt).
 * If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <stdint.h>
#include <stdio.h>
#include "tulip_helpers.h"

/* 
 * TinyPngOut data structure. Treat this as opaque; do not read or write any fields directly.
 */
struct TinyPngOut {
    
    // Configuration
    int32_t width;   // Measured in bytes, not pixels. A row has (width * 3 + 1) bytes.
    int32_t height;  // Measured in pixels
    
    // State
    mp_obj_t outStream;
    //FILE *outStream;
    int32_t positionX;  // Measured in bytes
    int32_t positionY;  // Measured in pixels
    int32_t deflateRemain;  // Measured in bytes
    int32_t deflateFilled;  // Number of bytes filled in the current block (0 <= n < 65535)
    uint32_t crc;    // For IDAT chunk
    uint32_t adler;  // For DEFLATE data within IDAT
    
};


/* 
 * Enumeration of status codes
 */
enum TinyPngOutStatus {
	TINYPNGOUT_OK,
	TINYPNGOUT_DONE,
	TINYPNGOUT_INVALID_ARGUMENT,
	TINYPNGOUT_IO_ERROR,
	TINYPNGOUT_IMAGE_TOO_LARGE,
};


/* 
 * Initialization function.
 * 
 * Example usage:
 *   #define WIDTH 640
 *   #define HEIGHT 480
 *   FILE *fout = fopen("image.png", "wb");
 *   struct TinyPngOut pngout;
 *   if (fout == NULL || TinyPngOut_init(&pngout, fout, WIDTH, HEIGHT) != TINYPNGOUT_OK) {
 *     ... (handle error) ...
 *   }
 */
enum TinyPngOutStatus TinyPngOut_init(struct TinyPngOut pngout[static 1], mp_obj_t fout[static 1], int32_t width, int32_t height);


/* 
 * Pixel-writing function. The function reads 3*count bytes from the array.
 * Pixels are presented in the array in RGB order, from top to bottom, left to right.
 * It is an error to write more pixels in total than width*height.
 * After all the pixels are written, every subsequent call will return TINYPNGOUT_DONE (which is considered success).
 * 
 * Example usage:
 *   uint8_t pixels[WIDTH * HEIGHT * 3];
 *   ... (fill pixels) ...
 *   if (TinyPngOut_write(&pngout, pixels, WIDTH * HEIGHT) != TINYPNGOUT_OK) {
 *     ... (handle error) ...
 *   }
 *   if (TinyPngOut_write(&pngout, NULL, 0) != TINYPNGOUT_DONE) {
 *     ... (handle error) ...
 *   }
 *   fclose(fout);
 */
enum TinyPngOutStatus TinyPngOut_write(struct TinyPngOut pngout[static 1], const uint8_t pixels[], int count);