/* Test connectivity of audio jack DAC
** it will check do all FPGA DAC lines have
** connection to DAC resistors network.
** This check cannot detect does signal
** really appears on audio jack.
* 
*  audio pins should have pull-up enabled in the bitstream
*  pulling down any of the bits on each channel should result in all 4 bits
*  reading in this channel be 0x0, otherwise all bits will 0xF 
*  any other value indicates bits connection error
*  
*  Todo: if probe bit is not connected,
*  complete checkword will fail. Construct better boolean
*  algebra so that using another probe bit reveals one disconnected line
*/
#include <Arduino.h>

// additional gpio for DAC lines testing (not integrated into arduino yet)
// this needs special bitstream with gpio 64-75 (bits 0-11) connected to
// 12 DAC resistor network
volatile uint32_t *gpio2_out = (uint32_t *) 0xfffff840;
volatile uint32_t *gpio2_ctl = (uint32_t *) 0xfffff844;
volatile uint32_t *gpio2_in  = (uint32_t *) 0xfffff858;
const uint32_t gpio2_bits = 0xFFF; // all bits/pins 0-11

void dac_init()
{
  *gpio2_ctl &= ~gpio2_bits; // inputs (output disabled)
  *gpio2_out &= ~gpio2_bits; // output default low when enabled
}

// 4-bit value converted to 4-byte string "3210"
void dac_decode(char *a, uint8_t v)
{
  for(uint8_t i = 0; i < 4; i++)
  {
    a[3-i] = v & 1 ? '_' : '0'+i;
    v >>= 1;
  }
  a[4] = '\0'; // null-string termination
}


void dac_read(char *a)
{
  uint32_t probeword, checkword, difference;
  // static uint8_t counter = 0; // moving bit for detection state
  uint32_t i;
  // testword 32-cycle
  uint32_t fail = 0; // reset fail flags
  difference = 0; // reset difference register
  // this for-loop should find and indentify
  // disconnected or faulty bits in DAC resistor network
  for(i = 0; i < 32; i++)
  {
    probeword = (((i & B100)<<6)  | ((i & B10)<<3) | (i & B1)) << ((i >> 3) & 3);
    checkword = 0xFFF & ~( (0xF*((i & B100)<<6))  | (0xF*((i & B10)<<3)) | (0xF*(i & B1)) );
    *gpio2_ctl = (*gpio2_ctl & ~gpio2_bits) | probeword;
    delay(1); // wait for change to take effect
    // any difference between checkword and read bits will set a bit in difference register 
    uint32_t error_bits = checkword ^ *gpio2_in;
    difference |= probeword & error_bits;
    fail |= error_bits;
  }
  difference &= 0xFFF; // final bitmask
  char dac_l[5], dac_r[5], dac_v[5];
  dac_decode(dac_l, difference);
  dac_decode(dac_r, difference >> 4);
  dac_decode(dac_v, difference >> 8);
  sprintf(a, "DAC: L%s %s R%s %s V%s %s\n",
      dac_l,
      (fail & 0x00F) == 0 ? " OK " : "FAIL",
      dac_r,
      (fail & 0x0F0) == 0 ? " OK " : "FAIL",
      dac_v,
      (fail & 0xF00) == 0 ? " OK " : "FAIL"
  );
}

