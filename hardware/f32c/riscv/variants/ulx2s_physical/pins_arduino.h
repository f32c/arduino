#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#define VU32P(x) ((x) & 0x7ff)
#define VARIANT_DIGITAL_PIN_MAP \
{ \
/* pin     port                 bit   pwm  icp   comment */ \
  [ 0] = { VU32P(IO_PUSHBTN),   -1,   -1, -1 }, \
  [ 1] = { VU32P(IO_PUSHBTN),   -1,   -1, -1 }, \
  [ 2] = { VU32P(IO_GPIO_DATA),  0,   -1, -1  /* lcd3 */ }, \
  [ 3] = { VU32P(IO_GPIO_DATA),  1,   -1, -1  /* lcd2 */ }, \
  [ 4] = { VU32P(IO_GPIO_DATA),  2,   -1, -1  /* lcd1 */ }, \
  [ 5] = { VU32P(IO_LED),        7,   -1, -1 }, \
  [ 6] = { VU32P(IO_LED),        6,   -1, -1 }, \
  [ 7] = { VU32P(IO_LED),        5,   -1, -1 }, \
  [ 8] = { VU32P(IO_GPIO_DATA),  3,   -1, -1  /* lcd0 */ }, \
  [ 9] = { VU32P(IO_GPIO_DATA),  4,   -1, -1  /* lcd5 */ }, \
  [10] = { VU32P(IO_PUSHBTN),    2,   -1, -1  /* btn_down */ }, \
  [11] = { VU32P(IO_PUSHBTN),    1,   -1, -1  /* btn_left */ }, \
  [12] = { VU32P(IO_LED),        4,   -1, -1 }, \
  [13] = { VU32P(IO_GPIO_DATA),  5,   -1, -1  /* lcd4 */ }, \
  [14] = { VU32P(IO_GPIO_DATA),  6,   -1, -1 }, \
  [15] = { VU32P(IO_GPIO_DATA),  7,   -1, -1 }, \
  [16] = { VU32P(IO_GPIO_DATA),  8,   -1, -1 }, \
  [17] = { VU32P(IO_GPIO_DATA),  9,   -1, -1 }, \
  [18] = { VU32P(IO_GPIO_DATA), 10,   -1, -1 }, \
  [19] = { VU32P(IO_GPIO_DATA), 11,   -1, -1 }, \
  [20] = { VU32P(IO_GPIO_DATA), 12,   -1, -1 }, \
  [21] = { VU32P(IO_GPIO_DATA), 13,   -1, -1 }, \
  [22] = { VU32P(IO_GPIO_DATA), 14,   -1, -1 }, \
  [23] = { VU32P(IO_GPIO_DATA), 15,   -1, -1 }, \
  [24] = { VU32P(IO_PUSHBTN),   -1,   -1, -1 }, \
  [25] = { VU32P(IO_PUSHBTN),   -1,   -1, -1 }, \
  [26] = { VU32P(IO_GPIO_DATA), 16,   -1, -1 }, \
  [27] = { VU32P(IO_GPIO_DATA), 17,   -1, -1 }, \
  [28] = { VU32P(IO_GPIO_DATA), 18,   -1, -1 }, \
  [29] = { VU32P(IO_GPIO_DATA), 19,   -1, -1 }, \
  [30] = { VU32P(IO_GPIO_DATA), 20,   -1, -1 }, \
  [31] = { VU32P(IO_GPIO_DATA), 21,   -1, -1 }, \
  [32] = { VU32P(IO_GPIO_DATA), 22,   -1, -1 }, \
  [33] = { VU32P(IO_GPIO_DATA), 23,   -1, -1 }, \
  [34] = { VU32P(IO_GPIO_DATA), 24,   -1, -1 }, \
  [35] = { VU32P(IO_GPIO_DATA), 25,   -1, -1 }, \
  [36] = { VU32P(IO_GPIO_DATA), 26,   -1, -1 }, \
  [37] = { VU32P(IO_GPIO_DATA), 27,   -1, -1 }, \
  [38] = { VU32P(IO_PUSHBTN),    0,   -1, -1  /* btn_right */ }, \
  [39] = { VU32P(IO_PUSHBTN),    4,   -1, -1  /* btn_center */ }, \
  [40] = { VU32P(IO_GPIO_DATA), 28,   -1, -1 }, \
  [41] = { VU32P(IO_LED),        0,   -1,  0  /* icp0 */ }, \
  [42] = { VU32P(IO_LED),        1,    0, -1  /* ocp0 */ }, \
  [43] = { VU32P(IO_LED),        2,    1, -1  /* ocp1 */ }, \
  [44] = { VU32P(IO_LED),        3,   -1,  1  /* icp1 */ }, \
  [45] = { VU32P(IO_GPIO_DATA),  0,   -1, -1  /* p_tip_0 */ }, \
  [46] = { VU32P(IO_GPIO_DATA),  0,   -1, -1  /* p_tip_1 */ }, \
  [47] = { VU32P(IO_GPIO_DATA),  0,   -1, -1  /* p_tip_2 */ }, \
  [48] = { VU32P(IO_GPIO_DATA),  0,   -1, -1}, \
  [49] = { VU32P(IO_GPIO_DATA),  0,   -1, -1}, \
  [50] = { VU32P(IO_GPIO_DATA),  0,   -1, -1}, \
  [51] = { VU32P(IO_GPIO_DATA),  0,   -1, -1}, \
  [52] = { VU32P(IO_GPIO_DATA),  0,   -1, -1}, \
  [53] = { VU32P(IO_GPIO_DATA),  0,   -1, -1}, \
  [54] = { VU32P(IO_GPIO_DATA),  0,   -1, -1}, \
  [55] = { VU32P(IO_GPIO_DATA),  0,   -1, -1}, \
  [56] = { VU32P(IO_GPIO_DATA),  0,   -1, -1}, \
  [57] = { VU32P(IO_GPIO_DATA),  0,   -1, -1}, \
  [58] = { VU32P(IO_GPIO_DATA),  0,   -1, -1}, \
  [59] = { VU32P(IO_GPIO_DATA),  0,   -1, -1}, \
  [60] = { VU32P(IO_GPIO_DATA),  0,   -1, -1}, \
  [61] = { VU32P(IO_GPIO_DATA),  0,   -1, -1}, \
  [62] = { VU32P(IO_GPIO_DATA),  0,   -1, -1}, \
  [63] = { VU32P(IO_GPIO_DATA),  0,   -1, -1}, \
}

#endif
