/*
*/
#include <Arduino.h>

// constants won't change. Used here to set a pin number:
const int ledPin = 17; // the number of the LED pin
const int pin = 32+20; // first audio pin

const int SPI_MISO = 20;
const int SPI_MOSI = 8+18;
const int SPI_CLK = 8+17;
const int SPI_CSN = 8+16;

// constants won't change:
const long interval = 100; // min interval at which to blink (milliseconds)

const uint16_t config_setup =
          (0x10 << 11) |  // CONFIG_SETUP
          (1 << 10)    |  // REFSEL external single ended (vs REF-).
          (0 << 9)     |  // AVGON averaging turned off.
          (0 << 7)     |  // NAVG 1 conversion per result.
          (0 << 5)     |  // NSCAN scans N and returns 4 results.
          (0 << 3)     |  // SPM all circuitry powered all the time.
          (1 << 2)     ;  // ECHO 1-enabled 0-disabled.

const uint16_t mode_control =
          (0 << 15) |  // REG_CNTL select ADC mode control.
          (3 << 11) |  // standard_int scan 0-N internal clock w/avg.
          (7 << 7)  |  // CHSEL scan up to channel 7.
          (0 << 5)  |  // RESET the FIFO only.
          (0 << 3)  |  // PM power management normal.
          (0 << 2)  |  // CHAN_ID set to 1 in ext mode to get chan id.
          (1 << 1)  ;  // SWCNV 1 (rising edge of CS)

const uint16_t sample_set =
          (B10110 << 11 ) |  // SMPL_SET select ADC sample set
               (4 << 3)   ;  // SEQ length

uint16_t SPIRW(uint16_t d)
{
  uint16_t r = 0, b = 1<<15;
  uint8_t miso;
  for(int i = 0; i < 16; i++)
  {
    digitalWrite(SPI_CLK, LOW);
    //digitalWrite(9, LOW);
    digitalWrite(SPI_MOSI, (d & b) != 0 ? HIGH : LOW);
    //digitalWrite(10, (d & b) != 0 ? HIGH : LOW);
    //delay(1);
    digitalWrite(SPI_CLK, HIGH);
    //digitalWrite(9, HIGH);
    // read now
    if(digitalRead(SPI_MISO))
    {
      r |= b;
      //digitalWrite(11, HIGH);
    }
    else
    {
      //digitalWrite(11, LOW);
    }
    b >>= 1;
    //delay(1);
  }
  return r;
}

uint16_t SPISend(const unsigned int adc_cbase, uint16_t d)
{
  digitalWrite(SPI_CSN, LOW);
  //digitalWrite(8, LOW);
  uint16_t retval = SPIRW(d);
  digitalWrite(SPI_CSN, HIGH);
  //digitalWrite(8, HIGH);
  return retval;
}

void SPIdriveCSN(int cs)
{
  digitalWrite(SPI_CSN, cs);
  //digitalWrite(8, cs);
}

void SPIstart()
{
  // first clock with CSN HI
  digitalWrite(SPI_CSN, HIGH); // make sure it's high
  //delay(1);
  digitalWrite(SPI_CLK, LOW);
  //delay(1);
  digitalWrite(SPI_CLK, HIGH);
  //delay(1);
  SPISend(0,mode_control); // to the rest 16 bit send normal mode control command
}

void configureMax1112x()
{
  const int adc_cbase = 0;
  // Config: Reset.
  SPISend(adc_cbase, (2 << 5)); // RESET reset all registers to defaults.

  // Config: ADC configuration.
  SPISend(adc_cbase, config_setup);

  // Config: Unipolar.
  SPISend(adc_cbase, (0x12 << 11));  // All channels unipolar.
  SPISend(adc_cbase, (0x11 << 11) |  // All channels unipolar.
                        (1 << 2));      // Reference all channels to REF-.

  // Config: ADC mode control.
  SPISend(adc_cbase, mode_control);

  // Defaults are good for:
  // Config: Bipolar.
  // Config: RANGE.
  // Config: Custom scan 0.
  // Config: Custom scan 1.
  // Config: Sample set.
}


void adc_init()
{
  for(int j = 0; j < 8; j++)
  {
    pinMode(8+j, OUTPUT);
    digitalWrite(8+j, LOW);
  }
  // set the digital pin as output:
  pinMode(ledPin, OUTPUT);
  configureMax1112x();
}

void adc_read(char *a)
{
  static uint8_t skip_eval = 5; // skip evaluation for first few readings
  static uint16_t reading[10], prev_reading[10];
  // alternating LED state will also alternate
  // digital output pins shared with ADC
  // ADC will read digital states
  static int ledState = LOW;
  {

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);

    // copy previous reading for evaluation
    for(int i = 0; i < 8; i++)
      prev_reading[i] = reading[i];

    SPIstart();
    delay(1); // wait conversion
    for(int i = 0; i < 9; i++)
      reading[i] = SPISend(0,0);
    // evaluation for ADC readings: OK/FAIL
    static char *channel_eval[8];
    for(uint16_t i = 0; i < 8; i++)
    {
      if(skip_eval)
        channel_eval[i] = "WAIT";
      else if(ledState)
      {
        if(reading[i] < ((i << 12) + 0x080) && prev_reading[i] > ((i << 12) + 0xF80) )
          channel_eval[i] = " OK ";
        else
          channel_eval[i] = "FAIL";
      }
    }
    // skip evaluation for first few readings
    if(skip_eval)
      skip_eval--;
    sprintf(a, "%04x %04x %04x %04x %04x %04x %04x %04x\nADC: %s      %s      %s      %s\n",
      reading[0],
      reading[1],
      reading[2],
      reading[3],
      reading[4],
      reading[5],
      reading[6],
      reading[7],
      channel_eval[1],
      channel_eval[3],
      channel_eval[5],
      channel_eval[7]
    );
    // change GPIO logic levels 0/1, ADC is connected to GPIO
    // and should convert logic levels
    pinMode(32+14, OUTPUT);
    digitalWrite(32+14, ledState);
    pinMode(32+15, OUTPUT);
    digitalWrite(32+15, ledState);
    pinMode(32+16, OUTPUT);
    digitalWrite(32+16, ledState);
    pinMode(32+17, OUTPUT);
    digitalWrite(32+17, ledState);
  }
}

