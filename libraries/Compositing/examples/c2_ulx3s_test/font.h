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

const char *shape_space[] =
{/*
 01234567890123456789012345678901 */
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
NULL
};

const char *shape_exclamation[] =
{/*
 01234567890123456789012345678901 */
"                ",
"       WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"                ",
"                ",
"       WW       ",
"       WW       ",
"                ",
NULL
};

const char *shape_double_quote[] =
{/*
 01234567890123456789012345678901 */
"                ",
"     WW  WW     ",
"     WW  WW     ",
"     WW  WW     ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
NULL
};

const char *shape_number[] =
{/*
 01234567890123456789012345678901 */
"                ",
"    WW    WW    ",
"    WW    WW    ",
"    WW    WW    ",
" WWWWWWWWWWWWWW ",
" WWWWWWWWWWWWWW ",
"    WW    WW    ",
"    WW    WW    ",
"    WW    WW    ",
"    WW    WW    ",
" WWWWWWWWWWWWWW ",
" WWWWWWWWWWWWWW ",
"    WW    WW    ",
"    WW    WW    ",
"    WW    WW    ",
"                ",
NULL
};

const char *shape_dollar[] =
{/*
 01234567890123456789012345678901 */
"                ",
"       WW       ",
"       WW       ",
"    WWWWWWWWW   ",
"  WWWWWWWWWWWW  ",
"  WW   WW       ",
"  WW   WW       ",
"  WWWWWWWWWWW   ",
"    WWWWWWWWWW  ",
"       WW   WW  ",
"       WW   WW  ",
"  WWWWWWWWWWWW  ",
"   WWWWWWWWWW   ",
"       WW       ",
"       WW       ",
"                ",
NULL
};

const char *shape_percent[] =
{/*
 01234567890123456789012345678901 */
"                ",
"                ",
"   WWW       WW ",
"  WW WW     WW  ",
"  WW WW    WW   ",
"   WWW    WW    ",
"         WW     ",
"        WW      ",
"       WW       ",
"      WW        ",
"     WW    WWW  ",
"    WW    WW WW ",
"   WW     WW WW ",
"  WW       WWW  ",
"                ",
"                ",
NULL
};

const char *shape_single_quote[] =
{/*
 01234567890123456789012345678901 */
"                ",
"       WW       ",
"       WW       ",
"       WW       ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
NULL
};

const char *shape_left_parenthesis[] =
{/*
 01234567890123456789012345678901 */
"                ",
"          WW    ",
"         WW     ",
"        WW      ",
"       WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"        WW      ",
"         WW     ",
"          WW    ",
"                ",
NULL
};

const char *shape_right_parenthesis[] =
{/*
 01234567890123456789012345678901 */
"                ",
"    WW          ",
"     WW         ",
"      WW        ",
"       WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"      WW        ",
"     WW         ",
"    WW          ",
"                ",
NULL
};

const char *shape_asterisk[] =
{/*
 01234567890123456789012345678901 */
"                ",
"                ",
"                ",
"       WW       ",
"       WW       ",
" WWW   WW   WWW ",
"   WWW WW WWW   ",
"     WWWWWW     ",
"     WWWWWW     ",
"   WWW WW WWW   ",
" WWW   WW   WWW ",
"       WW       ",
"       WW       ",
"                ",
"                ",
"                ",
NULL
};

const char *shape_plus[] =
{/*
 01234567890123456789012345678901 */
"                ",
"                ",
"                ",
"       WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"  WWWWWWWWWWWW  ",
"  WWWWWWWWWWWW  ",
"       WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"                ",
"                ",
"                ",
NULL
};

const char *shape_comma[] =
{/*
 01234567890123456789012345678901 */
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"      WWW       ",
"      WWW       ",
"      WWW       ",
"       WW       ",
"      WW        ",
NULL
};

const char *shape_minus[] =
{/*
 01234567890123456789012345678901 */
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"  WWWWWWWWWWWW  ",
"  WWWWWWWWWWWW  ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
NULL
};

const char *shape_dot[] =
{/*
 01234567890123456789012345678901 */
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"      WWW       ",
"      WWW       ",
"      WWW       ",
"                ",
NULL
};

const char *shape_slash[] =
{/*
 01234567890123456789012345678901 */
"                ",
"                ",
"             WW ",
"            WW  ",
"           WW   ",
"          WW    ",
"         WW     ",
"        WW      ",
"       WW       ",
"      WW        ",
"     WW         ",
"    WW          ",
"   WW           ",
"  WW            ",
"                ",
"                ",
NULL
};

