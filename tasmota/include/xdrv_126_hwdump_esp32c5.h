/*
  xdrv_126_hwdump_esp32c5.h - HwDump per-target data and register accessors for ESP32-C5

  SPDX-FileCopyrightText: 2026 Stephan Hadinger

  SPDX-License-Identifier: GPL-3.0-only
*/

// Only included from tasmota_xdrv_driver/xdrv_126_hwdump.ino, after HwdPad_t, HwdSig_t and HWD_SIG() are defined.
// Defines data (not only declarations), so it must be included exactly once.
#pragma once

#if CONFIG_IDF_TARGET_ESP32C5

/*********************************************************************************************\
 * ESP32-C5
 *
 * Bus clock, reset and most clock selectors/dividers are in the PCR (Power/Clock/Reset) registers, not in the
 * peripheral itself (UART sclk, I2C sclk, RMT group clock, LEDC clock, I2S tx/rx clocks, SPI2 module clock).
 * Peripheral register layouts are close to ESP32-C3/S3 (LEDC, RMT, I2S, SPI, UART, I2C, USB-Serial-JTAG), the shared
 * C3-layout dump functions are used with the hooks below.
 * LP_IO (GPIO0-6, LP IO n = GPIO n) replaces RTC_IO, sleep configuration is in PMU / LP_AON (no RTC_CNTL).
 * LP_UART, LP_I2C, TWAI, PARLIO, MCPWM and PCNT are not dumped.
\*********************************************************************************************/
#define HWDUMP_SUPPORTED
#include "hal/misc.h"                 // HAL_FORCE_READ_U32_REG_FIELD()
#include "hal/pmu_types.h"            // PMU_MODE_HP_SLEEP
#include "soc/pcr_struct.h"           // PCR
#include "soc/lp_aon_struct.h"        // LP_AON
#include "soc/lp_gpio_struct.h"       // LP_GPIO
#include "soc/lp_iomux_struct.h"      // LP_IO_MUX
#include "soc/lpperi_struct.h"        // LPPERI
#include "soc/pmu_struct.h"           // PMU
#include "soc/rtc_io_channel.h"       // RTCIO_CHANNEL_n_GPIO_NUM

// Pad names and IO_MUX functions from soc/io_mux_reg.h (FUNC_<pad>_<function> = MCU_SEL value)
const HwdPad_t kHwdPads[] = {
  { "XTAL_32K_P", "GPIO0_0|GPIO0" },              //  0
  { "XTAL_32K_N", "GPIO1_0|GPIO1" },              //  1
  { "MTMS",       "MTMS|GPIO2|FSPIQ" },           //  2
  { "MTDI",       "MTDI|GPIO3" },                 //  3
  { "MTCK",       "MTCK|GPIO4|FSPIHD" },          //  4
  { "MTDO",       "MTDO|GPIO5|FSPIWP" },          //  5
  { "GPIO6",      "GPIO6_0|GPIO6|FSPICLK" },      //  6
  { "GPIO7",      "SDIO_DATA1|GPIO7|FSPID" },     //  7
  { "GPIO8",      "GPIO8_0|GPIO8" },              //  8
  { "GPIO9",      "GPIO9_0|GPIO9" },              //  9
  { "GPIO10",     "SDIO_CMD|GPIO10|FSPICS0" },    // 10
  { "U0TXD",      "U0TXD|GPIO11" },               // 11
  { "U0RXD",      "U0RXD|GPIO12" },               // 12
  { "GPIO13",     "SDIO_DATA3|GPIO13" },          // 13
  { "GPIO14",     "SDIO_DATA2|GPIO14" },          // 14
  { "SPICS1",     "SPICS1|GPIO15" },              // 15
  { "SPICS0",     "SPICS0|GPIO16" },              // 16
  { "SPIQ",       "SPIQ|GPIO17" },                // 17
  { "SPIWP",      "SPIWP|GPIO18" },               // 18
  { "VDD_SPI",    "GPIO19_0|GPIO19" },            // 19
  { "SPIHD",      "SPIHD|GPIO20" },               // 20
  { "SPICLK",     "SPICLK|GPIO21" },              // 21
  { "SPID",       "SPID|GPIO22" },                // 22
  { "GPIO23",     "GPIO23_0|GPIO23" },            // 23
  { "GPIO24",     "GPIO24_0|GPIO24" },            // 24
  { "GPIO25",     "GPIO25_0|GPIO25" },            // 25
  { "GPIO26",     "GPIO26_0|GPIO26" },            // 26
  { "GPIO27",     "GPIO27_0|GPIO27" },            // 27
  { "GPIO28",     "GPIO28_0|GPIO28" },            // 28
};

// Sanity checks of the function indexes above against ESP-IDF definitions
static_assert(PIN_FUNC_GPIO == 1, "HwDump: unexpected PIN_FUNC_GPIO");
static_assert(FUNC_XTAL_32K_P_GPIO0_0 == 0 && FUNC_XTAL_32K_P_GPIO0 == 1, "HwDump: IO_MUX pad 0");
static_assert(FUNC_MTMS_MTMS == 0 && FUNC_MTMS_FSPIQ == 2, "HwDump: IO_MUX pad 2");
static_assert(FUNC_MTCK_MTCK == 0 && FUNC_MTCK_FSPIHD == 2, "HwDump: IO_MUX pad 4");
static_assert(FUNC_MTDO_MTDO == 0 && FUNC_MTDO_FSPIWP == 2, "HwDump: IO_MUX pad 5");
static_assert(FUNC_GPIO6_GPIO6_0 == 0 && FUNC_GPIO6_FSPICLK == 2, "HwDump: IO_MUX pad 6");
static_assert(FUNC_GPIO7_SDIO_DATA1 == 0 && FUNC_GPIO7_FSPID == 2, "HwDump: IO_MUX pad 7");
static_assert(FUNC_GPIO10_SDIO_CMD == 0 && FUNC_GPIO10_FSPICS0 == 2, "HwDump: IO_MUX pad 10");
static_assert(FUNC_U0TXD_U0TXD == 0 && FUNC_U0RXD_U0RXD == 0, "HwDump: IO_MUX pads 11-12");
static_assert(FUNC_GPIO13_SDIO_DATA3 == 0 && FUNC_GPIO14_SDIO_DATA2 == 0, "HwDump: IO_MUX pads 13-14");
static_assert(FUNC_SPICS1_SPICS1 == 0 && FUNC_SPID_SPID == 0 && FUNC_SPIHD_SPIHD == 0, "HwDump: IO_MUX pads 15-22");
static_assert(FUNC_GPIO28_GPIO28_0 == 0 && FUNC_GPIO28_GPIO28 == 1, "HwDump: IO_MUX pad 28");

