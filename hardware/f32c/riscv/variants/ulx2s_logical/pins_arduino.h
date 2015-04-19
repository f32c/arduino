#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#define VARIANT_DIGITAL_PIN_MAP \
{ \
/* arduino pin number               port         bit bitmask  pwm  icp comment */ \
  { /*  0 */  (volatile uint32_t *) IO_PUSHBTN,    0, 1<<0,    -1, -1 }, \
  { /*  1 */  (volatile uint32_t *) IO_PUSHBTN,    1, 1<<1,    -1, -1 }, \
  { /*  2 */  (volatile uint32_t *) IO_PUSHBTN,    2, 1<<2,    -1, -1 }, \
  { /*  3 */  (volatile uint32_t *) IO_PUSHBTN,    3, 1<<3,    -1, -1 }, \
  { /*  4 */  (volatile uint32_t *) IO_PUSHBTN,    4, 1<<4,    -1, -1 }, \
  { /*  5 */  (volatile uint32_t *) IO_PUSHBTN,    5, 1<<5,    -1, -1 }, \
  { /*  6 */  (volatile uint32_t *) IO_PUSHBTN,    6, 1<<6,    -1, -1 }, \
  { /*  7 */  (volatile uint32_t *) IO_PUSHBTN,    7, 1<<7,    -1, -1 }, \
  { /*  8 */  (volatile uint32_t *) IO_LED,        8, 1<<0,    -1,  0 /* led0, icp0 */ }, \
  { /*  9 */  (volatile uint32_t *) IO_LED,        9, 1<<1,     0, -1 /* led1, ocp0 */ }, \
  { /* 10 */  (volatile uint32_t *) IO_LED,       10, 1<<2,     1, -1 /* led2, ocp1 */ }, \
  { /* 11 */  (volatile uint32_t *) IO_LED,       11, 1<<3,    -1,  1 /* led3, icp1 */ }, \
  { /* 12 */  (volatile uint32_t *) IO_LED,       12, 1<<4,    -1, -1 /* led4 */ }, \
  { /* 13 */  (volatile uint32_t *) IO_LED,       13, 1<<5,    -1, -1 /* led5 */ }, \
  { /* 14 */  (volatile uint32_t *) IO_LED,       14, 1<<6,    -1, -1 /* led6, pwm0 */ }, \
  { /* 15 */  (volatile uint32_t *) IO_LED,       15, 1<<7,    -1, -1 /* led7, pwm1 */ }, \
  { /* 16 */  (volatile uint32_t *) IO_PUSHBTN,   16, 1<<16,   -1, -1 }, \
  { /* 17 */  (volatile uint32_t *) IO_PUSHBTN,   17, 1<<17,   -1, -1 }, \
  { /* 18 */  (volatile uint32_t *) IO_PUSHBTN,   18, 1<<18,   -1, -1 }, \
  { /* 19 */  (volatile uint32_t *) IO_PUSHBTN,   19, 1<<19,   -1, -1 }, \
  { /* 20 */  (volatile uint32_t *) IO_PUSHBTN,   20, 1<<20,   -1, -1 }, \
  { /* 21 */  (volatile uint32_t *) IO_PUSHBTN,   21, 1<<21,   -1, -1 }, \
  { /* 22 */  (volatile uint32_t *) IO_PUSHBTN,   22, 1<<22,   -1, -1 }, \
  { /* 23 */  (volatile uint32_t *) IO_PUSHBTN,   23, 1<<23,   -1, -1 }, \
  { /* 24 */  (volatile uint32_t *) IO_PUSHBTN,   24, 1<<24,   -1, -1 }, \
  { /* 25 */  (volatile uint32_t *) IO_PUSHBTN,   25, 1<<25,   -1, -1 }, \
  { /* 26 */  (volatile uint32_t *) IO_PUSHBTN,   26, 1<<26,   -1, -1 }, \
  { /* 27 */  (volatile uint32_t *) IO_PUSHBTN,   27, 1<<27,   -1, -1 }, \
  { /* 28 */  (volatile uint32_t *) IO_PUSHBTN,   28, 1<<28,   -1, -1 }, \
  { /* 29 */  (volatile uint32_t *) IO_PUSHBTN,   29, 1<<29,   -1, -1 }, \
  { /* 30 */  (volatile uint32_t *) IO_PUSHBTN,   30, 1<<30,   -1, -1 }, \
  { /* 31 */  (volatile uint32_t *) IO_PUSHBTN,   31, 1<<31,   -1, -1 }, \
  { /* 32 */  (volatile uint32_t *) IO_GPIO_DATA,  0, 1<<0,    -1, -1 }, \
  { /* 33 */  (volatile uint32_t *) IO_GPIO_DATA,  1, 1<<1,    -1, -1 }, \
  { /* 34 */  (volatile uint32_t *) IO_GPIO_DATA,  2, 1<<2,    -1, -1 }, \
  { /* 35 */  (volatile uint32_t *) IO_GPIO_DATA,  3, 1<<3,    -1, -1 }, \
  { /* 36 */  (volatile uint32_t *) IO_GPIO_DATA,  4, 1<<4,    -1, -1 }, \
  { /* 37 */  (volatile uint32_t *) IO_GPIO_DATA,  5, 1<<5,    -1, -1 }, \
  { /* 38 */  (volatile uint32_t *) IO_GPIO_DATA,  6, 1<<6,    -1, -1 }, \
  { /* 39 */  (volatile uint32_t *) IO_GPIO_DATA,  7, 1<<7,    -1, -1 }, \
  { /* 40 */  (volatile uint32_t *) IO_GPIO_DATA,  8, 1<<8,    -1, -1 }, \
  { /* 41 */  (volatile uint32_t *) IO_GPIO_DATA,  9, 1<<9,    -1, -1 }, \
  { /* 42 */  (volatile uint32_t *) IO_GPIO_DATA, 10, 1<<10,   -1, -1 }, \
  { /* 43 */  (volatile uint32_t *) IO_GPIO_DATA, 11, 1<<11,   -1, -1 }, \
  { /* 44 */  (volatile uint32_t *) IO_GPIO_DATA, 12, 1<<12,   -1, -1 }, \
  { /* 45 */  (volatile uint32_t *) IO_GPIO_DATA, 13, 1<<13,   -1, -1 }, \
  { /* 46 */  (volatile uint32_t *) IO_GPIO_DATA, 14, 1<<14,   -1, -1 }, \
  { /* 47 */  (volatile uint32_t *) IO_GPIO_DATA, 15, 1<<15,   -1, -1 }, \
  { /* 48 */  (volatile uint32_t *) IO_GPIO_DATA, 16, 1<<16,   -1, -1 }, \
  { /* 49 */  (volatile uint32_t *) IO_GPIO_DATA, 17, 1<<17,   -1, -1 }, \
  { /* 50 */  (volatile uint32_t *) IO_GPIO_DATA, 18, 1<<18,   -1, -1 }, \
  { /* 51 */  (volatile uint32_t *) IO_GPIO_DATA, 19, 1<<19,   -1, -1 }, \
  { /* 52 */  (volatile uint32_t *) IO_GPIO_DATA, 20, 1<<20,   -1, -1 }, \
  { /* 53 */  (volatile uint32_t *) IO_GPIO_DATA, 21, 1<<21,   -1, -1 }, \
  { /* 54 */  (volatile uint32_t *) IO_GPIO_DATA, 22, 1<<22,   -1, -1 }, \
  { /* 55 */  (volatile uint32_t *) IO_GPIO_DATA, 23, 1<<23,   -1, -1 }, \
  { /* 56 */  (volatile uint32_t *) IO_GPIO_DATA, 24, 1<<24,   -1, -1 }, \
  { /* 57 */  (volatile uint32_t *) IO_GPIO_DATA, 25, 1<<25,   -1, -1 }, \
  { /* 58 */  (volatile uint32_t *) IO_GPIO_DATA, 26, 1<<26,   -1, -1 }, \
  { /* 59 */  (volatile uint32_t *) IO_GPIO_DATA, 27, 1<<27,   -1, -1 }, \
  { /* 60 */  (volatile uint32_t *) IO_GPIO_DATA, 28, 1<<28,   -1, -1 }, \
  { /* 61 */  (volatile uint32_t *) IO_GPIO_DATA, 29, 1<<29,   -1, -1 }, \
  { /* 62 */  (volatile uint32_t *) IO_GPIO_DATA, 30, 1<<30,   -1, -1 }, \
  { /* 63 */  (volatile uint32_t *) IO_GPIO_DATA, 31, 1<<31,   -1, -1 }, \
}

#endif
