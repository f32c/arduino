/*
 Fade

 This example shows how to fade an LED on pin 9
 using the analogWrite() function.

 This example code is in the public domain.
 */

int led1 = 14;           // the pin that the LED is attached to
int led2 = 15;
int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by

// the setup routine runs once when you press reset:
void setup() {
  // declare pin 9 to be an output:
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  analogWriteResolution(12);
  digitalWrite(led1, HIGH);
  digitalWrite(led2, LOW);
}

// the loop routine runs over and over again forever:
void loop() {
  static int mode = 0;
  // set the brightness of pin 9:
  mode++;
  mode &= 16383;
  if((mode & 8192) == 0)
  {
    analogWrite(led1, brightness);
    analogWrite(led2, 4095-brightness);
  }
  else
  {
    digitalWrite(led1, (mode & 256) == 0 ? LOW  : HIGH);
    digitalWrite(led2, (mode & 256) == 0 ? HIGH : LOW);
  }

  // change the brightness for next time through the loop:
  brightness = brightness + fadeAmount;

  // reverse the direction of the fading at the ends of the fade:
  if (brightness == 0 || brightness == 4095) {
    fadeAmount = -fadeAmount ;
  }
  // wait for 30 milliseconds to see the dimming effect
  delay(1);
}
