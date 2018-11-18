#include <Arduino.h>
#include "spi.h"

volatile uint16_t *flash_spi = (uint16_t *)0xFFFFFB40;

uint8_t flash_id(void)
{
  spi_start_tx(flash_spi);
  spi_rxtx(flash_spi, 0xAB);
  spi_rxtx(flash_spi, 0);
  spi_rxtx(flash_spi, 0);
  spi_rxtx(flash_spi, 0);
  return spi_rxtx(flash_spi, 0);
}

// set up variables using the SD utility library functions:
void flash_read(char *a)
{
  uint8_t id = flash_id();
  if(id >= 0x15 && id <= 0x17)
    sprintf(a, "FLASH: %02x OK  ", id);
  else
    sprintf(a, "FLASH: %02x FAIL", id);
}
