/* EMARD */

#include "Arduino.h"
#include "wiring_analog.h"

__BEGIN_DECLS

static uint8_t analog_write_resolution_bits = 8;
/* old arduino uses 490 Hz */
/* new arduino uses 980 Hz */
static uint32_t analog_write_frequency = 980;

const struct pwm_enable_bitmask_s pwm_enable_bitmask[] = VARIANT_PWM_CHANNEL_ENABLE;

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

/* todo: handle the pin */
void analogWritePhase(uint32_t pin, uint32_t phase)
{
  int8_t pwm_channel;
  volatile uint32_t *start, *stop;

  if(pin >= variant_pin_map_size)
    return;

  pwm_channel = variant_pin_map[pin].pwm;
  if(pwm_channel >= 0)
  {
    start  = &EMARD_TIMER[pwm_enable_bitmask[pwm_channel].ocp_start];
    stop   = &EMARD_TIMER[pwm_enable_bitmask[pwm_channel].ocp_stop];
    
    *stop  = phase + (*stop - *start);
    *start = phase;
    
    EMARD_TIMER[TC_APPLY] = pwm_enable_bitmask[pwm_channel].apply;
  }
}

/* setup the common parameters (why isn't it called at startup?)
*/
void analogOutputInit( void )
{
}

void analogWrite(uint32_t ulPin, uint32_t ulValue)
{
  int8_t pwm_channel;
  volatile uint32_t *start, *stop;
  
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

    EMARD_TIMER[TC_CONTROL] &= pwm_enable_bitmask[pwm_channel].control_and;
    /* shift timer step value to match the set resolution */
    if(analog_write_resolution_bits < TIMER_BITS)
      ulValue <<= (TIMER_BITS-analog_write_resolution_bits);
    if(analog_write_resolution_bits > TIMER_BITS)
      ulValue >>= (analog_write_resolution_bits-analog_write_resolution_bits);
      
    start = &EMARD_TIMER[pwm_enable_bitmask[pwm_channel].ocp_start];
    stop  = &EMARD_TIMER[pwm_enable_bitmask[pwm_channel].ocp_stop];
    
    *stop  = /* ((1<<TIMER_BITS)-1) & */
             ( *start
             + ( ulValue < (1<<TIMER_BITS) ? ulValue : (1<<TIMER_BITS) )
             );

    if( *start <= *stop )
      EMARD_TIMER[TC_CONTROL] |= pwm_enable_bitmask[pwm_channel].control_and_or;
    
    #if 0
    /* input caputre setting
    ** this code doesn't belong here
    ** initializing ICP engine to capture all events
    */
    #endif

    EMARD_TIMER[TC_CONTROL] |= pwm_enable_bitmask[pwm_channel].control_or;
    EMARD_TIMER[TC_APPLY] = pwm_enable_bitmask[pwm_channel].apply;
  }
}

uint32_t analogRead(uint32_t ulPin)
{
  /* TODO
     external parallel RC circuit to GND,
     3 state output hardware that
     sets pin to output high, (charges C),
     set pin to input (high impedance, no pullup), 
     and captures time when input becomes low (C discharges),
     time is captured in a I/O mapped register
     which represents analog value of RC circuit
     
     similar as input capture, but with 3-state output and input
     on the same pin
  */
  return 0;
}

/* input capture setting */
uint32_t setInputCapture(uint32_t ulPin)
{
  int8_t icp_channel;
  
  if(ulPin >= variant_pin_map_size)
    return 0;
    
  icp_channel = variant_pin_map[ulPin].icp;
  if(icp_channel >= 0)
  {

    #if 0
    /* input caputre setting
    ** this code doesn't belong here
    ** here belongs RC co
    
    ** initializing ICP engine to capture all events
    */
    EMARD_TIMER[TC_CONTROL] &= pwm_enable_bitmask[icp_channel].control_and;
    EMARD_TIMER[TC_CONTROL] |= pwm_enable_bitmask[icp_channel].control_or;
    EMARD_TIMER[TC_APPLY] = pwm_enable_bitmask[icp_channel].apply;
    /*
    todo:
    [ ] make RC component, and 3 state pin
    [ ] create icp control array
    [ ] 
    */
    #endif

  }
  return 0;
}

__END_DECLS
