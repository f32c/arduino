#include <stdlib.h>
#include "shape.h"

// ascii-art of the shapes
const struct charcolors std_colors[] = 
{ 
  {' ', 0}, // transparent/black
  {'.', RGB2PIXEL(0x080808)},
  {'X', RGB2PIXEL(0x0000BD)},
  {'o', RGB2PIXEL(0x7B6B9C)},
  {'O', RGB2PIXEL(0x8473A5)},
  {'+', RGB2PIXEL(0x947BAD)},
  {'@', RGB2PIXEL(0x5A84A5)},
  {'#', RGB2PIXEL(0x6B94B5)},
  {'$', RGB2PIXEL(0x739CBD)},
  {'Z', RGB2PIXEL(0x7BA5C5)},
  {'&', RGB2PIXEL(0x848C84)},
  {'*', RGB2PIXEL(0x8C8C8C)},
  {'=', RGB2PIXEL(0x848C94)},
  {'-', RGB2PIXEL(0x949494)},
  {';', RGB2PIXEL(0x9C9C9C)},
  {':', RGB2PIXEL(0x9C84B5)},
  {'>', RGB2PIXEL(0xA58CBD)},
  {',', RGB2PIXEL(0xA5A5A5)},
  {'<', RGB2PIXEL(0xADADAD)},
  {'1', RGB2PIXEL(0xB5B5B5)},
  {'2', RGB2PIXEL(0xBDBDBD)},
  {'3', RGB2PIXEL(0xAD94C5)},
  {'4', RGB2PIXEL(0xB59CCE)},
  {'5', RGB2PIXEL(0x84ADCE)},
  {'6', RGB2PIXEL(0x94B5D6)},
  {'7', RGB2PIXEL(0x9CBDDE)},
  {'8', RGB2PIXEL(0xBDA5D6)},
  {'9', RGB2PIXEL(0xC5ADDE)},
  {'0', RGB2PIXEL(0xCEB5E6)},
  {'q', RGB2PIXEL(0xD6BDEF)},
  {'w', RGB2PIXEL(0xA5C5E6)},
  {'e', RGB2PIXEL(0xADCEEF)},
  {'r', RGB2PIXEL(0xB5D6F7)},
  {'t', RGB2PIXEL(0xBDDEFF)},
  {'y', RGB2PIXEL(0xC5C5C5)},
  {'u', RGB2PIXEL(0xCECECE)},
  {'i', RGB2PIXEL(0xD6D6D6)},
  {'p', RGB2PIXEL(0xDEDEDE)},
  {'a', RGB2PIXEL(0xDEC5F7)},
  {'s', RGB2PIXEL(0xE6CEFF)},
  {'d', RGB2PIXEL(0xFFDEFF)},
  {'f', RGB2PIXEL(0xDEFFFF)},
  {'g', RGB2PIXEL(0xE6E6E6)},
  {'h', RGB2PIXEL(0xEFEFEF)},
  {'j', RGB2PIXEL(0xF7F7F7)},
  {'k', RGB2PIXEL(0xff)}, // white
  {0, RGB2PIXEL(0)}
};


