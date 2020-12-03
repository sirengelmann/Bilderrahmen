/*****************************************************************************
* | File      	:   EPD_5in65f.c
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

#include "imagedata000.h"
#include "imagedata001.h"
#include "imagedata002.h"
#include "imagedata003.h"
#include <stdlib.h>
#include <string.h>
#include "epd5in65f.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"





/******************************************************************************
function :  Initialize the e-Paper register
parameter:
******************************************************************************/
int Init(void) {
  if (IfInit() != 0) {
      return -1;
  }
  epd5in65f_reset_pin = RST_PIN;
  epd5in65f_dc_pin = DC_PIN;
  epd5in65f_cs_pin = CS_PIN;
  epd5in65f_busy_pin = BUSY_PIN;
  epd5in65f_width = EPD_WIDTH;
  epd5in65f_height = EPD_HEIGHT;


  Reset();
    EPD_5IN65F_BusyHigh();
    SendCommand(0x00);
    SendData(0xEF);
    SendData(0x08);
    SendCommand(0x01);
    SendData(0x37);
    SendData(0x00);
    SendData(0x23);
    SendData(0x23);
    SendCommand(0x03);
    SendData(0x00);
    SendCommand(0x06);
    SendData(0xC7);
    SendData(0xC7);
    SendData(0x1D);
    SendCommand(0x30);
    SendData(0x3C);
    SendCommand(0x40);
    SendData(0x00);
    SendCommand(0x50);
    SendData(0x37);
    SendCommand(0x60);
    SendData(0x22);
    SendCommand(0x61);
    SendData(0x02);
    SendData(0x58);
    SendData(0x01);
    SendData(0xC0);
    SendCommand(0xE3);
    SendData(0xAA);
	
    DelayMs(100);
    SendCommand(0x50);
    SendData(0x37);
    return 0;
}

/**
 *  @brief: basic function for sending commands
 */
void SendCommand(unsigned char command) {
    DigitalWrite(epd5in65f_dc_pin, LOW);
    SpiTransfer(command);
}

/**
 *  @brief: basic function for sending data
 */
void SendData(unsigned char data) {
    DigitalWrite(epd5in65f_dc_pin, HIGH);
    SpiTransfer(data);
}

void SendDataBlock(unsigned char* data, unsigned long len) {
	DigitalWrite(epd5in65f_dc_pin, HIGH);
	SpiTransferBlock(data, len);
}

void EPD_5IN65F_BusyHigh(void)// If BUSYN=0 then waiting
{
    while(DigitalRead(BUSY_PIN) == LOW) vTaskDelay(10 / portTICK_PERIOD_MS);
}

void EPD_5IN65F_BusyLow(void)// If BUSYN=1 then waiting
{
    while(DigitalRead(BUSY_PIN) == HIGH) vTaskDelay(10 / portTICK_PERIOD_MS);
}

/**
 *  @brief: module reset.
 *          often used to awaken the module in deep sleep,
 *          see Epd::Sleep();
 */
void Reset(void) {
    DigitalWrite(epd5in65f_reset_pin, LOW);                //module reset
    DelayMs(1);
    DigitalWrite(epd5in65f_reset_pin, HIGH);
    DelayMs(200);
}

/******************************************************************************
function :	show 7 kind of color block
parameter:
******************************************************************************/
void EPD_5IN65F_Show7Block(void)
{
	unsigned long i,j,k;
    unsigned char const Color_seven[8] =
	{EPD_5IN65F_BLACK,EPD_5IN65F_BLUE,EPD_5IN65F_GREEN,EPD_5IN65F_ORANGE,
	EPD_5IN65F_RED,EPD_5IN65F_YELLOW,EPD_5IN65F_WHITE,EPD_5IN65F_PURPLE};
    SendCommand(0x61);//Set Resolution setting
    SendData(0x02);
    SendData(0x58);
    SendData(0x01);
    SendData(0xC0);
    SendCommand(0x10);

    for(i=0; i<224; i++) {
        for(k = 0 ; k < 4; k ++) {
            for(j = 0 ; j < 75; j ++) {
                SendData((Color_seven[k]<<4) |Color_seven[k]);
            }
        }
    }
    for(i=0; i<224; i++) {
        for(k = 4 ; k < 8; k ++) {
            for(j = 0 ; j < 75; j ++) {
                SendData((Color_seven[k]<<4) |Color_seven[k]);
            }
        }
    }
    SendCommand(0x04);//0x04
    EPD_5IN65F_BusyHigh();
    SendCommand(0x12);//0x12
    EPD_5IN65F_BusyHigh();
    SendCommand(0x02);  //0x02
    EPD_5IN65F_BusyLow();
	DelayMs(200);
}

