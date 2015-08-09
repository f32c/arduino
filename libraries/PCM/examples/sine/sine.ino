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

void setup() {
  // set audio parameters
  int i, vol = 21000, rate = 22050;
  pcm.dma((uint32_t *)buf, buflen);
  pcm.rate(rate);
  pcm.volume(0,0);
  for(i = 0; i < buflen; i++)
  {
    int16_t left  = 32767 * sin(2*M_PI/buflen * i * 200);
    int16_t right = 32767 * sin(2*M_PI/buflen * i * 201);
    buf[i].ch[0] = left;
    buf[i].ch[1] = right;
  }
  pcm.volume(vol, vol);
}

void loop() {
  static int cur, block = 0;
  // checking current position of DMA pointer and
  // updating the other half of the buffer
  cur = pcm.current();
  if ((cur & halfdma) != block) 
  {
    // read(f, buf + block, 0x4000);
    block = cur & halfdma;
  }
}
