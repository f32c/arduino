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

int input_pin = 33;         // connect to output_pin with wire
volatile uint32_t *input_pointer; // address of GPIO input MMIO register
static uint32_t input_bitmask; // GPIO input bit set to 1, other bits 0

int in_indicator_pin =  9;  // LED indicator, if interrputs work it will blink in sync with output pin
volatile uint32_t *in_indicator_pointer; // address of LED MMIO register
static uint32_t in_indicator_bitmask; // LED bit set to 1, other bits 0

#define USE_INTERRUPT 1
#define MANIFEST_MULTIPLY_BUG 0

void gpio_isr(void)
{
  //static int toggler;
  //toggler = ~toggler;
  //*in_indicator_pointer = toggler;
  #if MANIFEST_MULTIPLY_BUG
    digitalWrite(in_indicator_pin, digitalRead(input_pin));
  #else
    *in_indicator_pointer =  ((input_bitmask & *input_pointer) == 0)
                          ?  (*in_indicator_pointer & ~in_indicator_bitmask)
                          :  (*in_indicator_pointer |  in_indicator_bitmask);
  #endif
}

// the setup function runs once when you press reset or power the board
void setup() {

  // initialize digital pin 13 as an output.
  pinMode(output_pin, OUTPUT);
  pinMode(out_indicator_pin, OUTPUT);

  // inside of ISR access LED directly using MMIO register
  // to avoid interrupt-unfriendly Multiply instruction
  // probably hidden in digitalRead/digitalWrite
  input_pointer = portInputRegister(digitalPinToPort(input_pin));
  input_bitmask = digitalPinToBitMask(input_pin);
  //delay(1000);
  //Serial.println((uint32_t) input_pointer, HEX);
  //Serial.println((uint32_t) input_bitmask, HEX);
  in_indicator_pointer = portOutputRegister(digitalPinToPort(in_indicator_pin));
  in_indicator_bitmask = digitalPinToBitMask(in_indicator_pin);

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
  #if USE_INTERRUPT
    attachInterrupt(input_pin, gpio_isr, RISING);
    attachInterrupt(input_pin, gpio_isr, FALLING);
  #endif
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(output_pin, HIGH);         // turn the signal on (HIGH is the voltage level)
  digitalWrite(out_indicator_pin, HIGH);  // turn the LED on (HIGH)
  #if USE_INTERRUPT
  #else
    gpio_isr(); // this will execute with the interrupt instead
  #endif
  delay(100);                            // wait for a second

  digitalWrite(output_pin, LOW);          // turn the signal off by making the voltage LOW
  digitalWrite(out_indicator_pin, LOW);   // turn the LED off (LOW)
  #if USE_INTERRUPT
  #else
    gpio_isr(); // this will execute with the interrupt
  #endif
  delay(100);                            // wait for a second
}

