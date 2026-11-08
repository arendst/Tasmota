/*
  xdrv_126_hwdump.ino - Dump MCU internal hardware configuration

  SPDX-FileCopyrightText: 2026 Stephan Hadinger

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef ESP32
#ifdef USE_HWDUMP
/*********************************************************************************************\
 * HwDump - dump the internal hardware configuration of the MCU to the log (LOG_LEVEL_INFO)
 *
 * Only reads registers through ESP-IDF driver/LL calls, never writes anything.
 *
 * Supported commands:
 *   HwDump               - Dump GPIO / IO_MUX / GPIO matrix configuration, then LEDC (PWM), RMT, I2S, SPI, I2C, UART,
 *                          USB-Serial-JTAG, RTC_IO/LP_IO and sleep configuration
 *
 * Supported targets:
 *   ESP32, ESP32-S2, ESP32-S3, ESP32-C3, ESP32-C5, ESP32-C6, ESP32-P4
 *
 * Per-target data lives in tasmota/include/xdrv_126_hwdump_<target>.h (xdrv_126_hwdump_esp32.h, xdrv_126_hwdump_esp32s2.h,
 * xdrv_126_hwdump_esp32s3.h, xdrv_126_hwdump_esp32c3.h, xdrv_126_hwdump_esp32c5.h, xdrv_126_hwdump_esp32c6.h,
 * xdrv_126_hwdump_esp32p4.h).
 * To add a target, create tasmota/include/xdrv_126_hwdump_<target>.h, add it to the `#elif CONFIG_IDF_TARGET_xxx`
 * include list below, and provide:
 *   - HWDUMP_SUPPORTED
 *   - kHwdPads[SOC_GPIO_PIN_COUNT]         pad name and '|' separated IO_MUX function names (index = MCU_SEL)
 *   - kHwdSigOut[] / kHwdSigIn[]           GPIO matrix signal names, from soc/gpio_sig_map.h
 *   - HwdOutLevel() HwdOutInv() HwdInInv() target specific register field accessors
 *   - optional HWDUMP_LEDC with HWD_LEDC_MODE_NUM, kHwdLedcModes[], kHwdLedcClkNames and HwdLedc*() accessors
 *   - optional HWDUMP_USB_SERIAL_JTAG with HwdUsjBusClk(), HWD_USJ_DM_GPIO/HWD_USJ_DP_GPIO, HWD_USJ_DATE_REG and
 *     HWD_USJ_F() (status field name prefix, ESP32-P4 rev 3 adds "serial_jtag_")
 *   - optional HWDUMP_UART, HWDUMP_I2C with their per-target accessors (clock gating, clock selector, signals,
 *     HWD_UART_CONF0/HWD_UART_CLKDIV/HWD_UART_DIV_INT/HWD_UART_DIV_FRAG and HWD_I2C_RXFIFO_CNT/HWD_I2C_TXFIFO_CNT field names)
 *   - optional HWDUMP_RMT, HWDUMP_I2S, HWDUMP_SPI (ESP32-C3 / ESP32-S3 / ESP32-C5 / ESP32-C6 / ESP32-P4 register layout, RMT field name
 *     differences through HWD_RMT_TX_CONF(), HWD_RMT_RX_CONF(), HWD_RMT_TX_F(), HWD_RMT_RX_F(), HWD_RMT_SIG_OUT0/HWD_RMT_SIG_IN0,
 *     group clock through HwdRmtSclk*(),
 *     I2S clocks through HwdI2sClkGet(), SPI clock through HwdSpiClk*()...) or
 *     HWDUMP_RMT_ESP32, HWDUMP_I2S_ESP32, HWDUMP_SPI_ESP32 (ESP32 and ESP32-S2 register layout, field name differences
 *     through HWD_RMT_F(), HwdRmt*(), HwdI2sClkSel(), HWD_I2S_PDM, HWD_SPI_PIN(), HWD_SPI_CS_NUM, HwdSpiTransInten()...)
 *   - optional HWDUMP_RTCIO (RTC_IO pads, rtc_io_desc[], HWD_RTCIO_HOLD_FORCE_REG) or HWDUMP_LPIO (ESP32-C5 / ESP32-C6 / ESP32-P4
 *     LP_IO pads, register layout through HWD_LPIO_MUX(), HWD_LPIO_PIN(), HWD_LPIO_MUX_F(), HWD_LPIO_PIN_F(), HWD_LPIO_SLP_PULL,
 *     HWD_LPIO_FUNC_GPIO, HwdLpIoOe/Out/In/IntClk/HoldMask())
 *     and HWDUMP_SLEEP with HwdPinIsRtc(), HwdPadHold(), HwdWakeupEna() and kHwdWakeupNames, plus HWDUMP_SLEEP_EXT for
 *     ext0/ext1 wakeup (HwdRtcGpio()) and HWDUMP_SLEEP_DG_PAD_HOLD (RTC_CNTL) or HWDUMP_SLEEP_PMU_PAD_HOLD (PMU) pad hold
 *
 * Maintenance rules:
 *   - Use only ESP-IDF calls (driver / LL / HAL getters) or direct reads of the register structs. Never write a register.
 *   - Per-target headers define data (tables, static inline accessors): include them only once, from this file.
 *   - Prefer adding a hook to an existing shared function over writing a new per-target function. A new hook must be
 *     defined explicitly in every header that uses that function (no default value in this file).
 *     Optional sections are enabled per target with HWDUMP_xxx feature macros.
 *   - .ino function signatures use builtin types only (Arduino auto-generated prototypes are emitted before the custom types).
 *     Pass peripheral handles through macros or header inline functions (HwdUartDev(), HwdI2cDev()...), which may use any type.
 *   - Avoid #if inside the argument list of snprintf_P() (it is a macro): build the line in several snprintf_P() calls instead.
 *   - Snapshot a register once and decode fields from the copy: `__typeof__(reg) x; x.val = reg.val;`
 *   - Check the peripheral bus clock and reset state before reading it. Location varies by family:
 *     ESP32 / S2 / S3 periph_ll_periph_enabled() (S3 USB-Serial-JTAG: SYSTEM.perip_clk_en1), C3 SYSTEM.perip_clk_en0 /
 *     perip_rst_en0, C5 / C6 PCR.<periph>_conf (on C5 most clock selectors and dividers are in PCR too, not in the peripheral),
 *     P4 HP_SYS_CLKRST (soc_clk_ctrlN = bus clocks, hp_rst_enN = resets, peri_clk_ctrlN = clock selectors and dividers).
 *   - Output: booleans as "Y" / ".", "-" = not available or no pin. Niche tables are opt-in (USE_HWDUMP_SLEEP_PINS), because
 *     AddLog goes through the log ring buffer and long dumps are truncated in the web console (serial gets everything).
 *   - Values like I2C scl~, I2S fs~ and RC_FAST based clocks are approximations; SPI mode/sck are decoded for master only.
 *
 * Adding a target, in practice:
 *   - Start from the header of the closest family (C3-layout RMT/I2S/SPI: C3, S3, C5, C6, P4; ESP32-layout: ESP32, S2;
 *     RTC_IO: ESP32, S2, S3; LP_IO + PMU sleep: C5, C6, P4).
 *   - Generate kHwdPads[] and kHwdSigOut[] / kHwdSigIn[] by script from soc/io_mux_reg.h and soc/gpio_sig_map.h, listing every
 *     signal. Known traps: PIN_FUNC_GPIO is 2 on ESP32 and 1 on later targets (check io_mux_reg.h); some gpio_sig_map.h list input and
 *     output names at the same index (C5, C6: split them by name, e.g. USB_JTAG_TDO is an input on C5); some defines are
 *     malformed (S3 SDHOST_CCMD_OD_PULLUP_EN_N_IDX has no value); P4 uses xxx_PAD_OUT_IDX / xxx_PAD_IN_IDX names.
 *   - Verify every identifier (struct member, field, macro) against the target headers in
 *     ~/.platformio/packages/framework-arduinoespressif32/tools/esp32-arduino-libs/<target>/include
 *     Field names often change between targets (suffixes _chn/_chm, prefixes, renamed registers); check that each field belongs
 *     to the register it is read from. Some targets have several register sets selected by silicon revision
 *     (ESP32-P4: hw_ver1 / hw_ver3, select with CONFIG_ESP_REV_MIN_FULL like HAL_CONFIG(CHIP_SUPPORT_MIN_REV)).
 *   - Clock selector values differ per peripheral and target (e.g. LEDC tick_sel: ESP32 0=REF_TICK, S2 inverted, unused on
 *     S3/C3/C5), take them from the LL clock getters (xxx_ll_get_clock_source...) and document them next to the accessor.
 *   - Some LL helpers assert or abort on pins outside their range (P4 gpio_ll_is_digital_io_hold() on GPIO0-15): read the LL
 *     source before calling it in a loop over all pins.
 *   - Wakeup bit names come from the private esp_hw_support/port/<target>/private_include/pmu_bit_defs.h: hardcode the names.
 *
 * Read side effects: FIFO/data ports (UART_FIFO_REG, I2C_DATA_REG, RMT_CHnDATA_REG, USB_SERIAL_JTAG_EP1_REG,
 * ESP32 I2Sn.fifo_rd) pop data when read, they are never accessed. Peripherals are only read when their bus
 * clock is enabled and they are out of reset.
\*********************************************************************************************/

#define XDRV_126              126

#include "driver/gpio.h"
#include "hal/gpio_ll.h"
#include "soc/gpio_sig_map.h"
#include "soc/io_mux_reg.h"
#include "esp_private/esp_gpio_reserve.h"
#include "esp_clk_tree.h"
#include "soc/gpio_pins.h"            // GPIO_MATRIX_CONST_ONE_INPUT / GPIO_MATRIX_CONST_ZERO_INPUT
#if SOC_LEDC_SUPPORTED
#include "hal/ledc_ll.h"
#endif

typedef struct {
  const char *pad;          // physical pad name
  const char *funcs;        // IO_MUX functions indexed by MCU_SEL, '|' separated
} HwdPad_t;

typedef struct {
  uint16_t    idx;          // GPIO matrix signal index
  const char *name;
} HwdSig_t;

// Build a signal entry from the ESP-IDF macro name, so the index always matches gpio_sig_map.h
#define HWD_SIG(x)   { x##_IDX, #x }

/*********************************************************************************************\
 * Per-target data and register accessors (one header per SoC, see "To add a target" above)
\*********************************************************************************************/
#if CONFIG_IDF_TARGET_ESP32C3
#include "include/xdrv_126_hwdump_esp32c3.h"
#elif CONFIG_IDF_TARGET_ESP32S2
#include "include/xdrv_126_hwdump_esp32s2.h"
#elif CONFIG_IDF_TARGET_ESP32S3
#include "include/xdrv_126_hwdump_esp32s3.h"
#elif CONFIG_IDF_TARGET_ESP32
#include "include/xdrv_126_hwdump_esp32.h"
#elif CONFIG_IDF_TARGET_ESP32C5
#include "include/xdrv_126_hwdump_esp32c5.h"
#elif CONFIG_IDF_TARGET_ESP32C6
#include "include/xdrv_126_hwdump_esp32c6.h"
#elif CONFIG_IDF_TARGET_ESP32P4
#include "include/xdrv_126_hwdump_esp32p4.h"
#endif

/*********************************************************************************************\
 * Common code
\*********************************************************************************************/

#ifdef HWDUMP_SUPPORTED

static_assert(sizeof(kHwdPads) / sizeof(kHwdPads[0]) == SOC_GPIO_PIN_COUNT, "HwDump: kHwdPads must have SOC_GPIO_PIN_COUNT entries");

// Return the GPIO matrix signal name (output or input table), or nullptr if unknown
// Note: no custom types in the signature, to keep the auto-generated .ino prototypes valid
const char* HwdSigName(bool output, uint32_t idx) {
  const HwdSig_t *table = output ? kHwdSigOut : kHwdSigIn;
  uint32_t count = output ? nitems(kHwdSigOut) : nitems(kHwdSigIn);
  for (uint32_t i = 0; i < count; i++) {
    if (table[i].idx == idx) { return table[i].name; }
  }
  return nullptr;
}

