#include <Adafruit_GFX.h>
#include <Adafruit_F32C_VGA.h>
Adafruit_F32C_VGA display(1);

struct thin_sprites
{
  int16_t x[2]; // horizontal offset
  uint8_t color[2][32]; // pixel color
};
struct thin_sprites *tspr;

// move tiled area from inclusive (x1,y1) to (x2,y2)
// horizontally by x pixels 
void thin_move(int tx1, int ty1, int tx2, int ty2, int x)
{
  for(int j = ty1; j <= ty2; j++)
    for(int i = tx1; i <= tx2; i++)
      tspr[i/2+10*j].x[i&1] = x;
}

void setup() {
  tspr = (struct thin_sprites *) display.videomem;
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
  display.setTextColor(RED);
  display.print("V");
  display.setTextColor(GREEN);
  display.print("G");
  display.setTextColor(BLUE);
  display.print("A");
  display.setTextColor(WHITE);
  display.print(" DISPLAY ");
  display.setTextColor(YELLOW);
  display.print("TEST");
  display.drawRect(  0,0,   640,480, WHITE);
  display.drawLine(100,180, 480,150, YELLOW);
}

void loop() {
  static uint32_t counter = 0;
  int x1=6, y1=100, x2=11, y2=140;
  int move_xmin = -190, move_xmax = 280;
  int move_delay = 10;
  display.fillRect(100,200,400,28,BLACK);
  display.setTextColor(RED);
  display.setCursor(100,200);
  display.print(counter++);
  for(int j = 1; j < move_xmax; j++)
  {
    thin_move(x1,y1, x2,y2, j);
    delay(move_delay);
  }
  for(int j = move_xmax-1; j >= move_xmin; j--)
  {
    thin_move(x1,y1, x2,y2, j);
    delay(move_delay);
  }
  for(int j = move_xmin+1; j <= 0; j++)
  {
    thin_move(x1,y1, x2,y2, j);
    delay(move_delay);
  }
  delay(100);
}

