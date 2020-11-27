/*****************************************************************************
* | File      	:   EPD_5in65f.h
* | Author      :   Waveshare team
* | Function    :   5.65inch e-paper
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2020-07-08
* | Info        :
* -----------------------------------------------------------------------------
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/

#ifndef __EPD_5IN65F_H__
#define __EPD_5IN65F_H__

#include "epdif.h"
#include "driver/gpio.h"



#define UWORD   unsigned int
#define UBYTE   unsigned char
#define UDOUBLE  unsigned long

/**********************************
Color Index
**********************************/
#define EPD_5IN65F_BLACK   0x0	/// 000
#define EPD_5IN65F_WHITE   0x1	///	001
#define EPD_5IN65F_GREEN   0x2	///	010
#define EPD_5IN65F_BLUE    0x3	///	011
#define EPD_5IN65F_RED     0x4	///	100
#define EPD_5IN65F_YELLOW  0x5	///	101
#define EPD_5IN65F_ORANGE  0x6	///	110
#define EPD_5IN65F_PURPLE   0x7	///	111


gpio_num_t epd5in65f_reset_pin;
gpio_num_t epd5in65f_dc_pin;
gpio_num_t epd5in65f_cs_pin;
gpio_num_t epd5in65f_busy_pin;
unsigned long epd5in65f_width;
unsigned long epd5in65f_height;

#define HIGH 1
#define LOW 0

int  Init(void);
void EPD_5IN65F_BusyHigh(void);
void EPD_5IN65F_BusyLow(void);
void Reset(void);
void EPD_5IN65F_Display(const UBYTE *image);
void EPD_5IN65F_Display_part(const UBYTE *image, UWORD xstart, UWORD ystart,
							 UWORD image_width, UWORD image_heigh);
void EPD_5IN65F_Show7Block(void);
void SendCommand(unsigned char command);
void SendData(unsigned char data);
void Suspend(void);
void Clear(UBYTE color);

#endif
/* END OF FILE */
