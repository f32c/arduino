// (c)EMARD
// License=BSD

// Arcade Game of Life
// this example needs 8x8 font

#include <Compositing.h>
Compositing c2;

extern "C"
{
  #include "gol.h"
}

#define MISSILE_MAX 8
#define RESOLUTION_X 640
#define RESOLUTION_Y 480

int16_t ship_x = RESOLUTION_X/2, ship_y = RESOLUTION_Y/2;

struct missile
{
  uint8_t active;
  int16_t x, y;
  int8_t vx, vy;
  uint8_t sprite; // sprite index  
};

struct missile missile[MISSILE_MAX];

// crude initial "malloc" for vram=(tiled graphics) and videomem=(sprites)
// RAM
volatile uint16_t *vram = (volatile uint16_t *) 0x80006000;
// BRAM
// volatile uint16_t *vram = (volatile uint16_t *) 0x40000000;

#define text_addr (*(volatile uint32_t *)0xFFFFFB8C)
#define cntrl_reg (*(volatile uint8_t *)0xfffffb81)
#define finescroll_reg (*(volatile uint8_t *)0xfffffb8b)
#define vblank_reg (*(volatile uint8_t *)0xfffffb87)

// 8x8 font, 640x480 screen has 82*61 tile memory (soft scrolled, but visible 80x60)
#define FONT_HEIGHT 8
#define RANGE_TILE_X (RESOLUTION_X/FONT_HEIGHT+2)
#define RANGE_TILE_Y (RESOLUTION_Y/FONT_HEIGHT+1)

#if FONT_HEIGHT == 8
  #define F_SCROLL_Y_AND 0x70
  #define F_SCROLL_Y_CARRY 0x60
#endif
#if FONT_HEIGHT == 16
  #define F_SCROLL_Y_AND 0xF0
  #define F_SCROLL_Y_CARRY 0xE0
#endif

int more_iterate = 0;

void do_iterate(void)
{
  // return;
  if(more_iterate)
    more_iterate = gol_iterate();
}

// diagonal crawler
void create_crawler(uint16_t px, uint16_t py, uint8_t dir)
{
  int sx, sy;
  sx = dir & 2 ? 1 : -1;
  sy = dir & 1 ? 1 : -1;
  gol_plot(px  ,py  ,1);
  gol_plot(px+1*sx,py  ,1);
  gol_plot(px+2*sx,py  ,1);
  gol_plot(px+2*sx,py+1*sy,1);
  gol_plot(px+1*sx,py+2*sy,1);
}

// straight crawler
void create_crawler_straight(uint16_t px, uint16_t py, uint8_t dir)
{
  int sx, sy;
  sx = dir & 2 ? 1 : -1;
  sy = dir & 1 ? 1 : -1;
  gol_plot(px-1*sx,py-2*sy,1);
  gol_plot(px     ,py-2*sy,1);
  gol_plot(px+1*sx,py-2*sy,1);

  gol_plot(px-1*sx,py-1*sy,1);
  gol_plot(px+2*sx,py-1*sy,1);

  gol_plot(px-1*sx,py     ,1);
  gol_plot(px-1*sx,py+1*sy,1);
  gol_plot(px     ,py+2*sy,1);
  gol_plot(px+2*sx,py+2*sy,1);
}


void create_box(uint16_t px, uint16_t py)
{
  gol_plot(px  ,py  ,1);
  gol_plot(px+1,py+1,1);
  gol_plot(px+1,py  ,1);
  gol_plot(px  ,py+1,1);
}

void erase_box(uint16_t px, uint16_t py)
{
  gol_plot(px-1,py-1,0);
  gol_plot(px  ,py-1,0);
  gol_plot(px+1,py-1,0);
  gol_plot(px-1,py  ,0);
  gol_plot(px  ,py  ,0);
  gol_plot(px+1,py  ,0);
  gol_plot(px-1,py+1,0);
  gol_plot(px  ,py+1,0);
  gol_plot(px+1,py+1,0);
}


void create_explode(uint16_t px, uint16_t py)
{
  gol_plot(px-1,py-1,1);
  gol_plot(px-1,py  ,1);
  gol_plot(px-1,py+1,1);
  gol_plot(px  ,py+1,1);
  gol_plot(px+1,py-1,1);
  gol_plot(px+1,py  ,1);
  gol_plot(px+1,py+1,1);
}


void generate_living_beings(void)
{
  static uint16_t respawn_counter = 0;
  if( ((respawn_counter++) % 256) == 0)
  {
    switch(rand() % 4)
    {
      case 0:
        create_crawler(rand(),rand(), rand());
        break;
      case 1:
        create_crawler_straight(rand(), rand(), rand());
        break;
      case 2:
        create_box(rand(), rand());
        break;
      case 3:
        create_explode(rand(), rand());
        break;
    }
  }
}


void setup()
{
  int x, y;
  int spr;
  int i, j;
  
  c2.init();
  c2.alloc_sprites(50); // max 50 sprites
  // sprite 0: the ship
  spr = c2.sprite_fill_rect(20,20,RGB2PIXEL(0xFF0000));
  c2.Sprite[spr]->x = ship_x-10;
  c2.Sprite[spr]->y = ship_y-10;

  // create 1 missile and additionally clone it 7 times, will make 8 missiles total
  for(i = 0; i < MISSILE_MAX; i++)
  {
    if(i == 0) // sprite 1: the missile
      j = spr = c2.sprite_fill_rect(8,8,RGB2PIXEL(0xFFFF80));
    else
      j = c2.sprite_clone(spr);
    c2.Sprite[j]->x = 20 + i * 10;
    c2.Sprite[j]->y = 0;
    missile[i].active = 0;
    missile[i].sprite = j;
  }
  
  c2.sprite_refresh();
  vram = (volatile uint16_t *)malloc(RANGE_TILE_X*RANGE_TILE_Y*sizeof(uint16_t));
  //videomem = (uint8_t *)malloc(32768);
  text_addr = vram; // video text base address
  cntrl_reg = 0b11100000; // enable text mode, enable bitmap, no cursor
  gol_clear();
  // videodisplay_reg = &(videomem[0]);
  //tspr = (struct thin_sprites *) videomem;
  //create_box(50,20);
  //create_crawler(50,30,1);
}

