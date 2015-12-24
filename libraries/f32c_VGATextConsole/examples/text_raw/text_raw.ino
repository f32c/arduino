// crude malloc for video text memory
// assumes compiled exectuable is <2K
volatile uint16_t *vram = (volatile uint16_t *) 0x80000800;

#define text_addr (*(volatile uint32_t *)0xFFFFFB8C)
#define cntrl_reg (*(volatile uint8_t *)0xfffffb83)
#define finescroll_reg (*(volatile uint8_t *)0xfffffb8b)
#define vblank_reg (*(volatile uint8_t *)0xfffffb87)

void setup() {
  text_addr = vram; // video text base address
  cntrl_reg = 0xC0; // enable text mode, no bitmap
}

void loop() {
  // display scrolling charset
  int i;
  uint16_t glyph;
  static uint16_t glyphstart = 0;
  static uint8_t fscroll = 0;
  while((vblank_reg & 0x80) != 0);
  while((vblank_reg & 0x80) == 0);
  fscroll = ++fscroll & 7;
  finescroll_reg = fscroll;
  if(fscroll == 0)
  {
    glyph = glyphstart++;
    for(i = 0; i < 81*31; i++)
      vram[i] = glyph++;
  }
}