const char *shape_0[] =
{/*
 01234567890123456789012345678901 */
"                ",
"   WWWWWWWWWW   ",
"  WWWWWWWWWWWW  ",
" WW        WWWW ",
" WW       WWWWW ",
" WW      WWW WW ",
" WW     WWW  WW ",
" WW    WWW   WW ",
" WW   WWW    WW ",
" WW  WWW     WW ",
" WW WWW      WW ",
" WWWWW       WW ",
" WWWW        WW ",
"  WWWWWWWWWWWW  ",
"   WWWWWWWWWW   ",
"                ",
NULL
};

const char *shape_1[] =
{/*
 01234567890123456789012345678901 */
"                ",
"       WW       ",
"      WWW       ",
"     WWWW       ",
"    WWWWW       ",
"   WWW WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"    WWWWWWWW    ",
"    WWWWWWWW    ",
"                ",
NULL
};

const char *shape_2[] =
{/*
 01234567890123456789012345678901 */
"                ",
"    WWWWWWWW    ",
"   WWWWWWWWWW   ",
"  WW        WW  ",
" WW          WW ",
" WW          WW ",
"            WWW ",
"           WWW  ",
"         WWWW   ",
"       WWWW     ",
"     WWWW       ",
"   WWWW         ",
"  WWW           ",
" WWWWWWWWWWWWWW ",
" WWWWWWWWWWWWWW ",
"                ",
NULL
};

const char *shape_3[] =
{/*
 01234567890123456789012345678901 */
"                ",
"   WWWWWWWWWW   ",
"  WWWWWWWWWWWW  ",
"             WW ",
"             WW ",
"             WW ",
"            WW  ",
"     WWWWWWWW   ",
"     WWWWWWWW   ",
"            WW  ",
"             WW ",
"             WW ",
"             WW ",
" WWWWWWWWWWWWW  ",
"  WWWWWWWWWWW   ",
"                ",
NULL
};

const char *shape_4[] =
{/*
 01234567890123456789012345678901 */
"                ",
"         WWW    ",
"        WWWW    ",
"       WWWWW    ",
"      WWW WW    ",
"     WWW  WW    ",
"    WWW   WW    ",
"   WWW    WW    ",
"  WWW     WW    ",
" WWWWWWWWWWWWWW ",
" WWWWWWWWWWWWWW ",
"          WW    ",
"          WW    ",
"          WW    ",
"          WW    ",
"                ",
NULL
};

const char *shape_5[] =
{/*
 01234567890123456789012345678901 */
"                ",
" WWWWWWWWWWWWWW ",
" WWWWWWWWWWWWWW ",
" WW             ",
" WW             ",
" WW             ",
" WW             ",
" WWWWWWWWWWWW   ",
" WWWWWWWWWWWWW  ",
"             WW ",
"             WW ",
"             WW ",
"             WW ",
" WWWWWWWWWWWWW  ",
"  WWWWWWWWWWW   ",
"                ",
NULL
};

const char *shape_6[] =
{/*
 01234567890123456789012345678901 */
"                ",
"   WWWWWWWWWWW  ",
"  WWWWWWWWWWWWW ",
" WW             ",
" WW             ",
" WW             ",
" WW             ",
" WWWWWWWWWWWW   ",
" WWWWWWWWWWWWW  ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
"  WWWWWWWWWWWW  ",
"   WWWWWWWWWW   ",
"                ",
NULL
};

const char *shape_7[] =
{/*
 01234567890123456789012345678901 */
"                ",
" WWWWWWWWWWWWWW ",
" WWWWWWWWWWWWWW ",
" WW         WWW ",
" WW        WWW  ",
"          WWW   ",
"         WWW    ",
"        WWW     ",
"       WWW      ",
"      WWW       ",
"     WWW        ",
"    WWW         ",
"   WWW          ",
"  WWW           ",
"  WWW           ",
"                ",
NULL
};

const char *shape_8[] =
{/*
 01234567890123456789012345678901 */
"                ",
"   WWWWWWWWWW   ",
"  WWWWWWWWWWWW  ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
"  WWWWWWWWWWWW  ",
"  WWWWWWWWWWWW  ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
"  WWWWWWWWWWWW  ",
"   WWWWWWWWWW   ",
"                ",
NULL
};

