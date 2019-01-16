/*
  main.cpp - Main loop for Arduino sketches
  Copyright (c) 2005-2013 Arduino Team.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#define ARDUINO_MAIN
#include "Arduino.h"

void f32c_specific_initialization(void)
{
  // disable interrupts and clear their flags

  // GPIO interrupts are handled specific
  // because same pin can have falling and rising interrupt
  // while we cannot register MIPS ISR more than once to the
  // same interrupt source so current content of GPIO flags
  // is checked for this. We need to clean it at startup
  // otherwise GPIO interrupt will work only for the first
  // sketch upload after bitstream is uploaded.
  *((volatile uint32_t *)IO_GPIO_RISE_IE) = 0;
  *((volatile uint32_t *)IO_GPIO_FALL_IE) = 0;
  *((volatile uint32_t *)IO_GPIO_RISE_IF) = ~0;
  *((volatile uint32_t *)IO_GPIO_FALL_IF) = ~0;
}

/*
 * \brief Main entry point of Arduino application
 */
int main( void )
{
//	init();
        // Generic boards will not have f32c specific intialization.
        // This is support for picorv32+attosoc which should
        // work when generic RISC-V board is selected
        #if __F32C__ == 1
        f32c_specific_initialization();
        #endif
	setup();

	do {
		loop();
		if (serialEventRun)
			serialEventRun();
	} while (1);

	return 0;
}