// GPIO matrix output signals (GPIO_FUNCn_OUT_SEL), from soc/gpio_sig_map.h
// (C5 gpio_sig_map.h mixes input and output names: same index = input signal / output signal)
const HwdSig_t kHwdSigOut[] = {
  HWD_SIG(LEDC_LS_SIG_OUT0), HWD_SIG(LEDC_LS_SIG_OUT1), HWD_SIG(LEDC_LS_SIG_OUT2), HWD_SIG(LEDC_LS_SIG_OUT3),
  HWD_SIG(LEDC_LS_SIG_OUT4), HWD_SIG(LEDC_LS_SIG_OUT5), HWD_SIG(U0TXD_OUT), HWD_SIG(U0RTS_OUT), HWD_SIG(U0DTR_OUT),
  HWD_SIG(U1TXD_OUT), HWD_SIG(U1RTS_OUT), HWD_SIG(U1DTR_OUT), HWD_SIG(I2S_MCLK_OUT), HWD_SIG(I2SO_BCK_OUT),
  HWD_SIG(I2SO_WS_OUT), HWD_SIG(I2SO_SD_OUT), HWD_SIG(I2SI_BCK_OUT), HWD_SIG(I2SI_WS_OUT), HWD_SIG(I2SO_SD1_OUT),
  HWD_SIG(CPU_TESTBUS0), HWD_SIG(CPU_TESTBUS1), HWD_SIG(CPU_TESTBUS2), HWD_SIG(CPU_TESTBUS3), HWD_SIG(CPU_TESTBUS4),
  HWD_SIG(CPU_TESTBUS5), HWD_SIG(CPU_TESTBUS6), HWD_SIG(CPU_TESTBUS7), HWD_SIG(CPU_GPIO_OUT0), HWD_SIG(CPU_GPIO_OUT1),
  HWD_SIG(CPU_GPIO_OUT2), HWD_SIG(CPU_GPIO_OUT3), HWD_SIG(CPU_GPIO_OUT4), HWD_SIG(CPU_GPIO_OUT5),
  HWD_SIG(CPU_GPIO_OUT6), HWD_SIG(CPU_GPIO_OUT7), HWD_SIG(USB_JTAG_TRST), HWD_SIG(USB_JTAG_SRST),
  HWD_SIG(USB_JTAG_TCK), HWD_SIG(USB_JTAG_TMS), HWD_SIG(USB_JTAG_TDI), HWD_SIG(CPU_USB_JTAG_TDO),
  HWD_SIG(USB_EXTPHY_OEN), HWD_SIG(USB_EXTPHY_SPEED), HWD_SIG(USB_EXTPHY_VPO), HWD_SIG(USB_EXTPHY_VMO),
  HWD_SIG(USB_EXTPHY_SUSPND), HWD_SIG(I2CEXT0_SCL_OUT), HWD_SIG(I2CEXT0_SDA_OUT), HWD_SIG(PARL_TX_DATA0),
  HWD_SIG(PARL_TX_DATA1), HWD_SIG(PARL_TX_DATA2), HWD_SIG(PARL_TX_DATA3), HWD_SIG(PARL_TX_DATA4),
  HWD_SIG(PARL_TX_DATA5), HWD_SIG(PARL_TX_DATA6), HWD_SIG(PARL_TX_DATA7), HWD_SIG(FSPICLK_OUT), HWD_SIG(FSPIQ_OUT),
  HWD_SIG(FSPID_OUT), HWD_SIG(FSPIHD_OUT), HWD_SIG(FSPIWP_OUT), HWD_SIG(FSPICS0_OUT), HWD_SIG(PARL_RX_CLK_OUT),
  HWD_SIG(PARL_TX_CLK_OUT), HWD_SIG(RMT_SIG_OUT0), HWD_SIG(RMT_SIG_OUT1), HWD_SIG(TWAI0_TX),
  HWD_SIG(TWAI0_BUS_OFF_ON), HWD_SIG(TWAI0_CLKOUT), HWD_SIG(TWAI0_STANDBY), HWD_SIG(TWAI1_TX),
  HWD_SIG(TWAI1_BUS_OFF_ON), HWD_SIG(TWAI1_CLKOUT), HWD_SIG(TWAI1_STANDBY), HWD_SIG(EXTERN_PRIORITY_O),
  HWD_SIG(EXTERN_ACTIVE_O), HWD_SIG(GPIO_SD0_OUT), HWD_SIG(GPIO_SD1_OUT), HWD_SIG(GPIO_SD2_OUT),
  HWD_SIG(GPIO_SD3_OUT), HWD_SIG(PWM0_OUT0A), HWD_SIG(PWM0_OUT0B), HWD_SIG(PWM0_OUT1A), HWD_SIG(PWM0_OUT1B),
  HWD_SIG(PWM0_OUT2A), HWD_SIG(PWM0_OUT2B), HWD_SIG(PARL_TX_CS_O), HWD_SIG(GPIO_TASK_MATRIX_OUT0),
  HWD_SIG(GPIO_TASK_MATRIX_OUT1), HWD_SIG(GPIO_TASK_MATRIX_OUT2), HWD_SIG(GPIO_TASK_MATRIX_OUT3),
  HWD_SIG(CLK_OUT_OUT1), HWD_SIG(CLK_OUT_OUT2), HWD_SIG(CLK_OUT_OUT3), HWD_SIG(SDIO_TOHOST_INT_OUT),
  HWD_SIG(SIG_IN_FUNC_97), HWD_SIG(SIG_IN_FUNC_98), HWD_SIG(SIG_IN_FUNC_99), HWD_SIG(SIG_IN_FUNC_100),
  HWD_SIG(FSPICS1_OUT), HWD_SIG(FSPICS2_OUT), HWD_SIG(FSPICS3_OUT), HWD_SIG(FSPICS4_OUT), HWD_SIG(FSPICS5_OUT),
  HWD_SIG(MODEM_DIAG0), HWD_SIG(MODEM_DIAG1), HWD_SIG(MODEM_DIAG2), HWD_SIG(MODEM_DIAG3), HWD_SIG(MODEM_DIAG4),
  HWD_SIG(MODEM_DIAG5), HWD_SIG(MODEM_DIAG6), HWD_SIG(MODEM_DIAG7), HWD_SIG(MODEM_DIAG8), HWD_SIG(MODEM_DIAG9),
  HWD_SIG(MODEM_DIAG10), HWD_SIG(MODEM_DIAG11), HWD_SIG(MODEM_DIAG12), HWD_SIG(MODEM_DIAG13), HWD_SIG(MODEM_DIAG14),
  HWD_SIG(MODEM_DIAG15), HWD_SIG(MODEM_DIAG16), HWD_SIG(MODEM_DIAG17), HWD_SIG(MODEM_DIAG18), HWD_SIG(MODEM_DIAG19),
  HWD_SIG(MODEM_DIAG20), HWD_SIG(MODEM_DIAG21), HWD_SIG(MODEM_DIAG22), HWD_SIG(MODEM_DIAG23), HWD_SIG(MODEM_DIAG24),
  HWD_SIG(MODEM_DIAG25), HWD_SIG(MODEM_DIAG26), HWD_SIG(MODEM_DIAG27), HWD_SIG(MODEM_DIAG28), HWD_SIG(MODEM_DIAG29),
  HWD_SIG(MODEM_DIAG30), HWD_SIG(MODEM_DIAG31), HWD_SIG(ANT_SEL0), HWD_SIG(ANT_SEL1), HWD_SIG(ANT_SEL2),
  HWD_SIG(ANT_SEL3), HWD_SIG(ANT_SEL4), HWD_SIG(ANT_SEL5), HWD_SIG(ANT_SEL6), HWD_SIG(ANT_SEL7), HWD_SIG(ANT_SEL8),
  HWD_SIG(ANT_SEL9), HWD_SIG(ANT_SEL10), HWD_SIG(ANT_SEL11), HWD_SIG(ANT_SEL12), HWD_SIG(ANT_SEL13),
  HWD_SIG(ANT_SEL14), HWD_SIG(ANT_SEL15),
  { SIG_GPIO_OUT_IDX, "GPIO" },                   // simple GPIO output (GPIO_OUT register)
};