const char *shape_9[] =
{/*
 01234567890123456789012345678901 */
"                ",
"   WWWWWWWWWW   ",
"  WWWWWWWWWWWW  ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
"  WWWWWWWWWWWWW ",
"  WWWWWWWWWWWWW ",
"             WW ",
"             WW ",
"             WW ",
"             WW ",
"  WWWWWWWWWWWW  ",
"   WWWWWWWWWW   ",
"                ",
NULL
};

const char *shape_colon[] =
{/*
 01234567890123456789012345678901 */
"                ",
"                ",
"                ",
"      WWW       ",
"      WWW       ",
"      WWW       ",
"                ",
"                ",
"                ",
"                ",
"      WWW       ",
"      WWW       ",
"      WWW       ",
"                ",
"                ",
"                ",
NULL
};

const char *shape_semicolon[] =
{/*
 01234567890123456789012345678901 */
"                ",
"                ",
"                ",
"      WWW       ",
"      WWW       ",
"      WWW       ",
"                ",
"                ",
"                ",
"                ",
"      WWW       ",
"      WWW       ",
"      WWW       ",
"       WW       ",
"      WW        ",
"                ",
NULL
};

const char *shape_less_than[] =
{/*
 01234567890123456789012345678901 */
"                ",
"                ",
"          WW    ",
"         WW     ",
"        WW      ",
"       WW       ",
"      WW        ",
"     WW         ",
"    WW          ",
"     WW         ",
"      WW        ",
"       WW       ",
"        WW      ",
"         WW     ",
"          WW    ",
"                ",
NULL
};

const char *shape_equal[] =
{/*
 01234567890123456789012345678901 */
"                ",
"                ",
"                ",
"                ",
"                ",
"  WWWWWWWWWWWW  ",
"  WWWWWWWWWWWW  ",
"                ",
"                ",
"  WWWWWWWWWWWW  ",
"  WWWWWWWWWWWW  ",
"                ",
"                ",
"                ",
"                ",
"                ",
NULL
};

const char *shape_greater_than[] =
{/*
 01234567890123456789012345678901 */
"                ",
"                ",
"    WW          ",
"     WW         ",
"      WW        ",
"       WW       ",
"        WW      ",
"         WW     ",
"          WW    ",
"         WW     ",
"        WW      ",
"       WW       ",
"      WW        ",
"     WW         ",
"    WW          ",
"                ",
NULL
};

const char *shape_question[] =
{/*
 01234567890123456789012345678901 */
"                ",
"  WWWWWWWWWWWW  ",
" WWWWWWWWWWWWWW ",
" WW          WW ",
" WW          WW ",
"            WW  ",
"           WW   ",
"          WW    ",
"         WW     ",
"        WW      ",
"       WW       ",
"       WW       ",
"                ",
"       WW       ",
"       WW       ",
"                ",
NULL
};

const char *shape_a[] =
{/*
 01234567890123456789012345678901 */
"                ",
"      WWWW      ",
"     WWWWWW     ",
"    WWW  WWW    ",
"   WWW    WWW   ",
"  WWW      WWW  ",
" WWW        WWW ",
" WW          WW ",
" WW          WW ",
" WWWWWWWWWWWWWW ",
" WWWWWWWWWWWWWW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
"                ",
NULL
};

const char *shape_b[] =
{/*
 01234567890123456789012345678901 */
"                ",
" WWWWWWWWWWWW   ",
" WWWWWWWWWWWWW  ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW         WW  ",
" WWWWWWWWWWWW   ",
" WWWWWWWWWWWW   ",
" WW         WW  ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WWWWWWWWWWWWW  ",
" WWWWWWWWWWWW   ",
"                ",
NULL
};

const char *shape_c[] =
{/*
 01234567890123456789012345678901 */
"                ",
"   WWWWWWWWWW   ",
"  WWWWWWWWWWWW  ",
" WW          WW ",
" WW          WW ",
" WW             ",
" WW             ",
" WW             ",
" WW             ",
" WW             ",
" WW             ",
" WW          WW ",
" WW          WW ",
"  WWWWWWWWWWWW  ",
"   WWWWWWWWWW   ",
"                ",
NULL
};

const char *shape_d[] =
{/*
 01234567890123456789012345678901 */
"                ",
" WWWWWWWWWWWW   ",
" WWWWWWWWWWWWW  ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WWWWWWWWWWWWW  ",
" WWWWWWWWWWWW   ",
"                ",
NULL
};

