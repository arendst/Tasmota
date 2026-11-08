/*
  xdrv_126_hwdump_esp32c3.h - HwDump per-target data and register accessors for ESP32-C3

  SPDX-FileCopyrightText: 2026 Stephan Hadinger

  SPDX-License-Identifier: GPL-3.0-only
*/

// Only included from tasmota_xdrv_driver/xdrv_126_hwdump.ino, after HwdPad_t, HwdSig_t and HWD_SIG() are defined.
// Defines data (not only declarations), so it must be included exactly once.
#pragma once

#if CONFIG_IDF_TARGET_ESP32C3

/*********************************************************************************************\
 * ESP32-C3
\*********************************************************************************************/
#define HWDUMP_SUPPORTED

// Pad names and IO_MUX functions from soc/io_mux_reg.h (FUNC_<pad>_<function> = MCU_SEL value)
const HwdPad_t kHwdPads[] = {
  { "XTAL_32K_P", "GPIO0_0|GPIO0" },              //  0
  { "XTAL_32K_N", "GPIO1_0|GPIO1" },              //  1
  { "GPIO2",      "GPIO2_0|GPIO2|FSPIQ" },        //  2
  { "GPIO3",      "GPIO3_0|GPIO3" },              //  3
  { "MTMS",       "MTMS|GPIO4|FSPIHD" },          //  4
  { "MTDI",       "MTDI|GPIO5|FSPIWP" },          //  5
  { "MTCK",       "MTCK|GPIO6|FSPICLK" },         //  6
  { "MTDO",       "MTDO|GPIO7|FSPID" },           //  7
  { "GPIO8",      "GPIO8_0|GPIO8" },              //  8
  { "GPIO9",      "GPIO9_0|GPIO9" },              //  9
  { "GPIO10",     "GPIO10_0|GPIO10|FSPICS0" },    // 10
  { "VDD_SPI",    "GPIO11_0|GPIO11" },            // 11
  { "SPIHD",      "SPIHD|GPIO12" },               // 12
  { "SPIWP",      "SPIWP|GPIO13" },               // 13
  { "SPICS0",     "SPICS0|GPIO14" },              // 14
  { "SPICLK",     "SPICLK|GPIO15" },              // 15
  { "SPID",       "SPID|GPIO16" },                // 16
  { "SPIQ",       "SPIQ|GPIO17" },                // 17
  { "GPIO18",     "GPIO18_0|GPIO18" },            // 18
  { "GPIO19",     "GPIO19_0|GPIO19" },            // 19
  { "U0RXD",      "U0RXD|GPIO20" },               // 20
  { "U0TXD",      "U0TXD|GPIO21" },               // 21
};

// Sanity checks of the function indexes above against ESP-IDF definitions
static_assert(PIN_FUNC_GPIO == 1, "HwDump: unexpected PIN_FUNC_GPIO");
static_assert(FUNC_XTAL_32K_P_GPIO0_0 == 0 && FUNC_XTAL_32K_P_GPIO0 == 1, "HwDump: IO_MUX pad 0");
static_assert(FUNC_GPIO2_GPIO2_0 == 0 && FUNC_GPIO2_FSPIQ == 2, "HwDump: IO_MUX pad 2");
static_assert(FUNC_MTMS_MTMS == 0 && FUNC_MTMS_FSPIHD == 2, "HwDump: IO_MUX pad 4");
static_assert(FUNC_MTDI_MTDI == 0 && FUNC_MTDI_FSPIWP == 2, "HwDump: IO_MUX pad 5");
static_assert(FUNC_MTCK_MTCK == 0 && FUNC_MTCK_FSPICLK == 2, "HwDump: IO_MUX pad 6");
static_assert(FUNC_MTDO_MTDO == 0 && FUNC_MTDO_FSPID == 2, "HwDump: IO_MUX pad 7");
static_assert(FUNC_GPIO10_GPIO10_0 == 0 && FUNC_GPIO10_FSPICS0 == 2, "HwDump: IO_MUX pad 10");
static_assert(FUNC_SPIHD_SPIHD == 0 && FUNC_SPIQ_SPIQ == 0, "HwDump: IO_MUX pads 12-17");
static_assert(FUNC_U0RXD_U0RXD == 0 && FUNC_U0TXD_U0TXD == 0, "HwDump: IO_MUX pads 20-21");