// GPIO matrix input signals (GPIO_FUNCm_IN_SEL_CFG), from soc/gpio_sig_map.h
const HwdSig_t kHwdSigIn[] = {
  HWD_SIG(EXT_ADC_START), HWD_SIG(U0RXD_IN), HWD_SIG(U0CTS_IN), HWD_SIG(U0DSR_IN), HWD_SIG(U1RXD_IN),
  HWD_SIG(U1CTS_IN), HWD_SIG(U1DSR_IN), HWD_SIG(I2S_MCLK_IN), HWD_SIG(I2SO_BCK_IN), HWD_SIG(I2SO_WS_IN),
  HWD_SIG(I2SI_SD_IN), HWD_SIG(I2SI_BCK_IN), HWD_SIG(I2SI_WS_IN), HWD_SIG(CPU_GPIO_IN0), HWD_SIG(CPU_GPIO_IN1),
  HWD_SIG(CPU_GPIO_IN2), HWD_SIG(CPU_GPIO_IN3), HWD_SIG(CPU_GPIO_IN4), HWD_SIG(CPU_GPIO_IN5), HWD_SIG(CPU_GPIO_IN6),
  HWD_SIG(CPU_GPIO_IN7), HWD_SIG(USB_JTAG_TDO), HWD_SIG(USB_EXTPHY_VP), HWD_SIG(USB_EXTPHY_VM),
  HWD_SIG(USB_EXTPHY_RCV), HWD_SIG(I2CEXT0_SCL_IN), HWD_SIG(I2CEXT0_SDA_IN), HWD_SIG(PARL_RX_DATA0),
  HWD_SIG(PARL_RX_DATA1), HWD_SIG(PARL_RX_DATA2), HWD_SIG(PARL_RX_DATA3), HWD_SIG(PARL_RX_DATA4),
  HWD_SIG(PARL_RX_DATA5), HWD_SIG(PARL_RX_DATA6), HWD_SIG(PARL_RX_DATA7), HWD_SIG(FSPICLK_IN), HWD_SIG(FSPIQ_IN),
  HWD_SIG(FSPID_IN), HWD_SIG(FSPIHD_IN), HWD_SIG(FSPIWP_IN), HWD_SIG(FSPICS0_IN), HWD_SIG(PARL_RX_CLK_IN),
  HWD_SIG(PARL_TX_CLK_IN), HWD_SIG(RMT_SIG_IN0), HWD_SIG(RMT_SIG_IN1), HWD_SIG(TWAI0_RX), HWD_SIG(TWAI1_RX),
  HWD_SIG(EXTERN_PRIORITY_I), HWD_SIG(EXTERN_ACTIVE_I), HWD_SIG(PCNT_RST_IN0), HWD_SIG(PCNT_RST_IN1),
  HWD_SIG(PCNT_RST_IN2), HWD_SIG(PCNT_RST_IN3), HWD_SIG(PWM0_SYNC0_IN), HWD_SIG(PWM0_SYNC1_IN),
  HWD_SIG(PWM0_SYNC2_IN), HWD_SIG(PWM0_F0_IN), HWD_SIG(PWM0_F1_IN), HWD_SIG(PWM0_F2_IN), HWD_SIG(PWM0_CAP0_IN),
  HWD_SIG(PWM0_CAP1_IN), HWD_SIG(PWM0_CAP2_IN), HWD_SIG(GPIO_EVENT_MATRIX_IN0), HWD_SIG(GPIO_EVENT_MATRIX_IN1),
  HWD_SIG(GPIO_EVENT_MATRIX_IN2), HWD_SIG(GPIO_EVENT_MATRIX_IN3), HWD_SIG(PCNT_SIG_CH0_IN0),
  HWD_SIG(PCNT_SIG_CH1_IN0), HWD_SIG(PCNT_CTRL_CH0_IN0), HWD_SIG(PCNT_CTRL_CH1_IN0), HWD_SIG(PCNT_SIG_CH0_IN1),
  HWD_SIG(PCNT_SIG_CH1_IN1), HWD_SIG(PCNT_CTRL_CH0_IN1), HWD_SIG(PCNT_CTRL_CH1_IN1), HWD_SIG(PCNT_SIG_CH0_IN2),
  HWD_SIG(PCNT_SIG_CH1_IN2), HWD_SIG(PCNT_CTRL_CH0_IN2), HWD_SIG(PCNT_CTRL_CH1_IN2), HWD_SIG(PCNT_SIG_CH0_IN3),
  HWD_SIG(PCNT_SIG_CH1_IN3), HWD_SIG(PCNT_CTRL_CH0_IN3), HWD_SIG(PCNT_CTRL_CH1_IN3),
};

