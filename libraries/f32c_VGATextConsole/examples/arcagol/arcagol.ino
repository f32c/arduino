// this example needs 8x8 font
extern "C"
{
  #include "gol.h"
}
// crude malloc for video text memory
// RAM
// volatile uint16_t *vram = (volatile uint16_t *) 0x80020000;
// BRAM
volatile uint16_t *vram = (volatile uint16_t *) 0x40000000;

#define text_addr (*(volatile uint32_t *)0xFFFFFB8C)
#define cntrl_reg (*(volatile uint8_t *)0xfffffb81)
#define finescroll_reg (*(volatile uint8_t *)0xfffffb8b)
#define vblank_reg (*(volatile uint8_t *)0xfffffb87)

// 8x8 font, 82*31 screen memory (soft scrolled, but visible 80x30)
#define RANGE_TILE_X 82
#define RANGE_TILE_Y 61

int more_iterate = 0;

void do_iterate(void)
{
//  return;
  if(more_iterate)
    more_iterate = gol_iterate();
}

// diagonal crawler
void create_crawler(unsigned int px, unsigned int py, uint8_t dir)
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
void create_crawler_straight(unsigned int px, unsigned int py, uint8_t dir)
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

void create_box(unsigned int px, unsigned int py)
{
  gol_plot(px  ,py  ,1);
  gol_plot(px+1,py+1,1);
  gol_plot(px+1,py  ,1);
  gol_plot(px  ,py+1,1);
}

void create_explode(unsigned int px, unsigned int py)
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


void setup() {
  text_addr = vram; // video text base address
  cntrl_reg = 0xA0; // enable text mode, no bitmap, no cursor
  gol_clear();
  //create_box(50,20);
  //create_crawler(50,30,1);
}

void loop() {
  int i;
  static int8_t scroll_x = -1, scroll_y = -1;
  static int8_t dir = 0;
  static uint16_t change_direction_counter = 0;
  static uint16_t x0=0, y0=0; // viewport offset
  static uint16_t xc, yc; // screen center
  static uint8_t fscroll_x = 0, fscroll_y = 0;
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
    create_crawler(xc, yc, rand());
  }
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
    fscroll_y = (fscroll_y += 0x20) & 0x70;
    if(fscroll_y == 0x00)
      y0++;
  }
  if(scroll_y > 0)
  {
    fscroll_y = (fscroll_y -= 0x20) & 0x70;
    if(fscroll_y == 0x60)
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
        vram[ix+iy*RANGE_TILE_X] = (cell[y][x].n << 12) | 0x0F00 | (cell[y][x].v != 0 ? '*' : ' ');
      }
    }
  }
}

