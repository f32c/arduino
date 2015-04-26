/*
  Copyright (c) 2011 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "UARTClass.h"

#include <io.h>


#define	SIO_RXBUFSIZE	(1 << 3)
#define	SIO_RXBUFMASK	(SIO_RXBUFSIZE - 1)

static char sio_rxbuf[SIO_RXBUFSIZE];
static uint8_t sio_rxbuf_head;
static uint8_t sio_rxbuf_tail;
static uint8_t sio_tx_xoff;

UARTClass Serial;


static __attribute__((optimize("-Os"))) int
sio_probe_rx(void)
{
	int c, s;

	INB(s, IO_SIO_STATUS);
	if (s & SIO_RX_FULL) {
		INB(c, IO_SIO_BYTE);
		if (c == 0x13) {
			/* XOFF */
			sio_tx_xoff = 1;
			return(s);
		}
		if (c == 0x11) {
			/* XON */
			sio_tx_xoff = 0;
			return(s);
		}
		sio_rxbuf[sio_rxbuf_head++] = c;
		sio_rxbuf_head &= SIO_RXBUFMASK;
	}
	return(s);
}


static __attribute__((optimize("-Os"))) int
sio_getchar(int blocking)
{
	int c, busy;

	/* Any new characters received from RS-232? */
	do {
		sio_probe_rx();
		busy = (sio_rxbuf_head == sio_rxbuf_tail);
	} while (blocking && busy);

	if (busy)
		return (-1);
	c = sio_rxbuf[sio_rxbuf_tail++];
	sio_rxbuf_tail &= SIO_RXBUFMASK;
	return (c);
}


static __attribute__((optimize("-Os"))) int
sio_putchar(int c, int blocking)
{
	int in, busy;

	do {
		in = sio_probe_rx();
		busy = (in & SIO_TX_BUSY) || sio_tx_xoff;
	} while (blocking && busy);

	if (busy == 0)
		OUTB(IO_SIO_BYTE, c);
	return (busy);
}


/*
 * Set RS-232 baudrate.  Works well with FT-232R from 300 to 3000000 bauds.
 */
static __attribute__((optimize("-Os"))) void
sio_setbaud(int bauds)
{
	uint32_t val, freq_khz;

#if 0
	mfc0_macro(val, MIPS_COP_0_CONFIG);
	freq_khz = ((val >> 16) & 0xfff) * 1000 / ((val >> 29) + 1);
#else
	//freq_khz = VARIANT_MCK / 1000;
	freq_khz = 100000; /* XXX testing only, hardcoded, FIXME!!! */
#endif

	val = bauds;
	if (bauds > 1000000)
		val /= 10;
	val = val * 1024 / 1000 * 1024 / freq_khz + 1;
	if (bauds > 1000000)
		val *= 10;
	OUTH(IO_SIO_BAUD, val);
}


// Public Methods //////////////////////////////////////////////////////////////

void
UARTClass::begin(unsigned long bauds)
{
	
	sio_setbaud(bauds);
}


void
UARTClass::end(void)
{
}


int
UARTClass::available(void)
{

	sio_probe_rx();
	return (!(sio_rxbuf_head == sio_rxbuf_tail));
}


int
UARTClass::availableForWrite(void)
{

	return (1);
}


int
UARTClass::peek(void)
{

	sio_probe_rx();
	if (sio_rxbuf_tail == sio_rxbuf_head)
		return (-1);
	else
		return (sio_rxbuf[sio_rxbuf_tail]);
}


int
UARTClass::read(void)
{

	return (sio_getchar(1));
}


void
UARTClass::flush(void)
{
}


size_t
UARTClass::write(const uint8_t uc_data)
{

	sio_putchar(uc_data, 1);
	return (1);
}