// Target specific register fields (names differ between SoCs)
static inline uint32_t HwdOutLevel(uint32_t pin) { return (GPIO.out.val >> pin) & 1; }
static inline bool HwdOutInv(uint32_t pin) { return GPIO.func_out_sel_cfg[pin].out_inv_sel; }
static inline bool HwdInInv(uint32_t sig) { return GPIO.func_in_sel_cfg[sig].in_inv_sel; }

// LP_IO: GPIO0-6 are LP IO 0-6 (soc/rtc_io_channel.h), LP_AON_GPIO_MUX_REG switches a pad to LP_IO,
// then IO_MUX/GPIO matrix settings do not apply (see rtcio_ll_function_select)
#define HWDUMP_LPIO
static_assert(SOC_RTCIO_PIN_COUNT == 7 && RTCIO_CHANNEL_0_GPIO_NUM == 0 && RTCIO_CHANNEL_6_GPIO_NUM == 6, "HwDump: unexpected LP_IO pads");
// LP_IO bus clock and reset (see _rtcio_ll_enable_io_clock), LP_GPIO/LP_IO_MUX are only read when enabled
static inline bool HwdLpIoOn(void) { return LPPERI.clk_en.lp_io_ck_en && !LPPERI.reset_en.lp_io_reset_en; }
static inline uint32_t HwdLpIoMuxSel(void) { return HAL_FORCE_READ_U32_REG_FIELD(LP_AON.gpio_mux, gpio_mux_sel); }
// LP_IO register layout: pad config in LP_IO_MUX.gpion[] ("gpion_" field prefix), pin config in LP_GPIO.pinn[] ("pinn_" prefix)
#define HWD_LPIO_MUX(r)       (LP_IO_MUX.gpion[r])          // LP_IO_MUX_GPIOn_REG
#define HWD_LPIO_PIN(r)       (LP_GPIO.pinn[r])             // LP_GPIO_PINn_REG
#define HWD_LPIO_MUX_F(f)     gpion_##f
#define HWD_LPIO_PIN_F(f)     pinn_##f
static inline uint32_t HwdLpIoOe(void) { return LP_GPIO.enable.val; }
static inline uint32_t HwdLpIoOut(void) { return LP_GPIO.out.val; }
static inline uint32_t HwdLpIoIn(void) { return LP_GPIO.in.val; }
static inline bool HwdLpIoIntClk(void) { return LP_GPIO.clock_gate.clk_en; }
#define HWD_LPIO_SLP_PULL     1                             // sleep pull-up/pull-down/drive fields mcu_wpu/mcu_wpd/mcu_drv
#define HWD_LPIO_FUNC_GPIO    1                             // LP IO_MUX function of LP_GPIO (RTCIO_LL_PIN_FUNC)
static inline uint32_t HwdLpIoHoldMask(void) { return LP_AON.gpio_hold0.gpio_hold0; }   // same hold bits as gpio_ll_is_digital_io_hold()
static inline int HwdRtcGpio(uint32_t r) { return (int)r; }                         // LP IO index -> GPIO number
static inline bool HwdPinIsRtc(uint32_t pin) { return pin < SOC_RTCIO_PIN_COUNT; }
static inline bool HwdPadRtcMux(uint32_t pin) { return HwdPinIsRtc(pin) && (HwdLpIoMuxSel() & BIT(pin)); }

