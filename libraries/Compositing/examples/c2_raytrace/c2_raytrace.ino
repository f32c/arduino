/*************************************************** 
(c)Emanuel Stiebler
LICENSE=BSD

Example 
compositing2 used as simple linear framebuffer
****************************************************/
extern "C"
{
  #include "raytrace.h"
}
#include <Compositing.h>
#define SPRITE_MAX 10
Compositing c2;

//Define Visuals
//#define SCREEN_WIDTH 640
//#define SCREEN_HEIGHT 480

#define XRES (SCREEN_WIDTH-1)
#define YRES (SCREEN_HEIGHT-1)
#define ANZCOL 255

// crude malloc()
pixel_t *bitmap = (pixel_t *)0x80080000;

void emu_set_pix(int x, int y, int r, int g, int b)
{
  bitmap[x + SCREEN_WIDTH*y] = RGB2PIXEL((r << 16) | (g << 8) | b);
}

void cleanup(char *a)
{
  // print some message if malloc fails
}

void setup() 
{
  int i, x,y;

  c2.init();
  c2.alloc_sprites(SPRITE_MAX);
  // currently malloc() doesn't work for more than 1MB
  // bitmap = (pixel_t *) malloc(SCREEN_WIDTH*SCREEN_HEIGHT*sizeof(pixel_t)); // alloc memory for video
  c2.sprite_from_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT, bitmap); // create c2 sprite
  c2.sprite_refresh(); // show it on screen

  for(i = 0; i < SCREEN_WIDTH*SCREEN_HEIGHT; i++)
    bitmap[i] = RGB2PIXEL(i<<8);  // clear screen

  delay(1000);
  main_raytrace();
}

void loop()
{
}

