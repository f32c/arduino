/*
  Example for different sending methods
  
  http://code.google.com/p/rc-switch/

  F32C setup
  Bitstream: ulx2s_8e_mips_bram_433M92_25mhz.jed
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

#define EW_UP1 "111110000000001111110000001000000011111100000110000000001"
#define EW_UP2 "000000111111111111110000000010000000000011111000000111111"
void setup() {
  Serial.begin(115200);
  
  // Transmitter is connected to Arduino Pin #10  
  mySwitch.enableTransmit(TX433);
  
  // Optional set protocol (default is 1, will work for most outlets)
  mySwitch.setProtocol(5);

  // Optional set pulse length.
  mySwitch.setPulseLength(220);

  // Optional set number of transmission repetitions.
  mySwitch.setRepeatTransmit(1);
}

void loop() {
 if(digitalRead(BUTTON_DOWN))
 { 
  /* down */
  digitalWrite(LED, HIGH);
  /* Same switch as above, but using binary code */
  // mySwitch.send(EW_DOWN);
  digitalWrite(LED, LOW);
 }

 if(digitalRead(BUTTON_UP))
 {
  /* up */
  digitalWrite(LED, HIGH);
  /* Same switch as above, but using binary code */
  mySwitch.send(EW_UP1);
  mySwitch.send(EW_UP2);
  digitalWrite(LED, LOW);
 }
}
