#include <Arduino.h>

void btn_init()
{
  int i;
  for(i = 0; i < 6; i++)
    pinMode(i, INPUT);
  for(i = 16; i < 20; i++)
    pinMode(i, INPUT);
}

void btn_read(char *line)
{
  sprintf(line, "BTN:%c%c%c%c%c%c%c SW:%c%c%c%c\n",
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
    digitalRead(19) ? '4' : '_'
  );
}

