#include <SPI.h>

SPIClass SPI_flash(SPI_FLASH);

void setup(void)
{
  Serial.begin(115200);
  Serial.println("Startup");
  delay(1000);
  SPI_flash.begin(11);
  delay(1000);
}

void leading_zero_hex(uint8_t _value) {
  if (_value < 0x10) {
    Serial.print("0");
  }
  Serial.print(_value, HEX);
}

void flash_id(void)
{
  SPI_flash.beginTransaction();
  SPI_flash.transfer(0x9F);
  leading_zero_hex(SPI_flash.transfer(0x00));
  leading_zero_hex(SPI_flash.transfer(0x00));
  leading_zero_hex(SPI_flash.transfer(0x00));
  SPI_flash.endTransaction();
}

void loop(void)
{
  Serial.println("##############################");
  delay(500);
  Serial.print("FLASH ID: 0x");
  flash_id();
  Serial.println('\0');
  Serial.println("##############################");
  delay(1000);
}

