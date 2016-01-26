#include "sprite.h"
#include "shape.h"
#include <string.h>

struct sprite *Sprite[SPRITE_MAX]; // global pointer array to sprites

// convert shape into sprite
// addressed by index 
// this function will use malloc()
// will allocate content for the sprite
void shape_to_sprite(int shape, int sprite)
{
  char *pixels;
  int i,j;
  int ix=320,iy=200; // initial sprite position on screen
  int w=32,h=32; // width and height of the sprite
  int sprite_size; // how much to malloc
  uint8_t color_list[256];
  char **bmp; // bitmap: array of strings
  uint16_t x[0],y[0]; // running coordinates during ascii->pixel conversion
  int content_size;
  uint8_t *new_content; // malloc'd contiguous space for the sprite
  struct charcolors *chc;
  struct sprite *spr = &(Sprite[sprite]), *new_sprite;
  struct shape *sh = &(Shape[shape]); // shape to read from

  // fill the color array to speed up
  for(chc = sh->colors; chc->c != 0; chc++) // read until we don't hit 0
    color_list[chc->c] = chc->color;

  content_size = w*h;
  new_content = (uint8_t *)malloc(content_size);
  sprite_size = sizeof(struct sprite)+h*(sizeof(struct compositing_line));
  new_sprite = (struct sprite *)malloc(sprite_size);
  new_sprite->x = ix;
  new_sprite->y = iy;
  new_sprite->w = w;
  new_sprite->h = h;
  for(i = 0; i < h; i++)
  {
    new_sprite->line[i].next = NULL;
    new_sprite->line[i].x = ix;
    new_sprite->line[i].n = w;
    //for(j = 0; j < w; j++)
    //  new_sprite->line[i].pixel[j] = 0; // transparent
      // new_sprite->line[i].pixel[j] = i-j; // block of 45 deg blue-to-red-colors
  }
  // read ascii-art data and write color pixel content
  for(bmp = sh->bmp, y[0] = 0; y[0] < 32 && *bmp != NULL; y[0]++, bmp++)
  {
      char *clr;
      uint8_t *line_content = &(new_content[y[0]*w]); // pointer to current line in content
      new_sprite->line[y[0]].bmp = line_content;
      for(x[0] = 0, clr = *bmp; x[0] < 32 && *clr != 0; x[0]++, clr++)
      {
        line_content[x[0]] = color_list[*clr];
        //bmp_plot(ix+sx*rx[0],iy+sy*ry[0],color_list[*clr]);
      }
  }
  new_sprite->h = y[0];
  Sprite[sprite] = new_sprite;
}

