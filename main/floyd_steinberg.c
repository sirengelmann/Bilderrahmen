#include "floyd_steinberg.h"
#include "epdif.h"

const unsigned char epd_palette[8][3] = {
  {0, 0, 0}, /*BLACK*/
  {255, 255, 255}, /*WHITE*/
  {0, 255, 0}, /*GREEN*/
  {0, 0, 255}, /*BLUE*/
  {255, 0, 0}, /*RED*/
  {255, 247, 0}, /*YELLOW*/
  {255, 123, 0}, /*ORANGE*/
  {255, 126, 221}   /*MAGENTA*/
};

void dither(unsigned char* img /*3 bytes per pixel*/, unsigned int height, unsigned int width){
for (int y = 0; y < height -1; y++) {
    for (int x = 1; x < width-1; x++) {
      unsigned char oldR = img[3*buffer_index(x, y, width)];
      unsigned char oldG = img[3*buffer_index(x, y, width) + 1];
      unsigned char oldB = img[3*buffer_index(x, y, width) + 2];

      const unsigned char* returnC = findClosest(oldR, oldG, oldB);
      unsigned char newR = returnC[0];
      unsigned char newG = returnC[1];
      unsigned char newB = returnC[2];
      img[3*buffer_index(x, y, width)] = newR;
      img[3*buffer_index(x, y, width) + 1] = newG;
      img[3*buffer_index(x, y, width) + 2] = newB;

      signed int errR = oldR - newR;
      signed int errG = oldG - newG;
      signed int errB = oldB - newB;


      //x+1 y
      unsigned int index = buffer_index(x+1, y, width);
      unsigned char r = img[3*index];
	  unsigned char g = img[3*index + 1];
	  unsigned char b = img[3*index + 2];

      /*r = constrainChar(r + floorf(errR * 7.0/16.0));
      g = constrainChar(g + floorf(errG * 7.0/16.0));
      b = constrainChar(b + floorf(errB * 7.0/16.0));*/
	  r = constrainChar(r + ((errR * 7) / 16));
	  g = constrainChar(g + ((errG * 7) / 16));
	  b = constrainChar(b + ((errB * 7) / 16));

      img[3*index] = r;
      img[3*index + 1] = g;
      img[3*index + 2] = b;


      //x-1 y+1
      index = buffer_index(x-1, y+1, width);
      r = img[3*index];
	  g = img[3*index + 1];
	  b = img[3*index + 2];

	  /*r = constrainChar(r + floorf(errR * 3.0/16.0));
	  g = constrainChar(g + floorf(errG * 3.0/16.0));
	  b = constrainChar(b + floorf(errB * 3.0/16.0));*/
	  r = constrainChar(r + ((errR * 3) / 16));
	  g = constrainChar(g + ((errG * 3) / 16));
	  b = constrainChar(b + ((errB * 3) / 16));

	  img[3*index] = r;
	  img[3*index + 1] = g;
	  img[3*index + 2] = b;


	  //x y+1
      index = buffer_index(x, y+1, width);
      r = img[3*index];
      g = img[3*index + 1];
      b = img[3*index + 2];

      /*r = constrainChar(r + floorf(errR * 5.0/16.0));
      g = constrainChar(g + floorf(errG * 5.0/16.0));
      b = constrainChar(b + floorf(errB * 5.0/16.0));*/
      r = constrainChar(r + ((errR * 5) / 16));
	  g = constrainChar(g + ((errG * 5) / 16));
	  b = constrainChar(b + ((errB * 5) / 16));

      img[3*index] = r;
      img[3*index + 1] = g;
      img[3*index + 2] = b;


      //x+1 y+1
      index = buffer_index(x+1, y+1, width);
      r = img[3*index];
      g = img[3*index + 1];
      b = img[3*index + 2];

      /*r = constrainChar(r + floorf(errR * 1.0/16.0));
      g = constrainChar(g + floorf(errG * 1.0/16.0));
      b = constrainChar(b + floorf(errB * 1.0/16.0));*/
      r = constrainChar(r + (errR / 16));
	  g = constrainChar(g + (errG / 16));
	  b = constrainChar(b + (errB / 16));

      img[3*index] = r;
      img[3*index + 1] = g;
      img[3*index + 2] = b;
    }
  }
}


unsigned char* imgtoepd(unsigned char* img, unsigned int height, unsigned int width){
	if(img == NULL) {
		return NULL;
	}
	unsigned char* outbuf = (unsigned char*) malloc(height * width / 2 * sizeof(unsigned char));
	if(outbuf == NULL) {
		return NULL;
	}
	for (int y = 0; y < height; y++) {
	    for (int x = 0; x < width / 2; x++) {
	      unsigned char rMSB = img[3*buffer_index(2*x, y, width)];
	      unsigned char gMSB = img[3*buffer_index(2*x, y, width) + 1];
	      unsigned char bMSB = img[3*buffer_index(2*x, y, width) + 2];
	      unsigned char rLSB = img[3*buffer_index(1 + 2*x, y, width)];
		  unsigned char gLSB = img[3*buffer_index(1 + 2*x, y, width) + 1];
		  unsigned char bLSB = img[3*buffer_index(1 + 2*x, y, width) + 2];
		  //do color matching
		  unsigned char colMSB = findClosestEPD(rMSB, gMSB, bMSB);
		  unsigned char colLSB = findClosestEPD(rLSB, gLSB, bLSB);

		  outbuf[x + y * (width / 2)] = (colMSB<<4) | colLSB;
	    }
	}
	return outbuf;

}


unsigned int buffer_index(unsigned int x, unsigned int y, unsigned int width) {
  return x + y * width;
}



const unsigned char* findClosest(unsigned char r, unsigned char g, unsigned char b) {
  int closest = 0;
  unsigned long closest_dist = 99999999;
  for (int i=0; i < 8; i++) {
    unsigned char p_r = epd_palette[i][0];
    unsigned char p_g = epd_palette[i][1];
    unsigned char p_b = epd_palette[i][2];
    unsigned long temp_dist =
      (r - p_r)*(r - p_r) +
      (g - p_g)*(g - p_g) +
      (b - p_b)*(b - p_b);

    if (temp_dist < closest_dist) {
      closest_dist = temp_dist;
      closest = i;
    }
  }
  const unsigned char* returnColor = epd_palette[closest];
  return returnColor;
}

unsigned char findClosestEPD(unsigned char r, unsigned char g, unsigned char b) {
  unsigned char closest = 0;
  unsigned long closest_dist = 99999999;
  for (int i=0; i < 8; i++) {
    unsigned char p_r = epd_palette[i][0];
    unsigned char p_g = epd_palette[i][1];
    unsigned char p_b = epd_palette[i][2];
    unsigned long temp_dist =
      (r - p_r)*(r - p_r) +
      (g - p_g)*(g - p_g) +
      (b - p_b)*(b - p_b);

    if (temp_dist < closest_dist) {
      closest_dist = temp_dist;
      closest = i;
    }
  }
  return closest;
}

unsigned char constrainChar(int in){
	return in < 0 ? 0 : in > 255 ? 255 : (unsigned char)in;
}

