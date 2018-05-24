#include <Arduino.h>

void btn_init()
{
  int i;
  // buttons
  for(i = 0; i < 6; i++)
    pinMode(i, INPUT);
  // switches
  for(i = 16; i < 20; i++)
    pinMode(i, INPUT);
  // leds
  for(i = 8; i < 16; i++)
    pinMode(i, OUTPUT);
}

void btn_read(char *line)
{
  static uint8_t shiftdata = 0;
  uint8_t led = B10001 << (shiftdata++ & 3);
  sprintf(line, "BTN:%c%c%c%c%c%c%c SW:%c%c%c%c LED:%c%c%c%c%c%c%c%c\n",
    digitalRead(0) ? '_' : '0',
    digitalRead(1) ? '1' : '_',
    digitalRead(2) ? '2' : '_',
    digitalRead(3) ? '3' : '_',
    digitalRead(4) ? '4' : '_',
    digitalRead(5) ? '5' : '_',
    digitalRead(6) ? '6' : '_',
    digitalRead(16) ? '1' : '_',
    digitalRead(17) ? '2' : '_',
    digitalRead(18) ? '3' : '_',
    digitalRead(19) ? '4' : '_',
    led & B10000000 ? '7' : '_',
    led & B01000000 ? '6' : '_',
    led & B00100000 ? '5' : '_',
    led & B00010000 ? '4' : '_',
    led & B00001000 ? '3' : '_',
    led & B00000100 ? '2' : '_',
    led & B00000010 ? '1' : '_',
    led & B00000001 ? '0' : '_'
  );
  for(int i = 0; i < 8; i++)
    digitalWrite(8+i, led & (1<<i) ? 1 : 0);
}