// Log one line of the dump (passed as "%s" argument so '%' in the line is never interpreted)
void HwdPrint(const char *line) {
  AddLog(LOG_LEVEL_INFO, PSTR("%s"), line);
}

// Fill `out` with the Tasmota component name assigned to `pin` (empty if none), same naming as command `Gpio`
void HwdTasmotaName(char *out, size_t out_size, uint32_t pin) {
  out[0] = 0;
  if (pin >= MAX_GPIO_PIN) { return; }
  uint32_t sensor_type = TasmotaGlobal.gpio_pin[pin];
  if (GPIO_NONE == sensor_type) { return; }

  char sindex[4] = { 0 };
  uint32_t nice_list_search = sensor_type & 0xFFE0;
  for (uint32_t j = 0; j < nitems(kGpioNiceList); j++) {
    uint32_t nls_idx = pgm_read_word(&kGpioNiceList[j]);
    if (((nls_idx & 0xFFE0) == nice_list_search) && ((nls_idx & 0x001F) > 0)) {
      snprintf_P(sindex, sizeof(sindex), PSTR("%d"), (sensor_type & 0x001F) +1);
      break;
    }
  }
  uint32_t sensor_name_idx = BGPIO(sensor_type);
  const char *sensor_names = kSensorNames;
  if (sensor_name_idx > GPIO_FIX_START) {
    sensor_name_idx = sensor_name_idx - GPIO_FIX_START -1;
    sensor_names = kSensorNamesFixed;
  }
  char name[24];
  GetTextIndexed(name, sizeof(name), sensor_name_idx, sensor_names);
  snprintf_P(out, out_size, PSTR("%s%s"), name, sindex);
}

/*********************************************************************************************\
 * Shared helpers for peripheral sections
\*********************************************************************************************/

// Frequency of a soc_module_clk_t clock source in Hz, 0 if unknown (passed as uint32_t for .ino prototypes)
uint32_t HwdSrcHz(uint32_t soc_clk) {
  uint32_t hz = 0;
  if (soc_clk) { esp_clk_tree_src_get_freq_hz((soc_module_clk_t)soc_clk, ESP_CLK_TREE_SRC_FREQ_PRECISION_APPROX, &hz); }
  return hz;
}

// Format a frequency in Hz as "40 MHz", "100.000 kHz" or "977 Hz"
void HwdFmtHz(char *out, size_t size, uint32_t hz) {
  if (hz >= 1000000 && 0 == (hz % 1000000)) {
    snprintf_P(out, size, PSTR("%u MHz"), hz / 1000000);
  } else if (hz >= 1000000) {
    snprintf_P(out, size, PSTR("%u.%03u MHz"), hz / 1000000, (hz % 1000000) / 1000);
  } else if (hz >= 1000) {
    snprintf_P(out, size, PSTR("%u.%03u kHz"), hz / 1000, hz % 1000);
  } else {
    snprintf_P(out, size, PSTR("%u Hz"), hz);
  }
}

// Fill `out` with the GPIOs driven by GPIO matrix output signal `sig` or by IO_MUX function `iomux` (nullptr if none), "-" if none
void HwdOutPins(char *out, size_t size, uint32_t sig, const char *iomux) {
  uint32_t len = 0;
  out[0] = 0;
  for (uint32_t pin = 0; pin < SOC_GPIO_PIN_COUNT; pin++) {
    if (len >= size) { break; }
    if (!GPIO_IS_VALID_GPIO((int)pin)) { continue; }
    gpio_io_config_t io = {};
    if (ESP_OK != gpio_get_io_config((gpio_num_t)pin, &io)) { continue; }
    bool match = false;
    if (PIN_FUNC_GPIO == io.fun_sel) {
      match = (io.sig_out == sig);
    } else if (iomux) {
      char name[16];
      GetTextIndexed(name, sizeof(name), io.fun_sel, kHwdPads[pin].funcs);
      match = (0 == strcmp(name, iomux));
    }
    if (match) {
      len += snprintf_P(out + len, size - len, PSTR("%s%u"), len ? "," : "", pin);
    }
  }
  if (!out[0]) { strlcpy(out, "-", size); }
}

// Fill `out` with the source of GPIO matrix input signal `sig` ('!' = inverted), "const0"/"const1",
// the IO_MUX pad if the matrix is bypassed and `iomux` is selected on a pad, else "-"
void HwdInPin(char *out, size_t size, uint32_t sig, const char *iomux) {
  int in = gpio_ll_get_in_signal_connected_io(&GPIO, sig);
  if (in >= 0) {
    const char *inv = HwdInInv(sig) ? "!" : "";
    if (in < SOC_GPIO_PIN_COUNT) {
      snprintf_P(out, size, PSTR("%s%d"), inv, in);
    } else if (GPIO_MATRIX_CONST_ONE_INPUT == in) {
      snprintf_P(out, size, PSTR("%sconst1"), inv);
    } else if (GPIO_MATRIX_CONST_ZERO_INPUT == in) {
      snprintf_P(out, size, PSTR("%sconst0"), inv);
    } else {
      snprintf_P(out, size, PSTR("%sin%d"), inv, in);
    }
    return;
  }
  strlcpy(out, "-", size);
  if (!iomux) { return; }
  for (uint32_t pin = 0; pin < SOC_GPIO_PIN_COUNT; pin++) {     // matrix bypassed: look for the IO_MUX function
    if (!GPIO_IS_VALID_GPIO((int)pin)) { continue; }
    gpio_io_config_t io = {};
    if (ESP_OK != gpio_get_io_config((gpio_num_t)pin, &io)) { continue; }
    if (PIN_FUNC_GPIO == io.fun_sel) { continue; }
    char name[16];
    GetTextIndexed(name, sizeof(name), io.fun_sel, kHwdPads[pin].funcs);
    if (0 == strcmp(name, iomux)) {
      snprintf_P(out, size, PSTR("%u"), pin);
      return;
    }
  }
}

void HwDumpGpio(void) {
  HwdPrint("GPIO  *=reserved by IDF driver, inv=output inverted, oeP=output enable by peripheral, oe=output enable, od=open drain, ie=input enable, pu=pull-up, pd=pull-down, drv=drive strength 0-3, o=output level, i=input level, !=input inverted"
#ifdef HWDUMP_RTCIO
           ", route rtc=pad controlled by RTC_IO (see RTC_IO)"
#endif
#ifdef HWDUMP_LPIO
           ", route rtc=pad controlled by LP_IO (see LP_IO)"
#endif
           );
  HwdPrint(" pin pad         tasmota      iomux      route  output signal        inv oeP  oe  od  ie  pu  pd drv  o  i  inputs");

  for (uint32_t pin = 0; pin < SOC_GPIO_PIN_COUNT; pin++) {
    if (!GPIO_IS_VALID_GPIO((int)pin)) { continue; }
    if (FlashPin(pin)) { continue; }                       // same pins as hidden by command Gpio
    // Note: esp_gpio_is_reserved() is not only flash/PSRAM, drivers like LEDC also reserve the pins they use
    bool reserved = esp_gpio_is_reserved(BIT64(pin));

    gpio_io_config_t io = {};
    if (ESP_OK != gpio_get_io_config((gpio_num_t)pin, &io)) { continue; }

    // Tasmota assigned component
    char tasmota[24];
    HwdTasmotaName(tasmota, sizeof(tasmota), pin);

    // IO_MUX function currently selected
    char iomux[16];
    GetTextIndexed(iomux, sizeof(iomux), io.fun_sel, kHwdPads[pin].funcs);
    if (!iomux[0]) { snprintf_P(iomux, sizeof(iomux), PSTR("F%u"), io.fun_sel); }

    // Output signal: GPIO matrix signal if routed through the matrix, else the IO_MUX function
    bool matrix = (PIN_FUNC_GPIO == io.fun_sel);
#if defined(HWDUMP_RTCIO) || defined(HWDUMP_LPIO)
    bool rtc = HwdPadRtcMux(pin);                          // pad switched to RTC_IO/LP_IO, digital settings below are not effective
#else
    bool rtc = false;
#endif
    char sig_out[28];
    if (matrix) {
      const char *name = HwdSigName(true, io.sig_out);
      if (name) {
        strlcpy(sig_out, name, sizeof(sig_out));
      } else {
        snprintf_P(sig_out, sizeof(sig_out), PSTR("sig%u"), io.sig_out);
      }
    } else {
      strlcpy(sig_out, iomux, sizeof(sig_out));
    }

    char line[256];
    uint32_t len = snprintf_P(line, sizeof(line), PSTR("%3u%c %-11s %-12s %-10s %-6s %-20s %3s %3s %3s %3s %3s %3s %3s %3u %2u %2u "),
      pin, reserved ? '*' : ' ', kHwdPads[pin].pad, tasmota, iomux, rtc ? "rtc" : matrix ? "matrix" : "iomux", sig_out,
      (matrix && HwdOutInv(pin)) ? "Y" : ".",     // output inversion only applies to the GPIO matrix
      io.oe_ctrl_by_periph ? "Y" : ".",
      io.oe ? "Y" : ".",
      io.od ? "Y" : ".",
      io.ie ? "Y" : ".",
      io.pu ? "Y" : ".",
      io.pd ? "Y" : ".",
      (uint32_t)io.drv,
      HwdOutLevel(pin),
      (uint32_t)gpio_ll_get_level(&GPIO, pin));

    // Peripheral input signals routed from this pin through the GPIO matrix ('!' = inverted)
    for (uint32_t sig = 0; sig < SIG_GPIO_OUT_IDX; sig++) {
      if (len >= sizeof(line)) { break; }
      if (gpio_ll_get_in_signal_connected_io(&GPIO, sig) != (int)pin) { continue; }
      const char *name = HwdSigName(false, sig);
      const char *inv = HwdInInv(sig) ? "!" : "";
      if (name) {
        len += snprintf_P(line + len, sizeof(line) - len, PSTR(" %s%s"), inv, name);
      } else {
        len += snprintf_P(line + len, sizeof(line) - len, PSTR(" %ssig%u"), inv, sig);
      }
    }
    HwdPrint(line);
  }
}

