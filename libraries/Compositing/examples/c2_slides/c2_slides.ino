/************************************************* 
   Convert slide.jpg -scale 624x480 010pictr.ppm
   Copy all slides to SD card. Each slide file
   should be named in 8.3 format like "FILENAME.PPM".

   Recommended naming is this:

   /slides/000first.ppm
   /slides/050secnd.ppm
   ...
   /slides/999last.ppm

   All slides from "/slides" directory will be read from
   SD card to RAM and shown sorted in alphabetical order.
   Press BTN RIGHT/LEFT to display next/previous slide.
   About 50 slides can fit into 32 MB SDRAM for 16bpp display.
   During reading (3 minutes max), screen may flicker
   but it will become stable when all slides are read to RAM.
 *************************************************/

#include <Compositing.h>
#include <math.h>
#include <SD.h>

#define BTN_UP 3
#define BTN_DOWN 4
#define BTN_LEFT 5
#define BTN_RIGHT 6

// max number of slides to be loaded
#define SPRITE_MAX 100

Compositing c2;

//                            RRGGBB
#define C2_WHITE  RGB2PIXEL(0xFFFFFF)
#define C2_GREEN  RGB2PIXEL(0x002200)
#define C2_ORANGE RGB2PIXEL(0xFF7F00)
#define C2_BLUE   RGB2PIXEL(0x4080FF)

//#define RESOLUTION_X VGA_X_MAX
// 624x480 16bpp is max useable until SDRAM burst is supported
#define RESOLUTION_X 624
#define RESOLUTION_Y VGA_Y_MAX
#define OFF_SCREEN_Y 2000

// forward slides will have priority reading.
// N times more forward than backward slides reading
#define FORWARD_PRIORITY 2

PortRegister_t btn;

#define NAMES_MAX SPRITE_MAX

struct S_DirEntries
{
  char *name; // item name: pointer to 0-terminated string
  size_t size; // file size (bytes)
  // uint8_t type; // item type 1-directory, 0-file
};
struct S_DirEntries DirEntries[NAMES_MAX];
int DirEntries_count = 0;

File bg_file; // opened file for background_reader
uint8_t *PPM_line_buf;

// description struct for each slide
struct S_Slide
{
  char *path; // file path
  size_t pos; // seek() to continue reading
  uint16_t x,y; // x/y size
  uint16_t x_rd; // x-size on file (sprite size may be limited to resolution_x)
  uint16_t y_rd; // runs from 0 to y while reading image from SD
};
struct S_Slide Slide[NAMES_MAX];

int Slides_count = 0;
int Slide_shown = 0; // slide currently shown on-screen
int Old_Slide_shown = Slide_shown; // previous cycle

// string compare function for directory sorting
int DirNameCmp(const void *a, const void *b)
{
  const struct S_DirEntries *sa = (const struct S_DirEntries *)a;
  const struct S_DirEntries *sb = (const struct S_DirEntries *)b;
  // TODO: strcasecmp(sa,sb) should be used but it's not in f32c lib
  return strcmp(sa->name, sb->name);
}

// read positive integer from file
// used to parse PPM headers
int read_integer(File *file)
{
  char c;
  int i = -1; // return -1 if no integer found
  uint8_t comment = 0;
  while(file->available() > 0)
  {
    c = file->read();
    if(c == '#')
      comment = 1;
    if(c == '\n')
      return i;
    if(comment == 0)
    {
      if(i >= 0 && (c == ' ' || c == '\t'))
        return i;
      if(c >= '0' && c <= '9')
        i = 10 * (i > 0 ? i : 0) + (c - '0');
    }
  }
  return i;
}


