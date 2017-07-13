#ifndef _JPEG_ENCODING_H_
#define _JPEG_ENCODING_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
//#include <io.h>
//#include "system.h"
//#include <time.h>

#include "jpegclib/jpeglib.h"
#include <setjmp.h>

int jpeg_encode(char *destFilename, int8_t *rawData, int image_height, int image_width, int quality);

#endif //_JPEG_ENCODING_H_
