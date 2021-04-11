#ifndef _UDISP_
#define _UDISP_

#include <Adafruit_GFX.h>
#include <renderer.h>

#define _UDSP_I2C 1
#define _UDSP_SPI 2

#define UDISP1_WHITE 1
#define UDISP1_BLACK 0


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

#define SPI_BEGIN_TRANSACTION uspi->beginTransaction(spiSettings);
#define SPI_END_TRANSACTION uspi->endTransaction();
#define SPI_CS_LOW if (spi_cs >= 0) digitalWrite(spi_cs, LOW);
#define SPI_CS_HIGH if (spi_cs >= 0) digitalWrite(spi_cs, HIGH);
#define SPI_DC_LOW if (spi_dc >= 0) digitalWrite(spi_dc, LOW);
#define SPI_DC_HIGH if (spi_dc >= 0) digitalWrite(spi_dc, HIGH);

#define ESP32_PWM_CHANNEL 1

class uDisplay : public Renderer {
 public:
  uDisplay(char *);
  Renderer *Init(void);
  void DisplayInit(int8_t p,int8_t size,int8_t rot,int8_t font);
  void Updateframe();
  void DisplayOnff(int8_t on);
  void Splash(void);
  char *devname(void);
  uint16_t fgcol(void) const { return fg_col; };
  uint16_t bgcol(void) const { return bg_col; };
  void dim(uint8_t dim);
  uint16_t GetColorFromIndex(uint8_t index);
  void setRotation(uint8_t m);
  void fillScreen(uint16_t color);
  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
  void pushColors(uint16_t *data, uint16_t len, boolean first);

 private:
   void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
   void drawPixel(int16_t x, int16_t y, uint16_t color);
   void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
   void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
   uint32_t str2c(char **sp, char *vp, uint32_t len);
   void i2c_command(uint8_t val);
   void spi_command(uint8_t val);
   uint8_t strlen_ln(char *str);
   int32_t next_val(char **sp);
   uint32_t next_hex(char **sp);
   void setAddrWindow_int(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
   char dname[16];
   int8_t bpp;
   uint8_t interface;
   uint8_t i2caddr;
   int8_t i2c_scl;
   int8_t i2c_sda;
   int8_t reset;
   uint8_t dsp_cmds[128];
   uint8_t dsp_ncmds;
   uint8_t dsp_on;
   uint8_t dsp_off;
   uint16_t splash_font;
   uint16_t splash_size;
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
   int8_t bpanel;
   int8_t spi_miso;
   uint8_t dimmer;
   SPIClass *uspi;
   SPISettings spiSettings;
   uint32_t spi_speed;
   uint8_t spi_nr = 1;
   uint8_t rot_0;
   uint8_t rot_1;
   uint8_t rot_2;
   uint8_t rot_3;
   uint8_t saw_1;
   uint8_t saw_2;
   uint8_t saw_3;
   uint8_t flags;
};

#endif // _UDISP_
