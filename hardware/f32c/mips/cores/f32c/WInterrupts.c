#include <Arduino.h>
#include <io.h>
#include <mips/asm.h>
#include <mips/cpuregs.h>
#include <sys/isr.h>
#include "wiring_private.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 8 main MIPS interrupts, all initially disabled */
static volatile voidFuncPtr intFunc[8] = 
  { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, };
/* some interrupts sources are multiplexed on same mips irq, 
   here are callbacks for timer */
static volatile voidFuncPtr timerFunc[4] = {
    NULL, NULL, // OCP1, OCP2
    NULL, NULL, // ICP1, ICP2
};

static int tsc_next;
int timerInterval = VARIANT_MCK; // default timer interval is 1 second

/* MIPS timer (inside of the core) */
static int tsc_isr(void)
{
  if(intFunc[7])
  {
    intFunc[7]();
    tsc_next += timerInterval;
    mtc0_macro(tsc_next, MIPS_COP_0_COMPARE);
  }
  return 1;
}
static struct isr_link tsc_isr_link = {.handler_fn = &tsc_isr};

/* emard advanced timer, memory mapped I/O (outside of the core) */
static int timer_isr(void)
{
  // check hardware registers to determine source of interrupt
  if( (EMARD_TIMER[TC_CONTROL] & (1<<TCTRL_IF_OCP1)) != 0 )
  {
    EMARD_TIMER[TC_CONTROL] &= ~(1<<TCTRL_IF_OCP1); // clear interrupt flag
    if(timerFunc[0])
      timerFunc[0]();
  }
  if( (EMARD_TIMER[TC_CONTROL] & (1<<TCTRL_IF_OCP2)) != 0 )
  {
    EMARD_TIMER[TC_CONTROL] &= ~(1<<TCTRL_IF_OCP2); // clear interrupt flag
    if(timerFunc[1])
      timerFunc[1]();
  }
  return 1;
}
static struct isr_link timer_isr_link = {.handler_fn = &timer_isr};

void attachInterrupt(uint32_t pin, void (*callback)(void), uint32_t mode)
{
  int32_t irq = -1;
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
    uint8_t init_required = 0;
    irq = 7;
    if(mode)
      timerInterval = mode;
    if(intFunc[irq] == NULL)
      init_required = 1;
    intFunc[irq] = callback; // todo - set it above, 
    if(init_required)
    {
      isr_register_handler(irq, &tsc_isr_link); // 7 is MIPS timer interrput
      mfc0_macro(tsc_next, MIPS_COP_0_COUNT);
      tsc_next += timerInterval;
      mtc0_macro(tsc_next, MIPS_COP_0_COMPARE);
    }
    asm("ei");
  }
  if(pin == 14 || pin == 15)
  {
    irq = 4;
    if(intFunc[irq] == NULL)
    {
      isr_register_handler(irq, &timer_isr_link); // 4 is EMARD timer interrput
      intFunc[irq] = NULL+1; // not used as callback, just as non-zero to init only once
    }
    if(pin == 14)
    {
      timerFunc[0] = callback;
      EMARD_TIMER[TC_CONTROL] |= (1<<TCTRL_IE_OCP1);
      EMARD_TIMER[TC_APPLY] = (1<<TC_CONTROL);
    }
    if(pin == 15)
    {
      timerFunc[1] = callback;
      EMARD_TIMER[TC_CONTROL] |= (1<<TCTRL_IE_OCP2);
      EMARD_TIMER[TC_APPLY] = (1<<TC_CONTROL);
    }
    asm("ei");
  }
}

void detachInterrupt(uint32_t pin)
{
  if(pin == 13)
  {
    int irq = 7;
    asm("di");
    #if 0
    isr_remove_handler(irq, &tsc_isr_link); // 7 is MIPS timer interrput
    #endif
    intFunc[irq] = NULL;
    asm("ei");
  }
  if(pin == 14 || pin == 15)
  {
    if(pin == 14)
    {
      EMARD_TIMER[TC_CONTROL] &= ~(1<<TCTRL_IE_OCP1);
      EMARD_TIMER[TC_APPLY] = (1<<TC_CONTROL);
      timerFunc[0] = NULL;
    }
    if(pin == 15)
    {
      EMARD_TIMER[TC_CONTROL] &= ~(1<<TCTRL_IE_OCP2);
      EMARD_TIMER[TC_APPLY] = (1<<TC_CONTROL);
      timerFunc[1] = NULL;
    }
    if( (EMARD_TIMER[TC_CONTROL] 
        & ( (1<<TCTRL_IE_OCP1)
          | (1<<TCTRL_IE_OCP2) 
          | (1<<TCTRL_IE_ICP1)
          | (1<<TCTRL_IE_ICP2)
          )
        ) == 0
      )
    {
      #if 0
      int irq = 4;
      asm("di");
      isr_remove_handler(irq, &timer_isr_link); // 4 is EMARD timer interrput
      intFunc[irq] = NULL;
      asm("ei");
      #endif
    }
  }
}

#ifdef __cplusplus
}
#endif
