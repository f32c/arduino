
#ifndef _f32c_mips_variant_h
#define _f32c_mips_variant_h

/* CPU clock frequency */
#define VARIANT_MCK	81250000

/* LEDs */
#define PIN_LED_13           13
#define PIN_LED              13
#define LED_BUILTIN          13

#include "Arduino.h"
#ifdef __cplusplus
#include "UARTClass.h"
#endif

#ifdef __cplusplus
extern UARTClass Serial;
#endif

#endif /* _f32c_mips_variant_h */

