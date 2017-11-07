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

#include <dev/io.h>
#include "HardwareSerial.h"

class UARTClass : public HardwareSerial
{
  public:
    UARTClass(uint32_t base = IO_SIO_BYTE) : serbase(reinterpret_cast<uint8_t*>(base)), tx_xoff(0) {}
  
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

    // f32c extension (default is XON/XOFF disabled - so input is 8-bit transparent)
    void setXoffXon(bool enable) { if (enable) tx_xoff |= 0x80; else tx_xoff &= ~0x80; }

  protected:
    int sio_probe_rx();
    int sio_getchar(int blocking);
    int sio_putchar(int c, int blocking);
    void sio_setbaud(int bauds);
    
    enum {
      SIO_RXBUFSIZE = (1 << 3),
      SIO_RXBUFMASK = (SIO_RXBUFSIZE - 1)
    };

    volatile uint8_t *serbase;  // base address of SIO register for port
    volatile uint8_t  tx_xoff;  // bit 7 set enables Xon/Xoff flow control (default disabled)
    volatile uint8_t  sio_rxbuf_head;
    volatile uint8_t  sio_rxbuf_tail;
    char              sio_rxbuf[SIO_RXBUFSIZE];
};

#endif // _UART_CLASS_
