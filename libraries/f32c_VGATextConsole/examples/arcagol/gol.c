#include "gol.h"

struct gol cell[RANGE_Y][RANGE_X];
struct gol cell_buf[RANGE_Y_BUF][RANGE_X];
struct gol_context gol_context;

/* one part of gol field iteration
** returns 0 when complete field is iterated
** intended use: while(gol_iterate());
*/
int gol_iterate(void)
{
  uint16_t x, y, yb, y2, y2b;
  /* process one line */
  y   = (gol_context.y)         & (RANGE_Y-1);
  yb  = (gol_context.y_buf)     & (RANGE_Y_BUF-1);
  if(gol_context.y_buf_valid == 0)
  {
    int i;
    // buffer is not valid (cell changed in meantime)
    // copy it fresh from cell
    for(i = -1; i <= 1; i++)
    {
      y2  = (gol_context.y + i) & (RANGE_Y-1);
      y2b = (gol_context.y_buf + i) & (RANGE_Y_BUF-1);
      for(x = 0; x < RANGE_X; x++)
        cell_buf[y2b][x] = cell[y2][x];
    }
    gol_context.y_buf_valid = 1;
  }
  y2  = (gol_context.y + 2)     & (RANGE_Y-1);
  y2b = (gol_context.y_buf + 2) & (RANGE_Y_BUF-1);
  for(x = 0; x < RANGE_X; x++)
  {
    uint16_t xn,xp,yn,yp;
    uint8_t n; // cache here number of surrounding cells
    /* copy y+2 to y_buf+2 */
    cell_buf[y2b][x] = cell[y2][x];
    /* simple rule: cell surrounded by 2 or 3 other cells
    ** keep living, otherwise it dies.
    ** dead cell created by exactly 3 alive
    ** cells become a live cell
    ** use buffer for reference of a previous state,
    ** update buffer in-place
    */
    n = cell_buf[yb][x].n;
    /* live cell? */
    if(cell_buf[yb][x].v != 0)
    {
      // current state: live cell
      if(n < 2 || n > 3) // should live cell die?
      {
        xn = (x-1)&(RANGE_X-1);
        xp = (x+1)&(RANGE_X-1);
        yn = (y-1)&(RANGE_Y-1);
        yp = (y+1)&(RANGE_Y-1);
        cell[y][x].v = 0;
        // update surrounding n's
        cell[yn][xn].n--;
        cell[yn][x ].n--;
        cell[yn][xp].n--;
        cell[y ][xn].n--;
        cell[y ][xp].n--;
        cell[yp][xn].n--;
        cell[yp][x ].n--;
        cell[yp][xp].n--;
      }
    }
    else
    {
      // current state: dead cell
      if(n == 3) // should dead cell resurrect?
      {
        xn = (x-1)&(RANGE_X-1);
        xp = (x+1)&(RANGE_X-1);
        yn = (y-1)&(RANGE_Y-1);
        yp = (y+1)&(RANGE_Y-1);
        cell[y][x].v = 1;
        // update surrounding n's
        cell[yn][xn].n++;
        cell[yn][x ].n++;
        cell[yn][xp].n++;
        cell[y ][xn].n++;
        cell[y ][xp].n++;
        cell[yp][xn].n++;
        cell[yp][x ].n++;
        cell[yp][xp].n++;
      }
    }
  }
    
  gol_context.y = (gol_context.y+1) & (RANGE_Y-1);
  gol_context.y_buf = (gol_context.y_buf+1) & (RANGE_Y_BUF-1);
  return gol_context.y;
}

void gol_clear(void)
{
  gol_context.y = gol_context.y_buf = 0;
  uint16_t x, y;
  for(y = 0; y < RANGE_Y; y++)
  {
    for(x = 0; x < RANGE_X; x++)
    {
      cell[y][x].v = 0;
      cell[y][x].n = 0;
    }
  }
  for(y = 0; y < RANGE_Y_BUF; y++)
  {
    for(x = 0; x < RANGE_X; x++)
    {
      cell_buf[y][x].v = 0;
      cell_buf[y][x].n = 0;
    }
  }
}


// plot a pixel
// v=pixel value 1-set pixel 0-clear pixel
void gol_plot(uint16_t ix, uint16_t iy, uint8_t v)
{
  uint16_t xn,xp,yn,yp;
  // wraparound address
  // assuming range is 2^n value
  uint16_t x = ix & (RANGE_X-1);
  uint16_t y = iy & (RANGE_Y-1);
  // if pixel is alreadty there
  // return immediately
  if(cell[y][x].v == v)
    return;
  cell[y][x].v = v; // set it
  // increase/decrease number of surrounding pixels
  if(v == 0)
  {
    xn = (x-1)&(RANGE_X-1);
    xp = (x+1)&(RANGE_X-1);
    yn = (y-1)&(RANGE_Y-1);
    yp = (y+1)&(RANGE_Y-1);
    cell[yn][xn].n--;
    cell[yn][x ].n--;
    cell[yn][xp].n--;
    cell[y ][xn].n--;
    cell[y ][xp].n--;
    cell[yp][xn].n--;
    cell[yp][x ].n--;
    cell[yp][xp].n--;
  }
  else
  {
    xn = (x-1)&(RANGE_X-1);
    xp = (x+1)&(RANGE_X-1);
    yn = (y-1)&(RANGE_Y-1);
    yp = (y+1)&(RANGE_Y-1);
    cell[yn][xn].n++;
    cell[yn][x ].n++;
    cell[yn][xp].n++;
    cell[y ][xn].n++;
    cell[y ][xp].n++;
    cell[yp][xn].n++;
    cell[yp][x ].n++;
    cell[yp][xp].n++;
  }
  // was buffer overwritten?
  if( yn == gol_context.y
   || y  == gol_context.y
   || yp == gol_context.y )
    gol_context.y_buf_valid = 0; // set buffer not valid
}

