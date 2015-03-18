#include "wiring_private.h"

static volatile voidFuncPtr intFunc[EXTERNAL_NUM_INTERRUPTS];

void attachInterrupt(uint32_t pin, void (*callback)(void), uint32_t mode)
{
}

void detachInterrupt(uint32_t pin)
{
}