#ifdef HWDUMP_LEDC
void HwDumpLedc(void) {
  char line[128];
  if (!HwdLedcBusClk()) {
    HwdPrint("LEDC  peripheral clock disabled or in reset");
    return;
  }

  uint32_t clk_sel = HwdLedcClkSel();
  uint32_t clk_hz = HwdLedcClkHz(clk_sel);
  char clk_name[12], hz[20];
  GetTextIndexed(clk_name, sizeof(clk_name), clk_sel, kHwdLedcClkNames);
  HwdFmtHz(hz, sizeof(hz), clk_hz);
  uint32_t len = snprintf_P(line, sizeof(line), PSTR("LEDC  " HWD_LEDC_CLK_LABEL "=%s (%s)"), clk_name, hz);
#ifdef HWDUMP_LEDC_CLK_EN
  snprintf_P(line + len, sizeof(line) - len, PSTR(" clk_en=%s"), HwdLedcClkEn() ? "Y" : ".");
#endif
  HwdPrint(line);

  for (uint32_t m = 0; m < HWD_LEDC_MODE_NUM; m++) {
    uint32_t mode = kHwdLedcModes[m];
#if HWD_LEDC_MODE_NUM > 1
    char mode_name[16];
    GetTextIndexed(mode_name, sizeof(mode_name), m, kHwdLedcModeNames);
    snprintf_P(line, sizeof(line), PSTR("  %s"), mode_name);
    HwdPrint(line);
#endif

    // Timers: clock divider is a fixed-point value with LEDC_LL_FRACTIONAL_BITS fractional bits
    //   freq = timer clk / (div * 2^res)
    HwdPrint("   timer          div  res      freq Hz paused  rst    cnt  clk");
    for (uint32_t t = 0; t < SOC_LEDC_TIMER_NUM; t++) {
      uint32_t div_raw, res;
      ledc_ll_get_clock_divider(&LEDC, (ledc_mode_t)mode, (ledc_timer_t)t, &div_raw);
      ledc_ll_get_duty_resolution(&LEDC, (ledc_mode_t)mode, (ledc_timer_t)t, &res);
      uint32_t timer_hz = HwdLedcTimerClkHz(mode, t, clk_hz);
      uint32_t div_int = div_raw >> LEDC_LL_FRACTIONAL_BITS;
      uint32_t div_frac = ((div_raw & LEDC_LL_FRACTIONAL_MAX) * 10000 + (1 << (LEDC_LL_FRACTIONAL_BITS - 1))) >> LEDC_LL_FRACTIONAL_BITS;
      uint64_t freq_mhz = 0;      // in milli-Hertz
      if (div_raw) {
        uint64_t num = ((uint64_t)timer_hz * 1000) << LEDC_LL_FRACTIONAL_BITS;
        uint64_t den = (uint64_t)div_raw << res;
        freq_mhz = (num + den / 2) / den;
      }
      HwdFmtHz(hz, sizeof(hz), timer_hz);
      snprintf_P(line, sizeof(line), PSTR("%8u %7u.%04u %4u %8u.%03u %6s %4s %6u  %s"),
        t, div_int, div_frac, res,
        (uint32_t)(freq_mhz / 1000), (uint32_t)(freq_mhz % 1000),
        HwdLedcTimerPaused(mode, t) ? "Y" : ".",
        HwdLedcTimerRst(mode, t) ? "Y" : ".",
        HwdLedcTimerCnt(mode, t), hz);
      HwdPrint(line);
    }

    // Channels: duty is the current value (duty_rd), max = 2^res of the bound timer
    HwdPrint("      ch timer out_en idle hpoint    duty     max       %  pins");
    for (uint32_t ch = 0; ch < SOC_LEDC_CHANNEL_NUM; ch++) {
      ledc_timer_t timer;
      uint32_t hpoint, duty, max_duty;
      ledc_ll_get_channel_timer(&LEDC, (ledc_mode_t)mode, (ledc_channel_t)ch, &timer);
      ledc_ll_get_hpoint(&LEDC, (ledc_mode_t)mode, (ledc_channel_t)ch, &hpoint);
      ledc_ll_get_duty(&LEDC, (ledc_mode_t)mode, (ledc_channel_t)ch, &duty);
      ledc_ll_get_max_duty(&LEDC, (ledc_mode_t)mode, timer, &max_duty);
      uint32_t pct = max_duty ? (uint32_t)(((uint64_t)duty * 10000 + max_duty / 2) / max_duty) : 0;   // in 1/100 %
      len = snprintf_P(line, sizeof(line), PSTR("%8u %5u %6s %4u %6u %7u %7u %4u.%02u "),
        ch, (uint32_t)timer, HwdLedcChOutEn(mode, ch) ? "Y" : ".", HwdLedcChIdle(mode, ch),
        hpoint, duty, max_duty, pct / 100, pct % 100);

      // Pins driven by this channel through the GPIO matrix
      uint32_t sig = HwdLedcSig(mode, ch);
      for (uint32_t pin = 0; pin < SOC_GPIO_PIN_COUNT; pin++) {
        if (len >= sizeof(line)) { break; }
        if (!GPIO_IS_VALID_GPIO((int)pin)) { continue; }
        gpio_io_config_t io = {};
        if (ESP_OK != gpio_get_io_config((gpio_num_t)pin, &io)) { continue; }
        if ((PIN_FUNC_GPIO == io.fun_sel) && (sig == io.sig_out)) {
          len += snprintf_P(line + len, sizeof(line) - len, PSTR(" %u"), pin);
        }
      }
      HwdPrint(line);
    }
  }
}
#endif  // HWDUMP_LEDC

#ifdef HWDUMP_USB_SERIAL_JTAG
#include "soc/usb_serial_jtag_reg.h"  // HWD_USJ_DATE_REG (`date` is uint32_t on C3/S3, a register union on C5)
// Per-target hooks: HWD_USJ_DM_GPIO / HWD_USJ_DP_GPIO (USB D-/D+ pads), HWD_USJ_DATE_REG (date register address),
// HWD_USJ_F(f) (field names of misc_conf, fram_num, in_ep1_st, out_ep1_st, jfifo_st: ESP32-P4 rev 3 adds "serial_jtag_")

// Interrupt bits of USB_SERIAL_JTAG_INT_xxx_REG (bit 0 to 11)
const char kHwdUsjIntNames[] PROGMEM =
  "jtag_in_flush|sof|serial_out_recv_pkt|serial_in_empty|pid_err|crc5_err|crc16_err|stuff_err|"
  "in_token_rec_in_ep1|usb_bus_reset|out_ep1_zero_payload|out_ep2_zero_payload";

// Append the names of bits set in `mask` to `line`, returns new length
uint32_t HwdUsjIntDecode(char *line, size_t size, uint32_t len, uint32_t mask) {
  char name[24];
  for (uint32_t bit = 0; bit < 12; bit++) {
    if (len >= size) { break; }
    if (mask & (1 << bit)) {
      GetTextIndexed(name, sizeof(name), bit, kHwdUsjIntNames);
      len += snprintf_P(line + len, size - len, PSTR(" %s"), name);
    }
  }
  return len;
}

void HwDumpUsbSerialJtag(void) {
  char line[400];   // worst case: all 12 interrupt names listed twice on the "int" line
  const char *Y = "Y";
  const char *N = ".";
  if (!HwdUsjBusClk()) {
    HwdPrint("USB-Serial-JTAG  peripheral clock disabled or in reset");
    return;
  }
  usb_serial_jtag_dev_t *usj = &USB_SERIAL_JTAG;

  // Read each register once (all are status/config registers without read side effects)
  // Snapshot into local copies of the register unions (same pattern as gpio_ll_get_in_signal_connected_io)
  uint32_t int_raw = usj->int_raw.val & 0xFFF;
  uint32_t int_ena = usj->int_ena.val & 0xFFF;
  __typeof__(usj->conf0) conf0;        conf0.val = usj->conf0.val;
  __typeof__(usj->ep1_conf) ep1;       ep1.val = usj->ep1_conf.val;
  __typeof__(usj->in_ep1_st) in1;      in1.val = usj->in_ep1_st.val;
  __typeof__(usj->out_ep1_st) out1;    out1.val = usj->out_ep1_st.val;
  __typeof__(usj->jfifo_st) jfifo;     jfifo.val = usj->jfifo_st.val;

  snprintf_P(line, sizeof(line), PSTR("USB-Serial-JTAG  console=%s phy=%s pad_enable=%s D-=GPIO%u D+=GPIO%u reg_clk_force=%s date=%08X"),
    tasconsole_serial ? "UART" : "USB",
    conf0.phy_sel ? "external" : "internal",
    conf0.usb_pad_enable ? Y : N,
    (uint32_t)HWD_USJ_DM_GPIO, (uint32_t)HWD_USJ_DP_GPIO,
    usj->misc_conf.HWD_USJ_F(clk_en) ? Y : N,
    REG_READ(HWD_USJ_DATE_REG));
  HwdPrint(line);

  snprintf_P(line, sizeof(line), PSTR("  host   sof=%s frame=%u bus_reset=%s"),
    (int_raw & USB_SERIAL_JTAG_SOF_INT_RAW) ? Y : N,
    usj->fram_num.HWD_USJ_F(sof_frame_index),
    (int_raw & USB_SERIAL_JTAG_USB_BUS_RESET_INT_RAW) ? Y : N);
  HwdPrint(line);

  snprintf_P(line, sizeof(line), PSTR("  cdc    in_ep1_free=%s in_ep1_state=%u out_ep1_avail=%s out_ep1_cnt=%u out_ep1_state=%u"),
    ep1.serial_in_ep_data_free ? Y : N, in1.HWD_USJ_F(in_ep1_state),
    ep1.serial_out_ep_data_avail ? Y : N, out1.HWD_USJ_F(out_ep1_rec_data_cnt), out1.HWD_USJ_F(out_ep1_state));
  HwdPrint(line);

  snprintf_P(line, sizeof(line), PSTR("  jtag   in_fifo_cnt=%u in_empty=%s in_full=%s out_fifo_cnt=%u out_empty=%s out_full=%s"),
    jfifo.HWD_USJ_F(in_fifo_cnt), jfifo.HWD_USJ_F(in_fifo_empty) ? Y : N, jfifo.HWD_USJ_F(in_fifo_full) ? Y : N,
    jfifo.HWD_USJ_F(out_fifo_cnt), jfifo.HWD_USJ_F(out_fifo_empty) ? Y : N, jfifo.HWD_USJ_F(out_fifo_full) ? Y : N);
  HwdPrint(line);

  snprintf_P(line, sizeof(line), PSTR("  pads   pull_override=%s dp_pullup=%s dp_pulldown=%s dm_pullup=%s dm_pulldown=%s exchg_pins=%s"),
    conf0.pad_pull_override ? Y : N, conf0.dp_pullup ? Y : N, conf0.dp_pulldown ? Y : N,
    conf0.dm_pullup ? Y : N, conf0.dm_pulldown ? Y : N,
    (conf0.exchg_pins_override && conf0.exchg_pins) ? Y : N);
  HwdPrint(line);

  // Interrupts: ena = sources enabled to raise the CPU interrupt, raw = events latched by hardware (even if not enabled)
  uint32_t len = snprintf_P(line, sizeof(line), PSTR("  int    ena=0x%03X"), int_ena);
  len = HwdUsjIntDecode(line, sizeof(line), len, int_ena);
  if (len < sizeof(line)) {
    len += snprintf_P(line + len, sizeof(line) - len, PSTR("  raw=0x%03X"), int_raw);
    HwdUsjIntDecode(line, sizeof(line), len, int_raw);
  }
  HwdPrint(line);
}
#endif  // HWDUMP_USB_SERIAL_JTAG

/*********************************************************************************************\
 * UART
\*********************************************************************************************/
#ifdef HWDUMP_UART
void HwDumpUart(void) {
  char line[256];
  char clk_name[12], hz[20];
  char tx[24], rx[16], rts[24], cts[16];
  for (uint32_t n = 0; n < SOC_UART_HP_NUM; n++) {
    if (!uart_ll_is_enabled(n)) {
      snprintf_P(line, sizeof(line), PSTR("UART%u  peripheral clock disabled or in reset"), n);
      HwdPrint(line);
      continue;
    }
    uart_dev_t *hw = HwdUartDev(n);
    uint32_t sel = HwdUartClkSel(n);
    uint32_t clk_hz = HwdSrcHz(HwdUartClkSoc(sel));
    GetTextIndexed(clk_name, sizeof(clk_name), sel, kHwdUartClkNames);
    HwdFmtHz(hz, sizeof(hz), clk_hz);

    __typeof__(hw->HWD_UART_CONF0) conf0;   conf0.val = hw->HWD_UART_CONF0.val;   // UART_CONF0_REG (conf0_sync on C5)
    __typeof__(hw->status) status;  status.val = hw->status.val;
    __typeof__(hw->HWD_UART_CLKDIV) clk_div;  clk_div.val = hw->HWD_UART_CLKDIV.val;   // single 32-bit read, like uart_ll_get_baudrate
    uint32_t div = (clk_div.HWD_UART_DIV_INT << 4) | clk_div.HWD_UART_DIV_FRAG;
    uint32_t baud = (div && clk_hz) ? uart_ll_get_baudrate(hw, clk_hz) : 0;   // guard against division by zero
    static const char kStop[] = "?|1|1.5|2";
    char stop[4];
    GetTextIndexed(stop, sizeof(stop), conf0.stop_bit_num, kStop);
    char parity = conf0.parity_en ? (conf0.parity ? 'O' : 'E') : 'N';

    snprintf_P(line, sizeof(line), PSTR("UART%u  clk=%s (%s) baud=%u %u%c%s rts=%s cts=%s inv_rx=%s inv_tx=%s loopback=%s rx_fifo=%u tx_fifo=%u rxd=%u txd=%u int_ena=0x%05X raw=0x%05X"),
      n, clk_name, hz, baud, conf0.bit_num + 5, parity, stop,
      uart_ll_is_hw_rts_en(hw) ? "Y" : ".", uart_ll_is_hw_cts_en(hw) ? "Y" : ".",
      conf0.rxd_inv ? "Y" : ".", conf0.txd_inv ? "Y" : ".", conf0.loopback ? "Y" : ".",
      status.rxfifo_cnt, status.txfifo_cnt, status.rxd, status.txd,
      hw->int_ena.val & UART_LL_INTR_MASK, hw->int_raw.val & UART_LL_INTR_MASK);
    HwdPrint(line);

    HwdOutPins(tx, sizeof(tx), kHwdUartSig[n][0], kHwdUartIomux[n][0]);
    HwdInPin(rx, sizeof(rx), kHwdUartSig[n][1], kHwdUartIomux[n][1]);
    HwdOutPins(rts, sizeof(rts), kHwdUartSig[n][2], kHwdUartIomux[n][2]);
    HwdInPin(cts, sizeof(cts), kHwdUartSig[n][3], kHwdUartIomux[n][3]);
    snprintf_P(line, sizeof(line), PSTR("       pins tx=%s rx=%s rts=%s cts=%s"), tx, rx, rts, cts);
    HwdPrint(line);
  }
}
#endif  // HWDUMP_UART

