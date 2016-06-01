#ifndef SHAPE_H
#define SHAPE_H
#include "compositing.h" // pixel_t only

struct charcolors
{
  char c; // letter indicating the color (0 to end of list)
  pixel_t color; // color value bits: RRRGGGBB if 8bpp
};

// struct used to draw sprite in C
struct shape
{
  struct charcolors *colors; // array of colors
  char **bmp; // the bitmap, NULL-terminated
  uint8_t w,h; // width x-size, height y-size
};

extern struct shape Shape[];

#endif // SHAPE_H
