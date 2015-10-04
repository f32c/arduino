
//volatile uint32_t * videoram = (volatile uint32_t *) 0x800D0000; // near top 1MB
volatile uint32_t * videoram = (volatile uint32_t *) 0x8000F000; // half-screen remains visible after reset
volatile uint32_t * video_base = (volatile uint32_t *) 0xFFFFFB80;

void setup() {
  // put your setup code here, to run once:

  for (int i = 0; i < 44000; i++)
    videoram[i] = 0;
  for (int i = 80; i < 160; i += 2)
    videoram[i] = 0xFFFFFFFF;
#if 1
  videoram[0] = 0xffffff0F;
  videoram[1] = 0xff000000;
  videoram[2] = 0xff005505;
  videoram[3] = 0xffAA0000;
  videoram[17] = 0xAAaaaa;
#endif
  *video_base = videoram;
}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t j = 0;
  static int x = 79;
  static uint32_t u = 0x01010101;
  int k;
  //return;
  if (x > 38400) x = 0;
  videoram[x+0] ^= u;
  videoram[x+1] += u;
  videoram[x+2]  = u;
  u = (u >> 31) | (u << 1);
  x += 80;
  //delay(1);
  //return;
  if (1)
    for (int i = 80; i < 4096; i++)
    {
      videoram[i] = (i & 0) == 0 ? j | (j<<8) | (j<<16) | (j<<24) : 0;
      //videoram[i] = (i & 3) == 0 ? 129 : 0;
      if ((i & 3) == 0)j++;
      //delay(10);
    }
  // draw 45 angle line
  j = 1;
  k = 0;
  for (int i = 0; i < 480; i++)
  {
    videoram[k] = j; // j
    k += 80;
    j = (j << 1) | (j >> 7);
    if (j == 1) k++;
  }

  //videoram++;
  //*video_base=videoram;
  //delay(100);
}