const char *shape_e[] =
{/*
 01234567890123456789012345678901 */
"                ",
" WWWWWWWWWWWWWW ",
" WWWWWWWWWWWWWW ",
" WW             ",
" WW             ",
" WW             ",
" WW             ",
" WWWWWWWWWWW    ",
" WWWWWWWWWWW    ",
" WW             ",
" WW             ",
" WW             ",
" WW             ",
" WWWWWWWWWWWWWW ",
" WWWWWWWWWWWWWW ",
"                ",
NULL
};

const char *shape_f[] =
{/*
 01234567890123456789012345678901 */
"                ",
" WWWWWWWWWWWWWW ",
" WWWWWWWWWWWWWW ",
" WW             ",
" WW             ",
" WW             ",
" WW             ",
" WWWWWWWWWWW    ",
" WWWWWWWWWWW    ",
" WW             ",
" WW             ",
" WW             ",
" WW             ",
" WW             ",
" WW             ",
"                ",
NULL
};

const char *shape_g[] =
{/*
 01234567890123456789012345678901 */
"                ",
"   WWWWWWWWWW   ",
"  WWWWWWWWWWWW  ",
" WW          WW ",
" WW          WW ",
" WW             ",
" WW             ",
" WW             ",
" WW      WWWWWW ",
" WW      WWWWWW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
"  WWWWWWWWWWWW  ",
"   WWWWWWWWWW   ",
"                ",
NULL
};

const char *shape_h[] =
{/*
 01234567890123456789012345678901 */
"                ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WWWWWWWWWWWWWW ",
" WWWWWWWWWWWWWW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
"                ",
NULL
};

const char *shape_i[] =
{/*
 01234567890123456789012345678901 */
"                ",
"    WWWWWWWW    ",
"    WWWWWWWW    ",
"       WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"    WWWWWWWW    ",
"    WWWWWWWW    ",
"                ",
NULL
};

const char *shape_j[] =
{/*
 01234567890123456789012345678901 */
"                ",
"    WWWWWWWWWWW ",
"    WWWWWWWWWWW ",
"             WW ",
"             WW ",
"             WW ",
"             WW ",
"             WW ",
"             WW ",
"             WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
"  WWWWWWWWWWWW  ",
"   WWWWWWWWWW   ",
"                ",
NULL
};

const char *shape_k[] =
{/*
 01234567890123456789012345678901 */
"                ",
" WW          WW ",
" WW         WWW ",
" WW        WWW  ",
" WW       WWW   ",
" WW      WWW    ",
" WW     WWW     ",
" WWWWWWWWW      ",
" WWWWWWWWW      ",
" WW     WWW     ",
" WW      WWW    ",
" WW       WWW   ",
" WW        WWW  ",
" WW         WWW ",
" WW          WW ",
"                ",
NULL
};

const char *shape_l[] =
{/*
 01234567890123456789012345678901 */
"                ",
" WW             ",
" WW             ",
" WW             ",
" WW             ",
" WW             ",
" WW             ",
" WW             ",
" WW             ",
" WW             ",
" WW             ",
" WW             ",
" WW             ",
" WWWWWWWWWWWWW  ",
" WWWWWWWWWWWWW  ",
"                ",
NULL
};

const char *shape_m[] =
{/*
 01234567890123456789012345678901 */
"                ",
" WW          WW ",
" WWW        WWW ",
" WWWW      WWWW ",
" WWWWW    WWwWW ",
" WW WWW  WWW WW ",
" WW  WWWWWW  WW ",
" WW   WWWW   WW ",
" WW    WW    WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
"                ",
NULL
};

const char *shape_n[] =
{/*
 01234567890123456789012345678901 */
"                ",
" WW          WW ",
" WWW         WW ",
" WWWW        WW ",
" WWWWW       WW ",
" WW WWW      WW ",
" WW  WWW     WW ",
" WW   WWW    WW ",
" WW    WWW   WW ",
" WW     WWW  WW ",
" WW      WWW WW ",
" WW       WWWWW ",
" WW        WWWW ",
" WW         WWW ",
" WW          WW ",
"                ",
NULL
};

const char *shape_o[] =
{/*
 01234567890123456789012345678901 */
"                ",
"   WWWWWWWWWW   ",
"  WWWWWWWWWWWW  ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
"  WWWWWWWWWWWW  ",
"   WWWWWWWWWW   ",
"                ",
NULL
};

