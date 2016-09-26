/*
  Example for REV outlets (e.g. 8342L)
  
  http://code.google.com/p/rc-switch/
  
  Need help? http://forum.ardumote.com
*/

#include <RCSwitch.h>


// ULX2S SRAM 81.25MHz -> 433.33MHz transmitter is at physical pin 40 = arduino pin 15
// ULX2S BRAM 25MHz -> 433.92MHz transmitter is at physical pin 40 = arduino pin 15
// CPU clock in arduino must be set correctly (81.25MHz or 25MHz) for pulse timing to work
int tx433 = 15; 
int led = 8;   // indicator led is at physical pin 12

RCSwitch mySwitch = RCSwitch();

void setup()
{
  mySwitch.enableTransmit(tx433);
  // pulse 65
  // distance 56/315
  // packet distance 2553
  // set pulse length.
  // 65 bit
  // off ab 2a ca ab 55 55 4a d5 00
  // on  ab 2a ca ab 55 55 4b 55 00
  mySwitch.setProtocol(6);
  //mySwitch.setPulseLength(21);
  //mySwitch.setRepeatTransmit(10);
}

void loop() {

  // Switch on:
  // The first parameter represents the channel (a, b, c, d)
  // The second parameter represents the device number
  // 
  // In this example it's channel 'a', device #2 
  digitalWrite(led, HIGH); // indicator ON
  // on  ab 2a ca ab 55 55 4b 55 00
  mySwitch.send("10101011"
                "00101010"
                "11001010"
                "10101011"
                "01010101"
                "01010101"
                "01001011"
                "01010101"
                "0");

  // Wait 2 seconds
  delay(2000);
  
  // Switch off
  digitalWrite(led, LOW); // indicator OFF
  // off ab 2a ca ab 55 55 4a d5 00
  mySwitch.send("10101011"
                "00101010"
                "11001010"
                "10101011"
                "01010101"
                "01010101"
                "01001010"
                "11010101"
                "0");

  // Wait another 2 seconds
  delay(2000);
  
}
