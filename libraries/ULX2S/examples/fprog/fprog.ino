/*
         SPI flasher
         -----------

On fat32 formatted SD card,
in the root directoy make a
binary file:

ulx2s_4m.img (length: 4194304 bytes, 4MB)

fprog will copy content of this file
to oboard SPI flash
*/

extern "C" {
  #include "fprog.h"
}

void setup(void)
{
  Serial.begin(115200);
  Serial.println("Searching for SD card file ulx2s_4m.img");
  Serial.println("4194304 bytes will be written to SPI flash");
  fprog();
}

void loop(void)
{
}
