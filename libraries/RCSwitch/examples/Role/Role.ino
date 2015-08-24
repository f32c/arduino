/*
  Example for different sending methods
  
  http://code.google.com/p/rc-switch/

  F32C setup
  Bitstream: ulx2s_8e_mips_bram_433_25mhz.jed
  Boards manager -> Generic FPGA Board
  CPU Architecture: MIPS
  CPU Speed: 25 MHz
  RAM Size: 16KB BRAM Internal
  Protocol: HEX 115.2 kbit/s no verify RS232
  PIN Map: Generic
*/

#include <RCSwitch.h>

RCSwitch mySwitch;

#define TX433 15
#define LED 13

enum {BUTTON_DOWN = 1, BUTTON_UP = 2};

#define EW_UP   "011000100110"
#define EW_DOWN "011000100101"

void setup() {
  Serial.begin(115200);
  
  // Transmitter is connected to Arduino Pin #10  
  mySwitch.enableTransmit(TX433);
  
  // Optional set protocol (default is 1, will work for most outlets)
  mySwitch.setProtocol(4);

  // Optional set pulse length.
  mySwitch.setPulseLength(37);

  // Optional set number of transmission repetitions.
  mySwitch.setRepeatTransmit(1);
}

void loop() {
 if(digitalRead(BUTTON_DOWN))
 { 
  /* down */
  digitalWrite(LED, HIGH);
  /* Same switch as above, but using binary code */
  mySwitch.send(EW_DOWN);
  digitalWrite(LED, LOW);
 }

 if(digitalRead(BUTTON_UP))
 {
  /* up */
  digitalWrite(LED, HIGH);
  /* Same switch as above, but using binary code */
  mySwitch.send(EW_UP);
  digitalWrite(LED, LOW);
 }
}