// GPIO matrix output signals (GPIO_FUNCn_OUT_SEL), from soc/gpio_sig_map.h
const HwdSig_t kHwdSigOut[] = {
  HWD_SIG(SPIQ_OUT), HWD_SIG(SPID_OUT), HWD_SIG(SPIHD_OUT), HWD_SIG(SPIWP_OUT),
  HWD_SIG(SPICLK_OUT), HWD_SIG(SPICS0_OUT),
  HWD_SIG(U0TXD_OUT), HWD_SIG(U0RTS_OUT), HWD_SIG(U0DTR_OUT),
  HWD_SIG(U1TXD_OUT), HWD_SIG(U1RTS_OUT), HWD_SIG(U1DTR_OUT),
  HWD_SIG(I2S_MCLK_OUT), HWD_SIG(I2SO_BCK_OUT), HWD_SIG(I2SO_WS_OUT), HWD_SIG(I2SO_SD_OUT),
  HWD_SIG(I2SI_BCK_OUT), HWD_SIG(I2SI_WS_OUT),
  HWD_SIG(GPIO_WLAN_PRIO), HWD_SIG(GPIO_WLAN_ACTIVE),
  HWD_SIG(CPU_GPIO_OUT0), HWD_SIG(CPU_GPIO_OUT1), HWD_SIG(CPU_GPIO_OUT2), HWD_SIG(CPU_GPIO_OUT3),
  HWD_SIG(CPU_GPIO_OUT4), HWD_SIG(CPU_GPIO_OUT5), HWD_SIG(CPU_GPIO_OUT6), HWD_SIG(CPU_GPIO_OUT7),
  HWD_SIG(USB_JTAG_TCK_OUT), HWD_SIG(USB_JTAG_TMS_OUT), HWD_SIG(USB_JTAG_TDI_OUT), HWD_SIG(USB_JTAG_TDO_OUT),
  HWD_SIG(USB_EXTPHY_OEN), HWD_SIG(USB_EXTPHY_SPEED), HWD_SIG(USB_EXTPHY_VPO), HWD_SIG(USB_EXTPHY_VMO),
  HWD_SIG(USB_EXTPHY_SUSPND),
  HWD_SIG(LEDC_LS_SIG_OUT0), HWD_SIG(LEDC_LS_SIG_OUT1), HWD_SIG(LEDC_LS_SIG_OUT2),
  HWD_SIG(LEDC_LS_SIG_OUT3), HWD_SIG(LEDC_LS_SIG_OUT4), HWD_SIG(LEDC_LS_SIG_OUT5),
  HWD_SIG(RMT_SIG_OUT0), HWD_SIG(RMT_SIG_OUT1),
  HWD_SIG(I2CEXT0_SCL_OUT), HWD_SIG(I2CEXT0_SDA_OUT),
  HWD_SIG(GPIO_SD0_OUT), HWD_SIG(GPIO_SD1_OUT), HWD_SIG(GPIO_SD2_OUT), HWD_SIG(GPIO_SD3_OUT),
  HWD_SIG(I2SO_SD1_OUT),
  HWD_SIG(FSPICLK_OUT), HWD_SIG(FSPIQ_OUT), HWD_SIG(FSPID_OUT), HWD_SIG(FSPIHD_OUT), HWD_SIG(FSPIWP_OUT),
  HWD_SIG(FSPICS0_OUT), HWD_SIG(FSPICS1_OUT), HWD_SIG(FSPICS2_OUT), HWD_SIG(FSPICS3_OUT),
  HWD_SIG(FSPICS4_OUT), HWD_SIG(FSPICS5_OUT),
  HWD_SIG(TWAI_TX), HWD_SIG(TWAI_BUS_OFF_ON), HWD_SIG(TWAI_CLKOUT),
  HWD_SIG(BT_AUDIO0_IRQ), HWD_SIG(BT_AUDIO1_IRQ), HWD_SIG(BT_AUDIO2_IRQ),
  HWD_SIG(BLE_AUDIO0_IRQ), HWD_SIG(BLE_AUDIO1_IRQ), HWD_SIG(BLE_AUDIO2_IRQ),
  HWD_SIG(PCMFSYNC_OUT), HWD_SIG(PCMCLK_OUT), HWD_SIG(PCMDOUT),
  HWD_SIG(BLE_AUDIO_SYNC0_P), HWD_SIG(BLE_AUDIO_SYNC1_P), HWD_SIG(BLE_AUDIO_SYNC2_P),
  HWD_SIG(ANT_SEL0), HWD_SIG(ANT_SEL1), HWD_SIG(ANT_SEL2), HWD_SIG(ANT_SEL3),
  HWD_SIG(ANT_SEL4), HWD_SIG(ANT_SEL5), HWD_SIG(ANT_SEL6), HWD_SIG(ANT_SEL7),
  HWD_SIG(SIG_IN_FUNC_97), HWD_SIG(SIG_IN_FUNC_98), HWD_SIG(SIG_IN_FUNC_99), HWD_SIG(SIG_IN_FUNC_100),
  HWD_SIG(SYNCERR), HWD_SIG(SYNCFOUND_FLAG), HWD_SIG(EVT_CNTL_IMMEDIATE_ABORT), HWD_SIG(LINKLBL),
  HWD_SIG(DATA_EN), HWD_SIG(DATA), HWD_SIG(PKT_TX_ON), HWD_SIG(PKT_RX_ON), HWD_SIG(RW_TX_ON), HWD_SIG(RW_RX_ON),
  HWD_SIG(EVT_REQ_P), HWD_SIG(EVT_STOP_P), HWD_SIG(BT_MODE_ON),
  HWD_SIG(GPIO_LC_DIAG0), HWD_SIG(GPIO_LC_DIAG1), HWD_SIG(GPIO_LC_DIAG2),
  HWD_SIG(CH_IDX), HWD_SIG(RX_WINDOW), HWD_SIG(UPDATE_RX), HWD_SIG(RX_STATUS),
  HWD_SIG(CLK_GPIO), HWD_SIG(NBT_BLE),
  HWD_SIG(CLK_OUT_OUT1), HWD_SIG(CLK_OUT_OUT2), HWD_SIG(CLK_OUT_OUT3),
  HWD_SIG(SPICS1_OUT), HWD_SIG(USB_JTAG_TRST_OUT),
  { SIG_GPIO_OUT_IDX, "GPIO" },                   // simple GPIO output (GPIO_OUT register)
};

