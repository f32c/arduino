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

#ifndef _UART_CLASS_
#define _UART_CLASS_

#include "HardwareSerial.h"

template <int byte_reg, int status_reg, int baud_reg>
class UARTClass : public HardwareSerial
{
  public:
    void begin(unsigned long dwBaudRate);
    void end(void);
    int available(void);
    int availableForWrite(void);
    int peek(void);
    int read(void);
    void flush(void);
    size_t write(const uint8_t c);
    using Print::write; // pull in write(str) and write(buf, size) from Print

    operator bool() {return (true);}; // UART always active

  protected:
//    void init(const uint32_t dwBaudRate);

  private:
	enum
	{
		SIO_RXBUFSIZE =	(1 << 3),
		SIO_RXBUFMASK = (SIO_RXBUFSIZE - 1)
	};

	char sio_rxbuf[SIO_RXBUFSIZE];
	uint8_t sio_rxbuf_head;
	uint8_t sio_rxbuf_tail;
	uint8_t sio_tx_xoff;
	
	__attribute__((optimize("-Os"))) int sio_probe_rx(void);
	__attribute__((optimize("-Os"))) int sio_getchar(int blocking);
	__attribute__((optimize("-Os"))) int sio_putchar(int c, int blocking);
	__attribute__((optimize("-Os"))) void sio_setbaud(int bauds);
};

#endif // _UART_CLASS_
