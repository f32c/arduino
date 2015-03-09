
#ifndef _f32c_mips_variant_h
#define _f32c_mips_variant_h

#include <stdint.h>

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

#include "emard_timer.h"

#ifdef __cplusplus
extern UARTClass Serial;
#endif

/*
 * Analog pins
 */
static const uint8_t A0  = 16;
static const uint8_t A1  = 17;

/*
** PWM pins
*/
static const uint8_t PWM0  = 14;
static const uint8_t PWM1  = 15;

#endif /* _f32c_mips_variant_h */

