#include <sys/isr.h>
#include "wiring_private.h"

static volatile voidFuncPtr intFunc[EXTERNAL_NUM_INTERRUPTS];


void attachInterrupt(uint32_t pin, void (*callback)(void), uint32_t mode)
{
  if(pin == 14)
  {
    intFunc[0] = callback;
    EMARD_TIMER[TC_CONTROL] |= 
       (1<<TCTRL_IE_OCP1) | (0<<TCTRL_IE_OCP2)
     | (0<<TCTRL_IE_ICP1) | (0<<TCTRL_IE_ICP2);
    EMARD_TIMER[TC_APPLY] = (1<<TC_CONTROL);
  }
  if(pin == 15)
  {
    intFunc[1] = callback;
    EMARD_TIMER[TC_CONTROL] |= 
       (0<<TCTRL_IE_OCP1) | (1<<TCTRL_IE_OCP2)
     | (0<<TCTRL_IE_ICP1) | (0<<TCTRL_IE_ICP2);
    EMARD_TIMER[TC_APPLY] = (1<<TC_CONTROL);
  }
  // (*intFunc[0])();
}

void detachInterrupt(uint32_t pin)
{
}

static int wiring_timer_interrupt(void)
{
  if(intFunc[0])
  {
    if( (EMARD_TIMER[TC_CONTROL] & (1<<TCTRL_IF_OCP1)) != 0 )
    {
      EMARD_TIMER[TC_CONTROL] = ~(1<<TCTRL_IF_OCP1);
      // EMARD_TIMER[TC_APPLY] = (1<<TC_CONTROL);
      intFunc[0]();
    }
  }
  if(intFunc[1])
  {
    if( (EMARD_TIMER[TC_CONTROL] & (1<<TCTRL_IF_OCP2)) != 0 )
    {
      EMARD_TIMER[TC_CONTROL] = ~(1<<TCTRL_IF_OCP2);
      // EMARD_TIMER[TC_APPLY] = (1<<TC_CONTROL);
      intFunc[1]();
    }
  }
  return 0;
}

static struct isr_link timer_interrupt_isr = {
        .handler_fn = &wiring_timer_interrupt
};

void enableInterrupt(void)
{
  isr_register_handler(VARIANT_TIMER_INTERRUPT, &timer_interrupt_isr);
  asm("ei");
}
