#ifndef _UDISP_LVGL_
#define _UDISP_LVGL_

#include <Wire.h>
#include <SPI.h>

#define _UDSP_I2C 1
#define _UDSP_SPI 2

#define UDISP1_WHITE 1
#define UDISP1_BLACK 0

#define DISPLAY_INIT_MODE 0
#define DISPLAY_INIT_PARTIAL 1
#define DISPLAY_INIT_FULL 2

enum uColorType { uCOLOR_BW, uCOLOR_COLOR };

// Color definitions
#define UDISP_BLACK       0x0000      /*   0,   0,   0 */
#define UDISP_NAVY        0x000F      /*   0,   0, 128 */
#define UDISP_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define UDISP_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define UDISP_MAROON      0x7800      /* 128,   0,   0 */
#define UDISP_PURPLE      0x780F      /* 128,   0, 128 */
#define UDISP_OLIVE       0x7BE0      /* 128, 128,   0 */
#define UDISP_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define UDISP_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define UDISP_BLUE        0x001F      /*   0,   0, 255 */
#define UDISP_GREEN       0x07E0      /*   0, 255,   0 */
#define UDISP_CYAN        0x07FF      /*   0, 255, 255 */
#define UDISP_RED         0xF800      /* 255,   0,   0 */
#define UDISP_MAGENTA     0xF81F      /* 255,   0, 255 */
#define UDISP_YELLOW      0xFFE0      /* 255, 255,   0 */
#define UDISP_WHITE       0xFFFF      /* 255, 255, 255 */
#define UDISP_ORANGE      0xFD20      /* 255, 165,   0 */
#define UDISP_GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define UDISP_PINK        0xF81F

#ifdef ESP8266
#define PIN_OUT_SET 0x60000304
#define PIN_OUT_CLEAR 0x60000308
#define GPIO_SET(A) WRITE_PERI_REG( PIN_OUT_SET, 1 << A)
#define GPIO_CLR(A) WRITE_PERI_REG( PIN_OUT_CLEAR, 1 << A)
#else
#undef GPIO_SET
#define GPIO_SET(A) GPIO.out_w1ts = (1 << A)
#undef GPIO_CLR
#define GPIO_CLR(A) GPIO.out_w1tc = (1 << A)
#endif

#define SPI_BEGIN_TRANSACTION if (spi_nr <= 2) uspi->beginTransaction(spiSettings);
#define SPI_END_TRANSACTION if (spi_nr <= 2) uspi->endTransaction();
#define SPI_CS_LOW if (spi_cs >= 0) GPIO_CLR(spi_cs);
#define SPI_CS_HIGH if (spi_cs >= 0) GPIO_SET(spi_cs);
#define SPI_DC_LOW if (spi_dc >= 0) GPIO_CLR(spi_dc);
#define SPI_DC_HIGH if (spi_dc >= 0) GPIO_SET(spi_dc);

#define ESP32_PWM_CHANNEL 1

class uDisplay_lvgl {
 public:
  uDisplay_lvgl(char *);
  void Init(void);
  void DisplayInit(int8_t p,int8_t size,int8_t rot,int8_t font);
  void Updateframe();
  void DisplayOnff(int8_t on);
  // void Splash(void);
  char *devname(void);
  // uint16_t fgcol(void) const { return fg_col; };
  // uint16_t bgcol(void) const { return bg_col; };
  void dim(uint8_t dim);
  uint16_t GetColorFromIndex(uint8_t index);
  void setRotation(uint8_t m);
  void pushColors(uint16_t *data, uint16_t len, bool first);
  void TS_RotConvert(int16_t *x, int16_t *y);
  void invertDisplay(bool i);

  inline int32_t width(void) const { return gxs; }
  inline int32_t height(void) const { return gys; }
  inline uint8_t getRotation(void) const { return cur_rot; }

