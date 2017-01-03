#include "Compositing.h"

void Compositing::init()
{
  int i;

  *videobase_reg = (uint32_t) NULL; // NULL pointer
  scanlines = (struct compositing_line **)malloc(VGA_Y_MAX * sizeof(struct compositing_line *));
  for(i = 0; i < VGA_Y_MAX; i++)
    scanlines[i] = NULL;
  *videobase_reg = (uint32_t)&(scanlines[0]);
  n_sprites = 0;
  return;
}

void Compositing::alloc_sprites(int n)
{
  Sprite = (struct sprite **)malloc(n * sizeof(struct sprite));
  sprite_max = n;
}

// convert shape into sprite
// addressed by index 
// this function will use malloc()
// will allocate content for the sprite
// sh: shape to read from
// sprite: sprite to create
int Compositing::shape_to_sprite(struct shape *sh)
{
  int i,j;
  int ix=VGA_X_MAX/2,iy=VGA_Y_MAX/2; // initial sprite position on screen
  int w=32,h=32; // width and height of the sprite
  int sprite_size; // how much to malloc
  pixel_t color_list[256];
  char **bmp; // bitmap: array of strings
  uint16_t x,y; // running coordinates during ascii->pixel conversion
  int content_size;
  pixel_t *new_content, *line_content; // malloc'd contiguous space for the sprite
  struct charcolors *chc;
  // struct sprite *spr = &(Sprite[sprite]), *new_sprite;
  struct sprite *new_sprite;
  // struct shape *sh = &(Shape[shape]); // shape to read from

  // fill the color array to speed up
  for(chc = sh->colors; chc->c != 0; chc++) // read until we don't hit 0
    color_list[(int)(chc->c)] = chc->color;
  // 1st pass read ascii art - determine content size
  content_size = 0;
  for(bmp = sh->bmp, y = 0; *bmp != NULL; y++, bmp++)
      content_size += strlen(*bmp);
  h = y;
  new_content = (pixel_t *)malloc(content_size*sizeof(pixel_t));
  sprite_size = sizeof(struct sprite)+h*(sizeof(struct compositing_line));
  new_sprite = (struct sprite *)malloc(sprite_size);
  new_sprite->x = ix;
  new_sprite->y = iy;
  new_sprite->w = w; // max sprite width (maybe unused)
  new_sprite->h = h;
  new_sprite->ha = h; // allocated size
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
        *(line_content++) = color_list[(int)*clr];
      new_sprite->line[y].n = x-1; // set number of pixels
      #if USE_EXISTING_CONTENT
      // search all already generated sprites. if identical
      // content is found, then link to prevous content, that would
      // save RAM bandwidth
      // todo: we will still be wasting RAM because above
      // we already malloced full size which now we might not need all
      pixel_t *existing_content = NULL; // used to search for same content
      int l; // loops over lines of the existing sprite
      // first search for identical line in the same sprite
      for(l = 0; l < y-1 && existing_content == NULL; l++)
        if( new_sprite->line[l].n > x && new_sprite->line[l].bmp != NULL) // if existing pixels equal or larger than new content
          if( 0 == memcmp(new_sprite->line[l].bmp, new_sprite->line[y].bmp, x*sizeof(pixel_t)) ) // exact match
            existing_content = new_sprite->line[l].bmp;
      // then search for the same in all previous sprites
      for(j = 0; j < n_sprites && existing_content == NULL; j++)
      {
        for(l = 0; l < Sprite[j]->h && existing_content == NULL; l++) // loop over existing sprite lines
        {
          if( Sprite[j]->line[l].n > x && Sprite[j]->line[l].bmp != NULL) // if existing pixels equal or larger than new content
            if( 0 == memcmp(Sprite[j]->line[l].bmp, new_sprite->line[y].bmp, x*sizeof(pixel_t)) ) // exact match
              existing_content = Sprite[j]->line[l].bmp;
        }
      }
      // if found, relink new sprite content to existing content
      if(existing_content)
        new_sprite->line[y].bmp = existing_content; // Sprite[0]->line[0].bmp; // existing_content;
      #endif
  }
  new_sprite->h = y;
  i = n_sprites;
  Sprite[n_sprites++] = new_sprite;
  return i;
}

int Compositing::sprite_add(struct sprite *s)
{
  int i = n_sprites;
  Sprite[n_sprites++] = s;
  return i;
}

// create new sprite with content
// linked to existing sprite
int Compositing::sprite_clone(int original)
{
  struct sprite *orig = Sprite[original];
  struct sprite *clon;
  uint32_t sprite_size;
  int i;

  sprite_size = sizeof(struct sprite) + (orig->h) * sizeof(struct compositing_line);
  clon = (struct sprite *)malloc(sprite_size);
  memcpy(clon, orig, sprite_size);
  i = n_sprites;
  Sprite[n_sprites++] = clon;
  return i;
}