/*********************************************************************************************\
 * I2C
\*********************************************************************************************/
#ifdef HWDUMP_I2C
void HwDumpI2c(void) {
  char line[256];
  char clk_name[12], hz[20], scl_hz[20];
  char scl_out[24], scl_in[16], sda_out[24], sda_in[16];
  for (uint32_t n = 0; n < SOC_HP_I2C_NUM; n++) {             // HP I2C only, LP_I2C (C5) is not dumped
    if (!HwdI2cOn(n)) {
      snprintf_P(line, sizeof(line), PSTR("I2C%u  peripheral clock disabled or in reset"), n);
      HwdPrint(line);
      continue;
    }
    i2c_dev_t *hw = HwdI2cDev(n);
    uint32_t sel = HwdI2cClkSel(hw);
    uint32_t div = HwdI2cClkDiv(hw);
    uint32_t clk_hz = HwdSrcHz(HwdI2cClkSoc(sel));
    GetTextIndexed(clk_name, sizeof(clk_name), sel, kHwdI2cClkNames);
    HwdFmtHz(hz, sizeof(hz), clk_hz);

    // SCL period in module clock cycles ~ high + low (see i2c_ll_master_cal_bus_clk)
    int high, low;
    i2c_ll_get_scl_timing(hw, &high, &low);
    uint32_t period = (uint32_t)(high + low) + HWD_I2C_SCL_ADJ;
    HwdFmtHz(scl_hz, sizeof(scl_hz), period ? (clk_hz / div / period) : 0);

    __typeof__(HWD_I2C_SR(hw)) sr;   sr.val = HWD_I2C_SR(hw).val;
    snprintf_P(line, sizeof(line), PSTR("I2C%u  mode=%s clk=%s (%s) div=%u scl~%s busy=%s arb_lost=%s rx_fifo=%u tx_fifo=%u fsm=%u/%u int_ena=0x%05X raw=0x%05X"),
      n, HwdI2cIsMaster(hw) ? "master" : "slave", clk_name, hz, div, scl_hz,
      sr.bus_busy ? "Y" : ".", sr.arb_lost ? "Y" : ".", sr.HWD_I2C_RXFIFO_CNT, sr.HWD_I2C_TXFIFO_CNT,
      sr.scl_main_state_last, sr.scl_state_last,
      hw->int_ena.val & HWD_I2C_INT_MASK, hw->int_raw.val & HWD_I2C_INT_MASK);
    HwdPrint(line);

    HwdOutPins(scl_out, sizeof(scl_out), kHwdI2cSig[n][0], nullptr);
    HwdInPin(scl_in, sizeof(scl_in), kHwdI2cSig[n][1], nullptr);
    HwdOutPins(sda_out, sizeof(sda_out), kHwdI2cSig[n][2], nullptr);
    HwdInPin(sda_in, sizeof(sda_in), kHwdI2cSig[n][3], nullptr);
    snprintf_P(line, sizeof(line), PSTR("      pins scl=out:%s in:%s sda=out:%s in:%s"), scl_out, scl_in, sda_out, sda_in);
    HwdPrint(line);
  }
}
#endif  // HWDUMP_I2C

/*********************************************************************************************\
 * RMT
\*********************************************************************************************/
#ifdef HWDUMP_RMT
// ESP32-C3 / ESP32-S3 / ESP32-C5 RMT: dedicated tx channels, then rx channels
// Group clock through HwdRmtSclkSel()/HwdRmtSclkDiv()/HwdRmtMemForcePd() (RMT.sys_conf on C3/S3, PCR on C5),
// decoded with kHwdRmtClkNames/HwdRmtClkSoc(); the fractional part of the group divider is ignored
// HWD_RMT_TX_CONF(ch)/HWD_RMT_TX_STATUS(ch)/HWD_RMT_RX_CONF(i)/HWD_RMT_RX_STATUS(i) select the registers,
// HWD_RMT_TX_F(field)/HWD_RMT_RX_F(field) map the field names (ESP32-S3/C5/C6/P4 add "_chn" / "_chm" suffixes)
// HWD_RMT_SIG_OUT0/HWD_RMT_SIG_IN0 are the GPIO matrix signals of tx channel 0 / rx channel 0 (contiguous for other channels)
void HwDumpRmt(void) {
  char line[200];
  char clk_name[12], hz[20], pins[24];
  if (!HwdRmtOn()) {
    HwdPrint("RMT  peripheral clock disabled or in reset");
    return;
  }
  __typeof__(RMT.sys_conf) sys;   sys.val = RMT.sys_conf.val;
  uint32_t sclk_sel = HwdRmtSclkSel();
  uint32_t sclk_div = HwdRmtSclkDiv();                       // integer part of the group clock divider (div_num + 1)
  uint32_t clk_hz = HwdSrcHz(HwdRmtClkSoc(sclk_sel));
  uint32_t group_hz = clk_hz / sclk_div;
  GetTextIndexed(clk_name, sizeof(clk_name), sclk_sel, kHwdRmtClkNames);
  HwdFmtHz(hz, sizeof(hz), clk_hz);
  char ghz[20];
  HwdFmtHz(ghz, sizeof(ghz), group_hz);
  snprintf_P(line, sizeof(line), PSTR("RMT  clk=%s (%s) div=%u group=%s clk_en=%s mem_force_pd=%s int_ena=0x%04X raw=0x%04X"),
    clk_name, hz, sclk_div, ghz, sys.clk_en ? "Y" : ".", HwdRmtMemForcePd() ? "Y" : ".",
    RMT.int_ena.val & HWD_RMT_INT_MASK, RMT.int_raw.val & HWD_RMT_INT_MASK);
  HwdPrint(line);

  for (uint32_t ch = 0; ch < SOC_RMT_TX_CANDIDATES_PER_GROUP; ch++) {
    __typeof__(HWD_RMT_TX_CONF(0)) conf;   conf.val = HWD_RMT_TX_CONF(ch).val;
    __typeof__(HWD_RMT_TX_STATUS(0)) st;   st.val = HWD_RMT_TX_STATUS(ch).val;
    uint32_t div = rmt_ll_tx_get_channel_clock_div(&RMT, ch);
    uint32_t tick_ns = group_hz ? (uint32_t)(((uint64_t)div * 1000000000ULL + group_hz / 2) / group_hz) : 0;
    HwdOutPins(pins, sizeof(pins), HWD_RMT_SIG_OUT0 + ch, nullptr);
    snprintf_P(line, sizeof(line), PSTR("  ch%u tx  div=%u tick=%u ns idle_out=%s lv=%u carrier=%s loop=%s mem=%u state=%u pins=%s"),
      ch, div, tick_ns, conf.HWD_RMT_TX_F(idle_out_en) ? "Y" : ".", conf.HWD_RMT_TX_F(idle_out_lv),
      conf.HWD_RMT_TX_F(carrier_en) ? "Y" : ".", conf.HWD_RMT_TX_F(tx_conti_mode) ? "Y" : ".", conf.HWD_RMT_TX_F(mem_size),
      st.HWD_RMT_TX_F(state), pins);
    HwdPrint(line);
  }
  for (uint32_t i = 0; i < SOC_RMT_RX_CANDIDATES_PER_GROUP; i++) {
    __typeof__(HWD_RMT_RX_CONF(0).conf0) c0;   c0.val = HWD_RMT_RX_CONF(i).conf0.val;
    __typeof__(HWD_RMT_RX_CONF(0).conf1) c1;   c1.val = HWD_RMT_RX_CONF(i).conf1.val;
    __typeof__(HWD_RMT_RX_STATUS(0)) st;       st.val = HWD_RMT_RX_STATUS(i).val;
    uint32_t div = rmt_ll_rx_get_channel_clock_div(&RMT, i);
    uint32_t tick_ns = group_hz ? (uint32_t)(((uint64_t)div * 1000000000ULL + group_hz / 2) / group_hz) : 0;
    HwdInPin(pins, sizeof(pins), HWD_RMT_SIG_IN0 + i, nullptr);
    snprintf_P(line, sizeof(line), PSTR("  ch%u rx  en=%s div=%u tick=%u ns idle_thres=%u filter=%s/%u carrier=%s mem=%u state=%u pin=%s"),
      SOC_RMT_TX_CANDIDATES_PER_GROUP + i, c1.HWD_RMT_RX_F(rx_en) ? "Y" : ".", div, tick_ns, c0.HWD_RMT_RX_F(idle_thres),
      c1.HWD_RMT_RX_F(rx_filter_en) ? "Y" : ".", c1.HWD_RMT_RX_F(rx_filter_thres), c0.HWD_RMT_RX_F(carrier_en) ? "Y" : ".",
      c0.HWD_RMT_RX_F(mem_size), st.HWD_RMT_RX_F(state), pins);
    HwdPrint(line);
  }
}
#endif  // HWDUMP_RMT

