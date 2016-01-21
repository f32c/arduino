#ifndef COMPOSITING_H
#define COMPOSITING_H

#define VGA_X_MAX 640
#define VGA_Y_MAX 480

// compositing line without pixel content
// content needs to be malloc'd
struct compositing_line
{
   struct compositing_line *next; // 32-bit continuation of the same structure, NULL if no more
   int16_t x; // where to start on screen (can be negative)
   uint16_t n; // number of pixels contained here
   // pixels can be multiple of 4 (n lower 2 bits discarded)
   // minimal n is currently 4 (when composite2 core is fixed it could be 0)
   uint8_t *bmp; // pointer to array of pixels (could be more than 1 element)
};

extern struct compositing_line *scanlines[VGA_Y_MAX];

#endif

