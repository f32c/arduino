/* Compositing2 sprite example
 * with some moving vertical color stripes.
 */
extern "C"
{
  #include "compositing.h"
  #include "sprite.h"
}

volatile struct compositing_line *scanlines[2][480];
#define N_LETTERS 27

void setup()
{
  int i;
  
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
}


