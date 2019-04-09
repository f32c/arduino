#include <Arduino.h>
#include <math.h>
#include <PCM.h>

PCM pcm = PCM();

struct sample
{
  int16_t ch[2]; // 0:left and 1:right channel, signed 16 bit integers
};

// crude malloc()
volatile struct sample *buf = (volatile struct sample *) 0x80080000;
int buflen = 1<<13; // number of samples, halfdma code easier if power of 2
int halfdma = buflen*sizeof(struct sample)/2;
int maxvolume = 20000;
int stepvolume = 50;

void pcm_tone()
{
  for(int i = 0; i <= maxvolume; i += stepvolume)
  {
    pcm.volume(i, i);
    delayMicroseconds(1);
  }
}

void pcm_mute()
{
  for(int i = maxvolume; i >= 0; i -= stepvolume)
  {
    pcm.volume(i, i);
    delayMicroseconds(1);
  }
}

void pcm_init() {
  // set audio parameters
  int i, rate = 22050;
  buf = (struct sample *)malloc(buflen * sizeof(struct sample));
  pcm.dma((uint32_t *)buf, buflen);
  pcm.rate(rate);
  pcm_mute();
  for(i = 0; i < buflen; i++)
  {
    int16_t left  = 32767 * sin(2*M_PI/buflen * i * 200);
    int16_t right = 32767 * sin(2*M_PI/buflen * i * 201);
    buf[i].ch[0] = left;
    buf[i].ch[1] = right;
  }
}
