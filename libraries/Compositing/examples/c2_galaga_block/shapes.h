/* f32c Galaga
 * AUTHOR=EMARD
 * LICENSE=BSD
 * Artwork, shapes
 */
#include <stdlib.h>
#include "shape.h"

// ascii-art of the shapes
struct charcolors std_colors[] = 
{  //               RRGGBB
  {'O', RGB2PIXEL(0xFF7F00)}, // orange
  {'R', RGB2PIXEL(0xFF0000)}, // red
  {'Y', RGB2PIXEL(0xFFFF00)}, // yellow
  {'C', RGB2PIXEL(0x00FFFF)}, // cyan
  {'G', RGB2PIXEL(0x00FF00)}, // green
  {'B', RGB2PIXEL(0x0000FF)}, // blue
  {'V', RGB2PIXEL(0xFF00FF)}, // violett
  {'W', RGB2PIXEL(0xFFFFFF)}, // white
  {' ', RGB2PIXEL(0)}, // transparent
  {0, 0}
};


char *shape_alien1u[] =
{
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"WWWWBBBBWWWW",
"WWWWBBBBWWWW",
"WWWWBBBBWWWW",
"WWWWBBBBWWWW",
NULL
};

char *shape_alien1d[] =
{
"WWWWBBBBWWWW",
"WWWWBBBBWWWW",
"WWWWBBBBWWWW",
"WWWWBBBBWWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
NULL
};

char *shape_alien1l[] =
{
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"WWWWBBBB",
"WWWWBBBB",
"WWWWBBBB",
"WWWWBBBB",
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
NULL
};

char *shape_alien1r[] =
{
"WWWW",
"WWWW",
"WWWW",
"WWWW",
"BBBBWWWW",
"BBBBWWWW",
"BBBBWWWW",
"BBBBWWWW",
"WWWW",
"WWWW",
"WWWW",
"WWWW",
NULL
};

char *shape_alien2u[] =
{
"    CCCC",
"    CCCC",
"    CCCC",
"    CCCC",
"YYYYRRRRYYYY",
"YYYYRRRRYYYY",
"YYYYRRRRYYYY",
"YYYYRRRRYYYY",
NULL
};

char *shape_alien2d[] =
{
"YYYYRRRRYYYY",
"YYYYRRRRYYYY",
"YYYYRRRRYYYY",
"YYYYRRRRYYYY",
"    CCCC",
"    CCCC",
"    CCCC",
"    CCCC",
NULL
};

char *shape_alien2l[] =
{
"    YYYY",
"    YYYY",
"    YYYY",
"    YYYY",
"CCCCRRRR",
"CCCCRRRR",
"CCCCRRRR",
"CCCCRRRR",
"    YYYY",
"    YYYY",
"    YYYY",
"    YYYY",
NULL
};

char *shape_alien2r[] =
{
"YYYY",
"YYYY",
"YYYY",
"YYYY",
"RRRRCCCC",
"RRRRCCCC",
"RRRRCCCC",
"RRRRCCCC",
"YYYY",
"YYYY",
"YYYY",
"YYYY",
NULL
};

char *shape_alien3u[] =
{
"        WWWW",
"        WWWW",
"        WWWW",
"        WWWW",
"    VVVV    VVVV",
"    VVVV    VVVV",
"    VVVV    VVVV",
"    VVVV    VVVV",
"WWWW            WWWW",
"WWWW            WWWW",
"WWWW            WWWW",
"WWWW            WWWW",
NULL
};

char *shape_alien3d[] =
{
"WWWW            WWWW",
"WWWW            WWWW",
"WWWW            WWWW",
"WWWW            WWWW",
"    VVVV    VVVV",
"    VVVV    VVVV",
"    VVVV    VVVV",
"    VVVV    VVVV",
"        WWWW",
"        WWWW",
"        WWWW",
"        WWWW",
NULL
};

