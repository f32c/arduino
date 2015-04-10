#include <Arduino.h>
#include <io.h>
#include <mips/asm.h>
#include <mips/cpuregs.h>
#include <sys/isr.h>
#include "wiring_private.h"
#include "emard_timer.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 8 main MIPS interrupts, all initially disabled */
static volatile voidFuncPtr intFunc[8] = 
  { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, };

/* some interrupts sources are multiplexed on same mips irq, 
   here are callbacks for timer */
static volatile voidFuncPtr timerFunc[VARIANT_ICPN+VARIANT_OCPN] = {
    NULL, NULL, // OCP1, OCP2
    NULL, NULL, // ICP1, ICP2
};
/* todo: join flags with timeFunc with above in a suitable struct
** struct will help programmer that interrupt flags match called functions
*/
static uint32_t timerIFlags[VARIANT_ICPN+VARIANT_OCPN] = {
  1<<TCTRL_IF_OCP1, 1<<TCTRL_IF_OCP2,
  1<<TCTRL_IF_ICP1, 1<<TCTRL_IF_ICP2,
};

const struct variant_icp_control_s variant_icp_control[2] = VARIANT_ICP_CONTROL;

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
  int8_t i;
  
  for(i = VARIANT_ICPN+VARIANT_OCPN-1; i >= 0; i--)
  {
    if( (EMARD_TIMER[TC_CONTROL] & timerIFlags[i]) != 0 )
    {
      EMARD_TIMER[TC_CONTROL] &= ~timerIFlags[i]; // clear the interrupt flag
      if(timerFunc[i])
        timerFunc[i](); // call the function
    }
  }
  return 1;
}
static struct isr_link timer_isr_link = {.handler_fn = &timer_isr};

void icpFilter(uint32_t pin, uint32_t icp_start, uint32_t icp_stop)
{
  int8_t icp_channel;
  volatile uint32_t *start, *stop;
  
  if(pin >= variant_pin_map_size)
    return;
  
  icp_channel = variant_pin_map[pin].icp;
  if(icp_channel >= 0)
  {
    EMARD_TIMER[TC_CONTROL] &= variant_icp_control[icp_channel].control_and;
    
    start = &EMARD_TIMER[variant_icp_control[icp_channel].icp_start];
    stop  = &EMARD_TIMER[variant_icp_control[icp_channel].icp_stop];
    
    *start = icp_start;
    *stop = icp_stop;
    
    EMARD_TIMER[TC_CONTROL] &= variant_icp_control[icp_channel].control_and;
    if( start <= stop )
      EMARD_TIMER[TC_CONTROL] |= variant_icp_control[icp_channel].control_and_or;
    EMARD_TIMER[TC_CONTROL] |= variant_icp_control[icp_channel].control_or;
    EMARD_TIMER[TC_APPLY] = variant_icp_control[icp_channel].apply;
  }
}

void attachInterrupt(uint32_t pin, void (*callback)(void), uint32_t mode)
{
  int32_t irq = -1;
  int8_t icp, ocp;
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
  if(pin >= variant_pin_map_size)
    return;
  
  icp = variant_pin_map[pin].icp;
  ocp = variant_pin_map[pin].pwm;
  
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

  if(ocp >= 0 || icp >= 0)
  {
    irq = VARIANT_TIMER_INTERRUPT;
    if(intFunc[irq] == NULL)
    {
      isr_register_handler(irq, &timer_isr_link); // 4 is EMARD timer interrput
      intFunc[irq] = NULL+1; // not used as callback, just as non-zero to init only once
    }
    if(ocp >= 0)
    {
      timerFunc[ocp] = callback;
      EMARD_TIMER[TC_CONTROL] |= pwm_enable_bitmask[ocp].ocp_ie;
      EMARD_TIMER[TC_APPLY] = (1<<TC_CONTROL);
    }
    if(icp >= 0)
    {
      timerFunc[VARIANT_OCPN+icp] = callback;
      EMARD_TIMER[TC_CONTROL] |= variant_icp_control[icp].icp_ie;
      EMARD_TIMER[TC_APPLY] = (1<<TC_CONTROL);
    }
    asm("ei");
  }
}

void detachInterrupt(uint32_t pin)
{
  int8_t icp, ocp;
  if(pin >= variant_pin_map_size)
    return;
  icp = variant_pin_map[pin].icp;
  ocp = variant_pin_map[pin].pwm;
  if(pin == 13)
  {
    int irq = 7;
    asm("di");
    #if 1
    isr_remove_handler(irq, &tsc_isr_link); // 7 is MIPS timer interrput
    #endif
    intFunc[irq] = NULL;
    asm("ei");
  }
  if(ocp >= 0 || icp >= 0)
  {
    if(ocp >= 0)
    {
      EMARD_TIMER[TC_CONTROL] &= ~pwm_enable_bitmask[ocp].ocp_ie;
      EMARD_TIMER[TC_APPLY] = (1<<TC_CONTROL);
      timerFunc[icp] = NULL;
    }
    if(icp >= 0)
    {
      EMARD_TIMER[TC_CONTROL] &= ~variant_icp_control[icp].icp_ie;
      EMARD_TIMER[TC_APPLY] = (1<<TC_CONTROL);
      timerFunc[VARIANT_OCPN+icp] = NULL;
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
      #if 1
      int irq = VARIANT_TIMER_INTERRUPT;
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