int header_allocate_slide(char *slide_path)
{
  int i, x, y, sprite_index = -1;
  // allocate memory for each sprite sizeof(pixel_t)*file_size/3
  // generate this sprite for c2 use
  Serial.print("allocating: ");
  Serial.print(slide_path);
  File dataFile = SD.open(slide_path);
  // parse PPM header "P6"
  if(read_integer(&dataFile) != 6)
  {
    dataFile.close();
    return -1; // 6 was expected
  }
  for(x = -1; x < 0; x = read_integer(&dataFile));
  Serial.print(" ");
  Serial.print(x);
  for(y = -1; y < 0; y = read_integer(&dataFile));
  Serial.print("x");
  Serial.print(y);
  if(read_integer(&dataFile) != 255)
  {
    dataFile.close();
    return -1; // 6 was expected
  }
  // calculate how many pixels in total we have
  int x_sprite = x < RESOLUTION_X ? x : RESOLUTION_X;
  int total_pixels = x_sprite * y;
  int x_even = c2.x_even_size(x_sprite);
  int memory_size = x_even * y * sizeof(pixel_t);
  pixel_t *picture_memory = (pixel_t *)malloc(memory_size);
  Serial.print(" 0x");
  Serial.println((uint32_t)picture_memory, HEX);
  sprite_index = c2.sprite_from_bitmap(x_even, y, picture_memory);
  // make only first sprite visible, others off screen
  if(sprite_index == 0)
  {
    c2.Sprite[sprite_index]->x = 0;
    c2.Sprite[sprite_index]->y = 0;
    // refresh sprite will show it on screen
    c2.sprite_refresh();
    // enable video
    *c2.cntrl_reg = 0b11000000; // enable video, yes bitmap, no text mode, no cursor
  }
  else
  {
    c2.Sprite[sprite_index]->x = 0;
    c2.Sprite[sprite_index]->y = OFF_SCREEN_Y; // off screen
  }
  Slide[sprite_index].path = slide_path;
  Slide[sprite_index].x = x_sprite;
  Slide[sprite_index].y = y;
  Slide[sprite_index].x_rd = x;
  Slide[sprite_index].y_rd = 0; // no lines read from SD yet
  Slide[sprite_index].pos = dataFile.position();
  dataFile.close();
  return sprite_index;
}


void read_slides_dir(char *directory_path)
{
   SD.begin(0);
   String filename;
   // read directory, get file names and sizes
   int dir_path_len = strlen(directory_path);
   File dir = SD.open(directory_path);
   while(true)
   {
     File entry = dir.openNextFile();
     if (! entry)
     {
       // no more files
       break;
     }
     if (!entry.isDirectory())
     {
       // files have sizes, directories do not
       filename = entry.name(); // for string comparation endsWith
       if(DirEntries_count < NAMES_MAX && (filename.endsWith(".ppm") || filename.endsWith(".PPM")) )
       {
         DirEntries[DirEntries_count].name = (char *)malloc(dir_path_len+1+strlen(entry.name())+1);
         DirEntries[DirEntries_count].size = entry.size();
         strcpy(DirEntries[DirEntries_count].name, directory_path);
         DirEntries[DirEntries_count].name[dir_path_len] = '/';
         strcpy(DirEntries[DirEntries_count].name + 1 + dir_path_len, entry.name());
         DirEntries_count++;
       }
     }
     entry.close();
   }
   dir.close();
   // sort file names in alphabetic order
   qsort(DirEntries, DirEntries_count, sizeof(struct S_DirEntries), DirNameCmp);
   // initialize c2 sprites
   c2.init();
   c2.alloc_sprites(DirEntries_count);
   // read each sprite from file
   Serial.println("********* reading files *********");
   for(int i = 0; i < DirEntries_count; i++)
   {
     int sprite_index = header_allocate_slide(DirEntries[i].name);
     if(sprite_index >= 0)
     {
       //read_content(sprite_index);
       Slides_count = sprite_index+1;
     }
   }
}

