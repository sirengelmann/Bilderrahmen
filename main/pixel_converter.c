#include "pixel_converter.h"

unsigned char* BMPtoBUF(BMP* bmp){
	//use free(buf) when you are done with it
	unsigned long bufsize = BMP_GetHeight(bmp) * BMP_GetWidth(bmp) * 3;
	unsigned char* buf = (unsigned char*) malloc(bufsize * sizeof(unsigned char));
	if(buf == NULL) {
		printf("ERROR while trying to allocate %lu bytes for the pixel buffer using calloc\n", bufsize);
		return NULL;
	}
	//printf("allocated %lu bytes for the pixel buffer\n", bufsize);

	unsigned int x_max = BMP_GetWidth(bmp);
	unsigned int y_max = BMP_GetHeight(bmp);
	unsigned char r = 0, g = 0, b = 0;
	for(unsigned int y = 0; y < y_max; y++){
		for(unsigned int x = 0; x < x_max; x++){
			BMP_GetPixelRGB(bmp, x, y, &r, &g, &b);
			buf[3 * (x + y * x_max)] = r;
			buf[3 * (x + y * x_max) + 1] = g;
			buf[3 * (x + y * x_max) + 2] = b;
		}
	}
	return buf;
}
