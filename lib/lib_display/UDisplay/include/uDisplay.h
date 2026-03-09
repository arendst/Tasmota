#ifndef _UDISP_
#define _UDISP_

#include <Adafruit_GFX.h>
#include <renderer.h>
#include <Wire.h>
#include <SPI.h>
#include "uDisplay_config.h"

#ifdef ESP32
#if __has_include("soc/soc_caps.h")
# include "soc/soc_caps.h"
#else
# error "No ESP capability header found"
#endif
#if (SOC_LCDCAM_I80_NUM_BUSES && !SOC_PARLIO_GROUPS)
  #define UDISPLAY_I80
  #include "uDisplay_I80_panel.h"
#endif

#if defined(SOC_LCD_RGB_SUPPORTED)
  #include "uDisplay_RGB_panel.h"
#endif
#if SOC_MIPI_DSI_SUPPORTED
    #include "uDisplay_DSI_panel.h"
#endif

#ifdef CONFIG_IDF_TARGET_ESP32S3
#define USE_ESP32_S3
#endif
#include "soc/gpio_periph.h"
#include <rom/gpio.h>
// #include "driver/spi_master.h"
#endif

#include "uDisplay_SPI_controller.h"
#include "uDisplay_I2C_panel.h"
#include "uDisplay_EPD_panel.h"
#include "uDisplay_SPI_panel.h"

// ===== Panel Config Union =====
// Union to hold any panel configuration type
// Only one config is active at a time based on interface type
union PanelConfigUnion {
    SPIPanelConfig spi;
    I2CPanelConfig i2c;
    EPDPanelConfig epd;
#ifdef UDISPLAY_I80
    I80PanelConfig i80;
#endif
#if SOC_LCD_RGB_SUPPORTED
    esp_lcd_rgb_panel_config_t rgb;  // ESP-IDF native config
#endif
#if SOC_MIPI_DSI_SUPPORTED
    DSIPanelConfig dsi;
#endif
};

enum {
  UT_RD,UT_RDM,UT_CP,UT_RTF,UT_MV,UT_MVB,UT_RT,UT_RTT,UT_RDW,UT_RDWM,UT_WR,UT_WRW,UT_CPR,UT_AND,UT_SCALE,UT_LIM,UT_DBG,UT_GSRT,UT_XPT,UT_CPM,UT_END
};

#define UDSP_WRITE_16 0xf0
#define UDSP_READ_DATA 0xf1
#define UDSP_READ_STATUS 0xf2


// Simple resistive touch pin mapping (I80 only):
// XP = data_pins_low[1], XM = cs_pin, YP = dc_pin, YM = data_pins_low[0]

#define _UDSP_I2C 1
#define _UDSP_SPI 2
#define _UDSP_PAR8 3
#define _UDSP_PAR16 4
#define _UDSP_RGB 5
#define _UDSP_DSI 6

#define UDISP1_WHITE 1
#define UDISP1_BLACK 0

// #define MAX_LUTS 5

#define DISPLAY_INIT_MODE 0
#define DISPLAY_INIT_PARTIAL 1
#define DISPLAY_INIT_FULL 2

typedef union {
  uint8_t data;
  struct {
    uint8_t bp_invert : 1;
    uint8_t bp_nopwm : 1;
    uint8_t nutu3 : 1;
    uint8_t nutu4 : 1;
    uint8_t nutu5 : 1;
    uint8_t nutu6 : 1;
    uint8_t nutu7 : 1;
    uint8_t nutu8 : 1;  
  };
} BP_MODE;


class uDisplay : public Renderer {
 public:
  uDisplay(char *);
  ~uDisplay(void);
  Renderer *Init(void);
  void DisplayInit(int8_t p,int8_t size,int8_t rot,int8_t font);
  void Updateframe();
  void DisplayOnff(int8_t on);
  void HandeBP(int8_t on);
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
  void pushColors(uint16_t *data, uint32_t len, boolean first);
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
    uint8_t *frame_buffer;
    // uint8_t *lut_full;  // MOVED to EPDPanelConfig.lut_full_data
    // uint8_t *lut_partial;  // MOVED to EPDPanelConfig.lut_partial_data
    // uint8_t *lut_array[MAX_LUTS];  // MOVED to EPDPanelConfig.lut_array_data
#if SOC_MIPI_DSI_SUPPORTED
    uint8_t dsp_cmds[1024]; // for DSI, does not hurt for ESP32
#else
    uint8_t dsp_cmds[256];
#endif
    char dname[16];

    SPIController *spiController;
    TwoWire *wire;
    UniversalPanel* universal_panel = nullptr;
    