// LEDC (low speed mode only), fields not covered by ledc_ll getters
#define HWDUMP_LEDC
#define HWDUMP_LEDC_CLK_EN                                  // PCR_LEDC_SCLK_CONF_REG.ledc_sclk_en
#define HWD_LEDC_CLK_LABEL    "clk"                         // global clock shared by all timers
#define HWD_LEDC_MODE_NUM     1
static const uint8_t kHwdLedcModes[HWD_LEDC_MODE_NUM] = { LEDC_LOW_SPEED_MODE };
static_assert(LEDC_LS_SIG_OUT5_IDX == LEDC_LS_SIG_OUT0_IDX + 5, "HwDump: LEDC output signals must be contiguous");
// PCR_LEDC_SCLK_CONF_REG.ledc_sclk_sel: 0=XTAL 1=RC_FAST 2=PLL_F80M (see ledc_ll_set_slow_clk_sel)
// Note: ledc_ll_get_slow_clk_sel() calls abort() on an invalid selector, so read the field directly
const char kHwdLedcClkNames[] PROGMEM = "XTAL|RC_FAST|PLL_F80M|?";
static inline bool HwdLedcBusClk(void) { return PCR.ledc_conf.ledc_clk_en && !PCR.ledc_conf.ledc_rst_en; }
static inline uint32_t HwdLedcClkSel(void) { return PCR.ledc_sclk_conf.ledc_sclk_sel; }
static inline bool HwdLedcClkEn(void) { return PCR.ledc_sclk_conf.ledc_sclk_en; }       // see ledc_ll_enable_clock
static inline uint32_t HwdLedcClkHz(uint32_t sel) {
  uint32_t hz = 0;
  if (0 == sel) { esp_clk_tree_src_get_freq_hz(SOC_MOD_CLK_XTAL, ESP_CLK_TREE_SRC_FREQ_PRECISION_APPROX, &hz); }
  else if (1 == sel) { esp_clk_tree_src_get_freq_hz(SOC_MOD_CLK_RC_FAST, ESP_CLK_TREE_SRC_FREQ_PRECISION_APPROX, &hz); }
  else if (2 == sel) { esp_clk_tree_src_get_freq_hz(SOC_MOD_CLK_PLL_F80M, ESP_CLK_TREE_SRC_FREQ_PRECISION_APPROX, &hz); }
  return hz;
}
// No per-timer clock mux: tick_sel must be 0 (see ledc_ll_get_clock_source)
static inline uint32_t HwdLedcTimerClkHz(uint32_t mode, uint32_t t, uint32_t clk_hz) { return clk_hz; }
static inline bool HwdLedcTimerPaused(uint32_t mode, uint32_t t) { return LEDC.timer_group[mode].timer[t].conf.pause; }
static inline bool HwdLedcTimerRst(uint32_t mode, uint32_t t) { return LEDC.timer_group[mode].timer[t].conf.rst; }
static inline uint32_t HwdLedcTimerCnt(uint32_t mode, uint32_t t) { return LEDC.timer_group[mode].timer[t].value.timern_cnt; }
static inline bool HwdLedcChOutEn(uint32_t mode, uint32_t ch) { return LEDC.channel_group[mode].channel[ch].conf0.sig_out_en; }
static inline uint32_t HwdLedcChIdle(uint32_t mode, uint32_t ch) { return LEDC.channel_group[mode].channel[ch].conf0.idle_lv; }
static inline uint32_t HwdLedcSig(uint32_t mode, uint32_t ch) { return LEDC_LS_SIG_OUT0_IDX + ch; }

// USB-Serial-JTAG controller (USB D-/D+ on GPIO13/14)
// Warning: never read USB_SERIAL_JTAG.ep1 (USB_SERIAL_JTAG_EP1_REG), reading it pops a byte from the RX FIFO
#define HWDUMP_USB_SERIAL_JTAG
#include "soc/usb_serial_jtag_struct.h"
static_assert(USB_INT_PHY0_DM_GPIO_NUM == 13 && USB_INT_PHY0_DP_GPIO_NUM == 14, "HwDump: unexpected USB pins");
static inline bool HwdUsjBusClk(void) { return PCR.usb_device_conf.usb_device_clk_en && !PCR.usb_device_conf.usb_device_rst_en; }
#define HWD_USJ_DM_GPIO     USB_INT_PHY0_DM_GPIO_NUM        // USB D- / D+ pads
#define HWD_USJ_DP_GPIO     USB_INT_PHY0_DP_GPIO_NUM
#define HWD_USJ_DATE_REG    USB_SERIAL_JTAG_DATE_REG
#define HWD_USJ_F(f)        f                               // status field names without prefix

// Common 2-bit clock selector used by PCR_UARTn_SCLK_CONF_REG.uartn_sclk_sel and PCR_RMT_SCLK_CONF_REG.rmt_sclk_sel:
// 0=XTAL 1=RC_FAST 2=PLL_F80M (see uart_ll_get_sclk and rmt_ll_get_group_clock_src)
const char kHwdClk3Names[] PROGMEM = "XTAL|RC_FAST|PLL_F80M|?";
static inline uint32_t HwdClk3Soc(uint32_t sel) { return (0 == sel) ? SOC_MOD_CLK_XTAL : (1 == sel) ? SOC_MOD_CLK_RC_FAST : (2 == sel) ? SOC_MOD_CLK_PLL_F80M : 0; }

