/*
  Copyright (c) 2014 Arduino.  All right reserved.

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

extern "C" void __cxa_pure_virtual(void) __attribute__ ((__noreturn__));
extern "C" void __cxa_deleted_virtual(void) __attribute__ ((__noreturn__));

void __cxa_pure_virtual(void) {
  // We might want to write some diagnostics to uart in this case
  //std::terminate();
  while (1)
    ;
}

void __cxa_deleted_virtual(void) {
  // We might want to write some diagnostics to uart in this case
  //std::terminate();
  while (1)
    ;
}

// EMARD: I'm not sure is "abi.cpp" right place,
// but for compilation of some libraries like MIDI Library from Forty Seven Effects
// it will fix errors like:
// Callbacks.ino.cpp:(.text.startup._GLOBAL__sub_I_MIDI+0xb4): undefined reference to `__dso_handle'
// Callbacks.ino.cpp:(.text.startup._GLOBAL__sub_I_MIDI+0xbc): undefined reference to `__cxa_atexit'
__BEGIN_DECLS
void *__dso_handle = (void*) &__dso_handle;
int __cxa_atexit(void (_destructor) (void *), void *arg, void *dso) { return (0);}
__END_DECLS;
