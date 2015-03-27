//
// ULX2S sprites demo
//
// Pressing a button toggles between drawing random
// lines, circles, dots, rectangles and text
//


extern "C" {
#include <fb.h>
#include <sprite.h>
#include <io.h>
//uint32_t random();
}

int spr0_x = 210, spr0_y = 65, spr1_x, spr1_y;
int spr0_sizex, spr0_sizey;
int spr1_sizex, spr1_sizey;
int spr1_dirx, spr1_diry;

void setup() {
  // put your setup code here, to run once:
  fb_set_mode(1);
  spr_load(0, "c:/demo/logos/ulx2s_top.jpg", 0);
  spr_load(1, "c:/demo/logos/unizg.jpg", 0);
  // black is transparent
  spr_trans(0, 0);
  spr_trans(1, 0);
  spr_size(0, &spr0_sizex, &spr0_sizey);
  spr_size(1, &spr1_sizex, &spr1_sizey);
}

void loop() {
  // put your main code here, to run repeatedly:
  int key;
 
  INB(key, IO_PUSHBTN);
  if (key & BTN_UP && spr0_y > 0)
    spr0_y--;
  if (key & BTN_LEFT && spr0_x > 0)
    spr0_x--;
  if (key & BTN_DOWN && spr0_y < 288 - spr0_sizey)
    spr0_y++;
  if (key & BTN_RIGHT && spr0_x < 512 - spr0_sizex)
    spr0_x++;
  
  if (spr1_y == 0)
    spr1_diry = 1;
  if (spr1_y == 288 - spr1_sizey)
    spr1_diry = -1;
  if (spr1_x == 0)
    spr1_dirx = 1;
  if (spr1_x == 512 - spr1_sizex)
    spr1_dirx = -1;
  spr1_x += spr1_dirx;
  spr1_y += spr1_diry;
    
  if ((key & BTN_CENTER) == 0)
    spr_put(0, spr0_x, spr0_y);
  spr_put(1, spr1_x, spr1_y);
}
