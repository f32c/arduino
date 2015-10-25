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

#include <dev/io.h>

#include "variant.h"

UARTClass<IO_SIO_BYTE, IO_SIO_STATUS, IO_SIO_BAUD>  Serial;

#if defined(IO_SIO_BYTE_1)
UARTClass<IO_SIO_BYTE_1, IO_SIO_STATUS_1, IO_SIO_BAUD_1> Serial1;
#endif

template <int byte_reg, int status_reg, int baud_reg>
int UARTClass<byte_reg, status_reg, baud_reg>::sio_probe_rx(void)
{
	int c, s;

	INB(s, status_reg);
	if (s & SIO_RX_FULL) {
		INB(c, byte_reg);
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

template <int byte_reg, int status_reg, int baud_reg>
int UARTClass<byte_reg, status_reg, baud_reg>::sio_getchar(int blocking)
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


template <int byte_reg, int status_reg, int baud_reg>
int UARTClass<byte_reg, status_reg, baud_reg>::sio_putchar(int c, int blocking)
{
	int in, busy;

	do {
		in = sio_probe_rx();
		busy = (in & SIO_TX_BUSY) || sio_tx_xoff;
	} while (blocking && busy);

	if (busy == 0)
		OUTB(byte_reg, c);
	return (busy);
}


/*
 * Set RS-232 baudrate.  Works well with FT-232R from 300 to 3000000 bauds.
 */
template <int byte_reg, int status_reg, int baud_reg>
void UARTClass<byte_reg, status_reg, baud_reg>::sio_setbaud(int bauds)
{
	uint32_t val;


	val = bauds;
	if (bauds > 1000000)
		val /= 10;
	val = val * 1024 / 1000 * 1024 / (F_CPU / 1000) + 1;
	if (bauds > 1000000)
		val *= 10;
	OUTH(baud_reg, val);
}

// Public Methods //////////////////////////////////////////////////////////////

template <int byte_reg, int status_reg, int baud_reg>
void UARTClass<byte_reg, status_reg, baud_reg>::begin(unsigned long bauds)
{
	
	sio_setbaud(bauds);
}

template <int byte_reg, int status_reg, int baud_reg>
void UARTClass<byte_reg, status_reg, baud_reg>::end(void)
{
}

template <int byte_reg, int status_reg, int baud_reg>
int UARTClass<byte_reg, status_reg, baud_reg>::available(void)
{

	sio_probe_rx();
	return (!(sio_rxbuf_head == sio_rxbuf_tail));
}

template <int byte_reg, int status_reg, int baud_reg>
int UARTClass<byte_reg, status_reg, baud_reg>::availableForWrite(void)
{

	return (1);
}

template <int byte_reg, int status_reg, int baud_reg>
int UARTClass<byte_reg, status_reg, baud_reg>::peek(void)
{

	sio_probe_rx();
	if (sio_rxbuf_tail == sio_rxbuf_head)
		return (-1);
	else
		return (sio_rxbuf[sio_rxbuf_tail]);
}


template <int byte_reg, int status_reg, int baud_reg>
int UARTClass<byte_reg, status_reg, baud_reg>::read(void)
{

	return (sio_getchar(1));
}


template <int byte_reg, int status_reg, int baud_reg>
void UARTClass<byte_reg, status_reg, baud_reg>::flush(void)
{
}

template <int byte_reg, int status_reg, int baud_reg>
size_t UARTClass<byte_reg, status_reg, baud_reg>::write(const uint8_t uc_data)
{

	sio_putchar(uc_data, 1);
	return (1);
}
