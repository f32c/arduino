/*
 * ULX2S board GPIO & hardwired pins
 */

#include "Arduino.h"

#include <io.h>

#ifdef __cplusplus
extern "C" {
#endif


static uint32_t gpio_cfg, gpio_out, led_out;


void
pinMode(uint32_t pin, uint32_t mode)
{

	if (pin >= variant_pin_map_size ||
	    (variant_pin_map[pin].port == (volatile uint32_t *) IO_PUSHBTN))
		return;

	switch (mode) {
	case INPUT:
		gpio_cfg &= ~variant_pin_map[pin].bitmask;
		break ;
	case OUTPUT:
		gpio_cfg |= variant_pin_map[pin].bitmask;
		break ;
	}

	OUTW(IO_GPIO_CTL, gpio_cfg);
}


void
digitalWrite(uint32_t pin, uint32_t val)
{
	volatile uint32_t *port;
	uint32_t *var = &gpio_out;

	if (pin >= variant_pin_map_size)
		return;

	port = variant_pin_map[pin].port;

	if (port == (volatile uint32_t *) IO_PUSHBTN)
		var = &led_out;

	if (val)
		*port = (*var |= variant_pin_map[pin].bitmask);
	else
		*port = (*var &= ~variant_pin_map[pin].bitmask);
}


int
digitalRead(uint32_t pin)
{
	if (pin >= variant_pin_map_size)
		return 0;

	return ((*variant_pin_map[pin].port & variant_pin_map[pin].bitmask) != 0);
}

#ifdef __cplusplus
}
#endif