// xc,yc -- current screen center coordinates in gol cell array
void fly_missiles(uint16_t xc, uint16_t yc, uint8_t fsx, uint8_t fsy)
{
  int i;

  for(i = 0; i < MISSILE_MAX; i++)
  {
    if(missile[i].active)
    {
      int16_t mx = missile[i].x + missile[i].vx;
      int16_t my = missile[i].y + missile[i].vy;
      missile[i].x = mx;
      missile[i].y = my;
      c2.Sprite[missile[i].sprite]->x = mx-4; // center of sprite
      c2.Sprite[missile[i].sprite]->y = my-4;
      // clipping - out of screen will de-activate missile
      if(mx < 0 || my < 0 || mx > RESOLUTION_X || my > RESOLUTION_Y)
        missile[i].active = 0;
      // collision detection
      // convert missile screen position to gol cell position
      uint16_t gx = xc + (mx-12+fsx-RESOLUTION_X/2)/8;
      uint16_t gy = yc + (my-4+fsy-RESOLUTION_Y/2)/8;
      struct gol gol_cell = gol_peek(gx, gy);
      if(gol_cell.v != 0 || gol_cell.n != 0)
      {
        erase_box(gx, gy);
        missile[i].active = 0;
      }
    }
    else
    {
      // activate inactive missiles randomly
      missile[i].x = ship_x;
      missile[i].y = ship_y;
      missile[i].vx = 16*(rand()%3-1);
      missile[i].vy = 16*(rand()%3-1);
      if(missile[i].vx != 0 || missile[i].vy != 0)
        missile[i].active = 1;
    }
  }
}


void loop()
{
  int i;
  static int8_t scroll_x = -1, scroll_y = -1;
  static int8_t dir = 0;
  static uint16_t change_direction_counter = 0;
  static uint16_t x0=0, y0=0; // viewport offset
  static uint16_t xc, yc; // screen center
  static uint8_t fscroll_x = 0, fscroll_y = 0;

  //static int16_t ship_x = 320, ship_y = 240, ship_vx = 1, ship_vy = 1;
  // calculate the "center" in cell array
  // this is a cell array address that corresponds
  // to currently visible center of the screen
  xc = (x0 + RANGE_TILE_X/2) & (RANGE_X-1);
  yc = (y0 + RANGE_TILE_Y/2) & (RANGE_Y-1);
  // generate "living" beings
  // generate_living_beings();
  if( ((change_direction_counter++) % 64) == 0)
  {
    scroll_x = (rand() % 3)-1;
    scroll_y = (rand() % 3)-1;
    //scroll_x = 0;
    //scroll_y = 0;
    //create_crawler(xc+10, yc+20, rand());
    if((rand()&1) == 0)
      create_crawler(xc-32+rand()%64, yc-32+rand()%64, rand());
    else
      create_crawler_straight(xc-32+rand()%64, yc-32+rand()%64, rand());
    #if 0
    // shoooting targets to adjust missile center
    create_box(xc-20, yc+0);
    create_box(xc+20, yc+0);
    create_box(xc-1, yc+20);
    create_box(xc-1, yc-20);
    #endif
  }
  fly_missiles(xc,yc, (fscroll_x)&7, (fscroll_y>>4)&7);
  // wait for vertical video blank
  // busy waiting, calculating next
  // GoL generation
  more_iterate = 1;
  while((vblank_reg & 0x80) != 0)
    do_iterate();
  while((vblank_reg & 0x80) == 0)
    do_iterate();
  // soft scroll
  if(scroll_x < 0)
  {
    fscroll_x = (fscroll_x += 0x02) & 0x07;
    if(fscroll_x == 0x00)
      x0++;
  }
  if(scroll_x > 0)
  {
    fscroll_x = (fscroll_x -= 0x02) & 0x07;
    if(fscroll_x == 0x06)
      x0--;
  }
  if(scroll_y < 0)
  {
    fscroll_y = (fscroll_y += 0x20) & F_SCROLL_Y_AND;
    if(fscroll_y == 0x00)
      y0++;
  }
  if(scroll_y > 0)
  {
    fscroll_y = (fscroll_y -= 0x20) & F_SCROLL_Y_AND;
    if(fscroll_y == F_SCROLL_Y_CARRY)
      y0--;
  }
  finescroll_reg = fscroll_x | fscroll_y;
  // display GoL memory
  {
    uint32_t ix, iy, x,y;
    for(iy = 0; iy < RANGE_TILE_Y; iy++)
    {
      y = (iy+y0) & (RANGE_Y-1);
      for(ix = 0; ix < RANGE_TILE_X-1; ix++)
      {
        x = (ix+x0) & (RANGE_X-1);
        // vram[x+y*81] = 0x0241;
        vram[ix+iy*RANGE_TILE_X] = (cell[y][x].n << 12) | 0x0F00 | (cell[y][x].v != 0 ? '+' : ' ');
      }
    }
  }
  c2.sprite_refresh();
}

