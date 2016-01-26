#include "compositing.h"
#include "sprite.h"
#include "shape.h"
#include <string.h>

struct sprite *Sprite[SPRITE_MAX]; // global pointer array to sprites

char content_blank[] =
  {0,0,0,0};
struct compositing_line blank_line =
{
  NULL, // links to previous content
  320, // x-position
  4, // number of color pixels to follow:
  content_blank,
};

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
  uint8_t *new_content, *line_content; // malloc'd contiguous space for the sprite
  struct charcolors *chc;
  struct sprite *spr = &(Sprite[sprite]), *new_sprite;
  struct shape *sh = &(Shape[shape]); // shape to read from

  // fill the color array to speed up
  for(chc = sh->colors; chc->c != 0; chc++) // read until we don't hit 0
    color_list[chc->c] = chc->color;
  // 1st pass read ascii art - determine content size
  content_size = 0;
  for(bmp = sh->bmp, y[0] = 0; y[0] < 32 && *bmp != NULL; y[0]++, bmp++)
      content_size += strlen(*bmp);
  h = y[0];
  new_content = (uint8_t *)malloc(content_size);
  sprite_size = sizeof(struct sprite)+h*(sizeof(struct compositing_line));
  new_sprite = (struct sprite *)malloc(sprite_size);
  new_sprite->x = ix;
  new_sprite->y = iy;
  new_sprite->w = w; // max sprite width (maybe unused)
  new_sprite->h = h;
  for(i = 0; i < h; i++)
  {
    new_sprite->line[i].next = NULL;
    new_sprite->line[i].x = ix;
    // new_sprite->line[i].n = w;
  }
  // 2nd pass read ascii-art data and write color pixel content
  line_content = new_content;
  for(bmp = sh->bmp, y[0] = 0; y[0] < 32 && *bmp != NULL; y[0]++, bmp++)
  {
      char *clr;
      // uint8_t *line_content = &(new_content[y[0]*w]); // pointer to current line in content
      new_sprite->line[y[0]].bmp = line_content;
      for(x[0] = 0, clr = *bmp; x[0] < 32 && *clr != 0; x[0]++, clr++)
      {
        *(line_content++) = color_list[*clr];
        //bmp_plot(ix+sx*rx[0],iy+sy*ry[0],color_list[*clr]);
      }
      new_sprite->line[y[0]].n = x[0];
  }
  new_sprite->h = y[0];
  Sprite[sprite] = new_sprite;
}

void sprite_clone(int original, int clone)
{
  struct sprite *orig = Sprite[original];
  struct sprite *clon;
  uint32_t sprite_size;

  sprite_size = sizeof(struct sprite) + (orig->h) * sizeof(struct compositing_line);
  clon = (struct sprite *)malloc(sprite_size);
  memcpy(clon, orig, sprite_size);
  Sprite[clone] = clon;
}

void sprite_position(int sprite, int x, int y)
{
  Sprite[sprite]->x = x;
  Sprite[sprite]->y = y;
}

// refresh compositing linked list after changing x/y positions
// to avoid flickering, this must be called during video blank period
void sprite_refresh(void)
{
  int i, j, n = SPRITE_MAX;
  static int dbl_buf = 0; // double buffering

  // dbl_buf ^= 1; // alternate to another buffer

  // reset all screen lines to blank content
  for(i = 0; i < VGA_Y_MAX; i++)
    scanlines[dbl_buf][i] = &blank_line;

  // now link all sprites, insering them into the linked list
  for(i = 0; i < n; i++) // loop over all sprites
  {
    // cache x/y-offset of the sprite
    int x = Sprite[i]->x;
    int y = Sprite[i]->y;
    for(j = 0; j < Sprite[i]->h; j++) // loop over all hor.lines of the sprite
    {
      Sprite[i]->line[j].x = x;
      // insert sprite lines into the linked list of scan lines
      Sprite[i]->line[j].next = scanlines[dbl_buf][y+j];
      scanlines[dbl_buf][y+j] = &(Sprite[i]->line[j]);
    }
  }
  videodisplay_reg = &(scanlines[dbl_buf][0]);
}

