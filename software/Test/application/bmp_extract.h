#ifndef _BMP_EXTRACT_H_
#define _BMP_EXTRACT_H_

#include <stdio.h>
//#include <stdlib.h>
#include <stdint.h>
//#include <io.h>
//#include "system.h"
//#include <time.h>
//#include <string.h>

typedef union
{
	struct
	{
		uint32_t BMPFileSize;
		uint32_t BMPUnused;
		uint32_t BMPDataOffset;
		uint32_t BMPRemainingDIBBytes;
		uint32_t BMPWidth;
		uint32_t BMPHeight;
		uint16_t BMPColourPlaneCount;
		uint16_t BMPBitsPerPixel;
		uint32_t BMPPixelCompression;
		uint32_t BMPDataSize;
		uint32_t BMPHozRes;
		uint32_t BMPVertRes;
		uint32_t BMPColours;
		uint32_t BMPImportantColours;
	};
	int8_t BMPRawData[52];
} Bmp_header;

typedef struct {
	Bmp_header *header;		// Structure unpacking the contents of the BMP header
	int8_t *buffer;		// Buffer containing the entire contents of the BMP file
	int8_t *bitmap;		// Buffer pointing to the start of the bitmap area of <buffer>.
} Bmp_data;

int bmp_extract(char *file_name, Bmp_data *pic_data);

#endif //_BMP_EXTRACT_H_
