/*
  Copyright (c) 2011-2012 Arduino.  All right reserved.

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

#ifndef _WIRING_INTERRUPTS_
#define _WIRING_INTERRUPTS_

#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

struct variant_icp_control_s
{
  uint32_t control_and;   // first and'ed to preserve bits
  uint32_t control_or;    // last or'ed to set bits that enable this timer
  uint32_t control_and_or;// 1-use and logic, 0-use or logic
  uint32_t apply;
  uint8_t icp_start;
  uint8_t icp_stop;
  uint32_t icp_ie;
  uint32_t icp_if;
  uint8_t icp_register;
};

extern const struct variant_icp_control_s variant_icp_control[];

extern int timerInterval;
void attachInterrupt(uint32_t pin, void (*callback)(void), uint32_t mode);
void detachInterrupt(uint32_t pin);
void icpFilter(uint32_t pin, uint32_t icp_start, uint32_t icp_stop);

#ifdef __cplusplus
}
#endif

#endif /* _WIRING_INTERRUPTS_ */
