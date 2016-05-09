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

// warning strings length must be 4-byte aligned
char *shape_empty[] = 
{
  "    ",
  NULL
};


char *shape_a[] =
{/*
 01234567890123456789012345678901 */
"                ",
"      OOOO      ",
"     OOOOOO     ",
"    OO    OO    ",
"   OO      OO   ",
"  OO        OO  ",
" OO          OO ",
" OO          OO ",
" OO          OO ",
" OOOOOOOOOOOOOO ",
" OOOOOOOOOOOOOO ",
" OO          OO ",
" OO          OO ",
" OO          OO ",
" OO          OO ",
"                ",
NULL
};


char *shape_b[] =
{/*
 01234567890123456789012345678901 */
"                ",
" CCCCCCCCCCCC   ",
" CCCCCCCCCCCCC  ",
" CC          CC ",
" CC          CC ",
" CC          CC ",
" CC         CC  ",
" CCCCCCCCCCCC   ",
" CCCCCCCCCCCC   ",
" CC         CC  ",
" CC          CC ",
" CC          CC ",
" CC          CC ",
" CCCCCCCCCCCCC  ",
" CCCCCCCCCCCC   ",
"                ",
NULL
};


char *shape_c[] =
{/*
 01234567890123456789012345678901 */
"                ",
"   GGGGGGGGGG   ",
"  GGGGGGGGGGGG  ",
" GG          GG ",
" GG          GG ",
" GG             ",
" GG             ",
" GG             ",
" GG             ",
" GG             ",
" GG             ",
" GG          GG ",
" GG          GG ",
"  GGGGGGGGGGGG  ",
"   GGGGGGGGGG   ",
"                ",
NULL
};


char *shape_slash[] =
{/*
 01234567890123456789012345678901 */
"WW              ",
" WW             ",
"  WW            ",
"   WW           ",
"    WW          ",
"     WW         ",
"      WW        ",
"       WW       ",
"        WW      ",
"         WW     ",
"          WW    ",
"           WW   ",
"            WW  ",
"             WW ",
"              WW",
"W              W",
NULL
};

struct shape Shape[] =
{
  [0] = { std_colors, shape_empty },
  [1] = { std_colors, shape_slash },
  [2] = { std_colors, shape_a },
  [3] = { std_colors, shape_b },
  [4] = { std_colors, shape_c },
  [5] = { NULL, NULL }
};

