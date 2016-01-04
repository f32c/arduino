#ifndef SPRITE_H
#define SPRITE_H
/* thin sprte driver */
#include <stdint.h>

#define F32C_VGA_WIDTH  640
#define F32C_VGA_HEIGHT 480
#define F32C_VGA_COMPOSITING 17
#define F32C_VGA_SIZE ((F32C_VGA_WIDTH + 4*((F32C_VGA_WIDTH/4)/(F32C_VGA_COMPOSITING-1)) ) * (F32C_VGA_HEIGHT+8))

struct thin_sprites
{
  int16_t x[2]; // horizontal offset
  uint8_t color[2][32]; // pixel color
};

struct charcolors
{
  char c; // letter indicating the color (0 to end of list)
  uint8_t color; // color value bits: RRRGGGBB
};

// struct used to draw sprite in C
struct shape
{
  struct charcolors *colors; // array of colors
  char **bmp; // the bitmap, null, terminated
  uint8_t w,h; // width x-size, height y-size
};

struct sprite
{
  struct thin_sprites *ts; // the pointer to upper thin sprite in memory
  struct shape *shape; // pointer to the ascii-art shape
  int16_t x0, y0, x1, y1; // actual coordinates of bitmap (when compositing offset = 0)
  int16_t x, y; // real coordinates on the screen this sprite is currently shown
};

extern struct thin_sprites *tspr;
extern struct sprite sprite[]; // allocate 20 sprites

extern volatile uint8_t *videomem;
void thin_move(int tx1, int ty1, int tx2, int ty2, int x);
void bmp_plot(int16_t x, int16_t y, uint8_t color);
void bmp_clear(void);
void bmp_shape_draw(uint16_t ix, uint16_t iy, uint8_t in, uint8_t is, uint8_t r);
void bmp_grab_sprite(struct sprite *s, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void sprite_position(struct sprite *s, int16_t x, int16_t y);

#endif

