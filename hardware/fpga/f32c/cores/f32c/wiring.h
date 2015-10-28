/*
  Copyright (c) 2011 Arduino.  All right reserved.
  Copyright (c) 2013 by Paul Stoffregen <paul@pjrc.com> (delayMicroseconds)

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

#ifndef _WIRING_
#define _WIRING_

__BEGIN_DECLS

/**
 *
 */
extern void initVariant( void ) ;
extern void init( void ) ;

enum PIN_TYPE {
  PIN_TYPE_NC = 0, // not connected
  PIN_TYPE_INPUT,
  PIN_TYPE_OUTPUT,
  PIN_TYPE_GPIO,
  PIN_TYPE_SIZE // must be last, defines the size
};

enum OCP_TYPE {
  OCP_NONE = 15
};

enum ICP_TYPE {
  ICP_NONE = 15
};

extern const volatile uint32_t *pintype2ioaddr[];
extern const volatile uint32_t *pintype2ioaddr_in[];

struct variant_pin_map_s {
	uint8_t	io_port:3,  // max 8 pin types
                bit_pos:5;  // max 32 bits
	uint8_t	pwm:4, icp:4;
};

extern const struct variant_pin_map_s variant_pin_map[];
extern const uint32_t variant_pin_map_size;

/**
 * \brief Returns the number of milliseconds since the Arduino board began running the current program.
 *
 * This number will overflow (go back to zero), after approximately 50 days.
 *
 * \return Number of milliseconds since the program started (uint32_t)
 */
extern uint32_t millis( void ) ;

/**
 * \brief Returns the number of microseconds since the Arduino board began running the current program.
 *
 * This number will overflow (go back to zero), after approximately 70 minutes. On 16 MHz Arduino boards
 * (e.g. Duemilanove and Nano), this function has a resolution of four microseconds (i.e. the value returned is
 * always a multiple of four). On 8 MHz Arduino boards (e.g. the LilyPad), this function has a resolution
 * of eight microseconds.
 *
 * \note There are 1,000 microseconds in a millisecond and 1,000,000 microseconds in a second.
 */
extern uint32_t micros( void ) ;

/**
 * \brief Pauses the program for the amount of time (in miliseconds) specified as parameter.
 * (There are 1000 milliseconds in a second.)
 *
 * \param dwMs the number of milliseconds to pause (uint32_t)
 */
extern void delay( uint32_t dwMs ) ;

/**
 * \brief Pauses the program for the amount of time (in microseconds) specified as parameter.
 *
 * \param dwUs the number of microseconds to pause (uint32_t)
 */
static inline void delayMicroseconds(uint32_t) __attribute__((always_inline, unused));
static inline void delayMicroseconds(uint32_t usec){
    if (usec == 0) return;
#if 0
    // XXX MARKO FIXME!
    uint32_t n = usec * (F_CPU / 3000000);
    asm volatile(
        "L_%=_delayMicroseconds:"       "\n\t"
        "subs   %0, #1"                 "\n\t"
        "bne    L_%=_delayMicroseconds" "\n"
        : "+r" (n) :
    );
#endif
    int32_t endwait = micros() + usec;
    while( ((int32_t)micros())-endwait < 0);
}

__END_DECLS

#endif /* _WIRING_ */
