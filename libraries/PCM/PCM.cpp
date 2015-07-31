/*
Author=EMARD
License=CC
*/

#include "PCM.h"

/* constructor does nothing */
PCM::PCM()
{
}

void PCM::dma(uint32_t *addr, uint32_t size)
{
  this->buf = (volatile uint32_t *)addr;
  this->buf = size;
  OUTW(IO_PCM_FIRST, addr);
  OUTW(IO_PCM_LAST, addr + (size&(-1 ^ 3))-1 );
//  OUTW(IO_PCM_LAST, addr + size-1 );
}

void PCM::rate(uint32_t rate)
{
  OUTW(IO_PCM_FREQ, (uint32_t) (1.678061224489795E+7*rate/F_CPU)); /* 9108 is for 44.1 kHz sample rate */
}

void PCM::volume(int16_t left, int16_t right)
{
  OUTW(IO_PCM_VOLUME, left | (left << 16));
}

uint32_t PCM::current()
{
  uint32_t cur;
  INW(cur, IO_PCM_CUR);
}
