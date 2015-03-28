/*********************************************************************
This is a library for F32C framebuffer
*********************************************************************/

#include <avr/pgmspace.h>
#ifdef __AVR__
 #include <util/delay.h>
#endif
#include <stdlib.h>

#include <Wire.h>

#include "Adafruit_GFX.h"
#include "Adafruit_F32C_video.h"

extern "C" {
#include <fb.h>
#include <io.h>
}

// currently displayed buffer
static uint8_t visual = 0;

// the most basic function, set a single pixel
void Adafruit_F32C_video::drawPixel(int16_t x, int16_t y, uint16_t color) {
  if ((x < 0) || (x >= width()) || (y < 0) || (y >= height()))
    return;

  // check rotation, move pixel around if necessary
  switch (getRotation()) {
  case 1:
    swap(x, y);
    x = WIDTH - x - 1;
    break;
  case 2:
    x = WIDTH - x - 1;
    y = HEIGHT - y - 1;
    break;
  case 3:
    swap(x, y);
    y = HEIGHT - y - 1;
    break;
  }  

  fb_plot(x,y,color);
}

void Adafruit_F32C_video::drawLine(int16_t x0, int16_t y0,
			    int16_t x1, int16_t y1,
			    uint16_t color) {
  fb_line(x0,y0,x1,y1,color);
}


Adafruit_F32C_video::Adafruit_F32C_video(int8_t mode) :
Adafruit_GFX(F32C_VIDEOWIDTH, F32C_VIDEOHEIGHT) {
  fb_set_mode(mode);
}
  

void Adafruit_F32C_video::begin() {
}


void Adafruit_F32C_video::invertDisplay(uint8_t i) {
}

// startscrollright
// Activate a right handed scroll for rows start through stop
// Hint, the display is 16 rows tall. To scroll the whole display, run:
// display.scrollright(0x00, 0x0F) 
void Adafruit_F32C_video::startscrollright(uint8_t start, uint8_t stop){
}

// startscrollleft
// Activate a right handed scroll for rows start through stop
// Hint, the display is 16 rows tall. To scroll the whole display, run:
// display.scrollright(0x00, 0x0F) 
void Adafruit_F32C_video::startscrollleft(uint8_t start, uint8_t stop){
}

// startscrolldiagright
// Activate a diagonal scroll for rows start through stop
// Hint, the display is 16 rows tall. To scroll the whole display, run:
// display.scrollright(0x00, 0x0F) 
void Adafruit_F32C_video::startscrolldiagright(uint8_t start, uint8_t stop){
}

// startscrolldiagleft
// Activate a diagonal scroll for rows start through stop
// Hint, the display is 16 rows tall. To scroll the whole display, run:
// display.scrollright(0x00, 0x0F) 
void Adafruit_F32C_video::startscrolldiagleft(uint8_t start, uint8_t stop){
}

void Adafruit_F32C_video::stopscroll(void){
}

// Dim the display
// dim = true: display is dimmed
// dim = false: display is normal
void Adafruit_F32C_video::dim(uint8_t dim) {
}

void Adafruit_F32C_video::display(void) {
  fb_set_visible(visual);
  visual ^= 1;
  fb_set_drawable(visual);
}

// clear everything
void Adafruit_F32C_video::clearDisplay(void) {
  fb_rectangle(0,0,F32C_VIDEOWIDTH-1,F32C_VIDEOHEIGHT-1, BLACK);
}

void Adafruit_F32C_video::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
                          uint16_t color) {
  fb_rectangle(x,y,x+w-1,y+h-1, color);
}
 