/*************************************************** 
(c)Emanuel Stiebler
LICENSE=BSD

Example 
compositing2 used as simple linear framebuffer
****************************************************/

#include <Compositing.h>
#define SPRITE_MAX 10
Compositing c2;

//Define Visuals
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define ANZCOL 256

// crude malloc()
pixel_t *bitmap = (pixel_t *)0x80080000;

pixel_t color_map[ANZCOL];
void emu_set_pix(int x, int y, int c)
{
  bitmap[x + SCREEN_WIDTH*y] = RGB2PIXEL(color_map[c]);
}

void setup() 
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

void loop()
{
  int i;
	for(i = 0; i < SCREEN_WIDTH*SCREEN_HEIGHT; i++) bitmap[i] = 0;
	  frac_main(); 
}

void frac_main()
{
	double az,ac,bz,bc;
	double ha, hb, hc;
	int x, y, k, color, i;
	double spalt_x, spalt_y;
	double aecke, becke, seite_x, seite_y;
	double linke_ecke, rechte_ecke, untere_ecke, obere_ecke;
	int zaehler;

  // set color map
  for(i = 0; i < ANZCOL; i++)
    color_map[i] = rand();

	linke_ecke =  -1.45;
	rechte_ecke =  0.60;

	obere_ecke =   1.15;
	untere_ecke = -1.15;

	aecke  = linke_ecke;
	becke  = obere_ecke;

	seite_x = rechte_ecke - linke_ecke;
	seite_y = untere_ecke - obere_ecke;

	spalt_x = seite_x / (double) SCREEN_WIDTH;
	spalt_y = seite_y / (double) SCREEN_HEIGHT;

	bc = becke;
	for (y = 0; y < SCREEN_HEIGHT; y++)	{
		bc = bc+spalt_y;
		ac = aecke;
		for (x = 0 ; x < SCREEN_WIDTH ; x++)	{
			ac = ac+spalt_x;
			az = 0;
			bz = 0;
			zaehler = -1;
			ha = az*az;
			hb = bz*bz;
			do	{
				hc = az*bz;
				az = ha-hb+ac;
				bz = hc+hc+bc;
				zaehler++;
				ha = az*az;
				hb = bz*bz;
			}	while (((ha+hb) < 2.0) && (zaehler < ANZCOL));
			emu_set_pix(x, y, zaehler);
		}
	}
}

