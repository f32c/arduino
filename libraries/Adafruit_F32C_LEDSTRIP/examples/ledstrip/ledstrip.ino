#include <Adafruit_GFX.h>
#include <Adafruit_F32C_LEDSTRIP.h>
#include "bitmap.h"
#include <SPI.h>
#include <SD.h>

Adafruit_F32C_LEDSTRIP display(1);

#define LINES 50
// milliseconds interval to check motor counter
#define INTERVAL 400
#define BRIDGE_PIN 9
#define vblank_reg (*(volatile uint8_t *)0xfffffb87)
#define counter_reg (*(volatile int16_t *)0xfffffb88)
#define capture_reg (*(volatile uint32_t *)0xfffffb84)

int32_t target;
int16_t motor_power;

void readbitmap()
{
  int x,y;
  for(x = 0; x < 50; x++)
    for(y = 0; y < 72; y++)
      display.drawPixel(x,y, bitmap[y][x]);
}

void ppm_parser(uint8_t c)
{
  static int i=0, x=0, y=0;
  static int levels=0;
  static int xmax=0, ymax=0;
  static int state = 0;
  static uint8_t rgb[3];
  uint16_t rgb565;

  switch(state)
  {
    case 0: // read P
      if(c == 'P')
        state = 1;
      break;
    case 1: // read 6
      if(c == '6')
        state = 2;
      break;
    case 2: // read newline
      if(c == '\n')
        state = 3;
      break;
    case 3: // read optional comments
      if(c == '#') // comment
        state = 2; // read another newline
      if(c >= '0' && c <= '9')
      {
        xmax *= 10;
        xmax += c - '0';
        state = 4;
      }
      break;
    case 4: // read X or space
      if(c >= '0' && c <= '9')
      {
        xmax *= 10;
        xmax += c - '0';
      }
      if(c == ' ')
        state = 5;
      break;
    case 5: // read Y or newline
      if(c >= '0' && c <= '9')
      {
        ymax *= 10;
        ymax += c - '0';
      }
      if(c == '\n')
        state = 6;
      break;
    case 6: // read levels or newline
      if(c >= '0' && c <= '9')
      {
        levels *= 10;
        levels += c - '0';
      }
      if(c == '\n')
      {
        state = 7;
        Serial.print("X=");
        Serial.print(xmax);
        Serial.print(" Y=");
        Serial.print(ymax);
        Serial.print(" levels=");
        Serial.println(levels);
      }
      break;
    case 7: // read the bitmap
        rgb[i++] = c;
        if(i >= 3)
        {
          rgb565 = ((rgb[0]>>3)<<11) | ((rgb[1]>>2)<<5) | ((rgb[2]>>3));
          if(x < 50 && y < 70)
            display.drawPixel(x,y, rgb565);
          i = 0;
          if(++x >= xmax)
          {
            //Serial.print("line ");
            //Serial.println(y);
            x = 0;
            if(++y >= 70)
              state = 8;
          }
        }
        break;
      case 8:
        Serial.println("image read completed");
        state = 9;
        break;
  } // switch end
}

// read ppm file using fatfs
void readppm(char *name)
{
  int i=0, x=0, y=0;
  uint8_t rgb[3];
  uint16_t rgb565;
  int state = 0;
  String filerd;

  if (SD.begin(4))
  {
    Serial.println("card initialized.");
    File dataFile = SD.open(name);

    if (dataFile)
    {
      Serial.print(name);
      Serial.println(" file opened.");
      while (dataFile.available()) 
        ppm_parser(dataFile.read());
      dataFile.close();
    }
    else
    {
      Serial.print(name);
      Serial.println(" file not found.");
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  int i;
  
  for(i = 8; i < 15; i++)
    digitalWrite(i, 0); 

  // PWM output to motor controller bridge
  motor_power = 1700; // motor will slowly start
  analogWriteResolution(12); // 12-bit resolutin 0-4095
  analogWriteFrequency(20000); // 20 kHz
  pinMode(BRIDGE_PIN, OUTPUT);
  analogWrite(BRIDGE_PIN, motor_power);
  // while the motor starts, load the picture
  display.begin();
  display.clearDisplay();
  display.setTextSize(2);
  display.setRotation(1);
  #if 1
  for(i = 0; i < 1; i++)
  {
    display.setCursor(2+i*120, 2);
    display.setTextColor(RED);
    display.print("SD");
    display.setCursor(2+i*120, 19);
    display.setTextColor(GREEN);
    display.print("kugla");
    display.setCursor(2+i*120, 37);
    display.setTextColor(BLUE);
    display.print(".ppm"); 
  }
  #endif
  display.setTextColor(WHITE);
  #if 0
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
  readppm("kugla.ppm");
  //display.fillRect(0,0, 50,72, WHITE);
  // initialize serial port for debug printing
  Serial.begin(115200); 
  // timer
  target = micros();
  Serial.println("testing");
}

void loop() {
  static uint32_t counter = 0;
  static int16_t new_rotation, old_rotation;
  static int32_t new_time;

  static uint32_t scroll_counter = 0;
  static uint16_t prev_line;
  static uint32_t mpower = 10000;

  uint16_t line;
  uint16_t motor_counts;
  uint16_t rpm;
  
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

  new_rotation = counter_reg;
  motor_counts = new_rotation - old_rotation;
  old_rotation = new_rotation;
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
  if(rpm > 520)
    if(motor_power > 1400)
      motor_power--; // slow down
  //if((LINES+line - prev_line)%LINES < LINES/2) // positive
  if(rpm < 520)
    if(motor_power < 2300)
      motor_power++; // speed up
  analogWrite(BRIDGE_PIN, motor_power);
  prev_line = line;
  counter++;
  display.startscrollright((scroll_counter--)%50,0);
}

