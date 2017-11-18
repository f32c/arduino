#include <Arduino.h>
#include <mips/asm.h>
#include <mips/cpuregs.h>
#include <sys/isr.h>
#include <dev/io.h>
#include "wiring_private.h"
#include "emard_timer.h"

__BEGIN_DECLS

#ifdef __mips__
/* F32C interrupts are available only on MIPS */

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

static volatile voidFuncPtr gpio_rising_Func[32] = {
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
};
static volatile voidFuncPtr gpio_falling_Func[32] = {
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
};

const struct variant_icp_control_s variant_icp_control[2] = VARIANT_ICP_CONTROL;

static int tsc_next;
int timerInterval = F_CPU; // default timer interval is 1 second

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
      EMARD_TIMER[TC_CONTROL] |= timerIFlags[i]; // clear the interrupt flag
      if(timerFunc[i])
        timerFunc[i](); // call the function
    }
  }
  return 1;
}
static struct isr_link timer_isr_link = {.handler_fn = &timer_isr};

/* gpio with interrupts */
static int gpio_isr(void)
{
  int8_t i;
  uint32_t bit;
  volatile uint32_t *if_rising  = (volatile uint32_t *)IO_GPIO_RISE_IF;
  volatile uint32_t *if_falling = (volatile uint32_t *)IO_GPIO_FALL_IF;

  for(i = 31, bit = (1<<31); i >= 0; i--, bit >>= 1)
  {
    if( (bit & *if_rising) != 0 )
    {
      *if_rising = bit; // clear flag with implicit &=
      if(gpio_rising_Func[i])
        gpio_rising_Func[i]();
    }
    if( (bit & *if_falling) != 0 )
    {
      *if_falling = bit; // clear flag with implicit &=
      if(gpio_falling_Func[i])
        gpio_falling_Func[i]();
    }
  }
  return 1;
}
static struct isr_link gpio_isr_link = {.handler_fn = &gpio_isr};

static int serialrx_isr(void)
{
  if(intFunc[3])
    intFunc[3]();
  return 1;
}
static struct isr_link serialrx_isr_link = {.handler_fn = &serialrx_isr};

void icpFilter(uint32_t pin, uint32_t icp_start, uint32_t icp_stop)
{
  int8_t icp_channel;
  volatile uint32_t *start, *stop;
  
  if(pin >= variant_pin_map_size)
    return;
  
  icp_channel = variant_pin_map[pin].icp;
  if(icp_channel != ICP_NONE)
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
  int8_t icp, ocp, bit;
  volatile uint32_t *port;
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
  bit = variant_pin_map[pin].bit_pos;
  port = (PortRegister_t)digitalPinToPort(pin);
  
  if(bit == 5 && port == (volatile uint32_t *) IO_LED) // MIPS timer irq magic pin: arduino LED at pin 13
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
      isr_register_handler(irq, &tsc_isr_link); // 7 is MIPS timer interrupt
      mfc0_macro(tsc_next, MIPS_COP_0_COUNT);
      tsc_next += timerInterval;
      mtc0_macro(tsc_next, MIPS_COP_0_COMPARE);
    }
    asm("ei");
  }

  if(bit == 7 && port == (volatile uint32_t *) IO_LED) // serial RX irq magic pin: arduino LED at pin 15
  {
    irq = 3; // sio rx MIPS interrupt 3
    if(intFunc[irq] == NULL)
    {
      intFunc[irq] = callback;
      isr_register_handler(irq, &serialrx_isr_link); // 1 is serial rx interrupt
      asm("ei");
    }
  }

  if(ocp != OCP_NONE || icp != ICP_NONE)
  {
    irq = VARIANT_TIMER_INTERRUPT;
    if(intFunc[irq] == NULL)
    {
      isr_register_handler(irq, &timer_isr_link); // 4 is EMARD timer interrupt
      intFunc[irq] = NULL+1; // not used as callback, just as non-zero to init only once
    }
    if(ocp != OCP_NONE)
    {
      timerFunc[ocp] = callback;
      EMARD_TIMER[TC_CONTROL] |= pwm_enable_bitmask[ocp].ocp_ie;
      EMARD_TIMER[TC_APPLY] = (1<<TC_CONTROL);
    }
    if(icp != ICP_NONE)
    {
      timerFunc[VARIANT_OCPN+icp] = callback;
      EMARD_TIMER[TC_CONTROL] |= variant_icp_control[icp].icp_ie;
      EMARD_TIMER[TC_APPLY] = (1<<TC_CONTROL);
    }
    asm("ei");
  }
  if (port == (volatile uint32_t *) IO_GPIO_DATA)
  {
    volatile uint32_t *ie_rising  = (volatile uint32_t *)IO_GPIO_RISE_IE;
    volatile uint32_t *ie_falling = (volatile uint32_t *)IO_GPIO_FALL_IE;
    irq = 5; // VARIANT_GPIO_INTERRUPT
    /* standard GPIO pin */
    if(bit >= 0)
    {
      // warning problematic: if sketch using GPIO interrupt is uploaded again
      // the *ie_rising and *ie_falling must be cleared at startup
      // otherwise isr_register_handler() will not be initialized,
      // or in simple words, GPIO interrupts won't work.
      if(*ie_rising == 0 && *ie_falling == 0)
        isr_register_handler(irq, &gpio_isr_link); // 5 is gpio interrupt
      if(mode == RISING)
      {
        gpio_rising_Func[bit] = callback;
        *ie_rising |= (1<<bit);
      }
      if(mode == FALLING)
      {
        gpio_falling_Func[bit] = callback;
        *ie_falling |= (1<<bit);
      }
      asm("ei");
    }
  }
}

