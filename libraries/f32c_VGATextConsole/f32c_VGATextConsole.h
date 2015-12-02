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
#include <dev/io.h>

#include "Print.h"

// f32c VGA_textmode register definitions (little-endian assumed)
//
//	            +---------------+---------------+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---------------+---------------+
//	Config:     |           0xFFFFFB83          |           0xFFFFFB82          |           0xFFFFFB81          |           0xFFFFFB80          |
//	            |31  30  29  28  27  26  25  24 |23  22  21  20  19  18  17  16 |15  14  13  12  11  10   9   8 | 7   6   5   4   3   2   1   0 |
//	            +---------------+---------------+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---------------+---------------+
//	            | BRAM mem size |Font size/type |VGC|BGC|TGC|TCC|CBC|CPC|MTC| - |VGE|BME|TME|TCE|CBE| - | - | - | Backgnd color | Foregnd color |
//	            +---------------+---------------+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---------------+---------------+
//	BRAM mem size:      RO  0000=0K BRAM, 0001=1K, 0010=2K, 0011=4K, 0100=8K, 0101=16K, 0110=32K, 0111=64K (others reserved)
//	Font size/type:     RO  0000=no font, 0010=8x8x128, 0011=8x8x256, 0100=8x16x128, 0101=8x16x256 (others reserved)
//	VGC:                RO  Video Generation Configured,    0=no, 1=yes
//	BGC:                RO  Bitmap Generation Configured,   0=no, 1=yes
//	TGC:                RO  Text Generation Configured,     0=no, 1=yes
//	TCC:                RO  Text Cursor Configured,         0=no, 1=yes
//	CBC:                RO  Cursor Blink Configured,        0=no, 1=yes
//	CPC:                RO  Color Palette Configured,       0=no, 1=yes
//	MTC:                RO  Monochrome Text Configured,     0=no, 1=yes
//	VGE:                RW  Video Generation Enabled,       0=off, 1=on (always off if not configured)
//	BGE:                RW  Bitmap Generation Enabled,      0=off, 1=on (always off if not configured)
//	TGE:                RW  Text Generation Enabled,        0=off, 1=on (always off if not configured)
//	TCE:                RW  Text Cursor Enabled,            0=off, 1=on (always off if not configured)
//	CBE:                RW  Cursor Blink Enabled,           0=off, 1=on (always off if not configured)
//	Backgnd:            RW  4-bit text background color (either IRGB or palette index, if palette enabled)
//	Foregnd:            RW  4-bit text foreground color (either IRGB or palette index, if palette enabled)

#define IO_VGATEXT_CONFIG		IO_ADDR(0x380)

static inline void		VGAText_SetDefaultTextColor(uint8_t color)		{ ((volatile uint8_t *)IO_VGATEXT_CONFIG)[0] = color;		} 
static inline uint8_t	VGAText_GetDefaultTextColor()					{ return ((volatile uint8_t *)IO_VGATEXT_CONFIG)[0];		}

#define	VGATEXT_FEATURE_VIDEO_GEN		(1<<7)
#define	VGATEXT_FEATURE_BITMAP			(1<<6)
#define	VGATEXT_FEATURE_TEXT			(1<<5)
#define	VGATEXT_FEATURE_CURSOR			(1<<4)
#define	VGATEXT_FEATURE_CURSORBLINK		(1<<3)
#define	VGATEXT_FEATURE_PALETTE			(1<<2)
#define	VGATEXT_FEATURE_MONOCHROME		(1<<1)
#define	VGATEXT_FEATURE_BRAM_REG_READ	(1<<0)

static inline uint8_t	VGAText_GetFeatureConfigFlags()					{ return ((volatile uint8_t *)IO_VGATEXT_CONFIG)[2];		} 
static inline uint8_t	VGAText_GetFeatureEnableFlags()					{ return ((volatile uint8_t *)IO_VGATEXT_CONFIG)[1];		} 
static inline void		VGAText_SetFeatureEnableFlags(uint8_t features)	{ ((volatile uint8_t *)IO_VGATEXT_CONFIG)[1] = features;	}

