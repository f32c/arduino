#include <stdlib.h>
#include "shape.h"

// ascii-art of the shapes for 8-bpp RGB-332
#if BPP == 8
struct charcolors std_colors[] = 
{ //      RRRGGGBB
  {'O', 0b11110000}, // orange
  {'R', 0b11100000}, // red
  {'Y', 0b11111100}, // yellow
  {'C', 0b00011111}, // cyan
  {'G', 0b00011100}, // green
  {'B', 0b00000011}, // blue
  {'W', 0b11111111}, // white
  {' ', 0}, // transparent
  {0, 0}
};
#endif

// ascii-art of the shapes for 16-bpp RGB-565 (high color)
#if BPP == 16
struct charcolors std_colors[] = 
{ //      RRRRRGGGGGGBBBBB
  {'O', 0b1111110000000000}, // orange
  {'R', 0b1111100000000000}, // red
  {'Y', 0b1111111111100000}, // yellow
  {'C', 0b0000011111111111}, // cyan
  {'G', 0b0000011111100000}, // green
  {'B', 0b0000000000011111}, // blue
  {'W', 0b1111111111111111}, // white
  {' ', 0}, // transparent
  {0, 0}
};
#endif

// ascii-art of the shapes for 32-bpp RGB-888 (true color)
#if BPP == 32
struct charcolors std_colors[] = 
{ //      RRGGBB
  {'O', 0xFF7F00}, // orange
  {'R', 0xFF0000}, // red
  {'Y', 0xFFFF00}, // yellow
  {'C', 0x00FFFF}, // cyan
  {'G', 0x00FF00}, // green
  {'B', 0x0000FF}, // blue
  {'W', 0xFFFFFF}, // white
  {' ', 0}, // transparent
  {0, 0}
};
#endif

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
  [0] = { std_colors, shape_ship },
  [1] = { std_colors, shape_invader },
  [2] = { std_colors, shape_invader2 },
//  [3] = { NULL, NULL }
};

