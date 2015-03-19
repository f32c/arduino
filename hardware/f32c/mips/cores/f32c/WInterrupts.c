#include <io.h>
#include <mips/asm.h>
#include <mips/cpuregs.h>
#include <sys/isr.h>
#include "wiring_private.h"

static volatile voidFuncPtr intFunc[EXTERNAL_NUM_INTERRUPTS] = {NULL, NULL};
static int tsc_next;
int timerInterval = VARIANT_MCK; // default timer interval is 1 second

#if 0
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
}
#endif

static int tsc_isr(void)
{
  if(intFunc[0])
  {
    intFunc[0]();
    tsc_next += timerInterval;
    mtc0_macro(tsc_next, MIPS_COP_0_COMPARE);
  }
  return 1;
}

static struct isr_link tsc_isr_link = {.handler_fn = &tsc_isr};

void attachInterrupt(uint32_t pin, void (*callback)(void), uint32_t mode)
{
  /* attachInterrupt is ment to assign pin change interrupt
  ** on digital input pins
  ** but we will here misuse it to create timer interrupt.
  ** LED pin 13 is chosen as 'magic' pin which caries
  ** MIPS timer irq7 interrupt and has nothing to do with LED.
  ** 
  ** mode parameter (if nonzero) is used as timer interval
  ** if zero, timer interval is unchanged (default is 1 second) 
  ** (timerInterval = system clock value in Hz) 
  ** it can be changed at any time
  ** but beware of race condition
  */
  if(pin == 13)
  {
    if(mode)
      timerInterval = mode;
    if(intFunc[0] == NULL)
    {
      isr_register_handler(7, &tsc_isr_link); // 7 is MIPS timer interrput
      mfc0_macro(tsc_next, MIPS_COP_0_COUNT);
      tsc_next += timerInterval;
      mtc0_macro(tsc_next, MIPS_COP_0_COMPARE);
    }
    intFunc[0] = callback;
    asm("ei");
  }
}

void detachInterrupt(uint32_t pin)
{
}