void detachInterrupt(uint32_t pin)
{
  int8_t icp, ocp, bit;
  volatile uint32_t *port;
  if(pin >= variant_pin_map_size)
    return;
  icp = variant_pin_map[pin].icp;
  ocp = variant_pin_map[pin].pwm;
  bit = variant_pin_map[pin].bit_pos;
  port = (PortRegister_t)digitalPinToPort(pin);
  if(bit == 5 && port == (volatile uint32_t *) IO_LED) // MIPS timer irq magic pin: arduino LED at pin 13
  {
    int irq = 7;
    asm("di");
    isr_remove_handler(irq, &tsc_isr_link); // 7 is MIPS timer interrupt
    intFunc[irq] = NULL;
    asm("ei");
  }
  if(bit == 7 && port == (volatile uint32_t *) IO_LED) // serial RX irq magic pin: arduino LED at pin 15
  {
    int irq = 3; // MIPS CPU interrupt 3 is serial rx
    asm("di");
    isr_remove_handler(irq, &tsc_isr_link); // 7 is MIPS timer interrupt
    intFunc[irq] = NULL;
    asm("ei");
  }
  if(ocp != OCP_NONE || icp != ICP_NONE)
  {
    if(ocp != OCP_NONE)
    {
      EMARD_TIMER[TC_CONTROL] &= ~pwm_enable_bitmask[ocp].ocp_ie;
      EMARD_TIMER[TC_APPLY] = (1<<TC_CONTROL);
      timerFunc[icp] = NULL;
    }
    if(icp != ICP_NONE)
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
      int irq = VARIANT_TIMER_INTERRUPT;
      asm("di");
      isr_remove_handler(irq, &timer_isr_link); // 4 is EMARD timer interrupt
      intFunc[irq] = NULL;
      asm("ei");
    }
  }
  if (port == (volatile uint32_t *)IO_GPIO_DATA)
  {
    volatile uint32_t *ie_rising =  (volatile uint32_t *)IO_GPIO_RISE_IE;
    volatile uint32_t *ie_falling = (volatile uint32_t *)IO_GPIO_FALL_IE;
    int irq = 5; // VARIANT_GPIO_INTERRUPT
    /* standard GPIO pin */
    if(bit >= 0)
    {
      *ie_rising  &= ~(1<<bit);
      *ie_falling &= ~(1<<bit);
      if(*ie_rising == 0 && *ie_falling == 0)
      {
        asm("di");
        isr_remove_handler(irq, &gpio_isr_link); // 5 is gpio interrupt
        asm("ei");
      }
    }        
  }
}
#endif // __mips__

#ifdef __riscv
/* F32C RISC-V is early and currently doesn't have interrupts
** when implemented, #ifdef __riscv
** can be used for code than needs to be specifically compiled
** for riscv architecture
*/
#warning "RISC-V doesn't have interrupts yet. Choose Tools->CPU Architecture->MIPS"
#endif // __riscv

__END_DECLS
