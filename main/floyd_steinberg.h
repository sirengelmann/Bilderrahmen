#ifndef FLOYD_STEINBERG_H
#define FLOYD_STEINBERG_H

#include <math.h>

void dither(unsigned char* img);

const unsigned char epd_palette[8][3];

const unsigned char* findClosest(unsigned char r, unsigned char g, unsigned char b);

unsigned char findClosestEPD(unsigned char r, unsigned char g, unsigned char b);

unsigned char* imgtoepd(unsigned char* in);

unsigned int buffer_index(unsigned int x, unsigned int y);

unsigned char constrainChar(int in);

#endif
