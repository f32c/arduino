
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
  int i, vol = 21000;
  OUTW(IO_PCM_FIRST, (uint8_t *)buf);   
  OUTW(IO_PCM_LAST, (uint8_t *)(buf + buflen - 1));
  OUTW(IO_PCM_FREQ, 9108/2); /* 9108 => 44.1 kHz sample rate, 9108/2 => 22.05 kHz */
  OUTW(IO_PCM_VOLUME, 0); // quiet when updating buffer
  for(i = 0; i < buflen; i++)
  {
    uint16_t left  = 2*16383 * sin(2*M_PI/buflen * i * 200);
    uint16_t right = 2*16383 * sin(2*M_PI/buflen * i * 201);
    uint16_t mono;
    // left = right = 0;
    mono = left+right;
    buf[i].ch[0] = left;
    buf[i].ch[1] = right;
  }
  OUTW(IO_PCM_VOLUME, vol + (vol << 16));
}

void loop() {
  static int cur, block = 0;
  // checking current position of DMA pointer and
  // updating the other half of the buffer
  INW(cur, IO_PCM_CUR);
  if ((cur & halfdma) != block) 
  {
    // read(f, buf + block, 0x4000);
    block = cur & halfdma;
  }
}
