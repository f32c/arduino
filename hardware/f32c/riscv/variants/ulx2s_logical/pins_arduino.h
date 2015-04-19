#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#define VARIANT_DIGITAL_PIN_MAP \
{ \
/* pin     port          bit  pwm  icp  comment */ \
  [ 0] = { IO_PUSHBTN,    0,  -1, -1 }, \
  [ 1] = { IO_PUSHBTN,    1,  -1, -1 }, \
  [ 2] = { IO_PUSHBTN,    2,  -1, -1 }, \
  [ 3] = { IO_PUSHBTN,    3,  -1, -1 }, \
  [ 4] = { IO_PUSHBTN,    4,  -1, -1 }, \
  [ 5] = { IO_PUSHBTN,    5,  -1, -1 }, \
  [ 6] = { IO_PUSHBTN,    6,  -1, -1 }, \
  [ 7] = { IO_PUSHBTN,    7,  -1, -1 }, \
  [ 8] = { IO_LED,        0,  -1,  0 /* icp0 */ }, \
  [ 9] = { IO_LED,        1,   0, -1 /* ocp0 */ }, \
  [10] = { IO_LED,        2,   1, -1 /* ocp1 */ }, \
  [11] = { IO_LED,        3,  -1,  1 /* icp1 */ }, \
  [12] = { IO_LED,        4,  -1, -1 }, \
  [13] = { IO_LED,        5,  -1, -1 }, \
  [14] = { IO_LED,        6,  -1, -1 /* pwm0 */ }, \
  [15] = { IO_LED,        7,  -1, -1 /* pwm1 */ }, \
  [16] = { IO_PUSHBTN,   16,  -1, -1 }, \
  [17] = { IO_PUSHBTN,   17,  -1, -1 }, \
  [18] = { IO_PUSHBTN,   18,  -1, -1 }, \
  [19] = { IO_PUSHBTN,   19,  -1, -1 }, \
  [20] = { IO_PUSHBTN,   20,  -1, -1 }, \
  [21] = { IO_PUSHBTN,   21,  -1, -1 }, \
  [22] = { IO_PUSHBTN,   22,  -1, -1 }, \
  [23] = { IO_PUSHBTN,   23,  -1, -1 }, \
  [24] = { IO_PUSHBTN,   24,  -1, -1 }, \
  [25] = { IO_PUSHBTN,   25,  -1, -1 }, \
  [26] = { IO_PUSHBTN,   26,  -1, -1 }, \
  [27] = { IO_PUSHBTN,   27,  -1, -1 }, \
  [28] = { IO_PUSHBTN,   28,  -1, -1 }, \
  [29] = { IO_PUSHBTN,   29,  -1, -1 }, \
  [30] = { IO_PUSHBTN,   30,  -1, -1 }, \
  [31] = { IO_PUSHBTN,   31,  -1, -1 }, \
  [32] = { IO_GPIO_DATA,  0,  -1, -1 }, \
  [33] = { IO_GPIO_DATA,  1,  -1, -1 }, \
  [34] = { IO_GPIO_DATA,  2,  -1, -1 }, \
  [35] = { IO_GPIO_DATA,  3,  -1, -1 }, \
  [36] = { IO_GPIO_DATA,  4,  -1, -1 }, \
  [37] = { IO_GPIO_DATA,  5,  -1, -1 }, \
  [38] = { IO_GPIO_DATA,  6,  -1, -1 }, \
  [39] = { IO_GPIO_DATA,  7,  -1, -1 }, \
  [40] = { IO_GPIO_DATA,  8,  -1, -1 }, \
  [41] = { IO_GPIO_DATA,  9,  -1, -1 }, \
  [42] = { IO_GPIO_DATA, 10,  -1, -1 }, \
  [43] = { IO_GPIO_DATA, 11,  -1, -1 }, \
  [44] = { IO_GPIO_DATA, 12,  -1, -1 }, \
  [45] = { IO_GPIO_DATA, 13,  -1, -1 }, \
  [46] = { IO_GPIO_DATA, 14,  -1, -1 }, \
  [47] = { IO_GPIO_DATA, 15,  -1, -1 }, \
  [48] = { IO_GPIO_DATA, 16,  -1, -1 }, \
  [49] = { IO_GPIO_DATA, 17,  -1, -1 }, \
  [50] = { IO_GPIO_DATA, 18,  -1, -1 }, \
  [51] = { IO_GPIO_DATA, 19,  -1, -1 }, \
  [52] = { IO_GPIO_DATA, 20,  -1, -1 }, \
  [53] = { IO_GPIO_DATA, 21,  -1, -1 }, \
  [54] = { IO_GPIO_DATA, 22,  -1, -1 }, \
  [55] = { IO_GPIO_DATA, 23,  -1, -1 }, \
  [56] = { IO_GPIO_DATA, 24,  -1, -1 }, \
  [57] = { IO_GPIO_DATA, 25,  -1, -1 }, \
  [58] = { IO_GPIO_DATA, 26,  -1, -1 }, \
  [59] = { IO_GPIO_DATA, 27,  -1, -1 }, \
  [60] = { IO_GPIO_DATA, 28,  -1, -1 }, \
  [61] = { IO_GPIO_DATA, 29,  -1, -1 }, \
  [62] = { IO_GPIO_DATA, 30,  -1, -1 }, \
  [63] = { IO_GPIO_DATA, 31,  -1, -1 }, \
}

#endif
