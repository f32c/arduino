/*
 Timer interrupt

 This example shows how to blink LED
 using the interrput.

 This example code is in the public domain.
*/

static int led = 13; // timer interrupt hardwired to arduino LED
static int timer_ticks;
static int incrementInterval = 10000000;

void timer_handler()
{
  digitalWrite(led, timer_ticks & 1);
  if(timerInterval < VARIANT_MCK>>8)
    incrementInterval = 1000000;
  if(timerInterval > VARIANT_MCK>>2)
    incrementInterval = -1000000;
  timerInterval += incrementInterval;
  timer_ticks++;
}

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(115200);
  // timerInterval = VARIANT_MCK;
  attachInterrupt(led, timer_handler, VARIANT_MCK>>3);
}

// the loop routine runs over and over again forever:
void loop() {
  Serial.println(timer_ticks);
  delay(100);
}
