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

#include <limits.h>
#include <stdlib.h>
#include <sys/types.h>
#include "TinyPngOut.h"


/* Local declarations */

#define DEFLATE_MAX_BLOCK_SIZE 65535

static enum TinyPngOutStatus finish(const struct TinyPngOut pngout[static 1]);
static uint32_t crc32  (uint32_t state, const uint8_t data[], size_t len);
static uint32_t adler32(uint32_t state, const uint8_t data[], size_t len);


/* Public function implementations */

enum TinyPngOutStatus TinyPngOut_init(struct TinyPngOut pngout[static 1], mp_obj_t fout[static 1], int32_t width, int32_t height) {
	// Check arguments
	if (fout == NULL || width <= 0 || height <= 0)
		return TINYPNGOUT_INVALID_ARGUMENT;
	
	// Calculate data sizes
	if (width > (INT32_MAX - 1) / 3)
		return TINYPNGOUT_IMAGE_TOO_LARGE;
	int32_t lineSize = width * 3 + 1;
	
	if (lineSize > INT32_MAX / height)
		return TINYPNGOUT_IMAGE_TOO_LARGE;
	int32_t size = lineSize * height;  // Size of DEFLATE input
	pngout->deflateRemain = size;
	
	int32_t overhead = size / DEFLATE_MAX_BLOCK_SIZE;
	if (overhead * DEFLATE_MAX_BLOCK_SIZE < size)
		overhead++;  // Round up to next block
	overhead = overhead * 5 + 6;
	if (size > INT32_MAX - overhead)
		return TINYPNGOUT_IMAGE_TOO_LARGE;
	size += overhead;  // Size of zlib+DEFLATE output
	
	// Set most of the fields
	pngout->width = lineSize;  // In bytes
	pngout->height = height;   // In pixels
	pngout->outStream = fout;
	pngout->positionX = 0;
	pngout->positionY = 0;
	pngout->deflateFilled = 0;
	pngout->adler = 1;
	
	// Write header (not a pure header, but a couple of things concatenated together)
	#define HEADER_SIZE 43
	uint8_t header[HEADER_SIZE] = {
		// PNG header
		0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A,
		// IHDR chunk
		0x00, 0x00, 0x00, 0x0D,
		0x49, 0x48, 0x44, 0x52,
		width  >> 24, width  >> 16, width  >> 8, width  >> 0,
		height >> 24, height >> 16, height >> 8, height >> 0,
		0x08, 0x02, 0x00, 0x00, 0x00,
		0, 0, 0, 0,  // IHDR CRC-32 to be filled in (starting at offset 29)
		// IDAT chunk
		size >> 24, size >> 16, size >> 8, size >> 0,
		0x49, 0x44, 0x41, 0x54,
		// DEFLATE data
		0x08, 0x1D,
	};
	uint32_t crc = crc32(0, &header[12], 17);
	header[29] = crc >> 24;
	header[30] = crc >> 16;
	header[31] = crc >>  8;
	header[32] = crc >>  0;
	if(tulip_fwrite(fout, header, HEADER_SIZE) != HEADER_SIZE)
	//if (fwrite(header, sizeof(header[0]), HEADER_SIZE, fout) != HEADER_SIZE)
		return TINYPNGOUT_IO_ERROR;
	
	pngout->crc = crc32(0, &header[37], 6);
	return TINYPNGOUT_OK;
}


enum TinyPngOutStatus TinyPngOut_write(struct TinyPngOut pngout[static 1], const uint8_t pixels[], int count) {
	int32_t width  = pngout->width;
	int32_t height = pngout->height;
	if (pngout->positionY == height)
		return TINYPNGOUT_DONE;
	if (count < 0 || count > INT_MAX / 3 || pngout->positionY < 0 || pngout->positionY > height)
		return TINYPNGOUT_INVALID_ARGUMENT;
	
