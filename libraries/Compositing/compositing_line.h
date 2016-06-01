#ifndef COMPOSITING_LINE_H
#define COMPOSITING_LINE_H

#include <inttypes.h>

// bits per pixel
#define BPP 8

#if BPP == 8
typedef uint8_t pixel_t;
#endif

#if BPP == 16
typedef uint16_t pixel_t;
#endif

#if BPP == 32
typedef uint32_t pixel_t;
#endif

struct compositing_line
{
   struct compositing_line *next; // 32-bit continuation of the same structure, NULL if no more
   int16_t x; // where to start on screen (can be negative)
   uint16_t n; // number of pixels contained here
   // pixels can be multiple of 4 (n lower 2 bits discarded)
   // minimal n is currently 4 (when composite2 core is fixed it could be 0)
   pixel_t *bmp; // pointer to array of pixels (could be more than 1 element)
};

#endif
