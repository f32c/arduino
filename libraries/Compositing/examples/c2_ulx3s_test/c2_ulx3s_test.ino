#include <Compositing.h>
#include "font.h"
#include "rtc.h"
#include "adc.h"
#include "dac.h"
#include "edid.h"
#include "btn.h"
#include "flash.h"
#include "sd.h"
#include "oled.h"
#include "ram.h"

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
  dac_init();
  btn_init();
  oled_init();
}

void loop()
{
  static long counter = 99999;
  const int nlines = 6;
  char line[nlines][256];
  rtc_read(line[0]);
  edid_read(line[1]);
  adc_read(line[2]);
  dac_read(line[3]);
  btn_read(line[4]); // buttons. DIP switches and blink LEDs
  if(line[4][5] == '1') // BTN1 pressed
  {
    rtc_set_clock();
    rtc_set_alarm();
  }
  if(line[4][6] == '2') // BTN2 pressed - shutdown
  {
    volatile uint32_t *simple_out = (uint32_t *)0xFFFFFF10;
    simple_out[0] |= (1<<13); // bit 13 of simple_out is shutdown
  }
  if(line[4][7] == '3') // BTN3 pressed - oled init
    oled_init();
  char flash_str[64], sd_str[64], oled_str[64];
  flash_read(flash_str);
  sd_read(sd_str); // esp32 must be flashed not to access SD card
  oled_read(oled_str);
  sprintf(line[5], "%s %s   %s\n", flash_str, oled_str, sd_str);
  line[6][0]='\0';
  //ram_test(line[5]); // works but too slow, need speedup
  for(int i = 0; i < nlines; i++)
    Serial.print(line[i]);
  cls();
  for(int i = 0; i < nlines; i++)
    prints(line[i]);
  while((*c2.vblank_reg & 0x80) == 0);
  c2.sprite_refresh();
  // delay(1000);
}
