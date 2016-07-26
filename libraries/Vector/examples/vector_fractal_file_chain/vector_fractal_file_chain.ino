/*
 * (c)EMARD
 * LICENSE=BSD
 *
 * Vector processor test
 * Mandelbrot fractal written to file "fractal.ppm" on SD card
 * Vector commands manually chained
 * Based on the source of Emu
 */

#include <SPI.h>
#include <SD.h>
#include <Vector.h>

const int chipSelect = 4; // not used on f32c

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
#define SCREEN_COLORS 256

// disable output and/or vector store for vector processor speed test
// results for 100MHz spartan-6
// output  store   time (s)
//    1      1     25.55
//    0      1      7.62
//    0      0      4.23
#define ENABLE_OUTPUT 1
// possible to disable vector store, speed test
#define ENABLE_STORE 1

uint8_t pixel_line[3*SCREEN_WIDTH];
struct rgb
{
  uint8_t r,g,b;
};
struct rgb palette[SCREEN_COLORS];

// reset line (black)
void line_reset()
{
  int i;
  for(i = 0; i < 3*SCREEN_WIDTH; i++)
  pixel_line[i] = 0;
}

// set pixel in current line
void line_set_pixel(int16_t x, uint8_t color)
{
  int16_t a = 3*(x % SCREEN_WIDTH);
  pixel_line[a]   = palette[color].r;
  pixel_line[a+1] = palette[color].g;
  pixel_line[a+2] = palette[color].b;
}

void init_palette()
{
  int i;
  for(i = 0; i < SCREEN_COLORS; i++)
  {
    uint32_t rnd = rand();
    palette[i].r = rnd;
    palette[i].g = rnd >>= 8;
    palette[i].b = rnd >>= 8;
  }
}

