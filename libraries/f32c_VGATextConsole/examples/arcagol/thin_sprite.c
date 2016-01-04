#include "thin_sprite.h"
#include "shapes.h"
#include <stdlib.h>
struct thin_sprites *tspr;
struct sprite sprite[20];

// move tiled area from inclusive (x1,y1) to (x2,y2)
// horizontally by x pixels 
void thin_move(int tx1, int ty1, int tx2, int ty2, int x)
{
  int i, j;
  for(j = ty1; j <= ty2; j++)
    for(i = tx1; i <= tx2; i++)
      tspr[i/2+10*j].x[i&1] = x;
}

// return address of x/y
inline volatile uint8_t* bmp_addr(int16_t x, int16_t y)
{
  return &(videomem[(y * (F32C_VGA_WIDTH + 4*((F32C_VGA_WIDTH/4)/(F32C_VGA_COMPOSITING-1)) )) + x + 4*(1+(x/4)/((F32C_VGA_COMPOSITING-1)))]);
}

// the most basic function, set a single pixel
void bmp_plot(int16_t x, int16_t y, uint8_t color) {
  //uint8_t mem;
  volatile uint8_t *ptr;
  if ((x < 0) || (x >= F32C_VGA_WIDTH) || (y < 0) || (y >= F32C_VGA_HEIGHT))
    return;
  ptr = bmp_addr(x,y);
  *ptr = color;
}

void bmp_clear(void)
{
  int i;
  for(i = 0; i < F32C_VGA_SIZE; i++)
    videomem[i] = 0;
}

// initial draw of the sprite from
// its ascii art base to the plot
// n - number of the sprite
// s - index of the shape to initialize this sprite
// r - rotate n*90 deg
void bmp_shape_draw(uint16_t ix, uint16_t iy, uint8_t in, uint8_t is, uint8_t r)
{
    uint16_t x[0],y[0];
    uint16_t *rx, *ry;
    int16_t sx, sy;
    uint8_t color_list[256];
    struct shape *sh = &(shapes[is]);
    struct charcolors *chc;
    char **bmp; // bitmap: array of strings
    // fill the color array to speed up
    for(chc = sh->colors; chc->c != 0; chc++) // read until we don't hit 0
      color_list[chc->c] = chc->color;
    sx = sy = 1;
    if ( (r & 2) != 0 )
    {
      ix += 31;
      iy += 31;
      sx = sy = -1;
    }
    if( (r & 1) == 0 )
    {
      rx = x;
      ry = y;
    }
    else
    {
      rx = y;
      ry = x;
    }
    for(bmp = sh->bmp, y[0] = 0; y[0] < 256 && *bmp != NULL; y[0]++, bmp++)
    {
      char *clr;
      for(x[0] = 0, clr = *bmp; x[0] < 256 && *clr != 0; x[0]++, clr++)
        bmp_plot(ix+sx*rx[0],iy+sy*ry[0],color_list[*clr]);
    }
}

// "grab" shape from rectangular bitmap area into the sprite
// x-direction 32-pixel quantized
// y-direction  1-pixel quantized
void bmp_grab_sprite(struct sprite *s, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  // quantize X to 32-pixels
  x1 &= ~31;
  x2 &= ~31;
  s->x0 = x1;
  s->y0 = y1;
  s->x1 = x2;
  s->y1 = y2;
  s->ts = (struct thin_sprites *)(bmp_addr(x1,y1)-4);
  s->x = x1;
  s->y = y1;
}

// move sprite to a new location
void sprite_position(struct sprite *s, int16_t x, int16_t y)
{
  //int iy;
  int x0, my;
  struct thin_sprites *source, *target; // bitmap pointers
  uint32_t *src, *dst, *s0, *d0;
  int16_t ymove = y - s->y;
  int16_t ysize = s->y1 - s->y0 + 1;
  int32_t line_step = 10*(sizeof(struct thin_sprites)/sizeof(uint32_t));
  uint8_t word_count;
  if(ymove != 0)
  {
    source = s->ts;
    target = source + 10*(y - s->y); // 10 thin sprite structs per scan line
    word_count = (1 + ((s->x1 - s->x0)>>5) ) << 3;
    src = (uint32_t *)&(source->color[0][0]);
    dst = (uint32_t *)&(target->color[0][0]);
    // y position changed
    if(ymove < 0)
    {
      int i, j;
      // move up
      // copy
      for(i = ysize; i-- != 0;)
      {
        s0 = src;
        d0 = dst;
        for(j = word_count; j-- != 0;)
          *(d0++) = *(s0++);
        dst += line_step;
        src += line_step;
      }
      // erase remaining of the source
      // which wasn't overwritten with target
      for(i = -ymove < ysize ? -ymove : ysize; i >= 0; i--)
      {
        src -= line_step;
        s0 = src;
        for(j = word_count; j-- != 0;)
          *(s0++) = 0;
      }
    }
    else
    { // ymove > 0
      int i,j;
      int endsize = 10*sizeof(struct thin_sprites)/sizeof(uint32_t) * (ysize-1);
      // move down
      src += endsize;
      dst += endsize;
      for(i = ysize; i-- != 0;)
      {
        s0 = src;
        d0 = dst;
        for(j = word_count; j-- != 0;)
          *(d0++) = *(s0++);
        dst -= line_step;
        src -= line_step;
      }
      // erase remaining of the source
      // which wasn't overwritten with target
      for(i = ymove < ysize ? ymove : ysize; i-- != 0;)
      {
        src += line_step;
        s0 = src;
        for(j = word_count; j-- != 0;)
          *(s0++) = 0;
      }
    }
    s->ts = target;
    s->y = y;
    s->y0 += ymove;
    s->y1 += ymove;
  }
  if(x != s->x)
  {
    // x position changed
    // move thin sprite (specify relative offset)
    thin_move((s->x0)>>5, s->y0, (s->x1)>>5, s->y1, x - s->x0);
    s->x = x;
  }
}

