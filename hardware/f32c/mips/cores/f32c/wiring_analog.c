/* EMARD */

#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

uint32_t analogRead(uint32_t ulPin)
{
  return 0;
}

void analogWrite(uint32_t ulPin, uint32_t ulValue)
{
  if(ulPin >= variant_pin_map_size)
    return;
  if(variant_pin_map[ulPin].pwm >= 0)
  {
    /* todo: set PWM output with some default frequency of 25 kHz 
    ** we need to recompile bitstream with different prescaler
    ** 25000L*TIMER_CLOCK>>(TIMER_BITS+PRESCALER_BITS);
    */
    EMARD_TIMER[TC_INCREMENT] = (1024 * (uint64_t) TIMER_CLOCK) >> (TIMER_BITS+PRESCALER_BITS);
    EMARD_TIMER[TC_APPLY] = (1<<TC_INCREMENT);

    EMARD_TIMER[TC_CONTROL] = (1<<TCTRL_AND_OR_OCP1) | (1<<TCTRL_AND_OR_OCP2)
	                    | (1<<TCTRL_AND_OR_ICP1) | (1<<TCTRL_AND_OR_ICP2)
	                    | (0<<TCTRL_IE_OCP1)     | (0<<TCTRL_IE_OCP2)
	                    | (0<<TCTRL_IE_ICP1)     | (0<<TCTRL_IE_ICP2)
	                    | (0<<TCTRL_AFCEN_ICP1)  | (0<<TCTRL_AFCINV_ICP1)
	                    | (0<<TCTRL_AFCEN_ICP2)  | (0<<TCTRL_AFCINV_ICP2)
	                    | (0<<TCTRL_XOR_OCP1)    | (0<<TCTRL_XOR_OCP2)
	                    | (1<<TCTRL_XOR_ICP1)    | (1<<TCTRL_XOR_ICP2)
	                  ;
    
    if(variant_pin_map[ulPin].pwm == 0)
    {
      EMARD_TIMER[TC_OCP1_START] = 0;
      EMARD_TIMER[TC_OCP1_STOP]  = ulValue < (1<<TIMER_BITS) ? ulValue : 1<<TIMER_BITS;

      EMARD_TIMER[TC_APPLY] = (1<<TC_CONTROL)
                            | (1<<TC_OCP1_START) | (1<<TC_OCP1_STOP) 
                            | (0<<TC_OCP2_START) | (0<<TC_OCP2_STOP)
                            | (0<<TC_ICP1_START) | (0<<TC_ICP1_STOP) 
                            | (0<<TC_ICP2_START) | (0<<TC_ICP2_STOP)
                            | (0<<TC_INC_MIN)    | (0<<TC_INC_MAX)
                            | (0<<TC_ICP1)       | (0<<TC_ICP2)
                          ;
    }
      
    if(variant_pin_map[ulPin].pwm == 1)
    {
      EMARD_TIMER[TC_OCP2_START] = 0;
      EMARD_TIMER[TC_OCP2_STOP]  = ulValue < (1<<TIMER_BITS) ? ulValue : 1<<TIMER_BITS;

      EMARD_TIMER[TC_APPLY] = (1<<TC_CONTROL)
                            | (0<<TC_OCP1_START) | (0<<TC_OCP1_STOP) 
                            | (1<<TC_OCP2_START) | (1<<TC_OCP2_STOP)
                            | (0<<TC_ICP1_START) | (0<<TC_ICP1_STOP) 
                            | (0<<TC_ICP2_START) | (0<<TC_ICP2_STOP)
                            | (0<<TC_INC_MIN)    | (0<<TC_INC_MAX)
                            | (0<<TC_ICP1)       | (0<<TC_ICP2)
                          ;
    }

  }
}