#define VGATEXT_FONTCHARS_256	(1<<0)		// false when 128 character font
#define VGATEXT_FONTHEIGHT_8	(1<<1)		// true if 8 high font
#define VGATEXT_FONTHEIGHT_16	(1<<2)		// true if 16 high font

static inline uint8_t	VGAText_GetFontSizeType()						{ return ((volatile uint8_t *)IO_VGATEXT_CONFIG)[3] & 0xf;	} 

static inline uint8_t	VGAText_GetBRAMSize()							{ return ((volatile uint8_t *)IO_VGATEXT_CONFIG)[3] >> 4;	} 

//	
//	            +---------------+-----------------------------------------------+---------------+-----------------------------------------------+
//	Config2:    |           0xFFFFFB87          |           0xFFFFFB86          |           0xFFFFFB85          |           0xFFFFFB84          |
//	            |31  30  29  28  27  26  25  24 |23  22  21  20  19  18  17  16 |15  14  13  12  11  10   9   8 | 7   6   5   4   3   2   1   0 |
//	            +---+---+---+---+-----------------------------------------------+---------------+-----------------------------------------------+
//	(read)      |VBL|FS |Y2 |X2 |              screen height                    |  Bitmap bpp   |                 screen width                  |
//	(write)     | 0 | 0 |Y2 |X2 | 0   0   0   0   0   0   0   0   0   0   0   0 | 0   0   0   0 | 0   0   0   0   0   0   0   0   0   0   0   0 |
//	            +---+---+---+---+-----------------------------------------------+---------------+-----------------------------------------------+
//	Y2:                 RW  double font data vertically (16 or 32 pixel high font)
//	X2:                 RW  double font data horizontally (16 pixel wide font)
//  VBL:				RO  true when in vertical scan is off-screen
//	screen height:      RO  0-4095 pixels high
//	bitmap bpp:         RO  0000=1 bpp, 0001=2 bpp, 0010=4 bpp, 0011=8 bpp (others reserved)
//	screen width:       RO  0-4095 pixels wide
//	text width modulo:  WO  0-4095 additional offset at end of each text line

#define IO_VGATEXT_CONFIG2		IO_ADDR(0x384)

static inline uint16_t	VGAText_GetScreenWidth()						{ return ((volatile uint16_t *)IO_VGATEXT_CONFIG2)[0] & 0xfff;	} 
static inline uint16_t	VGAText_GetScreenHeight()						{ return ((volatile uint16_t *)IO_VGATEXT_CONFIG2)[1] & 0xfff;	} 
static inline uint8_t	VGAText_GetBitmapBPP()							{ return ((volatile uint8_t *)IO_VGATEXT_CONFIG2)[1] >> 4;		} 
static inline uint8_t	VGAText_GetPixelDouble()						{ return ((volatile uint8_t *)IO_VGATEXT_CONFIG2)[3] >> 4;		} 
static inline void		VGAText_SetPixelDouble(uint8_t dbl_yx)			{ ((volatile uint8_t *)IO_VGATEXT_CONFIG2)[3] = dbl_yx << 4;	}
static inline bool		VGAText_GetVerticalBlank()						{ return ((volatile int32_t *)IO_VGATEXT_CONFIG2)[0] < 0;		} 
static inline bool		VGAText_GetFineScrollConfig()					{ return (((volatile int8_t *)IO_VGATEXT_CONFIG2)[3]&0x40)!=0; } 