// background downloader that should be
// called multiple times during vblank
// it will open/close image files and
// store to RAM with priority first image
// shown and immediately following images
// it will exit after one scan line dowloaded
// keep internal state and resume when called
// next time
void background_reader()
{
  static uint8_t finished = 0;
  if(finished)
    return;
  static uint8_t file_open = 0;
  static int prev_slide_shown = 0; // track the previously viewed slide
  static int reading_slide = 0; // track slide being downloaded
  // if we are not finished we will get to this
  if(file_open == 0)
  {
    bg_file = SD.open(Slide[reading_slide].path);
    bg_file.seek(Slide[reading_slide].pos); // seek to position
    file_open = 1;
    Serial.print(reading_slide);
    Serial.print(" RD ");
    Serial.println(Slide[reading_slide].path);
  }

    // file is open
    // TODO: if currently viewed slide has changed to
    // new, not completely downloaded slide,
    // then immediately close this file and open new file for
    // priorty reading new slide.
    if(Slide_shown != prev_slide_shown && prev_slide_shown == reading_slide)
    {
      Slide[reading_slide].pos = bg_file.position();
      bg_file.close();
      file_open = 0;
      reading_slide = Slide_shown;
      prev_slide_shown = Slide_shown;
      return;
    }
    prev_slide_shown = Slide_shown;
    // check are all y lines of image completed
    if(Slide[reading_slide].y_rd < Slide[reading_slide].y)
    {
      // load state, read one line, save state and exit
      int read_remaining = 3*Slide[reading_slide].x_rd;
      // ***** WARNING ***** no overwrite LIMIT to PPM_line_buf
      // clamp read remaining and seek at the end
      int seek_skip = 0;
      if(read_remaining > 3*Slide[reading_slide].x)
      {
        seek_skip = bg_file.position() + read_remaining;
        read_remaining = 3*Slide[reading_slide].x;
      }
      int line_bytes = read_remaining;
      uint8_t *read_ptr = PPM_line_buf;
      while(bg_file.available() > 0 && read_remaining > 0)
      {
        int read_count = bg_file.available();
        if(read_count > read_remaining)
          read_count = read_remaining;
        bg_file.read(read_ptr, read_count);
        read_ptr += read_count;
        read_remaining -= read_count;
      }
      if(seek_skip)
        bg_file.seek(seek_skip);
      int x_even = c2.x_even_size(Slide[reading_slide].x);
      pixel_t *pixel_write = c2.Sprite[reading_slide]->line->bmp
      + x_even * Slide[reading_slide].y_rd;
      int i;
      for(i = 0; i < line_bytes; i += 3)
        *(pixel_write++) = RGB2PIXEL((PPM_line_buf[i]<<16)+(PPM_line_buf[i+1]<<8)+PPM_line_buf[i+2]); 
      // fill 0 up to even, to get rid from vertical garbage line at the right
      for(i = Slide[reading_slide].x; i < x_even; i++)
        *(pixel_write++) = RGB2PIXEL(0);
      Slide[reading_slide].y_rd++; // next line next time
    }
    else
    {
      // all y-lines done. close file
      bg_file.close();
      file_open = 0;
      // more slides to read?
      // priority reading: decide which slide
      // should be read now. Slides that in forward
      // direction follow the shown slide should 
      // have 2:1 or 3:1 priority to
      // the previously shown slides
      int next_forward_slide = -1;
      for(int i = Slide_shown; i < Slides_count && next_forward_slide < 0; i++)
        if(Slide[i].y_rd < Slide[i].y)
          next_forward_slide = i;
      int next_backward_slide = -1;
      for(int i = Slide_shown-1; i >= 0 && next_backward_slide < 0; i--)
        if(Slide[i].y_rd < Slide[i].y)
          next_backward_slide = i;
      int next_reading_slide = -1;
      if(next_forward_slide >= 0 && next_backward_slide >= 0)
      {
        next_reading_slide =
          (next_forward_slide - Slide_shown)
          < (Slide_shown - next_backward_slide) * FORWARD_PRIORITY
          ? next_forward_slide : next_backward_slide;       
      }
      else
      {
        if(next_forward_slide >= 0)
          next_reading_slide = next_forward_slide;
        else if(next_backward_slide >= 0)
          next_reading_slide = next_backward_slide;
      }
      reading_slide = next_reading_slide;
      if(reading_slide < 0) // all slides read, stop this background
        finished = 1;
    }
  return;
}

void setup() 
{
  int i;
  int x, y;
  // some delay to view first printed messages at serial
  btn = digitalPinToPort(0);
  // temporary buffer for one PPM line
  PPM_line_buf = (uint8_t *)malloc(3 * RESOLUTION_X * sizeof(uint8_t));
  delay(500);
  read_slides_dir("/slides");
}

void loop()
{
  int i;
  static uint8_t btnreleased = 1;
  if(btnreleased)
  {
    if((*btn & (1<<BTN_RIGHT)) != 0)
    {
      if(Slide_shown < Slides_count-1)
        Slide_shown++;
      btnreleased = 0;
    }
    if((*btn & (1<<BTN_LEFT)) != 0)
    {
      if(Slide_shown > 0)
        Slide_shown--;
      btnreleased = 0;
    }
  }
  else
  {
    if( (*btn & ((1<<BTN_LEFT)|(1<<BTN_RIGHT))) == 0 )
      btnreleased = 1;
  }
  if(Slide_shown != Old_Slide_shown)
  {
    c2.Sprite[Old_Slide_shown]->y = OFF_SCREEN_Y; // off screen
    c2.Sprite[Slide_shown]->y = 0; // on screen
    Old_Slide_shown = Slide_shown;
    while((*c2.vblank_reg & 0x80) == 0);
    c2.sprite_refresh();
    Serial.println(Slide_shown);
    while((*c2.vblank_reg & 0x80) != 0);
  }
  else
  {
    while((*c2.vblank_reg & 0x80) == 0);
    for(int i = 0; i < 3; i++)
      background_reader(); // read from SD card, allow interactive
    while((*c2.vblank_reg & 0x80) != 0);
  }
}