    // ===== Panel Configuration Union =====
    // Heap-allocated union holding the active panel config
    // Allocated after parsing :H line, populated during INI parsing
    PanelConfigUnion* panel_config = nullptr;


    uint16_t x_addr_offs[4];
    uint16_t y_addr_offs[4];
    uint16_t splash_xp;
    uint16_t splash_yp;
    uint16_t fg_col;
    uint16_t bg_col;
    uint16_t gxs;
    uint16_t gys;
    uint16_t dimmer10_gamma;
    uint16_t seta_xp1;
    uint16_t seta_xp2;
    uint16_t seta_yp1;
    uint16_t seta_yp2;
    // uint16_t lutptime;  // MOVED to EPDPanelConfig.lut_partial_time
    // uint16_t lut3time;  // MOVED to EPDPanelConfig.update_time
    uint16_t lut_num;

    uint8_t bpp;
    uint8_t col_type;
    uint8_t interface;
    uint8_t i2caddr;
    uint8_t i2c_col_start;
    uint8_t i2c_col_end;
    uint8_t i2c_page_start;
    uint8_t i2c_page_end;
    uint16_t dsp_ncmds;
    uint8_t dsp_on;
    uint8_t dsp_off;
    uint8_t allcmd_mode;
    uint8_t splash_size;
    uint8_t dimmer8;
    uint8_t spi_speed;
    // uint8_t spi_nr;
    uint8_t rot[4];
    uint8_t rot_t[4];
    uint8_t madctrl;
    uint8_t startline;
    uint8_t saw_1;
    uint8_t saw_2;
    uint8_t saw_3;
    uint8_t cur_rot;
    uint8_t col_mode;
    uint8_t inv_on;
    uint8_t inv_off;
    uint8_t sa_mode;
    uint8_t dim_op;
    // uint8_t lutfsize;  // MOVED to EPDPanelConfig.lutfsize
    // uint8_t lutpsize;  // MOVED to EPDPanelConfig.lutpsize
    // uint8_t lut_siz_full;  // Local variable only
    // uint8_t lut_siz_partial;  // Local variable only
    // uint8_t epcoffs_full;  // MOVED to EPDPanelConfig.epcoffs_full
    // uint8_t epc_full_cnt;  // MOVED to EPDPanelConfig.epc_full_cnt
    // uint8_t epcoffs_part;  // MOVED to EPDPanelConfig.epcoffs_part
    // uint8_t epc_part_cnt;  // MOVED to EPDPanelConfig.epc_part_cnt
    // uint8_t lut_cnt[MAX_LUTS];  // MOVED to EPDPanelConfig.lut_cnt_data
    // uint8_t lut_cmd[MAX_LUTS];  // MOVED to EPDPanelConfig.lut_cmd
    // uint8_t lut_siz[MAX_LUTS];  // MOVED to EPDPanelConfig.lut_siz
    uint8_t ep_mode;
    // uint8_t ep_update_mode;  // MOVED to EPDPanel.update_mode
    uint8_t sspi;

    int8_t spec_init;
    int8_t wire_n;
    int8_t i2c_scl;
    int8_t i2c_sda;
    int8_t reset;
    int8_t splash_font;
    BP_MODE bp_mode;
    // int8_t spi_cs;
    // int8_t spi_clk;
    // int8_t spi_mosi;
    // int8_t spi_dc;
    int8_t bpanel; // pin
    // int8_t spi_miso;
    // int8_t busy_pin;  // MOVED to EPDPanelConfig.busy_pin (EPD-only)

    // int16_t lutftime;  // MOVED to EPDPanelConfig.lut_full_time
    int16_t rotmap_xmin;
    int16_t rotmap_xmax;
    int16_t rotmap_ymin;
    int16_t rotmap_ymax;

    void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
    void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
    uint32_t str2c(char **sp, char *vp, uint32_t len);
    void clearDisplay(void);

    void i2c_command(uint8_t val);

    uint8_t strlen_ln(char *str);
    int32_t next_val(char **sp);
    uint32_t next_hex(char **sp);
    void setAddrWindow_int(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    void pushColorsMono(uint16_t *data, uint32_t len, bool rgb16_swap = false);
    void delay_sync(int32_t time);
    void reset_pin(int32_t delayl, int32_t delayh);
    void delay_arg(uint32_t arg);

    void send_spi_cmds(uint16_t cmd_offset, uint16_t cmd_size);
    void send_spi_icmds(uint16_t cmd_size);
   

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
  int8_t ut_spi_cs = -1;
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
