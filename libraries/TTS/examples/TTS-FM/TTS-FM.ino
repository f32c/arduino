/*
  Text To Speech syntesis library
  Copyright (c) 2008 Clive Webster.  All rights reserved.
  Nov. 29th 2009 - Modified to work with Arduino by Gabriel Petrut.
*/

/*
  On AVR The Text To Speech library uses Timer1 to generate the PWM
  output on digital pin 10. The output signal needs to be fed
  to an RC filter then through an amplifier to the speaker.

  On F32C it uses PCM over DMA to play sound.
  (Only SRAM and SDRAM based boards have enough memory for PCM)
*/

#include <TTS.h>
#include <PCM.h>
#include <RDS.h>

// Media pins
#define ledPin 13       // LED standard on digital pin 13

// Variables
char text [50];
boolean state=0;

TTS text2speech;
RDS rds; // FM radio

void setup() {
  rds.Hz(107900000); // Hz carrier wave frequency
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
}

//================================================================
// Main Loop
//================================================================
void loop(){
    state = !state;
    digitalWrite(ledPin, state);
    Test_Speech();
}  
//================================================================


void Test_Speech() {
 text2speech.setPitch(6); //higher values = lower voice pitch
 strcpy(text, "Hello master! How are you doing?");
 Serial.println(text);
 text2speech.say(text);
 delay(500);
 text2speech.setPitch(1); //lower values = higher voice pitch
 strcpy(text, "I am fine, thankyou.");
 Serial.println(text);
 text2speech.say(text);
 delay(1000);          // delay a second
}

