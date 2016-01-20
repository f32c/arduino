#define videodisplay_reg (*(volatile uint32_t *)0xFFFFFB90)

// the content: linked list of horizontal pixels
struct compositing_line
{
   struct compositing_line *next; // 32-bit continuation of the same structure, NULL if no more
   int16_t x; // where to start on screen (can be negative)
   uint16_t n; // number of pixels contained here
   // pixels can be multiple of 4 (n lower 2 bits discarded)
   // n could be 0 to skip this struct to the next
   uint8_t pixel[640]; // array of pixels continued here (could be more than 1 element)
};

// some green-to-blue colors
struct compositing_line green_blue =
{
  NULL, // last line in chain (has highest priority on display)
  30, // x-position
  32, // number of color pixels to follow
  {31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0}
};

// similar as above but reverse color order: blue-to-green
struct compositing_line blue_green =
{
  &green_blue, // next line in chain points to previous defined line
  60, // x-position
  32, // number of color pixels to follow:
  {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31}
};

struct compositing_line reddish_line =
{
  &blue_green, // links to previous content
  600, // x-position
  8, // number of color pixels to follow:
  {0xE0,0xE0,0xE0,0xF0,0xF0,0xE0,0xE0,0xE0,}
};

struct compositing_line *scanlines[480];

void setup() {
  // put your setup code here, to run once:
  int i;
  videodisplay_reg = &(scanlines[0]);
  for(i = 0; i < 480; i+=1)
    scanlines[i] = &green_blue;
  for(i = 120; i < 360; i+=1)
    scanlines[i] = &reddish_line;
}

void loop() {
  static int red_dir = 1, green_dir = -1;

  reddish_line.x += red_dir;
  if(reddish_line.x < 4 || reddish_line.x > 636)
    red_dir = -red_dir;

  green_blue.x += green_dir;
  if(green_blue.x < -80 || green_blue.x > 340)
    green_dir = -green_dir;

  delay(10);
}

