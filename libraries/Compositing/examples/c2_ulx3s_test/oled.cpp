#include <Arduino.h>
#include "spi.h"

enum 
{
  C_OLED_NOP1 = 0xBC,
  C_OLED_NOP2 = 0xBD, // delay nop
  C_OLED_NOP3 = 0xE3,
  C_OLED_SET_DISPLAY_OFF = 0xAE, // 10101110
  C_OLED_SET_REMAP_COLOR = 0xA0,
  C_OLED_ULX3S_REMAP = 0b00100010, // A[7:6] = 00; 256 color. A[7:6] = 01; 65k color format rotation for ULX3S, A[1] = 1 scan right to left
  C_OLED_SET_DISPLAY_START_LINE = 0xA1,
  C_OLED_SET_DISPLAY_OFFSET = 0xA1,
  C_OLED_SET_DISPLAY_MODE_NORMAL = 0xA4,
  C_OLED_SET_MULTIPLEX_RATIO = 0xA8,
  C_OLED_SET_MASTER_CONFIGURATION = 0xAD,
  C_OLED_SET_POWER_SAVE_MODE = 0xB0,
  C_OLED_SET_PHASE_1_AND_2_PERIOD_ADJUSTMENT = 0xB1,
  C_OLED_SET_DISPLAY_CLOCK_DIVIDER = 0xF0,
  C_OLED_SET_PRECHARGE_A = 0x8A,
  C_OLED_SET_PRECHARGE_B = 0x8B,
  C_OLED_SET_PRECHARGE_C = 0x8C,
  C_OLED_SET_PRECHARGE_LEVEL = 0xBB,
  C_OLED_SET_VCOMH = 0xBE,
  C_OLED_SET_MASTER_CURRENT_CONTROL = 0x87,
  C_OLED_SET_CONTRAST_COLOR_A = 0x81,
  C_OLED_SET_CONTRAST_COLOR_B = 0x82,
  C_OLED_SET_CONTRAST_COLOR_C = 0x83,
  C_OLED_SET_COLUMN_ADDRESS = 0x15,
  C_OLED_SET_ROW_ADDRESS = 0x75,
  C_OLED_SET_DISPLAY_ON = 0xAF,
};

uint8_t oled_init_sequence[] =
{
    C_OLED_NOP1, // 0, 10111100
    C_OLED_SET_DISPLAY_OFF, // 1, 10101110
    C_OLED_SET_REMAP_COLOR, C_OLED_ULX3S_REMAP, // 2
    C_OLED_SET_DISPLAY_START_LINE, 0x00, // 4
    C_OLED_SET_DISPLAY_OFFSET, 0x00, // 6
    C_OLED_SET_DISPLAY_MODE_NORMAL, // 8
    C_OLED_SET_MULTIPLEX_RATIO, 0b00111111, // 9, 15-16
    C_OLED_SET_MASTER_CONFIGURATION, 0b10001110, // 11, a[0]=0 Select external Vcc supply, a[0]=1 Reserved(reset)
    C_OLED_SET_POWER_SAVE_MODE, 0x00, // 13, 0-no power save, 0x1A-power save
    C_OLED_SET_PHASE_1_AND_2_PERIOD_ADJUSTMENT, 0x74, // 15
    C_OLED_SET_DISPLAY_CLOCK_DIVIDER, 0xF0, // 17
    C_OLED_SET_PRECHARGE_A, 0x64, // 19
    C_OLED_SET_PRECHARGE_B, 0x78, // 21
    C_OLED_SET_PRECHARGE_C, 0x64, // 23
    C_OLED_SET_PRECHARGE_LEVEL, 0x31, // 25
    C_OLED_SET_CONTRAST_COLOR_A, 0xFF, // 27, 255
    C_OLED_SET_CONTRAST_COLOR_B, 0xFF, // 29, 255
    C_OLED_SET_CONTRAST_COLOR_C, 0xFF, // 31, 255
    C_OLED_SET_VCOMH, 0x3E,
    C_OLED_SET_MASTER_CURRENT_CONTROL, 0x06,
    C_OLED_SET_COLUMN_ADDRESS, 0x00, 0x5F, // 33, 96
    C_OLED_SET_ROW_ADDRESS, 0x00, 0x3F, // 36, 63
    C_OLED_SET_DISPLAY_ON, // 39
    C_OLED_NOP1, // 40 -- during debugging sent as data
};

volatile uint16_t *oled_spi   = (uint16_t *)0xFFFFFB60;
volatile uint32_t *simple_out = (uint32_t *)0xFFFFFF10;


void dc(uint8_t state)
{
  if(state)
    *simple_out |= (1<<11);
  else
    *simple_out &= ~(1<<11);
}


void resn(uint8_t state)
{
  if(state)
    *simple_out |= (1<<10);
  else
    *simple_out &= ~(1<<10);
}


void oled_fill_screen(uint8_t color)
{
  dc(0); // command
  spi_rxtx(oled_spi, C_OLED_SET_COLUMN_ADDRESS);
  spi_rxtx(oled_spi, 0);
  spi_rxtx(oled_spi, 0x5F);
  spi_rxtx(oled_spi, C_OLED_SET_ROW_ADDRESS);
  spi_rxtx(oled_spi, 0);
  spi_rxtx(oled_spi, 0x3F);
  dc(1); // data
  for(int i = 0; i < 6144; i++)
    spi_rxtx(oled_spi, color);
}

void oled_init()
{
  dc(0); // commands
  resn(0);
  delay(5);
  resn(1);
  delay(20);
  // spi_start_tx(oled_spi);
  for(int i = 0; i < sizeof(oled_init_sequence)/sizeof(oled_init_sequence[0]); i++)
    spi_rxtx(oled_spi, oled_init_sequence[i]);
  oled_fill_screen(0x42);
}

void oled_horizontal_line(uint8_t y, uint8_t color)
{
  dc(0); // command
  spi_rxtx(oled_spi, C_OLED_SET_ROW_ADDRESS);
  spi_rxtx(oled_spi, y);
  spi_rxtx(oled_spi, y);
  dc(1); // data
  for(int i = 0; i < 96; i++)
    spi_rxtx(oled_spi, color);
}

uint8_t oled_color_stripes()
{
  static uint8_t y = 0;
  oled_horizontal_line((y+ 0) & 63, 0xFF); // white
  oled_horizontal_line((y+16) & 63, 0x03); // blue
  oled_horizontal_line((y+32) & 63, 0x1C); // green
  oled_horizontal_line((y+48) & 63, 0xE0); // red
  y++;
  return y;
}


// set up variables using the SD utility library functions:
void oled_read(char *a)
{
  uint8_t line = oled_color_stripes();
  sprintf(a, "OLED: %02x", line);
}