// GPIO matrix input signals (GPIO_FUNCm_IN_SEL_CFG), from soc/gpio_sig_map.h
const HwdSig_t kHwdSigIn[] = {
  HWD_SIG(SPIQ_IN), HWD_SIG(SPID_IN), HWD_SIG(SPIHD_IN), HWD_SIG(SPIWP_IN),
  HWD_SIG(U0RXD_IN), HWD_SIG(U0CTS_IN), HWD_SIG(U0DSR_IN),
  HWD_SIG(U1RXD_IN), HWD_SIG(U1CTS_IN), HWD_SIG(U1DSR_IN),
  HWD_SIG(I2S_MCLK_IN), HWD_SIG(I2SO_BCK_IN), HWD_SIG(I2SO_WS_IN),
  HWD_SIG(I2SI_SD_IN), HWD_SIG(I2SI_BCK_IN), HWD_SIG(I2SI_WS_IN),
  HWD_SIG(GPIO_BT_PRIORITY), HWD_SIG(GPIO_BT_ACTIVE),
  HWD_SIG(CPU_GPIO_IN0), HWD_SIG(CPU_GPIO_IN1), HWD_SIG(CPU_GPIO_IN2), HWD_SIG(CPU_GPIO_IN3),
  HWD_SIG(CPU_GPIO_IN4), HWD_SIG(CPU_GPIO_IN5), HWD_SIG(CPU_GPIO_IN6), HWD_SIG(CPU_GPIO_IN7),
  HWD_SIG(USB_EXTPHY_VP), HWD_SIG(USB_EXTPHY_VM), HWD_SIG(USB_EXTPHY_RCV),
  HWD_SIG(EXT_ADC_START),
  HWD_SIG(RMT_SIG_IN0), HWD_SIG(RMT_SIG_IN1),
  HWD_SIG(I2CEXT0_SCL_IN), HWD_SIG(I2CEXT0_SDA_IN),
  HWD_SIG(FSPICLK_IN), HWD_SIG(FSPIQ_IN), HWD_SIG(FSPID_IN), HWD_SIG(FSPIHD_IN), HWD_SIG(FSPIWP_IN),
  HWD_SIG(FSPICS0_IN),
  HWD_SIG(TWAI_RX),
  HWD_SIG(PCMFSYNC_IN), HWD_SIG(PCMCLK_IN), HWD_SIG(PCMDIN),
  HWD_SIG(RW_WAKEUP_REQ),
};

