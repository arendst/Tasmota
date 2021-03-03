
#ifndef RENDERER_H
#define RENDERER_H

#include <Adafruit_GFX.h>
#include "fonts.h"

#define BLACK 0
#define WHITE 1
#define INVERSE 2

#define MAX_INDEXCOLORS 32

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
  virtual void Begin(int16_t p1,int16_t p2,int16_t p3);
  virtual void Updateframe();
  virtual void dim(uint8_t contrast);
  virtual void pushColors(uint16_t *data, uint16_t len, boolean first);
  virtual void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
  virtual void invertDisplay(boolean i);
  virtual void reverseDisplay(boolean i);
  virtual void setScrollMargins(uint16_t top, uint16_t bottom);
  virtual void scrollTo(uint16_t y);
  void setDrawMode(uint8_t mode);
  uint8_t drawmode;
  virtual void FastString(uint16_t x,uint16_t y,uint16_t tcolor, const char* str);
  void setTextSize(uint8_t s);
private:
  void DrawCharAt(int16_t x, int16_t y, char ascii_char,int16_t colored);
  inline void drawFastVLineInternal(int16_t x, int16_t y, int16_t h, uint16_t color) __attribute__((always_inline));
  inline void drawFastHLineInternal(int16_t x, int16_t y, int16_t w, uint16_t color) __attribute__((always_inline));
  sFONT *selected_font;
  uint8_t font;
  uint8_t tsize = 1;
};

typedef union {
  uint8_t data;
  struct {
    uint8_t spare0 : 1;
    uint8_t spare1 : 1;
    uint8_t spare2 : 1;
    uint8_t slider : 1;
    uint8_t disable : 1;
    uint8_t on_off : 1;
    uint8_t is_pushbutton : 1;
    uint8_t is_virtual : 1;
  };
} TButton_State;


struct Slider {
  uint16_t xp;
  uint16_t yp;
  uint16_t xs;
  uint16_t ys;
  uint16_t nelem;
  uint16_t bgcol;
  uint16_t frcol;
  uint16_t barcol;
};

class VButton : public Adafruit_GFX_Button {
  public:
  TButton_State vpower;
  struct Slider spars;
  Renderer *rend;
  void xdrawButton(bool inverted);
  boolean didhit(int16_t x, int16_t y);
  uint16_t UpdateSlider(int16_t x, int16_t y);
  void SliderInit(Renderer *rend, uint16_t xp, uint16_t yp, uint16_t xs, uint16_t ys, uint16_t nelem, uint16_t bgcol, uint16_t frcol, uint16_t barcol);
};


#endif

/* END OF FILE */