// UART (UART0/1, LP_UART is not dumped) - Warning: never read UARTn.fifo (UART_FIFO_REG), reading it pops a byte from the RX FIFO
// The PCR sclk pre-divider (uartn_sclk_div_num) is applied by uart_ll_get_baudrate()
#define HWDUMP_UART
#include "hal/uart_ll.h"
#define HwdUartDev(n)   UART_LL_GET_HW(n)                   // macro: no custom types in .ino function signatures
#define HWD_UART_CONF0      conf0_sync                      // UART_CONF0_SYNC_REG member
#define HWD_UART_CLKDIV     clkdiv_sync                     // UART_CLKDIV_SYNC_REG member and its integer / fractional fields
#define HWD_UART_DIV_INT    clkdiv_int
#define HWD_UART_DIV_FRAG   clkdiv_frag
static inline uint32_t HwdUartClkSel(uint32_t n) { return n ? PCR.uart1_sclk_conf.uart1_sclk_sel : PCR.uart0_sclk_conf.uart0_sclk_sel; }
#define kHwdUartClkNames  kHwdClk3Names
static inline uint32_t HwdUartClkSoc(uint32_t sel) { return HwdClk3Soc(sel); }
// GPIO matrix signals: tx, rx, rts, cts and IO_MUX function names (nullptr if none)
static const uint16_t kHwdUartSig[SOC_UART_HP_NUM][4] = {
  { U0TXD_OUT_IDX, U0RXD_IN_IDX, U0RTS_OUT_IDX, U0CTS_IN_IDX },
  { U1TXD_OUT_IDX, U1RXD_IN_IDX, U1RTS_OUT_IDX, U1CTS_IN_IDX },
};
static const char * const kHwdUartIomux[SOC_UART_HP_NUM][4] = {
  { "U0TXD", "U0RXD", nullptr, nullptr },
  { nullptr, nullptr, nullptr, nullptr },
};

// I2C (I2C0 only, LP_I2C is not dumped) - Warning: never read I2C0.data (I2C_DATA_REG), reading it pops a byte from the RX FIFO
// Source clock and divider are in PCR_I2C_SCLK_CONF_REG (I2C0.clk_conf is not used for I2C0, see i2c_ll_set_source_clk)
#define HWDUMP_I2C
#include "hal/i2c_ll.h"
const char kHwdI2cClkNames[] PROGMEM = "XTAL|RC_FAST";                    // PCR_I2C_SCLK_CONF_REG.i2c_sclk_sel
#define HwdI2cDev(n)        (&I2C0)                         // macro: no custom types in .ino function signatures
#define HwdI2cClkSel(hw)    (PCR.i2c[0].i2c_sclk_conf.i2c_sclk_sel)
#define HwdI2cClkDiv(hw)    (HAL_FORCE_READ_U32_REG_FIELD(PCR.i2c[0].i2c_sclk_conf, i2c_sclk_div_num) + 1)
#define HwdI2cIsMaster(hw)  i2c_ll_is_master_mode(hw)
#define HWD_I2C_SR(hw)      ((hw)->sr)                      // I2C_SR_REG
#define HWD_I2C_RXFIFO_CNT  rxfifo_cnt                      // I2C_SR_REG field names
#define HWD_I2C_TXFIFO_CNT  txfifo_cnt
#define HWD_I2C_INT_MASK    0x7FFFF                         // I2C_INT_RAW_REG bits 0..18
#define HWD_I2C_SCL_ADJ     0                               // i2c_ll_get_scl_timing() already returns the full high and low periods
static inline bool HwdI2cOn(uint32_t n) { return PCR.i2c[0].i2c_conf.i2c_clk_en && !PCR.i2c[0].i2c_conf.i2c_rst_en; }
static inline uint32_t HwdI2cClkSoc(uint32_t sel) { return sel ? SOC_MOD_CLK_RC_FAST : SOC_MOD_CLK_XTAL; }
static const uint16_t kHwdI2cSig[SOC_HP_I2C_NUM][4] = {       // scl_out, scl_in, sda_out, sda_in
  { I2CEXT0_SCL_OUT_IDX, I2CEXT0_SCL_IN_IDX, I2CEXT0_SDA_OUT_IDX, I2CEXT0_SDA_IN_IDX },
};

// RMT (2 tx channels 0-1, 2 rx channels 2-3, C3 layout with "_chn" / "_chm" field suffixes)
// Warning: never read RMT.chndata[] (RMT_CHnDATA_REG), they are the channel FIFO access ports
#define HWDUMP_RMT
#include "hal/rmt_ll.h"
static inline bool HwdRmtOn(void) { return PCR.rmt_conf.rmt_clk_en && !PCR.rmt_conf.rmt_rst_en; }
static_assert(RMT_SIG_OUT1_IDX == RMT_SIG_OUT0_IDX + 1 && RMT_SIG_IN1_IDX == RMT_SIG_IN0_IDX + 1, "HwDump: RMT signals must be contiguous");
#define HWD_RMT_SIG_OUT0        RMT_SIG_OUT0_IDX            // GPIO matrix signals of tx channel 0 / rx channel 0
#define HWD_RMT_SIG_IN0         RMT_SIG_IN0_IDX
#define HWD_RMT_TX_CONF(ch)     (RMT.chnconf0[ch])          // RMT_CHnCONF0_REG
#define HWD_RMT_TX_STATUS(ch)   (RMT.chnstatus[ch])         // RMT_CHnSTATUS_REG
#define HWD_RMT_RX_CONF(i)      (RMT.chmconf[i])            // RMT_CHmCONF0_REG / RMT_CHmCONF1_REG as .conf0 / .conf1
#define HWD_RMT_RX_STATUS(i)    (RMT.chmstatus[i])          // RMT_CHmSTATUS_REG
#define HWD_RMT_TX_F(f)         f##_chn                     // tx field names: mem_size -> mem_size_chn
#define HWD_RMT_RX_F(f)         f##_chm                     // rx field names: mem_size -> mem_size_chm
#define HWD_RMT_INT_MASK        0x3FFF                      // RMT_INT_RAW_REG bits 0..13
// Group clock in PCR_RMT_SCLK_CONF_REG (RMT.sys_conf.sclk_sel/sclk_div_num are not used, see rmt_ll_set_group_clock_src)
#define kHwdRmtClkNames  kHwdClk3Names
static inline uint32_t HwdRmtClkSoc(uint32_t sel) { return HwdClk3Soc(sel); }
static inline uint32_t HwdRmtSclkSel(void) { return PCR.rmt_sclk_conf.rmt_sclk_sel; }
static inline uint32_t HwdRmtSclkDiv(void) { return HAL_FORCE_READ_U32_REG_FIELD(PCR.rmt_sclk_conf, rmt_sclk_div_num) + 1; }
static inline bool HwdRmtMemForcePd(void) { return PCR.rmt_pd_ctrl.rmt_mem_force_pd; }    // see rmt_ll_is_mem_force_powered_down

