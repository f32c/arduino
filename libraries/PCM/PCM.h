/*
Author=EMARD
License=CC    
*/

#ifndef _PCM_h
#define _PCM_h

#include <stdint.h>
#include <dev/io.h>

#if 0
struct sample
{
  int16_t ch[2]; // 0:left and 1:right channel, signed 16 bit integers
};  
#endif

class PCM {
  public:
    PCM();

    inline void dma(uint32_t *addr, uint32_t size)
    {
      OUTW(IO_PCM_FIRST, addr);
      OUTW(IO_PCM_LAST, addr + (size&(-1 ^ 3))-1 );
    }

    inline void dmafirst(uint32_t *addr)
    {
      OUTW(IO_PCM_FIRST, addr);
    }

    inline void dmalast(uint32_t *addr)
    {
      OUTW(IO_PCM_LAST, addr);
    }

    inline void Hz(uint32_t sample_rate_Hz)
    {
      OUTW(IO_PCM_FREQ, (uint32_t) (((float)(1<<24))/F_CPU*sample_rate_Hz));
    }

    inline void rate(uint32_t sample_rate_Hz)
    {
      OUTW(IO_PCM_FREQ, (uint32_t) (((float)(1<<24))/F_CPU*sample_rate_Hz));
    }

    inline void rateraw(uint32_t rate)
    {
      OUTW(IO_PCM_FREQ, rate); /* value 9108 at 81.25 MHz CPU makes 44.1 kHz sample rate */
    }

    inline void volume(int16_t left, int16_t right)
    {
      OUTW(IO_PCM_VOLUME, left | (left << 16));
    }

    inline uint32_t current()
    {
      uint32_t current_dma_pointer;
      INW(current_dma_pointer, IO_PCM_CUR);
      return current_dma_pointer;
    }

#if 0
  private:
    volatile uint32_t *buf = (volatile uint32_t *) 0x80080000;
    uint32_t size = 8192; // bytes (bytes/4 = samples)
#endif
};

#endif
