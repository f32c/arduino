/*********************************************************************
This is a library for F32C PAL composite video framebuffer
*********************************************************************/

#include <avr/pgmspace.h>
#ifdef __AVR__
 #include <util/delay.h>
#endif
#include <stdlib.h>

#include "Adafruit_GFX.h"
#include "Adafruit_F32C_LEDSTRIP.h"

// static initializer of compositing2 set of pointers
// that creates completely contiguous bitmap

struct compositing_line
{
   struct compositing_line *next; // 32-bit continuation of the same structure, NULL if no more
   int16_t x; // where to start on screen (can be negative)
   uint16_t n; // number of pixels contained here
   // pixels can be multiple of 4 (n lower 2 bits discarded)
   // minimal n is currently 4 (when composite2 core is fixed it could be 0)
   volatile uint8_t *bmp; // pointer to array of pixels (could be more than 1 element)
};

struct compositing_line *scanline[F32C_VGA_HEIGHT_MULTIPLE*F32C_VGA_HEIGHT], metadata[F32C_VGA_HEIGHT];

// the most basic function, set a single pixel
void Adafruit_F32C_LEDSTRIP::drawPixel(int16_t x, int16_t y, uint16_t color) {
  volatile uint8_t *ptr;
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

  // compositing2 can make linear bitmap
  ptr = &(videomem[x+y*F32C_VGA_WIDTH]);

  // color space highcolor RGB 565 -> RRRGGGBB
  // replace new bits (simple 16-color)
  *ptr = ((color& (3<< 3)) >> (3-0)) // blue
       | ((color& (7<< 8)) >> (8-2)) // green
       | ((color& (7<<13)) >> (13-6)) // red
       ;
}

Adafruit_F32C_LEDSTRIP::Adafruit_F32C_LEDSTRIP(int8_t mode) :
Adafruit_GFX(F32C_VGA_WIDTH, F32C_VGA_HEIGHT) {
//  fb_set_mode(mode);
}

void Adafruit_F32C_LEDSTRIP::begin() {
  int i;
  // (volatile uint8_t *)0xFFFFFB80 = 0xBF;   // b = background, f = foreground
  videobase = malloc(F32C_VGA_WIDTH*F32C_VGA_HEIGHT*sizeof(uint8_t));
  videomem = videobase;
  videopage = 0;
  for(i = 0; i < F32C_VGA_HEIGHT_MULTIPLE*F32C_VGA_HEIGHT; i++)
    scanline[i] = &(metadata[i%F32C_VGA_HEIGHT]);
  for(i = 0; i < F32C_VGA_HEIGHT; i++)
  {
    metadata[i].x = 0;
    metadata[i].n = F32C_VGA_WIDTH;
    metadata[i].bmp = videobase + i * F32C_VGA_WIDTH;
  }
  *videodisplay = (uint32_t)&(scanline[0]);
  *cntrl_reg = 0b11000000; // enable video, yes bitmap, no text mode, no cursor
}

void Adafruit_F32C_LEDSTRIP::invertDisplay(uint8_t i) {
}

// startscrollright
// it should
// Activate a right handed scroll for rows start through stop
// Hint, the display is 16 rows tall. To scroll the whole display, run:
// display.scrollright(0x00, 0x0F) 
// this function has non-adafruit-gfx compliant behaviour
// it doesn't start autoscroll but it does jump scroll to arbitrary
// offset given by start paramter. Scroll is done using compositing2
// acceleration feature by rotating an array of scanline pointers
// to metadata
void Adafruit_F32C_LEDSTRIP::startscrollright(uint8_t start, uint8_t stop)
{
  int i;

  for(i = 0; i < F32C_VGA_HEIGHT_MULTIPLE*F32C_VGA_HEIGHT; i++)
    scanline[i] = &(metadata[(i+start)%F32C_VGA_HEIGHT]);
}

// startscrollleft
// Activate a right handed scroll for rows start through stop
// Hint, the display is 16 rows tall. To scroll the whole display, run:
// display.scrollright(0x00, 0x0F) 
void Adafruit_F32C_LEDSTRIP::startscrollleft(uint8_t start, uint8_t stop){
}

// startscrolldiagright
// Activate a diagonal scroll for rows start through stop
// Hint, the display is 16 rows tall. To scroll the whole display, run:
// display.scrollright(0x00, 0x0F) 
void Adafruit_F32C_LEDSTRIP::startscrolldiagright(uint8_t start, uint8_t stop){
}

// startscrolldiagleft
// Activate a diagonal scroll for rows start through stop
// Hint, the display is 16 rows tall. To scroll the whole display, run:
// display.scrollright(0x00, 0x0F) 
void Adafruit_F32C_LEDSTRIP::startscrolldiagleft(uint8_t start, uint8_t stop){
}

void Adafruit_F32C_LEDSTRIP::stopscroll(void){
}

// Dim the display
// dim = true: display is dimmed
// dim = false: display is normal
void Adafruit_F32C_LEDSTRIP::dim(uint8_t dim) {
}

void Adafruit_F32C_LEDSTRIP::display(void) {
  *videodisplay = (uint32_t) &(scanline[0]);
  videomem = videobase;
}

// clear everything on current page
void Adafruit_F32C_LEDSTRIP::clearDisplay(void) {
  memset((uint32_t *)videomem, 0, videosize);
}
