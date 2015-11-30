//
// Basic console I/O functions for text output to the video screen
//
#include "f32c_VGATextConsole.h"

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
uint8_t		VGA::reg_read_flag;
uint8_t		VGA::smooth_scroll;

// Initialize video text console
void VGA::Setup(uint8_t *bufferptr)
{
	if (!IsDisplayConfigured() || !IsTextConfigured())	
		return;

	if (!bufferptr)
	{
		bufferptr = GetTextAddress();
		if (bufferptr == (uint8_t *)0x80000000)	// EVIL, but better than 100% chance of overwriting code...
			bufferptr = (uint8_t *)0x801f0000;		// put at 31MB
	}
	tilebuf = bufferptr;
	SetTextAddress(tilebuf);
	if (((uint32_t)GetTextAddress()&0x40000000) && IsBRAMRegisterReadConfigured())
		reg_read_flag = 1;

	width = GetTextDisplayWidth();
	height = GetTextDisplayHeight()+1;	// vertical scroll line
	color = 0x1f;
	VGAText_SetDefaultTextColor(color);
	color_flag = IsMonochromeConfigured() ? 0 : 1;
	
	EnableTextMode();
	EnableDisplay();

	SetWindow(0, 0, width, height);
	Clear(CLEAR_TILE);
	SetWindow(0, 0, width, height-1);
}

void VGA::SetColor(uint8_t c)
{
	if (!tilebuf)
		return;

		color = c;
		if (!color_flag)
			VGAText_SetDefaultTextColor(c);
}

// Clear text window
void VGA::Clear(int c)
{
	if (!tilebuf)
		return;

	cx = wx; cy = wy;
	VGAText_SetCursorX(cx);
	VGAText_SetCursorY(cy);
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
	if (!tilebuf)
		return;

	wx = x;
	wy = y;
	wwidth = w; wheight = h;
	
	cx = x;	cy = y;
	VGAText_SetCursorX(cx);
	VGAText_SetCursorY(cy);
}

void VGA::SetPos(int x, int y)
{
	if (!tilebuf)
		return;

	cx = wx + x;
	if (cx < wx || cx >= wx + wwidth)
		cx = wx;
	cy = wy + y;
	if (cy < wy || cy >= wy + wheight)
		cy = wy;
	VGAText_SetCursorX(cx);
	VGAText_SetCursorY(cy);
}

char *VGA::GetPos()
{
	return (char *) (tilebuf + (((cy * width) + cx)<<color_flag));
}

static inline uint8_t read_screen(uint8_t *ptr)
{
	if (VGA::reg_read_flag)
	{
		uint32_t w = VGAText_ReadBRAMData((void *)((uint32_t)ptr & ~3U));
		w >>= (((uint32_t)ptr & 3)<<3);
		return (uint8_t)w;
	}

	return *ptr;
}


// Scroll text window
void VGA::ScrollWindowUp()
{
	if (!tilebuf)
		return;

	if (smooth_scroll)
	{
		for (uint32_t vs = smooth_scroll; vs < 0x10; vs += smooth_scroll)
		{
		  while (!VGAText_GetVerticalBlank())
			  ;
		  VGAText_SetFineScroll(vs << 4);
		  while (VGAText_GetVerticalBlank())
			  ;
		}
		while (!VGAText_GetVerticalBlank())
			  ;
		VGAText_SetFineScroll(0);
	}

	uint8_t* scr = tilebuf + (((wy * width) + wx)<<color_flag);
	for (uint8_t y = 0; y < wheight-1; y++)
	{
		for (uint8_t x = 0; x < wwidth; x++)
		{
//			*scr = *(scr+(width<<color_flag));
			*scr = read_screen(scr+(width<<color_flag));
			scr++;
			if (color_flag)
			{
//				*scr = *(scr+(width<<color_flag));
				*scr = read_screen(scr+(width<<color_flag));
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
	if (!tilebuf)
		return;

	uint8_t* scr = tilebuf + ((((wy+wheight-1) * width) + wx)<<color_flag);
	for (uint8_t y = wheight-1; y != (uint8_t)-1 ; --y)
	{
		for (uint8_t x = 0; x < wwidth; x++)
		{
//			*scr = *(scr-(width<<color_flag));
			*scr = read_screen(scr-(width<<color_flag));
			scr++;
			if (color_flag)
			{
//				*scr = *(scr-(width<<color_flag));
				*scr = read_screen(scr-(width<<color_flag));
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
	if (!tilebuf)
		return;

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
	VGAText_SetCursorX(cx);
	VGAText_SetCursorY(cy);
}

// Output a character (and handle control characters)
void VGA::PrintChar(int ch)
{
	if (!tilebuf)
		return;

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
	VGAText_SetCursorX(cx);
	VGAText_SetCursorY(cy);
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
