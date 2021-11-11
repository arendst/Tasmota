#include "display_ops.h"

typedef struct {
#if defined(CONFIG_EPD_BOARD_REVISION_V5)
  bool power_enable : 1;
#else
  bool power_disable : 1;
#endif
  bool power_enable_vpos : 1;
  bool power_enable_vneg : 1;
  bool power_enable_gl : 1;
  bool ep_stv : 1;
  bool power_enable_gh : 1;
  bool ep_mode : 1;
  bool ep_output_enable : 1;
} epd_config_register_t;

static void config_reg_init(epd_config_register_t *cfg) {
#if defined(CONFIG_EPD_BOARD_REVISION_V5)
  cfg->power_enable = false;
#else
  cfg->power_disable = true;
#endif
  cfg->power_enable_vpos = false;
  cfg->power_enable_vneg = false;
  cfg->power_enable_gl = false;
  cfg->ep_stv = true;
  cfg->power_enable_gh = false;
  cfg->ep_mode = false;
  cfg->ep_output_enable = false;
}

inline static void IRAM_ATTR push_cfg_bit(bool bit);
void IRAM_ATTR busy_delay(uint32_t cycles);
inline static void fast_gpio_set_hi(gpio_num_t gpio_num);
inline static void fast_gpio_set_lo(gpio_num_t gpio_num);

static void IRAM_ATTR push_cfg(const epd_config_register_t *cfg) {
  fast_gpio_set_lo(CFG_STR);

  // push config bits in reverse order
  push_cfg_bit(cfg->ep_output_enable);
  push_cfg_bit(cfg->ep_mode);
  push_cfg_bit(cfg->power_enable_gh);
  push_cfg_bit(cfg->ep_stv);

  push_cfg_bit(cfg->power_enable_gl);
  push_cfg_bit(cfg->power_enable_vneg);
  push_cfg_bit(cfg->power_enable_vpos);
#if defined(CONFIG_EPD_BOARD_REVISION_V5)
  push_cfg_bit(cfg->power_enable);
#else
  push_cfg_bit(cfg->power_disable);
#endif

  fast_gpio_set_hi(CFG_STR);
  fast_gpio_set_lo(CFG_STR);
}

static void cfg_poweron(epd_config_register_t *cfg) {
  // POWERON
#if defined(CONFIG_EPD_BOARD_REVISION_V5)
  cfg->power_enable = true;
#else
  cfg->power_disable = false;
#endif
  push_cfg(cfg);
  busy_delay(100 * 240);
  cfg->power_enable_gl = true;
  push_cfg(cfg);
  busy_delay(500 * 240);
  cfg->power_enable_vneg = true;
  push_cfg(cfg);
  busy_delay(500 * 240);
  cfg->power_enable_gh = true;
  push_cfg(cfg);
  busy_delay(500 * 240);
  cfg->power_enable_vpos = true;
  push_cfg(cfg);
  busy_delay(100 * 240);
  cfg->ep_stv = true;
  push_cfg(cfg);
  fast_gpio_set_hi(STH);
  // END POWERON
}

static void cfg_poweroff(epd_config_register_t *cfg) {
  // POWEROFF
  cfg->power_enable_gh = false;
  cfg->power_enable_vpos = false;
  push_cfg(cfg);
  busy_delay(10 * 240);
  cfg->power_enable_gl = false;
  cfg->power_enable_vneg = false;
  push_cfg(cfg);
  busy_delay(100 * 240);

  cfg->ep_stv = false;
  cfg->ep_output_enable = false;
  cfg->ep_mode = false;
#if defined(CONFIG_EPD_BOARD_REVISION_V5)
  cfg->power_enable = false;
#else
  cfg->power_disable = true;
#endif
  push_cfg(cfg);
  // END POWEROFF
}
