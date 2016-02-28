#include <Adafruit_GFX.h>
#include <Adafruit_F32C_LEDSTRIP.h>
#include "bitmap.h"

Adafruit_F32C_LEDSTRIP display(1);

#define LINES 50
// milliseconds interval to check motor counter
#define INTERVAL 400
#define BRIDGE_PIN 9
#define vblank_reg (*(volatile uint8_t *)0xfffffb87)
#define counter_reg (*(volatile uint16_t *)0xfffffb88)
#define capture_reg (*(volatile uint32_t *)0xfffffb84)

int32_t target;
int16_t motor_power;

  static uint32_t counter = 0;
  static uint16_t old_rotation;
  static int32_t new_time;

  static uint32_t scroll_counter = 0;
  static uint16_t prev_line;
  static uint32_t mpower = 10000;

void readbitmap()
{
  int x,y;
  for(x = 0; x < 50; x++)
    for(y = 0; y < 72; y++)
      display.drawPixel(x,y, bitmap[y][x]);
}

void setup() {
  // put your setup code here, to run once:
  int i;
  
  display.begin();
  display.clearDisplay();
  display.setTextSize(2);
  display.setRotation(1);
  #if 1
  for(i = 0; i < 1; i++)
  {
  #if 0
  display.setCursor(2+i*120, 2);
  display.setTextColor(RED);
  display.print("RED");
  display.setCursor(2+i*120, 22);
  display.setTextColor(GREEN);
  display.print("GREEN");
  #endif

  #if 1
  display.setCursor(2+i*120, 2);
  display.setTextColor(RED);
  display.print("MARIJA");
  display.setCursor(2+i*120, 19);
  display.setTextColor(GREEN);
  display.print("JURIC");
  display.setCursor(2+i*120, 37);
  display.setTextColor(BLUE);
  display.print("ZAGORKA");
  #endif
  
  #if 0
  display.setCursor(2,42+i*120);
  display.setTextColor(BLUE);
  display.print("BLUE ");
  display.setCursor(2,62+i*120);
  display.setTextColor(CYAN);
  display.print("CYAN ");
  display.setCursor(2,82+i*120);
  display.setTextColor(MAGENTA);
  display.print("MAGENTA ");
  display.setCursor(2,102+i*120);
  display.setTextColor(YELLOW);
  display.print("YELLOW");
  #endif
  }
  #endif
  display.setTextColor(WHITE);
  #if 1
    //display.drawRect( 0,0,   50,72, WHITE);
    #if 0
      display.drawRect( 25,0, 50,72, RED);
    #endif
    #if 0
      //display.drawLine(  0,71, 0,71, MAGENTA);
      for(i = 0; i < 20; i++)
      display.drawLine( 0,0, 49,71-i,i*439);
      display.drawPixel(20,20, MAGENTA);
    #endif
  #endif
  #if 0
    display.drawLine(0,0, 50,72, WHITE);
  #endif
  readbitmap();
  // initialize serial port for debug printing
  Serial.begin(115200); 
  for(i = 8; i < 15; i++)
    digitalWrite(i, 0); 
  // PWM output to motor controller bridge 0-255
  motor_power = 1600; // 2000
  analogWriteResolution(12);
  analogWriteFrequency(16000);
  pinMode(BRIDGE_PIN, OUTPUT);
  analogWrite(BRIDGE_PIN, motor_power);
  // timer
  target = micros();
  Serial.println("testing");
}

void loop() {
  uint16_t line;
  uint16_t motor_counts;
  uint16_t rpm;
  #if 0
  display.fillRect(  0,0,   72,36, counter & 1 ? RED : GREEN);
  #endif
  // don't blink - it will mess up PWM
  digitalWrite(11, counter & 1);
  #if 1
  target += INTERVAL*1000; // microseconds
  do
  {
    new_time = micros();
  } while((int32_t(target - new_time)) > 0);
  #else
  delay(INTERVAL);
  #endif
  motor_counts = (int16_t)(counter_reg - old_rotation);
  old_rotation = counter_reg;
  rpm = motor_counts*60*1000/(INTERVAL*200);
  Serial.print("motor:");
  Serial.print(motor_counts, DEC); // print motor counter speed
  Serial.print(" RPM:");
  Serial.print(rpm, DEC);
  Serial.print(" power=");
  Serial.println(motor_power, DEC);
  Serial.print("capture line=");
  Serial.print(capture_reg >> 16, DEC);
  line = capture_reg>>16;
  Serial.print(" bit=");
  Serial.println(capture_reg & 0xFFFF, DEC);
  //if((LINES+line - prev_line)%LINES > LINES/2) // negative
  if(rpm > 460)
    if(motor_power > 1400)
      motor_power--; // slow down
  //if((LINES+line - prev_line)%LINES < LINES/2) // positive
  if(rpm < 460)
    if(motor_power < 2300)
      motor_power++; // speed up
  analogWrite(BRIDGE_PIN, motor_power);
  prev_line = line;
  counter++;
  counter%=10000;
  display.startscrollright((scroll_counter++)%50,0);
}

