/*  Original code from:
    
    PiFmRds - FM/RDS transmitter for the Raspberry Pi
    Copyright (C) 2014 Christophe Jacquet, F8FTK
    
    See https://github.com/ChristopheJacquet/PiFmRds

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    
    Modification: by EMARD
    deleted everything except RDS bit generator
    converted to c++ for arduino, renamed functions
    
*/

#ifndef _RDS_h
#define _RDS_h

#include <stdint.h>

// hardware address of 260-byte RDS buffer
// 260 32-bit words contain each a 8-bit byte (LSB)
#define RDS_ADDRESS 0xFFFFF000

#define GROUP_LENGTH 4
#define BITS_PER_GROUP (GROUP_LENGTH * (BLOCK_SIZE+POLY_DEG))

/* The RDS error-detection code generator polynomial is
   x^10 + x^8 + x^7 + x^5 + x^4 + x^3 + x^0
*/
#define POLY 0x1B9
#define POLY_DEG 10
#define MSB_BIT 0x8000
#define BLOCK_SIZE 16

#define RT_LENGTH 64
#define PS_LENGTH 8

class RDS {
  public:
    RDS();

    void set_pi(uint16_t pi_code);
    void set_rt(char *rt);
    void rt(char *rt);
    void set_ps(char *ps);
    void ps(char *ps);
    void set_ta(int ta);

    void write_buf_crc(uint8_t *buffer, uint16_t *blocks);
    void write_ps_group(uint8_t *buffer, uint8_t group_number);
    void write_rt_group(uint8_t *buffer, uint8_t group_number);

    // get_group() converts text message to binary
    // format suitable for sending
    void get_group(uint8_t *buffer); // convert message to binary

    // send() copies binary to hardware transmission buffer
    // void send();
    void send_ps();
    void send_rt();

    volatile uint32_t debugmem[512];

  private:
    // calculates checksums for binary format  
    uint16_t crc(uint16_t block);

    // internal RDS message in cleartext
    uint16_t pi = 0x1234; // program ID
    uint8_t ta = 0; // traffic announcement
    uint8_t stereo = 0;
    uint8_t afs = 1;
    uint16_t af[7] = {1079, 0, 0, 0, 0, 0, 0}; // x0.1 MHz
    char string_ps[PS_LENGTH]; // short 8-char text shown as station name
    char string_rt[RT_LENGTH]; // long 64-char text

    volatile uint32_t *rdsmem = (volatile uint32_t *)RDS_ADDRESS;
    // some constants required to compose binary format
    const uint16_t offset_words[4] = {0x0FC, 0x198, 0x168, 0x1B4};
    // We don't handle offset word C' here for the sake of simplicity
};
#endif
