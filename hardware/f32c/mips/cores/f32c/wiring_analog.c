/* EMARD */

#include "Arduino.h"
#include "wiring_analog.h"

#ifdef __cplusplus
extern "C" {
#endif

static uint8_t analog_write_resolution_bits = 8;
/* old arduino uses 490 Hz */
/* new arduino uses 980 Hz */
static uint32_t analog_write_frequency = 980;

const struct pwm_enable_bitmask_s pwm_enable_bitmask[] = VARIANT_PWM_CHANNEL_ENABLE;

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

void analogWriteFrequency(int freq)
{
  analog_write_frequency = freq;
}

/* setup the common parameters (why isn't it called at startup?)
*/
void analogOutputInit( void )
{
}

void analogWrite(uint32_t ulPin, uint32_t ulValue)
{
  int32_t pwm_channel;
  
  if(ulPin >= variant_pin_map_size)
    return;
    
  pwm_channel = variant_pin_map[ulPin].pwm;
  if(pwm_channel >= 0)
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
    EMARD_TIMER[TC_INCREMENT] = (((uint64_t)analog_write_frequency) 
                                   << (TIMER_BITS+PRESCALER_BITS)) / TIMER_CLOCK;

    EMARD_TIMER[TC_CONTROL] = ( EMARD_TIMER[TC_CONTROL] & 
                               (
 	                      (1<<TCTRL_ENABLE_OCP1) | (1<<TCTRL_ENABLE_OCP2)
	                    | (1<<TCTRL_ENABLE_ICP1) | (1<<TCTRL_ENABLE_ICP2)
                               )
                              )
                            | (1<<TCTRL_AND_OR_OCP1) | (1<<TCTRL_AND_OR_OCP2)
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

    EMARD_TIMER[pwm_enable_bitmask[pwm_channel].ocp_start] = 0;
    EMARD_TIMER[pwm_enable_bitmask[pwm_channel].ocp_stop]  = 
      ulValue < (1<<TIMER_BITS) ? ulValue : (1<<TIMER_BITS);

    EMARD_TIMER[TC_CONTROL] |= pwm_enable_bitmask[pwm_channel].control;
    EMARD_TIMER[TC_APPLY] = pwm_enable_bitmask[pwm_channel].apply;
  }
}