// Target specific register fields (names differ between SoCs)
static inline uint32_t HwdOutLevel(uint32_t pin) { return (GPIO.out.val >> pin) & 1; }
static inline bool HwdOutInv(uint32_t pin) { return GPIO.func_out_sel_cfg[pin].inv_sel; }
static inline bool HwdInInv(uint32_t sig) { return GPIO.func_in_sel_cfg[sig].sig_in_inv; }

// LEDC (low speed mode only), fields not covered by ledc_ll getters
#define HWDUMP_LEDC
#define HWDUMP_LEDC_CLK_EN                                  // LEDC_CONF_REG.clk_en exists
#define HWD_LEDC_CLK_LABEL    "clk"                         // global clock shared by all timers
#define HWD_LEDC_MODE_NUM     1
static const uint8_t kHwdLedcModes[HWD_LEDC_MODE_NUM] = { LEDC_LOW_SPEED_MODE };
static_assert(LEDC_LS_SIG_OUT5_IDX == LEDC_LS_SIG_OUT0_IDX + 5, "HwDump: LEDC output signals must be contiguous");
// LEDC_CONF_REG.apb_clk_sel: 0=none 1=APB 2=RC_FAST 3=XTAL (see ledc_ll_set_slow_clk_sel)
// Note: ledc_ll_get_slow_clk_sel() calls abort() when no clock is selected, so read the field directly
const char kHwdLedcClkNames[] PROGMEM = "none|APB|RC_FAST|XTAL";
static inline bool HwdLedcBusClk(void) { return SYSTEM.perip_clk_en0.reg_ledc_clk_en && !SYSTEM.perip_rst_en0.reg_ledc_rst; }
static inline uint32_t HwdLedcClkSel(void) { return LEDC.conf.apb_clk_sel; }
static inline bool HwdLedcClkEn(void) { return LEDC.conf.clk_en; }
static inline uint32_t HwdLedcClkHz(uint32_t sel) {
  uint32_t hz = 0;
  if (1 == sel) { esp_clk_tree_src_get_freq_hz(SOC_MOD_CLK_APB, ESP_CLK_TREE_SRC_FREQ_PRECISION_APPROX, &hz); }
  else if (2 == sel) { esp_clk_tree_src_get_freq_hz(SOC_MOD_CLK_RC_FAST, ESP_CLK_TREE_SRC_FREQ_PRECISION_APPROX, &hz); }
  else if (3 == sel) { esp_clk_tree_src_get_freq_hz(SOC_MOD_CLK_XTAL, ESP_CLK_TREE_SRC_FREQ_PRECISION_APPROX, &hz); }
  return hz;
}
static inline uint32_t HwdLedcTimerClkHz(uint32_t mode, uint32_t t, uint32_t clk_hz) { return clk_hz; }   // no per-timer mux
static inline bool HwdLedcTimerPaused(uint32_t mode, uint32_t t) { return LEDC.timer_group[mode].timer[t].conf.pause; }
static inline bool HwdLedcTimerRst(uint32_t mode, uint32_t t) { return LEDC.timer_group[mode].timer[t].conf.rst; }
static inline uint32_t HwdLedcTimerCnt(uint32_t mode, uint32_t t) { return LEDC.timer_group[mode].timer[t].value.timer_cnt; }
static inline bool HwdLedcChOutEn(uint32_t mode, uint32_t ch) { return LEDC.channel_group[mode].channel[ch].conf0.sig_out_en; }
static inline uint32_t HwdLedcChIdle(uint32_t mode, uint32_t ch) { return LEDC.channel_group[mode].channel[ch].conf0.idle_lv; }
static inline uint32_t HwdLedcSig(uint32_t mode, uint32_t ch) { return LEDC_LS_SIG_OUT0_IDX + ch; }