// make sprite as the rectangular area of some color
// by vertically extruding single coloured line
// only w pixels of content will be allocated
// in one contiguous memory block of pixel content
// representing one horizontal lines. All other lines
// in the sprite will refer to the same content
int Compositing::sprite_fill_rect(int w, int h, pixel_t color)
{
  int i;
  uint32_t sprite_size;
  struct sprite *spr;
  pixel_t *content = (pixel_t *)malloc(w * sizeof(pixel_t));

  for(i = 0; i < w; i++)
    content[i] = color;
  sprite_size = sizeof(struct sprite) + h * sizeof(struct compositing_line);

  spr = (struct sprite *)malloc(sprite_size);
  spr->x = 0;
  spr->y = 0;
  spr->w = w;
  spr->h = h;
  spr->ha = h;
  for(i = 0; i < h; i++)
  {
    spr->line[i].next = NULL;
    spr->line[i].x = spr->x;
    spr->line[i].n = w-1;
    spr->line[i].bmp = content;
  }
  return sprite_add(spr);
}

// user should allocate bitmap, sprite will
// be created to point its content to user's allocated bitmap
// content will not be allocated and copied but just reused
// only sprite metadata (vertical lines) will be allocated
// when making content, note that its location in memory
// and its width must be 32-bit word aligned in case of 8bpp or 16bpp
int Compositing::sprite_from_bitmap(int w, int h, pixel_t *bmp)
{
  int i;
  uint32_t sprite_size;
  struct sprite *spr;

  sprite_size = sizeof(struct sprite) + h * sizeof(struct compositing_line);

  spr = (struct sprite *)malloc(sprite_size);
  spr->x = 0;
  spr->y = 0;
  spr->w = w;
  spr->h = h;
  spr->ha = h;
  for(i = 0; i < h; i++)
  {
    spr->line[i].next = NULL;
    spr->line[i].x = spr->x;
    spr->line[i].n = w-1;
    spr->line[i].bmp = bmp;
    bmp += w;
  }
  return sprite_add(spr);
}

// change shape of sprite by relinking
// link content of existing original sprite to
// existing clone sprite
void Compositing::sprite_link_content(int original, int clone)
{
  int i, n;
  int m;

  struct compositing_line *src = Sprite[original]->line;
  struct compositing_line *dst = Sprite[clone]->line;

  // how many lines
  n = Sprite[clone]->ha; // number of lines destination sprite has allocated
  #if 1
  m = Sprite[original]->h; // number of lines original sprite has
  if(m < n)
    n = m; // smallest of the two
  #endif
  //memcpy(Sprite[original]->line[0], Sprite[clone]->line[0], sizeof(compositing_line)*n);
  for(i = 0; i < n; i++)
  {
    dst[i].bmp = src[i].bmp;
    dst[i].n = src[i].n;
  }
  // copy size (may be omitted when all are the same size)
  #if 1
  Sprite[clone]->h = n;
  Sprite[clone]->w = Sprite[original]->w;
  #endif
}

void Compositing::sprite_position(int sprite, int x, int y)
{
  Sprite[sprite]->x = x;
  Sprite[sprite]->y = y;
}

// refresh compositing linked list after changing x/y positions
// to avoid flickering, this must be called during video blank period
// display sprites from m to n
void Compositing::sprite_refresh(int m, int n)
{
  int i, j;

  // reset all screen lines to blank content
  for(i = 0; i < VGA_Y_MAX; i++)
    scanlines[i] = NULL;

  // now link all sprites, insering them into the linked list
  for(i = m; i < n; i++) // loop over all sprites
  {
    // cache x/y-offset of the sprite
    int x = Sprite[i]->x;
    int y = Sprite[i]->y;
    int h = Sprite[i]->h;
    // calculate clipping
    int j0 = 0;
    int j1 = h;
    int jy;
    if(y < 0)
      j0 = -y;
    if(y + h > VGA_Y_MAX)
      j1 = VGA_Y_MAX - y;
    for(j = j0, jy = j0+y; j < j1; j++, jy++) // loop over all visible hor.lines of the sprite
    {
      struct compositing_line **sl = &(scanlines[jy]);
      Sprite[i]->line[j].x = x;
      // insert sprite lines into the linked list of scan lines
      Sprite[i]->line[j].next = *sl;
      *sl = &(Sprite[i]->line[j]);
    }
  }
  *videobase_reg = (uint32_t) &(scanlines[0]);
}

void Compositing::sprite_refresh(int m)
{
  sprite_refresh(m, n_sprites);
}

void Compositing::sprite_refresh()
{
  sprite_refresh(0, n_sprites);
}
