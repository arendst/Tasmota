#ifndef _UDISP_
#define _UDISP_

#include <Adafruit_GFX.h>
#include <renderer.h>
#include <Wire.h>
#include <SPI.h>
#ifdef ESP32
#include "driver/spi_master.h"
#endif

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
#define GPIO_CLR_SLOW(A) digitalWrite(A, LOW)
#define GPIO_SET_SLOW(A) digitalWrite(A, HIGH)
#else
#undef GPIO_SET
#undef GPIO_CLR
#undef GPIO_SET_SLOW
#undef GPIO_CLR_SLOW
#if CONFIG_IDF_TARGET_ESP32C3
#define GPIO_CLR(A) GPIO.out_w1tc.val = (1 << A)
#define GPIO_SET(A) GPIO.out_w1ts.val = (1 << A)
#else // plain ESP32
#define GPIO_CLR(A) GPIO.out_w1tc = (1 << A)
#define GPIO_SET(A) GPIO.out_w1ts = (1 << A)
#endif
#define GPIO_CLR_SLOW(A) digitalWrite(A, LOW)
#define GPIO_SET_SLOW(A) digitalWrite(A, HIGH)

#endif

#define SPI_BEGIN_TRANSACTION if (spi_nr <= 2) beginTransaction(spiSettings);
#define SPI_END_TRANSACTION if (spi_nr <= 2) endTransaction();

#define SPI_CS_LOW if (spi_cs >= 0) GPIO_CLR_SLOW(spi_cs);
#define SPI_CS_HIGH if (spi_cs >= 0) GPIO_SET_SLOW(spi_cs);
#define SPI_DC_LOW if (spi_dc >= 0) GPIO_CLR_SLOW(spi_dc);
#define SPI_DC_HIGH if (spi_dc >= 0) GPIO_SET_SLOW(spi_dc);


#define ESP32_PWM_CHANNEL 1

#define LUTMAXSIZE 64

class uDisplay : public Renderer {
 public:
  uDisplay(char *);
  ~uDisplay(void);
  Renderer *Init(void);
  void DisplayInit(int8_t p,int8_t size,int8_t rot,int8_t font);
  void Updateframe();
  void DisplayOnff(int8_t on);
  void Splash(void);
  char *devname(void);
  uint16_t fgcol(void);
  uint16_t bgcol(void);
  int8_t color_type(void);
//   void dim(uint8_t dim);            // original version with 4 bits resolution 0..15
  virtual void dim8(uint8_t dim, uint8_t dim_gamma);           // dimmer with 8 bits resolution, 0..255. Gamma correction must be done by caller
  uint16_t GetColorFromIndex(uint8_t index);
  void setRotation(uint8_t m);
  void fillScreen(uint16_t color);
  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
  void pushColors(uint16_t *data, uint16_t len, boolean first);
  void TS_RotConvert(int16_t *x, int16_t *y);
  void invertDisplay(boolean i);
  void SetPwrCB(pwr_cb cb) { pwr_cbp = cb; };
  void SetDimCB(dim_cb cb) { dim_cbp = cb; };

 private:
   void beginTransaction(SPISettings s);
   void endTransaction(void);
   void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
   void drawPixel(int16_t x, int16_t y, uint16_t color);
   void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
   void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
   uint32_t str2c(char **sp, char *vp, uint32_t len);
   void i2c_command(uint8_t val);
   void spi_command_one(uint8_t val);
   void spi_command(uint8_t val);
   void spi_data8(uint8_t val);
   void spi_data16(uint16_t val);
   void spi_data32(uint32_t val);
   void write8(uint8_t val);
   void write8_slow(uint8_t val);
   void write9(uint8_t val, uint8_t dc);
   void write9_slow(uint8_t val, uint8_t dc);
   void hw_write9(uint8_t val, uint8_t dc);
   void write16(uint16_t val);
   void write32(uint32_t val);
   void spi_data9(uint8_t d, uint8_t dc);
   void WriteColor(uint16_t color);
   void SetLut(const unsigned char* lut);
   void SetLuts(void);
   void DisplayFrame_29(void);
   void Updateframe_EPD();
   //void DisplayFrame_42(const unsigned char* frame_buffer);
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
   //void SetPartialWindow_42(uint8_t* frame_buffer, int16_t x, int16_t y, int16_t w, int16_t l, int16_t dtm);
   void ClearFrameMemory(unsigned char color);
   void ClearFrame_42(void);
   void DisplayFrame_42(void);
   uint8_t strlen_ln(char *str);
   int32_t next_val(char **sp);
   uint32_t next_hex(char **sp);
   void setAddrWindow_int(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
   char dname[16];
   int8_t bpp;
   uint8_t col_type;
   uint8_t interface;
   uint8_t i2caddr;
   int8_t i2c_scl;
   TwoWire *wire;
   int8_t wire_n;
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
   uint8_t allcmd_mode;
   int8_t splash_font;
   uint8_t splash_size;
   uint16_t splash_xp;
   uint16_t splash_yp;
   uint16_t fg_col;
   uint16_t bg_col;
   uint16_t gxs;
   uint16_t gys;
   int8_t spi_cs;
   int8_t spi_clk;
   int8_t spi_mosi;
   int8_t spi_dc;
   int8_t bpanel;          // backbanel GPIO, -1 if none
   int8_t spi_miso;
   uint8_t dimmer8;        // 8 bits resolution, 0..255
   uint8_t dimmer8_gamma;  // 8 bits resolution, 0..255, gamma corrected
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
   uint16_t lut_num;
   uint8_t ep_mode;
   uint8_t lut_full[LUTMAXSIZE];
   uint8_t lut_partial[LUTMAXSIZE];
   uint8_t lut_array[LUTMAXSIZE][5];
   uint8_t lut_cnt[5];
   uint8_t lut_cmd[5];
   uint16_t seta_xp1;
   uint16_t seta_xp2;
   uint16_t seta_yp1;
   uint16_t seta_yp2;
   int16_t rotmap_xmin;
   int16_t rotmap_xmax;
   int16_t rotmap_ymin;
   int16_t rotmap_ymax;
   void pushColorsMono(uint16_t *data, uint16_t len, bool rgb16_swap = false);
#ifdef ESP32
   // dma section
   bool DMA_Enabled = false;
   uint8_t  spiBusyCheck = 0;
   spi_transaction_t trans;
   spi_device_handle_t dmaHAL;
   spi_host_device_t spi_host = VSPI_HOST;
   // spi_host_device_t spi_host = VSPI_HOST;
   bool initDMA(bool ctrl_cs);
   void deInitDMA(void);
   bool dmaBusy(void);
   void dmaWait(void);
   void pushPixelsDMA(uint16_t* image, uint32_t len);
   void pushPixels3DMA(uint8_t* image, uint32_t len);
#endif // ESP32
};



#endif // _UDISP_
