#include <Adafruit_GFX.h>
#include <Adafruit_F32C_VGA.h>
Adafruit_F32C_VGA display(1);

void setup() {
  // put your setup code here, to run once:
  display.begin();
  display.clearDisplay();
  display.setTextSize(4);
  display.setCursor(100,100);
  display.print("VGA DISPLAY TEST");
  display.drawRect(  0,0,   640,480, WHITE);
  display.drawLine(100,180, 480,150, 0xFF0);
}

void loop() {
  static uint32_t counter = 0;
  display.fillRect(100,200,400,28,BLACK);
  display.setTextColor(0x00FF);
  display.setCursor(100,200);
  display.print(counter++);
  delay(1000);
}

