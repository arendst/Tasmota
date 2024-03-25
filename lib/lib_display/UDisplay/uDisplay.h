#ifndef _UDISP_
#define _UDISP_

#include <Adafruit_GFX.h>
#include <renderer.h>
#include <Wire.h>
#include <SPI.h>

#ifdef ESP32
#ifdef CONFIG_IDF_TARGET_ESP32S3
#define USE_ESP32_S3
#endif
#include "driver/spi_master.h"
#if ESP_IDF_VERSION_MAJOR >= 5
#include "soc/gpio_periph.h"
#include <rom/gpio.h>
#endif // ESP_IDF_VERSION_MAJOR >= 5
#endif

enum {
  UT_RD,UT_RDM,UT_CP,UT_RTF,UT_MV,UT_MVB,UT_RT,UT_RTT,UT_RDW,UT_RDWM,UT_WR,UT_WRW,UT_CPR,UT_AND,UT_SCALE,UT_LIM,UT_DBG,UT_GSRT,UT_XPT,UT_CPM,UT_END
};

#define RA8876_DATA_WRITE  0x80
#define RA8876_DATA_READ   0xC0
#define RA8876_CMD_WRITE   0x00
#define RA8876_STATUS_READ 0x40

#define UDSP_WRITE_16 0xf0
#define UDSP_READ_DATA 0xf1
#define UDSP_READ_STATUS 0xf2


#define SIMPLERS_XP par_dbl[1]
#define SIMPLERS_XM par_cs
#define SIMPLERS_YP par_rs
#define SIMPLERS_YM par_dbl[0]

#ifdef USE_ESP32_S3
#include <esp_lcd_panel_io.h>
#include "esp_private/gdma.h"
#include <hal/gpio_ll.h>
#include <hal/lcd_hal.h>
#include <soc/lcd_cam_reg.h>
#include <soc/lcd_cam_struct.h>
static inline volatile uint32_t* get_gpio_hi_reg(int_fast8_t pin) { return (pin & 32) ? &GPIO.out1_w1ts.val : &GPIO.out_w1ts; }
//static inline volatile uint32_t* get_gpio_hi_reg(int_fast8_t pin) { return (volatile uint32_t*)((pin & 32) ? 0x60004014 : 0x60004008) ; } // workaround Eratta
static inline volatile uint32_t* get_gpio_lo_reg(int_fast8_t pin) { return (pin & 32) ? &GPIO.out1_w1tc.val : &GPIO.out_w1tc; }
//static inline volatile uint32_t* get_gpio_lo_reg(int_fast8_t pin) { return (volatile uint32_t*)((pin & 32) ? 0x60004018 : 0x6000400C) ; }
static inline bool gpio_in(int_fast8_t pin) { return ((pin & 32) ? GPIO.in1.data : GPIO.in) & (1 << (pin & 31)); }
static inline void gpio_hi(int_fast8_t pin) { if (pin >= 0) *get_gpio_hi_reg(pin) = 1 << (pin & 31); } // ESP_LOGI("LGFX", "gpio_hi: %d", pin); }
static inline void gpio_lo(int_fast8_t pin) { if (pin >= 0) *get_gpio_lo_reg(pin) = 1 << (pin & 31); } // ESP_LOGI("LGFX", "gpio_lo: %d", pin); }
#include "esp_lcd_panel_interface.h"
#include "esp_lcd_panel_rgb.h"
#include "esp_pm.h"
#include "esp_lcd_panel_ops.h"
#include <hal/dma_types.h>
#include <rom/cache.h>
#if ESP_IDF_VERSION_MAJOR >= 5
#include "esp_rom_lldesc.h"
#endif // ESP_IDF_VERSION_MAJOR >= 5
#endif // USE_ESP32_S3

#define _UDSP_I2C 1
#define _UDSP_SPI 2
#define _UDSP_PAR8 3
#define _UDSP_PAR16 4
#define _UDSP_RGB 5

#define UDISP1_WHITE 1
#define UDISP1_BLACK 0

#define MAX_LUTS 5

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

