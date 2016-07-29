/*
 Timer interrupt

 This example shows how to blink LED
 using the interrupt.

 This example code is in the public domain.
*/

static int led = 13; // timer interrupt hardwired to arduino LED
volatile uint8_t *led_pointer; // address of LED MMIO register
static uint8_t led_bitmask; // LED bit set to 1, other bits 0
static int timer_ticks;
static int incrementInterval = 1000000;

#define MANIFEST_MUL_INTERRUPT_BUG 0

// WARNING:
// Currently (2016-07-30), when f32c core is in interrupt context,
// it doesn't correctly execute integer multiply instruction.
// Sometimes, register corruption will occur.
// Depending on how the code is compiled, bug will not always
// manifest.

void timer_handler()
{
  #if MANIFEST_MUL_INTERRUPT_BUG
    digitalWrite(led, timer_ticks & 1); // may contain MUL
  #else
    *led_pointer = timer_ticks & 1 ? led_bitmask : 0; // no MUL
  #endif
  if(timerInterval < (F_CPU>>8))
    incrementInterval =  100000;
  if(timerInterval > (F_CPU>>2))
    incrementInterval = -100000;
  timerInterval += incrementInterval;
  timer_ticks++;
}

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(115200);
  // led_pointer = -240; // dirty way
  led_pointer = (volatile uint8_t *) portOutputRegister(digitalPinToPort(led)); // clean way
  led_bitmask = digitalPinToBitMask(led);
  attachInterrupt(led, timer_handler, F_CPU>>3);
}

// the loop routine runs over and over again forever:
void loop() {
  Serial.println(timer_ticks);
  delay(100);
}