// I2S0 (C3 layout, no FIFO register, data goes through GDMA), tx/rx clocks in PCR
#define HWDUMP_I2S
#include "soc/i2s_struct.h"
const char kHwdI2sClkNames[] PROGMEM = "XTAL|PLL_F240M|PLL_F160M|external";  // PCR_I2S_TX/RX_CLKM_CONF_REG.i2s_tx/rx_clkm_sel (see i2s_ll_tx_clk_set_src)
#define HwdI2sDev(n)    (&I2S0)                             // macro: no custom types in .ino function signatures
static inline bool HwdI2sOn(uint32_t n) { return PCR.i2s_conf.i2s_clk_en && !PCR.i2s_conf.i2s_rst_en; }
static inline uint32_t HwdI2sClkSoc(uint32_t sel) {
  return (0 == sel) ? SOC_MOD_CLK_XTAL : (1 == sel) ? SOC_MOD_CLK_PLL_F240M : (2 == sel) ? SOC_MOD_CLK_PLL_F160M : 0;
}
#define HWD_I2S_BCK(c, c1)  (c)                             // tx/rx_bck_div_num is in I2S_TX/RX_CONF_REG
static inline bool HwdI2sClkEn(uint32_t u) { return I2S0.clk_gate.clk_en; }                    // I2S_CLK_GATE_REG
static inline uint32_t HwdI2sMclkSel(uint32_t u) { return PCR.i2s_rx_clkm_conf.i2s_mclk_sel; }  // see i2s_ll_mclk_bind_to_rx_clk
// Clock selector, enable, integer divider and fractional divider fields (PCR_I2S_TX/RX_CLKM_CONF_REG, PCR_I2S_TX/RX_CLKM_DIV_CONF_REG)
static inline void HwdI2sClkGet(uint32_t u, bool tx, uint32_t *sel, uint32_t *active, uint32_t *n,
                                uint32_t *x, uint32_t *y, uint32_t *z, uint32_t *yn1) {
  if (tx) {
    __typeof__(PCR.i2s_tx_clkm_conf) k;     k.val = PCR.i2s_tx_clkm_conf.val;
    __typeof__(PCR.i2s_tx_clkm_div_conf) d; d.val = PCR.i2s_tx_clkm_div_conf.val;
    *sel = k.i2s_tx_clkm_sel; *active = k.i2s_tx_clkm_en; *n = k.i2s_tx_clkm_div_num;
    *x = d.i2s_tx_clkm_div_x; *y = d.i2s_tx_clkm_div_y; *z = d.i2s_tx_clkm_div_z; *yn1 = d.i2s_tx_clkm_div_yn1;
  } else {
    __typeof__(PCR.i2s_rx_clkm_conf) k;     k.val = PCR.i2s_rx_clkm_conf.val;
    __typeof__(PCR.i2s_rx_clkm_div_conf) d; d.val = PCR.i2s_rx_clkm_div_conf.val;
    *sel = k.i2s_rx_clkm_sel; *active = k.i2s_rx_clkm_en; *n = k.i2s_rx_clkm_div_num;
    *x = d.i2s_rx_clkm_div_x; *y = d.i2s_rx_clkm_div_y; *z = d.i2s_rx_clkm_div_z; *yn1 = d.i2s_rx_clkm_div_yn1;
  }
}
// mclk out, mclk in, tx bck out/in, tx ws out/in, dout, dout1, rx bck out/in, rx ws out/in, din
static const uint16_t kHwdI2sSig[SOC_I2S_NUM][13] = {
  { I2S_MCLK_OUT_IDX, I2S_MCLK_IN_IDX, I2SO_BCK_OUT_IDX, I2SO_BCK_IN_IDX, I2SO_WS_OUT_IDX, I2SO_WS_IN_IDX,
    I2SO_SD_OUT_IDX, I2SO_SD1_OUT_IDX, I2SI_BCK_OUT_IDX, I2SI_BCK_IN_IDX, I2SI_WS_OUT_IDX, I2SI_WS_IN_IDX, I2SI_SD_IN_IDX },
};

