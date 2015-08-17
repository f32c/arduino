/*
  Example for REV outlets (e.g. 8342L)
  
  http://code.google.com/p/rc-switch/
  
  Need help? http://forum.ardumote.com
*/

#include <RCSwitch.h>


// ULX2S SRAM 81.25MHz -> 433.33MHz transmitter is at physical pin 40 = arduino pin 60
// ULX2S BRAM 25MHz -> 433.92MHz transmitter is at physical pin 40 = arduino pin 15
// CPU clock in arduino must be set correctly (81.25MHz or 25MHz) for pulse timing to work
int tx433 = 60; 
int led = 12;   // indicator led is at physical pin 12

RCSwitch mySwitch = RCSwitch();

void setup() {

  mySwitch.enableTransmit(tx433);
  
  // set pulse length.
  mySwitch.setPulseLength(360);
  
}

void loop() {

  // Switch on:
  // The first parameter represents the channel (a, b, c, d)
  // The second parameter represents the device number
  // 
  // In this example it's channel 'a', device #2 
  digitalWrite(led, HIGH); // indicator ON
  mySwitch.switchOn('a', 2);

  // Wait a second
  delay(1000);
  
  // Switch off
  digitalWrite(led, LOW); // indicator OFF
  mySwitch.switchOff('a', 2);
  
  // Wait another second
  delay(1000);
  
}
