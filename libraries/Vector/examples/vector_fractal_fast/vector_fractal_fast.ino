/*
 * (c)EMARD
 * LICENSE=BSD
 *
 * Vector processor test
 * 
 * Mandelbrot fractal, fixed iteration count, custom colorized,
 * demonstrates real speed of the vector processor.
 * chaining is not used here so it could be even faster.
 * 
 * writes file "fractal.ppm" on SD card in 4.4 seconds
 * Based on the source of Emu
 */

#include <SPI.h>
#include <SD.h>
#include <Vector.h>
#include <Compositing.h>

Compositing c2;
pixel_t *bitmap = (pixel_t *)0x80080000;
#define c2_set_pixel(x,y,c) bitmap[x + SCREEN_WIDTH*y] = RGB2PIXEL(c)
#define SPRITE_MAX 1

const int led = 8; // one of the leds in the port for blinking
volatile uint8_t *led_pointer; // address of LED MMIO register

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480

Vector_RAM Mac(SCREEN_WIDTH);
Vector_RAM Mred(SCREEN_WIDTH), Mgreen(SCREEN_WIDTH), Mblue(SCREEN_WIDTH);
Vector_RAM Mconst(8); // up to 8 constants loaded at top of Vbc

void alloc_bitmap()
{
  int i;

  c2.init();
  c2.alloc_sprites(SPRITE_MAX);
  bitmap = (pixel_t *) malloc(SCREEN_WIDTH*SCREEN_HEIGHT*sizeof(pixel_t)); // alloc memory for video
  c2.sprite_from_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT, bitmap); // create c2 sprite
  c2.sprite_refresh(); // show it on screen
  *c2.cntrl_reg = 0b11000000; // vgatextmode: enable video, yes bitmap, no text mode, no cursor

  for(i = 0; i < SCREEN_WIDTH*SCREEN_HEIGHT; i++) bitmap[i] = 0;  // clear screen
}

