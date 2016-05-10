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
  // put your setup code here, to run once:
  int i;

  #if 1
    for(i = 0; i < SPRITE_MAX && i < 3; i++)
      shape_to_sprite(i, i);
    for(i = 3; i < SPRITE_MAX; i++)
      sprite_clone(i%3, i);
    for(i = 0; i < SPRITE_MAX; i++)
    {
      //shape_to_sprite(1 + (i % 3),i);
      Sprite[i]->x = 20 + (rand() % 600);
      Sprite[i]->y = 20 + (rand() % 400);
      Sprite_speed[i].x = (rand() % 3)-1;
      Sprite_speed[i].y = (rand() % 3)-1;
    }
  #endif

  for(i = 0; i < 480; i++)
  {
    scanlines[0][i] = &blue_green;
    scanlines[1][i] = &reddish_line;
  }
  if(0)
  for(i = 0; i < Sprite[0]->h; i++)
    scanlines[0][i+60] = &(Sprite[0]->line[i]);
  if(0)
  for(i = 0; i < Sprite[1]->h; i++)
    scanlines[0][i+400] = &(Sprite[1]->line[i]);
  if(1)
  for(i = 120; i < 360; i+=1)
    scanlines[0][i] = &reddish_line;

  if(1)
  {
  // enable video fetching after all the
  // pointers have been correctly sat.
    sprite_refresh();
  }
  // prevents random RAM content from
  // causing extensive fetching, and slowing
  // down CPU
  videodisplay_reg = &(scanlines[0][0]);
  // this is needed for vgatext
  // to disable textmode and enable bitmap
  cntrl_reg = 0b11000000; // enable video, yes bitmap, no text mode, no cursor
  // try it with text to "see" what's going
  // on with linked list :)
  //cntrl_reg = 0b11100000; // enable video, yes bitmap, yes text mode, no cursor
}

void loop()
{
  int i;
  static int red_dir = 1, green_dir = -1;
 // return;
  reddish_line.x += red_dir;
  if(reddish_line.x < -20 || reddish_line.x > 80)
    red_dir = -red_dir;

  green_blue.x += green_dir;
  if(green_blue.x < -80 || green_blue.x > 80)
    green_dir = -green_dir;

  if(1)
  {
  for(i = 0; i < SPRITE_MAX; i++)
  {
    Sprite[i]->x += Sprite_speed[i].x;
    Sprite[i]->y += Sprite_speed[i].y;
    if(Sprite[i]->x < -40)
    {
      Sprite_speed[i].x = 1;
      if( (rand()&7) == 0 )
        Sprite_speed[i].y = (rand()%3)-1;
    }
    if(Sprite[i]->x > 640)
    {
      Sprite_speed[i].x = -1;
    }

    if(Sprite[i]->y < 1)
    {
      Sprite_speed[i].y = 1;
      if( (rand()&7) == 0 )
        Sprite_speed[i].x = (rand()%3)-1;
    }
    if(Sprite[i]->y > 460)
      Sprite_speed[i].y = -1;
  }
  while((vblank_reg & 0x80) == 0);
  sprite_refresh();
  while((vblank_reg & 0x80) != 0);
  //delay(400);
  }
  else
  {
  while((vblank_reg & 0x80) == 0);
  while((vblank_reg & 0x80) != 0);
  }
}