void fractal_to_file(File &Image)
{
  // Create vectors in RAM.
  // Vector_RAM will be allocated in RAM to given number of elemennts.
  // It can be any number of Vector_RAM registers as long as they fit into RAM memory.
  // Each Vector_RAM can be of any size but only first 2048 elements can be loaded or stored
  // to Vector_REG registers (hardware limitation).
  // CPU can directly read or write to elements of Vector_RAM.
  // Vector_RAM can be used for loading and storing Vector_REG results
  // and as a temporary storage.
  Vector_RAM Mac(SCREEN_WIDTH), Mbc(SCREEN_WIDTH), Mbc_inc(SCREEN_WIDTH), Mresult(SCREEN_WIDTH);

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
  int c;
  int pixdone[SCREEN_WIDTH]; // 0 if pix is not yet calculated
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
  {
    Mac.vh->data[x].f = ac;
    ac += spalt_x;
    Mbc.vh->data[x].f = becke;
    Mbc_inc.vh->data[x].f = spalt_y;
  }

  Image.write("P6\n# creator f32c fractal\n640 480 255\n");
  Serial.print("line: ");

  // set range of all vectors and their aliases
  // it can be set once as the application range won't change
  // each vector contains data for whole scan line, it has 640 elements
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
  Vbc = Mbc; // use Vtmp to temporary hold Mbc
  for(y = 0; y < SCREEN_HEIGHT; y++)
  {
    // set no pixels are done yet in this line
    int pixinline=0;
    #if ENABLE_OUTPUT
    for(x = 0; x < SCREEN_WIDTH; x++)
      pixdone[x] = 0;
    line_reset();
    #endif

    // printf("processing screen line %d\n", y);
    // set vectors to zero before each horizontal line
    // Vaz = Vac - Vac; // 0
    vector_mmio[4] = 0xE0404AA0;
    wait_vector_mask( (1<<0) );

    //Vbz = Vac - Vac; // 0
    vector_mmio[4] = 0xE0404AA2;
    wait_vector_mask( (1<<2) );

    //Vha = Vac - Vac; // 0
    vector_mmio[4] = 0xE0404AA4;
    wait_vector_mask( (1<<4) );

    //Vhb = Vac - Vac; // 0
    vector_mmio[4] = 0xE0404AA6;
    wait_vector_mask( (1<<6) );

    // calculate the line using vectors
    for(i = 0; i < SCREEN_COLORS && pixinline < SCREEN_WIDTH; i++)
    {
      // main fractal loop is completely done in vector registers (FAST)

      //Vhc = Vaz * Vbz;
      vector_mmio[4] = 0xE1004208; // don't wait, next will parallel
      //wait_vector_mask( (1<<8) );

      //Vtmp = Vha - Vhb;
      vector_mmio[4] = 0xE040464E; // parallel run, completely independent
      wait_vector_mask( (1<<8) | (1<<14) ); // wait parallel run

      //Vaz = Vtmp + Vac;
      vector_mmio[4] = 0xE0004AE0; // don't wait, next will chain
      //wait_vector_mask( (1<<0) );

      //Vha = Vaz * Vaz;
      asm("nop");asm("nop");asm("nop");asm("nop");asm("nop"); // few NOPs for chaining to start
      vector_mmio[4] = 0xE1004114; // chaining, use alias of Vaz
      wait_vector_mask( (1<<0) | (1<<4) ); // wait chained

      //Vtmp = Vhc + Vhc;
      vector_mmio[4] = 0xE000488E; // normal run
      wait_vector_mask( (1<<14) );

      //Vbz = Vtmp + Vbc;
      vector_mmio[4] = 0xE0004CE2; // don't wait, next will chain

      // Vhb = Vbz * Vbz;
      asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop"); // few NOPs for chaining to start
      vector_mmio[4] = 0xE1004336; // chained, use alias of Vbz
      wait_vector_mask( (1<<2) | (1<<6) ); // wait chained

      // Vtmp = Vha + Vhb;
      vector_mmio[4] = 0xE000464E; // don't wait, chain to I/O
      //vector_mmio[4] = 0xE000420E; // alternate picture: nice modification Vtmp = Vaz + Vbz;
      //wait_vector_mask( (1<<14) );

      // only I/O is to store temporery result in RAM to plot pixels
      //Mresult = Vtmp1; // I/O chained, using alias of vtmp
      // nops not needed to chain, I/O starts slow enough
      #if ENABLE_STORE
        vector_mmio[0] = (uint32_t) Mresult.vh; // address where to store
        vector_mmio[4] = 0xE381F0FF; // store alias of Vtmp
      #endif
      //vector_flush(Mresult.vh); // discard cache, CPU will read fresh Mresult
      // on scarab it works without discarding cache -> great speed increase :)
      // we don't need to wait for I/O here because CPU plots pixels slow enough
      //wait_vector_mask( (1<<16) | (1<<14) ); // wait for I/O and previous chain member
      #if ENABLE_OUTPUT
      for(x = 0; x < SCREEN_WIDTH; x++)
      {
        if(pixdone[x] == 0)
        {
          // if pixel is not yet placed, check calculation results
          //if(Mresult.vh->data[x].f > 2.0)
          if(Mresult.vh->data[x].part.sign == 0 && Mresult.vh->data[x].part.exponent > 127) // same as above but faster
          {
            line_set_pixel(x, i);
            pixdone[x] = 1; // pixel is placed, won't check anymore for this line
            pixinline++;
          } // if > 2.0
        } // if pixdone
      } // for screen width
      #endif
      #if ENABLE_STORE
        wait_vector_mask( (1<<16) | (1<<14) ); // just to clear flag wait for I/O and Vtmp calculation
      #else
        wait_vector_mask( (1<<14) ); // just wait for Vtmp calculation
      #endif
    } // for screen_colors
    #if ENABLE_OUTPUT
      Image.write(pixel_line, 3*SCREEN_WIDTH);
    #endif
    if((y % 50) == 0)
    {
      Serial.print(y);
      Serial.print(" ");
    }
    // increment bc for the next line
    Vtmp = Mbc_inc;
    //Vbc = Vbc1 + Vtmp; // in-place compound expression must use different alias on left and right hand side
    vector_mmio[4] = 0xE0004EDC; // compound add to "C" with self-alias "D" on right hand side
    wait_vector_mask( (1<<12) );

  } // for y
  int tstop = millis();
  Serial.println(y);
  Serial.print("time: ");
  Serial.print(tstop-tstart);
  Serial.println(" ms");
}

void create_file()
{
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("fractal.ppm", FILE_WRITE|O_TRUNC);

  // if the file is available, write to it:
  if (dataFile) {
    fractal_to_file(dataFile);
    dataFile.close();
    // print to the serial port too:
    Serial.println("image written to file fractal.ppm");
  }
  // if the file isn't open, pop up an error:
  else
  {
    Serial.println("error opening datalog.txt");
  }
}

void setup()
{
  // Open serial communications and wait for port to open:
  //Serial.begin(115200);

  delay(1000);
  init_palette();
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
  create_file();
}

void loop()
{
  delay(1000);
}

