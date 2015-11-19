//
// Basic console I/O functions for text output to the video screen
//
#include "f32c_VGATextConsole.h"

// f32c vga_texmode registers
#define	cursor_x		(*(volatile uint8_t  *)0xfffffb80)
#define	cursor_y		(*(volatile uint8_t  *)0xfffffb81)
#define	mono_color		(*(volatile uint8_t  *)0xfffffb82)
#define	cntrl_reg		(*(volatile uint8_t  *)0xfffffb83)
#define	bitmap_addr		(*(volatile uint32_t *)0xfffffb84)
#define	bitmap_color	(*(volatile uint32_t *)0xfffffb88)
#define	text_palette	(*(volatile uint32_t *)0xfffffb8c)
#define	text_addr		(*(volatile uint32_t *)0xfffffb90)

VGA			vga;

uint8_t		VGA::width;
uint8_t		VGA::height;
uint8_t		VGA::cx;
uint8_t		VGA::cy;
uint8_t		VGA::wx;
uint8_t		VGA::wy;
uint8_t		VGA::wwidth;
uint8_t		VGA::wheight;
uint8_t		VGA::color;
uint8_t*	VGA::tilebuf;
uint8_t		VGA::color_flag;

uint8_t VGA::GetHWConfig()
{
	return cntrl_reg;
}

void VGA::SetTextPalette(int entry, uint32_t rgb)
{
	text_palette = ((entry & 0xf) << 24) | (rgb & 0xffffff);
}

void VGA::SetBitmapColor(uint32_t rgb)
{
	bitmap_color = (rgb & 0xffffff);
}

// address should be in correct memory type
void VGA::SetBitmapAddress(volatile void *addr)
{
	bitmap_addr = (uint32_t)addr;
}
void VGA::EnableDisplay(bool onoff)
{
	if (onoff)
		cntrl_reg |= VGA_HW_DISPLAY_ENA;
	else
		cntrl_reg &= ~VGA_HW_DISPLAY_ENA;
}

void VGA::EnableTextMode(bool onoff)
{
	if (onoff)
		cntrl_reg |= VGA_HW_TEXTMODE_ENA;
	else
		cntrl_reg &= ~VGA_HW_TEXTMODE_ENA;
}

void VGA::EnableBitmap(bool onoff)
{
	if (onoff)
		cntrl_reg |= VGA_HW_BITMAP_ENA;
	else
		cntrl_reg &= ~VGA_HW_BITMAP_ENA;
}

void VGA::EnableTextCursor(bool onoff)
{
	if (onoff)
		cntrl_reg |= VGA_HW_CURSOR_ENA;
	else
		cntrl_reg &= ~VGA_HW_CURSOR_ENA;
}

// Initialize video text console
void VGA::Setup(uint8_t *bufferptr, int columns, int rows)
{
	cntrl_reg = 0b11010000;
	tilebuf = bufferptr ? bufferptr : (uint8_t *)0x40000000;
	text_addr = tilebuf;

	cursor_x = 0xff;
	cursor_y = 0xff;
	
	width = columns ? columns : cursor_x;
	height = rows ? rows : cursor_y;
	color = 0x07;
	color_flag = (cntrl_reg & VGA_HW_MONOCHROME) ? 0 : 1;

	SetWindow(0, 0, width, height);
	Clear(CLEAR_TILE);
}

void VGA::SetColor(uint8_t c)
{
		color = c;
		if (!color_flag)
			mono_color = c;
}

// Clear text window
void VGA::Clear(int c)
{
	cx = wx; cy = wy;
	cursor_x = cx;
	cursor_y = cy;
	uint8_t* scr = tilebuf + (((wy * width) + wx)<<color_flag);
	for (uint8_t y = 0; y < wheight; y++)
	{
		for (uint8_t x = 0; x < wwidth; x++)
		{
			*scr++ = c;
			if (color_flag)
				*scr++ = color;
		}

		scr += (width - wwidth)<<color_flag;
	}
}

void VGA::SetWindow(int x, int y, int w, int h)
{
	wx = x;
	wy = y;
	wwidth = w; wheight = h;
	
	cx = x;	cy = y;
	cursor_x = cx;
	cursor_y = cy;
}

void VGA::SetPos(int x, int y)
{
	cx = wx + x;
	if (cx < wx || cx >= wx + wwidth)
		cx = wx;
	cy = wy + y;
	if (cy < wy || cy >= wy + wheight)
		cy = wy;
	cursor_x = cx;
	cursor_y = cy;
}

char *VGA::GetPos()
{
	return (char *) (tilebuf + (((cy * width) + cx)<<color_flag));
}