/******************************************************************************
function :  Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_5IN65F_Display(const UBYTE *image) {
    unsigned long i,j;
    SendCommand(0x61);//Set Resolution setting
    SendData(0x02);
    SendData(0x58);
    SendData(0x01);
    SendData(0xC0);
    SendCommand(0x10);
    for(i=0; i<epd5in65f_height; i++) {
        for(j=0; j<epd5in65f_width/2; j++)
            SendData(image[j+((epd5in65f_width/2)*i)]);
    }
    SendCommand(0x04);//0x04
    EPD_5IN65F_BusyHigh();
    SendCommand(0x12);//0x12
    EPD_5IN65F_BusyHigh();
    SendCommand(0x02);  //0x02
    EPD_5IN65F_BusyLow();
	DelayMs(200);
}

/******************************************************************************
function :   Sends the image buffer in RAM to e-paper and displays
			 IMPROVED FUNCTION written by Soren Engelmann
******************************************************************************/

void EPD_5IN65F_DisplayFast(const UBYTE *image) {
	unsigned char* image_temp_ptr = (unsigned char*)image;
	unsigned char* buf = malloc(64 * sizeof(unsigned char));
	if(buf == NULL) return;
    unsigned long i;
    SendCommand(0x61);//Set Resolution setting
    SendData(0x02);
    SendData(0x58);
    SendData(0x01);
    SendData(0xC0);
    SendCommand(0x10);
    for(i=0; i<epd5in65f_height * epd5in65f_width / 128; i++) {
    	memcpy(buf, image_temp_ptr, 64);
    	SendDataBlock(buf, 64);
    	image_temp_ptr += 64;
    }
    SendCommand(0x04);//0x04
    EPD_5IN65F_BusyHigh();
    SendCommand(0x12);//0x12
    EPD_5IN65F_BusyHigh();
    SendCommand(0x02);  //0x02
    EPD_5IN65F_BusyLow();
    free(buf);
	DelayMs(200);
}



/******************************************************************************
function :  Sends the part image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_5IN65F_Display_part(const UBYTE *image, UWORD xstart, UWORD ystart,
                                        UWORD image_width, UWORD image_heigh)
{
	unsigned long i,j;
    SendCommand(0x61);//Set Resolution setting
    SendData(0x02);
    SendData(0x58);
    SendData(0x01);
    SendData(0xC0);
    SendCommand(0x10);
    for(i=0; i<epd5in65f_height; i++) {
        for(j=0; j< epd5in65f_width/2; j++)
            if(i<image_heigh+ystart && i>=ystart && j<(image_width+xstart)/2 && j>=xstart/2) {
              SendData(image[(j-xstart/2) + (image_width/2*(i-ystart))]);
            }
			else {
				SendData(0x11);
			}
    }
    SendCommand(0x04);//0x04
    EPD_5IN65F_BusyHigh();
    SendCommand(0x12);//0x12
    EPD_5IN65F_BusyHigh();
    SendCommand(0x02);  //0x02
    EPD_5IN65F_BusyLow();
	DelayMs(200);
}

/******************************************************************************
function : 
      Clear screen
******************************************************************************/
void Clear(UBYTE color) {
    SendCommand(0x61);//Set Resolution setting
    SendData(0x02);
    SendData(0x58);
    SendData(0x01);
    SendData(0xC0);
    SendCommand(0x10);
    for(int i=0; i<epd5in65f_width/2; i++) {
        for(int j=0; j<epd5in65f_height; j++)
            SendData((color<<4)|color);
    }
    SendCommand(0x04);//0x04
    EPD_5IN65F_BusyHigh();
    SendCommand(0x12);//0x12
    EPD_5IN65F_BusyHigh();
    SendCommand(0x02);  //0x02
    EPD_5IN65F_BusyLow();
    DelayMs(500);
}

/**
 *  @brief: After this command is transmitted, the chip would enter the 
 *          deep-sleep mode to save power. 
 *          The deep sleep mode would return to standby by hardware reset. 
 *          The only one parameter is a check code, the command would be
 *          You can use EPD_Reset() to awaken
 */
void Suspend(void) {
    DelayMs(100);
    SendCommand(0x07);
    SendData(0xA5);
    DelayMs(100);
	DigitalWrite(RST_PIN, 0); // Reset
	IfSuspend();
}



/* END OF FILE */
