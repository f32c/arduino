#include <Compositing.h>
#include "font.h"
#include "rtc.h"
#include "adc.h"
#include "edid.h"
#include "btn.h"

#define N_LETTERS (sizeof(Font)/sizeof(Font[0]))

Compositing c2;

const int Cols = 40;
const int Rows = 30;
int Cursor_col = 0, Cursor_row = 0;

void cls()
{
  for(int row = 0; row < Rows; row++)
    for(int col = 0; col < Cols; col++)
    {
      int i = N_LETTERS + row*Cols + col;
      c2.sprite_link_content('@'-'@', i);
    }
  Cursor_col = Cursor_row = 0;
}

void prints(char *a)
{
  for(;*a != '\0'; a++)
  {
    int i = N_LETTERS + Cursor_row*Cols + Cursor_col;
    uint8_t letter = 0; // space is default
    char c = *a;
    if(c == '\n')
    {
      Cursor_col = 0;
      Cursor_row = (Cursor_row + 1) % Rows;
      continue;
    }
    c = toupper(c);
    if(c >= ' ' && c <= '_')
      letter = c - ' ';
    c2.sprite_link_content(letter, i);
    if(++Cursor_col >= Cols)
    {
      Cursor_col = 0;
      Cursor_row = (Cursor_row + 1) % Rows;
    }
  }  
}

// initialize compositing2 text using sprites
void video_init()
{
  int i;
  int unique_sprites;
  c2.init();
  c2.alloc_sprites(N_LETTERS+Rows*Cols); // it sets c2.sprite_max
  *c2.videobase_reg = NULL; // disable video during update
  *c2.cntrl_reg = 0;
  #if 1
    for(i = 0; i < c2.sprite_max && i < N_LETTERS; i++)
      c2.shape_to_sprite(&(Font[i]));
    unique_sprites = c2.n_sprites;
    // unique sprites containing alphabet are not displayed, set them off-screen
    for(i = 0; i < unique_sprites; i++)
    {
      c2.Sprite[i]->x = -100;
      c2.Sprite[i]->y = -100;
    }
    // position cloned sprites on display
    // i = N_LETTERS;
    for(int row = 0; row < Rows; row++)
      for(int col = 0; col < Cols; col++)
    {
      c2.sprite_clone(0); // by default clone 0-sprite which is space
      int n = N_LETTERS + row*Cols + col;
      c2.Sprite[n]->x = col*16;
      c2.Sprite[n]->y = row*16;
      i++;
    }
  #endif
  // cls();
  c2.sprite_refresh();
  // this is needed for vgatext
  // to disable textmode and enable bitmap
  *c2.cntrl_reg = 0b11000000; // enable video, yes bitmap, no text mode, no cursor
}

void setup()
{
  video_init();
  rtc_init();
  adc_init();
  btn_init();
  rtc_set_clock();
  rtc_set_alarm();
}

void loop()
{
  static long counter = 99999;
  const int nlines = 4;
  char line[nlines][256];
  char edid[128];
  int edid_count = edidreadbytes(edid);
  uint8_t edid_checksum = edidchecksum(edid);
  rtc_read(line[0]);
  edid_read(line[1]);
  adc_read(line[2]);
  btn_read(line[3]);
  for(int i = 0; i < nlines; i++)
    Serial.print(line[i]);
  cls();
  for(int i = 0; i < nlines; i++)
    prints(line[i]);
  while((*c2.vblank_reg & 0x80) == 0);
  c2.sprite_refresh();
  // delay(1000);
}

