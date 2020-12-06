#ifndef FLOYD_STEINBERG_H
#define FLOYD_STEINBERG_H

#include <math.h>

void dither(unsigned char* img /*3 bytes per pixel*/, unsigned int height, unsigned int width);

const unsigned char epd_palette[8][3];

const unsigned char* findClosest(unsigned char r, unsigned char g, unsigned char b);

unsigned char findClosestEPD(unsigned char r, unsigned char g, unsigned char b);

unsigned char* imgtoepd(unsigned char* img, unsigned int height, unsigned int width);

unsigned int buffer_index(unsigned int x, unsigned int y, unsigned int width);

unsigned char constrainChar(int in);

#endif