const char *shape_space[] =
{/*
 0123456789012345*/
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
{
"    .XkkkkkkX.  ",
"    .XhhhhhXX.  ",
"    .XgggghX..  ",
"    .XpgppXX.   ",
"    .XiipiX..   ",
"    .XuiuXX.    ",
"    .XyyuX..    ",
"   ..XXXXX.     ",
"   .XX11XX.     ",
"   .X<1<1X.     ",
"   .X,<,<X.     ",
"   .XX,;XX.     ",
"   ..XXXX..     ",
"    ......      ",
"                ",
"                ",
NULL
};

const char *shape_double_quote[] =
{
" .XkkkX..XkkkX. ",
" .XhhhX..XhhhX. ",
" .XXggX..XggXX. ",
" ..XXpX..XpXX.. ",
"  ..XXX..XXX..  ",
"   ..........   ",
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

const char *shape_hash[] =
{
" .XkkkX.XkkkX...",
" XXhhhXXXhhhXXX.",
" gggggggggggggX.",
" ppgppgppgppgpX.",
" XXipiXXXipiXXX.",
" XXuuiXXXuuiXXX.",
" yyuyyyyyuyyyyX.",
" 2y2y2y2y2y2y2X.",
" XX121XXX121XXX.",
" .X<1<X.X<1<X...",
" .XXXXX.XXXXX.  ",
" .............  ",
"                ",
"                ",
"                ",
"                ",
NULL
};

const char *shape_dollar[] =
{
".XXXXXkkkXXXXX..",
"XXhhhhhhhhhhhXX.",
"XggggggghgggggX.",
"XppgXXpgpXXpgpX.",
"XipiXXiiiXXXXXX.",
"XuuuuuiuiuuuuXX.",
"XXyuyuyyuyuyuyX.",
"XXXXXX2y2XX2y2X.",
"X111XX121XX121X.",
"X1<1<1<1<1<1<1X.",
"XX,<,<,<,<,<,XX.",
".XXXXX,;,XXXXX..",
".....XXXXX..... ",
"    .......     ",
"                ",
"                ",
NULL
};

const char *shape_percent[] =
{
"XXkkXX...XXXXXX.",
"XhhhhX..XXhhhhX.",
"XggggX.XXggggXX.",
"XXpgXXXXppgpXX..",
".XXXXXXiipiXX.. ",
"....XXuuiuXX..  ",
" ..XXyyuyXX.... ",
"..XX2y2yXXXXXX..",
".XX1212XXXX11XX.",
"XX<1<1XX.X<1<1X.",
"X,<,<XX..X,<,<X.",
"XXXXXX...XX;,XX.",
"....... ..XXXX..",
"         ...... ",
"                ",
"                ",
NULL
};

const char *shape_ampersand[] =
{
".XXkkkkkXX..    ",
"XXhhhhhhhXX.    ",
"XgggXXXgggX.....",
"XppgXXXppgXXXXX.",
"XXiiiXiipXXiiiX.",
".XXuiuuiXXuuiuX.",
"XXyyyuyyXyyuXXX.",
"X2y2yX2y2y2XX...",
"X121XXX1212XX...",
"X<1<XXX<1<1<XXX.",
"X,<,<,,<,X,<,,X.",
"XX;,;,;,XXX;,,X.",
".XXXXXXXX.XXXXX.",
"................",
"                ",
"                ",
NULL
};

const char *shape_tick[] =
{
"     .XkkkkX.   ",
"     .XhhhhX.   ",
"     .XgggXX.   ",
"    ..XpgXX..   ",
"    .XXiXX..    ",
"    .XuXX..     ",
"    .XXX..      ",
"    .....       ",
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
{
"   ..XXXkkkkkX. ",
"  ..XXhhhhXXXX. ",
"  .XXgggXXX.... ",
"  .XppgXX...    ",
"  .XipiX..      ",
"  .XuuuX.       ",
"  .XyuyX.       ",
"  .X2y2X..      ",
"  .X121XX...    ",
"  .XX<1<XXX.... ",
"  ..XX,<,<XXXX. ",
"   ..XXX,;,;,X. ",
"    ...XXXXXXX. ",
"      ......... ",
"                ",
"                ",
NULL
};

const char *shape_right_parenthesis[] =
{
" .XkkkkkXXX..   ",
" .XXXXhhhhXX..  ",
" ....XXXgggXX.  ",
"    ...XXpppX.  ",
"      ..XipiX.  ",
"       .XuiuX.  ",
"       .XyyyX.  ",
"      ..X2y2X.  ",
"    ...XX121X.  ",
" ....XXX<1<XX.  ",
" .XXXX,,<,XX..  ",
" .X;,;,,XXX..   ",
" .XXXXXXX...    ",
" .........      ",
"                ",
"                ",
NULL
};

const char *shape_asterisk[] =
{
" ....XkkkX....  ",
" .XXXXhhhXXXX.  ",
" .XggXgggXggX.  ",
"..XpppgpppppX...",
"XXXXipiipipXXXX.",
"XuuuiuiuuiuuuuX.",
"XyuyyyuyuyyuyuX.",
"XXXX2y2y2y2XXXX.",
"..X121212121X...",
" .X<1X<1<X<1X.  ",
" .XXXX,<,XXXX.  ",
" ....X,;,X....  ",
"    .XXXXX.     ",
"    .......     ",
"                ",
"                ",
NULL
};

const char *shape_plus[] =
{
"    .XXXXX.     ",
"    .XhhhX.     ",
"    .XgggX.     ",
".....XpgpX......",
"XXXXXXiiiXXXXXX.",
"XuuuuuiuiuuuuuX.",
"XyuyuyyuyyuyuyX.",
"XXXXXX2y2XXXXXX.",
".....X121X......",
"    .X1<1X.     ",
"    .X,<,X.     ",
"    .XXXXX.     ",
"    .......     ",
"                ",
"                ",
"                ",
NULL
};

const char *shape_comma[] =
{
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"     ........   ",
"     .XXXXXX.   ",
"     .X1112X.   ",
"     .X11<<X.   ",
"     .X,,<XX.   ",
"    ..X,,XX..   ",
"    .XX-XX..    ",
"    .X-XX..     ",
"    .XXX..      ",
"    .....       ",
NULL
};

const char *shape_minus[] =
{
"                ",
"                ",
"                ",
" .............. ",
" .XXXXXXXXXXXX. ",
" .XuuuuuuuuuuX. ",
" .XXXXXXXXXXXX. ",
" .............. ",
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

const char *shape_period[] =
{
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"     ......     ",
"    ..XXXX..    ",
"    .XX11XX.    ",
"    .X<1<1X.    ",
"    .X,<,<X.    ",
"    .XX,;XX.    ",
"    ..XXXX..    ",
"     ......     ",
"                ",
"                ",
NULL
};

const char *shape_slash[] =
{
"       ..XXXXXX.",
"      ..XXhhhhX.",
"     ..XXggggXX.",
"    ..XXppgpXX..",
"   ..XXipiiXX.. ",
"  ..XXuuuiXX..  ",
" ..XXyuyuXX..   ",
"..XX2y2yXX..    ",
".XX1212XX..     ",
"XX<1<1XX..      ",
"X,<,<XX..       ",
"XXXXXX..        ",
".......         ",
"                ",
"                ",
"                ",
NULL
};

const char *shape_0[] =
{
"                ",
".XXkkkkkkkkkkXX.",
"XXhhhhhhhhhhhhX.",
"XggggXXXXXXgggX.",
"XppgXX....XpgpX.",
"XipiXXXXXXXiiiX.",
"XuuiXXuuuXXuiuX.",
"XyuyXXyyuXXyyuX.",
"X2y2XXXXXXX2y2X.",
"X121X....XX112X.",
"X<1<XXXXXX<1<1X.",
"X,<,<,,,,<,<,XX.",
"XX;,;,,;,;,;XX..",
".XXXXXXXXXXXX.. ",
"..............  ",
"                ",
NULL
};

const char *shape_1[] =
{
"                ",
"     ..XXkX.    ",
"    ..XXhhX.    ",
"   ..XXgggX.    ",
"  ..XXppgpX.    ",
"  .XXipiiiX.    ",
"  .XuuuiuiX.    ",
"  .XXXXyuyX.    ",
"  ....X2y2X.    ",
"  ....X211X.... ",
"  .XXXX<1<XXXX. ",
"  .X,,<,<,<,<X. ",
"  .X,,;,;,;,;X. ",
"  .XXXXXXXXXXX. ",
"  ............. ",
"                ",
NULL
};

const char *shape_2[] =
{
"                ",
"XXkkkkkkkkkkkXX.",
"XhhhhhhhhhhhhhX.",
"XgggXXXXXXXgggX.",
"XppgX....XXpgpX.",
"XXXXX..XXXiiiXX.",
".....XXXuuiuXX..",
"...XXXyyyuXXX.. ",
".XXX22y2XXX.....",
"XX1212XXX.XXXXX.",
"X<1<XXXXXXX<<<X.",
"X,<,<,,,,,,<<,X.",
"X,;,;,,,;,,;,;X.",
"XXXXXXXXXXXXXXX.",
"................",
"                ",
NULL
};

const char *shape_3[] =
{
"                ",
"XkkkkkkkkkkkkkX.",
"XhhhhhhhhhhhhhX.",
"XgggXXXXXXgggXX.",
"XpgpX..XXppgXX..",
"XXXXX.XXiipXXX..",
"......XuuiuuuXX.",
"......XXXXXyuyX.",
"XXXXX.....X2y2X.",
"X121X.....X121X.",
"X<1<XXXXXXX<1<X.",
"X,<,<,,<,,<,<,X.",
"XX;,;,;,;,;,;XX.",
".XXXXXXXXXXXXX..",
"............... ",
"                ",
NULL
};

const char *shape_4[] =
{
"                ",
"     ...XXXkkkX.",
"   ...XXXhhhhhX.",
" ...XXXgggggXXX.",
"..XXXpppgpXXXXX.",
".XXiipipXXXiiiX.",
"XXuuiuXXXXXuuiX.",
"XyyuyyyyyyyuyyX.",
"X2y2y2y2y2y2y2X.",
"XXXXXXXXXXX121X.",
"..........X<1<X.",
"         .X,<,X.",
"         .X,;,X.",
"         .XXXXX.",
"         .......",
"                ",
NULL
};

const char *shape_5[] =
{
"                ",
"XkkkkkkkkkkkkkX.",
"XhhhhhhhhhhhhhX.",
"XggXXXXXXXXXXXX.",
"XppXXXXXXXXXXX..",
"XipiiiiiiiiiiXX.",
"XuuiuiuiuiuiuuX.",
"XyuyXXXXXXXyuyX.",
"XXXXX.....X22yX.",
"XXXXX.....X211X.",
"X<<<XXXXXXX<1<X.",
"X<,<,,<,,,<,<,X.",
"XX;,,;,;,,;,;XX.",
".XXXXXXXXXXXXX..",
"............... ",
"                ",
NULL
};

const char *shape_6[] =
{
"                ",
" ...XXXkkkkkkX. ",
"..XXXhhhhhhhhX. ",
".XXgggggXXXXXX. ",
"XXppgpXXXXXXXX..",
"XiipXXXiiiiiiXX.",
"XuiuX.XuiuiuuuX.",
"XyyyX.XXXXXyuyX.",
"X2y2X.....X2y2X.",
"X212X.....X121X.",
"X<1<XXXXXXX<1<X.",
"X,<,<,,<,,<,<,X.",
"XX;,;,;,;,;,;XX.",
".XXXXXXXXXXXXX..",
"............... ",
"                ",
NULL
};

const char *shape_7[] =
{
"                ",
"XkkkkkkkkkkkkkX.",
"XhhhhhhhhhhhhhX.",
"XXXXXXXXXXggggX.",
".......XXppgpXX.",
"  .XXXXXiipiXX..",
"  .XuuuuiuuiuX. ",
"  .XyuyuyyuyyX. ",
" ..XX2y2yXXXXX. ",
"..XX1212XX..... ",
".XX<1<1XX..     ",
"XX,<,<XX..      ",
"X;,;,XX..       ",
"XXXXXX..        ",
".......         ",
"                ",
NULL
};

const char *shape_8[] =
{  
"                ",
"XXkkkkkkkkkkkkX.",
"XhhhhhhhhhhhhhX.",
"XgggXXXXXXXXXXX.",
"XpgpX....XppppX.",
"XiipXXXXXXXipiX.",
"XXuuuuuuuuuiuXX.",
"XyuyuyuyuyuyyyX.",
"X2y2XXXXXXX2y2X.",
"X121X.....X121X.",
"X<1<XXXXXXX<1<X.",
"X,<,<,,,,,<,<,X.",
"XX;,;,,,,;,;,XX.",
".XXXXXXXXXXXXX..",
"............... ",
"                ",
NULL
};

const char *shape_9[] =
{
"                ",
"XXkkkkkkkkkkkXX.",
"XhhhhhhhhhhhhhX.",
"XgggXXXXXXXgggX.",
"XpgpX.....XpgpX.",
"XiipX.....XiiiX.",
"XuiuXXXXX.XuiuX.",
"XyyuyyyyX.XyyuX.",
"XX2y2y2yXXX2y2X.",
".XXXXXXXX1121XX.",
".XXXXXX<<1<1XX..",
".X,,,,,<,<XXX.. ",
".X,,;,,;XXX...  ",
".XXXXXXXX...    ",
"..........      ",
"                ",
NULL
};

const char *shape_colon[] =
{
"    ..XXXX..    ",
"    .XXhhXX.    ",
"    .XggggX.    ",
"    .XppgpX.    ",
"    .XXiiXX.    ",
"    ..XXXX..    ",
"    ..XXXX..    ",
"    .XX22XX.    ",
"    .X1212X.    ",
"    .X<1<1X.    ",
"    .XX,<XX.    ",
"    ..XXXX..    ",
"     ......     ",
"                ",
"                ",
"                ",
NULL
};

const char *shape_semicolon[] =
{
"     ..XXXX..   ",
"     .XXhhXX.   ",
"     .XggggX.   ",
"     .XppgpX.   ",
"     .XXiiXX.   ",
"     ..XXXX..   ",
"     .XXXXXX.   ",
"     .X2222X.   ",
"     .X2121X.   ",
"     .X<1<1X.   ",
"     .X,<,XX.   ",
"    ..X,;XX..   ",
"    .XX-XX..    ",
"    .X-XX..     ",
"    .XXX..      ",
"    .....       ",
NULL
};

const char *shape_less[] =
{
"     ...XXXkkkX.",
"   ...XXXhhhhXX.",
" ...XXXggggXXX..",
"..XXXpppgXXX... ",
"XXXiipiXXX...   ",
"XuuiuXXX...     ",
"XXXyyyyXXX...   ",
"..XXXy2y2XXX... ",
" ...XXX1211XXX..",
"   ...XXX<1<1XX.",
"     ...XXX,<,X.",
"       ...XXXXX.",
"         .......",
"                ",
"                ",
"                ",
NULL
};

const char *shape_equal[] =
{
"                ",
" .............. ",
" .XXXXXXXXXXXX. ",
" .XppppppppppX. ",
" .XipipipipipX. ",
" .XXXXXXXXXXXX. ",
" .XXXXXXXXXXXX. ",
" .X2222222222X. ",
" .X2122122121X. ",
" .XXXXXXXXXXXX. ",
" .............. ",
"                ",
"                ",
"                ",
"                ",
"                ",
NULL
};

const char *shape_greater[] =
{
"XkkkXXX...      ",
"XXhhhhXXX...    ",
".XXXggggXXX...  ",
"...XXXppppXXX...",
"  ...XXXipiiXXX.",
"    ...XXXuuuuX.",
"  ...XXXyyuyXXX.",
"...XXX22y2XXX...",
".XXX1212XXX...  ",
"XX<1<1XXX...    ",
"X,<,XXX...      ",
"XXXXX...        ",
"......          ",
"                ",
"                ",
"                ",
NULL
};

const char *shape_question[] =
{
"XXkkkkkkkkkkkXX.",
"XhhhhhhhhhhhhhX.",
"XgggXXXXXXXgggX.",
"XppgX.XXXpppgXX.",
"XXXXXXXiiipXXX..",
"....XXuuiXXX... ",
"   .XyyuyX...   ",
"   .XXXXXX.     ",
"   .XX11XX.     ",
"   .X<1<<X.     ",
"   .X,<,<X.     ",
"   .XX,;XX.     ",
"   ..XXXX..     ",
"    ......      ",
"                ",
"                ",
NULL
};

const char *shape_at[] =
{
"XXkkkkkkkkkkkXX.",
"XhhhhhhhhhhhhhXX",
"ggggXXXXXXXggggX",
"pgpXXpppppXXpgpX",
"piiXipipipiXiipX",
"uiuXuuiXuuuXuiuX",
"uyyXyuyXyuyXyyyX",
"2y2Xy22y2y2y2y2X",
"212XX2121X2121XX",
"1<1<XXXXXXXXXXX.",
"X,<,,<,,<,,,<X..",
"XX;,,;,;,;,,;X. ",
".XXXXXXXXXXXXX. ",
"............... ",
"                ",
"                ",
NULL
};

const char *shape_A[] =
{
"XXkkkkkkkkkkkkX.",
"XhhhhhhhhhhhhhX.",
"XgggXXXXXXXXXXX.",
"XpgpX....XppppX.",
"XiipX.  .XXipiX.",
"XuuuX.....XuuiX.",
"XyuyXXXXXXXyuyX.",
"X2y2XX2222y2y2X.",
"X121XX12212112X.",
"X<1<XXXXXXX1<1X.",
"X,<,X.....X,<,X.",
"X,;,X.   .X,;,X.",
"XXXXX.   .XXXXX.",
"......   .......",
"                ",
"                ",
NULL
};

const char *shape_B[] =
{
"XkkkkkkkkkkkkXX.",
"XhhhhhhhhhhhhhX.",
"XgggXXXXXXXgggX.",
"XpgpX.....XpgpX.",
"XiipXXXXXXXiiiX.",
"XuiuXXuuuuuiuXX.",
"XyyuXXyyuyuyyXX.",
"X2y2XXXXXXX2y2X.",
"X212X.....X121X.",
"X<1<XXXXXXX<1<X.",
"X,<,XX,,,,<,<,X.",
"X,;,XX;,,;,;,XX.",
"XXXXXXXXXXXXXX..",
"............... ",
"                ",
"                ",
NULL
};

const char *shape_C[] =
{
"XXkkkkkkkkkkkXX.",
"XhhhhhhhhhhhhhX.",
"XgggXXXXXXXgggX.",
"XpgpX.....XXXXX.",
"XiipX.   .......",
"XuiuX.          ",
"XyyuX.          ",
"X2y2X.   .......",
"X212X.....XXXXX.",
"X<1<XXXXXXX<<<X.",
"X,<,<,<,,,<,<,X.",
"XX;,;,;,,;,;,XX.",
".XXXXXXXXXXXXX..",
"............... ",
"                ",
"                ",
NULL
};

const char *shape_D[] =
{
"XkkkkkkkkkkkXX..",
"XhhhhhhhhhhhhXX.",
"XgggXXXXXXggggX.",
"XpgpX....XXpgpX.",
"XiipX.  ..XiipX.",
"XuiuX.   .XuiuX.",
"XyyyX.   .XyyyX.",
"Xy2yX.  ..X2y2X.",
"X121X....XX121X.",
"X<1<XXXXXX<1<1X.",
"X,<,XX,,<,<,<XX.",
"X,;,XX,,;,;,XX..",
"XXXXXXXXXXXXX.. ",
"..............  ",
"                ",
"                ",
NULL
};

const char *shape_E[] =
{
"XkkkkkkkkkkkkkX.",
"XhhhhhhhhhhhhhX.",
"XgggXXXXXXXXXXX.",
"XpgpX...........",
"XiipXXXXX.      ",
"XuiuuuuuX.      ",
"XyyuyuyuX.      ",
"X2y2XXXXX.      ",
"X212X...........",
"X<1<XXXXXXXXXXX.",
"X,<,<,,,<,,,,,X.",
"X,;,;,,;,;,,,;X.",
"XXXXXXXXXXXXXXX.",
"................",
"                ",
"                ",
NULL
};

const char *shape_F[] =
{
"XkkkkkkkkkkkkkX.",
"XhhhhhhhhhhhhhX.",
"XgggXXXXXXXXXXX.",
"XpgpX...........",
"XiipXXXXX.      ",
"XuiuuuuuX.      ",
"XyyuyuyuX.      ",
"X2y2XXXXX.      ",
"X121X.....      ",
"X<1<X.          ",
"X,<,X.          ",
"X,;,X.          ",
"XXXXX.          ",
"......          ",
"                ",
"                ",
NULL
};

const char *shape_G[] =
{
"XXkkkkkkkkkkkXX.",
"XhhhhhhhhhhhhhX.",
"XgggXXXXXXXgggX.",
"XpgpX.....XXXXX.",
"XiipX...........",
"XuiuX..XXXXXXXX.",
"XyyuX..XyyyyyyX.",
"X2y2X..X2y2y2yX.",
"X121X..XXXX121X.",
"X<1<XXXXXXX<1<X.",
"X,<,<,,,,,<,<,X.",
"XX;,;,,,;,;,;,X.",
".XXXXXXXXXXXXXX.",
"................",
"                ",
"                ",
NULL
};

const char *shape_H[] =
{
"XkkkX.   .XkkkX.",
"XhhhX.   .XhhhX.",
"XgggX.   .XgggX.",
"XpgpX.....XpgpX.",
"XiiiXXXXXXXiiiX.",
"XuiuXuuuuuuiuiX.",
"XyyuXyuyuyuyyyX.",
"Xy22XXXXXXX2y2X.",
"X121X.....X121X.",
"X<1<X.   .X1<1X.",
"X,<,X.   .X,<,X.",
"X,;,X.   .X,;,X.",
"XXXXX.   .XXXXX.",
"......   .......",
"                ",
"                ",
NULL
};

const char *shape_I[] =
{
" .XkkkkkkkkkX.  ",
" .XhhhhhhhhhX.  ",
" .XXXXgggXXXX.  ",
" ....XpgpX....  ",
"    .XiipX.     ",
"    .XuiuX.     ",
"    .XyyuX.     ",
"    .Xy22X.     ",
" ....X121X....  ",
" .XXXX<1<XXXX.  ",
" .X,,<,<,<,<X.  ",
" .X,;,;,,;,;X.  ",
" .XXXXXXXXXXX.  ",
" .............  ",
"                ",
"                ",
NULL
};

const char *shape_J[] =
{
"      .XkkkkkkX.",
"      .XhhhhhhX.",
"      .XXXXgggX.",
"      ....XpgpX.",
"         .XiiiX.",
"         .XuiuX.",
"         .XyyuX.",
"......   .Xy22X.",
"XXXXX.....X121X.",
"X<<<XXXXXXX<1<X.",
"X<,<,,,,,,<,<,X.",
"XX,;,,,,,;,;,XX.",
".XXXXXXXXXXXXX..",
"............... ",
"                ",
"                ",
NULL
};

const char *shape_K[] =
{
"XkkkkX...XXkkkX.",
"XhhhXX..XXhhhhX.",
"XgggX..XXgggXXX.",
"XpgpX.XXppgXX...",
"XiiiXXXiipXX..  ",
"XuiuXXuuiXX..   ",
"XyyuXXyuyXX..   ",
"Xy22XXX222XX..  ",
"X121X.XX212XX...",
"X<1<X..XX<1<XXX.",
"X,<,X...XX,<,,X.",
"X,;,X. ..XX;,,X.",
"XXXXX.  ..XXXXX.",
"......   .......",
"                ",
"                ",
NULL
};

const char *shape_L[] =
{
"XkkkkX.         ",
"XhhhXX.         ",
"XgggX..         ",
"XpgpX.          ",
"XiiiX.          ",
"XuiuX.          ",
"XyyuX.          ",
"X2y2X.   .......",
"X121X.....XXXXX.",
"X<1<XXXXXXX<<<X.",
"X,<,<,<,,,<,<,X.",
"X,;,;,;,,;,;,,X.",
"XXXXXXXXXXXXXXX.",
"................",
"                ",
"                ",
NULL
};

const char *shape_M[] =
{
"XkkkXX...XXkkkX.",
"XhhhhXX.XXhhhhX.",
"XgggggXXXgggggX.",
"XpgppgpXppgppgX.",
"XiipiipiipiipiX.",
"XuiuXuuiuuXuiuX.",
"XyyuXXyyuXXyyuX.",
"X2y2XXX2XXX2y2X.",
"X121X.XXX.X121X.",
"X<1<X.....X<1<X.",
"X,<,X.   .X<,<X.",
"X,;,X.   .X;,;X.",
"XXXXX.   .XXXXX.",
"......   .......",
"                ",
"                ",
NULL
};

const char *shape_N[] =
{
"XkkkXX.. .XkkkX.",
"XhhhhXX...XhhhX.",
"XgggggXX..XgggX.",
"XpgppgpXX.XpgpX.",
"XiipiipiXXXiiiX.",
"XuiuXuuiuXXuiuX.",
"XyyuXXyyyyXyyuX.",
"X2y2XXXy2y2y22X.",
"X212X.XX121212X.",
"X<1<X..XX<1<1<X.",
"X,<,X...XX,<,<X.",
"X,;,X. ..XX,;,X.",
"XXXXX.  ..XXXXX.",
"......   .......",
"                ",
"                ",
NULL
};

const char *shape_O[] =
{
"XXkkkkkkkkkkkXX.",
"XhhhhhhhhhhhhhX.",
"XgggXXXXXXXgggX.",
"XpgpX.....XpgpX.",
"XiipX.   .XiiiX.",
"XuiuX.   .XuiuX.",
"XyyuX.   .XyyuX.",
"X2y2X.   .Xy22X.",
"X212X.....X121X.",
"X<1<XXXXXXX<1<X.",
"X,<,<,<,,,<,<,X.",
"XX;,;,;,,;,;,XX.",
".XXXXXXXXXXXXX..",
"............... ",
"                ",
"                ",
NULL
};

const char *shape_P[] =
{
"XkkkkkkkkkkkkXX.",
"XhhhhhhhhhhhhhX.",
"XgggXXXXXXXgggX.",
"XpgpX.....XpgpX.",
"XiipX.   .XiiiX.",
"XuiuX.....XuiuX.",
"XyyuXXXXXXXyyuX.",
"X2y2XX2222y2y2X.",
"X121XX1221212XX.",
"X<1<XXXXXXXXXX..",
"X,<,X.......... ",
"X,;,X.          ",
"XXXXX.          ",
"......          ",
"                ",
"                ",
NULL
};

const char *shape_Q[] =
{
"XXkkkkkkkkkkkXX.",
"XhhhhhhhhhhhhhX.",
"XgggXXXXXXXgggX.",
"XpgpX.....XpgpX.",
"XiipX.   .XiiiX.",
"XuiuX.   .XuiuX.",
"XyyuX.....XyyuX.",
"X2y2XXXXXXX2y2X.",
"X212XX112XX121X.",
"X<1<XX<1<XX<1<X.",
"X,<,,<,<,,<,<,X.",
"XX;,,;,;,;,;,XX.",
".XXXXX;;-XXXXX..",
".....X*--X..... ",
"    .XXXXX.     ",
"    .......     ",
NULL
};

const char *shape_R[] =
{
"XkkkkkkkkkkkkXX.",
"XhhhhhhhhhhhhhX.",
"XgggXXXXXXXgggX.",
"XpgpX.....XpgpX.",
"XiipXXXXXXXiiiX.",
"XuiuXXuuuuuiuiX.",
"XyyuXXyyuyuyyXX.",
"X2y2XXX2y2XXXX..",
"X212X.XX121XX.. ",
"X<1<X..XX<1<XX..",
"X,<,X...XX,<,XX.",
"X,;,X. ..XX;,;X.",
"XXXXX.  ..XXXXX.",
"......   .......",
"                ",
"                ",
NULL
};

const char *shape_S[] =
{
".XXkkkkkkkkkkXX.",
"XXhhhhhhhhhhhhX.",
"XggggXXXXXXgggX.",
"XppgXX....XXXXX.",
"XipiiXXXXXXXX...",
"XXuiuuuuuuuuXX..",
".XXyuyuyuyuyyXX.",
"..XXXXXXXX2y2yX.",
"XXXXX....XX121X.",
"X<<<XXXXXX<1<1X.",
"X<,<,,,,,<,<,XX.",
"XX,;,,,,;,;,XX..",
".XXXXXXXXXXXX.. ",
"..............  ",
"                ",
"                ",
NULL
};

const char *shape_T[] =
{
"XkkkkkkkkkkkkkX.",
"XhhhhhhhhhhhhhX.",
"XXXXXXXXXXXXXXX.",
".....XpppX......",
"    .XipiX.     ",
"    .XuuuX.     ",
"    .XyuyX.     ",
"    .X2y2X.     ",
"    .X121X.     ",
"    .X<1<X.     ",
"    .X,<,X.     ",
"    .X,;,X.     ",
"    .XXXXX.     ",
"    .......     ",
"                ",
"                ",
NULL
};

const char *shape_U[] =
{
"XkkkX.  .XkkkkX.",
"XhhhX.  .XXhhhX.",
"XgggX.  ..XgggX.",
"XpgpX.   .XpgpX.",
"XiiiX.   .XiiiX.",
"XuiuX.   .XuiuX.",
"XyyuX.   .XyyuX.",
"X2y2X.   .Xy22X.",
"X121X.....X121X.",
"X<1<XXXXXXX<1<X.",
"X,<,<,<,,,<,<,X.",
"XX;,;,;,,;,;,XX.",
".XXXXXXXXXXXXX..",
"............... ",
"                ",
"                ",
NULL
};

const char *shape_V[] =
{
"XkkkX.  .XkkkkX.",
"XhhhX.  .XXhhhX.",
"XgggX.  ..XgggX.",
"XpgpX.. ..XpgpX.",
"XiiiXX...XXiiiX.",
"XuiuuXX.XXuiuiX.",
"XXyuyyXXXyyyuXX.",
".XX2y2yX2y2yXX..",
"..XX1211212XX.. ",
" ..XX<1<1<XX..  ",
"  ..XX,<,XX..   ",
"   ..XX,XX..    ",
"    ..XXX..     ",
"     .....      ",
"                ",
"                ",
NULL
};

const char *shape_W[] =
{
"XkkkX.  .XkkkkX.",
"XhhhX.  .XXhhhX.",
"XgggX.  ..XgggX.",
"XpgpX.   .XpgpX.",
"XiiiX.....XiiiX.",
"XuiuXXXXXXXuiuX.",
"XyyuXXyyyXXyyuX.",
"Xy22XX2y2XX2y2X.",
"X121XX121XX121X.",
"X<1<XX<1<XX<1<X.",
"X,<,<,<,<,<,<,X.",
"XX;,;,;X,;,;,;X.",
".XXXXXXXXXXXXXX.",
"................",
"                ",
"                ",
NULL
};

const char *shape_X[] =
{
"XkkkXX...XXkkkX.",
"XhhhhXX.XXhhhhX.",
"XXXgggXXXgggXXX.",
"..XXpppXppgXX...",
" ..XXipiipXX..  ",
"  ..XXuuiXX..   ",
"  ..XXyuyXX..   ",
" ..XX2y222XX..  ",
"..XX121X212XX...",
"XXX<1<XXX1<<XXX.",
"X,<,<XX.XX,<,,X.",
"X;,;XX...XX,;,X.",
"XXXXX.. ..XXXXX.",
"......   .......",
"                ",
"                ",
NULL
};

const char *shape_Y[] =
{
"XkkkX.  .XkkkkX.",
"XhhhX.. .XXhhhX.",
"XgggXX...XXgggX.",
"XXpgpXX.XXppgXX.",
".XXiiiXXXiipXX..",
"..XXuiuXuuiXX.. ",
" ..XXyyyuyXX..  ",
"  ..XX2y2XX..   ",
"   ..X121X..    ",
"    .X1<1X.     ",
"    .X,<,X.     ",
"    .X,;,X.     ",
"    .XXXXX.     ",
"    .......     ",
"                ",
"                ",
NULL
};

const char *shape_Z[] =
{
"XkkkkkkkkkkkkkX.",
"XhhhhhhhhhhhhhX.",
"XggXXXXXXggggXX.",
"XpXX..XXppgpXX..",
"XXX..XXiipiXX.. ",
"....XXuuiuXX..  ",
" ..XXyyuyXX.....",
"..XX2y2yXX..XXX.",
".XX1212XX..XX1X.",
"XX<1<1XXXXXX<1X.",
"X,<,<,,,,,,<,,X.",
"X,;,;,,,;,,;,;X.",
"XXXXXXXXXXXXXXX.",
"................",
"                ",
"                ",
NULL
};

const char *shape_backslash[] =
{
"XXXXXX..        ",
"XhhhhXX..       ",
"XXggggXX..      ",
".XXpgppXX..     ",
"..XXipiiXX..    ",
" ..XXuiuuXX..   ",
"  ..XXyuyyXX..  ",
"   ..XX2y2yXX.. ",
"    ..XX1211XX..",
"     ..XX<1<1XX.",
"      ..XX,<,,X.",
"       ..XXXXXX.",
"        ........",
"                ",
"                ",
"                ",
NULL
};

const char *shape_underscore[] =
{
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"................",
"XXXXXXXXXXXXXXX.",
"X,,,<,,,,,,,,,X.",
"X,,;,;,,,,,,,;X.",
"XXXXXXXXXXXXXXX.",
"................",
"                ",
"                ",
NULL
};

const char *shape_up_triangle[] =
{
"           ..XX1",
"          ..XX@1",
"         ..XX@@2",
"        ..XX@@#1",
"       ..XX@@#$1",
"      ..XX@@##Z1",
"     ..XX@&##ZZ2",
"    ..XX@@##ZZ61",
"   ..XX@@##Z5661",
"  ..XX@@#$ZZ6672",
" ..XX@@##ZZ66ww2",
"..XX@@#$$5667we1",
".XX@@##ZZ66wwet2",
"XX@@#$$5667wrtf1",
"X@@##Z5577eetfj2",
"XXXXXXXXXXXXXXXX",
NULL
};


const char *shape_down_triangle[] =
{
"XX..            ",
"oXX..           ",
"OoXX..          ",
"+ooXX..         ",
":+ooXX..        ",
"::+ooXX..       ",
"3::+ooXX..      ",
"4>>++oOXX..     ",
"44>>++ooXX..    ",
"944>>:+oOXX..   ",
"9944>::=OoXX..  ",
"908833::+ooXX.. ",
"a098833::+ooXX..",
"sqq9944>>++ooXX.",
"dsqq9944>>:+ooX.",
"XXXXXXXXXXXXXXX.",
NULL
};

const char *shape_backtick[] =
{
"   .XkkkkkkX.   ",
"   .XXhhhhhX.   ",
"   ..XXggggX.   ",
"    ..XXpgpX.   ",
"     ..XXiiX.   ",
"      ..XXuX.   ",
"       ..XXX.   ",
"        .....   ",
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

const char *shape_a[] =
{
"  ............. ",
"  .XXXXXXXXXXX..",
"  .XgggggggggXX.",
"  .XpgppgppgppX.",
"...XXXXXXXXipiX.",
".XXXXXXXXXXuuiX.",
"XXyyyyyyyXXyyyX.",
"X2y2y2y2yXX2y2X.",
"X121XXXXXXX121X.",
"X<1<XXXXXXX<1<X.",
"X,<,<,,,,,<,<,X.",
"XX;,;,,,;,;,;,X.",
".XXXXXXXXXXXXXX.",
"................",
"                ",
"                ",
NULL
};

const char *shape_b[] =
{
"XkkkkX......... ",
"XhhhXXXXXXXXXX..",
"XgggXXgggggggXX.",
"XpgpXXppgppgppX.",
"XiiiXXXXXXXipiX.",
"XuiuX.....XuuiX.",
"XyyuX.   .XyuyX.",
"Xy22X.   .X2y2X.",
"X121X.....X121X.",
"X<1<XXXXXXX<1<X.",
"X,<,<,<,,<,<,<X.",
"X,;,;,;,,;,;,XX.",
"XXXXXXXXXXXXXX..",
"............... ",
"                ",
"                ",
NULL
};

const char *shape_c[] =
{
"                ",
"..............  ",
".XXXXXXXXXXXX.. ",
"XXggggggggggXX. ",
"XpppgppgppgppX. ",
"XipiXXXXXXipiX. ",
"XuuiX....XXXXX. ",
"XyuyX.  ....... ",
"X2y2X.  ....... ",
"X121X....XXXXX. ",
"X<1<XXXXXX<1<X. ",
"X,<,<,<,,<,<,X. ",
"XX;,;,;,;,;,XX. ",
".XXXXXXXXXXXX.. ",
"..............  ",
"                ",
"                ",
NULL
};

const char *shape_d[] =
{
"                ",
".........XkkkkX.",
".XXXXXXXXXXhhhX.",
"XXgggggggXXgggX.",
"XpppgppgpXXpgpX.",
"XipiXXXXXXXiiiX.",
"XuuiX.....XuiuX.",
"XyuyX.   .XyyuX.",
"X2y2X.   .X2y2X.",
"X121X.....X121X.",
"X<1<XXXXXXX<1<X.",
"X,<,<,<,,<,<,<X.",
"XX;,;,;,,;,;,;X.",
".XXXXXXXXXXXXXX.",
"................",
"                ",
"                ",
NULL
};

const char *shape_e[] =
{
"                ",
"............... ",
".XXXXXXXXXXXXX..",
"XXgggggggggggXX.",
"XpppgppgppgpppX.",
"XipiXXXXXXXipiX.",
"XuuiXXXXXXXuuiX.",
"XyuyXXyyyyyyuyX.",
"X2y2XX2y2y2y22X.",
"X121XXXXXXXXXXX.",
"X<1<XXXXXXXX....",
"X,<,<,,,,,,X.   ",
"XX;,;,,;,,;X.   ",
".XXXXXXXXXXX.   ",
".............   ",
"                ",
"                ",
NULL
};

const char *shape_f[] =
{
"    .XXkkkkkX.  ",
"    .XhhhhhhX. .",
"    .XgggXXXX. .",
"    .XpgpX.... .",
" ....XiipX.... .",
" .XXXXuiuXXXX.  ",
" .XyyyuyyyyyX.  ",
" .X2y2y2y2y2X.  ",
" .XXXX121XXXX.  ",
" ....X<1<X....  ",
"    .X<,<X.     ",
" ....X;,;X.     ",
" .XXXX;;-X.     ",
" .X-*-*--X.     ",
" .X&&&&&XX.     ",
" .XXXXXXX..     ",
"                ",
NULL
};

const char *shape_g[] =
{
"                ",
"................",
".XXXXXXXXXXXXXX.",
"XXggggggggggggX.",
"XpppgppgppgppgX.",
"XipiXXXXXXXipiX.",
"XuuiX.....XuuuX.",
"XyuyX.   .XyuyX.",
"X2y2X.....X2y2X.",
"X121XXXXXXX212X.",
"X<1<1<1<<XX<1<X.",
"XX,<,<,<,XX,<,X.",
".XXXXXXXXXX;,;X.",
"...XXXXXXXX-;;X.",
"  .X*-**-*-*-*X.",
"  .X&&&*&&&*&XX.",
"  .XXXXXXXXXXX..",
NULL
};

const char *shape_h[] =
{
"                ",
"XkkkkX......... ",
"XhhhXXXXXXXXXX..",
"XgggXggggggggXX.",
"XpgpXppgppgpppX.",
"XiiiXXXXXXXipiX.",
"XuiuX.....XuuiX.",
"XyyuX.   .XyuyX.",
"X2y2X.   .X2y2X.",
"X212X.   .X212X.",
"X<1<X.   .X<1<X.",
"X,<,X.   .X,<,X.",
"X,;,X.   .X,;,X.",
"XXXXX.   .XXXXX.",
"......   .......",
"                ",
"                ",
NULL
};

const char *shape_i[] =
{
"                ",
"     .XkkkX.    ",
"     .XhhhX.    ",
"  ....XXXXX.    ",
"  .XXXXXXXX.    ",
"  .XiiiiiiX.    ",
"  .XuiuiuuX.    ",
"  .XXXXyuyX.    ",
"  ....X22yX.    ",
"  ....X211X.... ",
"  .XXXX<1<XXXX. ",
"  .X,,<,<,<,<X. ",
"  .X,,;,;,;,;X. ",
"  .XXXXXXXXXXX. ",
"  ............. ",
"                ",
"                ",
NULL
};

const char *shape_j[] =
{
"                ",
"        .XkkkX. ",
"        .XhhhX..",
"     ....XXXXX..",
"     .XXXXXXXX..",
"     .XiiiiiiX..",
"     .XuiuiuuX..",
"     .XXXXyuyX..",
"     ....X22yX..",
"        .X211X..",
"....... .X<1<X..",
".XXXXX. .X,<,X..",
".X;,;X...X,;,X..",
".X;-;XXXXX-;;X..",
".X*-*-*-*--*-X..",
".XX&&&&&&&&*XX. ",
"..XXXXXXXXXXX.. ",
NULL
};

const char *shape_k[] =
{
"                ",
"XkkkkX.  .......",
"XhhhXX. ..XXXXX.",
"XgggX....XXgggX.",
"XpgpX...XXpppXX.",
"XiiiX..XXiipXX..",
"XuiuXXXXuiuXX.. ",
"XyyuXyyyyyXX..  ",
"X2y2X2y2y2XX..  ",
"X121XXXX121XX.. ",
"X<1<X..XX<1<XX..",
"X,<,X...XX,<,XX.",
"X,;,X. ..XX;,;X.",
"XXXXX.  ..XXXXX.",
"......   .......",
"                ",
"                ",
NULL
};

const char *shape_l[] =
{
"                ",
" .XkkkkkXX.     ",
" .XhhhhhhX.     ",
" .XXXXgggX.     ",
" ....XpgpX.     ",
"    .XiiiX.     ",
"    .XuiuX.     ",
"    .XyyuX.     ",
"    .Xy22X.     ",
"    .X121X....  ",
"    .X<1<XXXX.  ",
"    .X,<,<,<X.  ",
"    .XX,;,;,X.  ",
"    ..XXXXXXX.  ",
"     .........  ",
"                ",
"                ",
NULL
};

const char *shape_m[] =
{
"                ",
"............... ",
"XXXXXXXXXXXXXX..",
"XggggggXgggggXX.",
"XpppgppppgppppX.",
"XipiXXipiXXipiX.",
"XuuiXXuiuXXuiuX.",
"XyuyXXyyuXXyyuX.",
"X2y2XX2y2XX2y2X.",
"X121XXXXXXX121X.",
"X<1<X.....X<1<X.",
"X,<,X.   .X,<,X.",
"X,;,X.   .X,;,X.",
"XXXXX.   .XXXXX.",
"......   .......",
"                ",
"                ",
NULL
};

const char *shape_n[] =
{
"                ",
"............... ",
"XXXXXXXXXXXXXX..",
"XggggggggggggXX.",
"XppgppgppgppgpX.",
"XipiXXXXXXXiiiX.",
"XuuiX.....XuiuX.",
"XyuyX.   .XyuyX.",
"X2y2X.   .X2y2X.",
"X121X.   .X212X.",
"X<1<X.   .X<1<X.",
"X,<,X.   .X,<,X.",
"X,;,X.   .X,;,X.",
"XXXXX.   .XXXXX.",
"......   .......",
"                ",
"                ",
NULL
};

const char *shape_o[] =
{
"............... ",
".XXXXXXXXXXXXX..",
"XXgggggggggggXX.",
"XpppgppgppgpppX.",
"XipiXXXXXXXipiX.",
"XuuiX.....XuuiX.",
"XyuyX.   .XyuyX.",
"X2y2X.   .X2y2X.",
"X121X.....X121X.",
"X<1<XXXXXXX<1<X.",
"X,<,<,<,,<,<,<X.",
"XX;,;,;,;,;,;XX.",
".XXXXXXXXXXXXX..",
"............... ",
"                ",
"                ",
NULL
};

const char *shape_p[] =
{
"............... ",
"XXXXXXXXXXXXXX..",
"XggggggggggggXX.",
"XppgppgppgppgpX.",
"XipiXXXXXXXiiiX.",
"XuuiX.....XuiuX.",
"XyuyX.   .XyuyX.",
"X2y2X.   .X2y2X.",
"X121X.....X121X.",
"X<1<XXXXXXX<1<X.",
"X,<,XX,,<,<,<,X.",
"X,;,XX,,;,;,;XX.",
"X-;-XXXXXXXXXX..",
"X-*-X.......... ",
"X&&&X.          ",
"XXXXX.          ",
NULL
};

const char *shape_q[] =
{
"................",
".XXXXXXXXXXXXXX.",
"XXggggggggggggX.",
"XpppgppgppgppgX.",
"XipiXXXXXXXipiX.",
"XuuiX.....XuuuX.",
"XyuyX.   .XyuyX.",
"X2y2X.   .X2y2X.",
"X121X.....X121X.",
"X<1<XXXXXXX<1<X.",
"X,<,<,<,,XX,<,X.",
"XX;,;,;,;XX;,;X.",
".XXXXXXXXXX;-;X.",
".........XX*--XX",
"        .X&&&&&X",
"        .XXXXXXX",
NULL
};

const char *shape_r[] =
{
"............... ",
"XXXXXXXXXXXXXX..",
"XggggggggggggXX.",
"XppgppgppgppgpX.",
"XipiXXXXXXXiiiX.",
"XuuuX.....XuiuX.",
"XyuyX.   .XXXXX.",
"X2y2X.   .......",
"X121X.          ",
"X<1<X.          ",
"X,<,X.          ",
"X,;,X.          ",
"XXXXX.          ",
"......          ",
"                ",
"                ",
NULL
};

const char *shape_s[] =
{
"..............  ",
".XXXXXXXXXXXX.  ",
"XXggggggggggX.  ",
"XpppgppgppgpX.  ",
"XipiXXXXXXXXX.. ",
"XuuiXXXXXXXXXX..",
"XyuyyyyyyyyyyXX.",
"XX2y2y2y2y2y2yX.",
".XXXXXXXXXX121X.",
"..XXXXXXXXX<1<X.",
" .X,,,,,,,<,<,X.",
" .X,,;,,,;,;,XX.",
" .XXXXXXXXXXXX..",
" .............. ",
"                ",
"                ",
NULL
};

const char *shape_t[] =
{
" .XkkkX.......  ",
" .XhhhXXXXXXX.  ",
" .XgggggggggX.  ",
" .XpgppgppgpX.  ",
" .XiipXXXXXXX.  ",
" .XuiuX.......  ",
" .XyyuX.        ",
" .Xy22X.        ",
" .X121X.......  ",
" .X<1<XXXXXXX.  ",
" .X,<,<,<,,,X.  ",
" .XX,;,;,;,,X.  ",
" ..XXXXXXXXXX.  ",
"  ............  ",
"                ",
"                ",
NULL
};

const char *shape_u[] =
{
"......  ........",
"XXXXX.  .XXXXXX.",
"XgggX.  .XggggX.",
"XppgX.  .XXppgX.",
"XipiX.  ..XipiX.",
"XuuuX.   .XuuuX.",
"XyuyX.   .XyuyX.",
"X2y2X.   .X2y2X.",
"X121X.....X121X.",
"X<1<XXXXXXX<1<X.",
"X,<,<,<,,,<,<,X.",
"XX;,;,;,,;,;,XX.",
".XXXXXXXXXXXXX..",
"............... ",
"                ",
"                ",
NULL
};

const char *shape_v[] =
{
"......  ........",
"XXXXX.  .XXXXXX.",
"XgggX.  .XggggX.",
"XpppX.. .XXpgpX.",
"XipiXX...XXiiiX.",
"XuuiuXX.XXuuiuX.",
"XXyyuyXXXyyuyXX.",
".XX2y22X2y2yXX..",
"..XX1212121XX.. ",
" ..XX<1<1<XX..  ",
"  ..XX,<,XX..   ",
"   ..XX,XX..    ",
"    ..XXX..     ",
"     .....      ",
"                ",
"                ",
NULL
};

const char *shape_w[] =
{
"......  ........",
"XXXXX.  .XXXXXX.",
"XgggX.  .XggggX.",
"XpppX.  .XXpppX.",
"XipiX.....XipiX.",
"XuiuXXXXXXXuuiX.",
"XyyuXXyyyXXyuyX.",
"X2y2XX2y2XX2y2X.",
"X212XX121XX121X.",
"X<1<XX<1<XX<1<X.",
"X,<,,<,<,<,<,<X.",
"XX;,,;,X,;,;,;X.",
".XXXXXXXXXXXXXX.",
"................",
"                ",
"                ",
NULL
};

const char *shape_x[] =
{
"......   .......",
"XXXXX.. ..XXXXX.",
"XgggXX...XXgggX.",
"XppgpXX.XXpppgX.",
"XXXiiiXXXiipXXX.",
"..XXuiuXuuiXX...",
" ..XXyyyuyXX..  ",
" ..XX2y2y2XX..  ",
"..XX121X121XX...",
"XXX<1<XXX<1<XXX.",
"X,<,<XX.XX,<,,X.",
"X;,;XX...XX;,,X.",
"XXXXX.. ..XXXXX.",
"......   .......",
"                ",
"                ",
NULL
};

const char *shape_y[] =
{
"......  ........",
"XXXXX.  .XXXXXX.",
"XgggX.  .XggggX.",
"XpppX.  .XXppgX.",
"XipiX.  ..XipiX.",
"XuuiX.   .XuuuX.",
"XyuyX.   .XyuyX.",
"X2y2X.....X2y2X.",
"X121XXXXXXX121X.",
"X<1<1<1<<<X1<1X.",
"XX,<,<,<<,X,<,X.",
".XXXXXXXXXX,;,X.",
"...XXXXXXXX-;-X.",
"  .X*-**-*-*--X.",
"  .X&&&*&&&*&XX.",
"  .XXXXXXXXXXX..",
NULL
};

const char *shape_z[] =
{
"................",
"XXXXXXXXXXXXXXX.",
"XgggggggggggggX.",
"XppgppgppgppgXX.",
"XiXXXXXXiipiXX..",
"XXX..XXuuiuXX.. ",
"....XXyyuyXX..  ",
" ..XX2y2yXX.....",
"..XX1212XX..XXX.",
".XX<1<1XXXXXX<X.",
"XX,<,<,,,,,,<,X.",
"X;,;,;,,;,,;,;X.",
"XXXXXXXXXXXXXXX.",
"................",
"                ",
"                ",
NULL
};

const char *shape_left_brace[] =
{
"    .XXkkkkX.   ",
"    .XhhhXXX.   ",
"   ..XgggX...   ",
"  ..XXpgpX.     ",
" ..XXiiiXX.     ",
" .XXuiuXX..     ",
" .XyyyXX..      ",
" .XXy2yXX..     ",
" ..XX111XX.     ",
"  ..XX1<1X.     ",
"   ..X,<,X...   ",
"    .X,;,XXX.   ",
"    .XX;-;-X.   ",
"    ..XXXXXX.   ",
"     ........   ",
"                ",
NULL
};

const char *shape_copyright[] =
{
" ..XXXkkkkXXX.. ",
"..XXhhhXXhhhXX..",
".XXggXXXXXXggXX.",
".XppXXppppXXppX.",
"XXipXiiXXiiXipXX",
"XuuXuiXXXXXXXuuX",
"XyuXyyX.....XyuX",
"X2yX2yX.....X22X",
"X11X21XXXXXXX21X",
"XX1<X<<XX<<X<1XX",
".X,<XX<,<,XX,<X.",
".XX;,XXXXXX;,XX.",
"..XX-;-XX-;;XX..",
" ..XXX*-*-XXX.. ",
"  ...XXXXXX...  ",
"    ........    ",
NULL
};

const char *shape_right_brace[] =
{
"   .XkkkkXX.    ",
"   .XXXhhhX.    ",
"   ...XgggX..   ",
"     .XpgpXX..  ",
"     .XXiiiXX.. ",
"     ..XXuiuXX. ",
"      ..XXyyyX. ",
"     ..XX2y2XX. ",
"     .XX121XX.. ",
"     .X<1<XX..  ",
"   ...X,<,X..   ",
"   .XXX,;,X.    ",
"   .X-;-;XX.    ",
"   .XXXXXX..    ",
"   ........     ",
"                ",
NULL
};

const char *shape_tilde[] =
{
".XXkkkkXX.XkkkX.",
"XXhhhhhhXXXhhhX.",
"XggggXgggXggggX.",
"XppgXXXppppgpXX.",
"XipiX.XXipiiXX..",
"XXXXX..XXXXXX.. ",
"..............  ",
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

const char *shape_right_triangle[] =
{
"&XXX...         ",
"-X&XXX...       ",
"<X-X&XXX...     ",
"1X,X-X&XXX...   ",
"yX2X,X-X&XXX... ",
"pX2X1X,X-X&XXX..",
"gXiXyX1X,X-X&XXX",
"kXgXiXyX1X,X-X&X",
"gXiXyX2X,X;X&XXX",
"pXyX2X,X;X&XXX..",
"yX1X,X;X&XXX... ",
"2X,X;X&XXX...   ",
",X;X&XXX...     ",
";X&XXX...       ",
"&XXX...         ",
"XX...           ",
NULL
};

const struct shape Font[] =
{
   [0] = { std_colors, shape_space },
   [1] = { std_colors, shape_space },
   [2] = { std_colors, shape_space },
   [3] = { std_colors, shape_space },
   [4] = { std_colors, shape_space },
   [5] = { std_colors, shape_space },
   [6] = { std_colors, shape_space },
   [7] = { std_colors, shape_space },
   [8] = { std_colors, shape_space },
   [9] = { std_colors, shape_space },
  [10] = { std_colors, shape_space },
  [11] = { std_colors, shape_space },
  [12] = { std_colors, shape_space },
  [13] = { std_colors, shape_space },
  [14] = { std_colors, shape_space },
  [15] = { std_colors, shape_space },
  [16] = { std_colors, shape_space },
  [17] = { std_colors, shape_space },
  [18] = { std_colors, shape_space },
  [19] = { std_colors, shape_space },
  [20] = { std_colors, shape_space },
  [21] = { std_colors, shape_space },
  [22] = { std_colors, shape_space },
  [23] = { std_colors, shape_space },
  [24] = { std_colors, shape_space },
  [25] = { std_colors, shape_space },
  [26] = { std_colors, shape_space },
  [27] = { std_colors, shape_space },
  [28] = { std_colors, shape_space },
  [29] = { std_colors, shape_space },
  [30] = { std_colors, shape_space },
  [31] = { std_colors, shape_space },
  [32] = { std_colors, shape_space },
  [33] = { std_colors, shape_exclamation },
  [34] = { std_colors, shape_double_quote },
  [35] = { std_colors, shape_hash },
  [36] = { std_colors, shape_dollar },
  [37] = { std_colors, shape_percent },
  [38] = { std_colors, shape_ampersand },
  [39] = { std_colors, shape_tick },
  [40] = { std_colors, shape_left_parenthesis },
  [41] = { std_colors, shape_right_parenthesis },
  [42] = { std_colors, shape_asterisk },
  [43] = { std_colors, shape_plus },
  [44] = { std_colors, shape_comma },
  [45] = { std_colors, shape_minus },
  [46] = { std_colors, shape_period },
  [47] = { std_colors, shape_slash },
  [48] = { std_colors, shape_0 },
  [49] = { std_colors, shape_1 },
  [50] = { std_colors, shape_2 },
  [51] = { std_colors, shape_3 },
  [52] = { std_colors, shape_4 },
  [53] = { std_colors, shape_5 },
  [54] = { std_colors, shape_6 },
  [55] = { std_colors, shape_7 },
  [56] = { std_colors, shape_8 },
  [57] = { std_colors, shape_9 },
  [58] = { std_colors, shape_colon },
  [59] = { std_colors, shape_semicolon },
  [60] = { std_colors, shape_less },
  [61] = { std_colors, shape_equal },
  [62] = { std_colors, shape_greater },
  [63] = { std_colors, shape_question },
  [64] = { std_colors, shape_at },
  [65] = { std_colors, shape_A },
  [66] = { std_colors, shape_B },
  [67] = { std_colors, shape_C },
  [68] = { std_colors, shape_D },
  [69] = { std_colors, shape_E },
  [70] = { std_colors, shape_F },
  [71] = { std_colors, shape_G },
  [72] = { std_colors, shape_H },
  [73] = { std_colors, shape_I },
  [74] = { std_colors, shape_J },
  [75] = { std_colors, shape_K },
  [76] = { std_colors, shape_L },
  [77] = { std_colors, shape_M },
  [78] = { std_colors, shape_N },
  [79] = { std_colors, shape_O },
  [80] = { std_colors, shape_P },
  [81] = { std_colors, shape_Q },
  [82] = { std_colors, shape_R },
  [83] = { std_colors, shape_S },
  [84] = { std_colors, shape_T },
  [85] = { std_colors, shape_U },
  [86] = { std_colors, shape_V },
  [87] = { std_colors, shape_W },
  [88] = { std_colors, shape_X },
  [89] = { std_colors, shape_Y },
  [90] = { std_colors, shape_Z },
  [91] = { std_colors, shape_up_triangle }, // [
  [92] = { std_colors, shape_backslash },
  [93] = { std_colors, shape_down_triangle }, // ]
  [94] = { std_colors, shape_space }, // ^
  [95] = { std_colors, shape_underscore },
  [96] = { std_colors, shape_backtick },
  [97] = { std_colors, shape_a },
  [98] = { std_colors, shape_b },
  [99] = { std_colors, shape_c },
 [100] = { std_colors, shape_d },
 [101] = { std_colors, shape_e },
 [102] = { std_colors, shape_f },
 [103] = { std_colors, shape_g },
 [104] = { std_colors, shape_h },
 [105] = { std_colors, shape_i },
 [106] = { std_colors, shape_j },
 [107] = { std_colors, shape_k },
 [108] = { std_colors, shape_l },
 [109] = { std_colors, shape_m },
 [110] = { std_colors, shape_n },
 [111] = { std_colors, shape_o },
 [112] = { std_colors, shape_p },
 [113] = { std_colors, shape_q },
 [114] = { std_colors, shape_r },
 [115] = { std_colors, shape_s },
 [116] = { std_colors, shape_t },
 [117] = { std_colors, shape_u },
 [118] = { std_colors, shape_v },
 [119] = { std_colors, shape_w },
 [120] = { std_colors, shape_x },
 [121] = { std_colors, shape_y },
 [122] = { std_colors, shape_z },
 [123] = { std_colors, shape_left_brace },
 [124] = { std_colors, shape_copyright }, // |
 [125] = { std_colors, shape_right_brace },
 [126] = { std_colors, shape_tilde }, // ~ 
 [127] = { std_colors, shape_right_triangle }, // DEL
};