//	
//	            +-------------------------------+-------------------------------+-------------------------------+-------------------------------+
//	Cursor:     |           0xFFFFFB8B          |           0xFFFFFB8A          |           0xFFFFFB89          |           0xFFFFFB88          |
//	            |31  30  29  28  27  26  25  24 |23  22  21  20  19  18  17  16 |15  14  13  12  11  10   9   8 | 7   6   5   4   3   2   1   0 |
//	            +---------------+---------------+-----------------------+-------+-------------------------------+-------------------------------+
//	(read)      |       text cell height        |   text font address   | 0   0 |       cursor Y position       |       cursor X position       |
//	(write)     | Y fine scroll | X fine scroll |   text font address   | 0   0 |       cursor Y position       |       cursor X position       |
//	            +---------------+---------------+-----------------------+-------+-------------------------------+-------------------------------+
//	text cell height:   RO  0x00-0xFF text cell height
//	text fine Y scroll: WO  0x00-(char_cell_height) pixel Y fine scroll
//	text fine X scroll: WO  0x00-(char_cell_width) pixel X fine scroll
//	text font address:  RW  0x00-0xFC upper 8-bits of BRAM font address (on 1K or 2K boundary, depending on font size)
//	cursor Y:           RW  0x00-0xFF cursor position
//	cursor X:           RW  0x00-0xFF cursor position

#define IO_VGATEXT_CURSOR		IO_ADDR(0x388)

static inline uint8_t	VGAText_GetCursorX()							{ return ((volatile uint8_t *)IO_VGATEXT_CURSOR)[0];	} 
static inline uint8_t	VGAText_GetCursorY()							{ return ((volatile uint8_t *)IO_VGATEXT_CURSOR)[1];	}
static inline void		VGAText_SetCursorX(uint8_t x)					{ ((volatile uint8_t *)IO_VGATEXT_CURSOR)[0] = x;		} 
static inline void		VGAText_SetCursorY(uint8_t y)					{ ((volatile uint8_t *)IO_VGATEXT_CURSOR)[1] = y;		} 

static inline uint8_t*	VGAText_GetFontAddr()							{ return (uint8_t *)(0x40000000|((volatile uint8_t *)IO_VGATEXT_CURSOR)[2]<<8);	} 
static inline void		VGAText_SetFontAddr(void *addr)					{ ((volatile uint8_t *)IO_VGATEXT_CURSOR)[2] = (uint8_t)((uint32_t)addr >> 8);	} 

static inline uint8_t	VGAText_GetTextCellHeight()						{ return ((volatile uint8_t *)IO_VGATEXT_CURSOR)[3];	} 
static inline void		VGAText_SetFineScroll(uint8_t yx)				{ ((volatile uint8_t *)IO_VGATEXT_CURSOR)[3] = yx;		} 

//	
//	            +-------------------------------+-------------------------------+-------------------------------+-------------------------------+
//	TextAddr:   |           0xFFFFFB8F          |           0xFFFFFB8E          |           0xFFFFFB8D          |           0xFFFFFB8C          |
//	            |31  30  29  28  27  26  25  24 |23  22  21  20  19  18  17  16 |15  14  13  12  11  10   9   8 | 7   6   5   4   3   2   1   0 |
//	            +---------------+---------------+-------------------------------+-------------------------------+-----------------------+-------+
//	            |    Mem Type   |                                           TextAddr                                                    | 0   0 |
//	            +---------------+---------------+-------------------------------+-------------------------------+-----------------------+-------+
//	Mem Type:           RO  0100 = BRAM, 1000 = SDRAM/SRAM (others reserved)
//	TextAddr:           RW  Text/color attribute screen memory address  (27 downto 2)

#define IO_VGATEXT_TEXT_ADDR	IO_ADDR(0x38C)

static inline uint8_t*	VGAText_GetTextAddr()									{ return (uint8_t *)((volatile uint32_t *)IO_VGATEXT_TEXT_ADDR)[0];	} 
static inline void		VGAText_SetTextAddr(void *addr)							{ ((volatile uint32_t *)IO_VGATEXT_TEXT_ADDR)[0] = (uint32_t)addr;	} 

