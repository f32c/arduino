#include "compositing.h"
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
  uint16_t x,y; // running coordinates during ascii->pixel conversion
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
  for(bmp = sh->bmp, y = 0; *bmp != NULL; y++, bmp++)
      content_size += strlen(*bmp);
  h = y;
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
  for(bmp = sh->bmp, y = 0; *bmp != NULL; y++, bmp++)
  {
      char *clr;
      // uint8_t *line_content = &(new_content[y[0]*w]); // pointer to current line in content
      new_sprite->line[y].bmp = line_content;
      for(x = 0, clr = *bmp; *clr != 0; x++, clr++) // copy content
        *(line_content++) = color_list[*clr];
      new_sprite->line[y].n = x; // set number of pixels
      #if USE_EXISTING_CONTENT
      // search all already generated sprites. if identical
      // content is found, then link to prevous content, that would
      // save RAM bandwidth
      // todo: we will still be wasting RAM because above
      // we already malloced full size which now we might not need all
      uint8_t *existing_content = NULL; // used to search for same content
      int l; // loops over lines of the existing sprite
      // first search for identical line in the same sprite
      for(l = 0; l < y-1 && existing_content == NULL; l++)
        if( new_sprite->line[l].n >= x && new_sprite->line[l].bmp != NULL) // if existing pixels equal or larger than new content
          if( 0 == memcmp(new_sprite->line[l].bmp, new_sprite->line[y].bmp, x*sizeof(uint8_t)) ) // exact match
            existing_content = new_sprite->line[l].bmp;
      // then search for the same in all previous sprites
      for(j = 0; j < sprite-1 && existing_content == NULL; j++)
      {
        for(l = 0; l < Sprite[j]->h && existing_content == NULL; l++) // loop over existing sprite lines
        {
          if( Sprite[j]->line[l].n >= x && Sprite[j]->line[l].bmp != NULL) // if existing pixels equal or larger than new content
            if( 0 == memcmp(Sprite[j]->line[l].bmp, new_sprite->line[y].bmp, x*sizeof(uint8_t)) ) // exact match
              existing_content = Sprite[j]->line[l].bmp;
        }
      }
      // if found, relink new sprite content to existing content
      if(existing_content)
        new_sprite->line[y].bmp = existing_content; // Sprite[0]->line[0].bmp; // existing_content;
      #endif
  }
  new_sprite->h = y;
  Sprite[sprite] = new_sprite;
}

// create new sprite with content
// linked to existing sprite
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

// change shape of sprite by relinking
// link content of existing original sprite to
// existing clone sprite
void sprite_link_content(int original, int clone)
{
  int i, n, m;

  struct compositing_line *src = Sprite[original]->line;
  struct compositing_line *dst = Sprite[clone]->line;

  // how many lines
  n = Sprite[clone]->h; // number of lines destination sprite has
  #if 0
  m = Sprite[original]->h; // number of lines original sprite has
  if(m < n)
    n = m; // smallest of the two
  #endif
  //memcpy(Sprite[original]->line[0], Sprite[clone]->line[0], sizeof(compositing_line)*n);
  for(i = 0; i < n; i++)
    dst[i].bmp = src[i].bmp;
  // copy size (may be omitted when all are the same size)
  #if 0
  Sprite[clone]->h = n;
  Sprite[clone]->w = Sprite[original]->w;
  #endif
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

  #if BUFFERING == 2
    dbl_buf ^= 1; // alternate to another buffer
  #endif

  // reset all screen lines to blank content
  for(i = 0; i < VGA_Y_MAX; i++)
    scanlines[dbl_buf][i] = NULL;

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

