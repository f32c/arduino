/* Compositing2 sprite example
 * with some moving vertical color stripes.
 */
extern "C"
{
  #include "compositing.h"
  #include "sprite.h"
}

extern uint8_t content_blue_green[];
extern uint8_t content_green_blue[];
extern uint8_t content_reddish[];

volatile struct compositing_line blue_green =
{
  NULL, // next line in chain points to previous defined line
  0, // x-position
  32, // number of color pixels to follow:
  content_blue_green,
};

volatile struct compositing_line green_blue =
{
  &blue_green, // last line in chain (has highest priority on display)
  12, // x-position
  32, // number of color pixels to follow
  content_green_blue,
};

volatile struct compositing_line reddish_line =
{
  &green_blue, // links to previous content
  -2, // x-position
  8, // number of color pixels to follow:
  content_reddish,
};

// similar as above but reverse color order: blue-to-green
uint8_t content_blue_green[] =
  {255,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};

// some green-to-blue colors
uint8_t content_green_blue[] =
  {0xE0,0xE0,0xE0,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,255};

uint8_t content_reddish[] =
  {0x80,0xE0,0xE0,0xF0,0xF0,0xE0,0xE0,0x80};

volatile struct compositing_line *scanlines[2][480];

struct sprite_speed 
{
  int x,y;
};
struct sprite_speed Sprite_speed[SPRITE_MAX];

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
    shape_to_sprite(1, 0);
    shape_to_sprite(2, 1);
    shape_to_sprite(3, 2);
    shape_to_sprite(4, 3);
    for(i = 3; i < SPRITE_MAX; i++)
     sprite_clone(i%4, i);
    // position all sprites in form of a font
    int col=0, row=0;
    for(i = 0; i < SPRITE_MAX; i++)
    {
      Sprite[i]->x = col*16;
      Sprite[i]->y = row*16;
      Sprite_speed[i].x = 0;
      Sprite_speed[i].y = 0;
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


