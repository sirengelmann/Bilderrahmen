# ESP32 WROVER E-Ink Picture Frame
 Code for ESP-IDF 4.1, written using Eclipse
 
 (this project is under development, do not expect everything to work)
  
  it is going to be a present for christmas, so, as long as it somewhat works I am happy
  
  this project is inspired by a YouTube video of cnlohr, GitHub user name: cnlohr 
    
  ## Hardware
 
  ### used hardware (so far):
    - "5.65inch e-Paper Module" 7 color ACeP display from waveshare
    - "ESP32 WROVER-B 4MB" Module from Espressif Systems
    - "LIS3DH 3-axis MEMS accelerometer" from STMicroelectronics
    - 5000 mAh LiPo battery from EREMIT.de
    
  the PCB is still in the prototyping phase, I plan to publish the BOM and the production files when I am happy with it
  
  ### pin mapping:
    - SDCARD CMD: GPIO15
    - SDCARD CLK: GPIO14
    - SDCARD  D0: GPIO2
    - SDCARD  D1: GPIO4
    - SDCARD  D2: GPIO12
    - SDCARD  D3: GPIO13
    - SDCARD  CD: GPIO21
    
    - SDCARD POWER (and DC/DC enable): GPIO22
    
    - EPAPER  CLK: GPIO18
    - EPAPER  DIN: GPIO23
    - EPAPER BUSY: GPIO25
    - EPAPER  RST: GPIO26
    - EPAPER  D/C: GPIO27
    - EPAPER   CS: GPIO33
    
    - LIS3DH SCL: GPIO19
    - LIS3DH SDA: GPIO5
    - LIS3DH INT: GPIO35
    
    - CHARGER INT: GPIO36
    
  ## Software
  
  ### libraries taken from other people:
    - qdbmp (Quick n' Dirty BMP) by madwyn, a minimalistic BMP library for C
    - e-paper demo code by waveshare, ported and modified for ESP32 ESP-IDF using C
  
  ### Currently working:
    - use accelerometer to choose fitting image out of "horizontal" and "vertical" folders off the SD card
    - read BITMAP images from SDCard (SPI Mode), 448 x 600, 24bpp, minimal header
    - apply floyd steinberg dithering algorithm to image, mapping to 8 different colors
    - convert image data to E-Ink specific encoding and flush to display
    - read acceleration data from LIS3DH accelerometer in i2c mode, interrupt on crossing position threshold
    - remember which picture was displayed last in nvs flash, increment counter on every boot
    - battery management code with undervoltage shutdown and use charging IC as interrupt source for re-enabling
    - interrupt signal of the accelerometer as a wakeup source for the ESP32
 
  ### Currently NOT working, but planned to be implemented:
    - change the SD card interface to SDIO (when PCB is done)
    - support 600 x 448 images too
   
