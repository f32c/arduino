/*************************************************** 
  This is a Pong port for the Arduino. The application 
  uses an Arduino Uno, Adafruit’s 128x64 OLED display, 
  2 potentiometers and an piezo buzzer.

  More info about this project can be found on my blog:
  http://michaelteeuw.nl

  Written by Michael Teeuw | Xonay Labs.  
  Apache 2 license, all text above must be included 
  in any redistribution.
  Self-play mode by RADIONA
 ****************************************************/

#include <SPI.h>
#include <Wire.h>
#include <SoftwareWire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Define Pins
#if 1
/* variant logical mapping */
#define OLED_CLK   44
#define OLED_MOSI  45
#define OLED_CS    46
#define OLED_DC    47
#define OLED_RESET 13
#define BEEPER     33
#endif

#if 0
/* variant physical mapping */
#define OLED_CLK   20
#define OLED_MOSI  21
#define OLED_CS    22
#define OLED_DC    23
#define OLED_RESET 12
#define BEEPER 3
#endif

#define HAVE_ANALOG 0
#if HAVE_ANALOG
#define CONTROL_A A0
#define CONTROL_B A1
#endif

#define HAVE_TONE 0

//Define Visuals
#define FONT_SIZE 2
#define SCREEN_WIDTH 127  //real size minus 1, because coordinate system starts with 0
#define SCREEN_HEIGHT 63  //real size minus 1, because coordinate system starts with 0
#define PADDLE_WIDTH 4
#define PADDLE_HEIGHT 10
#define PADDLE_PADDING 10
#define BALL_SIZE 3
#define SCORE_PADDING 10

#define EFFECT_SPEED 1
#define MIN_Y_SPEED 1
#define MAX_Y_SPEED 3

#define X_BOUNCE_DISTANCE (SCREEN_WIDTH-1*PADDLE_PADDING-1*BALL_SIZE)
#define Y_BOUNCE_DISTANCE (SCREEN_HEIGHT-1*BALL_SIZE)

#if 0
#define abs(x) (x >= 0 ? x : -x)
#endif

//Define Variables
// Adafruit_SSD1306 display(OLED_RESET);
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);


int paddleLocationA = 0;
int paddleLocationB = 0;

int ballX = SCREEN_WIDTH/2;
int ballY = SCREEN_HEIGHT/2;
int ballSpeedX = 2;
int ballSpeedY = 1;

int lastPaddleLocationA = 0;
int lastPaddleLocationB = 0;

/* self-play mode ball Y position prediction
** there we should place the paddle
*/
int expectAY = 0;
int expectBY = 0; 

int scoreA = 0;
int scoreB = 0;

int expect_y(int x_distance)
{
  int normalY; // normalized Y position
  // as if ball is always traveling positive
  int absSpeedX, absSpeedY;
  int expectY;
  int expect2Y;

  absSpeedX = ballSpeedX > 0 ? ballSpeedX : -ballSpeedX;
  absSpeedY = ballSpeedY > 0 ? ballSpeedY : -ballSpeedY;
  normalY = ballSpeedY > 0 ? ballY : Y_BOUNCE_DISTANCE - ballY;

  /* reflection from "double" y */
  if(absSpeedX != 0)
    expect2Y = (normalY + x_distance*absSpeedY/absSpeedX) % (2*Y_BOUNCE_DISTANCE);
  else
    expect2Y = normalY;
  /* convert double Y to reflect up or down */
  expectY = expect2Y < Y_BOUNCE_DISTANCE ? expect2Y : 2*Y_BOUNCE_DISTANCE - expect2Y;
  if(ballSpeedY < 0)
    expectY = (SCREEN_HEIGHT - BALL_SIZE) - expectY;
  return expectY;
}

int expectA()
{
  return expect_y(ballX - PADDLE_PADDING-BALL_SIZE);
}

int expectB()
{
  return expect_y(SCREEN_WIDTH-PADDLE_PADDING-BALL_SIZE - ballX);
}