//	
//	            +-------------------------------+-------------------------------+-------------------------------+-------------------------------+
//	BitmapAddr: |           0xFFFFFB93          |           0xFFFFFB92          |           0xFFFFFB91          |           0xFFFFFB90          |
//	            |31  30  29  28  27  26  25  24 |23  22  21  20  19  18  17  16 |15  14  13  12  11  10   9   8 | 7   6   5   4   3   2   1   0 |
//	            +---------------+---------------+-------------------------------+-------------------------------+-----------------------+-------+
//	            |    Mem Type   |                                           BitmapAddr                                                  | 0   0 |
//	            +---------------+---------------+-------------------------------+-------------------------------+-----------------------+-------+
//	Mem Type:           RO  0100 = BRAM, 1000 = SDRAM/SRAM (others reserved)
//	BitmapAddr:         RW  Bitmap screen memory address (27 downto 2)

#define IO_VGATEXT_BMAP_ADDR	IO_ADDR(0x390)

static inline uint8_t*	VGAText_GetBitmapAddr()									{ return (uint8_t *)((volatile uint32_t *)IO_VGATEXT_BMAP_ADDR)[0];	} 
static inline void		VGAText_SetBitmapAddr(void *addr)						{ ((volatile uint32_t *)IO_VGATEXT_BMAP_ADDR)[0] = (uint32_t)addr;	} 

//	
//	            +---------------+---------------+-------------------------------+-------------------------------+-------------------------------+
//	SetPalette: |           0xFFFFFF97          |           0xFFFFFB96          |           0xFFFFFB95          |           0xFFFFFB94          |
//	            |31  30  29  28  27  26  25  24 |23  22  21  20  19  18  17  16 |15  14  13  12  11  10   9   8 | 7   6   5   4   3   2   1   0 |
//	            +---+-----------+---------------+-------------------------------+-------------------------------+-------------------------------+
//	            |BM | 0   0   0 |     index     |               red             |             green             |             blue              |
//	            +---+-----------+---------------+-------------------------------+-------------------------------+-------------------------------+
//	BM:                 WO  BM=1, set bitmap color (only when bitmap bits per pixel = 1)
//	index:              WO  0x0-0xF palette index
//	red:                WO  0x00-0xFF red gun (only configured significant bits will be kept)
//	green:              WO  0x00-0xFF green gun (only configured significant bits will be kept)
//	blue:               WO  0x00-0xFF blue gun (only configured significant bits will be kept)

#define IO_VGATEXT_PALETTE		IO_ADDR(0x394)

static inline void		VGAText_SetPalette(uint32_t index, uint32_t rgb)		{ ((volatile uint32_t *)IO_VGATEXT_PALETTE)[0] = (index<<24)|rgb; } 

//	
//	            +---+-----------+---------------+-------------------------------+-------------------------------+-----------------------+-------+
//	BRAMAddr:   |           0xFFFFFB9B          |           0xFFFFFB9A          |           0xFFFFFB99          |           0xFFFFFB98          |
//	            |31  30  29  28  27  26  25  24 |23  22  21  20  19  18  17  16 |15  14  13  12  11  10   9   8 | 7   6   5   4   3   2   1   0 |
//	            +---------------+---------------+-------------------------------+-------------------------------+-----------------------+---+---+
//	(write)     | 0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0 |                           BRAMAddr                    | 0   0 |
//	(read)      | 0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0 | 0   0   0   0   0   0   0   0   0   0   0   0   0   0 | 0 |WAI|
//	            +---------------+---------------+-------------------------------+-------------------------------+-----------------------+---+---+
//	BRAMAddr:           Wr  BRAM memory address to read
//	WAI:                Rd  WAI=1 when BRAMData data not valid yet
//	
//	            +-------------------------------+-------------------------------+-------------------------------+-------------------------------+
//	BRAMData:   |           0xFFFFFB9F          |           0xFFFFFB9E          |           0xFFFFFB9D          |           0xFFFFFB9C          |
//	            |31  30  29  28  27  26  25  24 |23  22  21  20  19  18  17  16 |15  14  13  12  11  10   9   8 | 7   6   5   4   3   2   1   0 |
//	            +-------------------------------+-------------------------------+-------------------------------+-------------------------------+
//	            |                                                           BRAMData                                                            |
//	            +-------------------------------+-------------------------------+-------------------------------+-------------------------------+
//	BRAMData:           RO  BRAM data at address BRAMAddr (when BRAMAddr(0)=0)
//	
#define IO_VGATEXT_BRAM_ADDR	IO_ADDR(0x398)
#define IO_VGATEXT_BRAM_DATA	IO_ADDR(0x39C)

