#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;

void sd_read(char *a)
{
  char *cardtype;
  if (card.init(SPI_HALF_SPEED, 2))
  {
    switch (card.type())
    {
    case SD_CARD_TYPE_SD1:
      cardtype = "SD1";
      break;
    case SD_CARD_TYPE_SD2:
      cardtype = "SD2";
      break;
    case SD_CARD_TYPE_SDHC:
      cardtype = "SDHC";
      break;
    default:
      cardtype = "Unknown";
    }
    sprintf(a, "SD: %s OK\n", cardtype);
  }
  else
  {
    sprintf(a, "SD: FAIL\n");
  }
}
