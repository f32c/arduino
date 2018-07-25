/* RTC MCP7940N test
 * Battery required: CR1225 3V
 * Board v1.7 needs changes: D11=RED LED, R4=4.7k
 * To enter sleep mode, D18 (GREEN LED) should be OFF
 * D18 is OFF by default when FT231x usb-serial is not enumerated
 * Either plug FPGA into 5V USB charger
 * or reprogram FT231x to drive D18 OFF:
 * ftx_prog --cbus 3 drive_0
 * to restore default D18 state:
 * ftx_prob --cbus 3 sleep
 */

#include <SoftwareWire.h>
SoftwareWire Wire(32+29,32+28);

#define I2CADDR 0x6F // Microchip MCP7940N

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
}

void rtc_set_clock()
{
  // set clock
  regwrite(0x06, 0x17); // year
  regwrite(0x05, 0x12); // month
  regwrite(0x04, 0x31); // day
  regwrite(0x03, 0x07 | 8); // weekday | battery_backup
  regwrite(0x02, 0x23); // hour
  regwrite(0x01, 0x59); // minute
  regwrite(0x00, 0x00 | 0x80); // second | start_oscillator
}

void rtc_set_alarm()
{
  // set alarm (in 2 minutes)
  regwrite(0x10, 0x18); // year
  regwrite(0x0F, 0x01); // month
  regwrite(0x0E, 0x01); // day
  regwrite(0x0D, 0x01 | 0x70); // weekday | match_condition full timedate
  regwrite(0x0C, 0x00); // hour
  regwrite(0x0B, 0x00); // minute
  regwrite(0x0A, 0x00); // seconds
}

void rtc_init() {
  Wire.setClock(100000L); // 100000-400000 Hz i2c bus clock
  Wire.begin();            // join i2c bus (address optional for master)

  // set control register, enable only alarm0
  regwrite(0x07, 0x10);

  // set oscillator digital trim
  regwrite(0x08, 0x45);
}

void rtc_read(char *a)
{
  char *eval;
  uint8_t yr = regread(0x06,1) & 0x3F;
  if(yr >= 0x17 && yr <= 0x18)
    eval = regread(0x0D,1) & 8 ? " OK " : "WAIT";
  else
    eval = "FAIL";
  sprintf(a, "20%02x-%02x-%02x %02x:%02x:%02x %s *-%02x %02x:%02x:%02x\n",
    regread(0x06,1) & 0x3F, // year
    regread(0x05,1) & 0x7F, // month
    regread(0x04,1) & 0x7F, // day
    regread(0x02,1) & 0x3F, // hour
    regread(0x01,1) & 0x7F, // minute
    regread(0x00,1) & 0x7F, // second
    eval,  // time evaluation and alarm flag
    regread(0x0E,1) & 0x3F, // alarm day
    regread(0x0C,1) & 0x3F, // alarm hour
    regread(0x0B,1) & 0x7F, // alarm minute
    regread(0x0A,1) & 0x7F  // alarm second
  );
}