static inline uint32_t	VGAText_ReadBRAMData(void *addr)
{
	((volatile uint32_t *)IO_VGATEXT_BRAM_ADDR)[0] = (uint32_t)addr;
	while (((volatile uint32_t *)IO_VGATEXT_BRAM_ADDR)[0])
		;
	return ((volatile uint32_t *)IO_VGATEXT_BRAM_DATA)[0];
} 

#if !defined(CLEAR_TILE)
#define CLEAR_TILE ' '	// tile used for clearing display (space unless set already)
#endif

struct VGA	 : public Print
{
	// Initialize video text console with maximum sized text window
	// tile buffer (at least columns*rows bytes), tile columns, tile rows
	static void Setup(uint8_t *bufferptr = NULL);
	
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
	static inline uint8_t GetPosX()						{ return cx; }
	
	// Get current Y position
	static inline uint8_t GetPosY()						{ return cy; }
	
	// Set smooth scroll speed (0=disabled, 1=slowest, 8=fastest)
	static void SetSmoothScrollSpeed(uint8_t ss)		{ smooth_scroll = ss & 0xf; }

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

	static bool IsDisplayConfigured()					{ return (VGAText_GetFeatureConfigFlags() & VGATEXT_FEATURE_VIDEO_GEN);		}
	static void EnableDisplay()							{ VGAText_SetFeatureEnableFlags(VGAText_GetFeatureEnableFlags() | VGATEXT_FEATURE_VIDEO_GEN);	}
	static void DisableDisplay()						{ VGAText_SetFeatureEnableFlags(VGAText_GetFeatureEnableFlags() & ~VGATEXT_FEATURE_VIDEO_GEN);	}
	static bool IsTextConfigured()						{ return (VGAText_GetFeatureConfigFlags() & VGATEXT_FEATURE_TEXT);			}
	static void EnableTextMode()						{ VGAText_SetFeatureEnableFlags(VGAText_GetFeatureEnableFlags() | VGATEXT_FEATURE_TEXT);		}
	static void DisableTextMode()						{ VGAText_SetFeatureEnableFlags(VGAText_GetFeatureEnableFlags() & ~VGATEXT_FEATURE_TEXT);		}
	static bool IsBitmapConfigured()					{ return (VGAText_GetFeatureConfigFlags() & VGATEXT_FEATURE_BITMAP);		}
	static void EnableBitmap()							{ VGAText_SetFeatureEnableFlags(VGAText_GetFeatureEnableFlags() | VGATEXT_FEATURE_BITMAP);		}
	static void DisableBitmap()							{ VGAText_SetFeatureEnableFlags(VGAText_GetFeatureEnableFlags() & ~VGATEXT_FEATURE_BITMAP);		}
	static bool IsCursorConfigured()					{ return (VGAText_GetFeatureConfigFlags() & VGATEXT_FEATURE_CURSOR);		}
	static void EnableTextCursor()						{ VGAText_SetFeatureEnableFlags(VGAText_GetFeatureEnableFlags() | VGATEXT_FEATURE_CURSOR);		}
	static void DisableTextCursor()     	            { VGAText_SetFeatureEnableFlags(VGAText_GetFeatureEnableFlags() & ~VGATEXT_FEATURE_CURSOR);		}
	static bool IsCursorBlinkConfigured()				{ return (VGAText_GetFeatureConfigFlags() & VGATEXT_FEATURE_CURSORBLINK);	}
	static void EnableCursorBlink()						{ VGAText_SetFeatureEnableFlags(VGAText_GetFeatureEnableFlags() | VGATEXT_FEATURE_CURSORBLINK);	}
	static void DisableCursorBlink()               		{ VGAText_SetFeatureEnableFlags(VGAText_GetFeatureEnableFlags() & ~VGATEXT_FEATURE_CURSORBLINK);}

