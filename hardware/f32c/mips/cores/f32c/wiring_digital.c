/*
 * ULX2S board GPIO & hardwired pins
 */

#include "Arduino.h"

#include <io.h>

#ifdef __cplusplus
extern "C" {
#endif


static uint32_t gpio_cfg, gpio_out, led_out;

struct variant_pin_mapping
{
  volatile uint32_t *port;
  uint32_t bitmask;
};

struct variant_pin_mapping digital_pin_map[] = VARIANT_DIGITAL_PIN_MAP;

void delay(uint32_t ms)
{

	/* XXX hardwired, imprecise */
	DELAY(ms * 81250);
}


void
pinMode(uint32_t pin, uint32_t mode)
{

	if (pin < 32 || pin > 64)
		return;

	switch (mode) {
        case INPUT:
		gpio_cfg &= ~digital_pin_map[pin].bitmask;
        break ;
        case OUTPUT:
		gpio_cfg |=  digital_pin_map[pin].bitmask;
        break ;
	}

	OUTW(IO_GPIO_CTL, gpio_cfg);
}


void
digitalWrite(uint32_t pin, uint32_t val)
{
	volatile uint32_t *port;
	uint32_t *var = &gpio_out;

	if (pin > 64)
		return;

	port = digital_pin_map[pin].port;

	if(port == (volatile uint32_t *) IO_PUSHBTN)
		var = &led_out;

	if(val)
		*port = (*var |=  digital_pin_map[pin].bitmask);
	else
		*port = (*var &= ~digital_pin_map[pin].bitmask);
}


int
digitalRead(uint32_t pin)
{
	if (pin > 64)
		return 0;

	return (*digital_pin_map[pin].port & digital_pin_map[pin].bitmask) == 0 ? 0 : 1;
}

#ifdef __cplusplus
}
#endif

