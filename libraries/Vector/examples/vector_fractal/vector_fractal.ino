/*************************************************** 
(c)EMARD
LICENSE=BSD

Vectorized fractal calculation (Mandelbrot)
based on Emanuel's source
****************************************************/
#include <Compositing.h>
#include <Vector.h>

#define ANZCOL 256
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SPRITE_MAX 10

Compositing c2;
Vector V;

// crude malloc()
pixel_t *bitmap = (pixel_t *)0x80080000;
pixel_t color_map[ANZCOL];

#define emu_set_pix(x,y,c) bitmap[x + SCREEN_WIDTH*y] = color_map[c]

void alloc_bitmap()
{
  int i, x,y;

  c2.init();
  c2.alloc_sprites(SPRITE_MAX);
  bitmap = (pixel_t *) malloc(SCREEN_WIDTH*SCREEN_HEIGHT*sizeof(pixel_t)); // alloc memory for video
  c2.sprite_from_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT, bitmap); // create c2 sprite
  c2.sprite_refresh(); // show it on screen
  *c2.cntrl_reg = 0b11000000; // vgatextmode: enable video, yes bitmap, no text mode, no cursor

  for(i = 0; i < SCREEN_WIDTH*SCREEN_HEIGHT; i++) bitmap[i] = 0;  // clear screen
}

void fractal()
{
  float ac;
  struct vector_header_s *vac, *vbc, *vbc_inc, *vresult;
  Vector_RAM Mac(640);
  Vector_REG Vaz(0), Vbz(1), Vha(2), Vhb(3), Vhc(4), Vac(5), Vbc(6), Vtmp(7);
  int x, y, i;
  float spalt_x, spalt_y;
  float aecke, becke, seite_x, seite_y;
  float linke_ecke, rechte_ecke, untere_ecke, obere_ecke;
  int pixdone[SCREEN_WIDTH]; // 0 if pix is not yet calculated

  // set color map
  for(i = 0; i < ANZCOL; i++)
    color_map[i] = RGB2PIXEL(rand());

  // vector length is 1 screen line (640 elements)
  vac = V.create(SCREEN_WIDTH);
  vbc = V.create(SCREEN_WIDTH);
  vbc_inc = V.create(SCREEN_WIDTH);
  vresult = V.create(SCREEN_WIDTH);

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
    vac->data[x].f = ac;
    ac += spalt_x;
    vbc->data[x].f = becke;
    vbc_inc->data[x].f = spalt_y;
  }
  int tstart = millis();
  V.load(5, vac); // v5=vac
  // Vac = vac;
  V.load(1, vbc); // v1=vbc
  // Vbc = vbc;
  for(y = 0; y < SCREEN_HEIGHT; y++)
  {
    // printf("processing screen line %d\n", y);
    //V.sub(0, 1, 1); // az=0: v0=v1-v1
    Vaz = Vbz - Vbz;
    //V.add(6, 0, 1); // vbc:  v6=v0+v1
    Vbc = Vaz + Vbz;
    // set vectors to zero before each horizontal line
    //V.sub(1, 0, 0); // bz=0: v1=v0-v0
    Vbz = Vaz - Vaz;
    //V.sub(2, 0, 0); // ha=0: v2=v0-v0
    Vha = Vaz - Vaz;
    //V.sub(3, 0, 0); // hb=0: v3=v0-v0
    Vhb = Vaz - Vaz;
    // set no pixels are done yet in this line
    int pixinline=0;
    for(x = 0; x < SCREEN_WIDTH; x++)
      pixdone[x] = 0;
    // calculate the line using vectors
    for(i = 0; i < ANZCOL && pixinline < SCREEN_WIDTH; i++)
    {
      // main fractal loop is completely done in vector registers (FAST)
      //V.mul(4, 0, 1); // v4=v0*v1 // hc=az*bz
      Vhc = Vaz * Vbz;
      //V.sub(7, 2, 3); // v7=v2-v3 // az: tmp=ha-hb
      Vtmp = Vha - Vhb;
      //V.add(0, 7, 5); // v0=v7+v5 // az=tmp+ac
      Vaz = Vtmp + Vac;
      //V.add(7, 4, 4); // v7=v4+v4 // bz: tmp=hc+hc
      Vtmp = Vhc + Vhc;
      //V.add(1, 7, 6); // v1=v7+v6 // bz=tmp+bc
      Vbz = Vtmp + Vbc;
      //V.mul(2, 0, 0); // v2=v0*v0 // ha=az*az
      Vha = Vaz * Vaz;
      //V.mul(3, 1, 1); // v3=v1*v1 // hb=bz*bz
      Vhb = Vbz * Vbz;
      //V.add(7, 2, 3); // v7=v2+v3 // tmp=ha+hb
      Vtmp = Vha + Vhb;
      // only I/O is to store temporery result in RAM to plot pixels
      V.store(vresult, 7); // vresult=v7 // tmp
      // vresult = Vtmp;
      for(x = 0; x < SCREEN_WIDTH; x++)
      {
      	if(pixdone[x] == 0)
        {
          // if pixel is not yet placed, check calculation results
          //if(vresult->data[x].f > 2.0)
          if(vresult->data[x].part.sign == 0 && vresult->data[x].part.exponent > 127) // same as above but faster
          {
            emu_set_pix(x, y, i);
            pixdone[x] = 1; // pixel is placed, won't check anymore for this line
            pixinline++;
          } // if > 2.0
        } // if pixdone
      } // for screen width
    } // for anzcol
    // increment bc for the next line
    V.load(0, vbc_inc); // v0=vbc_inc
    // Vaz = vbc_inc;
    //V.add(1, 6, 0); // v1=v6+v0 (vbc+vbc_inc)
    Vbz = Vbc + Vaz;
  } // for y
  int tstop = millis();

  printf("time %d ms\n", (tstop-tstart));
}

void setup(void)
{
  alloc_bitmap();
  fractal();
}

void loop(void)
{
  delay(1000);
}