// Scroll text window
void VGA::ScrollWindowUp()
{
	uint8_t* scr = tilebuf + (((wy * width) + wx)<<color_flag);
	for (uint8_t y = 0; y < wheight-1; y++)
	{
		for (uint8_t x = 0; x < wwidth; x++)
		{
			*scr = *(scr+(width<<color_flag));
			scr++;
			if (color_flag)
			{
				*scr = *(scr+(width<<color_flag));
				scr++;
			}
		}

		scr += ((width - wwidth)<<color_flag);
	}
	
	for (uint8_t x = 0; x < wwidth; x++)
	{
		*scr++ = CLEAR_TILE;
		if (color_flag)
			*scr++ = color;
	}
}

// Scroll text window
void VGA::ScrollWindowDown()
{
	uint8_t* scr = tilebuf + ((((wy+wheight-1) * width) + wx)<<color_flag);
	for (uint8_t y = wheight-1; y != (uint8_t)-1 ; --y)
	{
		for (uint8_t x = 0; x < wwidth; x++)
		{
			*scr = *(scr-(width<<color_flag));
			scr++;
			if (color_flag)
			{
				*scr = *(scr-(width<<color_flag));
				scr++;
			}
		}

		scr -= ((width - wwidth)<<color_flag);
	}
	
	for (uint8_t x = 0; x < wwidth; x++)
	{
		*scr++ = CLEAR_TILE;
		if (color_flag)
			*scr++ = color;
	}
}

void VGA::PrintRawChar(int c)
{
	if (cy >= wy + wheight)
	{
		cy = wy + wheight-1;
		ScrollWindowUp();
	}
	tilebuf[((cy * width) + cx)<<color_flag] = c;
	if (color_flag)
		tilebuf[(((cy * width) + cx)<<color_flag)+1] = color;
	
	if (++cx >= wx + wwidth)
	{
		cx = wx;
		++cy;
	}
	cursor_x = cx;
	cursor_y = cy;
}

// Output a character (and handle control characters)
void VGA::PrintChar(int ch)
{
	uint8_t c = (uint8_t)ch;

	if (c >= ' ')
	{
		if (cy >= wy + wheight)
		{
			cy = wy + wheight-1;
			ScrollWindowUp();
		}
		tilebuf[((cy * width) + cx)<<color_flag] = c;
		if (color_flag)
			tilebuf[(((cy * width) + cx)<<color_flag)+1] = color;
		
		if (++cx >= wx + wwidth)
		{
			cx = wx;
			++cy;
		}
	}
	else if (c == '\r')
	{
		cx = wx;
	}
	else if (c == '\n')
	{
		cx = wx;
		if (++cy >= wy + wheight)
		{
			cy = wy + wheight-1;
			ScrollWindowUp();
		}
	}
	else if (c == '\b')
	{
		if (cx-- <= wx)
		{
			cx = wx + wwidth-1;
			if (cy-- <= wy)
				cy = wy;
		}
	}
	else if (c == '\f')
	{
		Clear();
	}
	else if (c == '\v')
	{
		uint8_t *scr = tilebuf + (((cy * width) + cx)<<color_flag);
		for (uint8_t x = cx; x < wx + wwidth; x++)
		{
			*scr++ = CLEAR_TILE;
			if (color_flag)
				*scr++ = color;
		}
	}
	cursor_x = (cy < (wy+wheight)) ? cx : wx+wwidth-1;
	cursor_y = (cy < (wy+wheight)) ? cy : wy+wheight-1;
}

// Output a string
int VGA::PrintStr(const char *str)
{
	int len = 0;
	char c;
	while ((c = *str++) != 0)
	{
		PrintChar(c);
		len++;
	}
	
	return len;
}

// Output a raw string
int VGA::PrintRawStr(const char *str)
{
	int len = 0;
	char c;
	while ((c = *str++) != 0)
	{
		PrintRawChar(c);
		len++;
	}
	
	return len;
}

// Low-level hex output
void VGA::PrintHex(uint8_t val)
{
	char c;
	c = ((val>>4)&0xf) + '0';
	if (c > '9')
		c += 7;
	PrintChar(c);
	c = (val&0xf) + '0';
	if (c > '9')
		c += 7;
	PrintChar(c);
}

void VGA::PrintHex(uint16_t val)
{
	PrintHex((uint8_t) (val>>8));
	PrintHex((uint8_t) val);
}

void VGA::PrintHex(uint32_t val)
{
	PrintHex((uint16_t) (val>>16));
	PrintHex((uint16_t) val);
}

size_t VGA::write(uint8_t c)
{
	PrintChar(c);
	return 1;
}

// EOF
