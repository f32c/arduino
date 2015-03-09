/* EMARD */

#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

static uint8_t analog_write_resolution_bits = 8;

/* old arduino uses 490 Hz */
// #define ANALOG_WRITE_PWM_FREQUENCY_HZ 490
/* new arduino uses 980 Hz */
#define ANALOG_WRITE_PWM_FREQUENCY_HZ 980

uint32_t analogRead(uint32_t ulPin)
{
  return 0;
}

/* todo: support analogWriteResolution(bits)
** default is 8 for old arduino,
** new arduino can use up to 12
*/
void analogWriteResolution(int res)
{
  analog_write_resolution_bits = res;
}

void analogWrite(uint32_t ulPin, uint32_t ulValue)
{
  if(ulPin >= variant_pin_map_size)
    return;
  if(variant_pin_map[ulPin].pwm >= 0)
  {
    /* standard PWM frequency is 
    ** 490 Hz on old arduino
    ** 980 Hz in new arduino
    ** for better high freq coverage and compatibility with
    ** 12-bit analogWrite we need to recompile bitstream with 
    ** PRESCALER_BITS=10 and with TIMER_BITS=12
    ** to calculate increment:
    ** increment = (frequency_hz << (TIMER_BITS+PRESCALER_BITS) ) / TIMER_CLOCK_hz;
    */
    EMARD_TIMER[TC_INCREMENT] = (((uint64_t)ANALOG_WRITE_PWM_FREQUENCY_HZ) 
                                   << (TIMER_BITS+PRESCALER_BITS)) / TIMER_CLOCK;

    EMARD_TIMER[TC_CONTROL] = (1<<TCTRL_AND_OR_OCP1) | (1<<TCTRL_AND_OR_OCP2)
	                    | (1<<TCTRL_AND_OR_ICP1) | (1<<TCTRL_AND_OR_ICP2)
	                    | (0<<TCTRL_IE_OCP1)     | (0<<TCTRL_IE_OCP2)
	                    | (0<<TCTRL_IE_ICP1)     | (0<<TCTRL_IE_ICP2)
	                    | (0<<TCTRL_AFCEN_ICP1)  | (0<<TCTRL_AFCINV_ICP1)
	                    | (0<<TCTRL_AFCEN_ICP2)  | (0<<TCTRL_AFCINV_ICP2)
	                    | (0<<TCTRL_XOR_OCP1)    | (0<<TCTRL_XOR_OCP2)
	                    | (1<<TCTRL_XOR_ICP1)    | (1<<TCTRL_XOR_ICP2)
	                  ;
	                  
    /* shift timer step value to match the set resolution */
    if(analog_write_resolution_bits < TIMER_BITS)
      ulValue <<= (TIMER_BITS-analog_write_resolution_bits);
    if(analog_write_resolution_bits > TIMER_BITS)
      ulValue >>= (analog_write_resolution_bits-analog_write_resolution_bits);
    
    if(variant_pin_map[ulPin].pwm == 0)
    {
      EMARD_TIMER[TC_OCP1_START] = 0;
      EMARD_TIMER[TC_OCP1_STOP]  = ulValue < (1<<TIMER_BITS) ? ulValue : (1<<TIMER_BITS);

      EMARD_TIMER[TC_APPLY] = (1<<TC_CONTROL)
                            | (1<<TC_INCREMENT)
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
      EMARD_TIMER[TC_OCP2_STOP]  = ulValue < (1<<TIMER_BITS) ? ulValue : (1<<TIMER_BITS);

      EMARD_TIMER[TC_APPLY] = (1<<TC_CONTROL)
                            | (1<<TC_INCREMENT)
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
