#include <stdlib.h>
#include "shape.h"

// ascii-art of the shapes
const struct charcolors std_colors[] =
{ //      RRGGBB
  {'O', RGB2PIXEL(0xFF7F00)}, // orange
  {'R', RGB2PIXEL(0xFF0000)}, // red
  {'Y', RGB2PIXEL(0xFFFF00)}, // yellow
  {'C', RGB2PIXEL(0x00FFFF)}, // cyan
  {'G', RGB2PIXEL(0x00FF00)}, // green
  {'B', RGB2PIXEL(0x0000FF)}, // blue
  {'W', RGB2PIXEL(0xFFFFFF)}, // white
  {' ', RGB2PIXEL(0)}, // transparent
  {0, 0}
};

const char *shape_ship[] =
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

const char *shape_invader[] =
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

const char *shape_invader2[] =
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

const struct shape Shape[] =
{
  [0] = { std_colors, shape_ship },
  [1] = { std_colors, shape_invader },
  [2] = { std_colors, shape_invader2 },
//  [3] = { NULL, NULL }
};

