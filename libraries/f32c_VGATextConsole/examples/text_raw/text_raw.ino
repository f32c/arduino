// crude malloc for video text memory
// assumes compiled exectuable is <2K
volatile uint16_t *vram = (volatile uint16_t *) 0x80000800;

#define text_addr (*(volatile uint32_t *)0xFFFFFB90)
#define cntrl_reg (*(volatile uint8_t *)0xfffffb83)

void setup() {
  text_addr = vram; // video text base address
  cntrl_reg = 0xC0; // enable text mode, no bitmap
}

void loop() {
  // display scrolling charset
  int i;
  uint16_t glyph;
  static uint16_t glyphstart = 0;
  glyph = glyphstart++;
  for(i = 0; i < 80*30; i++)
    vram[i] = glyph++;
  delay(100);
}
