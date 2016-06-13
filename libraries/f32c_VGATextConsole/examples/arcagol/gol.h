#ifndef GOL_H
#define GOL_H

#include <stdint.h>

/* GoL struct
*/
struct gol
{
  uint8_t v:1, n:4;
  // v: 1-bit pixel 1=on 0=off
  // n: 4-bit number of surrounding pixels (0-8)
};

/* context required to iterate gol */
struct gol_context
{
  uint16_t y; // current line to process
  /* buffer contains lines at y-1, y, y+1, y+2
  ** value of y_buf represents line at y
  */
  uint8_t y_buf; // buffer circular pointer 0-3 here is copy of lines
  uint8_t y_buf_valid; // is the buffer content valid?
};

extern struct gol_context gol_context;

/* cell playfield dimensions
** represent infinite circular closed-in-itself
** area, use 2^n dimensions so the calculation of
** wraparound offset is easy
*/
enum { RANGE_X=128, RANGE_Y=64, RANGE_Y_BUF=4 };

extern struct gol cell[RANGE_Y][RANGE_X];
/* as we sequentially update we will store here
** previous values of 4 lines, one before and 2 ahead
*/
extern struct gol cell_buf[RANGE_Y_BUF][RANGE_X];

/* short iteration of a part of the GOL
** cells update. Saves state and can be
** called again any time again,
** when CPU has nothing to do and 
** waits for vblank this will be called
*/
int gol_iterate(void); // calculates a part of the iteration
void gol_clear(void);
void gol_plot(uint16_t x, uint16_t y, uint8_t v); // plot a gol pixel
// peek for the gol cell (returns struct gol)
#define gol_peek(x,y) (cell[y & (RANGE_Y-1)][x & (RANGE_X-1)])
#endif

