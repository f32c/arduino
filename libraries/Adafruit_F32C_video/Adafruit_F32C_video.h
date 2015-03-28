/*********************************************************************
This is a library for F32C framebuffer
*********************************************************************/

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Adafruit_GFX.h>

#define BLACK 0
#define WHITE 0xFFFF

#define F32C_VIDEOWIDTH                  512
#define F32C_VIDEOHEIGHT                 288

class Adafruit_F32C_video : public Adafruit_GFX {
 public:
  Adafruit_F32C_video(int8_t mode);

  void begin(void);

  void clearDisplay(void);
  void invertDisplay(uint8_t i);
  void display();

  void startscrollright(uint8_t start, uint8_t stop);
  void startscrollleft(uint8_t start, uint8_t stop);

  void startscrolldiagright(uint8_t start, uint8_t stop);
  void startscrolldiagleft(uint8_t start, uint8_t stop);
  void stopscroll(void);

  void dim(uint8_t contrast);

  void drawPixel(int16_t x, int16_t y, uint16_t color);
  void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
};