// USB-Serial-JTAG controller
// Warning: never read USB_SERIAL_JTAG.ep1 (USB_SERIAL_JTAG_EP1_REG), reading it pops a byte from the RX FIFO
#define HWDUMP_USB_SERIAL_JTAG
#include "soc/usb_serial_jtag_struct.h"
static_assert(USB_INT_PHY0_DM_GPIO_NUM == 18 && USB_INT_PHY0_DP_GPIO_NUM == 19, "HwDump: unexpected USB pins");
static inline bool HwdUsjBusClk(void) { return SYSTEM.perip_clk_en0.reg_usb_device_clk_en && !SYSTEM.perip_rst_en0.reg_usb_device_rst; }
#define HWD_USJ_DM_GPIO     USB_INT_PHY0_DM_GPIO_NUM        // USB D- / D+ pads
#define HWD_USJ_DP_GPIO     USB_INT_PHY0_DP_GPIO_NUM
#define HWD_USJ_DATE_REG    USB_SERIAL_JTAG_DATE_REG
#define HWD_USJ_F(f)        f                               // status field names without prefix

// UART - Warning: never read UARTn.fifo (UART_FIFO_REG), reading it pops a byte from the RX FIFO
#define HWDUMP_UART
#include "hal/uart_ll.h"
// Common 2-bit clock selector used by UART_CLK_CONF_REG.sclk_sel and RMT_SYS_CONF_REG.sclk_sel
// (see uart_ll_get_sclk and rmt_ll_get_group_clock_src)
const char kHwdClk4Names[] PROGMEM = "none|APB|RC_FAST|XTAL";
static inline uint32_t HwdClk4Soc(uint32_t sel) { return (1 == sel) ? SOC_MOD_CLK_APB : (2 == sel) ? SOC_MOD_CLK_RC_FAST : (3 == sel) ? SOC_MOD_CLK_XTAL : 0; }
#define HwdUartDev(n)   ((0 == (n)) ? &UART0 : &UART1)     // macro: no custom types in .ino function signatures
#define HWD_UART_CONF0      conf0                           // UART_CONF0_REG member
#define HWD_UART_CLKDIV     clk_div                         // UART_CLKDIV_REG member and its integer / fractional fields
#define HWD_UART_DIV_INT    div_int
#define HWD_UART_DIV_FRAG   div_frag
static inline uint32_t HwdUartClkSel(uint32_t n) { return HwdUartDev(n)->clk_conf.sclk_sel; }
#define kHwdUartClkNames  kHwdClk4Names
static inline uint32_t HwdUartClkSoc(uint32_t sel) { return HwdClk4Soc(sel); }
// GPIO matrix signals: tx, rx, rts, cts and IO_MUX function names (nullptr if none)
static const uint16_t kHwdUartSig[SOC_UART_HP_NUM][4] = {
  { U0TXD_OUT_IDX, U0RXD_IN_IDX, U0RTS_OUT_IDX, U0CTS_IN_IDX },
  { U1TXD_OUT_IDX, U1RXD_IN_IDX, U1RTS_OUT_IDX, U1CTS_IN_IDX },
};
static const char * const kHwdUartIomux[SOC_UART_HP_NUM][4] = {
  { "U0TXD", "U0RXD", nullptr, nullptr },
  { nullptr, nullptr, nullptr, nullptr },
};

// I2C - Warning: never read I2C0.data (I2C_DATA_REG), reading it pops a byte from the RX FIFO
#define HWDUMP_I2C
#include "hal/i2c_ll.h"
const char kHwdI2cClkNames[] PROGMEM = "XTAL|RC_FAST";                    // I2C_CLK_CONF_REG.sclk_sel (see i2c_ll_set_source_clk)
#define HwdI2cDev(n)    (&I2C0)                             // macro: no custom types in .ino function signatures
#define HwdI2cClkSel(hw)    ((hw)->clk_conf.sclk_sel)
#define HwdI2cClkDiv(hw)    (HAL_FORCE_READ_U32_REG_FIELD((hw)->clk_conf, sclk_div_num) + 1)
#define HwdI2cIsMaster(hw)  i2c_ll_is_master_mode(hw)
#define HWD_I2C_SR(hw)      ((hw)->sr)                      // I2C_SR_REG
#define HWD_I2C_RXFIFO_CNT  rx_fifo_cnt                     // I2C_SR_REG field names
#define HWD_I2C_TXFIFO_CNT  tx_fifo_cnt
#define HWD_I2C_INT_MASK    0x3FFFF
#define HWD_I2C_SCL_ADJ     0                               // added to high + low periods for the SCL estimate
static inline bool HwdI2cOn(uint32_t n) { return SYSTEM.perip_clk_en0.reg_i2c_ext0_clk_en && !SYSTEM.perip_rst_en0.reg_i2c_ext0_rst; }
static inline uint32_t HwdI2cClkSoc(uint32_t sel) { return sel ? SOC_MOD_CLK_RC_FAST : SOC_MOD_CLK_XTAL; }
static const uint16_t kHwdI2cSig[SOC_I2C_NUM][4] = {          // scl_out, scl_in, sda_out, sda_in
  { I2CEXT0_SCL_OUT_IDX, I2CEXT0_SCL_IN_IDX, I2CEXT0_SDA_OUT_IDX, I2CEXT0_SDA_IN_IDX },
};

