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

void emu_set_pix(int x, int y, int c)
{
  bitmap[x + SCREEN_WIDTH*y] = RGB2PIXEL(color_map[c]);
}

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
	struct vector_header_s *vaz,*vac,*vbz,*vbc,*vha,*vhb,*vhc;
	struct vector_header_s *vbc_inc,*vresult;
	int x, y, i;
	float	spalt_x, spalt_y;
	float aecke, becke, seite_x, seite_y;
	float	linke_ecke, rechte_ecke, untere_ecke, obere_ecke;
	int pixdone[SCREEN_WIDTH]; // 0 if pix is not yet calculated

  // set color map
  for(i = 0; i < ANZCOL; i++)
    color_map[i] = rand();

  // vector length is 1 screen line (640 elements)
	vaz = V.create(SCREEN_WIDTH);
	vac = V.create(SCREEN_WIDTH);
	vbz = V.create(SCREEN_WIDTH);
	vbc = V.create(SCREEN_WIDTH);
	vha = V.create(SCREEN_WIDTH);
	vhb = V.create(SCREEN_WIDTH);
	vhc = V.create(SCREEN_WIDTH);
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
	V.io(5, vac, 0);
	V.io(6, vbc, 0);
	for(y = 0; y < SCREEN_HEIGHT; y++)
	{
    //printf("processing screen line %d\n", y);
	  // set vectors to zero before each horizontal line
    V.oper(0, 5, 5, 1); // az=0: v0=v5-v5 
    V.oper(1, 5, 5, 1); // bz=0: v0=v5-v5 
    V.oper(2, 5, 5, 1); // ha=0: v0=v5-v5 
    V.oper(3, 5, 5, 1); // hb=0: v0=v5-v5 
    // set no pixels are done yet in this line
    for(x = 0; x < SCREEN_WIDTH; x++)
      pixdone[x] = 0;
    // calculate the line using vectors
    for(i = 0; i < ANZCOL; i++)
    {
      //printf("iter\n");
            // main fractal loop is completely done in vector registers (FAST)
            V.oper(4, 0, 1, 2); // v4=v0*v1 // hc=az*bz
            V.oper(7, 2, 3, 1); // v7=v2-v3 // az: tmp=ha-hb
            V.oper(0, 7, 5, 0); // v0=v7+v5 // az=tmp+ac
            V.oper(7, 4, 4, 0); // v7=v4+v4 // bz: tmp=hc+hc
            V.oper(1, 7, 6, 0); // v1=v7+v6 // bz=tmp+bc
            V.oper(2, 0, 0, 2); // v2=v0*v0 // ha=az*az
            V.oper(3, 1, 1, 2); // v3=v1*v1 // hb=bz*bz
            V.oper(7, 2, 3, 0); // v7=v2+v3 // tmp=ha+hb
            // only I/O is to store temporery result in RAM to plot pixels
            V.io(7, vresult, 1); // vres=v7 // tmp
            for(x = 0; x < SCREEN_WIDTH; x++)
            {
              if(pixdone[x] == 0)
              {
                // if pixel is not yet placed, check calculation results
                if(vresult->data[x].f > 2.0)
                {
                  emu_set_pix(x, y, i);
                  pixdone[x] = 1; // pixel is placed, won't check anymore for this line
                }
              }
            }
	  }
	  // increment bc for the next line
	  V.io(0, vbc, 0); // v0=vbc
	  V.io(1, vbc_inc, 0); // v1=vbc_inc
    V.oper(6, 0, 1, 0); // v6=v0+v1
    V.io(6, vbc, 1); // vbc=v6
	}
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