const char *shape_p[] =
{/*
 01234567890123456789012345678901 */
"                ",
" WWWWWWWWWWWW   ",
" WWWWWWWWWWWWW  ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WWWWWWWWWWWWW  ",
" WWWWWWWWWWWW   ",
" WW             ",
" WW             ",
" WW             ",
" WW             ",
" WW             ",
" WW             ",
"                ",
NULL
};

const char *shape_q[] =
{/*
 01234567890123456789012345678901 */
"                ",
"   WWWWWWWWWW   ",
"  WWWWWWWWWWWW  ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW     WW   WW ",
" WW     WWW  WW ",
" WW      WWW WW ",
" WW       WWWW  ",
" WW        WWW  ",
"  WWWWWWWWWWWWW ",
"   WWWWWWWW  WW ",
"                ",
NULL
};

const char *shape_r[] =
{/*
 01234567890123456789012345678901 */
"                ",
" WWWWWWWWWWWW   ",
" WWWWWWWWWWWWW  ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WWWWWWWWWWWWW  ",
" WWWWWWWWWWWW   ",
" WW     WWW     ",
" WW      WWW    ",
" WW       WWW   ",
" WW        WWW  ",
" WW         WWW ",
" WW          WW ",
"                ",
NULL
};

const char *shape_s[] =
{/*
 01234567890123456789012345678901 */
"                ",
"   WWWWWWWWWWWW ",
"  WWWWWWWWWWWWW ",
" WW             ",
" WW             ",
" WW             ",
" WW             ",
"  WWWWWWWWWWW   ",
"   WWWWWWWWWWW  ",
"             WW ",
"             WW ",
"             WW ",
"             WW ",
" WWWWWWWWWWWWW  ",
" WWWWWWWWWWWW   ",
"                ",
NULL
};

const char *shape_t[] =
{/*
 01234567890123456789012345678901 */
"                ",
" WWWWWWWWWWWWWW ",
" WWWWWWWWWWWWWW ",
"       WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"                ",
NULL
};

const char *shape_u[] =
{/*
 01234567890123456789012345678901 */
"                ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
"  WWWWWWWWWWWW  ",
"   WWWWWWWWWW   ",
"                ",
NULL
};

const char *shape_v[] =
{/*
 01234567890123456789012345678901 */
"                ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WWW        WWW ",
"  WWW      WWW  ",
"   WWW    WWW   ",
"    WWW  WWW    ",
"     WWWWWW     ",
"      WWWW      ",
"       WW       ",
"                ",
NULL
};

const char *shape_w[] =
{/*
 01234567890123456789012345678901 */
"                ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW          WW ",
" WW    WW    WW ",
" WW   WWWW   WW ",
" WW  WWWWWW  WW ",
" WW WWW  WWW WW ",
" WWWWW    WWWWW ",
" WWWW      WWWW ",
" WWW        WWW ",
" WW          WW ",
"                ",
NULL
};

const char *shape_x[] =
{/*
 01234567890123456789012345678901 */
"                ",
" WW          WW ",
" WWW        WWW ",
"  WWW      WWW  ",
"   WWW    WWW   ",
"    WWW  WWW    ",
"     WWWWWW     ",
"      WWWW      ",
"      WWWW      ",
"     WWWWWW     ",
"    WWW  WWW    ",
"   WWW    WWW   ",
"  WWW      WWW  ",
" WWW        WWW ",
" WW          WW ",
"                ",
NULL
};

const char *shape_y[] =
{/*
 01234567890123456789012345678901 */
"                ",
" WW          WW ",
" WWW        WWW ",
"  WWW      WWW  ",
"   WWW    WWW   ",
"    WWW  WWW    ",
"     WWWWWW     ",
"      WWWW      ",
"       WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"       WW       ",
"                ",
NULL
};

const char *shape_z[] =
{/*
 01234567890123456789012345678901 */
"                ",
" WWWWWWWWWWWWWW ",
" WWWWWWWWWWWWWW ",
"           WWW  ",
"          WWW   ",
"         WWW    ",
"        WWW     ",
"       WWW      ",
"      WWW       ",
"     WWW        ",
"    WWW         ",
"   WWW          ",
"  WWW           ",
" WWWWWWWWWWWWWW ",
" WWWWWWWWWWWWWW ",
"                ",
NULL
};

const char *shape_left_bracket[] =
{/*
 01234567890123456789012345678901 */
"                ",
"    WWWWWWWW    ",
"    WWWWWWWW    ",
"    WW          ",
"    WW          ",
"    WW          ",
"    WW          ",
"    WW          ",
"    WW          ",
"    WW          ",
"    WW          ",
"    WW          ",
"    WW          ",
"    WWWWWWWW    ",
"    WWWWWWWW    ",
"               ",
NULL
};