// RMT - Warning: never read RMT.data_ch[] (RMT_CHnDATA_REG), it is the channel FIFO access port
#define HWDUMP_RMT
#include "hal/rmt_ll.h"
static inline bool HwdRmtOn(void) { return SYSTEM.perip_clk_en0.reg_rmt_clk_en && !SYSTEM.perip_rst_en0.reg_rmt_rst; }
static_assert(RMT_SIG_OUT1_IDX == RMT_SIG_OUT0_IDX + 1 && RMT_SIG_IN1_IDX == RMT_SIG_IN0_IDX + 1, "HwDump: RMT signals must be contiguous");
#define HWD_RMT_SIG_OUT0        RMT_SIG_OUT0_IDX            // GPIO matrix signals of tx channel 0 / rx channel 0
#define HWD_RMT_SIG_IN0         RMT_SIG_IN0_IDX
#define HWD_RMT_TX_CONF(ch)     (RMT.tx_conf[ch])           // RMT_CHnCONF0_REG
#define HWD_RMT_TX_STATUS(ch)   (RMT.tx_status[ch])         // RMT_CHnSTATUS_REG
#define HWD_RMT_RX_CONF(i)      (RMT.rx_conf[i])            // RMT_CHmCONF0_REG / RMT_CHmCONF1_REG as .conf0 / .conf1
#define HWD_RMT_RX_STATUS(i)    (RMT.rx_status[i])          // RMT_CHmSTATUS_REG
#define HWD_RMT_TX_F(f)         f                           // tx field names
#define HWD_RMT_RX_F(f)         f                           // rx field names
#define HWD_RMT_INT_MASK        0x3FFF                      // RMT_INT_RAW_REG bits 0..13
// Group clock in RMT_SYS_CONF_REG
#define kHwdRmtClkNames  kHwdClk4Names
static inline uint32_t HwdRmtClkSoc(uint32_t sel) { return HwdClk4Soc(sel); }
static inline uint32_t HwdRmtSclkSel(void) { return RMT.sys_conf.sclk_sel; }
static inline uint32_t HwdRmtSclkDiv(void) { return HAL_FORCE_READ_U32_REG_FIELD(RMT.sys_conf, sclk_div_num) + 1; }
static inline bool HwdRmtMemForcePd(void) { return RMT.sys_conf.mem_force_pd; }

