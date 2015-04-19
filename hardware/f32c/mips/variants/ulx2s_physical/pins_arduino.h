#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#define VARIANT_DIGITAL_PIN_MAP \
{ \
/* arduino pin number               port         bit   pwm  icp comment*/ \
  { /*  0 */  (volatile uint32_t *) IO_PUSHBTN,   -1,   -1, -1 }, \
  { /*  1 */  (volatile uint32_t *) IO_PUSHBTN,   -1,   -1, -1 }, \
  { /*  2 */  (volatile uint32_t *) IO_GPIO_DATA,  0,   -1, -1  /* lcd3 */ }, \
  { /*  3 */  (volatile uint32_t *) IO_GPIO_DATA,  1,   -1, -1  /* lcd2 */ }, \
  { /*  4 */  (volatile uint32_t *) IO_GPIO_DATA,  2,   -1, -1  /* lcd1 */ }, \
  { /*  5 */  (volatile uint32_t *) IO_LED,        7,   -1, -1  /* led7 */ }, \
  { /*  6 */  (volatile uint32_t *) IO_LED,        6,   -1, -1  /* led6 */ }, \
  { /*  7 */  (volatile uint32_t *) IO_LED,        5,   -1, -1  /* led5 */ }, \
  { /*  8 */  (volatile uint32_t *) IO_GPIO_DATA,  3,   -1, -1  /* lcd0 */ }, \
  { /*  9 */  (volatile uint32_t *) IO_GPIO_DATA,  4,   -1, -1  /* lcd5 */ }, \
  { /* 10 */  (volatile uint32_t *) IO_PUSHBTN,    2,   -1, -1  /* btn_down */ }, \
  { /* 11 */  (volatile uint32_t *) IO_PUSHBTN,    1,   -1, -1  /* btn_left */ }, \
  { /* 12 */  (volatile uint32_t *) IO_LED,        4,   -1, -1  /* led 4 */ }, \
  { /* 13 */  (volatile uint32_t *) IO_GPIO_DATA,  5,   -1, -1  /* lcd4 */ }, \
  { /* 14 */  (volatile uint32_t *) IO_GPIO_DATA,  6,   -1, -1 }, \
  { /* 15 */  (volatile uint32_t *) IO_GPIO_DATA,  7,   -1, -1 }, \
  { /* 16 */  (volatile uint32_t *) IO_GPIO_DATA,  8,   -1, -1 }, \
  { /* 17 */  (volatile uint32_t *) IO_GPIO_DATA,  9,   -1, -1 }, \
  { /* 18 */  (volatile uint32_t *) IO_GPIO_DATA, 10,   -1, -1 }, \
  { /* 19 */  (volatile uint32_t *) IO_GPIO_DATA, 11,   -1, -1 }, \
  { /* 20 */  (volatile uint32_t *) IO_GPIO_DATA, 12,   -1, -1 }, \
  { /* 21 */  (volatile uint32_t *) IO_GPIO_DATA, 13,   -1, -1 }, \
  { /* 22 */  (volatile uint32_t *) IO_GPIO_DATA, 14,   -1, -1 }, \
  { /* 23 */  (volatile uint32_t *) IO_GPIO_DATA, 15,   -1, -1 }, \
  { /* 24 */  (volatile uint32_t *) IO_PUSHBTN,   -1,   -1, -1 }, \
  { /* 25 */  (volatile uint32_t *) IO_PUSHBTN,   -1,   -1, -1 }, \
  { /* 26 */  (volatile uint32_t *) IO_GPIO_DATA, 16,   -1, -1 }, \
  { /* 27 */  (volatile uint32_t *) IO_GPIO_DATA, 17,   -1, -1 }, \
  { /* 28 */  (volatile uint32_t *) IO_GPIO_DATA, 18,   -1, -1 }, \
  { /* 29 */  (volatile uint32_t *) IO_GPIO_DATA, 19,   -1, -1 }, \
  { /* 30 */  (volatile uint32_t *) IO_GPIO_DATA, 20,   -1, -1 }, \
  { /* 31 */  (volatile uint32_t *) IO_GPIO_DATA, 21,   -1, -1 }, \
  { /* 32 */  (volatile uint32_t *) IO_GPIO_DATA, 22,   -1, -1 }, \
  { /* 33 */  (volatile uint32_t *) IO_GPIO_DATA, 23,   -1, -1 }, \
  { /* 34 */  (volatile uint32_t *) IO_GPIO_DATA, 24,   -1, -1 }, \
  { /* 35 */  (volatile uint32_t *) IO_GPIO_DATA, 25,   -1, -1 }, \
  { /* 36 */  (volatile uint32_t *) IO_GPIO_DATA, 26,   -1, -1 }, \
  { /* 37 */  (volatile uint32_t *) IO_GPIO_DATA, 27,   -1, -1 }, \
  { /* 38 */  (volatile uint32_t *) IO_PUSHBTN,    0,   -1, -1  /* btn_right */ }, \
  { /* 39 */  (volatile uint32_t *) IO_PUSHBTN,    4,   -1, -1  /* btn_center */ }, \
  { /* 40 */  (volatile uint32_t *) IO_GPIO_DATA, 28,   -1, -1 }, \
  { /* 41 */  (volatile uint32_t *) IO_LED,        0,   -1,  0  /* led_0, icp0 */ }, \
  { /* 42 */  (volatile uint32_t *) IO_LED,        1,    0, -1  /* led_1, ocp0 */ }, \
  { /* 43 */  (volatile uint32_t *) IO_LED,        2,    1, -1  /* led_2, ocp1 */ }, \
  { /* 44 */  (volatile uint32_t *) IO_LED,        3,   -1,  1  /* led_3, icp1 */ }, \
  { /* 45 */  (volatile uint32_t *) IO_GPIO_DATA,  0,   -1  /* p_tip_0 */ }, \
  { /* 46 */  (volatile uint32_t *) IO_GPIO_DATA,  0,   -1  /* p_tip_1 */ }, \
  { /* 47 */  (volatile uint32_t *) IO_GPIO_DATA,  0,   -1  /* p_tip_2 */ }, \
  { /* 48 */  (volatile uint32_t *) IO_GPIO_DATA,  0,   -1 }, \
  { /* 49 */  (volatile uint32_t *) IO_GPIO_DATA,  0,   -1 }, \
  { /* 50 */  (volatile uint32_t *) IO_GPIO_DATA,  0,   -1 }, \
  { /* 51 */  (volatile uint32_t *) IO_GPIO_DATA,  0,   -1 }, \
  { /* 52 */  (volatile uint32_t *) IO_GPIO_DATA,  0,   -1 }, \
  { /* 53 */  (volatile uint32_t *) IO_GPIO_DATA,  0,   -1 }, \
  { /* 54 */  (volatile uint32_t *) IO_GPIO_DATA,  0,   -1 }, \
  { /* 55 */  (volatile uint32_t *) IO_GPIO_DATA,  0,   -1 }, \
  { /* 56 */  (volatile uint32_t *) IO_GPIO_DATA,  0,   -1 }, \
  { /* 57 */  (volatile uint32_t *) IO_GPIO_DATA,  0,   -1 }, \
  { /* 58 */  (volatile uint32_t *) IO_GPIO_DATA,  0,   -1 }, \
  { /* 59 */  (volatile uint32_t *) IO_GPIO_DATA,  0,   -1 }, \
  { /* 60 */  (volatile uint32_t *) IO_GPIO_DATA,  0,   -1 }, \
  { /* 61 */  (volatile uint32_t *) IO_GPIO_DATA,  0,   -1 }, \
  { /* 62 */  (volatile uint32_t *) IO_GPIO_DATA,  0,   -1 }, \
  { /* 63 */  (volatile uint32_t *) IO_GPIO_DATA,  0,   -1 }, \
}

#endif
