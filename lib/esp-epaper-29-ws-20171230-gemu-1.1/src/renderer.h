
#ifndef RENDERER_H
#define RENDERER_H

#include <Adafruit_GFX.h>
#include "fonts.h"

#define BLACK 0
#define WHITE 1
#define INVERSE 2

// depends on GFX driver
// GFX patched
// a. in class GFX setCursor,setTextSize => virtual
// b. textcolor,textbgcolor => public;


class Renderer : public Adafruit_GFX {
  //Paint(unsigned char* image, int width, int height);
  //~Renderer();
public:
  Renderer(int16_t x, int16_t y);
  void setTextFont(uint8_t f);
  void clearDisplay(void);
  virtual void DrawStringAt(int16_t x, int16_t y, const char* text,uint16_t colored,uint8_t flag);
  virtual void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
  virtual void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
  virtual void drawPixel(int16_t x, int16_t y, uint16_t color);
  virtual uint16_t GetColorFromIndex(uint8_t index);

  virtual void DisplayOnff(int8_t on);
  virtual void DisplayInit(int8_t p,int8_t size,int8_t rot,int8_t font);
  virtual int16_t Begin(int16_t p1,int16_t p2,int16_t p3);
  virtual void Updateframe();
  virtual void dim(uint8_t contrast);
  virtual void pushColors(uint16_t *data, uint8_t len, boolean first);
  virtual void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
  void setDrawMode(uint8_t mode);
  uint8_t drawmode;
  virtual void FastString(uint16_t x,uint16_t y,uint16_t tcolor, const char* str);
private:
  void DrawCharAt(int16_t x, int16_t y, char ascii_char,int16_t colored);
  inline void drawFastVLineInternal(int16_t x, int16_t y, int16_t h, uint16_t color) __attribute__((always_inline));
  inline void drawFastHLineInternal(int16_t x, int16_t y, int16_t w, uint16_t color) __attribute__((always_inline));
  sFONT *selected_font;
  uint8_t font;
};

class VButton : public Adafruit_GFX_Button {
  public:
  uint8_t vpower;
  void xdrawButton(bool inverted);
};


#endif

/* END OF FILE */
