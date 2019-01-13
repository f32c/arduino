#include <SPI.h>

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

#define LED_RED 8
#define LED_GREEN 10

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

#if (_BYTE_ORDER == _LITTLE_ENDIAN)
#define	SPI_READY_MASK (1 << 8)
#else
#define	SPI_READY_MASK (1 << 16)
#endif

SPIClass SPI_oled(SPI_OLED);

volatile uint32_t *simpleio  = (uint32_t *)0xFFFFFF10;


void dc(uint8_t state)
{
  if(state)
    *simpleio |= (1<<11);
  else
    *simpleio &= ~(1<<11);
}


void resn(uint8_t state)
{
  if(state)
    *simpleio |= (1<<10);
  else
    *simpleio &= ~(1<<10);
}


void reset_sequence()
{
  dc(1); // data
  resn(1);
  delay(20);
  resn(0);
  delay(20);
  resn(1);
  delay(100);
}


void initialize()
{
  dc(0); // commands
  SPI_oled.begin();
  SPI_oled.setClock(6600000);
  SPI_oled.beginTransaction();
  for(int i = 0; i < sizeof(oled_init_sequence)/sizeof(oled_init_sequence[0]); i++)
    SPI_oled.transfer(oled_init_sequence[i]);
}


void fill_screen(uint8_t color)
{
  dc(0); // command
  SPI_oled.transfer(C_OLED_SET_COLUMN_ADDRESS);
  SPI_oled.transfer(0);
  SPI_oled.transfer(0x5F);
  SPI_oled.transfer(C_OLED_SET_ROW_ADDRESS);
  SPI_oled.transfer(0);
  SPI_oled.transfer(0x3F);
  dc(1); // data
  for(int i = 0; i < 6144; i++)
    SPI_oled.transfer(color);
}


void horizontal_line(uint8_t y, uint8_t color)
{
  dc(0); // command
  SPI_oled.transfer(C_OLED_SET_ROW_ADDRESS);
  SPI_oled.transfer(y);
  SPI_oled.transfer(y);
  dc(1); // data
  for(int i = 0; i < 96; i++)
    SPI_oled.transfer(color);
}


void color_oled_lines()
{
  static uint8_t y = 0;
  horizontal_line((y+ 0) & 63, 0xFF); // white
  horizontal_line((y+16) & 63, 0x03); // blue
  horizontal_line((y+32) & 63, 0x1C); // green
  horizontal_line((y+48) & 63, 0xE0); // red
  y++;
}


void demo_blink(int n)
{
  for(int i = 0; i < n; i++)
  {
    digitalWrite(LED_RED, 0);
    fill_screen(0x10);
    digitalWrite(LED_GREEN, 1);
    Serial.println("OLED GREEN");
    delay(500);

    digitalWrite(LED_GREEN, 0);
    fill_screen(0x80);
    digitalWrite(LED_RED, 1);
    Serial.println("OLED RED");
    delay(500);
  }
}


void setup(void)
{
  reset_sequence();
  initialize();
  for(int i = 8; i < 16; i++)
    digitalWrite(i, 0); // all LEDs off
  demo_blink(2);
}


void loop(void)
{
  color_oled_lines();
  digitalWrite(LED_RED, 0);
  digitalWrite(LED_GREEN, 1);
  delay(20);

  color_oled_lines();
  digitalWrite(LED_GREEN, 0);
  digitalWrite(LED_RED, 1);
  delay(20);
}