char *shape_alien3l[] =
{
"        WWWW",
"        WWWW",
"        WWWW",
"        WWWW",
"    VVVV",
"    VVVV",
"    VVVV",
"    VVVV",
"WWWW",
"WWWW",
"WWWW",
"WWWW",
"    VVVV",
"    VVVV",
"    VVVV",
"    VVVV",
"        WWWW",
"        WWWW",
"        WWWW",
"        WWWW",
NULL
};

char *shape_alien3r[] =
{
"WWWW",
"WWWW",
"WWWW",
"WWWW",
"    VVVV",
"    VVVV",
"    VVVV",
"    VVVV",
"        WWWW",
"        WWWW",
"        WWWW",
"        WWWW",
"    VVVV",
"    VVVV",
"    VVVV",
"    VVVV",
"WWWW",
"WWWW",
"WWWW",
"WWWW",
NULL
};

char *shape_alien4u[] =
{
"        WWWW",
"        WWWW",
"        WWWW",
"        WWWW",
"    OOOOWWWWOOOO",
"    OOOOWWWWOOOO",
"    OOOOWWWWOOOO",
"    OOOOWWWWOOOO",
"VVVV            VVVV",
"VVVV            VVVV",
"VVVV            VVVV",
"VVVV            VVVV",
NULL
};

char *shape_alien4d[] =
{
"VVVV            VVVV",
"VVVV            VVVV",
"VVVV            VVVV",
"VVVV            VVVV",
"    OOOOWWWWOOOO",
"    OOOOWWWWOOOO",
"    OOOOWWWWOOOO",
"    OOOOWWWWOOOO",
"        WWWW",
"        WWWW",
"        WWWW",
"        WWWW",
NULL
};

char *shape_alien4l[] =
{
"        VVVV",
"        VVVV",
"        VVVV",
"        VVVV",
"    OOOO",
"    OOOO",
"    OOOO",
"    OOOO",
"WWWWWWWW",
"WWWWWWWW",
"WWWWWWWW",
"WWWWWWWW",
"    OOOO",
"    OOOO",
"    OOOO",
"    OOOO",
"        VVVV",
"        VVVV",
"        VVVV",
"        VVVV",
NULL
};

char *shape_alien4r[] =
{
"VVVV",
"VVVV",
"VVVV",
"VVVV",
"    OOOO",
"    OOOO",
"    OOOO",
"    OOOO",
"    WWWWWWWW",
"    WWWWWWWW",
"    WWWWWWWW",
"    WWWWWWWW",
"    OOOO",
"    OOOO",
"    OOOO",
"    OOOO",
"VVVV",
"VVVV",
"VVVV",
"VVVV",
NULL
};

char *shape_alien_radiate1[] =
{
"WWWW",
"WWWW",
"WWWW",
"WWWW",
NULL
};

char *shape_alien_radiate3[] =
{
"WWWWWWWWWWWW",
"WWWWWWWWWWWW",
"WWWWWWWWWWWW",
"WWWWWWWWWWWW",
NULL
};

char *shape_alien_radiate5[] =
{
"WWWWWWWWWWWWWWWWWWWW",
"WWWWWWWWWWWWWWWWWWWW",
"WWWWWWWWWWWWWWWWWWWW",
"WWWWWWWWWWWWWWWWWWWW",
NULL
};

char *shape_alien_radiate7[] =
{
"WWWWWWWWWWWWWWWWWWWWWWWWWWWW",
"WWWWWWWWWWWWWWWWWWWWWWWWWWWW",
"WWWWWWWWWWWWWWWWWWWWWWWWWWWW",
"WWWWWWWWWWWWWWWWWWWWWWWWWWWW",
NULL
};

char *shape_alien_radiate9[] =
{
"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
NULL
};

char *shape_alien_radiate11[] =
{
"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
NULL
};

char *shape_bomb[] =
{
"WWWW",
"WWWW",
"WWWW",
"WWWW",
NULL
};

char *shape_missile0[] =
{
" WW ",
"  WW",
" WW ",
"WW  ",
" WW ",
"  WW",
" WW ",
"WW  ",
" WW ",
"  WW",
" WW ",
NULL
};