	count *= 3;
	mp_obj_t f = pngout->outStream;
	while (count > 0) {
		// Start DEFLATE block
		if (pngout->deflateFilled == 0) {
			#define BLOCK_HEADER_SIZE 5
			uint16_t size = DEFLATE_MAX_BLOCK_SIZE;
			if (pngout->deflateRemain < (int32_t)size)
				size = (uint16_t)pngout->deflateRemain;
			uint8_t blockheader[BLOCK_HEADER_SIZE] = {
				pngout->deflateRemain <= DEFLATE_MAX_BLOCK_SIZE ? 1 : 0,
				size >> 0,
				size >> 8,
				(size >> 0) ^ 0xFF,
				(size >> 8) ^ 0xFF,
			};
			if(tulip_fwrite(f, blockheader, BLOCK_HEADER_SIZE ) != BLOCK_HEADER_SIZE)
			//if (fwrite(blockheader, sizeof(blockheader[0]), BLOCK_HEADER_SIZE, f) != BLOCK_HEADER_SIZE)
				return TINYPNGOUT_IO_ERROR;
			pngout->crc = crc32(pngout->crc, blockheader, BLOCK_HEADER_SIZE);
		}
		
		// Calculate number of bytes to write in this loop iteration
		size_t n = SIZE_MAX;
		if ((unsigned int)count < n)
			n = (size_t)count;
		if ((uint32_t)(width - pngout->positionX) < n)
			n = (size_t)(width - pngout->positionX);
		if (pngout->deflateFilled >= DEFLATE_MAX_BLOCK_SIZE)  // Impossible
			exit(EXIT_FAILURE);
		if ((uint32_t)(DEFLATE_MAX_BLOCK_SIZE - pngout->deflateFilled) < n)
			n = DEFLATE_MAX_BLOCK_SIZE - pngout->deflateFilled;
		if (n == 0)  // Impossible
			exit(EXIT_FAILURE);
		
		// Beginning of row - write filter method
		if (pngout->positionX == 0) {
			uint8_t b = 0;
			tulip_fwrite(f, &b, 1);
			//if (fputc(b, f) == EOF)
			//	return TINYPNGOUT_IO_ERROR;
			pngout->crc = crc32(pngout->crc, &b, 1);
			pngout->adler = adler32(pngout->adler, &b, 1);
			pngout->deflateRemain--;
			pngout->deflateFilled++;
			pngout->positionX++;
			n--;
		}
		
		// Write bytes and update checksums
		if (tulip_fwrite(f, (uint8_t*)pixels, n) != n) 
		//if (fwrite(pixels, sizeof(pixels[0]), n, f) != n)
			return TINYPNGOUT_IO_ERROR;
		pngout->crc = crc32(pngout->crc, pixels, n);
		pngout->adler = adler32(pngout->adler, pixels, n);
		
		// Increment the position
		count -= n;
		pixels += n;
		
		pngout->deflateRemain -= n;
		pngout->deflateFilled += n;
		if (pngout->deflateFilled == DEFLATE_MAX_BLOCK_SIZE)
			pngout->deflateFilled = 0;
		
		pngout->positionX += n;
		if (pngout->positionX == width) {
			pngout->positionX = 0;
			pngout->positionY++;
			if (pngout->positionY == height) {
				if (count > 0)
					return TINYPNGOUT_INVALID_ARGUMENT;
				return finish(pngout);
			}
		}
	}
	return TINYPNGOUT_OK;
}


/* Private function implementations */

static enum TinyPngOutStatus finish(const struct TinyPngOut pngout[static 1]) {
	#define FOOTER_SIZE 20
	uint32_t adler = pngout->adler;
	uint8_t footer[FOOTER_SIZE] = {
		adler >> 24, adler >> 16, adler >> 8, adler >> 0,
		0, 0, 0, 0,  // IDAT CRC-32 to be filled in (starting at offset 4)
		// IEND chunk
		0x00, 0x00, 0x00, 0x00,
		0x49, 0x45, 0x4E, 0x44,
		0xAE, 0x42, 0x60, 0x82,
	};
	uint32_t crc = crc32(pngout->crc, &footer[0], 4);
	footer[4] = crc >> 24;
	footer[5] = crc >> 16;
	footer[6] = crc >>  8;
	footer[7] = crc >>  0;
	if (tulip_fwrite((pngout->outStream), footer, FOOTER_SIZE ) != FOOTER_SIZE)
	//if (fwrite(footer, sizeof(footer[0]), FOOTER_SIZE, pngout->outStream) != FOOTER_SIZE)
		return TINYPNGOUT_IO_ERROR;
	return TINYPNGOUT_OK;
}


static uint32_t crc32(uint32_t state, const uint8_t data[], size_t len) {
	state = ~state;
	for (size_t i = 0; i < len; i++) {
		for (int j = 0; j < 8; j++) {  // Inefficient bitwise implementation, instead of table-based
			uint32_t bit = (state ^ (data[i] >> j)) & 1;
			state = (state >> 1) ^ ((-bit) & UINT32_C(0xEDB88320));
		}
	}
	return ~state;
}


static uint32_t adler32(uint32_t state, const uint8_t data[], size_t len) {
	uint16_t s1 = (uint16_t)(state >>  0);
	uint16_t s2 = (uint16_t)(state >> 16);
	for (size_t i = 0; i < len; i++) {
		s1 = ((uint32_t)s1 + data[i]) % 65521;
		s2 = ((uint32_t)s2 + s1) % 65521;
	}
	return (uint32_t)s2 << 16 | s1;
}