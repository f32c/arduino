#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#define VARIANT_DIGITAL_PIN_MAP \
{ \
/* arduino pin number               port        bitmask */ \
  { /*  0 */  (volatile uint32_t *) IO_PUSHBTN,      0  }, \
  { /*  1 */  (volatile uint32_t *) IO_PUSHBTN,      0  }, \
  { /*  2 */  (volatile uint32_t *) IO_GPIO_DATA, 1<<0  /* lcd3 */ }, \
  { /*  3 */  (volatile uint32_t *) IO_GPIO_DATA, 1<<1  /* lcd2 */ }, \
  { /*  4 */  (volatile uint32_t *) IO_GPIO_DATA, 1<<2  /* lcd1 */ }, \
  { /*  5 */  (volatile uint32_t *) IO_PUSHBTN,   1<<(8+7)  /* led7 */ }, \
  { /*  6 */  (volatile uint32_t *) IO_PUSHBTN,   1<<(8+6)  /* led6 */ }, \
  { /*  7 */  (volatile uint32_t *) IO_PUSHBTN,   1<<(8+5)  /* led5 */ }, \
  { /*  8 */  (volatile uint32_t *) IO_GPIO_DATA, 1<<3  /* lcd0 */ }, \
  { /*  9 */  (volatile uint32_t *) IO_GPIO_DATA, 1<<4  /* lcd5 */ }, \
  { /* 10 */  (volatile uint32_t *) IO_PUSHBTN,   1<<2  /* btn_down */ }, \
  { /* 11 */  (volatile uint32_t *) IO_PUSHBTN,   1<<1  /* btn_left */ }, \
  { /* 12 */  (volatile uint32_t *) IO_PUSHBTN,   1<<(8+4)  /* led 4 */ }, \
  { /* 13 */  (volatile uint32_t *) IO_GPIO_DATA, 1<<5  /* lcd4 */ }, \
  { /* 14 */  (volatile uint32_t *) IO_GPIO_DATA, 1<<6  }, \
  { /* 15 */  (volatile uint32_t *) IO_GPIO_DATA, 1<<7  }, \
  { /* 16 */  (volatile uint32_t *) IO_GPIO_DATA, 1<<8  }, \
  { /* 17 */  (volatile uint32_t *) IO_GPIO_DATA, 1<<9  }, \
  { /* 18 */  (volatile uint32_t *) IO_GPIO_DATA, 1<<10 }, \
  { /* 19 */  (volatile uint32_t *) IO_GPIO_DATA, 1<<11 }, \
  { /* 20 */  (volatile uint32_t *) IO_GPIO_DATA, 1<<12 }, \
  { /* 21 */  (volatile uint32_t *) IO_GPIO_DATA, 1<<13 }, \
  { /* 22 */  (volatile uint32_t *) IO_GPIO_DATA, 1<<14 }, \
  { /* 23 */  (volatile uint32_t *) IO_GPIO_DATA, 1<<15 }, \
  { /* 24 */  (volatile uint32_t *) IO_PUSHBTN,       0 }, \
  { /* 25 */  (volatile uint32_t *) IO_PUSHBTN,       0 }, \
  { /* 26 */  (volatile uint32_t *) IO_GPIO_DATA, 1<<16 }, \
  { /* 27 */  (volatile uint32_t *) IO_GPIO_DATA, 1<<17 }, \
  { /* 28 */  (volatile uint32_t *) IO_GPIO_DATA, 1<<18 }, \
  { /* 29 */  (volatile uint32_t *) IO_GPIO_DATA, 1<<19 }, \
  { /* 30 */  (volatile uint32_t *) IO_GPIO_DATA, 1<<20 }, \
  { /* 31 */  (volatile uint32_t *) IO_GPIO_DATA, 1<<21 }, \
  { /* 32 */  (volatile uint32_t *) IO_GPIO_DATA, 1<<22 }, \
  { /* 33 */  (volatile uint32_t *) IO_GPIO_DATA, 1<<23 }, \
  { /* 34 */  (volatile uint32_t *) IO_GPIO_DATA, 1<<24 }, \
  { /* 35 */  (volatile uint32_t *) IO_GPIO_DATA, 1<<25 }, \
  { /* 36 */  (volatile uint32_t *) IO_GPIO_DATA, 1<<26 }, \
  { /* 37 */  (volatile uint32_t *) IO_GPIO_DATA, 1<<27 }, \
  { /* 38 */  (volatile uint32_t *) IO_PUSHBTN,   1<<0  /* btn_right */ }, \
  { /* 39 */  (volatile uint32_t *) IO_PUSHBTN,   1<<4  /* btn_center */ }, \
  { /* 40 */  (volatile uint32_t *) IO_GPIO_DATA, 1<<28 }, \
  { /* 41 */  (volatile uint32_t *) IO_PUSHBTN,   1<<(8+0)  /* led_0 */ }, \
  { /* 42 */  (volatile uint32_t *) IO_PUSHBTN,   1<<(8+1)  /* led_1 */ }, \
  { /* 43 */  (volatile uint32_t *) IO_PUSHBTN,   1<<(8+2)  /* led_2 */ }, \
  { /* 44 */  (volatile uint32_t *) IO_PUSHBTN,   1<<(8+3)  /* led_3 */ }, \
  { /* 45 */  (volatile uint32_t *) IO_GPIO_DATA,    0  /* p_tip_0 */ }, \
  { /* 46 */  (volatile uint32_t *) IO_GPIO_DATA,    0  /* p_tip_1 */ }, \
  { /* 47 */  (volatile uint32_t *) IO_GPIO_DATA,    0  /* p_tip_2 */ }, \
  { /* 48 */  (volatile uint32_t *) IO_GPIO_DATA,    0  }, \
  { /* 49 */  (volatile uint32_t *) IO_GPIO_DATA,    0  }, \
  { /* 50 */  (volatile uint32_t *) IO_GPIO_DATA,    0  }, \
  { /* 51 */  (volatile uint32_t *) IO_GPIO_DATA,    0  }, \
  { /* 52 */  (volatile uint32_t *) IO_GPIO_DATA,    0  }, \
  { /* 53 */  (volatile uint32_t *) IO_GPIO_DATA,    0  }, \
  { /* 54 */  (volatile uint32_t *) IO_GPIO_DATA,    0  }, \
  { /* 55 */  (volatile uint32_t *) IO_GPIO_DATA,    0  }, \
  { /* 56 */  (volatile uint32_t *) IO_GPIO_DATA,    0  }, \
  { /* 57 */  (volatile uint32_t *) IO_GPIO_DATA,    0  }, \
  { /* 58 */  (volatile uint32_t *) IO_GPIO_DATA,    0  }, \
  { /* 59 */  (volatile uint32_t *) IO_GPIO_DATA,    0  }, \
  { /* 60 */  (volatile uint32_t *) IO_GPIO_DATA,    0  }, \
  { /* 61 */  (volatile uint32_t *) IO_GPIO_DATA,    0  }, \
  { /* 62 */  (volatile uint32_t *) IO_GPIO_DATA,    0  }, \
  { /* 63 */  (volatile uint32_t *) IO_GPIO_DATA,    0  }, \
}

#endif
