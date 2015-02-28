/*
 * ULX2S board GPIO & hardwired pins
 */

#include "Arduino.h"

#include <io.h>

#ifdef __cplusplus
extern "C" {
#endif


static uint32_t gpio_cfg, gpio_out, led_out;


void delay(uint32_t ms)
{

	/* XXX hardwired, imprecise */
	DELAY(ms * 81250);
}


void
pinMode(uint32_t pin, uint32_t mode)
{

	if (pin > 32)
		return;

	switch (mode) {
        case INPUT:
		gpio_cfg &= ~(1 << pin);
        break ;
        case OUTPUT:
		gpio_cfg |= (1 << pin);
        break ;
	}

	OUTW(IO_GPIO_CTL, gpio_cfg);
}


void
digitalWrite(uint32_t pin, uint32_t val)
{

	if (pin > 64)
		return;

	if (pin < 32) {
		if (val)
			gpio_out |= (1 << pin);
		else
			gpio_out &= ~(1 << pin);
		OUTW(IO_GPIO_DATA, gpio_out);
	} else {
		pin &= 0x7;
		if (val)
			led_out |= (1 << pin);
		else
			led_out &= ~(1 << pin);
		OUTB(IO_LED, led_out);
	}
}


int
digitalRead(uint32_t pin)
{
	int val;

	if (pin > 64)
		return (0);

	if (pin < 32) {
		INW(val, IO_GPIO_DATA);
		return ((val >> pin) & 1);
	} else {
		pin &= 0x7;
		INB(val, IO_PUSHBTN);
		return ((val >> pin) & 1);
	}
}

#ifdef __cplusplus
}
#endif