#if CONFIG_IDF_TARGET_ESP32C2 || CONFIG_IDF_TARGET_ESP32C3 || CONFIG_IDF_TARGET_ESP32C6
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


#if defined(USE_ESP32_S3) && ESP_IDF_VERSION_MAJOR < 5
struct esp_lcd_i80_bus_t {
    int bus_id;            // Bus ID, index from 0
    portMUX_TYPE spinlock; // spinlock used to protect i80 bus members(hal, device_list, cur_trans)
    lcd_hal_context_t hal; // Hal object
    size_t bus_width;      // Number of data lines
    intr_handle_t intr;    // LCD peripheral interrupt handle
    void* pm_lock; // Power management lock
    size_t num_dma_nodes;  // Number of DMA descriptors
    uint8_t *format_buffer;  // The driver allocates an internal buffer for DMA to do data format transformer
    size_t resolution_hz;    // LCD_CLK resolution, determined by selected clock source
    gdma_channel_handle_t dma_chan; // DMA channel handle
};

// extract from esp-idf esp_lcd_rgb_panel.c
struct esp_rgb_panel_t
{
  esp_lcd_panel_t base;                                        // Base class of generic lcd panel
  int panel_id;                                                // LCD panel ID
  lcd_hal_context_t hal;                                       // Hal layer object
  size_t data_width;                                           // Number of data lines (e.g. for RGB565, the data width is 16)
  size_t sram_trans_align;                                     // Alignment for framebuffer that allocated in SRAM
  size_t psram_trans_align;                                    // Alignment for framebuffer that allocated in PSRAM
  int disp_gpio_num;                                           // Display control GPIO, which is used to perform action like "disp_off"
  intr_handle_t intr;                                          // LCD peripheral interrupt handle
  esp_pm_lock_handle_t pm_lock;                                // Power management lock
  size_t num_dma_nodes;                                        // Number of DMA descriptors that used to carry the frame buffer
  uint8_t *fb;                                                 // Frame buffer
  size_t fb_size;                                              // Size of frame buffer
  int data_gpio_nums[SOC_LCD_RGB_DATA_WIDTH];                  // GPIOs used for data lines, we keep these GPIOs for action like "invert_color"
  size_t resolution_hz;                                        // Peripheral clock resolution
  esp_lcd_rgb_timing_t timings;                                // RGB timing parameters (e.g. pclk, sync pulse, porch width)
  gdma_channel_handle_t dma_chan;                              // DMA channel handle
#if ESP_IDF_VERSION_MAJOR < 5
  esp_lcd_rgb_panel_frame_trans_done_cb_t on_frame_trans_done; // Callback, invoked after frame trans done
#endif // ESP_IDF_VERSION_MAJOR < 5
  void *user_ctx;                                              // Reserved user's data of callback functions
  int x_gap;                                                   // Extra gap in x coordinate, it's used when calculate the flush window
  int y_gap;                                                   // Extra gap in y coordinate, it's used when calculate the flush window
  struct
  {
    unsigned int disp_en_level : 1; // The level which can turn on the screen by `disp_gpio_num`
    unsigned int stream_mode : 1;   // If set, the LCD transfers data continuously, otherwise, it stops refreshing the LCD when transaction done
    unsigned int fb_in_psram : 1;   // Whether the frame buffer is in PSRAM
  } flags;
  dma_descriptor_t dma_nodes[]; // DMA descriptor pool of size `num_dma_nodes`
};
#endif //USE_ESP32_S3 && ESP_IDF_VERSION_MAJOR < 5


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
  virtual void dim10(uint8_t dim, uint16_t dim_gamma);           // dimmer with 8 bits resolution, 0..255. Gamma correction must be done by caller with 10 bits resolution
  uint16_t GetColorFromIndex(uint8_t index);
  void setRotation(uint8_t m);
  void fillScreen(uint16_t color);
  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
  void pushColors(uint16_t *data, uint16_t len, boolean first);
  void TS_RotConvert(int16_t *x, int16_t *y);
  void invertDisplay(boolean i);
  void SetPwrCB(pwr_cb cb) { pwr_cbp = cb; };
  void SetDimCB(dim_cb cb) { dim_cbp = cb; };