char *shape_missile1[] =
{
"  WW",
" WW ",
"WW  ",
" WW ",
"  WW",
" WW ",
"WW  ",
" WW ",
"  WW",
" WW ",
"WW  ",
NULL
};

char *shape_missile2[] =
{
" WW ",
"WW  ",
" WW ",
"  WW",
" WW ",
"WW  ",
" WW ",
"  WW",
" WW ",
"WW  ",
" WW ",
NULL
};

char *shape_missile3[] =
{
"WW  ",
" WW ",
"  WW",
" WW ",
"WW  ",
" WW ",
"  WW",
" WW ",
"WW  ",
" WW ",
"  WW",
NULL
};

char *shape_ship1u[] =
{
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"WWWW    WWWW",
"WWWW    WWWW",
"WWWW    WWWW",
"WWWW    WWWW",
NULL
};

char *shape_ship1d[] =
{
"WWWW    WWWW",
"WWWW    WWWW",
"WWWW    WWWW",
"WWWW    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
NULL
};

char *shape_ship1l[] =
{
"        WWWW",
"        WWWW",
"        WWWW",
"        WWWW",
"WWWWWWWW",
"WWWWWWWW",
"WWWWWWWW",
"WWWWWWWW",
"        WWWW",
"        WWWW",
"        WWWW",
"        WWWW",
NULL
};

char *shape_ship1r[] =
{
"WWWW",
"WWWW",
"WWWW",
"WWWW",
"    WWWWWWWW",
"    WWWWWWWW",
"    WWWWWWWW",
"    WWWWWWWW",
"WWWW",
"WWWW",
"WWWW",
"WWWW",
NULL
};

char *shape_ship2u[] =
{
"    WWWW    WWWW",
"    WWWW    WWWW",
"    WWWW    WWWW",
"    WWWW    WWWW",
"    WWWW    WWWW",
"    WWWW    WWWW",
"    WWWW    WWWW",
"    WWWW    WWWW",
"WWWW    WWWW    WWWW",
"WWWW    WWWW    WWWW",
"WWWW    WWWW    WWWW",
"WWWW    WWWW    WWWW",
NULL
};

char *shape_block_red[] =
{
"RRRR",
"RRRR",
"RRRR",
"RRRR",
NULL
};

char *shape_block_yellow[] =
{
"YYYY",
"YYYY",
"YYYY",
"YYYY",
NULL
};

char *shape_block_orange[] =
{
"OOOO",
"OOOO",
"OOOO",
"OOOO",
NULL
};

char *shape_block_green[] =
{
"GGGG",
"GGGG",
"GGGG",
"GGGG",
NULL
};

char *shape_block_cyan[] =
{
"CCCC",
"CCCC",
"CCCC",
"CCCC",
NULL
};

char *shape_block_blue[] =
{
"BBBB",
"BBBB",
"BBBB",
"BBBB",
NULL
};

char *shape_block_violett[] =
{
"VVVV",
"VVVV",
"VVVV",
"VVVV",
NULL
};

char *shape_block_white[] =
{
"WWWW",
"WWWW",
"WWWW",
"WWWW",
NULL
};

char *shape_placeholder[] =
{
"RRRR",
"RRRR",
"RRRR",
"RRRR",
"OOOO",
"OOOO",
"OOOO",
"OOOO",
"YYYY",
"YYYY",
"YYYY",
"YYYY",
"GGGG",
"GGGG",
"GGGG",
"GGGG",
"CCCC",
"CCCC",
"CCCC",
"CCCC",
"BBBB",
"BBBB",
"BBBB",
"BBBB",
"VVVV",
"VVVV",
"VVVV",
"VVVV",
NULL
};

