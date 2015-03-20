/*
  Prints to the serial monitor.

 This example code is in the public domain.
 */

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  static int sensorValue = 0;
  // print out the value you read:
  Serial.println(sensorValue++);
  digitalWrite(13, sensorValue & 1);
  // Serial.print(1.234);
  delay(100);        // delay in between reads for stability
}
