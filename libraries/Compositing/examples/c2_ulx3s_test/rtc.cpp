/* RTC test
 * Battery required: CR1225 3V
 * Board v1.7 needs changes: D11=RED LED, R4=4.7k
 * To enter sleep mode, D18 (GREEN LED) should be OFF
 * D18 is OFF by default when FT231x usb-serial is not enumerated
 * Either plug FPGA into 5V USB charger
 * or reprogram FT231x to drive D18 OFF: :
 * ftx_prog --cbus 3 drive_0
 * to restore default D18 state:
 * ftx_prob --cbus 3 sleep
 */

#include <SoftwareWire.h>
SoftwareWire Wire(32+29,32+28);

#define I2CADDR B1101000

void regwrite(uint8_t reg, uint8_t value)
{
  Wire.beginTransmission(I2CADDR);
  Wire.write(byte(reg));
  Wire.write(byte(value));
  Wire.endTransmission();  
}

int regread(uint8_t reg, int8_t n)
{
  Wire.beginTransmission(I2CADDR);
  Wire.write(byte(reg));
  Wire.endTransmission();

  Wire.requestFrom(I2CADDR, n);
  if(Wire.available()) // slave may send less than requested
    return Wire.read();
  return -1;  
}

void rtc_clear_interrupts()
{
  // clear all interrupts
  regwrite(0x01, B00000000);
  regwrite(0x02, B10000000); // use battery backup during power off
}

void rtc_set_clock()
{
  // set clock
  regwrite(0x09, 0x17); // year
  regwrite(0x08, 0x12); // month
  regwrite(0x06, 0x31); // day
  regwrite(0x05, 0x23); // hour
  regwrite(0x04, 0x58); // minute
  regwrite(0x03, 0x00); // second
  rtc_clear_interrupts();
}

void rtc_set_alarm()
{
  // set alarm (in 2 minutes)
  // to disable each: |0x80
  regwrite(0x0D, 0x80); // weekdeay disabled
  regwrite(0x0C, 0x01); // day
  regwrite(0x0B, 0x00); // hour
  regwrite(0x0A, 0x00); // minute
  rtc_clear_interrupts();
}

void rtc_init() {
  Wire.setClock(100000L); // 100000-400000 Hz i2c bus clock
  Wire.begin();            // join i2c bus (address optional for master)

  // resonator capacity 12.5pF, alarm interrupt
  regwrite(0x00, B10000010);

  // disable CLKOUT, short (pulsed interrupts)
  regwrite(0x0F, B11111000);

  rtc_clear_interrupts();
}

void rtc_read(char *a)
{
  sprintf(a, "20%02x-%02x-%02x %02x:%02x:%02x ALM *-%02x %02x:%02x:00\n",
    regread(0x09,1) & 0xFF, // year
    regread(0x08,1) & 0x1F, // month
    regread(0x06,1) & 0x3F, // day
    regread(0x05,1) & 0x3F, // hour
    regread(0x04,1) & 0x7F, // minute
    regread(0x03,1) & 0x7F, // second
    regread(0x0C,1) & 0x3F, // alarm day
    regread(0x0B,1) & 0x3F, // alarm hour
    regread(0x0A,1) & 0x7F  // alarm minute
  );
}