#ifdef USE_UNIVERSAL_TOUCH
// universal touch driver
  bool utouch_Init(char **name);
  uint16_t touched(void);
  int16_t getPoint_x();
  int16_t getPoint_y();
#endif // USE_UNIVERSAL_TOUCH

 private:
   void beginTransaction(SPISettings s);
   void endTransaction(void);
   void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
   void drawPixel(int16_t x, int16_t y, uint16_t color);
   void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
   void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
   uint32_t str2c(char **sp, char *vp, uint32_t len);
   void i2c_command(uint8_t val);
   void ulcd_command_one(uint8_t val);
   void ulcd_command(uint8_t val);
   void ulcd_data8(uint8_t val);
   void ulcd_data16(uint16_t val);
   void ulcd_data32(uint32_t val);
   void write8(uint8_t val);
   void write8_slow(uint8_t val);
   void write9(uint8_t val, uint8_t dc);
   void write9_slow(uint8_t val, uint8_t dc);
   void hw_write9(uint8_t val, uint8_t dc);
   void write16(uint16_t val);
   void write32(uint32_t val);
   void spi_data9(uint8_t d, uint8_t dc);
   uint8_t readData(void);
   uint8_t readStatus(void);
   uint8_t writeReg16(uint8_t reg, uint16_t wval);
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
   int8_t spec_init;
   TwoWire *wire;
   int8_t wire_n;
   int8_t i2c_sda;
   uint8_t i2c_col_start;
   uint8_t i2c_col_end;
   uint8_t i2c_page_start;
   uint8_t i2c_page_end;
   int8_t reset;
   uint8_t dsp_cmds[256];
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
   int8_t bpmode;
   int8_t spi_cs;
   int8_t spi_clk;
   int8_t spi_mosi;
   int8_t spi_dc;
   int8_t bpanel;          // backbanel GPIO, -1 if none
   int8_t spi_miso;
   uint8_t dimmer8;        // 8 bits resolution, 0..255
   uint16_t dimmer10_gamma;  // 10 bits resolution, 0..1023, gamma corrected
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
   int16_t lutftime;
   int8_t busy_pin;
   uint16_t lutptime;
   uint16_t lut3time;
   uint16_t lut_num;
   uint8_t ep_mode;
   uint8_t ep_update_mode;
   uint8_t *lut_full;
   uint8_t lut_siz_full;
   uint8_t *lut_partial;
   uint8_t lut_siz_partial;
   uint8_t *frame_buffer;

   uint8_t epcoffs_full;
   uint8_t epc_full_cnt;
   uint8_t epcoffs_part;
   uint8_t epc_part_cnt;

   uint8_t *lut_array[MAX_LUTS];
   uint8_t lut_cnt[MAX_LUTS];
   uint8_t lut_cmd[MAX_LUTS];
   uint8_t lut_siz[MAX_LUTS];
   uint16_t seta_xp1;
   uint16_t seta_xp2;
   uint16_t seta_yp1;
   uint16_t seta_yp2;
   int16_t rotmap_xmin;
   int16_t rotmap_xmax;
   int16_t rotmap_ymin;
   int16_t rotmap_ymax;
   void pushColorsMono(uint16_t *data, uint16_t len, bool rgb16_swap = false);
   void delay_sync(int32_t time);
   void reset_pin(int32_t delayl, int32_t delayh);
   void delay_arg(uint32_t arg);
   void Send_EP_Data(void);
   void send_spi_cmds(uint16_t cmd_offset, uint16_t cmd_size);
   void send_spi_icmds(uint16_t cmd_size);
   

#ifdef USE_ESP32_S3
   int8_t par_cs;
   int8_t par_rs;
   int8_t par_wr;
   int8_t par_rd;

   int8_t par_dbl[8];
   int8_t par_dbh[8];

   int8_t de;
   int8_t vsync;
   int8_t hsync;
   int8_t pclk;

   uint16_t hsync_polarity;
   uint16_t hsync_front_porch;
   uint16_t hsync_pulse_width;
   uint16_t hsync_back_porch;
   uint16_t vsync_polarity;
   uint16_t vsync_front_porch;
   uint16_t vsync_pulse_width;
   uint16_t vsync_back_porch;
   uint16_t pclk_active_neg;

   esp_lcd_panel_handle_t _panel_handle = NULL;
