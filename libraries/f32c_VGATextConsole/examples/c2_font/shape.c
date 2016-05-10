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


char *shape_d[] =
{/*
 01234567890123456789012345678901 */
"                ",
" CCCCCCCCCCCC   ",
" CCCCCCCCCCCCC  ",
" CC          CC ",
" CC          CC ",
" CC          CC ",
" CC          CC ",
" CC          CC ",
" CC          CC ",
" CC          CC ",
" CC          CC ",
" CC          CC ",
" CC          CC ",
" CCCCCCCCCCCCC  ",
" CCCCCCCCCCCC   ",
"                ",
NULL
};


char *shape_e[] =
{/*
 01234567890123456789012345678901 */
"                ",
" GGGGGGGGGGGGGG ",
" GGGGGGGGGGGGGG ",
" GG             ",
" GG             ",
" GG             ",
" GG             ",
" GGGGGGGGGGG    ",
" GGGGGGGGGGG    ",
" GG             ",
" GG             ",
" GG             ",
" GG             ",
" GGGGGGGGGGGGGG ",
" GGGGGGGGGGGGGG ",
"                ",
NULL
};

char *shape_f[] =
{/*
 01234567890123456789012345678901 */
"                ",
" GGGGGGGGGGGGGG ",
" GGGGGGGGGGGGGG ",
" GG             ",
" GG             ",
" GG             ",
" GG             ",
" GGGGGGGGGGG    ",
" GGGGGGGGGGG    ",
" GG             ",
" GG             ",
" GG             ",
" GG             ",
" GG             ",
" GG             ",
"                ",
NULL
};


char *shape_g[] =
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
" GG      GGGGGG ",
" GG      GGGGGG ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
"  GGGGGGGGGGGG  ",
"   GGGGGGGGGG   ",
"                ",
NULL
};


char *shape_h[] =
{/*
 01234567890123456789012345678901 */
"                ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
" GGGGGGGGGGGGGG ",
" GGGGGGGGGGGGGG ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
"                ",
NULL
};


char *shape_i[] =
{/*
 01234567890123456789012345678901 */
"                ",
"    YYYYYYYY    ",
"    YYYYYYYY    ",
"       YY       ",
"       YY       ",
"       YY       ",
"       YY       ",
"       YY       ",
"       YY       ",
"       YY       ",
"       YY       ",
"       YY       ",
"       YY       ",
"    YYYYYYYY    ",
"    YYYYYYYY    ",
"                ",
NULL
};


char *shape_j[] =
{/*
 01234567890123456789012345678901 */
"                ",
"    GGGGGGGGGGG ",
"    GGGGGGGGGGG ",
"             GG ",
"             GG ",
"             GG ",
"             GG ",
"             GG ",
"             GG ",
"             GG ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
"  GGGGGGGGGGGG  ",
"   GGGGGGGGGG   ",
"                ",
NULL
};


char *shape_k[] =
{/*
 01234567890123456789012345678901 */
"                ",
" GG          GG ",
" GG         GG  ",
" GG        GG   ",
" GG       GG    ",
" GG      GG     ",
" GG     GG      ",
" GGGGGGGG       ",
" GGGGGGGG       ",
" GG     GG      ",
" GG      GG     ",
" GG       GG    ",
" GG        GG   ",
" GG         GG  ",
" GG          GG ",
"                ",
NULL
};


char *shape_l[] =
{/*
 01234567890123456789012345678901 */
"                ",
" GG             ",
" GG             ",
" GG             ",
" GG             ",
" GG             ",
" GG             ",
" GG             ",
" GG             ",
" GG             ",
" GG             ",
" GG             ",
" GG             ",
" GGGGGGGGGGGGG  ",
" GGGGGGGGGGGGG  ",
"                ",
NULL
};

char *shape_m[] =
{/*
 01234567890123456789012345678901 */
"                ",
" GG          GG ",
" GGG        GGG ",
" GGGG      GGGG ",
" GG GG    GG GG ",
" GG  GG  GG  GG ",
" GG   GGGG   GG ",
" GG    GG    GG ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
"                ",
NULL
};


char *shape_n[] =
{/*
 01234567890123456789012345678901 */
"                ",
" GG          GG ",
" GGG         GG ",
" GGGG        GG ",
" GG GG       GG ",
" GG  GG      GG ",
" GG   GG     GG ",
" GG    GG    GG ",
" GG     GG   GG ",
" GG      GG  GG ",
" GG       GG GG ",
" GG        GGGG ",
" GG         GGG ",
" GG          GG ",
" GG          GG ",
"                ",
NULL
};

char *shape_o[] =
{/*
 01234567890123456789012345678901 */
"                ",
"   GGGGGGGGGG   ",
"  GGGGGGGGGGGG  ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
"  GGGGGGGGGGGG  ",
"   GGGGGGGGGG   ",
"                ",
NULL
};


char *shape_p[] =
{/*
 01234567890123456789012345678901 */
"                ",
" CCCCCCCCCCCC   ",
" CCCCCCCCCCCCC  ",
" CC          CC ",
" CC          CC ",
" CC          CC ",
" CC          CC ",
" CCCCCCCCCCCCC  ",
" CCCCCCCCCCCC   ",
" CC             ",
" CC             ",
" CC             ",
" CC             ",
" CC             ",
" CC             ",
"                ",
NULL
};


