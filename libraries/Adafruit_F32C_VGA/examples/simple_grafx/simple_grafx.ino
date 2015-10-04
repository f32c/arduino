#include <Adafruit_GFX.h>
#include <Adafruit_F32C_VGA.h>
Adafruit_F32C_VGA display(1);

void setup() {
  // put your setup code here, to run once:
  display.begin();
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(10,10);
  display.setTextColor(RED);
  display.print("RED ");
  display.setTextColor(GREEN);
  display.print("GREEN ");
  display.setTextColor(BLUE);
  display.print("BLUE ");
  display.setTextColor(CYAN);
  display.print("CYAN ");
  display.setTextColor(MAGENTA);
  display.print("MAGENTA ");
  display.setTextColor(YELLOW);
  display.print("YELLOW");
  display.setTextColor(WHITE);
  display.setCursor(10,30);
  display.setTextColor(DARKRED);
  display.print("RED ");
  display.setTextColor(DARKGREEN);
  display.print("GREEN ");
  display.setTextColor(DARKBLUE);
  display.print("BLUE ");
  display.setTextColor(DARKCYAN);
  display.print("CYAN ");
  display.setTextColor(DARKMAGENTA);
  display.print("MAGENTA ");
  display.setTextColor(DARKYELLOW);
  display.print("YELLOW");
  display.setTextColor(WHITE);
  display.setTextSize(4);
  display.setCursor(100,100);
  display.print("VGA DISPLAY TEST");
  display.drawRect(  0,0,   640,480, WHITE);
  display.drawLine(100,180, 480,150, YELLOW);
}

void loop() {
  static uint32_t counter = 0;
  display.fillRect(100,200,400,28,BLACK);
  display.setTextColor(RED);
  display.setCursor(100,200);
  display.print(counter++);
  delay(1000);
}

