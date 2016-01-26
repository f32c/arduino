#include <stdlib.h>
#include "shape.h"

// ascii-art of the shapes
struct charcolors std_colors[] = 
{ 
  {'O', 0b11110000}, // orange
  {'R', 0b11100000}, // red
  {'Y', 0b11111100}, // yellow
  {'C', 0b00011111}, // cyan
  {'G', 0b00011100}, // green
  {'B', 0b00000011}, // blue
  {'W', 0b11111111}, // white
  {' ', 0x00}, // transparent
  {0, 0}
};

char *shape_ship[] =
{//01234567890123456789012345678901 
"   WWW                          ",
"   WWWOOOOO                     ",
" WWWWWYYYYYYYYYY                ",
"   WWWWWOOO                     ",
"   WWWWWW                       ",
"   WWCWWWWW       BBBBBB        ",
" WWWWCCWWWWWW    BBWWWWWW       ",
"WWWWOOOOOWWWWWWWWWWWWWWWWWW     ",
"  WWOOOOOOYYYWWWWWWYOORRROYWWWWW",
"WWWWOOOOOWWWWWWWWWWWWWWWWWW     ",
" WWWWCCWWWWWW    BBWWWWWW       ",
"   WWCWWWWW       BBBBBB        ",
"   WWWWWW                       ",
"   WWWWWOOO                     ",
" WWWWWYYYYYYYYYY                ",
"   WWWOOOOO                     ",
"   WWW                          ",
NULL
};

struct shape Shape[] =
{
  [0] = { std_colors, shape_ship },
  [1] = { NULL, NULL }
};

