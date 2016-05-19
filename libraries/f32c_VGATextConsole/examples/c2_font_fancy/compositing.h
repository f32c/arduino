#ifndef COMPOSITING_H
#define COMPOSITING_H

#define videodisplay_reg (*(volatile uint32_t *)0xFFFFFB90)
#define cntrl_reg        (*(volatile  uint8_t *)0xFFFFFB81)
#define vblank_reg       (*(volatile  uint8_t *)0xFFFFFB87)

#define VGA_X_MAX 640
#define VGA_Y_MAX 480

// 1-single buffering 2-double buffering (2 not fully implemented)
#define BUFFERING 1

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

// double buffering scanlines
extern volatile struct compositing_line *scanlines[BUFFERING][VGA_Y_MAX];

#endif

