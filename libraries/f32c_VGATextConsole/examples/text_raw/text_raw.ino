volatile uint16_t *vram = (volatile uint16_t *) 0x80100000;
volatile uint32_t *text_addr = (volatile uint32_t *)0xFFFFFB90;
#define cntrl_reg (*(volatile uint8_t *)0xfffffb83)

void setup() {
  *text_addr = vram; // video text base address
  cntrl_reg = 0xE0; // enable text mode
}

void loop() {
  // display scrolling charse
  int i;
  uint8_t glyph;
  static uint8_t glyphstart = 0;
  glyphstart++;
  glyph = glyphstart;
  for(i = 0; i < 80*30; i++)
    vram[i] = 0x1f00 | (glyph++);

  delay(100);
}