const char *shape_backslash[] =
{/*
 01234567890123456789012345678901 */
"WW              ",
"WWW             ",
" WWW            ",
"  WWW           ",
"   WWW          ",
"    WWW         ",
"     WWW        ",
"      WWW       ",
"       WWW      ",
"        WWW     ",
"         WWW    ",
"          WWW   ",
"           WWW  ",
"            WWW ",
"             WWW",
"              WW",
NULL
};

const char *shape_right_bracket[] =
{/*
 01234567890123456789012345678901 */
"                ",
"    WWWWWWWW    ",
"    WWWWWWWW    ",
"          WW    ",
"          WW    ",
"          WW    ",
"          WW    ",
"          WW    ",
"          WW    ",
"          WW    ",
"          WW    ",
"          WW    ",
"          WW    ",
"    WWWWWWWW    ",
"    WWWWWWWW    ",
"                ",
NULL
};

const char *shape_caret[] =
{/*
 01234567890123456789012345678901 */
"                ",
"       W        ",
"      WWW       ",
"     WW WW      ",
"    WW   WW     ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
NULL
};

const char *shape_underscore[] =
{/*
 01234567890123456789012345678901 */
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"  WWWWWWWWWWWW  ",
"  WWWWWWWWWWWW  ",
"                ",
NULL
};

const struct shape Font[] =
{
   [0] = { std_colors, shape_space },
   [1] = { std_colors, shape_exclamation },
   [2] = { std_colors, shape_double_quote },
   [3] = { std_colors, shape_number },
   [4] = { std_colors, shape_dollar },
   [5] = { std_colors, shape_percent },
   [6] = { std_colors, shape_space }, // todo "&"
   [7] = { std_colors, shape_single_quote },
   [8] = { std_colors, shape_left_parenthesis },
   [9] = { std_colors, shape_right_parenthesis },
  [10] = { std_colors, shape_asterisk },
  [11] = { std_colors, shape_plus },
  [12] = { std_colors, shape_comma },
  [13] = { std_colors, shape_minus },
  [14] = { std_colors, shape_dot },
  [15] = { std_colors, shape_slash },
  [16] = { std_colors, shape_0 },
  [17] = { std_colors, shape_1 },
  [18] = { std_colors, shape_2 },
  [19] = { std_colors, shape_3 },
  [20] = { std_colors, shape_4 },
  [21] = { std_colors, shape_5 },
  [22] = { std_colors, shape_6 },
  [23] = { std_colors, shape_7 },
  [24] = { std_colors, shape_8 },
  [25] = { std_colors, shape_9 },
  [26] = { std_colors, shape_colon },
  [27] = { std_colors, shape_semicolon },
  [28] = { std_colors, shape_less_than },
  [29] = { std_colors, shape_equal },
  [30] = { std_colors, shape_greater_than },
  [31] = { std_colors, shape_question },
  [32] = { std_colors, shape_space }, // todo "@"
  [33] = { std_colors, shape_a },
  [34] = { std_colors, shape_b },
  [35] = { std_colors, shape_c },
  [36] = { std_colors, shape_d },
  [37] = { std_colors, shape_e },
  [38] = { std_colors, shape_f },
  [39] = { std_colors, shape_g },
  [40] = { std_colors, shape_h },
  [41] = { std_colors, shape_i },
  [42] = { std_colors, shape_j },
  [43] = { std_colors, shape_k },
  [44] = { std_colors, shape_l },
  [45] = { std_colors, shape_m },
  [46] = { std_colors, shape_n },
  [47] = { std_colors, shape_o },
  [48] = { std_colors, shape_p },
  [49] = { std_colors, shape_q },
  [50] = { std_colors, shape_r },
  [51] = { std_colors, shape_s },
  [52] = { std_colors, shape_t },
  [53] = { std_colors, shape_u },
  [54] = { std_colors, shape_v },
  [55] = { std_colors, shape_w },
  [56] = { std_colors, shape_x },
  [57] = { std_colors, shape_y },
  [58] = { std_colors, shape_z },
  [59] = { std_colors, shape_left_bracket },
  [60] = { std_colors, shape_backslash },
  [61] = { std_colors, shape_right_bracket },
  [62] = { std_colors, shape_caret },
  [63] = { std_colors, shape_underscore },
};

