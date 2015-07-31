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

    void dma(uint32_t *addr, uint32_t size);
    void rate(uint32_t rate);
    void volume(int16_t left, int16_t right);
    
    uint32_t current(); // return current DMA pointer
  private:
    volatile uint32_t *buf = (volatile uint32_t *) 0x80080000;
    uint32_t size = 8192; // bytes (bytes/4 = samples)
};

#endif