#ifdef HWDUMP_RMT_ESP32
// ESP32 / ESP32-S2 RMT: 8 / 4 channels, each one can transmit or receive, channel clock is APB or REF_TICK (conf1.ref_always_on)
// HWD_RMT_F(field) maps the ESP32 field names to the target names (ESP32-S2 adds a "_chn" suffix)
void HwDumpRmt(void) {
  char line[256];
  char clk_s[20], out_pins[24], in_pin[16];
  if (!HwdRmtOn()) {
    HwdPrint("RMT  peripheral clock disabled or in reset");
    return;
  }
  snprintf_P(line, sizeof(line), PSTR("RMT  mem_access=%s mem_tx_wrap=%s mem_pd=%s int_ena=0x%08X raw=0x%08X"),
    HwdRmtFifoMask() ? "direct" : "fifo", RMT.apb_conf.mem_tx_wrap_en ? "Y" : ".", HwdRmtMemPd() ? "Y" : ".",
    RMT.int_ena.val & HWD_RMT_INT_MASK, RMT.int_raw.val & HWD_RMT_INT_MASK);
  HwdPrint(line);

  uint32_t apb_hz = HwdSrcHz(SOC_MOD_CLK_APB);
  uint32_t ref_hz = HwdSrcHz(SOC_MOD_CLK_REF_TICK);
  for (uint32_t ch = 0; ch < SOC_RMT_CHANNELS_PER_GROUP; ch++) {
    __typeof__(RMT.conf_ch[0].conf0) c0;   c0.val = RMT.conf_ch[ch].conf0.val;
    __typeof__(RMT.conf_ch[0].conf1) c1;   c1.val = RMT.conf_ch[ch].conf1.val;
    uint32_t div = c0.HWD_RMT_F(div_cnt) ? c0.HWD_RMT_F(div_cnt) : 256;     // see rmt_ll_tx_get_channel_clock_div
    uint32_t src_hz = c1.HWD_RMT_F(ref_always_on) ? apb_hz : ref_hz;
    uint32_t tick_ns = src_hz ? (uint32_t)(((uint64_t)div * 1000000000ULL + src_hz / 2) / src_hz) : 0;
    HwdFmtHz(clk_s, sizeof(clk_s), src_hz);
    HwdOutPins(out_pins, sizeof(out_pins), RMT_SIG_OUT0_IDX + ch, nullptr);
    HwdInPin(in_pin, sizeof(in_pin), RMT_SIG_IN0_IDX + ch, nullptr);
    snprintf_P(line, sizeof(line), PSTR("  ch%u clk=%s (%s) div=%u tick=%u ns mem=%u owner=%s tx_start=%s loop=%s idle_out=%s lv=%u carrier=%s rx_en=%s idle_thres=%u filter=%s/%u state=%u out=%s in=%s"),
      ch, c1.HWD_RMT_F(ref_always_on) ? "APB" : "REF_TICK", clk_s, div, tick_ns, c0.HWD_RMT_F(mem_size),
      c1.HWD_RMT_F(mem_owner) ? "rx" : "tx",
      c1.HWD_RMT_F(tx_start) ? "Y" : ".", c1.HWD_RMT_F(tx_conti_mode) ? "Y" : ".",
      c1.HWD_RMT_F(idle_out_en) ? "Y" : ".", c1.HWD_RMT_F(idle_out_lv),
      c0.HWD_RMT_F(carrier_en) ? "Y" : ".", c1.HWD_RMT_F(rx_en) ? "Y" : ".", c0.HWD_RMT_F(idle_thres),
      c1.HWD_RMT_F(rx_filter_en) ? "Y" : ".", c1.HWD_RMT_F(rx_filter_thres), HwdRmtState(ch), out_pins, in_pin);
    HwdPrint(line);
  }
}
#endif  // HWDUMP_RMT_ESP32

/*********************************************************************************************\
 * I2S
\*********************************************************************************************/
#ifdef HWDUMP_I2S
// MCLK = src / (N + b/a), a/b encoded in x/y/z/yn1 (inverse of i2s_ll_tx_set_raw_clk_div / TRM "fractional divider")
uint32_t HwdI2sMclk(uint32_t src_hz, uint32_t n, uint32_t x, uint32_t y, uint32_t z, uint32_t yn1) {
  if (0 == n) { n = 256; }
  uint64_t a = 1, b = 0;
  if (z) {
    a = (uint64_t)(x + 1) * z + y;
    b = yn1 ? (a - z) : z;
  }
  uint64_t den = (uint64_t)n * a + b;
  return den ? (uint32_t)(((uint64_t)src_hz * a + den / 2) / den) : 0;
}

// ESP32-C3 I2S0, ESP32-S3 I2S0/I2S1, ESP32-C5 I2S0: separate tx and rx clock dividers, data goes through GDMA (no FIFO register)
// Clocks through HwdI2sClkEn()/HwdI2sMclkSel()/HwdI2sClkGet() (I2S registers on C3/S3, PCR on C5),
// HWD_I2S_BCK(c, c1) selects the register copy holding tx/rx_bck_div_num (conf1 on C3/S3, conf on C5)
// kHwdI2sSig[unit][]: mclk out, mclk in, tx bck out/in, tx ws out/in, dout, dout1, rx bck out/in, rx ws out/in, din
// (0xFFFF = no such output signal)
void HwDumpI2s(void) {
  char line[256];
  char clk_name[12], mclk_s[20], bck_s[20];
  char p_mclk[24], p_bck[24], p_ws[24], p_sd[24], p_sd1[24];
  for (uint32_t u = 0; u < SOC_I2S_NUM; u++) {
    if (!HwdI2sOn(u)) {
      snprintf_P(line, sizeof(line), PSTR("I2S%u  peripheral clock disabled or in reset"), u);
      HwdPrint(line);
      continue;
    }
    i2s_dev_t *hw = HwdI2sDev(u);
    const uint16_t *sig = kHwdI2sSig[u];
    snprintf_P(line, sizeof(line), PSTR("I2S%u  clk_en=%s mclk_sel=%s int_ena=0x%X raw=0x%X"),
      u, HwdI2sClkEn(u) ? "Y" : ".", HwdI2sMclkSel(u) ? "rx" : "tx",
      hw->int_ena.val & 0xF, hw->int_raw.val & 0xF);
    HwdPrint(line);

    for (uint32_t dir = 0; dir < 2; dir++) {      // 0 = tx, 1 = rx
      bool tx = (0 == dir);
      uint32_t start, slave, mono, pdm, tdm, bits, half, bck_div, clk_sel, clk_active, n, x, y, z, yn1, slots;
      HwdI2sClkGet(u, tx, &clk_sel, &clk_active, &n, &x, &y, &z, &yn1);
      if (tx) {
        __typeof__(hw->tx_conf) c;        c.val = hw->tx_conf.val;
        __typeof__(hw->tx_conf1) c1;      c1.val = hw->tx_conf1.val;
        start = c.tx_start; slave = c.tx_slave_mod; mono = c.tx_mono; pdm = c.tx_pdm_en; tdm = c.tx_tdm_en;
        bits = c1.tx_bits_mod + 1; half = c1.tx_half_sample_bits + 1; bck_div = HWD_I2S_BCK(c, c1).tx_bck_div_num + 1;
        slots = hw->tx_tdm_ctrl.tx_tdm_tot_chan_num + 1;
      } else {
        __typeof__(hw->rx_conf) c;        c.val = hw->rx_conf.val;
        __typeof__(hw->rx_conf1) c1;      c1.val = hw->rx_conf1.val;
        start = c.rx_start; slave = c.rx_slave_mod; mono = c.rx_mono; pdm = c.rx_pdm_en; tdm = c.rx_tdm_en;
        bits = c1.rx_bits_mod + 1; half = c1.rx_half_sample_bits + 1; bck_div = HWD_I2S_BCK(c, c1).rx_bck_div_num + 1;
        slots = hw->rx_tdm_ctrl.rx_tdm_tot_chan_num + 1;
      }
      uint32_t mclk = HwdI2sMclk(HwdSrcHz(HwdI2sClkSoc(clk_sel)), n, x, y, z, yn1);
      uint32_t bck = mclk / bck_div;
      uint32_t fs = bck / (half * 2);         // WS period = 2 half frames
      GetTextIndexed(clk_name, sizeof(clk_name), clk_sel, kHwdI2sClkNames);
      HwdFmtHz(mclk_s, sizeof(mclk_s), mclk);
      HwdFmtHz(bck_s, sizeof(bck_s), bck);
      snprintf_P(line, sizeof(line), PSTR("  %s  start=%s role=%s fmt=%s bits=%u half_frame=%u slots=%u mono=%s clk=%s active=%s mclk=%s bck=%s fs~%u Hz"),
        tx ? "tx" : "rx", start ? "Y" : ".", slave ? "slave" : "master", pdm ? "pdm" : tdm ? "tdm/std" : "std",
        bits, half, slots, mono ? "Y" : ".", clk_name, clk_active ? "Y" : ".", mclk_s, bck_s, fs);
      HwdPrint(line);

      if (tx) {
        HwdOutPins(p_mclk, sizeof(p_mclk), sig[0], nullptr);
        if (slave) {
          HwdInPin(p_bck, sizeof(p_bck), sig[3], nullptr);
          HwdInPin(p_ws, sizeof(p_ws), sig[5], nullptr);
        } else {
          HwdOutPins(p_bck, sizeof(p_bck), sig[2], nullptr);
          HwdOutPins(p_ws, sizeof(p_ws), sig[4], nullptr);
        }
        HwdOutPins(p_sd, sizeof(p_sd), sig[6], nullptr);
        HwdOutPins(p_sd1, sizeof(p_sd1), sig[7], nullptr);      // 0xFFFF prints "-"
        snprintf_P(line, sizeof(line), PSTR("       pins mclk=%s bck=%s ws=%s dout=%s dout1=%s"), p_mclk, p_bck, p_ws, p_sd, p_sd1);
      } else {
        HwdInPin(p_mclk, sizeof(p_mclk), sig[1], nullptr);
        if (slave) {
          HwdInPin(p_bck, sizeof(p_bck), sig[9], nullptr);
          HwdInPin(p_ws, sizeof(p_ws), sig[11], nullptr);
        } else {
          HwdOutPins(p_bck, sizeof(p_bck), sig[8], nullptr);
          HwdOutPins(p_ws, sizeof(p_ws), sig[10], nullptr);
        }
        HwdInPin(p_sd, sizeof(p_sd), sig[12], nullptr);
        snprintf_P(line, sizeof(line), PSTR("       pins mclk_in=%s bck=%s ws=%s din=%s"), p_mclk, p_bck, p_ws, p_sd);
      }
      HwdPrint(line);
    }
  }
}
#endif  // HWDUMP_I2S

