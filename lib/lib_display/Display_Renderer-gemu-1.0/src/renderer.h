
#ifndef RENDERER_H
#define RENDERER_H

#include <Adafruit_GFX.h>
#include "fonts.h"
#include "tasmota_options.h"

#define BLACK 0
#define WHITE 1
#define INVERSE 2

#define USE_EPD_FONTS
//#define USE_ALL_EPD_FONTS
//#define USE_GFX_FONTS
#define USE_TINY_FONT
#define USE_7SEG_FONT

#define MAX_INDEXCOLORS 32

#ifdef USE_DISPLAY_LVGL_ONLY
#undef USE_EPD_FONTS
#endif

// depends on GFX driver
// GFX patched
// a. in class GFX setCursor,setTextSize => virtual
// b. textcolor,textbgcolor => public;

typedef struct LVGL_PARAMS {
  uint16_t flushlines;
  union {
    uint8_t data;
    struct {
      uint8_t use_dma : 1;
      uint8_t swap_color : 1;
      uint8_t async_dma : 1;   // force DMA completion before returning, avoid conflict with other devices on same bus. If set you should make sure the display is the only device on the bus
      uint8_t busy_invert : 1;
      uint8_t invert_bw : 1;
      uint8_t resvd_3 : 1;
      uint8_t resvd_4 : 1;
      uint8_t resvd_5 : 1;
    };
  };
}LVGL_PARAMS;

typedef void (*pwr_cb)(uint8_t);
typedef void (*dim_cb)(uint8_t);

#define USE_GFX

#ifdef USE_GFX
class Renderer : public Adafruit_GFX {
#else
class Renderer {
#endif

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
  void SetRamfont(uint8_t *font);
  virtual void DisplayOnff(int8_t on);
  virtual void DisplayInit(int8_t p,int8_t size,int8_t rot,int8_t font);
  virtual void Begin(int16_t p1,int16_t p2,int16_t p3);
  virtual void Updateframe();
  virtual void dim(uint8_t contrast);   // input has range 0..15
  virtual void dim10(uint8_t contrast, uint16_t contrast_gamma);  // input has range 0..255, second arg has gamma correction for PWM with 10 bits resolution
  virtual void pushColors(uint16_t *data, uint16_t len, boolean first);
  virtual void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
  virtual void invertDisplay(boolean i);
  virtual void reverseDisplay(boolean i);
  virtual void setScrollMargins(uint16_t top, uint16_t bottom);
  virtual void scrollTo(uint16_t y);
  virtual void TS_RotConvert(int16_t *x, int16_t *y);
  virtual void SetPwrCB(pwr_cb cb);
  virtual void SetDimCB(dim_cb cb);
  virtual uint16_t fgcol(void);
  virtual uint16_t bgcol(void);
  virtual int8_t color_type(void);
  virtual void Splash(void);
  virtual void Sleep(void);
  virtual char *devname(void);
  virtual LVGL_PARAMS *lvgl_pars(void);
  virtual void ep_update_mode(uint8_t mode);
  virtual void ep_update_area(uint16_t xp, uint16_t yp, uint16_t width, uint16_t height, uint8_t mode);
  virtual uint32_t get_sr_touch(uint32_t xp, uint32_t xm, uint32_t yp, uint32_t ym);
  virtual bool utouch_Init(char **);
  virtual uint16_t touched(void);
  virtual int16_t getPoint_x();
  virtual int16_t getPoint_y();

  void setDrawMode(uint8_t mode);
  uint8_t drawmode;
  virtual void FastString(uint16_t x,uint16_t y,uint16_t tcolor, const char* str);
  void setTextSize(uint8_t s);
  virtual uint8_t *allocate_framebuffer(uint32_t size);
  pwr_cb pwr_cbp = 0;
  dim_cb dim_cbp = 0;
  LVGL_PARAMS lvgl_param;
  int8_t disp_bpp;
private:
  void DrawCharAt(int16_t x, int16_t y, char ascii_char,int16_t colored);
  inline void drawFastVLineInternal(int16_t x, int16_t y, int16_t h, uint16_t color) __attribute__((always_inline));
  inline void drawFastHLineInternal(int16_t x, int16_t y, int16_t w, uint16_t color) __attribute__((always_inline));
  sFONT *selected_font;
  uint8_t font;
  uint8_t tsize = 1;
  GFXfont *ramfont = 0;
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

// #ifndef USE_DISPLAY_LVGL_ONLY
class VButton : public Adafruit_GFX_Button {
  public:
  TButton_State vpower;
  struct Slider spars;
  Renderer *rend;
  void xdrawButton(bool inverted);
  boolean didhit(int16_t x, int16_t y);
  uint16_t UpdateSlider(int16_t x, int16_t y);
  void SliderInit(Renderer *rend, uint16_t xp, uint16_t yp, uint16_t xs, uint16_t ys, uint16_t nelem, uint16_t bgcol, uint16_t frcol, uint16_t barcol);
  void xinitButtonUL(Renderer *renderer, int16_t gxp, int16_t gyp, uint16_t gxs, uint16_t gys, uint16_t outline,\
    uint16_t fill, uint16_t textcolor , char *label, uint8_t textsize);
};
// #endif // USE_DISPLAY_LVGL_ONLY


#endif

/* END OF FILE */
