//
// ULX2S framebuffer demo
//
// Pressing a button toggles between drawing random
// lines, circles, dots, rectangles and text
//


extern "C" {
#include <fb.h>
#include <io.h>
//uint32_t random();
}


void setup() {
  // put your setup code here, to run once:
  fb_set_mode(1);
}

int test;
int key;

void loop() {
  // put your main code here, to run repeatedly:
  int i, tmp;
  int x0, y0, x1, y1, r;
  
  INB(tmp, IO_PUSHBTN);
  if (key != tmp && tmp != 0)
    test++;
  key = tmp;
  if (test > 5)
    test = 0;
  
  OUTB(IO_LED, test);

  switch (test) {
  case 0:
    for (i = 0; i < 1000; i++) {
      x0 = random() & 0x1ff;
      y0 = (random() & 0xff) + 16;
      x1 = random() & 0x1ff;
      y1 = (random() & 0xff) + 16;
      fb_line(x0, y0, x1, y1, random());
    }
    break;
  case 1:
    for (i = 0; i < 1000; i++) {
      x0 = random() & 0x1ff;
      y0 = (random() & 0xff) + 16;
      r = random() & 0x7f;
      fb_circle(x0, y0, r, random());
    }
    break;
  case 2:
    for (i = 0; i < 100; i++) {
      x0 = random() & 0x1ff;
      y0 = (random() & 0xff) + 16;
      r = random() & 0x7f;
      fb_filledcircle(x0, y0, r, random());
    }
    break;
  case 3:
    for (i = 0; i < 1000; i++) {
      x0 = random() & 0x1ff;
      y0 = (random() & 0xff) + 16;
      fb_plot(x0, y0, random());
    }
    break;
  case 4:
    for (i = 0; i < 100; i++) {
      x0 = random() & 0x1ff;
      y0 = (random() & 0xff) + 16;
      x1 = random() & 0x1ff;
      y1 = (random() & 0xff) + 16;
      fb_rectangle(x0, y0, x1, y1, random());
    }
    break;
  case 5:
    for (i = 0; i < 100; i++) {
      x0 = random() & 0x1ff;
      y0 = (random() & 0xff) + 16;
      r = random() & 0x70007; // size X and Y
      fb_text(x0, y0, "FPGArduino", random(), random(), r);
    }
    break;
  }
}