#ifdef HWDUMP_I2S_ESP32
// ESP32 I2S0/I2S1, ESP32-S2 I2S0: MCLK = src / (N + b/a) is shared by tx and rx, bck = MCLK / bck_div,
// data goes through DMA (never read the ESP32 fifo_rd register)
void HwDumpI2s(void) {
  char line[256];
  char clk_name[12], src_s[20], mclk_s[20], bck_s[20];
  char p_bck[24], p_ws[24], p_sd[24];
  for (uint32_t n = 0; n < SOC_I2S_NUM; n++) {
    if (!HwdI2sOn(n)) {
      snprintf_P(line, sizeof(line), PSTR("I2S%u  peripheral clock disabled or in reset"), n);
      HwdPrint(line);
      continue;
    }
    i2s_dev_t *hw = HwdI2sDev(n);
    __typeof__(hw->conf) conf;                conf.val = hw->conf.val;
    __typeof__(hw->conf2) conf2;              conf2.val = hw->conf2.val;
    __typeof__(hw->clkm_conf) clkm;           clkm.val = hw->clkm_conf.val;
    __typeof__(hw->sample_rate_conf) rate;    rate.val = hw->sample_rate_conf.val;
#ifdef HWD_I2S_PDM
    __typeof__(hw->pdm_conf) pdm;             pdm.val = hw->pdm_conf.val;
#endif
    __typeof__(hw->conf_chan) chan;           chan.val = hw->conf_chan.val;
    __typeof__(hw->fifo_conf) fifo;           fifo.val = hw->fifo_conf.val;

    // MCLK (see i2s_ll_set_raw_mclk_div), b/a is ignored when a is 0
    uint32_t clk_sel = HwdI2sClkSel(clkm);
    uint32_t src_hz = HwdSrcHz(HwdI2sClkSoc(clk_sel));
    uint32_t nd = clkm.clkm_div_num ? clkm.clkm_div_num : 256;
    uint32_t a = clkm.clkm_div_a, b = clkm.clkm_div_b;
    uint32_t den = nd * a + b;
    uint32_t mclk = a ? (uint32_t)(((uint64_t)src_hz * a + den / 2) / den) : src_hz / nd;
    GetTextIndexed(clk_name, sizeof(clk_name), clk_sel, kHwdI2sClkNames);
    HwdFmtHz(src_s, sizeof(src_s), src_hz);
    HwdFmtHz(mclk_s, sizeof(mclk_s), mclk);
    uint32_t len = snprintf_P(line, sizeof(line), PSTR("I2S%u  clk=%s (%s) clk_en=%s mclk=%s div=%u+%u/%u lcd=%s cam=%s loopback=%s dma=%s int_ena=0x%05X raw=0x%05X"),
      n, clk_name, src_s, clkm.clk_en ? "Y" : ".", mclk_s, nd, b, a,
      conf2.lcd_en ? "Y" : ".", conf2.camera_en ? "Y" : ".", conf.sig_loopback ? "Y" : ".", fifo.dscr_en ? "Y" : ".",
      hw->int_ena.val & HWD_I2S_INT_MASK, hw->int_raw.val & HWD_I2S_INT_MASK);
#ifdef HWD_I2S_MCLK_SIG
    if (len < sizeof(line)) {                   // MCLK output through the GPIO matrix
      char p_mclk[24];
      HwdOutPins(p_mclk, sizeof(p_mclk), HWD_I2S_MCLK_SIG, nullptr);
      snprintf_P(line + len, sizeof(line) - len, PSTR(" mclk_pin=%s"), p_mclk);
    }
#else
    (void)len;
#endif
    HwdPrint(line);

    const uint16_t *sig = kHwdI2sSig[n];
    for (uint32_t dir = 0; dir < 2; dir++) {      // 0 = tx, 1 = rx
      bool tx = (0 == dir);
      uint32_t start = tx ? conf.tx_start : conf.rx_start;
      uint32_t slave = tx ? conf.tx_slave_mod : conf.rx_slave_mod;
      uint32_t mono = tx ? conf.tx_mono : conf.rx_mono;
      uint32_t msb_shift = tx ? conf.tx_msb_shift : conf.rx_msb_shift;
#ifdef HWD_I2S_PDM
      uint32_t is_pdm = tx ? pdm.tx_pdm_en : pdm.rx_pdm_en;
#else
      uint32_t is_pdm = 0;
#endif
      uint32_t bits = tx ? rate.tx_bits_mod : rate.rx_bits_mod;
      uint32_t bck_div = tx ? rate.tx_bck_div_num : rate.rx_bck_div_num;
      uint32_t chan_mod = tx ? chan.tx_chan_mod : chan.rx_chan_mod;
      uint32_t bck = bck_div ? mclk / bck_div : 0;
      uint32_t fs = (!is_pdm && bits) ? bck / (2 * bits) : 0;     // standard mode: 2 channels of `bits` bits
      HwdFmtHz(bck_s, sizeof(bck_s), bck);
      snprintf_P(line, sizeof(line), PSTR("  %s  start=%s role=%s fmt=%s msb_shift=%s bits=%u chan_mod=%u mono=%s bck_div=%u bck=%s fs~%u Hz"),
        tx ? "tx" : "rx", start ? "Y" : ".", slave ? "slave" : "master", is_pdm ? "pdm" : "std",
        msb_shift ? "Y" : ".", bits, chan_mod, mono ? "Y" : ".", bck_div, bck_s, fs);
      HwdPrint(line);

      const uint16_t *s = tx ? sig : sig + 5;     // bck master, bck slave, ws master, ws slave, data
      if (slave) {
        HwdInPin(p_bck, sizeof(p_bck), s[1], nullptr);
        HwdInPin(p_ws, sizeof(p_ws), s[3], nullptr);
      } else {
        HwdOutPins(p_bck, sizeof(p_bck), s[0], nullptr);
        HwdOutPins(p_ws, sizeof(p_ws), s[2], nullptr);
      }
      if (tx) {
        HwdOutPins(p_sd, sizeof(p_sd), s[4], nullptr);
      } else {
        HwdInPin(p_sd, sizeof(p_sd), s[4], nullptr);
      }
      snprintf_P(line, sizeof(line), PSTR("       pins bck=%s ws=%s %s=%s"), p_bck, p_ws, tx ? "dout" : "din", p_sd);
      HwdPrint(line);
    }
  }

  // CLK_OUT1..3 IO_MUX functions, source selected by PIN_CTRL (on ESP32 this is the only way to output MCLK)
  char p1[16], p2[16], p3[16];
  HwdOutPins(p1, sizeof(p1), 0xFFFF, "CLK_OUT1");             // 0xFFFF: no GPIO matrix signal, IO_MUX function only
  HwdOutPins(p2, sizeof(p2), 0xFFFF, "CLK_OUT2");
  HwdOutPins(p3, sizeof(p3), 0xFFFF, "CLK_OUT3");
  snprintf_P(line, sizeof(line), PSTR("  clk_out pin_ctrl=0x%03X CLK_OUT1=%s CLK_OUT2=%s CLK_OUT3=%s"),
    REG_READ(PIN_CTRL) & 0xFFF, p1, p2, p3);
  HwdPrint(line);
}
#endif  // HWDUMP_I2S_ESP32

/*********************************************************************************************\
 * SPI (general purpose SPI only)
\*********************************************************************************************/
#ifdef HWDUMP_SPI
// ESP32-C3 SPI2, ESP32-S3 SPI2/SPI3, ESP32-C5 SPI2 (GPSPI layout), pins are decoded in master direction
// Module clock through HwdSpiClkSel()/HwdSpiClkSoc()/HwdSpiClkPreDiv() (clk_gate.mst_clk_sel on C3/S3, PCR on C5)
// kHwdSpiSig[i][]: sck, d (mosi), q (miso), hd, wp, cs0..csN (0xFFFF = no such signal), kHwdSpiIomux[i][]: sck, d, q, hd, wp, cs0
void HwDumpSpi(void) {
  char line[256];
  char clk_name[12], hz[20], sck_s[20];
  char p_sck[24], p_mosi[24], p_miso[16], p_cs[64], p_hd[24], p_wp[24];
  for (uint32_t i = 0; i < HWD_SPI_NUM; i++) {
    if (!HwdSpiOn(i)) {
      snprintf_P(line, sizeof(line), PSTR("%s  peripheral clock disabled or in reset"), kHwdSpiNames[i]);
      HwdPrint(line);
      continue;
    }
    spi_dev_t *hw = HwdSpiDev(i);
    __typeof__(hw->clock) clk;   clk.val = hw->clock.val;
    __typeof__(hw->user) user;   user.val = hw->user.val;
    __typeof__(hw->misc) misc;   misc.val = hw->misc.val;
    __typeof__(hw->ctrl) ctrl;   ctrl.val = hw->ctrl.val;
    uint32_t sel = HwdSpiClkSel(i);
    uint32_t pre_div = HwdSpiClkPreDiv(i);                 // PCR pre-divider of the module clock (1 on C3/S3)
    uint32_t clk_hz = HwdSrcHz(HwdSpiClkSoc(sel)) / pre_div;
    uint32_t sck = clk.clk_equ_sysclk ? clk_hz : clk_hz / ((clk.clkdiv_pre + 1) * (clk.clkcnt_n + 1));
    GetTextIndexed(clk_name, sizeof(clk_name), sel, kHwdSpiClkNames);
    HwdFmtHz(hz, sizeof(hz), clk_hz);
    HwdFmtHz(sck_s, sizeof(sck_s), sck);
    // SPI mode from clock idle level and output edge, inverse of spi_ll_master_set_mode()
    uint32_t mode = (misc.ck_idle_edge << 1) | (misc.ck_idle_edge ^ user.ck_out_edge);
    bool slave = hw->slave.slave_mode;
    snprintf_P(line, sizeof(line), PSTR("%s  clk_en=%s role=%s mode=%u clk=%s/%u (%s) sck=%s order=%s duplex=%s busy=%s dma_tx=%s dma_rx=%s cs_dis=0x%02X int_ena=0x%X raw=0x%X"),
      kHwdSpiNames[i], hw->clk_gate.clk_en ? "Y" : ".", slave ? "slave" : "master", mode, clk_name, pre_div, hz, sck_s,
      ctrl.wr_bit_order ? "LSB" : "MSB", user.doutdin ? "full" : "half", hw->cmd.usr ? "Y" : ".",
      hw->dma_conf.dma_tx_ena ? "Y" : ".", hw->dma_conf.dma_rx_ena ? "Y" : ".",
      misc.val & 0x3F,                                  // bits 0..5 = cs0_dis..cs5_dis
      hw->dma_int_ena.val, hw->dma_int_raw.val);
    HwdPrint(line);

    const uint16_t *s = kHwdSpiSig[i];
    const char * const *mx = kHwdSpiIomux[i];
    HwdOutPins(p_sck, sizeof(p_sck), s[0], mx[0]);
    HwdOutPins(p_mosi, sizeof(p_mosi), s[1], mx[1]);
    HwdInPin(p_miso, sizeof(p_miso), s[2], mx[2]);
    HwdOutPins(p_hd, sizeof(p_hd), s[3], mx[3]);
    HwdOutPins(p_wp, sizeof(p_wp), s[4], mx[4]);
    uint32_t len = 0;
    p_cs[0] = 0;
    for (uint32_t c = 0; c < HWD_SPI_CS_NUM; c++) {
      char p[16];
      HwdOutPins(p, sizeof(p), s[5 + c], (0 == c) ? mx[5] : nullptr);
      if (strcmp(p, "-") && len < sizeof(p_cs)) {
        len += snprintf_P(p_cs + len, sizeof(p_cs) - len, PSTR("%scs%u:%s"), len ? " " : "", c, p);
      }
    }
    if (!p_cs[0]) { strlcpy(p_cs, "-", sizeof(p_cs)); }
    snprintf_P(line, sizeof(line), PSTR("      pins sck=%s mosi=%s miso=%s hd=%s wp=%s cs=%s"), p_sck, p_mosi, p_miso, p_hd, p_wp, p_cs);
    HwdPrint(line);
  }
}
#endif  // HWDUMP_SPI