// I2S (I2S0 only, no FIFO register on C3, data goes through GDMA)
#define HWDUMP_I2S
#include "soc/i2s_struct.h"
const char kHwdI2sClkNames[] PROGMEM = "XTAL|?|PLL_F160M|external";      // I2S_TX/RX_CLKM_CONF_REG.clk_sel (see i2s_ll_tx_clk_set_src)
#define HwdI2sDev(n)    (&I2S0)                             // macro: no custom types in .ino function signatures
static inline bool HwdI2sOn(uint32_t n) { return SYSTEM.perip_clk_en0.reg_i2s0_clk_en && !SYSTEM.perip_rst_en0.reg_i2s0_rst; }
static inline uint32_t HwdI2sClkSoc(uint32_t sel) { return (0 == sel) ? SOC_MOD_CLK_XTAL : (2 == sel) ? SOC_MOD_CLK_PLL_F160M : 0; }
#define HWD_I2S_BCK(c, c1)  (c1)                            // tx/rx_bck_div_num is in I2S_TX/RX_CONF1_REG
static inline bool HwdI2sClkEn(uint32_t u) { return HwdI2sDev(u)->tx_clkm_conf.clk_en; }
static inline uint32_t HwdI2sMclkSel(uint32_t u) { return HwdI2sDev(u)->rx_clkm_conf.mclk_sel; }
// Clock selector, active flag, integer divider and fractional divider fields (I2S_TX/RX_CLKM_CONF_REG, I2S_TX/RX_CLKM_DIV_CONF_REG)
static inline void HwdI2sClkGet(uint32_t u, bool tx, uint32_t *sel, uint32_t *active, uint32_t *n,
                                uint32_t *x, uint32_t *y, uint32_t *z, uint32_t *yn1) {
  i2s_dev_t *hw = HwdI2sDev(u);
  if (tx) {
    __typeof__(hw->tx_clkm_conf) k;     k.val = hw->tx_clkm_conf.val;
    __typeof__(hw->tx_clkm_div_conf) d; d.val = hw->tx_clkm_div_conf.val;
    *sel = k.tx_clk_sel; *active = k.tx_clk_active; *n = k.tx_clkm_div_num;
    *x = d.tx_clkm_div_x; *y = d.tx_clkm_div_y; *z = d.tx_clkm_div_z; *yn1 = d.tx_clkm_div_yn1;
  } else {
    __typeof__(hw->rx_clkm_conf) k;     k.val = hw->rx_clkm_conf.val;
    __typeof__(hw->rx_clkm_div_conf) d; d.val = hw->rx_clkm_div_conf.val;
    *sel = k.rx_clk_sel; *active = k.rx_clk_active; *n = k.rx_clkm_div_num;
    *x = d.rx_clkm_div_x; *y = d.rx_clkm_div_y; *z = d.rx_clkm_div_z; *yn1 = d.rx_clkm_div_yn1;
  }
}
// mclk out, mclk in, tx bck out/in, tx ws out/in, dout, dout1, rx bck out/in, rx ws out/in, din
static const uint16_t kHwdI2sSig[SOC_I2S_NUM][13] = {
  { I2S_MCLK_OUT_IDX, I2S_MCLK_IN_IDX, I2SO_BCK_OUT_IDX, I2SO_BCK_IN_IDX, I2SO_WS_OUT_IDX, I2SO_WS_IN_IDX,
    I2SO_SD_OUT_IDX, I2SO_SD1_OUT_IDX, I2SI_BCK_OUT_IDX, I2SI_BCK_IN_IDX, I2SI_WS_OUT_IDX, I2SI_WS_IN_IDX, I2SI_SD_IN_IDX },
};

// SPI2 (GPSPI2), SPI0/1 are used by flash and not dumped
// Warning: never read GPSPI2.data_buf[] while a transaction is ongoing (not dumped anyway)
#define HWDUMP_SPI
#include "soc/spi_struct.h"
const char kHwdSpiClkNames[] PROGMEM = "XTAL|APB";                        // SPI_CLK_GATE_REG.mst_clk_sel (see spi_ll_set_clk_source)
#define HWD_SPI_NUM     1
static const char * const kHwdSpiNames[HWD_SPI_NUM] = { "SPI2" };
#define HwdSpiDev(i)    (&GPSPI2)                           // macro: no custom types in .ino function signatures
#define HWD_SPI_CS_NUM  6
static inline bool HwdSpiOn(uint32_t i) { return SYSTEM.perip_clk_en0.reg_spi2_clk_en && !SYSTEM.perip_rst_en0.reg_spi2_rst; }
static inline uint32_t HwdSpiClkSel(uint32_t i) { return HwdSpiDev(i)->clk_gate.mst_clk_sel; }
static inline uint32_t HwdSpiClkSoc(uint32_t sel) { return sel ? SOC_MOD_CLK_APB : SOC_MOD_CLK_XTAL; }
static inline uint32_t HwdSpiClkPreDiv(uint32_t i) { return 1; }                   // no module clock pre-divider
// GPIO matrix signals: sck, d (mosi), q (miso), hd, wp, cs0..cs5 - input and output signals share the same index
static const uint16_t kHwdSpiSig[HWD_SPI_NUM][5 + HWD_SPI_CS_NUM] = {
  { FSPICLK_OUT_IDX, FSPID_OUT_IDX, FSPIQ_IN_IDX, FSPIHD_OUT_IDX, FSPIWP_OUT_IDX,
    FSPICS0_OUT_IDX, FSPICS1_OUT_IDX, FSPICS2_OUT_IDX, FSPICS3_OUT_IDX, FSPICS4_OUT_IDX, FSPICS5_OUT_IDX },
};
// IO_MUX function names: sck, d, q, hd, wp, cs0
static const char * const kHwdSpiIomux[HWD_SPI_NUM][6] = {
  { "FSPICLK", "FSPID", "FSPIQ", "FSPIHD", "FSPIWP", "FSPICS0" },
};

