# ESP32 WROVER E-Ink Picture Frame
 Code for ESP IDF 4.1, written using Eclipse
 
 (this project is under development, do not expect everything to work)
  
  it is going to be a present for christmas, so, as long as it somewhat works I am happy
  
  this project is inspired by a YouTube video of cnlohr, GitHub user name: cnlohr 
    
  ## Hardware
 
  ### used hardware (so far):
    - "5.65inch e-Paper Module" from waveshare
    - "ESP32 WROVER-B 4MB Module" from Espressif Inc
    - "LIS3DH 3-axis MEMS accelerometer" by STMicroelectronics
    - 5000 mAh LiPo battery from EREMIT.de
    
  the PCB is still in the prototyping phase, I plan to publish the BOM and the production files when I am happy with it
    
  ## Software
  
  ### libraries taken from other people:
    - qdbmp (Quick n' Dirty BMP) by madwyn, a minimalistic BMP library for C
    - e-paper demo code by waveshare, ported and modified for ESP32 ESP-IDF using C
  
  ### Currently working:
    - read BITMAP images from SDCard (SPI Mode), 448 x 600, 24bpp, minimal header
    - apply floyd steinberg dithering algorithm to image, mapping to 8 different colors
    - convert image data to E-Ink specific encoding and flush to display
    - read acceleration data from LIS3DH accelerometer in i2c mode, interrupt on position
 
  ### Currently NOT working, but planned to be implemented:
    - change the SD card interface to SDIO (when PCB is done)
    - support 600 x 448 images too
    - using the accelerometer to only display fitting images
    - when the circuit boards arrive, implement proper battery management code