#ifdef HWDUMP_SPI_ESP32
// ESP32 SPI2 (HSPI) / SPI3 (VSPI), ESP32-S2 SPI2 (FSPI) / SPI3: clock is APB, SPI mode and sck are decoded for master only
// HWD_SPI_PIN(hw) is the register holding ck_idle_edge and csN_dis (ESP32 SPI_PIN_REG, ESP32-S2 SPI_MISC_REG)
void HwDumpSpi(void) {
  char line[256];
  char mode_s[4], sck_s[20];
  char p_sck[24], p_mosi[24], p_miso[24], p_hd[24], p_wp[24], p_cs[64];
  uint32_t apb_hz = HwdSrcHz(SOC_MOD_CLK_APB);
  for (uint32_t i = 0; i < HWD_SPI_NUM; i++) {
    if (!HwdSpiOn(i)) {
      snprintf_P(line, sizeof(line), PSTR("%s  peripheral clock disabled or in reset"), kHwdSpiNames[i]);
      HwdPrint(line);
      continue;
    }
    spi_dev_t *hw = HwdSpiDev(i);
    __typeof__(hw->clock) clk;    clk.val = hw->clock.val;
    __typeof__(hw->user) user;    user.val = hw->user.val;
    __typeof__(HWD_SPI_PIN(hw)) pin;   pin.val = HWD_SPI_PIN(hw).val;
    __typeof__(hw->ctrl) ctrl;    ctrl.val = hw->ctrl.val;
    __typeof__(hw->slave) slv;    slv.val = hw->slave.val;
    bool slave = slv.slave_mode;
    if (slave) {
      strlcpy(mode_s, "-", sizeof(mode_s));
      strlcpy(sck_s, "-", sizeof(sck_s));
    } else {
      // SPI mode from clock idle level and output edge, inverse of spi_ll_master_set_mode()
      snprintf_P(mode_s, sizeof(mode_s), PSTR("%u"), (pin.ck_idle_edge << 1) | (pin.ck_idle_edge ^ user.ck_out_edge));
      uint32_t sck = clk.clk_equ_sysclk ? apb_hz : apb_hz / ((clk.clkdiv_pre + 1) * (clk.clkcnt_n + 1));
      HwdFmtHz(sck_s, sizeof(sck_s), sck);
    }
    snprintf_P(line, sizeof(line), PSTR("%s  role=%s mode=%s clk=APB sck=%s order=%s duplex=%s busy=%s cs_dis=0x%X trans_done=%s trans_inten=%s dma_int_ena=0x%03X raw=0x%03X"),
      kHwdSpiNames[i], slave ? "slave" : "master", mode_s, sck_s,
      ctrl.wr_bit_order ? "LSB" : "MSB", user.doutdin ? "full" : "half", hw->cmd.usr ? "Y" : ".",
      pin.val & ((1 << HWD_SPI_CS_NUM) - 1),           // bits 0..n = cs0_dis..csn_dis
      slv.trans_done ? "Y" : ".", HwdSpiTransInten(slv) ? "Y" : ".",
      hw->dma_int_ena.val & HWD_SPI_DMA_INT_MASK, hw->dma_int_raw.val & HWD_SPI_DMA_INT_MASK);
    HwdPrint(line);

    const uint16_t *s = kHwdSpiSig[i];                  // sck, d, q, hd, wp, cs0..csN (0xFFFF = no such signal)
    const char * const *mx = kHwdSpiIomux[i];           // sck, d, q, hd, wp, cs0
    if (slave) {
      HwdInPin(p_sck, sizeof(p_sck), s[0], mx[0]);
      HwdInPin(p_mosi, sizeof(p_mosi), s[1], mx[1]);
      HwdOutPins(p_miso, sizeof(p_miso), s[2], mx[2]);
      strlcpy(p_hd, "-", sizeof(p_hd));
      strlcpy(p_wp, "-", sizeof(p_wp));
      HwdInPin(p_cs, sizeof(p_cs), s[5], mx[5]);
    } else {
      HwdOutPins(p_sck, sizeof(p_sck), s[0], mx[0]);
      HwdOutPins(p_mosi, sizeof(p_mosi), s[1], mx[1]);
      HwdInPin(p_miso, sizeof(p_miso), s[2], mx[2]);
      HwdOutPins(p_hd, sizeof(p_hd), s[3], mx[3]);
      HwdOutPins(p_wp, sizeof(p_wp), s[4], mx[4]);
      uint32_t len = 0;
      p_cs[0] = 0;
      for (uint32_t c = 0; c < HWD_SPI_CS_NUM; c++) {
        char p[16];
        HwdOutPins(p, sizeof(p), s[5 + c], (0 == c) ? mx[5] : nullptr);
        if (strcmp(p, "-") && len < sizeof(p_cs)) {
          len += snprintf_P(p_cs + len, sizeof(p_cs) - len, PSTR("%scs%u:%s"), len ? " " : "", c, p);
        }
      }
      if (!p_cs[0]) { strlcpy(p_cs, "-", sizeof(p_cs)); }
    }
    snprintf_P(line, sizeof(line), PSTR("      pins sck=%s mosi=%s miso=%s hd=%s wp=%s cs=%s"), p_sck, p_mosi, p_miso, p_hd, p_wp, p_cs);
    HwdPrint(line);
  }
}
#endif  // HWDUMP_SPI_ESP32

/*********************************************************************************************\
 * RTC_IO / LP_IO and sleep configuration
\*********************************************************************************************/
#if defined(HWDUMP_SLEEP) || defined(HWDUMP_RTCIO) || defined(HWDUMP_LPIO)
const char kHwdIntTypeNames[] PROGMEM = "-|rise|fall|any|low|high|6|7";        // gpio_int_type_t
#endif

#ifdef HWDUMP_RTCIO
// Return "Y"/"." for bit `mask` of `val`, "-" when the pad has no such bit (mask 0 in rtc_io_desc[])
const char* HwdBitYN(uint32_t val, uint32_t mask) {
  return mask ? ((val & mask) ? "Y" : ".") : "-";
}

// RTC_IO pads, decoded through rtc_io_desc[] (same masks as rtc_io_ll.h)
void HwDumpRtcIo(void) {
  char line[200];
  char name[8], drv[4];
  uint32_t hold_force = REG_READ(HWD_RTCIO_HOLD_FORCE_REG);       // register of rtc_io_desc[].hold_force bits
  uint32_t oe = RTCIO.enable.val >> RTC_GPIO_ENABLE_S;
  uint32_t out = RTCIO.out.val >> RTC_GPIO_OUT_DATA_S;
  uint32_t in = RTCIO.in_val.val >> RTC_GPIO_IN_NEXT_S;
  HwdPrint("RTC_IO  mux=rtc: pad controlled by RTC_IO (IO_MUX/GPIO matrix settings do not apply), fun=RTC function (0=RTC_GPIO), force=RTC_CNTL hold force, -=not available on this pad");
  HwdPrint(" rtc gpio pad         mux fun  ie  oe  od  pu  pd drv slp_sel slp_ie slp_oe hold force  o  i  int wake");
  for (uint32_t r = 0; r < SOC_RTCIO_PIN_COUNT; r++) {
    const rtc_io_desc_t *d = &rtc_io_desc[r];
    if (!d->reg) { continue; }
    uint32_t v = REG_READ(d->reg);
    __typeof__(RTCIO.pin[0]) p;   p.val = RTCIO.pin[r].val;
    if (d->drv_v) {
      snprintf_P(drv, sizeof(drv), PSTR("%u"), (v >> d->drv_s) & d->drv_v);     // see rtcio_ll_get_drive_capability
    } else {
      strlcpy(drv, "-", sizeof(drv));
    }
    GetTextIndexed(name, sizeof(name), p.int_type, kHwdIntTypeNames);
    bool valid = (d->rtc_num >= 0) && (d->rtc_num < SOC_GPIO_PIN_COUNT);
    snprintf_P(line, sizeof(line), PSTR("%4u %4d %-11s %3s %3u %3s %3s %3s %3s %3s %3s %7s %6s %6s %4s %5s %2u %2u %4s %4s"),
      r, d->rtc_num, valid ? kHwdPads[d->rtc_num].pad : "?",
      (v & d->mux) ? "rtc" : "dig", (v >> d->func) & 0x3,                     // see rtcio_ll_iomux_func_sel
      HwdBitYN(v, d->ie), ((oe >> r) & 1) ? "Y" : ".", p.pad_driver ? "Y" : ".",
      HwdBitYN(v, d->pullup), HwdBitYN(v, d->pulldown), drv,
      HwdBitYN(v, d->slpsel), HwdBitYN(v, d->slpie), HwdBitYN(v, d->slpoe),
      HwdBitYN(v, d->hold), HwdBitYN(hold_force, d->hold_force),
      (out >> r) & 1, (in >> r) & 1, name, p.wakeup_enable ? "Y" : ".");
    HwdPrint(line);
  }
}
#endif  // HWDUMP_RTCIO

#ifdef HWDUMP_LPIO
// ESP32-C5 / ESP32-C6 / ESP32-P4 LP_IO pads (LP IO n = GPIO n): pad config, data/enable/wakeup, mux select and hold
// HWD_LPIO_MUX(r)/HWD_LPIO_PIN(r) select the pad config and pin registers, HWD_LPIO_MUX_F(f)/HWD_LPIO_PIN_F(f) map the
// field names (ESP32-C5 LP_IO_MUX.gpion[] / LP_GPIO.pinn[] with "gpion_" / "pinn_" prefixes, ESP32-C6 LP_IO.gpio[] / LP_IO.pin[],
// ESP32-P4 LP_IOMUX.pad[] / LP_GPIO.pin[] with per field names)
// HWD_LPIO_SLP_PULL: 1 if the pad has separate sleep pull-up/pull-down/drive fields (mcu_wpu/mcu_wpd/mcu_drv), else printed "-"
// HWD_LPIO_FUNC_GPIO: LP IO_MUX function number of LP_GPIO (RTCIO_LL_PIN_FUNC), HwdLpIoHoldMask(): pad hold bits indexed by GPIO
// All registers below are configuration/status registers without read side effects
void HwDumpLpIo(void) {
  char line[200];
  char name[8];
  char slp_pu[2], slp_pd[2], slp_drv[4];
  if (!HwdLpIoOn()) {
    HwdPrint("LP_IO  peripheral clock disabled or in reset");
    return;
  }
  uint32_t mux_sel = HwdLpIoMuxSel();                       // 1 = pad connected to LP_IO
  uint32_t hold = HwdLpIoHoldMask();
  uint32_t oe = HwdLpIoOe();
  uint32_t out = HwdLpIoOut();
  uint32_t in = HwdLpIoIn();
  snprintf_P(line, sizeof(line), PSTR("LP_IO  mux=lp: pad controlled by LP_IO (IO_MUX/GPIO matrix settings do not apply), fun=LP IO_MUX function (%u=LP_GPIO), slp_*=sleep settings (used when slp_sel), -=not available, int_clk=%s"),
    HWD_LPIO_FUNC_GPIO, HwdLpIoIntClk() ? "Y" : ".");
  HwdPrint(line);
  HwdPrint("  lp gpio pad         mux fun  ie  oe  od  pu  pd drv slp_sel slp_ie slp_oe slp_pu slp_pd slp_drv hold  o  i  int wake");
  for (uint32_t r = 0; r < SOC_RTCIO_PIN_COUNT; r++) {
    __typeof__(HWD_LPIO_MUX(0)) m;   m.val = HWD_LPIO_MUX(r).val;
    __typeof__(HWD_LPIO_PIN(0)) p;   p.val = HWD_LPIO_PIN(r).val;
    uint32_t gpio = HwdRtcGpio(r);
    GetTextIndexed(name, sizeof(name), p.HWD_LPIO_PIN_F(int_type), kHwdIntTypeNames);
#if HWD_LPIO_SLP_PULL
    strlcpy(slp_pu, m.HWD_LPIO_MUX_F(mcu_wpu) ? "Y" : ".", sizeof(slp_pu));
    strlcpy(slp_pd, m.HWD_LPIO_MUX_F(mcu_wpd) ? "Y" : ".", sizeof(slp_pd));
    snprintf_P(slp_drv, sizeof(slp_drv), PSTR("%u"), (uint32_t)m.HWD_LPIO_MUX_F(mcu_drv));
#else
    strlcpy(slp_pu, "-", sizeof(slp_pu));
    strlcpy(slp_pd, "-", sizeof(slp_pd));
    strlcpy(slp_drv, "-", sizeof(slp_drv));
#endif
    snprintf_P(line, sizeof(line), PSTR("%4u %4u %-11s %3s %3u %3s %3s %3s %3s %3s %3u %7s %6s %6s %6s %6s %7s %4s %2u %2u %4s %4s"),
      r, gpio, (gpio < SOC_GPIO_PIN_COUNT) ? kHwdPads[gpio].pad : "?",
      (mux_sel & BIT(r)) ? "lp" : "dig", m.HWD_LPIO_MUX_F(mcu_sel),
      m.HWD_LPIO_MUX_F(fun_ie) ? "Y" : ".", ((oe >> r) & 1) ? "Y" : ".", p.HWD_LPIO_PIN_F(pad_driver) ? "Y" : ".",
      m.HWD_LPIO_MUX_F(fun_wpu) ? "Y" : ".", m.HWD_LPIO_MUX_F(fun_wpd) ? "Y" : ".", m.HWD_LPIO_MUX_F(fun_drv),
      m.HWD_LPIO_MUX_F(slp_sel) ? "Y" : ".", m.HWD_LPIO_MUX_F(mcu_ie) ? "Y" : ".", m.HWD_LPIO_MUX_F(mcu_oe) ? "Y" : ".",
      slp_pu, slp_pd, slp_drv,
      ((hold >> gpio) & 1) ? "Y" : ".", (out >> r) & 1, (in >> r) & 1, name, p.HWD_LPIO_PIN_F(wakeup_enable) ? "Y" : ".");
    HwdPrint(line);
  }
}
#endif  // HWDUMP_LPIO

#ifdef HWDUMP_SLEEP
const char kHwdSleepCauseNames[] PROGMEM =                                      // esp_sleep_source_t
  "undefined|all|ext0|ext1|timer|touchpad|ulp|gpio|uart0|uart1|uart2|wifi|cocpu|cocpu_trap|bt|vad|vbat_uv|usb";

