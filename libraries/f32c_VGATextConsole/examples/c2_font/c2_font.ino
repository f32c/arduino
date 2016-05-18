/* Compositing2 sprite example
 * with some moving vertical color stripes.
 */
extern "C"
{
  #include "compositing.h"
  #include "sprite.h"
}

volatile struct compositing_line *scanlines[BUFFERING][480];
#define N_LETTERS 38

void setup()
{
  int i;

  #if 1
  // reset pointers to all sprites
  for(i = 0; i < SPRITE_MAX; i++)
    Sprite[i] = NULL;
  #endif
  // disable video during update
  // prevents random RAM content from
  // causing extensive fetching, and slowing
  // down CPU
  cntrl_reg = 0b00000000;
  videodisplay_reg = NULL;
  #if 1
    for(i = 0; i < SPRITE_MAX && i < N_LETTERS; i++)
      shape_to_sprite(i, i);
    for(i = N_LETTERS; i < SPRITE_MAX; i++)
     sprite_clone(i%N_LETTERS, i);
    // position all sprites in form of a font
    int col=0, row=0;
    for(i = 0; i < SPRITE_MAX; i++)
    {
      Sprite[i]->x = col*16;
      Sprite[i]->y = row*16;
      col++;
      if(col==40)
      {
        col=0;
        row++;
      }
    }
  #endif

  // enable video fetching after all the
  // pointers have been correctly sat.
  sprite_refresh();

  // clear screen
  #if 0
  for(int x = 0; x < 40; x++)
    for(int y = 1; y < 30; y++)
      sprite_link_content(0, x+40*y);
  #endif

  //videodisplay_reg = &(scanlines[0][0]); // sprite refresh will do it
  // this is needed for vgatext
  // to disable textmode and enable bitmap
  cntrl_reg = 0b11000000; // enable video, yes bitmap, no text mode, no cursor
  // try it with text to "see" what's going
  // on with linked list :)
  //cntrl_reg = 0b11100000; // enable video, yes bitmap, yes text mode, no cursor
}

void loop()
{
  static int f = 0;
  static int dbl_buf = 0; // double buffering
return;
  int c = f++;
  while((vblank_reg & 0x80) == 0);
  while((vblank_reg & 0x80) != 0);
  for(int x = 0; x < 40; x++)
    for(int y = 1; y < 30; y++)
      sprite_link_content((c--) % N_LETTERS, x+40*y);
}


