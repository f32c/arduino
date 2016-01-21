#include "shape.h"
#include <stdlib.h>

#if 0
// initial draw of the sprite from
// its ascii art base to the plot
// n - number of the sprite
// s - index of the shape to initialize this sprite
// r - rotate n*90 deg
void bmp_shape_draw(uint16_t ix, uint16_t iy, uint8_t in, uint8_t is, uint8_t r)
{
    uint16_t x[0],y[0];
    uint16_t *rx, *ry;
    int16_t sx, sy;
    uint8_t color_list[256];
    struct shape *sh = &(shapes[is]);
    struct charcolors *chc;
    char **bmp; // bitmap: array of strings
    // fill the color array to speed up
    for(chc = sh->colors; chc->c != 0; chc++) // read until we don't hit 0
      color_list[chc->c] = chc->color;
    sx = sy = 1;
    if ( (r & 2) != 0 )
    {
      ix += 31;
      iy += 31;
      sx = sy = -1;
    }
    if( (r & 1) == 0 )
    {
      rx = x;
      ry = y;
    }
    else
    {
      rx = y;
      ry = x;
    }
    for(bmp = sh->bmp, y[0] = 0; y[0] < 256 && *bmp != NULL; y[0]++, bmp++)
    {
      char *clr;
      for(x[0] = 0, clr = *bmp; x[0] < 256 && *clr != 0; x[0]++, clr++)
        bmp_plot(ix+sx*rx[0],iy+sy*ry[0],color_list[*clr]);
    }
}
#endif

