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

char *shape_ship[] =
{/* 
 01234567890123456789012345678901 */
"   WWW  ",
"   WWWOOOOO ",
" WWWWWYYYYYYYYYY",
"   WWWWWOOO ",
"   WWWWWW   ",
"   WWCWWWWW       BBBBBB",
" WWWWCCWWWWWW    BBWWWWWW   ",
"WWWWOOOOOWWWWWWWWWWWWWWWWWW ",
"  WWOOOOOOYYYWWWWWWYOORRROYWWWWW",
"WWWWOOOOOWWWWWWWWWWWWWWWWWW ",
" WWWWCCWWWWWW    BBWWWWWW   ",
"   WWCWWWWW       BBBBBB",
"   WWWWWW   ",
"   WWWWWOOO ",
" WWWWWYYYYYYYYYY",
"   WWWOOOOO ",
"   WWW  ",
NULL
};

char *shape_invader[] =
{/*
 01234567890123456789012345678901 */
"      GGGG  ",
"      GGGG  ",
"    GGGGGGGG",
"    GGGGGGGG",
"  GGGGGGGGGGGG  ",
"  GGGGGGGGGGGG  ",
"GGGG  GGGG  GGGG",
"GGGG  GGGG  GGGG",
"GGGGGGGGGGGGGGGG",
"GGGGGGGGGGGGGGGG",
"    GG    GG",
"    GG    GG",
"  GG  GGGG  GG  ",
"  GG  GGGG  GG  ",
"GG  GG    GG  GG",
"GG  GG    GG  GG",
NULL
};

char *shape_invader2[] =
{/*
 01234567890123456789012345678901 */
"      OOOO  ",
"      OOOO  ",
"    OOOOOOOO",
"    OOOOOOOO",
"  OOOOOOOOOOOO  ",
"  OOOOOOOOOOOO  ",
"OOOO  OOOO  OOOO",
"OOOO  OOOO  OOOO",
"OOOOOOOOOOOOOOOO",
"OOOOOOOOOOOOOOOO",
"  OO        OO  ",
"  OO        OO  ",
"OO            OO",
"OO            OO",
"  OO        OO  ",
"  OO        OO  ",
NULL
};

struct shape Shape[] =
{
  [0] = { std_colors, shape_empty },
  [1] = { std_colors, shape_ship },
  [2] = { std_colors, shape_invader },
  [3] = { std_colors, shape_invader2 },
  [4] = { NULL, NULL }
};