#ifdef HWDUMP_SLEEP_EXT
// Append the GPIO numbers of the RTC pads set in `rtc_mask`, returns new length
uint32_t HwdAppendRtcGpios(char *line, size_t size, uint32_t len, uint32_t rtc_mask) {
  bool any = false;
  for (uint32_t r = 0; r < SOC_RTCIO_PIN_COUNT && len < size; r++) {
    if (!(rtc_mask & BIT(r))) { continue; }
    len += snprintf_P(line + len, size - len, PSTR(" %d"), HwdRtcGpio(r));
    any = true;
  }
  if (!any && len < size) { len += snprintf_P(line + len, size - len, PSTR(" none")); }
  return len;
}
#endif  // HWDUMP_SLEEP_EXT

void HwDumpSleep(void) {
  char line[256];
  char name[16];
  uint32_t len;

  // RTC_IO / LP_IO (dumped by HwDumpRtcIo() when the SoC has RTC_IO pads)
#if SOC_RTCIO_PIN_COUNT == 0
  HwdPrint("RTC_IO  none on this SoC (SOC_RTCIO_PIN_COUNT=0), RTC domain pads are listed below with their deep-sleep wakeup");
#endif

  // Last wakeup and power management
  GetTextIndexed(name, sizeof(name), esp_sleep_get_wakeup_cause(), kHwdSleepCauseNames);
  len = snprintf_P(line, sizeof(line), PSTR("SLEEP  last_wakeup=%s causes=0x%05X"), name, esp_sleep_get_wakeup_causes());
  esp_pm_config_t pm = {};
  if (ESP_OK == esp_pm_get_configuration(&pm)) {
    snprintf_P(line + len, sizeof(line) - len, PSTR(" pm=enabled max=%d MHz min=%d MHz light_sleep=%s"),
      pm.max_freq_mhz, pm.min_freq_mhz, pm.light_sleep_enable ? "Y" : ".");
  } else {
    snprintf_P(line + len, sizeof(line) - len, PSTR(" pm=disabled"));
  }
  HwdPrint(line);

  // Wakeup sources programmed in RTC_CNTL / PMU (as set for the last or next sleep)
  uint32_t ena = HwdWakeupEna();
  len = snprintf_P(line, sizeof(line), PSTR("  wakeup_ena=0x%05X"), ena);
  for (uint32_t bit = 0; bit < HWD_WAKEUP_BITS && len < sizeof(line); bit++) {
    if (!(ena & BIT(bit))) { continue; }
    GetTextIndexed(name, sizeof(name), bit, kHwdWakeupNames);
    if (name[0]) {
      len += snprintf_P(line + len, sizeof(line) - len, PSTR(" %s"), name);
    } else {
      len += snprintf_P(line + len, sizeof(line) - len, PSTR(" bit%u"), bit);
    }
  }
  HwdPrint(line);

#ifdef HWDUMP_SLEEP_EXT
  // ext0: one RTC pad at a given level, ext1: set of RTC/LP pads, programmed by IDF when entering sleep
  // HwdRtcGpio(r) converts an RTC_IO / LP_IO index to its GPIO number
#ifdef SOC_PM_SUPPORT_EXT0_WAKEUP
  uint32_t ext0 = HwdExt0Rtc();
  snprintf_P(line, sizeof(line), PSTR("  ext0 en=%s gpio=%d level=%s"),
    (ena & HWD_WAKEUP_EXT0_EN) ? "Y" : ".", (ext0 < SOC_RTCIO_PIN_COUNT) ? HwdRtcGpio(ext0) : -1,
    HwdExt0High() ? "high" : "low");
  HwdPrint(line);
#endif  // SOC_PM_SUPPORT_EXT0_WAKEUP
#ifdef SOC_PM_SUPPORT_EXT1_WAKEUP_MODE_PER_PIN
  // one trigger level per pad: "high" lists the pads waking up on high level, the others wake up on low level
  len = snprintf_P(line, sizeof(line), PSTR("  ext1 en=%s gpios:"), (ena & HWD_WAKEUP_EXT1_EN) ? "Y" : ".");
  len = HwdAppendRtcGpios(line, sizeof(line), len, HwdExt1Mask());
  if (len < sizeof(line)) {
    len += snprintf_P(line + len, sizeof(line) - len, PSTR("  high:"));
    len = HwdAppendRtcGpios(line, sizeof(line), len, HwdExt1Mask() & HwdExt1HighMask());
  }
#else
  // one mode for all pads: any high or all low
  len = snprintf_P(line, sizeof(line), PSTR("  ext1 en=%s mode=%s gpios:"),
    (ena & HWD_WAKEUP_EXT1_EN) ? "Y" : ".", HwdExt1AnyHigh() ? "any_high" : "all_low");
  len = HwdAppendRtcGpios(line, sizeof(line), len, HwdExt1Mask());
#endif  // SOC_PM_SUPPORT_EXT1_WAKEUP_MODE_PER_PIN
  if (len < sizeof(line)) {
    len += snprintf_P(line + len, sizeof(line) - len, PSTR("  status:"));
    HwdAppendRtcGpios(line, sizeof(line), len, HwdExt1Status());
  }
  HwdPrint(line);
#endif  // HWDUMP_SLEEP_EXT

#ifdef HWDUMP_SLEEP_DG_PAD_HOLD
  // RTC_CNTL_DIG_ISO_REG: digital pad hold status and controls
  snprintf_P(line, sizeof(line), PSTR("  pad_hold autohold=%s autohold_en=%s force_hold=%s"),
    HwdDgPadAutohold() ? "Y" : ".", HwdDgPadAutoholdEn() ? "Y" : ".", HwdDgPadForceHold() ? "Y" : ".");
  HwdPrint(line);
#endif  // HWDUMP_SLEEP_DG_PAD_HOLD
#ifdef HWDUMP_SLEEP_PMU_PAD_HOLD
  // per-pad hold mask (bit n = GPIOn), and pad hold settings applied by the PMU in HP_SLEEP mode
  uint64_t hold_mask = HwdPadHoldMask();
  char hold_s[20];
  if (SOC_GPIO_PIN_COUNT > 32) {
    snprintf_P(hold_s, sizeof(hold_s), PSTR("0x%08X%08X"), (uint32_t)(hold_mask >> 32), (uint32_t)hold_mask);
  } else {
    snprintf_P(hold_s, sizeof(hold_s), PSTR("0x%08X"), (uint32_t)hold_mask);
  }
  snprintf_P(line, sizeof(line), PSTR("  pad_hold mask=%s sleep: hp_pad_hold_all=%s lp_pad_hold_all=%s dig_pad_slp_sel=%s"),
    hold_s, HwdSlpHpPadHoldAll() ? "Y" : ".", HwdSlpLpPadHoldAll() ? "Y" : ".", HwdSlpDigPadSlpSel() ? "Y" : ".");
  HwdPrint(line);
#endif  // HWDUMP_SLEEP_PMU_PAD_HOLD

#if SOC_GPIO_SUPPORT_DEEPSLEEP_WAKEUP
  len = snprintf_P(line, sizeof(line), PSTR("  deep_sleep_gpio_wakeup clk=%s pins:"), HwdDeepSleepWakeClk() ? "Y" : ".");
  bool any = false;
  for (uint32_t pin = 0; pin < SOC_GPIO_PIN_COUNT && len < sizeof(line); pin++) {
    if (!(SOC_GPIO_DEEP_SLEEP_WAKE_VALID_GPIO_MASK & BIT64(pin))) { continue; }
    if (!HwdDeepSleepWakeEn(pin)) { continue; }
    GetTextIndexed(name, sizeof(name), HwdDeepSleepWakeType(pin), kHwdIntTypeNames);
    len += snprintf_P(line + len, sizeof(line) - len, PSTR(" %u:%s"), pin, name);
    any = true;
  }
  if (!any && len < sizeof(line)) { snprintf_P(line + len, sizeof(line) - len, PSTR(" none")); }
  HwdPrint(line);
#endif  // SOC_GPIO_SUPPORT_DEEPSLEEP_WAKEUP

#ifdef USE_HWDUMP_SLEEP_PINS
  // Per pin sleep configuration (IO_MUX SLP_* bits apply only when slp_sel is set and the chip is in light sleep)
  HwdPrint(" pin rtc slp_sel slp_oe slp_ie slp_pu slp_pd slp_drv hold ls_wake int");
  for (uint32_t pin = 0; pin < SOC_GPIO_PIN_COUNT; pin++) {
    if (!GPIO_IS_VALID_GPIO((int)pin)) { continue; }
    if (FlashPin(pin)) { continue; }
    uint32_t mux = REG_READ(GPIO_PIN_MUX_REG[pin]);
    GetTextIndexed(name, sizeof(name), HwdGpioIntType(pin), kHwdIntTypeNames);
    snprintf_P(line, sizeof(line), PSTR("%4u %3s %7s %6s %6s %6s %6s %7u %4s %7s %3s"),
      pin,
      HwdPinIsRtc(pin) ? "Y" : ".",
      (mux & SLP_SEL_M) ? "Y" : ".",
      (mux & SLP_OE_M) ? "Y" : ".",
      (mux & SLP_IE_M) ? "Y" : ".",
      (mux & SLP_PU_M) ? "Y" : ".",
      (mux & SLP_PD_M) ? "Y" : ".",
      (mux & SLP_DRV_M) >> SLP_DRV_S,
      HwdPadHold(pin) ? "Y" : ".",
      HwdGpioWakeupEn(pin) ? "Y" : ".",
      name);
    HwdPrint(line);
  }
#endif  // USE_HWDUMP_SLEEP_PINS
}
#endif  // HWDUMP_SLEEP

#endif  // HWDUMP_SUPPORTED

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

const char kHwDumpCommands[] PROGMEM = "|"  // No prefix
  "HwDump";

void (* const HwDumpCommand[])(void) PROGMEM = {
  &CmndHwDump };

void CmndHwDump(void) {
#ifdef HWDUMP_SUPPORTED
  HwDumpGpio();
#ifdef HWDUMP_LEDC
  HwDumpLedc();
#endif  // HWDUMP_LEDC
#if defined(HWDUMP_RMT) || defined(HWDUMP_RMT_ESP32)
  HwDumpRmt();
#endif  // HWDUMP_RMT
#if defined(HWDUMP_I2S) || defined(HWDUMP_I2S_ESP32)
  HwDumpI2s();
#endif  // HWDUMP_I2S
#if defined(HWDUMP_SPI) || defined(HWDUMP_SPI_ESP32)
  HwDumpSpi();
#endif  // HWDUMP_SPI
#ifdef HWDUMP_I2C
  HwDumpI2c();
#endif  // HWDUMP_I2C
#ifdef HWDUMP_UART
  HwDumpUart();
#endif  // HWDUMP_UART
#ifdef HWDUMP_USB_SERIAL_JTAG
  HwDumpUsbSerialJtag();
#endif  // HWDUMP_USB_SERIAL_JTAG
#ifdef HWDUMP_RTCIO
  HwDumpRtcIo();
#endif  // HWDUMP_RTCIO
#ifdef HWDUMP_LPIO
  HwDumpLpIo();
#endif  // HWDUMP_LPIO
#ifdef HWDUMP_SLEEP
  HwDumpSleep();
#endif  // HWDUMP_SLEEP
  ResponseCmndDone();
#else
  ResponseCmndChar_P(PSTR("Target not supported"));
#endif  // HWDUMP_SUPPORTED
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv126(uint32_t function) {
  bool result = false;

  switch (function) {
    case FUNC_COMMAND:
      result = DecodeCommand(kHwDumpCommands, HwDumpCommand);
      break;
    case FUNC_ACTIVE:
      result = true;
      break;
  }
  return result;
}

#endif  // USE_HWDUMP
#endif  // ESP32
