#include "uDisplay.h"
#include "uDisplay_config.h"

#ifdef USE_ESP32_S3
static inline volatile uint32_t* get_gpio_hi_reg(int_fast8_t pin) { return (pin & 32) ? &GPIO.out1_w1ts.val : &GPIO.out_w1ts; }
//static inline volatile uint32_t* get_gpio_hi_reg(int_fast8_t pin) { return (volatile uint32_t*)((pin & 32) ? 0x60004014 : 0x60004008) ; } // workaround Eratta
static inline volatile uint32_t* get_gpio_lo_reg(int_fast8_t pin) { return (pin & 32) ? &GPIO.out1_w1tc.val : &GPIO.out_w1tc; }
//static inline volatile uint32_t* get_gpio_lo_reg(int_fast8_t pin) { return (volatile uint32_t*)((pin & 32) ? 0x60004018 : 0x6000400C) ; }
static inline bool gpio_in(int_fast8_t pin) { return ((pin & 32) ? GPIO.in1.data : GPIO.in) & (1 << (pin & 31)); }
static inline void gpio_hi(int_fast8_t pin) { if (pin >= 0) *get_gpio_hi_reg(pin) = 1 << (pin & 31); } // ESP_LOGI("LGFX", "gpio_hi: %d", pin); }
static inline void gpio_lo(int_fast8_t pin) { if (pin >= 0) *get_gpio_lo_reg(pin) = 1 << (pin & 31); } // ESP_LOGI("LGFX", "gpio_lo: %d", pin); }

// ===== ESP32-S3 Clock Calculation =====

void uDisplay::calcClockDiv(uint32_t* div_a, uint32_t* div_b, uint32_t* div_n, uint32_t* clkcnt, uint32_t baseClock, uint32_t targetFreq) {
    uint32_t diff = INT32_MAX;
    *div_n = 256;
    *div_a = 63;
    *div_b = 62;
    *clkcnt = 64;
    uint32_t start_cnt = std::min<uint32_t>(64u, (baseClock / (targetFreq * 2) + 1));
    uint32_t end_cnt = std::max<uint32_t>(2u, baseClock / 256u / targetFreq);
    if (start_cnt <= 2) { end_cnt = 1; }
    for (uint32_t cnt = start_cnt; diff && cnt >= end_cnt; --cnt)
    {
      float fdiv = (float)baseClock / cnt / targetFreq;
      uint32_t n = std::max<uint32_t>(2u, (uint32_t)fdiv);
      fdiv -= n;

      for (uint32_t a = 63; diff && a > 0; --a)
      {
        uint32_t b = roundf(fdiv * a);
        if (a == b && n == 256) {
          break;
        }
        uint32_t freq = baseClock / ((n * cnt) + (float)(b * cnt) / (float)a);
        uint32_t d = abs((int)targetFreq - (int)freq);
        if (diff <= d) { continue; }
        diff = d;
        *clkcnt = cnt;
        *div_n = n;
        *div_b = b;
        *div_a = a;
        if (b == 0 || a == b) {
          break;
        }
      }
    }
    if (*div_a == *div_b)
    {
        *div_b = 0;
        *div_n += 1;
    }
}

// ===== ESP32-S3 DMA Descriptor Management =====

void uDisplay::_alloc_dmadesc(size_t len) {
    if (_dmadesc) heap_caps_free(_dmadesc);
    _dmadesc_size = len;
    _dmadesc = (lldesc_t*)heap_caps_malloc(sizeof(lldesc_t) * len, MALLOC_CAP_DMA);
}

void uDisplay::_setup_dma_desc_links(const uint8_t *data, int32_t len) {
    static constexpr size_t MAX_DMA_LEN = (4096-4);
}

// ===== ESP32-S3 Pin Control =====

void uDisplay::cs_control(bool level) {
    auto pin = par_cs;
    if (pin < 0) return;
    if (level) {
      gpio_hi(pin);
    }
    else {
      gpio_lo(pin);
    }
}

void uDisplay::_pb_init_pin(bool read) {
    if (read) {
      if (interface == _UDSP_PAR8) {
        for (size_t i = 0; i < 8; ++i) {
          gpio_ll_output_disable(&GPIO, (gpio_num_t)par_dbl[i]);
        }
      } else {
        for (size_t i = 0; i < 8; ++i) {
          gpio_ll_output_disable(&GPIO, (gpio_num_t)par_dbl[i]);
        }
        for (size_t i = 0; i < 8; ++i) {
          gpio_ll_output_disable(&GPIO, (gpio_num_t)par_dbh[i]);
        }
      }
    }
    else {
      auto idx_base = LCD_DATA_OUT0_IDX;
      if (interface == _UDSP_PAR8) {
        for (size_t i = 0; i < 8; ++i) {
          gpio_matrix_out(par_dbl[i], idx_base + i, 0, 0);
        }
      } else {
        for (size_t i = 0; i < 8; ++i) {
          gpio_matrix_out(par_dbl[i], idx_base + i, 0, 0);
        }
        for (size_t i = 0; i < 8; ++i) {
          gpio_matrix_out(par_dbh[i], idx_base + 8 + i, 0, 0);
        }
      }
    }
}

// ===== ESP32-S3 Simple Resistive Touch =====

uint32_t uDisplay::get_sr_touch(uint32_t _xp, uint32_t _xm, uint32_t _yp, uint32_t _ym) {
  uint32_t aval = 0;
  uint16_t xp,yp;
  if (pb_busy()) return 0;

  _pb_init_pin(true);
  gpio_matrix_out(par_rs, 0x100, 0, 0);

  pinMode(_ym, INPUT_PULLUP); // d0
  pinMode(_yp, INPUT_PULLUP); // rs

  pinMode(_xm, OUTPUT); // cs
  pinMode(_xp, OUTPUT); // d1
  digitalWrite(_xm, HIGH); // cs
  digitalWrite(_xp, LOW); // d1

  xp = 4096 - analogRead(_ym); // d0

  pinMode(_xm, INPUT_PULLUP); // cs
  pinMode(_xp, INPUT_PULLUP); // d1

  pinMode(_ym, OUTPUT); // d0
  pinMode(_yp, OUTPUT); // rs
  digitalWrite(_ym, HIGH); // d0
  digitalWrite(_yp, LOW); // rs

  yp = 4096 - analogRead(_xp); // d1

  aval = (xp << 16) | yp;

  pinMode(_yp, OUTPUT); // rs
  pinMode(_xm, OUTPUT); // cs
  pinMode(_ym, OUTPUT); // d0
  pinMode(_xp, OUTPUT); // d1
  digitalWrite(_yp, HIGH); // rs
  digitalWrite(_xm, HIGH); // cs

  _pb_init_pin(false);
  gpio_matrix_out(par_rs, LCD_DC_IDX, 0, 0);

  return aval;
}

#endif // USE_ESP32_S3