// RTC_IO / LP_IO: none on C3 (SOC_RTCIO_PIN_COUNT == 0), GPIO0-5 are in the VDD3P3_RTC domain
// Sleep: RTC_CNTL registers
#define HWDUMP_SLEEP
#include "soc/rtc_cntl_reg.h"
#include "soc/rtc.h"
#include "esp_sleep.h"
#include "esp_pm.h"
// RTC_CNTL_WAKEUP_ENA bits, from esp_hw_support/port/esp32c3/include/soc/rtc.h (RTC_xxx_TRIG_EN)
// index = bit number, empty = not used on this target
const char kHwdWakeupNames[] PROGMEM = "||gpio|timer||wifi|uart0|uart1|||bt||xtal32k_dead||usb||brownout";
#define HWD_WAKEUP_BITS   17
static_assert(RTC_GPIO_TRIG_EN == BIT(2) && RTC_TIMER_TRIG_EN == BIT(3) && RTC_USB_TRIG_EN == BIT(14), "HwDump: unexpected RTC wakeup bits");
static inline uint32_t HwdWakeupEna(void) { return REG_GET_FIELD(RTC_CNTL_WAKEUP_STATE_REG, RTC_CNTL_WAKEUP_ENA); }
// Pads in the VDD3P3_RTC domain (deep-sleep wakeup capable)
static inline bool HwdPinIsRtc(uint32_t pin) { return SOC_GPIO_DEEP_SLEEP_WAKE_VALID_GPIO_MASK & BIT64(pin); }
static inline bool HwdDeepSleepWakeEn(uint32_t pin) { return REG_READ(RTC_CNTL_GPIO_WAKEUP_REG) & BIT(RTC_CNTL_GPIO_PIN0_WAKEUP_ENABLE_S - pin); }
static inline uint32_t HwdDeepSleepWakeType(uint32_t pin) { return (REG_READ(RTC_CNTL_GPIO_WAKEUP_REG) >> (RTC_CNTL_GPIO_PIN0_INT_TYPE_S - pin * 3)) & 0x7; }
static inline bool HwdDeepSleepWakeClk(void) { return REG_READ(RTC_CNTL_GPIO_WAKEUP_REG) & RTC_CNTL_GPIO_PIN_CLK_GATE; }
static inline bool HwdPadHold(uint32_t pin) {
  if (pin <= GPIO_NUM_5) { return REG_READ(RTC_CNTL_PAD_HOLD_REG) & BIT(pin); }     // same as gpio_ll_hold_en()
  return REG_READ(RTC_CNTL_DIG_PAD_HOLD_REG) & GPIO_HOLD_MASK[pin];
}
#define HWDUMP_SLEEP_DG_PAD_HOLD                            // RTC_CNTL_DIG_ISO_REG pad hold bits
static inline bool HwdDgPadAutohold(void) { return REG_READ(RTC_CNTL_DIG_ISO_REG) & RTC_CNTL_DG_PAD_AUTOHOLD; }
static inline bool HwdDgPadAutoholdEn(void) { return REG_READ(RTC_CNTL_DIG_ISO_REG) & RTC_CNTL_DG_PAD_AUTOHOLD_EN; }
static inline bool HwdDgPadForceHold(void) { return REG_READ(RTC_CNTL_DIG_ISO_REG) & RTC_CNTL_DG_PAD_FORCE_HOLD; }
static inline uint32_t HwdGpioIntType(uint32_t pin) { return GPIO.pin[pin].int_type; }
static inline bool HwdGpioWakeupEn(uint32_t pin) { return GPIO.pin[pin].wakeup_enable; }

#endif  // CONFIG_IDF_TARGET_ESP32C3