enum
{
  SH_ALIEN1R=0,  SH_ALIEN1U, SH_ALIEN1L, SH_ALIEN1D,
  SH_ALIEN2R=4,  SH_ALIEN2U, SH_ALIEN2L, SH_ALIEN2D,
  SH_ALIEN3R=8,  SH_ALIEN3U, SH_ALIEN3L, SH_ALIEN3D,
  SH_ALIEN4R=12, SH_ALIEN4U, SH_ALIEN4L, SH_ALIEN4D,
  SH_ALIEN_RADIATE1=16, SH_ALIEN_RADIATE3, SH_ALIEN_RADIATE5, SH_ALIEN_RADIATE7, SH_ALIEN_RADIATE9 ,SH_ALIEN_RADIATE11,
  SH_MISSILE0=22, SH_MISSILE1, SH_MISSILE2, SH_MISSILE3,
  SH_SHIP1R=26, SH_SHIP1U, SH_SHIP1L, SH_SHIP1D, SH_SHIP2,
  SH_BLOCK_RED=31, SH_BLOCK_ORANGE, SH_BLOCK_YELLOW, SH_BLOCK_GREEN, SH_BLOCK_CYAN, SH_BLOCK_BLUE, SH_BLOCK_VIOLETT, SH_BLOCK_WHITE,
  SH_PLACEHOLDER,
  SH_MAX
};

struct shape Shape[] =
{
  [SH_ALIEN1R] = { std_colors, shape_alien1r },
  [SH_ALIEN1U] = { std_colors, shape_alien1u },
  [SH_ALIEN1L] = { std_colors, shape_alien1l },
  [SH_ALIEN1D] = { std_colors, shape_alien1d },

  [SH_ALIEN2R] = { std_colors, shape_alien2r },
  [SH_ALIEN2U] = { std_colors, shape_alien2u },
  [SH_ALIEN2L] = { std_colors, shape_alien2l },
  [SH_ALIEN2D] = { std_colors, shape_alien2d },

  [SH_ALIEN3R] = { std_colors, shape_alien3r },
  [SH_ALIEN3U] = { std_colors, shape_alien3u },
  [SH_ALIEN3L] = { std_colors, shape_alien3l },
  [SH_ALIEN3D] = { std_colors, shape_alien3d },

  [SH_ALIEN4R] = { std_colors, shape_alien4r },
  [SH_ALIEN4U] = { std_colors, shape_alien4u },
  [SH_ALIEN4L] = { std_colors, shape_alien4l },
  [SH_ALIEN4D] = { std_colors, shape_alien4d },

  [SH_ALIEN_RADIATE1]  = { std_colors, shape_alien_radiate1 },
  [SH_ALIEN_RADIATE3]  = { std_colors, shape_alien_radiate3 },
  [SH_ALIEN_RADIATE5]  = { std_colors, shape_alien_radiate5 },
  [SH_ALIEN_RADIATE7]  = { std_colors, shape_alien_radiate7 },
  [SH_ALIEN_RADIATE9]  = { std_colors, shape_alien_radiate9 },
  [SH_ALIEN_RADIATE11] = { std_colors, shape_alien_radiate11 },

  [SH_MISSILE0] = { std_colors, shape_missile0 },
  [SH_MISSILE1] = { std_colors, shape_missile1 },
  [SH_MISSILE2] = { std_colors, shape_missile2 },
  [SH_MISSILE3] = { std_colors, shape_missile3 },

  [SH_SHIP1R] = { std_colors, shape_ship1r },
  [SH_SHIP1U] = { std_colors, shape_ship1u },
  [SH_SHIP1L] = { std_colors, shape_ship1l },
  [SH_SHIP1D] = { std_colors, shape_ship1d },
  [SH_SHIP2]  = { std_colors, shape_ship2u },

  [SH_BLOCK_RED] = { std_colors, shape_block_red },
  [SH_BLOCK_ORANGE] = { std_colors, shape_block_orange },
  [SH_BLOCK_YELLOW] = { std_colors, shape_block_yellow },
  [SH_BLOCK_GREEN] = { std_colors, shape_block_green },
  [SH_BLOCK_CYAN] = { std_colors, shape_block_cyan },
  [SH_BLOCK_BLUE] = { std_colors, shape_block_blue },
  [SH_BLOCK_VIOLETT] = { std_colors, shape_block_violett },
  [SH_BLOCK_WHITE] = { std_colors, shape_block_white },
 
  [SH_PLACEHOLDER] = { std_colors, shape_placeholder },

//  [99] = { NULL, NULL }
};

