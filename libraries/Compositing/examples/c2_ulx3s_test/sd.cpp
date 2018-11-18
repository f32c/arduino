#include <Arduino.h>

#if 0
#include "spi.h"
volatile uint16_t *sd_spi    = (uint16_t *)0xFFFFFB50;
uint8_t sd_cmd0()
{
 int i, r;

 spi_start_tx(sd_spi);
 for(i = 0; i < 10; i++)
   spi_rxtx(sd_spi, 0xFF);
 spi_rxtx(sd_spi, 0x40);
 spi_rxtx(sd_spi, 0x00);
 spi_rxtx(sd_spi, 0x00);
 spi_rxtx(sd_spi, 0x00);
 spi_rxtx(sd_spi, 0x95);

 //Polling it to check  response to 0x01
 r = 0xFF;
 for(int i = 0; i < 1000 && r != 0xFF; i++)
   r = spi_rxtx(sd_spi, 0xFF);
 return r;
}

void sd_read(char *a)
{
  uint8_t cmd0_response = sd_cmd0();
  if(cmd0_response == 1)
    sprintf(a, "SD: %02x OK  ", cmd0_response);
  else
    sprintf(a, "SD: %02x FAIL", cmd0_response);
}
#endif

#if 1
#include <SPI.h>
#include <SD.h>
// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;
// reading sd card type is slow
void sd_read(char *a)
{
  char *cardtype;
  if (card.init(SPI_HALF_SPEED, 2))
  {
    switch (card.type())
    {
    case SD_CARD_TYPE_SD1:
      cardtype = "1 ";
      break;
    case SD_CARD_TYPE_SD2:
      cardtype = "2 ";
      break;
    case SD_CARD_TYPE_SDHC:
      cardtype = "HC";
      break;
    default:
      cardtype = "? ";
    }
    sprintf(a, "SD: %s OK", cardtype);
  }
  else
  {
    sprintf(a, "SD: FAIL ");
  }
}
#endif
