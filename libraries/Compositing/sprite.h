#ifndef SPRITE_H
#define SPRITE_H
#include "compositing_line.h"
#include "shape.h"

// if using existing content, only 2K of video cache is sufficient
// 1-USE 0-DON'T USE
#define USE_EXISTING_CONTENT 1

// struct used to draw sprite in C
struct sprite
{
  int16_t x,y; // current position on the screen
  uint16_t w,h; // w: width x-size (currently ignored), h: height y-size (number of lines)
  struct compositing_line line[]; // content: array of lines - NULL to terminate
};

#endif // SPRITE_H

