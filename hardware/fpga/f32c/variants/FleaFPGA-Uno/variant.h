
#ifndef _f32c_mips_variant_h
#define _f32c_mips_variant_h

#include <stdint.h>
#include <dev/io.h>

#define __f32c_FleaFPGA_Uno__	1	// Use to identify FleaFPGA-Uno board features

/* LEDs */
#define PIN_LED_13           32
#define PIN_LED              32
#define LED_BUILTIN          32

#define LED1_BUILTIN         33
#define LED2_BUILTIN         33
#define BUTTON_BUILTIN       48
#define BUTTON1_BUILTIN      48

static const uint8_t SS   = 10;		// TODO: determine FleaFPGA-Uno SPI pin numbers...
static const uint8_t MOSI = 11;
static const uint8_t MISO = 12;
static const uint8_t SCK  = 13;

static const uint8_t SDA = 18;
static const uint8_t SCL = 19;

/*
 * Analog pins
 */
#define	IO_ADC_A0	IO_ADDR(0x740)	/* half, RD */
#define IO_ADC_MINVAL	32		/* minimum value from hardware ADC */
#define IO_ADC_MAXVAL	1952	/* maximum value from hardware ADC */
 
static const uint8_t A0  = 14;
static const uint8_t A1  = 15;
static const uint8_t A2  = 16;
static const uint8_t A3  = 17;
static const uint8_t A4  = 18;
static const uint8_t A5  = 19;

/*
** PWM pins
*/
static const uint8_t PWM0  = 14;	// TODO: do these make sense for FleaFPGA-Uno?
static const uint8_t PWM1  = 15;

/*
** 2nd UART (ESP-8266)
*/
#define	IO_SIO_BYTE_1	IO_ADDR(0x310)	/* byte, RW */
#define	IO_SIO_STATUS_1	IO_ADDR(0x311)	/* byte, RD */
#define	IO_SIO_BAUD_1	IO_ADDR(0x312)	/* half, WR */

/*
** Arduino Uno AVR ATmega328 compatibility defines
*/
#define PORTA	(((volatile uint8_t *)IO_GPIO_DATA)[0])
#define PORTA0	0
#define PORTA1	1
#define PORTA2	2
#define PORTA3	3
#define PORTA4	4
#define PORTA5	5
#define PORTA6	6
#define PORTA7	7
#define	DDRA	(((volatile uint8_t *) IO_GPIO_CTL)[0])
#define DDRA0	0
#define DDRA1	1
#define DDRA2	2
#define DDRA3	3
#define DDRA4	4
#define DDRA5	5
#define DDRA6	6
#define DDRA7	7
#define PINA	(((volatile uint8_t *)IO_GPIO_INPUT)[0])
#define PINA0	0
#define PINA1	1
#define PINA2	2
#define PINA3	3
#define PINA4	4
#define PINA5	5
#define PINA6	6
#define PINA7	7
#define PORTB	(((volatile uint8_t *)IO_GPIO_DATA)[1])
#define PORTB0	0
#define PORTB1	1
#define PORTB2	2
#define PORTB3	3
#define PORTB4	4
#define PORTB5	5
#define PORTB6	6
#define PORTB7	7
#define	DDRB	(((volatile uint8_t *) IO_GPIO_CTL)[1])
#define DDRB0	0
#define DDRB1	1
#define DDRB2	2
#define DDRB3	3
#define DDRB4	4
#define DDRB5	5
#define DDRB6	6
#define DDRB7	7
#define PINB	(((volatile uint8_t *)IO_GPIO_INPUT)[1])
#define PINB0	0
#define PINB1	1
#define PINB2	2
#define PINB3	3
#define PINB4	4
#define PINB5	5
#define PINB6	6
#define PINB7	7
#define PORRTC	(((volatile uint8_t *)IO_GPIO_DATA)[2])
#define PORTC0	0
#define PORTC1	1
#define PORTC2	2
#define PORTC3	3
#define PORTC4	4
#define PORTC5	5
#define PORTC6	6
#define PORTC7	7
#define	DDRC	(((volatile uint8_t *) IO_GPIO_CTL)[2])
#define DDRC0	0
#define DDRC1	1
#define DDRC2	2
#define DDRC3	3
#define DDRC4	4
#define DDRC5	5
#define DDRC6	6
#define DDRC7	7
#define PINC	(((volatile uint8_t *)IO_GPIO_INPUT)[2])
#define PINC0	0
#define PINC1	1
#define PINC2	2
#define PINC3	3
#define PINC4	4
#define PINC5	5
#define PINC6	6
#define PORTD	(((volatile uint8_t *)IO_GPIO_DATA)[3])
#define PORTD0	0
#define PORTD1	1
#define PORTD2	2
#define PORTD3	3
#define PORTD4	4
#define PORTD5	5
#define PORTD6	6
#define PORTD7	7
#define	DDRD	(((volatile uint8_t *) IO_GPIO_CTL)[3])
#define DDRD0	0
#define DDRD1	1
#define DDRD2	2
#define DDRD3	3
#define DDRD4	4
#define DDRD5	5
#define DDRD6	6
#define DDRD7	7
#define PIND	(((volatile uint8_t *)IO_GPIO_INPUT)[3])
#define PIND0	0
#define PIND1	1
#define PIND2	2
#define PIND3	3
#define PIND4	4
#define PIND5	5
#define PIND6	6
#define PIND7	7

#include "Arduino.h"

#include "emard_timer.h"

#ifdef __cplusplus
#include "UARTClass.h"

extern UARTClass Serial;			// FTDI USB UART
//TODO: Xark - 2nd UART extern UARTClass<IO_SIO_BYTE_1, IO_SIO_STATUS_1, IO_SIO_BAUD_1> Serial1;	// ESP-8266 UART
#endif

#endif /* _f32c_mips_variant_h */