// SPI2 (GPSPI2), SPI0/1 are used by flash/PSRAM and not dumped
// Warning: never read GPSPI2.data_buf[] while a transaction is ongoing (not dumped anyway)
// Module clock source and pre-divider in PCR_SPI2_CLKM_CONF_REG (see spi_ll_set_clk_source, spi_ll_clk_source_pre_div)
#define HWDUMP_SPI
#include "soc/spi_struct.h"
const char kHwdSpiClkNames[] PROGMEM = "XTAL|PLL_F160M|RC_FAST|?";        // PCR_SPI2_CLKM_CONF_REG.spi2_clkm_sel
#define HWD_SPI_NUM     1
static const char * const kHwdSpiNames[HWD_SPI_NUM] = { "SPI2" };
#define HwdSpiDev(i)    (&GPSPI2)                           // macro: no custom types in .ino function signatures
#define HWD_SPI_CS_NUM  6
static inline bool HwdSpiOn(uint32_t i) { return PCR.spi2_conf.spi2_clk_en && !PCR.spi2_conf.spi2_rst_en; }
static inline uint32_t HwdSpiClkSel(uint32_t i) { return PCR.spi2_clkm_conf.spi2_clkm_sel; }
static inline uint32_t HwdSpiClkSoc(uint32_t sel) { return (0 == sel) ? SOC_MOD_CLK_XTAL : (1 == sel) ? SOC_MOD_CLK_PLL_F160M : (2 == sel) ? SOC_MOD_CLK_RC_FAST : 0; }
static inline uint32_t HwdSpiClkPreDiv(uint32_t i) { return HAL_FORCE_READ_U32_REG_FIELD(PCR.spi2_clkm_conf, spi2_clkm_div_num) + 1; }
// GPIO matrix signals: sck, d (mosi), q (miso), hd, wp, cs0..cs5 - input and output signals share the same index
static const uint16_t kHwdSpiSig[HWD_SPI_NUM][5 + HWD_SPI_CS_NUM] = {
  { FSPICLK_OUT_IDX, FSPID_OUT_IDX, FSPIQ_IN_IDX, FSPIHD_OUT_IDX, FSPIWP_OUT_IDX,
    FSPICS0_OUT_IDX, FSPICS1_OUT_IDX, FSPICS2_OUT_IDX, FSPICS3_OUT_IDX, FSPICS4_OUT_IDX, FSPICS5_OUT_IDX },
};
// IO_MUX function names: sck, d, q, hd, wp, cs0
static const char * const kHwdSpiIomux[HWD_SPI_NUM][6] = {
  { "FSPICLK", "FSPID", "FSPIQ", "FSPIHD", "FSPIWP", "FSPICS0" },
};

// Sleep: PMU (wakeup enable, sleep mode pad settings) and LP_AON (ext1, pad hold) registers, no RTC_CNTL
#define HWDUMP_SLEEP
#define HWDUMP_SLEEP_EXT                                    // ext1 (LP_AON) wakeup, no ext0 on C5
#define HWDUMP_SLEEP_PMU_PAD_HOLD                           // pad hold in LP_AON / PMU
#include "esp_sleep.h"
#include "esp_pm.h"
// PMU_SLP_WAKEUP_CNTL2_REG bits, from esp_hw_support/port/esp32c5/private_include/pmu_bit_defs.h (PMU_xxx_WAKEUP_EN)
// index = bit number, empty = not used on this target
const char kHwdWakeupNames[] PROGMEM = "ext0|ext1|gpio|wifi_beacon|lp_timer|wifi|uart0|uart1|||ble|lp_core|||usb";
#define HWD_WAKEUP_BITS   15
#define HWD_WAKEUP_EXT1_EN  BIT(1)                          // PMU_EXT1_WAKEUP_EN
static inline uint32_t HwdWakeupEna(void) { return PMU.wakeup.cntl2; }              // see pmu_ll_hp_set_wakeup_enable
static inline bool HwdPadHold(uint32_t pin) { return gpio_ll_is_digital_io_hold(&GPIO, pin); }   // LP_AON_GPIO_HOLD0_REG, LP pads included
static inline uint32_t HwdPadHoldMask(void) { return LP_AON.gpio_hold0.gpio_hold0; }
static inline bool HwdSlpHpPadHoldAll(void) { return PMU.hp_sys[PMU_MODE_HP_SLEEP].syscntl.hp_pad_hold_all; }
static inline bool HwdSlpLpPadHoldAll(void) { return PMU.hp_sys[PMU_MODE_HP_SLEEP].syscntl.lp_pad_hold_all; }
static inline bool HwdSlpDigPadSlpSel(void) { return PMU.hp_sys[PMU_MODE_HP_SLEEP].syscntl.dig_pad_slp_sel; }
static inline uint32_t HwdGpioIntType(uint32_t pin) { return GPIO.pin[pin].int_type; }
static inline bool HwdGpioWakeupEn(uint32_t pin) { return GPIO.pin[pin].wakeup_enable; }
// Deep-sleep GPIO wakeup is the LP_IO pad wakeup (see rtcio_ll_wakeup_enable), "clk" = LP_GPIO clock gate
static inline bool HwdDeepSleepWakeEn(uint32_t pin) { return HwdLpIoOn() && HwdPinIsRtc(pin) && LP_GPIO.pinn[pin].pinn_wakeup_enable; }
static inline uint32_t HwdDeepSleepWakeType(uint32_t pin) { return (HwdLpIoOn() && HwdPinIsRtc(pin)) ? LP_GPIO.pinn[pin].pinn_int_type : 0; }
static inline bool HwdDeepSleepWakeClk(void) { return HwdLpIoOn() && LP_GPIO.clock_gate.clk_en; }
// ext1: mask of LP pads, one trigger level per pad (1 = high), programmed by IDF when entering sleep (see lp_aon_ll_ext1_set_wakeup_pins)
static inline uint32_t HwdExt1Mask(void) { return HAL_FORCE_READ_U32_REG_FIELD(LP_AON.ext_wakeup_cntl, ext_wakeup_sel); }
static inline uint32_t HwdExt1HighMask(void) { return HAL_FORCE_READ_U32_REG_FIELD(LP_AON.ext_wakeup_cntl, ext_wakeup_lv); }
static inline uint32_t HwdExt1Status(void) { return HAL_FORCE_READ_U32_REG_FIELD(LP_AON.ext_wakeup_cntl, ext_wakeup_status); }

#endif  // CONFIG_IDF_TARGET_ESP32C5
