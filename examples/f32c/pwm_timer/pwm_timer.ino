/*
 PWM timer
 
 This example shows how to use pwm timer (aka emard timer)
 which does PWM (pulse width modulation) for analogWrite
 and interrupts for each PWM OCP (output compare) channel

 This example code is in the public domain.
*/

int led1 = 14;         // the pin for OCP1 channel (hardwired)
int led2 = 15;         // the pin for OCP2 channel (hardwired)
int led_activ1 = 12;   // interrupt activity indicator for OCP1
int led_activ2 = 13;   // interrupt activity indicator for OCP2

void pwm1_isr(void)
{
  digitalWrite(led_activ1, HIGH);
}

void pwm2_isr(void)
{
  digitalWrite(led_activ2, HIGH);
}

// the setup routine runs once when you press reset:
void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  analogWriteResolution(12); // 12 bits -> max value is 4095
  analogWriteFrequency(20); // 20 Hz switching frequency
  analogWrite(led1, 20); // intensity = 20/4095
  analogWrite(led2, 100); // intensity = 100/4095
  // currently timer supports interrupts only on OCP rising edge (hardwired)
  attachInterrupt(led1, pwm1_isr, RISING); // rising edge of OCP1 will trigger interrupt
  attachInterrupt(led2, pwm2_isr, RISING); // rising edge of OCP2 will trigger interrupt
}

// the loop routine runs over and over again forever:
void loop() {
  delay(100);
  digitalWrite(led_activ1, LOW);
  digitalWrite(led_activ2, LOW);
}