	static bool IsMonochromeConfigured()				{ return (VGAText_GetFeatureConfigFlags() & VGATEXT_FEATURE_MONOCHROME);	}
	static bool IsPaletteConfigured()					{ return (VGAText_GetFeatureConfigFlags() & VGATEXT_FEATURE_PALETTE);		}
	static bool IsBRAMRegisterReadConfigured()			{ return (VGAText_GetFeatureConfigFlags() & VGATEXT_FEATURE_BRAM_REG_READ);	}
	static bool IsFineScrollConfigured()				{ return VGAText_GetFineScrollConfig();										}
	
	static int	GetBRAMSize()							{ return VGAText_GetBRAMSize() ? 1<<(VGAText_GetBRAMSize()-1) : 0;			}	
	static int	GetDisplayWidth()						{ return VGAText_GetScreenWidth();	}
	static int	GetDisplayHeight()						{ return VGAText_GetScreenHeight();	}

	static int	GetTextDisplayWidth()					{ return VGAText_GetScreenWidth() >> 3;	}
	static int	GetTextDisplayHeight()					{ return VGAText_GetScreenHeight() / VGAText_GetTextCellHeight();	}
	
	static int	GetFontCharacters()						{ return VGAText_GetFontSizeType() ? (VGAText_GetFontSizeType() & VGATEXT_FONTCHARS_256) ? 256 : 128 : 0; }
	static int	GetFontWidth()							{ return 8;	}
	static int	GetFontHeight()							{ return VGAText_GetFontSizeType() & VGATEXT_FONTHEIGHT_8 ? 8 : VGAText_GetFontSizeType() & VGATEXT_FONTHEIGHT_16 ? 16 : 0; }
	
	static int	GetBitmapBitsPerPixel()					{ return VGAText_GetBitmapBPP() > 1 ? 1<<(VGAText_GetBitmapBPP()) : 1;	}
	static void SetBitmapColor(uint32_t rgb)			{ VGAText_SetPalette(0x80, rgb); 	}

	static void SetTextPalette(int entry, uint32_t rgb)	{ VGAText_SetPalette(entry, rgb); 	}
	
	// address should be in correct memory type
	static uint8_t*GetTextAddress()						{ return VGAText_GetTextAddr();		}
	static void SetTextAddress(void *addr)				{ VGAText_SetTextAddr(addr);		}
	static uint8_t*GetFontAddress()						{ return VGAText_GetFontAddr();	}
	static void SetFontAddress(void *addr)				{ VGAText_SetFontAddr(addr);		}
	static uint8_t*GetBitmapAddress()					{ return VGAText_GetBitmapAddr();	}
	static void SetBitmapAddress(void *addr)			{ VGAText_SetBitmapAddr(addr);		}
	
	// internal variables
	static uint8_t		first_glyph;				// value added to ASCII value before storing to screen
	static uint8_t		width, height;				// width and height in tiles of screen tile buffer
	static uint8_t		cx, cy;						// cursor coordinates (0,0 is upper left of tile buffer)
	static uint8_t		wx, wy, wwidth, wheight;	// current window edges
	static uint8_t		color;						// current 4-bit color (background | foreground)
	static uint8_t*		tilebuf;					// pointer to start of tile buffer in RAM
	static uint8_t		color_flag;					// 0 for mono, 1 for color
	static uint8_t		smooth_scroll;				// smooth scroll speed (0=disabled, 1=slowest..8=fastest)
	static uint8_t		reg_read_flag;				// true if using register read interface
};

extern VGA	vga;

#endif // F32C_VIDEOTEXTCONSOLE_H

// EOF

