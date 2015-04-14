/* GPIO interrupts demo

 * every GPIO pin can have interrupt on
 * rising edge, falling edge or both edges.
 * each edge can have its own interrupt service routine or
 * both edges can share the same interrupt service routine
 * if interrupt service routine is attached to output pin
 * then changing this pin can trigger software generated interrupts
 * detachInterrupt will detach all interrupts from a pin

 * in this example we will demonstrate internal (software
 * generated) interrupt (2 leds should blink)
 * and external (hardware genrated)
 * for this user should set input_pin to different GPIO
 * and connect it with a wire to output_pin in order
 * to test external interrupts.
 * (2 leds should blink when wire is connected)
 * (1 led will blink when wire is not connected)
*/
 
int output_pin = 32;        // GPIO pin as pulse generator
int out_indicator_pin = 8;  // LED indicator blinks when pulse is on

int input_pin = output_pin; // software interrupt (no external wiring)
// int input_pin = 33;      // connect to output_pin with wire
int in_indicator_pin =  9;  // LED indicator, if interrputs work it will blink in sync with output pin

void gpio_isr(void)
{
  digitalWrite(in_indicator_pin, digitalRead(input_pin));
}

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(output_pin, OUTPUT);
  pinMode(out_indicator_pin, OUTPUT);

  /* in software interrupt mode,
  ** this if clause prevents setting
  ** output pin to input
  */
  if(output_pin != input_pin)
    pinMode(input_pin, INPUT);
  pinMode(in_indicator_pin, OUTPUT);

  /* each pin can have rising and falling interrupts
  ** both enabled at the same time, with 
  ** same or different interrupt service routine
  */
  attachInterrupt(input_pin, gpio_isr, RISING);
  attachInterrupt(input_pin, gpio_isr, FALLING);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(output_pin, HIGH);         // turn the signal on (HIGH is the voltage level)
  digitalWrite(out_indicator_pin, HIGH);  // turn the LED on (HIGH)
  delay(1000);                            // wait for a second

  digitalWrite(output_pin, LOW);          // turn the signal off by making the voltage LOW
  digitalWrite(out_indicator_pin, LOW);   // turn the LED off (LOW)
  delay(1000);                            // wait for a second
}