//void fractal_to_file(File &Image)
void fractal_to_screen(uint8_t iterations)
{
  // Create vectors in RAM.
  // Vector_RAM will be allocated in RAM to given number of elemennts.
  // It can be any number of Vector_RAM registers as long as they fit into RAM memory.
  // Each Vector_RAM can be of any size but only first 2048 elements can be loaded or stored
  // to Vector_REG registers (hardware limitation).
  // CPU can directly read or write to elements of Vector_RAM.
  // Vector_RAM can be used for loading and storing Vector_REG results
  // and as a temporary storage.

  // Associate hardware vector registers indexed by number 0-7 with
  // comprehensive variable names which perform vector operation in
  // simple c++ expressions: A=B+C, A=B*C, A=B+B, A=B-B...
  // Only simple binary expressions currently do work.
  // Expressions which do not work are: A=A+A, A=A+B, A=B+C+D, A=B+C*D.
  // Compound operators also don't work: A+=A, A+=B, etc ...
  // CPU cannot directly access individual elements of Vector_REG.
  // For CPU to prepare input data and get results from Vector_REG,
  // load or store to Vector_RAM registers has to be used.
  // Hardware limit of hardware vector register length is 2048 elements,
  // therefore load/store can max to this length, ignoring the reset.
  Vector_REG Vaz(0), Vbz(2), Vha(4), Vhb(6), Vhc(8), Vac(10), Vbc(12), Vtmp(14);
  // odd numbers are aliases of the above
  Vector_REG Vaz1(1),Vbz1(3),Vha1(5),Vhb1(7),Vhc1(9),Vac1(11),Vbc1(13),Vtmp1(15);

  // Arithmetic operations between Vector_REG's are FAST (1 element = 1 clock).
  // load/store operations between Vector_RAM and Vector_REG are SLOW.

  int x, y, i;
  float ac;
  float spalt_x, spalt_y;
  float aecke, becke, seite_x, seite_y;
  float linke_ecke, rechte_ecke, untere_ecke, obere_ecke;

  linke_ecke = -1.45;
  rechte_ecke = 0.60;

  obere_ecke = 1.15;
  untere_ecke = -1.15;

  aecke = linke_ecke;
  becke = obere_ecke;

  seite_x = rechte_ecke - linke_ecke;
  seite_y = untere_ecke - obere_ecke;

  spalt_x = seite_x / (float) SCREEN_WIDTH;
  spalt_y = seite_y / (float) SCREEN_HEIGHT;

  ac = aecke;
  for(x = 0; x < SCREEN_WIDTH; x++)
    Mac.vh->data[x].f = (ac += spalt_x);

  // the constants to Vbc
  Mconst.vh->data[1].f = 1024.0; // red scale constant
  Mconst.vh->data[2].f = 1024.0; // green scale constant
  Mconst.vh->data[3].f = 1024.0; // blue scale constant
  Mconst.vh->data[7].f = spalt_y; // the Mbc increment
  Vhc.range(2040,2047);
  Vhc = Mconst;

  Mconst.vh->data[0].f = becke;
  Vac.range(2047,2047); // only Mbc at top of Vac
  Vac = Mconst; // Vac(2047) contains Mbc

  //Image.write("P6\n# creator f32c fractal\n640 480 255\n");
  Serial.print("line: ");

  //initialize: all vectors must have the same size, aliases too
  Vaz.range(0, SCREEN_WIDTH-1);
  Vaz1.range(0, SCREEN_WIDTH-1);
  Vbz.range(0, SCREEN_WIDTH-1);
  Vbz1.range(0, SCREEN_WIDTH-1);
  Vha.range(0, SCREEN_WIDTH-1);
  Vha1.range(0, SCREEN_WIDTH-1);
  Vhb.range(0, SCREEN_WIDTH-1);
  Vhb1.range(0, SCREEN_WIDTH-1);
  Vhc.range(0, SCREEN_WIDTH-1);
  Vhc1.range(0, SCREEN_WIDTH-1);
  Vac.range(0, SCREEN_WIDTH-1);
  Vac1.range(0, SCREEN_WIDTH-1);
  Vbc.range(0, SCREEN_WIDTH-1);
  Vbc1.range(0, SCREEN_WIDTH-1);
  Vtmp.range(0, SCREEN_WIDTH-1);
  Vtmp1.range(0, SCREEN_WIDTH-1);

  // run time
  int tstart = millis();
  // now real application initialization
  Vac = Mac;
  // Vbc = Mbc; // vbc unused???
  for(y = 0; y < SCREEN_HEIGHT; y++)
  {
    // printf("processing screen line %d\n", y);
    // set vectors to zero before each horizontal line
    Vaz = Vac - Vac; // 0
    Vbz = Vac - Vac; // 0
    Vha = Vac - Vac; // 0
    Vhb = Vac - Vac; // 0
    // set no pixels are done yet in this line
    //line_reset();
    // calculate the line using vectors
    for(i = iterations-1; i >= 0; i--)
    {
      // main fractal loop is completely done in vector registers (FAST)
      Vhc = Vaz * Vbz;
      Vtmp = Vha - Vhb;
      Vaz = Vtmp + Vac;
      Vtmp = Vhc + Vhc;
      Vac.range(2047,2047); // vbc is constant at the top of vac
      Vbz = Vtmp + Vac; // add vbc constant
      Vac.range(0, SCREEN_WIDTH-1); // range back to normal
      Vha = Vaz * Vaz;
      Vhb = Vbz * Vbz;
      if(i == 1)
      {
        // store iteration before the last one
        Mred = Vha;
        Mgreen = Vhb;
      }
    } // for ITERATIONS

    // colorization:
    // red and green channel: difference current Vha,Vhb - previous Vha,Vhb
    // blue channel: Vha+Vhb

    // red channel
    Vaz = Mred;
    Vaz = Vha-Vaz1;
    Vhc.range(2041,2041); // range to red scale constant
    Vaz = Vaz1 * Vhc; // *constant
    Mred = Vaz;
    // green channel
    Vbz = Mgreen;
    Vbz = Vbz1-Vhb;
    Vhc.range(2042,2042); // range to green scale constant
    Vbz = Vbz1 * Vhc; // *constant
    Mgreen = Vbz;
    // blue channel
    Vtmp = Vha + Vhb;
    Vhc.range(2043,2043); // range to blue scale constant
    Vtmp = Vtmp1 * Vhc; // *constant
    Mblue = Vtmp;

    // plot the line
    for(x = 0; x < SCREEN_WIDTH; x++)
      c2_set_pixel(x,y,
              ((       -(uint8_t) Mred.vh->data[x].f)   <<16)
             |((0xFF & -(uint8_t) Mgreen.vh->data[x].f) <<8 )
             |((0xFF & -(uint8_t) Mblue.vh->data[x].f)      )
      );
    // increment bc for the next line
    Vhc.range(2047,2047); // range to constant for Vbc increment
    Vac.range(2047,2047); // Mbc is here at top, Vac(2047)
    Vac1.range(2047,2047);
    Vac = Vac1 + Vhc; // increment
    Vac.range(0, SCREEN_WIDTH-1);
    Vac1.range(0, SCREEN_WIDTH-1);
    Vhc.range(0, SCREEN_WIDTH-1); // range back to normal

    *led_pointer = y; // blink the whole port binary counting
    if((y % 50) == 0)
    {
      Serial.print(y);
      Serial.print(" ");
    }
  } // for y
  int tstop = millis();
  Serial.println(y);
  Serial.print("time: ");
  Serial.print(tstop-tstart);
  Serial.println(" ms");
}

void setup()
{
  // Open serial communications and wait for port to open:
  // Serial.begin(115200);

  // the blinkie pointers
  led_pointer = (volatile uint8_t *) portOutputRegister(digitalPinToPort(led));

  alloc_bitmap();
}

void loop()
{
  static uint8_t i;
  fractal_to_screen(++i % 28);
  delay(1000);
}

