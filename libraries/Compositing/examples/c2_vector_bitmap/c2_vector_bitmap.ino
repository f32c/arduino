/*************************************************** 
  An example that compares floating point vector processor unit
  with original software floats from C library on a set
  of data from rand()/x^2 function.

  If there is no vector processor,, only 4 static sprites
  will be shown, 2 in random color, 2 in green-red gradient.

  If vector processor is present, then
  Results are shown in color on compositing2 sprites,
  where the vector engine I/O is used as video blitter.

  4 primary floating operations are tested: +,-,*,/.
  Parallel run of multiple vector arithmetic and I/O
  is also tested.

  Input data are shown as horizontal stripes 8*256 pixels.
  First stripe is a result of "+" operation from hardware vector unit, 
  second is result from "+" of software C floating point, 
  third is colorized difference of hardware-software where green pixels
  indicate success and red pixels failure. 
  The 3-stripe pattern is repeated for (-,*,/).
  
  4 horizontal completely green stripes in animated c2 sprites
  should be visible as indication of successful test.
  Test failure is visible by stripes partially or 
  completely red.

  Vector AXI DMA has some startup problems (f32c core freezes)
  so keep trying:
  1. upload the vector bitstream
  2. uploat this sketch
  3. if it doesn't work, goto 1
  Normally less then 10 retries should be enough
  to get this working properly.
 ****************************************************/

#include <Compositing.h>
#include <math.h>
#define SPRITE_MAX 10

pixel_t *green_blue = (pixel_t *) 0x80080000;
pixel_t *red_green  = (pixel_t *) 0x80180000;

#define VECTOR_SIZE 2048
#define ALLOW_LOAD 1
#define ALLOW_VECTOR 1
#define ALLOW_STORE 1
#define ALLOW_VIDEO 1

Compositing c2;

//                            RRGGBB
#define C2_WHITE  RGB2PIXEL(0xFFFFFF)
#define C2_GREEN  RGB2PIXEL(0x002200)
#define C2_ORANGE RGB2PIXEL(0xFF7F00)
#define C2_BLUE   RGB2PIXEL(0x4080FF)

#define RESOLUTION_X 640
#define RESOLUTION_Y 480

#define BLOCK_X 256
#define BLOCK_Y 256

// xy rotation center and radius
#define CX ((RESOLUTION_X-BLOCK_X)/2)
#define CY ((RESOLUTION_Y-BLOCK_Y)/2)

#define RX CX
#define RY CY

volatile uint32_t *vector_mmio = (volatile uint32_t *)0xFFFFFC20;

struct floa32
{
  uint32_t mantissa:23;
  uint32_t exponent:8;
  uint32_t sign:1;
};

// wait for vector done
// all bits in mask must appear in order to exit this function
void wait_vector_mask(uint32_t mask)
{
  uint32_t i=0, a;
  do
  {
    a = vector_mmio[1];
  } while((a & mask) != mask /*&& ++i < 100000*/);
  vector_mmio[1] = a; // clear interrupt flag(s)
}

// wait for first interrupt flag to be set
void wait_vector(void)
{
  uint32_t i=0, a;
  do
  {
    a = vector_mmio[1];
  } while(a == 0 /*&& ++i < 100000*/);
  vector_mmio[1] = a; // clear interrupt flag(s)
}