 public:
  void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
  void writePixels(int16_t x, int16_t y, int16_t w, int16_t h,
                   uint16_t *colors, uint32_t len);
 private:
   void drawPixel(int16_t x, int16_t y, uint16_t color);
   uint32_t str2c(char **sp, char *vp, uint32_t len);
   void i2c_command(uint8_t val);
   void spi_command(uint8_t val);
   void spi_command_one(uint8_t val);
   void spi_data8(uint8_t val);
   void spi_data16(uint16_t val);
   void spi_data32(uint32_t val);
   void write8(uint8_t val);
   void write9(uint8_t val, uint8_t dc);
   void hw_write9(uint8_t val, uint8_t dc);
   void write16(uint16_t val);
   void write32(uint32_t val);
   void spi_data9(uint8_t d, uint8_t dc);
   void WriteColor(uint16_t color);
   void SetLut(const unsigned char* lut);
   void DisplayFrame(void);
   void Updateframe_EPD();
   void SetFrameMemory(const unsigned char* image_buffer);
   void SetFrameMemory(const unsigned char* image_buffer, uint16_t x, uint16_t y, uint16_t image_width, uint16_t image_height);
   void SetMemoryArea(int x_start, int y_start, int x_end, int y_end);
   void SetMemoryPointer(int x, int y);
   void DrawAbsolutePixel(int x, int y, int16_t color);
   void drawPixel_EPD(int16_t x, int16_t y, uint16_t color);
   void fillRect_EPD(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
   void drawFastVLine_EPD(int16_t x, int16_t y, int16_t h, uint16_t color);
   void drawFastHLine_EPD(int16_t x, int16_t y, int16_t w, uint16_t color);
   void Init_EPD(int8_t p);
   void spi_command_EPD(uint8_t val);
   void spi_data8_EPD(uint8_t val);
   void ClearFrameMemory(unsigned char color);
   uint8_t strlen_ln(char *str);
   int32_t next_val(char **sp);
   uint32_t next_hex(char **sp);
   void setAddrWindow_int(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
   char dname[16];
   int8_t bpp;
   uint8_t interface;
   uint8_t i2caddr;
   int8_t i2c_scl;
   TwoWire *wire;
   int8_t i2c_sda;
   uint8_t i2c_col_start;
   uint8_t i2c_col_end;
   uint8_t i2c_page_start;
   uint8_t i2c_page_end;
   int8_t reset;
   uint8_t dsp_cmds[128];
   uint8_t dsp_ncmds;
   uint8_t dsp_on;
   uint8_t dsp_off;
  //  uint16_t splash_font;
  //  uint16_t splash_size;
  //  uint16_t splash_xp;
  //  uint16_t splash_yp;
  //  uint16_t fg_col;
  //  uint16_t bg_col;
   uint16_t gxs, _width;
   uint16_t gys, _height;
   int8_t spi_cs;
   int8_t spi_clk;
   int8_t spi_mosi;
   int8_t spi_dc;
   int8_t bpanel;
   int8_t spi_miso;
   uint8_t dimmer;
   SPIClass *uspi;
   uint8_t sspi;
   SPISettings spiSettings;
   uint8_t spi_speed;
   uint8_t spi_nr = 1;
   uint8_t madctrl;
   uint8_t startline;
   uint8_t rot[4];
   uint8_t rot_t[4];
   uint16_t x_addr_offs[4];
   uint16_t y_addr_offs[4];
   uint8_t saw_1;
   uint8_t saw_2;
   uint8_t saw_3;
   uint8_t cur_rot;
   uint8_t col_mode;
   uint8_t inv_on;
   uint8_t inv_off;
   uint8_t sa_mode;
   uint8_t dim_op;
   uint8_t lutfsize;
   uint8_t lutpsize;
   uint16_t lutftime;
   uint16_t lutptime;
   uint16_t lut3time;
   uint8_t ep_mode;
   uint8_t lut_full[64];
   uint8_t lut_partial[64];
};




#endif // USE_LVGL
