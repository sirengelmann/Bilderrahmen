# ESP32 WROVER E-Ink Picture Frame
 Code for ESP IDF 4.1 w/ Eclipse
 
 (this project is under development, do not expect everything to work)
  
  it is going to be a present for christmas, so, as long as it somewhat works I am happy
  
  this project is inspired by cnlohr
 
  ### Currently working:
    - read BITMAP images from SDCard (SPI Mode), 448 x 600, 24bpp, minimal header
    - apply floyd steinberg dithering algorithm to image, mapping to 8 different colors
    - convert image data to E-Ink specific encoding and flush to display
    - read WHO_AM_I test register from LIS3DH accelerometer in i2c mode
 
  ### Currently NOT working, but planned to be implemented:
    - accelerate the e paper drawing functions (maybe with DMA?)
    - support 600 x 448 images too
    - using the accelerometer to find out orientation and only display fitting images
    - when the circuit boards arrive, implement proper battery management code