//Setup 
void setup() 
{
  delay(200);
 // display.begin(SSD1306_SWITCHCAPVCC, 0x3D);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.begin(SSD1306_SWITCHCAPVCC); // inicijalizacija za SPI
  display.clearDisplay();   // clears the screen and buffer
  display.display();   
  display.setTextWrap(false);

    splash();
  
  display.setTextColor(WHITE);
  display.setTextSize(FONT_SIZE);
  display.clearDisplay(); 
  expectBY = expectB();
}

#if 0
long imap(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
#endif

//Splash Screen
void splash()
{
  display.clearDisplay(); 

  display.setTextColor(WHITE);
  centerPrint("PONG",0,3);
  centerPrint("By Allan Alcorn",24,1);
  centerPrint("Ported by",33,1);
  centerPrint("MichaelTeeuw.nl",42,1);

  display.fillRect(0,SCREEN_HEIGHT-10,SCREEN_WIDTH,10,WHITE);
  display.setTextColor(BLACK);
  centerPrint("Move paddle to start!",SCREEN_HEIGHT-9,1);

  display.display();

#if HAVE_ANALOG
  int controlA = analogRead(CONTROL_A);
  int controlB = analogRead(CONTROL_B);
  while (abs(controlA - analogRead(CONTROL_A) + controlB - analogRead(CONTROL_B)) < 10) {
    // show as long as the total absolute change of 
    // both potmeters is smaler than 5
  }
#endif

  soundStart();
}

//Loop
void loop()
{
  calculateMovement();
  draw();
  // delay(50);
}

void calculateMovement() 
{
#if HAVE_ANALOG
  int controlA = analogRead(CONTROL_A);
  int controlB = analogRead(CONTROL_B);
#else
  static int controlA = 0;
  static int controlB = 0;
  if(ballSpeedX < 0)
    controlA += 2*(expectAY-paddleLocationA-PADDLE_HEIGHT/2+BALL_SIZE/2);
  if(ballSpeedX > 0)
    controlB += 2*(expectBY-paddleLocationB-PADDLE_HEIGHT/2+BALL_SIZE/2);
#endif
  paddleLocationA = map(controlA, 0, 1023, 0, SCREEN_HEIGHT - PADDLE_HEIGHT);
  paddleLocationB = map(controlB, 0, 1023, 0, SCREEN_HEIGHT - PADDLE_HEIGHT);

  int paddleSpeedA = paddleLocationA - lastPaddleLocationA;
  int paddleSpeedB = paddleLocationB - lastPaddleLocationB;

  ballX += ballSpeedX;
  ballY += ballSpeedY;

  //bounce from top and bottom
  if (ballY >= SCREEN_HEIGHT - BALL_SIZE || ballY <= 0) {
    ballSpeedY = -ballSpeedY;
    soundBounce();
  }

  //bounce from paddle A
  if (ballX >= PADDLE_PADDING && ballX <= PADDLE_PADDING+BALL_SIZE && ballSpeedX < 0) {
    if (ballY > paddleLocationA - BALL_SIZE && ballY < paddleLocationA + PADDLE_HEIGHT) {
      soundBounce();
      ballSpeedX = -ballSpeedX;
    
      addEffect(paddleSpeedA);
      expectBY = expectB();
    }

  }

  //bounce from paddle B
  if (ballX >= SCREEN_WIDTH-PADDLE_WIDTH-PADDLE_PADDING-BALL_SIZE && ballX <= SCREEN_WIDTH-PADDLE_PADDING-BALL_SIZE && ballSpeedX > 0) {
    if (ballY > paddleLocationB - BALL_SIZE && ballY < paddleLocationB + PADDLE_HEIGHT) {
      soundBounce();
      ballSpeedX = -ballSpeedX;
    
      addEffect(paddleSpeedB);
      expectAY = expectA();
    }

  }

  //score points if ball hits wall behind paddle
  if (ballX >= SCREEN_WIDTH - BALL_SIZE || ballX <= 0) {
    if (ballSpeedX > 0) {
      scoreA++;
      ballX = SCREEN_WIDTH / 4;
      expectBY = expectB();
    }
    if (ballSpeedX < 0) {
      scoreB++;
      ballX = SCREEN_WIDTH / 4 * 3;
      expectAY = expectA();
    }

    soundPoint();   
  }

  //set last paddle locations
  lastPaddleLocationA = paddleLocationA;
  lastPaddleLocationB = paddleLocationB;  
}

void draw() 
{
  display.clearDisplay(); 

  //draw paddle A
  display.fillRect(PADDLE_PADDING,paddleLocationA,PADDLE_WIDTH,PADDLE_HEIGHT,WHITE);

  //draw paddle B
  display.fillRect(SCREEN_WIDTH-PADDLE_WIDTH-PADDLE_PADDING,paddleLocationB,PADDLE_WIDTH,PADDLE_HEIGHT,WHITE);

  //draw center line
  for (int i=0; i<SCREEN_HEIGHT; i+=4) {
    display.drawFastVLine(SCREEN_WIDTH/2, i, 2, WHITE);
  }
  
  // draw horizontal top line
  display.drawFastHLine(0, 0, SCREEN_WIDTH, WHITE);

  // draw horizontal bottom line
  display.drawFastHLine(0, SCREEN_HEIGHT, SCREEN_WIDTH, WHITE);

  //draw ball
  display.fillRect(ballX,ballY,BALL_SIZE,BALL_SIZE,WHITE);

  //print scores

  //backwards indent score A. This is dirty, but it works ... ;)
  int scoreAWidth = 5 * FONT_SIZE;
  if (scoreA > 9) scoreAWidth += 6 * FONT_SIZE;
  if (scoreA > 99) scoreAWidth += 6 * FONT_SIZE;
  if (scoreA > 999) scoreAWidth += 6 * FONT_SIZE;
  if (scoreA > 9999) scoreAWidth += 6 * FONT_SIZE;

  display.setCursor(SCREEN_WIDTH/2 - SCORE_PADDING - scoreAWidth,8);
  display.print(scoreA);

  display.setCursor(SCREEN_WIDTH/2 + SCORE_PADDING+1,8); //+1 because of dotted line.
  display.print(scoreB);
  
  display.display();
} 


void addEffect(int paddleSpeed)
{
  int oldBallSpeedY = ballSpeedY;

  //add effect to ball when paddle is moving while bouncing.
  //for every pixel of paddle movement, add or substact EFFECT_SPEED to ballspeed.
  for (int effect = 0; effect < abs(paddleSpeed); effect++) {
    if (paddleSpeed > 0) {
      ballSpeedY += EFFECT_SPEED;
    } else {
      ballSpeedY -= EFFECT_SPEED;
    }
  }

  //limit to minimum speed
  if (ballSpeedY < MIN_Y_SPEED && ballSpeedY > -MIN_Y_SPEED) {
    if (ballSpeedY > 0) ballSpeedY = MIN_Y_SPEED;
    if (ballSpeedY < 0) ballSpeedY = -MIN_Y_SPEED;
    if (ballSpeedY == 0) ballSpeedY = oldBallSpeedY;
  }

  //limit to maximum speed
  if (ballSpeedY > MAX_Y_SPEED) ballSpeedY = MAX_Y_SPEED;
  if (ballSpeedY < -MAX_Y_SPEED) ballSpeedY = -MAX_Y_SPEED;
}

void soundStart() 
{
  #if HAVE_TONE
  tone(BEEPER, 250);
  delay(100);
  tone(BEEPER, 500);
  delay(100);
  tone(BEEPER, 1000);
  delay(100);
  noTone(BEEPER);
  #endif
}

void soundBounce() 
{
  #if HAVE_TONE
  tone(BEEPER, 500, 50);
  #endif
}

void soundPoint() 
{
  #if HAVE_TONE
  tone(BEEPER, 150, 150);
  #endif
}

void centerPrint(char *text, int y, int size)
{
  display.setTextSize(size);
  display.setCursor(SCREEN_WIDTH/2 - ((strlen(text))*6*size)/2,y);
  display.print(text);
}