char *shape_q[] =
{/*
 01234567890123456789012345678901 */
"                ",
"   GGGGGGGGGG   ",
"  GGGGGGGGGGGG  ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
" GG     GG   GG ",
" GG      GG GG  ",
" GG       GGG   ",
" GG       GGG   ",
"  GGGGGGGGG GG  ",
"   GGGGGGG   GG ",
"                ",
NULL
};


char *shape_r[] =
{/*
 01234567890123456789012345678901 */
"                ",
" CCCCCCCCCCCC   ",
" CCCCCCCCCCCCC  ",
" CC          CC ",
" CC          CC ",
" CC          CC ",
" CC          CC ",
" CCCCCCCCCCCCC  ",
" CCCCCCCCCCCC   ",
" CC     CC      ",
" CC      CC     ",
" CC       CC    ",
" CC        CC   ",
" CC         CC  ",
" CC          CC ",
"                ",
NULL
};

char *shape_s[] =
{/*
 01234567890123456789012345678901 */
"                ",
"   CCCCCCCCCCCC ",
"  CCCCCCCCCCCCC ",
" CC             ",
" CC             ",
" CC             ",
" CC             ",
"  CCCCCCCCCCC   ",
"   CCCCCCCCCCC  ",
"             CC ",
"             CC ",
"             CC ",
"             CC ",
" CCCCCCCCCCCCC  ",
" CCCCCCCCCCCC   ",
"                ",
NULL
};

char *shape_t[] =
{/*
 01234567890123456789012345678901 */
"                ",
" YYYYYYYYYYYYYY ",
" YYYYYYYYYYYYYY ",
"       YY       ",
"       YY       ",
"       YY       ",
"       YY       ",
"       YY       ",
"       YY       ",
"       YY       ",
"       YY       ",
"       YY       ",
"       YY       ",
"       YY       ",
"       YY       ",
"                ",
NULL
};

char *shape_u[] =
{/*
 01234567890123456789012345678901 */
"                ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
"  GGGGGGGGGGGG  ",
"   GGGGGGGGGG   ",
"                ",
NULL
};

char *shape_v[] =
{/*
 01234567890123456789012345678901 */
"                ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
"  GG        GG  ",
"   GG      GG   ",
"    GG    GG    ",
"     GG  GG     ",
"      GGGG      ",
"       GG       ",
"                ",
NULL
};

char *shape_w[] =
{/*
 01234567890123456789012345678901 */
"                ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
" GG          GG ",
" GG    GG    GG ",
" GG   GGGG   GG ",
" GG  GG  GG  GG ",
" GG GG    GG GG ",
" GGGG      GGGG ",
" GGG        GGG ",
" GG          GG ",
"                ",
NULL
};

char *shape_x[] =
{/*
 01234567890123456789012345678901 */
"                ",
" GG          GG ",
"  GG        GG  ",
"   GG      GG   ",
"    GG    GG    ",
"     GG  GG     ",
"      GGGG      ",
"       GG       ",
"       GG       ",
"      GGGG      ",
"     GG  GG     ",
"    GG    GG    ",
"   GG      GG   ",
"  GG        GG  ",
" GG          GG ",
"                ",
NULL
};


char *shape_y[] =
{/*
 01234567890123456789012345678901 */
"                ",
" GG          GG ",
"  GG        GG  ",
"   GG      GG   ",
"    GG    GG    ",
"     GG  GG     ",
"      GGGG      ",
"       GG       ",
"       GG       ",
"       GG       ",
"       GG       ",
"       GG       ",
"       GG       ",
"       GG       ",
"       GG       ",
"                ",
NULL
};

char *shape_z[] =
{/*
 01234567890123456789012345678901 */
"                ",
" CCCCCCCCCCCCCC ",
" CCCCCCCCCCCCC  ",
"           CC   ",
"          CC    ",
"         CC     ",
"        CC      ",
"       CC       ",
"      CC        ",
"     CC         ",
"    CC          ",
"   CC           ",
"  CC            ",
" CCCCCCCCCCCCCC ",
" CCCCCCCCCCCCCC ",
"                ",
NULL
};


struct shape Shape[] =
{
  [0] = { std_colors, shape_slash },
  [1] = { std_colors, shape_a },
  [2] = { std_colors, shape_b },
  [3] = { std_colors, shape_c },
  [4] = { std_colors, shape_d },
  [5] = { std_colors, shape_e },
  [6] = { std_colors, shape_f },
  [7] = { std_colors, shape_g },
  [8] = { std_colors, shape_h },
  [9] = { std_colors, shape_i },
  [10] = { std_colors, shape_j },
  [11] = { std_colors, shape_k },
  [12] = { std_colors, shape_l },
  [13] = { std_colors, shape_m },
  [14] = { std_colors, shape_n },
  [15] = { std_colors, shape_o },
  [16] = { std_colors, shape_p },
  [17] = { std_colors, shape_q },
  [18] = { std_colors, shape_r },
  [19] = { std_colors, shape_s },
  [20] = { std_colors, shape_t },
  [21] = { std_colors, shape_u },
  [22] = { std_colors, shape_v },
  [23] = { std_colors, shape_w },
  [24] = { std_colors, shape_x },
  [25] = { std_colors, shape_y },
  [26] = { std_colors, shape_z },
  [27] = { NULL, NULL }
};

