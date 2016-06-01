#ifndef SPRITE_H
#define SPRITE_H
#include "compositing.h"
#include "shape.h"

#define SPRITE_MAX (40*30)
//#define SPRITE_MAX (436)

// if using existing content, only 2K of video cache is sufficient
// 1-USE 0-DON'T USE
#define USE_EXISTING_CONTENT 1

// struct used to draw sprite in C
struct sprite
{
  int16_t x,y; // current position on the screen
  uint8_t w,h; // width x-size, height y-size (number of lines)
  struct compositing_line line[]; // content: array of lines - NULL to terminate
};

extern struct sprite *Sprite[]; // global array of all sprites, each sprite of different size

void sprite_refresh(void); // refresh compositing linked list after changing x/y positions
void shape_to_sprite(int shape, int sprite);
void sprite_clone(int original, int clone); // clone content from one sprite to another
void sprite_position(int sprite, int x, int y);
void sprite_link_content(int original, int clone);

#endif // SPRITE_H

