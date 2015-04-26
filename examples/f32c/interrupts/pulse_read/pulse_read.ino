/*
 ICP interrupt test
 
 This example shows how to use pwm timer (aka emard timer)
 which does PWM (pulse width modulation) for analogWrite
 and interrupts for each PWM OCP (output compare) channel

 This example code is in the public domain.
*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define INC_EXTEND_SIGN (-(1<<(TIMER_BITS+PRESCALER_BITS-1)))

int led_ocp1 = 9;         // the pin for OCP1 channel (hardwired)
int led_ocp2 = 10;         // the pin for OCP2 channel (hardwired)
int led_icp1 = 8;   // interrupt activity indicator for OCP1
int led_icp2 = 11;   // interrupt activity indicator for OCP2
int32_t print_event;
int32_t print_interval = 1000;
uint32_t pulse_read;

void pwm1_isr(void)
{
  /* ocp interrupt will generate rising pulse for both icps */
  digitalWrite(led_icp1, HIGH);
  digitalWrite(led_icp2, HIGH);
  /* icp interrupt should follow led */
}

void icp1_isr(void)
{
  /* not used */
}

void pwm2_isr(void)
{
  /* ocp interrupt will generate falling edge for both icps */
  digitalWrite(led_icp1, LOW);
  digitalWrite(led_icp2, LOW);
  /* icp interrupt should follow led */
}

void icp2_isr(void)
{
  /* icp interrupt2 at trailing edge, when pulse read data are available */
  pulse_read = pulseRead();
}

// the setup routine runs once when you press reset:
void setup() {
  pinMode(led_ocp1, OUTPUT);
  pinMode(led_ocp2, OUTPUT);
  analogWriteResolution(12);  // 12 bits -> max value is 4095
  analogWriteFrequency(1000);   // 10 kHz switching frequency
  analogWrite(led_ocp1, 120);  // intensity = 20/4095
  analogWritePhase(led_ocp1,  0);
  analogWrite(led_ocp2, 120); // intensity = 100/4095
  analogWritePhase(led_ocp2, 2000);
  // currently timer supports interrupts only on OCP rising edge (hardwired)
  attachInterrupt(led_ocp1, pwm1_isr, RISING); // rising edge of OCP1 will trigger interrupt
  attachInterrupt(led_ocp2, pwm2_isr, RISING); // rising edge of OCP2 will trigger interrupt
  //attachInterrupt(led_icp1, icp1_isr, RISING); // rising edge of ICP1 will trigger interrupt
  attachInterrupt(led_icp2, icp2_isr, RISING); // rising edge of ICP1 will trigger interrupt
  pulseListen(led_icp1, led_icp2, HIGH);
  print_event = millis();
  Serial.begin(115200);
}

void print_timer()
{
  char line[255];
  int32_t i;
  int64_t f;
  
  i = EMARD_TIMER[TC_INCREMENT] & ((1<<(TIMER_BITS+PRESCALER_BITS))-1);
  if(i & (1<<(TIMER_BITS+PRESCALER_BITS-1))) /* is i negative? */
    i |= INC_EXTEND_SIGN;
  
  f = ((uint64_t)VARIANT_MCK * i) >> (TIMER_BITS+PRESCALER_BITS);

  print_event += print_interval;
  Serial.write("inc= "); Serial.print(i, DEC);
  Serial.write(" ("); Serial.print(i, HEX);
  Serial.write(") f="); Serial.print((uint32_t) f, DEC);
  Serial.write(" Hz cnt="); Serial.print(EMARD_TIMER[TC_COUNTER], DEC);
  Serial.write(" icp1="); Serial.print(EMARD_TIMER[TC_ICP1], DEC);
  Serial.write(" icp2="); Serial.print(EMARD_TIMER[TC_ICP2], DEC);
  Serial.write(" pulse_width="); Serial.print(timerTicks2us(pulse_read), DEC);
  Serial.write(" us\n"); 
}

// the loop routine runs over and over again forever:
void loop() {
  delay(100);
  digitalWrite(led_icp1, LOW);
  digitalWrite(led_icp2, LOW);
  if( ((int32_t)millis()-print_event) > 0)
  {
    print_timer();
  }
}

