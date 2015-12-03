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


UARTClass Serial;
#if defined(IO_SIO_BYTE_1)
UARTClass Serial1(IO_SIO_BYTE_1);
#endif

int
UARTClass::sio_probe_rx()
{
  int c, s;

  s = serbase[IO_SIO_STATUS-IO_SIO_BYTE];
  if (s & SIO_RX_FULL) {
    c = serbase[IO_SIO_BYTE-IO_SIO_BYTE];
    if (!(tx_xoff & 0x80)) {
      if (c == 0x13) {
        /* XOFF */
        tx_xoff = 1;
        return(s);
      }
      if (c == 0x11) {
        /* XON */
        tx_xoff = 0;
        return(s);
      }
    }
    sio_rxbuf[sio_rxbuf_head++] = c;
    sio_rxbuf_head &= SIO_RXBUFMASK;
  }
  return(s);
}


int
UARTClass::sio_getchar(int blocking)
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


int
UARTClass::sio_putchar(int c, int blocking)
{
  int in, busy;

  do {
    in = sio_probe_rx();
    busy = (in & SIO_TX_BUSY) || (tx_xoff & 1);
  } while (blocking && busy);

  if (busy == 0)
    serbase[IO_SIO_BYTE-IO_SIO_BYTE] = c;
  return (busy);
}


/*
 * Set RS-232 baudrate.  Works well with FT-232R from 300 to 3000000 bauds.
 */
void
UARTClass::sio_setbaud(int bauds)
{
  uint32_t val;

  val = bauds;
  if (bauds > 1000000)
    val /= 10;
  val = val * 1024 / 1000 * 1024 / (F_CPU / 1000) + 1;
  if (bauds > 1000000)
    val *= 10;

  volatile uint16_t  *hp = (volatile uint16_t *)&serbase[IO_SIO_BAUD-IO_SIO_BYTE];
  *hp = val;
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
  int in, busy;

  in = sio_probe_rx();
  busy = (in & SIO_TX_BUSY) || (tx_xoff & 1);
  
  return (!busy);
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
