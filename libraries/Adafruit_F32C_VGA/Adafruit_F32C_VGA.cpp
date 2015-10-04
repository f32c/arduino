/*********************************************************************
This is a library for F32C PAL composite video framebuffer
*********************************************************************/

#include <avr/pgmspace.h>
#ifdef __AVR__
 #include <util/delay.h>
#endif
#include <stdlib.h>

#include "Adafruit_GFX.h"
#include "Adafruit_F32C_VGA.h"

#if 0
extern "C" {
#include <dev/fb.h>
#include <dev/io.h>
}
#endif

// the most basic function, set a single pixel
void Adafruit_F32C_VGA::drawPixel(int16_t x, int16_t y, uint16_t color) {
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
  videomem[x/8+y*80] |= (color != 0 ? 0x01010101 : 0)<<(x&7);
}

Adafruit_F32C_VGA::Adafruit_F32C_VGA(int8_t mode) :
Adafruit_GFX(F32C_VGA_WIDTH, F32C_VGA_HEIGHT) {
//  fb_set_mode(mode);
}

void Adafruit_F32C_VGA::begin() {
  videomem = videobase;
  videopage = 0;
  *videodisplay = videomem;
}

void Adafruit_F32C_VGA::invertDisplay(uint8_t i) {
}

// startscrollright
// Activate a right handed scroll for rows start through stop
// Hint, the display is 16 rows tall. To scroll the whole display, run:
// display.scrollright(0x00, 0x0F) 
void Adafruit_F32C_VGA::startscrollright(uint8_t start, uint8_t stop){
}

// startscrollleft
// Activate a right handed scroll for rows start through stop
// Hint, the display is 16 rows tall. To scroll the whole display, run:
// display.scrollright(0x00, 0x0F) 
void Adafruit_F32C_VGA::startscrollleft(uint8_t start, uint8_t stop){
}

// startscrolldiagright
// Activate a diagonal scroll for rows start through stop
// Hint, the display is 16 rows tall. To scroll the whole display, run:
// display.scrollright(0x00, 0x0F) 
void Adafruit_F32C_VGA::startscrolldiagright(uint8_t start, uint8_t stop){
}

// startscrolldiagleft
// Activate a diagonal scroll for rows start through stop
// Hint, the display is 16 rows tall. To scroll the whole display, run:
// display.scrollright(0x00, 0x0F) 
void Adafruit_F32C_VGA::startscrolldiagleft(uint8_t start, uint8_t stop){
}

void Adafruit_F32C_VGA::stopscroll(void){
}

// Dim the display
// dim = true: display is dimmed
// dim = false: display is normal
void Adafruit_F32C_VGA::dim(uint8_t dim) {
}

void Adafruit_F32C_VGA::display(void) {
  *videodisplay = videomem;
  videopage ^= 1;
  videomem = videobase + (videopage ? videosize : 0);
}

// clear everything on current page
void Adafruit_F32C_VGA::clearDisplay(void) {
  memset(videomem, 0, videosize);
}
