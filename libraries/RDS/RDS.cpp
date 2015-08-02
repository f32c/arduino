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

#include <string.h>
#include "RDS.h"

/* constructor does nothing */
RDS::RDS()
{
}

/* Classical CRC computation */
uint16_t RDS::crc(uint16_t block) {
    uint16_t crc = 0;
    
    for(int j=0; j<BLOCK_SIZE; j++) {
        int bit = (block & MSB_BIT) != 0;
        block <<= 1;

        int msb = (crc >> (POLY_DEG-1)) & 1;
        crc <<= 1;
        if((msb ^ bit) != 0) {
            crc = crc ^ POLY;
        }
    }
    
    return crc;
}

#if 0
// this works on unix
// Arduino has no time.h struct
// the low-cost hardware also has no RTC

/* Possibly generates a CT (clock time) group if the minute has just changed
   Returns 1 if the CT group was generated, 0 otherwise
*/
int RDS::get_ct_group(uint16_t *blocks) {
    static int latest_minutes = -1;

    // Check time
    time_t now;
    struct tm *utc;
    
    now = time (NULL);
    utc = gmtime (&now);

    if(utc->tm_min != latest_minutes) {
        // Generate CT group
        latest_minutes = utc->tm_min;
        
        int l = utc->tm_mon <= 1 ? 1 : 0;
        int mjd = 14956 + utc->tm_mday + 
                        (int)((utc->tm_year - l) * 365.25) +
                        (int)((utc->tm_mon + 2 + l*12) * 30.6001);
        
        blocks[1] = 0x4400 | (mjd>>15);
        blocks[2] = (mjd<<1) | (utc->tm_hour>>4);
        blocks[3] = (utc->tm_hour & 0xF)<<12 | utc->tm_min<<6;
        
        utc = localtime(&now);
        
        int offset = utc->tm_gmtoff / (30 * 60);
        blocks[3] |= abs(offset);
        if(offset < 0) blocks[3] |= 0x20;
        
        //printf("Generated CT: %04X %04X %04X\n", blocks[1], blocks[2], blocks[3]);
        return 1;
    } else return 0;
}
#endif

// write block to the buffer and append the CRC
void RDS::write_buf_crc(uint8_t buffer[], uint16_t blocks[])
{
    int bitptr = 0; // pointer to a bit in the buffer

    /* erase buffer */
    for(int i = 0; i < BITS_PER_GROUP/8; i++)
      buffer[i] = 0;

    // Calculate the checkword for each block and emit the bits
    for(int i=0; i<GROUP_LENGTH; i++) {
        uint16_t block = blocks[i];
        uint16_t check = crc(block) ^ this->offset_words[i];
        for(int j=0; j<BLOCK_SIZE; j++) {
            buffer[bitptr/8] |= ((block & (1<<(BLOCK_SIZE-1))) != 0) << (7 - bitptr % 8);
            bitptr++;
            block <<= 1;
        }
        for(int j=0; j<POLY_DEG; j++) {
            buffer[bitptr/8] |= ((check & (1<<(POLY_DEG-1))) != 0) << (7 - bitptr % 8);
            bitptr++;
            check <<= 1;
        }
    }
}

// write buffer with n-th group of PS
// PS consists of 4 groups of 13 bytes each numbered 0..3 
// lower 2 bits of n define the group number
void RDS::write_ps_group(uint8_t *buffer, uint8_t group_number)
{
  uint16_t blocks[GROUP_LENGTH] = {this->pi, 0, 0, 0};
  uint8_t gn = group_number & 3; // group number

  blocks[1] = 0x0400 | gn;
  if(this->stereo != 0 && gn == 3)
    blocks[1] |= 0x0004;
  if(this->ta)
    blocks[1] |= 0x0010;
  blocks[2] = 0xCDCD;     // no AF
  if(gn == 0)
    // 224..249 -> 0..25 AFs but we support max 7
    blocks[2] = (blocks[2] & 0x00FF) | ((this->afs+224)<<8);
  else
  {
    if(this->af[2*gn-1] > 875)
     blocks[2] = (blocks[2] & 0x00FF) | ((this->af[2*gn-1]-875)<<8);    
  }
  if(this->af[2*gn] > 875)
    blocks[2] = (blocks[2] & 0xFF00) | (this->af[2*gn]-875);
  blocks[3] = this->string_ps[gn*2]<<8 | this->string_ps[gn*2+1];
  write_buf_crc(buffer, blocks);
}

// write buffer with n-th group of RT
// RT consists of 16 groups of 13 bytes each numbered 0..15 
// lower 4 bits of n define the group number
void RDS::write_rt_group(uint8_t *buffer, uint8_t group_number)
{
  uint16_t blocks[GROUP_LENGTH] = {this->pi, 0, 0, 0};
  uint8_t gn = group_number & 15; // group number

  blocks[1] = 0x2400 | gn;
  blocks[2] = this->string_rt[gn*4+0]<<8 | this->string_rt[gn*4+1];
  blocks[3] = this->string_rt[gn*4+2]<<8 | this->string_rt[gn*4+3];

  write_buf_crc(buffer, blocks);
}

void RDS::send_ps(void)
{
  int rds_mem_offset = 0;
  uint8_t bit_buffer[BITS_PER_GROUP/8];
  for(int i = 0; i < 4; i++)
  {
    rds_mem_offset = (BITS_PER_GROUP/8) * (i*5);
    write_ps_group(bit_buffer, i);
    for(int j = 0; j < BITS_PER_GROUP/8; j++)
    {
      // this->debugmem[rds_mem_offset] = bit_buffer[j];
      this->rdsmem[rds_mem_offset++] = bit_buffer[j];
    }
  }
}

void RDS::send_rt(void)
{
  int rds_mem_offset = 0;
  uint8_t bit_buffer[BITS_PER_GROUP/8];
 
  for(int i = 0; i < 16; i++)
  {
    if( (i & 3) == 0) // skip locations of PS packets
      rds_mem_offset += (BITS_PER_GROUP/8);
    write_rt_group(bit_buffer, i);
    for(int j = 0; j < BITS_PER_GROUP/8; j++)
    {
      // this->debugmem[rds_mem_offset] = bit_buffer[j];
      this->rdsmem[rds_mem_offset++] = bit_buffer[j];
    }
  }
}

void RDS::set_pi(uint16_t pi_code) {
    this->pi = pi_code;
}

void RDS::set_rt(char *rt) {
    strncpy(this->string_rt, rt, 64);
    for(int i=0; i<64; i++) {
        if(this->string_rt[i] == 0) this->string_rt[i] = 32;
    }
}

void RDS::rt(char *rt)
{
  set_rt(rt);
  send_rt();
}

void RDS::set_ps(char *ps) {
    strncpy(this->string_ps, ps, 8);
    for(int i=0; i<8; i++) {
        if(this->string_ps[i] == 0) this->string_ps[i] = 32;
    }
}

void RDS::ps(char *ps)
{
  set_ps(ps);
  send_ps();
}

void RDS::set_ta(int ta) {
    this->ta = ta;
}
