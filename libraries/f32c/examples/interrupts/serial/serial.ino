uint8_t serialrx_irq_magic=15; // magic pin that attaches serial interrupt

uint8_t led=13;
volatile uint8_t *led_pointer; // address of LED MMIO register
static uint8_t led_bitmask; // LED bit set to 1, other bits 0

void serialrx_irq(void)
{
  if(Serial.available())
  {
    uint8_t c = Serial.read();
    *led_pointer = c;
  }
}

void setup() {
  led_pointer = (volatile uint8_t *) portOutputRegister(digitalPinToPort(led)); // clean way
  led_bitmask = digitalPinToBitMask(led);
  attachInterrupt(serialrx_irq_magic, serialrx_irq, RISING);
}

void loop() {
  delay(1000);
}