#if ESP_IDF_VERSION_MAJOR < 5
   esp_rgb_panel_t *_rgb_panel;
#endif //ESP_IDF_VERSION_MAJOR < 5
   uint16_t *rgb_fb;


   esp_lcd_i80_bus_handle_t _i80_bus = nullptr;
   gdma_channel_handle_t _dma_chan;
   lldesc_t *_dmadesc = nullptr;
   uint32_t _dmadesc_size = 0;
   uint32_t _clock_reg_value;
   void calcClockDiv(uint32_t* div_a, uint32_t* div_b, uint32_t* div_n, uint32_t* clkcnt, uint32_t baseClock, uint32_t targetFreq);
   void _alloc_dmadesc(size_t len);
   void _setup_dma_desc_links(const uint8_t *data, int32_t len);
   void pb_beginTransaction(void);
   void pb_endTransaction(void);
   void pb_wait(void);
   bool pb_busy(void);
   void _pb_init_pin(bool);
   bool pb_writeCommand(uint32_t data, uint_fast8_t bit_length);
   void pb_writeData(uint32_t data, uint_fast8_t bit_length);
   void pb_pushPixels(uint16_t* data, uint32_t length, bool swap_bytes, bool use_dma);
   void pb_writeBytes(const uint8_t* data, uint32_t length, bool use_dma);
   void _send_align_data(void);
   volatile lcd_cam_dev_t* _dev;
   uint32_t* _cache_flip;
   static constexpr size_t CACHE_SIZE = 256;
   uint32_t _cache[2][CACHE_SIZE / sizeof(uint32_t)];
   bool _has_align_data;
   uint8_t _align_data;
   void cs_control(bool level);
   uint32_t get_sr_touch(uint32_t xp, uint32_t xm, uint32_t yp, uint32_t ym);
   void drawPixel_RGB(int16_t x, int16_t y, uint16_t color);
#endif

#ifdef ESP32
   // dma section
   bool DMA_Enabled = false;
   uint8_t  spiBusyCheck = 0;
   spi_transaction_t trans;
   spi_device_handle_t dmaHAL;
   spi_host_device_t spi_host = VSPI_HOST;
   // spi_host_device_t spi_host = VSPI_HOST;
   bool initDMA(int32_t ctrl_cs);
   void deInitDMA(void);
   bool dmaBusy(void);
   void dmaWait(void);
   void pushPixelsDMA(uint16_t* image, uint32_t len);
   void pushPixels3DMA(uint8_t* image, uint32_t len);
#endif // ESP32

#ifdef USE_UNIVERSAL_TOUCH
// universal touch driver
  void ut_trans(char **sp, uint8_t **ut_code);
  int16_t ut_execute(uint8_t *ut_code);
  uint32_t ut_par(char **cp, uint32_t mode);
  uint8_t *ut_rd(uint8_t *io, uint32_t len, uint32_t amode);
  uint8_t *ut_wr(uint8_t *io, uint32_t amode);
  uint16_t ut_XPT2046(uint16_t zh);
  int16_t besttwoavg( int16_t x , int16_t y , int16_t z );

  uint8_t ut_array[16];
  uint8_t ut_i2caddr;
  uint8_t ut_spi_cs = -1;
  int8_t ut_reset = -1;
  int8_t ut_irq = -1;
  uint8_t ut_spi_nr;
  TwoWire *ut_wire = nullptr;;
  SPIClass *ut_spi = nullptr;;
  SPISettings ut_spiSettings;
  char ut_name[8];
  uint8_t *ut_init_code = nullptr;
  uint8_t *ut_touch_code = nullptr;
  uint8_t *ut_getx_code = nullptr;
  uint8_t *ut_gety_code = nullptr;

#endif // USE_UNIVERSAL_TOUCH
};



#endif // _UDISP_
