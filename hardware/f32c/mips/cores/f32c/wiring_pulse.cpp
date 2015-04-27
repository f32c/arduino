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

#include "Arduino.h"
//#include "wiring_private.h"


/* Measures the length (in microseconds) of a pulse on the pin; state is HIGH
 * or LOW, the type of pulse to measure.  Works on pulses from 2-3 microseconds
 * to 3 minutes in length, but must be called at least a few dozen microseconds
 * before the start of the pulse. */
extern uint32_t
pulseIn(uint32_t pin, bool state, uint32_t timeout)
{
	uint32_t numloops = 0;
	uint32_t maxloops = microsecondsToClockCycles(timeout) / 16;
	uint32_t start, end;
	
	// wait for any previous pulse to end
	while (digitalRead(pin) == state)
		if (numloops++ == maxloops)
			return 0;
	
	// wait for the pulse to start
	while (digitalRead(pin) != state)
		if (numloops++ == maxloops)
			return 0;
	
	// wait for the pulse to stop
	start = micros();
	while (digitalRead(pin) == state)
		if (numloops++ == maxloops)
			return 0;
	end  = micros();

	return (end - start);
}

/* use advanced timer input capture digital logic 
to automatically capture the asynchronous or very fast pulse.

Frees the CPU from busy waiting (user's program can continue
to execute while digital logic listens to the pulse.

Pulse should keep state for at least 3 CPU clock cycles to be 
captured.

This will work when 2 dedicated input capture pins are both 
connected to the incoming pulse source.

Maximum pulse width that can be measured is the period of the
PWM cycle of the timer. use analogWriteFrequency() to set this.

If several pulses arrive before reading, only the last 
events of leading and trailing edges will be stored in 
ICP registers.

The best way to read pulse is attachInterrupt on ICP2.
Read can be done without interrupt in time window after 
trailing edge of the pulse of interest 
and before leading edge of the next pulse.

pin with ICP1 captures the leading edge
pin with ICP2 captures the trailing edge

pin1, pin2 = pins of the input capture logic
state = HIGH to measure from rising to falling edge
state = LOW to measure from falling to rising edge

call this function after setting digtalWriteFrequency()
icpFilter() and similar functions which alter timer 
registers that control ICP.
*/
extern void pulseListen(uint32_t pin1, uint32_t pin2, bool state)
{
  /* TODO: input parameters pin1 and pin2 currently ignored
   * as there's only one timer with 2 icp units, ICP1 and ICP2
   */
  /* completely open ICP Filter - captures every incoming pulse edge */
  EMARD_TIMER[TC_ICP1_START] = 0;
  EMARD_TIMER[TC_ICP1_STOP] = 0;

  EMARD_TIMER[TC_ICP2_START] = 0;
  EMARD_TIMER[TC_ICP2_STOP] = 0;

  EMARD_TIMER[TC_CONTROL] &= ~(
    (1<<TCTRL_AND_OR_ICP1)
  | (1<<TCTRL_AND_OR_ICP2)
  | (1<<TCTRL_XOR_ICP1)
  | (1<<TCTRL_XOR_ICP2)
  );
  
  EMARD_TIMER[TC_CONTROL] |= 
    (0<<TCTRL_AND_OR_ICP1) 
  | (0<<TCTRL_AND_OR_ICP2)
  | ((state == HIGH ? 0 : 1)<<TCTRL_XOR_ICP1)
  | ((state == HIGH ? 1 : 0)<<TCTRL_XOR_ICP2)
  ;
  
  EMARD_TIMER[TC_APPLY] = 
    (1<<TC_CONTROL)
  | (1<<TC_ICP1_START)
  | (1<<TC_ICP1_STOP)
  | (1<<TC_ICP2_START)
  | (1<<TC_ICP2_STOP)
  ;
}

/* reads measured pulse width (in timer ticks)
 * by subtracting timer input capture values and
 * wrapping them to bitsize of the timer counter
 */
extern uint32_t pulseRead(void)
{
  return (EMARD_TIMER[TC_ICP2] - EMARD_TIMER[TC_ICP1]) & ((1<<TIMER_BITS)-1);
}

/*
** convert timer ticks to microseconds
*/
extern uint32_t timerTicks2us(uint32_t t)
{
  return
    ( 
      ((uint64_t) t) << PRESCALER_BITS
    ) * 1000000 / ( EMARD_TIMER[TC_INCREMENT] * TIMER_CLOCK)
    ;
}
