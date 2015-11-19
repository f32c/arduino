#ifndef F32C_VIDEOTEXTCONSOLE_H
#define F32C_VIDEOTEXTCONSOLE_H
//
// Basic console I/O functions for text output to the video screen.
//
// Supports basic text console features such as cursor wrapping and scrolling.
// Control characters understood: '\n', '\r', '\b', '\f' (clear) and '\v' (clear to EOL)
// All other characters output to screen without translation.
//

#include <stdint.h>
#include <stdio.h>

#include "Print.h"

#if !defined(CLEAR_TILE)
#define CLEAR_TILE ' '	// tile used for clearing display
#endif

struct VGA	 : public Print
{
	enum
	{
		VGA_HW_DISPLAY_ENA		= (1<<7),	// RW video signal generation enabled
		VGA_HW_TEXTMODE_ENA		= (1<<6),	// RW text-mode generation enabled
		VGA_HW_BITMAP_ENA		= (1<<5),	// RW bitmap generation enabled (if present)
		VGA_HW_CURSOR_ENA		= (1<<4),	// RW text cursor generation enabled
		VGA_HW_MONOCHROME		= (1<<3),	// RD 2-color monochrome present (otherwise 16-color with color byte)
		VGA_HW_BITMAP			= (1<<2),	// RD bitmap hardware present
		VGA_HW_MODERES			= (1<<1),	// RD 0=640x480, 1=800x600
		VGA_HW_UNUSED			= (1<<0)	// RD undefined (reads 0)
	};

	// Initialize video text console with maximum sized text window
	// tile buffer (at least columns*rows bytes), tile columns, tile rows
	static void Setup(uint8_t *bufferptr = NULL, int columns = 0, int rows = 0);
	
	// Set terminal text window boundaries and position cursor at upper left
	static void SetWindow(int x, int y, int w, int h);

	// Set text color (background<<4) | foreground
	static void SetColor(uint8_t c);

	// Clear screen and position cursor at upper left (clears to char c, space by default)
	static void Clear(int c = CLEAR_TILE);

	// Scroll screen window up (called automatically when cursor off right edge on bottom row)
	static void ScrollWindowUp();

	// Scroll screen window down
	static void ScrollWindowDown();

	// Set position of next character printed (0,0 is upper left of current window)
	static void SetPos(int x, int y);

	// Set position of next character printed (0,0 is upper left of current window)
	static char *GetPos();
	
	// Get current X position
	static inline uint8_t GetPosX() { return cx; }
	
	// Get current Y position
	static inline uint8_t GetPosY() { return cy; }
	
	// Output a single character to the current cursor position
	static void PrintChar(int c);

	// Output a single raw character to the current cursor position (no control codes interpreted)
	static void PrintRawChar(int c);
	
	// Output a null terminated string up to maxlen characters (0=unlimited), returns characters output
	static int PrintStr(const char *str);
	static int PrintRawStr(const char *str);

	// Print hexadecimal value (for low-memory low-level debugging)
	static void PrintHex(uint8_t val);				// 2 digits
	static void PrintHex(uint16_t val);				// 4 digits
	static void PrintHex(uint32_t val);				// 8 digits

	virtual size_t write(uint8_t);		// used by Arduino "Print.h" (and the one required virtual function)

	static void SetTextPalette(int entry, uint32_t rgb);
	static void SetBitmapColor(uint32_t rgb);
	static void EnableDisplay(bool onoff);
	static void EnableTextMode(bool onoff);
	static void EnableBitmap(bool onoff);
	static void EnableTextCursor(bool onoff);
	
	// address should be in correct memory type
	static void SetBitmapAddress(volatile void *addr);
	
	static uint8_t	GetHWConfig();

	// internal variables
	static uint8_t		first_glyph;				// value added to ASCII value before storing to screen
	static uint8_t		width, height;				// width and height in tiles of screen tile buffer
	static uint8_t		cx, cy;						// cursor coordinates (0,0 is upper left of tile buffer)
	static uint8_t		wx, wy, wwidth, wheight;	// current window edges
	static uint8_t		color;						// current 4-bit color (background | foreground)
	static uint8_t*		tilebuf;					// pointer to start of tile buffer in RAM
	static uint8_t		color_flag;					// 0 for mono, 1 for color
};

extern VGA	vga;

#endif // F32C_VIDEOTEXTCONSOLE_H

// EOF