void setup() 
{
  int i;
  float *gb, *rg;

  Serial.begin(115200);

  //Serial.println("start creating data set");
  // sprite 0
  // pixel_t *green_blue = (pixel_t *)malloc(BLOCK_X*BLOCK_Y*sizeof(pixel_t));
  gb = (float *)green_blue;
  for(int i = 0; i < BLOCK_X*BLOCK_Y; i++)
    gb[i] = 1.0*(BLOCK_X*BLOCK_Y-1-i);
    //gb[i] = 1.0/(i*i*i);

  // sprite 1
  // pixel_t *red_green = (pixel_t *)malloc(BLOCK_X*BLOCK_Y*sizeof(pixel_t));
  rg = (float *)red_green;
  for(int i = 0; i < BLOCK_X*BLOCK_Y; i++)
    rg[i] = 1.0*rand()/(i*i);
    //rg[i] = 1.0*i;
  Serial.println("finished creating data set");

  #if ALLOW_VIDEO
  c2.init();
  c2.alloc_sprites(SPRITE_MAX);

  // sprite 0
  i = c2.sprite_from_bitmap(BLOCK_X, BLOCK_Y, green_blue);
  c2.Sprite[i]->x = 0;
  c2.Sprite[i]->y = 0;

  // sprite 1
  i = c2.sprite_from_bitmap(BLOCK_X, BLOCK_Y, red_green);
  c2.Sprite[i]->x = RESOLUTION_X-BLOCK_X;
  c2.Sprite[i]->y = 0;

  // sprite 2
  i = c2.sprite_clone(0);
  c2.Sprite[i]->x = RESOLUTION_X-BLOCK_X;
  c2.Sprite[i]->y = RESOLUTION_Y-BLOCK_Y;

  // sprite 3
  i = c2.sprite_clone(1); 
  c2.Sprite[i]->x = 0;
  c2.Sprite[i]->y = RESOLUTION_Y-BLOCK_Y;

  // draw them all
  c2.sprite_refresh();
  *c2.cntrl_reg = 0b11000000; // enable video, yes bitmap, no text mode, no cursor
  #endif

  // run vector engine and test with C library
  
  // vector_mmio[0] = green_blue+128*256-1024;
  uint32_t *arg1    = red_green+96*256,  *arg2     = red_green+128*256,
           *vec_add = green_blue+ 0*256, *test_add = green_blue+10*256, *diff_add = green_blue+20*256,
           *vec_sub = green_blue+32*256, *test_sub = green_blue+42*256, *diff_sub = green_blue+52*256,
           *vec_mul = green_blue+64*256, *test_mul = green_blue+74*256, *diff_mul = green_blue+84*256,
           *vec_div = green_blue+96*256, *test_div = green_blue+106*256,*diff_div = green_blue+116*256;

  // cast pointers to represent floats
  // software float will used them to 
  // calculate results for comparison
  float *farg1 = (float *)arg1;
  float *farg2 = (float *)arg2;
  float *ftest_add = (float *)test_add;
  float *fvec_add = (float *)vec_add;
  float *ftest_sub = (float *)test_sub;
  float *fvec_sub = (float *)vec_sub;
  float *ftest_mul = (float *)test_mul;
  float *fvec_mul = (float *)vec_mul;
  float *ftest_div = (float *)test_div;
  float *fvec_div = (float *)vec_div;

  // hardware vector engine math run
  vector_mmio[1] = 0xFFFFFFFF; // clear all interrupt flags

  #if ALLOW_LOAD
    vector_mmio[0] = (uint32_t) arg1;
    vector_mmio[4] = 0x01001200; // load vector 1 and 4 (selected by bitmask)
    wait_vector();
    vector_mmio[0] = (uint32_t) arg2;
    vector_mmio[4] = 0x01002400; // load vector 2 and 5 (selected by bitmask)
    wait_vector();
  #endif
  #if ALLOW_VECTOR
    // parallel 2 vector operations
    vector_mmio[4] = 0x34000210; // v(0) = v(1) / v(2) float (selected by index)
    vector_mmio[4] = 0x33020543; // v(3) = v(4) * v(5) float (selected by index)
    wait_vector_mask((1<<3)|(1<<0)); // wait for parallel run of 2 functional units
    #if ALLOW_STORE
      // parallel store of vector 3 and calculation of vector 4
      vector_mmio[0] = vec_div;
      vector_mmio[4] = 0x01800000; // store vector 0 (selected by index)
      vector_mmio[4] = 0x33010214; // v(4) = v(1) - v(2) float (selected by index)
      wait_vector_mask((1<<16)|(1<<4)); // wait for parallel I/O (16) and vector(4)
    #else
      vector_mmio[4] = 0x33010214; // v(4) = v(1) - v(2) float (selected by index)
      wait_vector_mask((1<<4));
    #endif
    vector_mmio[4] = 0x33000210; // v(0) = v(1) + v(2) float (selected by index)
    wait_vector();
  #endif
  #if ALLOW_STORE
    vector_mmio[0] = (uint32_t) vec_mul;
    vector_mmio[4] = 0x01800003; // store vector 3 (selected by index)
    wait_vector();  
    vector_mmio[0] = (uint32_t) vec_sub;
    vector_mmio[4] = 0x01800004; // store vector 4 (selected by index)
    wait_vector();
    vector_mmio[0] = (uint32_t) vec_add;
    vector_mmio[4] = 0x01800000; // store vector 0 (selected by index)
    wait_vector();
  #endif

  // software math run
  // do the same on traditional way
  // using software floating point from C library
  for(i = 0; i < VECTOR_SIZE; i++)
  {
    ftest_add[i] = farg1[i] + farg2[i];
    ftest_sub[i] = farg1[i] - farg2[i];
    ftest_mul[i] = farg1[i] * farg2[i];
    ftest_div[i] = farg1[i] / farg2[i];
  }

  // the comparison

  // +,-,* vector are compared to 0-tolerance binary-exact values
  // coming from software C library floating point.
  // for vector division "/", sign/exp/mantissa are separately
  // compared, tolerating mantissa to differ by max 1
  struct floa32 *vec_div_f32 = (floa32 *)vec_div;
  struct floa32 *test_div_f32 = (floa32 *)test_div;
  int fail_add_count=0, fail_sub_count=0, fail_mul_count=0, fail_div_count=0;
  for(i = 0; i < VECTOR_SIZE; i++)
  {
    int cmp_add = vec_add[i] == test_add[i];
    int cmp_sub = vec_sub[i] == test_sub[i];
    int cmp_mul = vec_mul[i] == test_mul[i];
    int cmp_div = vec_div_f32[i].sign       == test_div_f32[i].sign
               && vec_div_f32[i].exponent == test_div_f32[i].exponent
           && abs(vec_div_f32[i].mantissa -  test_div_f32[i].mantissa) < 2; // tolerate difference by 1
    // show on the video
    diff_add[i] = cmp_add ? RGB2PIXEL(0x00FF00)/*green*/ : RGB2PIXEL(0xFF0000)/*red*/;
    diff_sub[i] = cmp_sub ? RGB2PIXEL(0x00FF00)/*green*/ : RGB2PIXEL(0xFF0000)/*red*/;
    diff_mul[i] = cmp_mul ? RGB2PIXEL(0x00FF00)/*green*/ : RGB2PIXEL(0xFF0000)/*red*/;
    diff_div[i] = cmp_div ? RGB2PIXEL(0x00FF00)/*green*/ : RGB2PIXEL(0xFF0000)/*red*/;
    // count difference for printing
    fail_add_count += cmp_add ? 0 : 1;
    fail_sub_count += cmp_sub ? 0 : 1;
    fail_mul_count += cmp_mul ? 0 : 1;
    fail_div_count += cmp_div ? 0 : 1;
  }
  Serial.print("add errors: ");
  Serial.println(fail_add_count, DEC);
  Serial.print("sub errors: ");
  Serial.println(fail_sub_count, DEC);
  Serial.print("mul errors: ");
  Serial.println(fail_mul_count, DEC);
  Serial.print("div errors: ");
  Serial.println(fail_div_count, DEC);

  // red=error, green=pass
}

// this will animate the sprites
// and use vector I/O as blitter
void loop()
{
  int i;
  static uint8_t a; // rotation angle

  a++;

  #if ALLOW_STORE
  for(i = 0; i < BLOCK_X*BLOCK_Y; i+=VECTOR_SIZE)
  {
    vector_mmio[0] = red_green+i;
    vector_mmio[4] = a&1?0x01800001:0x01800004;
    wait_vector();
  }
  #endif

  #if ALLOW_VIDEO
    for(i = 0; i < 4; i++)
    {
      c2.Sprite[i]->x = CX + RX * cos(2*M_PI/256*(a+64*i));    
      c2.Sprite[i]->y = CY + RY * sin(2*M_PI/256*(a+64*i));
    }
    while((*c2.vblank_reg & 0x80) == 0);
    c2.sprite_refresh();
    while((*c2.vblank_reg & 0x80) != 0);
  #endif

  //delay(1000);
}

