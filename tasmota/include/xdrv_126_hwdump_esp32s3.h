/*
  xdrv_126_hwdump_esp32s3.h - HwDump per-target data and register accessors for ESP32-S3

  SPDX-FileCopyrightText: 2026 Stephan Hadinger

  SPDX-License-Identifier: GPL-3.0-only
*/

// Only included from tasmota_xdrv_driver/xdrv_126_hwdump.ino, after HwdPad_t, HwdSig_t and HWD_SIG() are defined.
// Defines data (not only declarations), so it must be included exactly once.
#pragma once

#if CONFIG_IDF_TARGET_ESP32S3

/*********************************************************************************************\
 * ESP32-S3
 *
 * Bus clock and reset bits are in SYSTEM_PERIP_CLK_EN0/1_REG / SYSTEM_PERIP_RST_EN0/1_REG, read through
 * periph_ll_periph_enabled() (USB-Serial-JTAG is not covered by it and is read from SYSTEM directly).
 * Peripheral register layouts are close to ESP32-C3 (LEDC, RMT, I2S, SPI, UART, I2C, USB-Serial-JTAG), the shared
 * C3-layout dump functions are used with the hooks below. RTC_IO and sleep (ext0/ext1) are close to ESP32-S2.
 * USB-OTG is not dumped.
\*********************************************************************************************/
#define HWDUMP_SUPPORTED
#include "hal/clk_gate_ll.h"          // periph_ll_periph_enabled()
#include "soc/system_struct.h"        // SYSTEM
#include "soc/rtc_io_periph.h"        // rtc_io_desc[], rtc_io_num_map[]

// Pad names and IO_MUX functions from soc/io_mux_reg.h (FUNC_<pad>_<function> = MCU_SEL value, empty = no function)
// GPIO22-25 do not exist (filtered by GPIO_IS_VALID_GPIO)
const HwdPad_t kHwdPads[] = {
  { "GPIO0",      "GPIO0_0|GPIO0" },                                      //  0
  { "GPIO1",      "GPIO1_0|GPIO1" },                                      //  1
  { "GPIO2",      "GPIO2_0|GPIO2" },                                      //  2
  { "GPIO3",      "GPIO3_0|GPIO3" },                                      //  3
  { "GPIO4",      "GPIO4_0|GPIO4" },                                      //  4
  { "GPIO5",      "GPIO5_0|GPIO5" },                                      //  5
  { "GPIO6",      "GPIO6_0|GPIO6" },                                      //  6
  { "GPIO7",      "GPIO7_0|GPIO7" },                                      //  7
  { "GPIO8",      "GPIO8_0|GPIO8||SUBSPICS1" },                           //  8
  { "GPIO9",      "GPIO9_0|GPIO9||SUBSPIHD|FSPIHD" },                     //  9
  { "GPIO10",     "GPIO10_0|GPIO10|FSPIIO4|SUBSPICS0|FSPICS0" },          // 10
  { "GPIO11",     "GPIO11_0|GPIO11|FSPIIO5|SUBSPID|FSPID" },              // 11
  { "GPIO12",     "GPIO12_0|GPIO12|FSPIIO6|SUBSPICLK|FSPICLK" },          // 12
  { "GPIO13",     "GPIO13_0|GPIO13|FSPIIO7|SUBSPIQ|FSPIQ" },              // 13
  { "GPIO14",     "GPIO14_0|GPIO14|FSPIDQS|SUBSPIWP|FSPIWP" },            // 14
  { "XTAL_32K_P", "GPIO15_0|GPIO15|U0RTS" },                              // 15
  { "XTAL_32K_N", "GPIO16_0|GPIO16|U0CTS" },                              // 16
  { "DAC_1",      "GPIO17_0|GPIO17|U1TXD" },                              // 17
  { "DAC_2",      "GPIO18_0|GPIO18|U1RXD|CLK_OUT3" },                     // 18
  { "GPIO19",     "GPIO19_0|GPIO19|U1RTS|CLK_OUT2" },                     // 19  USB D-
  { "GPIO20",     "GPIO20_0|GPIO20|U1CTS|CLK_OUT1" },                     // 20  USB D+
  { "GPIO21",     "GPIO21_0|GPIO21" },                                    // 21
  { "-",          "" },                                                   // 22
  { "-",          "" },                                                   // 23
  { "-",          "" },                                                   // 24
  { "-",          "" },                                                   // 25
  { "SPICS1",     "SPICS1|GPIO26" },                                      // 26
  { "SPIHD",      "SPIHD|GPIO27" },                                       // 27
  { "SPIWP",      "SPIWP|GPIO28" },                                       // 28
  { "SPICS0",     "SPICS0|GPIO29" },                                      // 29
  { "SPICLK",     "SPICLK|GPIO30" },                                      // 30
  { "SPIQ",       "SPIQ|GPIO31" },                                        // 31
  { "SPID",       "SPID|GPIO32" },                                        // 32
  { "GPIO33",     "GPIO33_0|GPIO33|FSPIHD|SUBSPIHD|SPIIO4" },             // 33
  { "GPIO34",     "GPIO34_0|GPIO34|FSPICS0|SUBSPICS0|SPIIO5" },           // 34
  { "GPIO35",     "GPIO35_0|GPIO35|FSPID|SUBSPID|SPIIO6" },               // 35
  { "GPIO36",     "GPIO36_0|GPIO36|FSPICLK|SUBSPICLK|SPIIO7" },           // 36
  { "GPIO37",     "GPIO37_0|GPIO37|FSPIQ|SUBSPIQ|SPIDQS" },               // 37
  { "GPIO38",     "GPIO38_0|GPIO38|FSPIWP|SUBSPIWP" },                    // 38
  { "MTCK",       "MTCK|GPIO39|CLK_OUT3|SUBSPICS1" },                     // 39
  { "MTDO",       "MTDO|GPIO40|CLK_OUT2" },                               // 40
  { "MTDI",       "MTDI|GPIO41|CLK_OUT1" },                               // 41
  { "MTMS",       "MTMS|GPIO42" },                                        // 42
  { "U0TXD",      "U0TXD|GPIO43|CLK_OUT1" },                              // 43
  { "U0RXD",      "U0RXD|GPIO44|CLK_OUT2" },                              // 44
  { "GPIO45",     "GPIO45_0|GPIO45" },                                    // 45
  { "GPIO46",     "GPIO46_0|GPIO46" },                                    // 46
  { "SPICLK_P",   "SPICLK_DIFF|GPIO47|SUBSPICLK_DIFF" },                  // 47
  { "SPICLK_N",   "SPICLK_DIFF|GPIO48|SUBSPICLK_DIFF" },                  // 48
};

// Sanity checks of the function indexes above against ESP-IDF definitions
static_assert(PIN_FUNC_GPIO == 1, "HwDump: unexpected PIN_FUNC_GPIO");
static_assert(FUNC_GPIO0_GPIO0_0 == 0 && FUNC_GPIO0_GPIO0 == 1, "HwDump: IO_MUX pad 0");
static_assert(FUNC_GPIO8_SUBSPICS1 == 3 && FUNC_GPIO9_SUBSPIHD == 3 && FUNC_GPIO9_FSPIHD == 4, "HwDump: IO_MUX pads 8-9");
static_assert(FUNC_GPIO10_FSPIIO4 == 2 && FUNC_GPIO10_SUBSPICS0 == 3 && FUNC_GPIO10_FSPICS0 == 4, "HwDump: IO_MUX pad 10");
static_assert(FUNC_GPIO11_FSPIIO5 == 2 && FUNC_GPIO11_SUBSPID == 3 && FUNC_GPIO11_FSPID == 4, "HwDump: IO_MUX pad 11");
static_assert(FUNC_GPIO12_FSPIIO6 == 2 && FUNC_GPIO12_SUBSPICLK == 3 && FUNC_GPIO12_FSPICLK == 4, "HwDump: IO_MUX pad 12");
static_assert(FUNC_GPIO13_FSPIIO7 == 2 && FUNC_GPIO13_SUBSPIQ == 3 && FUNC_GPIO13_FSPIQ == 4, "HwDump: IO_MUX pad 13");
static_assert(FUNC_GPIO14_FSPIDQS == 2 && FUNC_GPIO14_SUBSPIWP == 3 && FUNC_GPIO14_FSPIWP == 4, "HwDump: IO_MUX pad 14");
static_assert(FUNC_XTAL_32K_P_U0RTS == 2 && FUNC_XTAL_32K_N_U0CTS == 2 && FUNC_DAC_1_U1TXD == 2 &&
              FUNC_DAC_2_U1RXD == 2 && FUNC_DAC_2_CLK_OUT3 == 3, "HwDump: IO_MUX pads 15-18");
static_assert(FUNC_GPIO19_U1RTS == 2 && FUNC_GPIO19_CLK_OUT2 == 3 && FUNC_GPIO20_U1CTS == 2 && FUNC_GPIO20_CLK_OUT1 == 3,
              "HwDump: IO_MUX pads 19-20");
static_assert(FUNC_SPICS1_SPICS1 == 0 && FUNC_SPICS1_GPIO26 == 1 && FUNC_SPID_SPID == 0 && FUNC_SPID_GPIO32 == 1,
              "HwDump: IO_MUX pads 26-32");
static_assert(FUNC_GPIO33_FSPIHD == 2 && FUNC_GPIO33_SUBSPIHD == 3 && FUNC_GPIO33_SPIIO4 == 4 &&
              FUNC_GPIO34_FSPICS0 == 2 && FUNC_GPIO34_SPIIO5 == 4 && FUNC_GPIO35_FSPID == 2 && FUNC_GPIO35_SPIIO6 == 4,
              "HwDump: IO_MUX pads 33-35");
static_assert(FUNC_GPIO36_FSPICLK == 2 && FUNC_GPIO36_SPIIO7 == 4 && FUNC_GPIO37_FSPIQ == 2 && FUNC_GPIO37_SPIDQS == 4 &&
              FUNC_GPIO38_FSPIWP == 2 && FUNC_GPIO38_SUBSPIWP == 3, "HwDump: IO_MUX pads 36-38");
static_assert(FUNC_MTCK_MTCK == 0 && FUNC_MTCK_CLK_OUT3 == 2 && FUNC_MTCK_SUBSPICS1 == 3 && FUNC_MTDO_CLK_OUT2 == 2 &&
              FUNC_MTDI_CLK_OUT1 == 2 && FUNC_MTMS_GPIO42 == 1, "HwDump: IO_MUX pads 39-42");
static_assert(FUNC_U0TXD_U0TXD == 0 && FUNC_U0TXD_CLK_OUT1 == 2 && FUNC_U0RXD_U0RXD == 0 && FUNC_U0RXD_CLK_OUT2 == 2,
              "HwDump: IO_MUX pads 43-44");
static_assert(FUNC_SPICLK_P_SPICLK_DIFF == 0 && FUNC_SPICLK_P_GPIO47 == 1 && FUNC_SPICLK_P_SUBSPICLK_DIFF == 2 &&
              FUNC_SPICLK_N_SPICLK_DIFF == 0 && FUNC_SPICLK_N_GPIO48 == 1 && FUNC_SPICLK_N_SUBSPICLK_DIFF == 2,
              "HwDump: IO_MUX pads 47-48");

// GPIO matrix output signals (GPIO_FUNCn_OUT_SEL), from soc/gpio_sig_map.h
const HwdSig_t kHwdSigOut[] = {
  HWD_SIG(SPIQ_OUT), HWD_SIG(SPID_OUT), HWD_SIG(SPIHD_OUT), HWD_SIG(SPIWP_OUT),
  HWD_SIG(SPICLK_OUT), HWD_SIG(SPICS0_OUT), HWD_SIG(SPICS1_OUT),
  HWD_SIG(SPID4_OUT), HWD_SIG(SPID5_OUT), HWD_SIG(SPID6_OUT), HWD_SIG(SPID7_OUT), HWD_SIG(SPIDQS_OUT),
  HWD_SIG(U0TXD_OUT), HWD_SIG(U0RTS_OUT), HWD_SIG(U0DTR_OUT),
  HWD_SIG(U1TXD_OUT), HWD_SIG(U1RTS_OUT), HWD_SIG(U1DTR_OUT),
  HWD_SIG(U2TXD_OUT), HWD_SIG(U2RTS_OUT), HWD_SIG(U2DTR_OUT),
  HWD_SIG(I2S1_MCLK_OUT), HWD_SIG(I2S0O_BCK_OUT), HWD_SIG(I2S0_MCLK_OUT), HWD_SIG(I2S0O_WS_OUT), HWD_SIG(I2S0O_SD_OUT),
  HWD_SIG(I2S0I_BCK_OUT), HWD_SIG(I2S0I_WS_OUT),
  HWD_SIG(I2S1O_BCK_OUT), HWD_SIG(I2S1O_WS_OUT), HWD_SIG(I2S1O_SD_OUT), HWD_SIG(I2S1I_BCK_OUT), HWD_SIG(I2S1I_WS_OUT),
  HWD_SIG(GPIO_WLAN_PRIO), HWD_SIG(GPIO_WLAN_ACTIVE),
  HWD_SIG(BB_DIAG0), HWD_SIG(BB_DIAG1), HWD_SIG(BB_DIAG2), HWD_SIG(BB_DIAG3), HWD_SIG(BB_DIAG4),
  HWD_SIG(BB_DIAG5), HWD_SIG(BB_DIAG6), HWD_SIG(BB_DIAG7), HWD_SIG(BB_DIAG8), HWD_SIG(BB_DIAG9),
  HWD_SIG(BB_DIAG10), HWD_SIG(BB_DIAG11), HWD_SIG(BB_DIAG12), HWD_SIG(BB_DIAG13), HWD_SIG(BB_DIAG14),
  HWD_SIG(BB_DIAG15), HWD_SIG(BB_DIAG16), HWD_SIG(BB_DIAG17), HWD_SIG(BB_DIAG18),
  HWD_SIG(CORE1_GPIO_OUT7),
  HWD_SIG(USB_EXTPHY_OEN), HWD_SIG(USB_EXTPHY_SPEED), HWD_SIG(USB_EXTPHY_VPO), HWD_SIG(USB_EXTPHY_VMO),
  HWD_SIG(USB_EXTPHY_SUSPND), HWD_SIG(USB_OTG_IDPULLUP), HWD_SIG(USB_OTG_DPPULLDOWN), HWD_SIG(USB_OTG_DMPULLDOWN),
  HWD_SIG(USB_OTG_DRVVBUS), HWD_SIG(USB_SRP_CHRGVBUS), HWD_SIG(USB_SRP_DISCHRGVBUS),
  HWD_SIG(SPI3_CLK_OUT), HWD_SIG(SPI3_Q_OUT), HWD_SIG(SPI3_D_OUT), HWD_SIG(SPI3_HD_OUT), HWD_SIG(SPI3_WP_OUT),
  HWD_SIG(SPI3_CS0_OUT), HWD_SIG(SPI3_CS1_OUT),
  HWD_SIG(LEDC_LS_SIG_OUT0), HWD_SIG(LEDC_LS_SIG_OUT1), HWD_SIG(LEDC_LS_SIG_OUT2), HWD_SIG(LEDC_LS_SIG_OUT3),
  HWD_SIG(LEDC_LS_SIG_OUT4), HWD_SIG(LEDC_LS_SIG_OUT5), HWD_SIG(LEDC_LS_SIG_OUT6), HWD_SIG(LEDC_LS_SIG_OUT7),
  HWD_SIG(RMT_SIG_OUT0), HWD_SIG(RMT_SIG_OUT1), HWD_SIG(RMT_SIG_OUT2), HWD_SIG(RMT_SIG_OUT3),
  HWD_SIG(USB_JTAG_TCK), HWD_SIG(USB_JTAG_TMS), HWD_SIG(USB_JTAG_TDI), HWD_SIG(USB_JTAG_TDO),
  HWD_SIG(I2CEXT0_SCL_OUT), HWD_SIG(I2CEXT0_SDA_OUT), HWD_SIG(I2CEXT1_SCL_OUT), HWD_SIG(I2CEXT1_SDA_OUT),
  HWD_SIG(GPIO_SD0_OUT), HWD_SIG(GPIO_SD1_OUT), HWD_SIG(GPIO_SD2_OUT), HWD_SIG(GPIO_SD3_OUT),
  HWD_SIG(GPIO_SD4_OUT), HWD_SIG(GPIO_SD5_OUT), HWD_SIG(GPIO_SD6_OUT), HWD_SIG(GPIO_SD7_OUT),
  HWD_SIG(FSPICLK_OUT), HWD_SIG(FSPIQ_OUT), HWD_SIG(FSPID_OUT), HWD_SIG(FSPIHD_OUT), HWD_SIG(FSPIWP_OUT),
  HWD_SIG(FSPIIO4_OUT), HWD_SIG(FSPIIO5_OUT), HWD_SIG(FSPIIO6_OUT), HWD_SIG(FSPIIO7_OUT),
  HWD_SIG(FSPICS0_OUT), HWD_SIG(FSPICS1_OUT), HWD_SIG(FSPICS2_OUT), HWD_SIG(FSPICS3_OUT),
  HWD_SIG(FSPICS4_OUT), HWD_SIG(FSPICS5_OUT),
  HWD_SIG(TWAI_TX), HWD_SIG(TWAI_BUS_OFF_ON), HWD_SIG(TWAI_CLKOUT),
  HWD_SIG(SUBSPICLK_OUT), HWD_SIG(SUBSPIQ_OUT), HWD_SIG(SUBSPID_OUT), HWD_SIG(SUBSPIHD_OUT), HWD_SIG(SUBSPIWP_OUT),
  HWD_SIG(SUBSPICS0_OUT), HWD_SIG(SUBSPICS1_OUT),
  HWD_SIG(FSPIDQS_OUT), HWD_SIG(SPI3_CS2_OUT), HWD_SIG(I2S0O_SD1_OUT),
  HWD_SIG(CORE1_GPIO_OUT0), HWD_SIG(CORE1_GPIO_OUT1), HWD_SIG(CORE1_GPIO_OUT2),
  HWD_SIG(LCD_CS),
  HWD_SIG(LCD_DATA_OUT0), HWD_SIG(LCD_DATA_OUT1), HWD_SIG(LCD_DATA_OUT2), HWD_SIG(LCD_DATA_OUT3),
  HWD_SIG(LCD_DATA_OUT4), HWD_SIG(LCD_DATA_OUT5), HWD_SIG(LCD_DATA_OUT6), HWD_SIG(LCD_DATA_OUT7),
  HWD_SIG(LCD_DATA_OUT8), HWD_SIG(LCD_DATA_OUT9), HWD_SIG(LCD_DATA_OUT10), HWD_SIG(LCD_DATA_OUT11),
  HWD_SIG(LCD_DATA_OUT12), HWD_SIG(LCD_DATA_OUT13), HWD_SIG(LCD_DATA_OUT14), HWD_SIG(LCD_DATA_OUT15),
  HWD_SIG(CAM_CLK), HWD_SIG(LCD_H_ENABLE), HWD_SIG(LCD_H_SYNC), HWD_SIG(LCD_V_SYNC), HWD_SIG(LCD_DC), HWD_SIG(LCD_PCLK),
  HWD_SIG(SUBSPID4_OUT), HWD_SIG(SUBSPID5_OUT), HWD_SIG(SUBSPID6_OUT), HWD_SIG(SUBSPID7_OUT), HWD_SIG(SUBSPIDQS_OUT),
  HWD_SIG(PWM0_OUT0A), HWD_SIG(PWM0_OUT0B), HWD_SIG(PWM0_OUT1A), HWD_SIG(PWM0_OUT1B), HWD_SIG(PWM0_OUT2A), HWD_SIG(PWM0_OUT2B),
  HWD_SIG(PWM1_OUT0A), HWD_SIG(PWM1_OUT0B), HWD_SIG(PWM1_OUT1A), HWD_SIG(PWM1_OUT1B), HWD_SIG(PWM1_OUT2A), HWD_SIG(PWM1_OUT2B),
  HWD_SIG(SDHOST_CCLK_OUT_1), HWD_SIG(SDHOST_CCLK_OUT_2), HWD_SIG(SDHOST_RST_N_1), HWD_SIG(SDHOST_RST_N_2),
  { 176, "SDHOST_CCMD_OD_PULLUP_EN_N" },          // SDHOST_CCMD_OD_PULLUP_EN_N_IDX is malformed in gpio_sig_map.h (no value)
  HWD_SIG(SDIO_TOHOST_INT_OUT),
  HWD_SIG(SDHOST_CCMD_OUT_1), HWD_SIG(SDHOST_CCMD_OUT_2),
  HWD_SIG(SDHOST_CDATA_OUT_10), HWD_SIG(SDHOST_CDATA_OUT_11), HWD_SIG(SDHOST_CDATA_OUT_12), HWD_SIG(SDHOST_CDATA_OUT_13),
  HWD_SIG(SDHOST_CDATA_OUT_14), HWD_SIG(SDHOST_CDATA_OUT_15), HWD_SIG(SDHOST_CDATA_OUT_16), HWD_SIG(SDHOST_CDATA_OUT_17),
  HWD_SIG(BT_AUDIO0_IRQ), HWD_SIG(BT_AUDIO1_IRQ), HWD_SIG(BT_AUDIO2_IRQ),
  HWD_SIG(BLE_AUDIO0_IRQ), HWD_SIG(BLE_AUDIO1_IRQ), HWD_SIG(BLE_AUDIO2_IRQ),
  HWD_SIG(PCMFSYNC_OUT), HWD_SIG(PCMCLK_OUT), HWD_SIG(PCMDOUT),
  HWD_SIG(BLE_AUDIO_SYNC0_P), HWD_SIG(BLE_AUDIO_SYNC1_P), HWD_SIG(BLE_AUDIO_SYNC2_P),
  HWD_SIG(ANT_SEL0), HWD_SIG(ANT_SEL1), HWD_SIG(ANT_SEL2), HWD_SIG(ANT_SEL3),
  HWD_SIG(ANT_SEL4), HWD_SIG(ANT_SEL5), HWD_SIG(ANT_SEL6), HWD_SIG(ANT_SEL7),
  HWD_SIG(SIG_IN_FUNC_208), HWD_SIG(SIG_IN_FUNC_209), HWD_SIG(SIG_IN_FUNC_210), HWD_SIG(SIG_IN_FUNC_211), HWD_SIG(SIG_IN_FUNC_212),
  HWD_SIG(SDHOST_CDATA_OUT_20), HWD_SIG(SDHOST_CDATA_OUT_21), HWD_SIG(SDHOST_CDATA_OUT_22), HWD_SIG(SDHOST_CDATA_OUT_23),
  HWD_SIG(SDHOST_CDATA_OUT_24), HWD_SIG(SDHOST_CDATA_OUT_25), HWD_SIG(SDHOST_CDATA_OUT_26), HWD_SIG(SDHOST_CDATA_OUT_27),
  HWD_SIG(PRO_ALONEGPIO_OUT0), HWD_SIG(PRO_ALONEGPIO_OUT1), HWD_SIG(PRO_ALONEGPIO_OUT2), HWD_SIG(PRO_ALONEGPIO_OUT3),
  HWD_SIG(PRO_ALONEGPIO_OUT4), HWD_SIG(PRO_ALONEGPIO_OUT5), HWD_SIG(PRO_ALONEGPIO_OUT6), HWD_SIG(PRO_ALONEGPIO_OUT7),
  HWD_SIG(SYNCERR), HWD_SIG(SYNCFOUND_FLAG), HWD_SIG(EVT_CNTL_IMMEDIATE_ABORT), HWD_SIG(LINKLBL),
  HWD_SIG(DATA_EN), HWD_SIG(DATA), HWD_SIG(PKT_TX_ON), HWD_SIG(PKT_RX_ON), HWD_SIG(RW_TX_ON), HWD_SIG(RW_RX_ON),
  HWD_SIG(EVT_REQ_P), HWD_SIG(EVT_STOP_P), HWD_SIG(BT_MODE_ON),
  HWD_SIG(GPIO_LC_DIAG0), HWD_SIG(GPIO_LC_DIAG1), HWD_SIG(GPIO_LC_DIAG2),
  HWD_SIG(CH_IDX), HWD_SIG(RX_WINDOW), HWD_SIG(UPDATE_RX), HWD_SIG(RX_STATUS),
  HWD_SIG(CLK_GPIO), HWD_SIG(NBT_BLE),
  HWD_SIG(USB_JTAG_TRST),
  HWD_SIG(CORE1_GPIO_OUT3), HWD_SIG(CORE1_GPIO_OUT4), HWD_SIG(CORE1_GPIO_OUT5), HWD_SIG(CORE1_GPIO_OUT6),
  { SIG_GPIO_OUT_IDX, "GPIO" },                   // simple GPIO output (GPIO_OUT register)
};

// GPIO matrix input signals (GPIO_FUNCm_IN_SEL_CFG), from soc/gpio_sig_map.h
const HwdSig_t kHwdSigIn[] = {
  HWD_SIG(SPIQ_IN), HWD_SIG(SPID_IN), HWD_SIG(SPIHD_IN), HWD_SIG(SPIWP_IN),
  HWD_SIG(SPID4_IN), HWD_SIG(SPID5_IN), HWD_SIG(SPID6_IN), HWD_SIG(SPID7_IN), HWD_SIG(SPIDQS_IN),
  HWD_SIG(U0RXD_IN), HWD_SIG(U0CTS_IN), HWD_SIG(U0DSR_IN),
  HWD_SIG(U1RXD_IN), HWD_SIG(U1CTS_IN), HWD_SIG(U1DSR_IN),
  HWD_SIG(U2RXD_IN), HWD_SIG(U2CTS_IN), HWD_SIG(U2DSR_IN),
  HWD_SIG(I2S1_MCLK_IN), HWD_SIG(I2S0O_BCK_IN), HWD_SIG(I2S0_MCLK_IN), HWD_SIG(I2S0O_WS_IN), HWD_SIG(I2S0I_SD_IN),
  HWD_SIG(I2S0I_BCK_IN), HWD_SIG(I2S0I_WS_IN),
  HWD_SIG(I2S1O_BCK_IN), HWD_SIG(I2S1O_WS_IN), HWD_SIG(I2S1I_SD_IN), HWD_SIG(I2S1I_BCK_IN), HWD_SIG(I2S1I_WS_IN),
  HWD_SIG(PCNT_SIG_CH0_IN0), HWD_SIG(PCNT_SIG_CH1_IN0), HWD_SIG(PCNT_CTRL_CH0_IN0), HWD_SIG(PCNT_CTRL_CH1_IN0),
  HWD_SIG(PCNT_SIG_CH0_IN1), HWD_SIG(PCNT_SIG_CH1_IN1), HWD_SIG(PCNT_CTRL_CH0_IN1), HWD_SIG(PCNT_CTRL_CH1_IN1),
  HWD_SIG(PCNT_SIG_CH0_IN2), HWD_SIG(PCNT_SIG_CH1_IN2), HWD_SIG(PCNT_CTRL_CH0_IN2), HWD_SIG(PCNT_CTRL_CH1_IN2),
  HWD_SIG(PCNT_SIG_CH0_IN3), HWD_SIG(PCNT_SIG_CH1_IN3), HWD_SIG(PCNT_CTRL_CH0_IN3), HWD_SIG(PCNT_CTRL_CH1_IN3),
  HWD_SIG(GPIO_BT_ACTIVE), HWD_SIG(GPIO_BT_PRIORITY),
  HWD_SIG(I2S0I_SD1_IN), HWD_SIG(I2S0I_SD2_IN), HWD_SIG(I2S0I_SD3_IN),
  HWD_SIG(CORE1_GPIO_IN7),
  HWD_SIG(USB_EXTPHY_VP), HWD_SIG(USB_EXTPHY_VM), HWD_SIG(USB_EXTPHY_RCV),
  HWD_SIG(USB_OTG_IDDIG_IN), HWD_SIG(USB_OTG_AVALID_IN), HWD_SIG(USB_SRP_BVALID_IN), HWD_SIG(USB_OTG_VBUSVALID_IN),
  HWD_SIG(USB_SRP_SESSEND_IN),
  HWD_SIG(SPI3_CLK_IN), HWD_SIG(SPI3_Q_IN), HWD_SIG(SPI3_D_IN), HWD_SIG(SPI3_HD_IN), HWD_SIG(SPI3_WP_IN), HWD_SIG(SPI3_CS0_IN),
  HWD_SIG(EXT_ADC_START),
  HWD_SIG(RMT_SIG_IN0), HWD_SIG(RMT_SIG_IN1), HWD_SIG(RMT_SIG_IN2), HWD_SIG(RMT_SIG_IN3),
  HWD_SIG(I2CEXT0_SCL_IN), HWD_SIG(I2CEXT0_SDA_IN), HWD_SIG(I2CEXT1_SCL_IN), HWD_SIG(I2CEXT1_SDA_IN),
  HWD_SIG(FSPICLK_IN), HWD_SIG(FSPIQ_IN), HWD_SIG(FSPID_IN), HWD_SIG(FSPIHD_IN), HWD_SIG(FSPIWP_IN),
  HWD_SIG(FSPIIO4_IN), HWD_SIG(FSPIIO5_IN), HWD_SIG(FSPIIO6_IN), HWD_SIG(FSPIIO7_IN), HWD_SIG(FSPICS0_IN),
  HWD_SIG(TWAI_RX),
  HWD_SIG(SUBSPIQ_IN), HWD_SIG(SUBSPID_IN), HWD_SIG(SUBSPIHD_IN), HWD_SIG(SUBSPIWP_IN),
  HWD_SIG(CORE1_GPIO_IN0), HWD_SIG(CORE1_GPIO_IN1), HWD_SIG(CORE1_GPIO_IN2),
  HWD_SIG(CAM_DATA_IN0), HWD_SIG(CAM_DATA_IN1), HWD_SIG(CAM_DATA_IN2), HWD_SIG(CAM_DATA_IN3),
  HWD_SIG(CAM_DATA_IN4), HWD_SIG(CAM_DATA_IN5), HWD_SIG(CAM_DATA_IN6), HWD_SIG(CAM_DATA_IN7),
  HWD_SIG(CAM_DATA_IN8), HWD_SIG(CAM_DATA_IN9), HWD_SIG(CAM_DATA_IN10), HWD_SIG(CAM_DATA_IN11),
  HWD_SIG(CAM_DATA_IN12), HWD_SIG(CAM_DATA_IN13), HWD_SIG(CAM_DATA_IN14), HWD_SIG(CAM_DATA_IN15),
  HWD_SIG(CAM_PCLK), HWD_SIG(CAM_H_ENABLE), HWD_SIG(CAM_H_SYNC), HWD_SIG(CAM_V_SYNC),
  HWD_SIG(SUBSPID4_IN), HWD_SIG(SUBSPID5_IN), HWD_SIG(SUBSPID6_IN), HWD_SIG(SUBSPID7_IN), HWD_SIG(SUBSPIDQS_IN),
  HWD_SIG(PWM0_SYNC0_IN), HWD_SIG(PWM0_SYNC1_IN), HWD_SIG(PWM0_SYNC2_IN), HWD_SIG(PWM0_F0_IN), HWD_SIG(PWM0_F1_IN),
  HWD_SIG(PWM0_F2_IN), HWD_SIG(PWM0_CAP0_IN), HWD_SIG(PWM0_CAP1_IN), HWD_SIG(PWM0_CAP2_IN),
  HWD_SIG(PWM1_SYNC0_IN), HWD_SIG(PWM1_SYNC1_IN), HWD_SIG(PWM1_SYNC2_IN), HWD_SIG(PWM1_F0_IN), HWD_SIG(PWM1_F1_IN),
  HWD_SIG(PWM1_F2_IN), HWD_SIG(PWM1_CAP0_IN), HWD_SIG(PWM1_CAP1_IN), HWD_SIG(PWM1_CAP2_IN),
  HWD_SIG(SDHOST_CCMD_IN_1), HWD_SIG(SDHOST_CCMD_IN_2),
  HWD_SIG(SDHOST_CDATA_IN_10), HWD_SIG(SDHOST_CDATA_IN_11), HWD_SIG(SDHOST_CDATA_IN_12), HWD_SIG(SDHOST_CDATA_IN_13),
  HWD_SIG(SDHOST_CDATA_IN_14), HWD_SIG(SDHOST_CDATA_IN_15), HWD_SIG(SDHOST_CDATA_IN_16), HWD_SIG(SDHOST_CDATA_IN_17),
  HWD_SIG(PCMFSYNC_IN), HWD_SIG(PCMCLK_IN), HWD_SIG(PCMDIN), HWD_SIG(RW_WAKEUP_REQ),
  HWD_SIG(SDHOST_DATA_STROBE_1), HWD_SIG(SDHOST_DATA_STROBE_2),
  HWD_SIG(SDHOST_CARD_DETECT_N_1), HWD_SIG(SDHOST_CARD_DETECT_N_2),
  HWD_SIG(SDHOST_CARD_WRITE_PRT_1), HWD_SIG(SDHOST_CARD_WRITE_PRT_2),
  HWD_SIG(SDHOST_CARD_INT_N_1), HWD_SIG(SDHOST_CARD_INT_N_2),
  HWD_SIG(SDHOST_CDATA_IN_20), HWD_SIG(SDHOST_CDATA_IN_21), HWD_SIG(SDHOST_CDATA_IN_22), HWD_SIG(SDHOST_CDATA_IN_23),
  HWD_SIG(SDHOST_CDATA_IN_24), HWD_SIG(SDHOST_CDATA_IN_25), HWD_SIG(SDHOST_CDATA_IN_26), HWD_SIG(SDHOST_CDATA_IN_27),
  HWD_SIG(PRO_ALONEGPIO_IN0), HWD_SIG(PRO_ALONEGPIO_IN1), HWD_SIG(PRO_ALONEGPIO_IN2), HWD_SIG(PRO_ALONEGPIO_IN3),
  HWD_SIG(PRO_ALONEGPIO_IN4), HWD_SIG(PRO_ALONEGPIO_IN5), HWD_SIG(PRO_ALONEGPIO_IN6), HWD_SIG(PRO_ALONEGPIO_IN7),
  HWD_SIG(USB_JTAG_TDO_BRIDGE),
  HWD_SIG(CORE1_GPIO_IN3), HWD_SIG(CORE1_GPIO_IN4), HWD_SIG(CORE1_GPIO_IN5), HWD_SIG(CORE1_GPIO_IN6),
};

// Target specific register fields (names differ between SoCs)
static inline uint32_t HwdOutLevel(uint32_t pin) { return (pin < 32) ? ((GPIO.out >> pin) & 1) : ((GPIO.out1.val >> (pin - 32)) & 1); }
static inline bool HwdOutInv(uint32_t pin) { return GPIO.func_out_sel_cfg[pin].inv_sel; }
static inline bool HwdInInv(uint32_t sig) { return GPIO.func_in_sel_cfg[sig].sig_in_inv; }

// RTC_IO: GPIO0-21 can be switched to the RTC_IO controller (rtc_io_desc[].mux), then IO_MUX/GPIO matrix settings do not apply
#define HWDUMP_RTCIO
#include "soc/rtc_io_struct.h"
#include "soc/rtc_io_reg.h"
#include "soc/rtc_cntl_reg.h"
#define HWD_RTCIO_HOLD_FORCE_REG  RTC_CNTL_PAD_HOLD_REG       // rtc_io_desc[].hold_force bits (see rtcio_ll_force_hold_enable)
static inline int HwdRtcNum(uint32_t pin) { return (pin < SOC_GPIO_PIN_COUNT) ? rtc_io_num_map[pin] : -1; }
static inline bool HwdPinIsRtc(uint32_t pin) { return HwdRtcNum(pin) >= 0; }
static inline bool HwdPadRtcMux(uint32_t pin) {
  int r = HwdRtcNum(pin);
  return (r >= 0) && rtc_io_desc[r].reg && (REG_READ(rtc_io_desc[r].reg) & rtc_io_desc[r].mux);
}

// LEDC (low speed mode only), fields not covered by ledc_ll getters
#define HWDUMP_LEDC
#define HWDUMP_LEDC_CLK_EN                                  // LEDC_CONF_REG.clk_en exists
#define HWD_LEDC_CLK_LABEL    "clk"                         // global clock shared by all timers
#define HWD_LEDC_MODE_NUM     1
static const uint8_t kHwdLedcModes[HWD_LEDC_MODE_NUM] = { LEDC_LOW_SPEED_MODE };
static_assert(LEDC_LS_SIG_OUT7_IDX == LEDC_LS_SIG_OUT0_IDX + 7, "HwDump: LEDC output signals must be contiguous");
// LEDC_CONF_REG.apb_clk_sel: 0=none 1=APB 2=RC_FAST 3=XTAL (see ledc_ll_set_slow_clk_sel)
// Note: ledc_ll_get_slow_clk_sel() calls abort() when no clock is selected, so read the field directly
const char kHwdLedcClkNames[] PROGMEM = "none|APB|RC_FAST|XTAL";
static inline bool HwdLedcBusClk(void) { return periph_ll_periph_enabled(PERIPH_LEDC_MODULE); }
static inline uint32_t HwdLedcClkSel(void) { return LEDC.conf.apb_clk_sel; }
static inline bool HwdLedcClkEn(void) { return LEDC.conf.clk_en; }
static inline uint32_t HwdLedcClkHz(uint32_t sel) {
  uint32_t hz = 0;
  if (1 == sel) { esp_clk_tree_src_get_freq_hz(SOC_MOD_CLK_APB, ESP_CLK_TREE_SRC_FREQ_PRECISION_APPROX, &hz); }
  else if (2 == sel) { esp_clk_tree_src_get_freq_hz(SOC_MOD_CLK_RC_FAST, ESP_CLK_TREE_SRC_FREQ_PRECISION_APPROX, &hz); }
  else if (3 == sel) { esp_clk_tree_src_get_freq_hz(SOC_MOD_CLK_XTAL, ESP_CLK_TREE_SRC_FREQ_PRECISION_APPROX, &hz); }
  return hz;
}
// No per-timer clock mux: tick_sel exists in the register but is not used (see ledc_ll_get_clock_source)
static inline uint32_t HwdLedcTimerClkHz(uint32_t mode, uint32_t t, uint32_t clk_hz) { return clk_hz; }
static inline bool HwdLedcTimerPaused(uint32_t mode, uint32_t t) { return LEDC.timer_group[mode].timer[t].conf.pause; }
static inline bool HwdLedcTimerRst(uint32_t mode, uint32_t t) { return LEDC.timer_group[mode].timer[t].conf.rst; }
static inline uint32_t HwdLedcTimerCnt(uint32_t mode, uint32_t t) { return LEDC.timer_group[mode].timer[t].value.timer_cnt; }
static inline bool HwdLedcChOutEn(uint32_t mode, uint32_t ch) { return LEDC.channel_group[mode].channel[ch].conf0.sig_out_en; }
static inline uint32_t HwdLedcChIdle(uint32_t mode, uint32_t ch) { return LEDC.channel_group[mode].channel[ch].conf0.idle_lv; }
static inline uint32_t HwdLedcSig(uint32_t mode, uint32_t ch) { return LEDC_LS_SIG_OUT0_IDX + ch; }

// USB-Serial-JTAG controller (USB D-/D+ on GPIO19/20, shared with USB-OTG through conf0.phy_sel)
// Warning: never read USB_SERIAL_JTAG.ep1 (USB_SERIAL_JTAG_EP1_REG), reading it pops a byte from the RX FIFO
#define HWDUMP_USB_SERIAL_JTAG
#include "soc/usb_serial_jtag_struct.h"
static_assert(USB_INT_PHY0_DM_GPIO_NUM == 19 && USB_INT_PHY0_DP_GPIO_NUM == 20, "HwDump: unexpected USB pins");
// same as usb_serial_jtag_ll_module_is_enabled() (not covered by periph_ll_periph_enabled)
static inline bool HwdUsjBusClk(void) { return SYSTEM.perip_clk_en1.usb_device_clk_en && !SYSTEM.perip_rst_en1.usb_device_rst; }
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
#define HwdUartDev(n)   UART_LL_GET_HW(n)                   // macro: no custom types in .ino function signatures
#define HWD_UART_CONF0      conf0                           // UART_CONF0_REG member
#define HWD_UART_CLKDIV     clkdiv                          // UART_CLKDIV_REG member and its integer / fractional fields
#define HWD_UART_DIV_INT    clkdiv
#define HWD_UART_DIV_FRAG   clkdiv_frag
static inline uint32_t HwdUartClkSel(uint32_t n) { return HwdUartDev(n)->clk_conf.sclk_sel; }
#define kHwdUartClkNames  kHwdClk4Names
static inline uint32_t HwdUartClkSoc(uint32_t sel) { return HwdClk4Soc(sel); }
// GPIO matrix signals: tx, rx, rts, cts and IO_MUX function names (nullptr if none)
static const uint16_t kHwdUartSig[SOC_UART_HP_NUM][4] = {
  { U0TXD_OUT_IDX, U0RXD_IN_IDX, U0RTS_OUT_IDX, U0CTS_IN_IDX },
  { U1TXD_OUT_IDX, U1RXD_IN_IDX, U1RTS_OUT_IDX, U1CTS_IN_IDX },
  { U2TXD_OUT_IDX, U2RXD_IN_IDX, U2RTS_OUT_IDX, U2CTS_IN_IDX },
};
static const char * const kHwdUartIomux[SOC_UART_HP_NUM][4] = {
  { "U0TXD", "U0RXD", "U0RTS", "U0CTS" },
  { "U1TXD", "U1RXD", "U1RTS", "U1CTS" },
  { nullptr, nullptr, nullptr, nullptr },
};

// I2C - Warning: never read I2Cn.data (I2C_DATA_REG), reading it pops a byte from the RX FIFO
#define HWDUMP_I2C
#include "hal/i2c_ll.h"
const char kHwdI2cClkNames[] PROGMEM = "XTAL|RC_FAST";                    // I2C_CLK_CONF_REG.sclk_sel (see i2c_ll_set_source_clk)
#define HwdI2cDev(n)        I2C_LL_GET_HW(n)                // macro: no custom types in .ino function signatures
#define HwdI2cClkSel(hw)    ((hw)->clk_conf.sclk_sel)
#define HwdI2cClkDiv(hw)    (HAL_FORCE_READ_U32_REG_FIELD((hw)->clk_conf, sclk_div_num) + 1)
#define HwdI2cIsMaster(hw)  i2c_ll_is_master_mode(hw)
#define HWD_I2C_SR(hw)      ((hw)->sr)                      // I2C_SR_REG
#define HWD_I2C_RXFIFO_CNT  rxfifo_cnt                      // I2C_SR_REG field names
#define HWD_I2C_TXFIFO_CNT  txfifo_cnt
#define HWD_I2C_INT_MASK    0x3FFFF
#define HWD_I2C_SCL_ADJ     0                               // i2c_ll_get_scl_timing() already returns the full high and low periods
static inline bool HwdI2cOn(uint32_t n) { return periph_ll_periph_enabled(n ? PERIPH_I2C1_MODULE : PERIPH_I2C0_MODULE); }
static inline uint32_t HwdI2cClkSoc(uint32_t sel) { return sel ? SOC_MOD_CLK_RC_FAST : SOC_MOD_CLK_XTAL; }
static const uint16_t kHwdI2cSig[SOC_I2C_NUM][4] = {          // scl_out, scl_in, sda_out, sda_in
  { I2CEXT0_SCL_OUT_IDX, I2CEXT0_SCL_IN_IDX, I2CEXT0_SDA_OUT_IDX, I2CEXT0_SDA_IN_IDX },
  { I2CEXT1_SCL_OUT_IDX, I2CEXT1_SCL_IN_IDX, I2CEXT1_SDA_OUT_IDX, I2CEXT1_SDA_IN_IDX },
};

// RMT (4 tx channels 0-3, 4 rx channels 4-7, C3 layout with "_chn" / "_chm" field suffixes)
// Warning: never read RMT.chndata[] / RMT.chmdata[] (RMT_CHnDATA_REG), they are the channel FIFO access ports
#define HWDUMP_RMT
#include "hal/rmt_ll.h"
static inline bool HwdRmtOn(void) { return periph_ll_periph_enabled(PERIPH_RMT_MODULE); }
static_assert(RMT_SIG_OUT3_IDX == RMT_SIG_OUT0_IDX + 3 && RMT_SIG_IN3_IDX == RMT_SIG_IN0_IDX + 3, "HwDump: RMT signals must be contiguous");
#define HWD_RMT_SIG_OUT0        RMT_SIG_OUT0_IDX            // GPIO matrix signals of tx channel 0 / rx channel 0
#define HWD_RMT_SIG_IN0         RMT_SIG_IN0_IDX
#define HWD_RMT_TX_CONF(ch)     (RMT.chnconf0[ch])          // RMT_CHnCONF0_REG
#define HWD_RMT_TX_STATUS(ch)   (RMT.chnstatus[ch])         // RMT_CHnSTATUS_REG
#define HWD_RMT_RX_CONF(i)      (RMT.chmconf[i])            // RMT_CHmCONF0_REG / RMT_CHmCONF1_REG as .conf0 / .conf1
#define HWD_RMT_RX_STATUS(i)    (RMT.chmstatus[i])          // RMT_CHmSTATUS_REG
#define HWD_RMT_TX_F(f)         f##_chn                     // tx field names: mem_size -> mem_size_chn
#define HWD_RMT_RX_F(f)         f##_chm                     // rx field names: mem_size -> mem_size_chm
#define HWD_RMT_INT_MASK        0x3FFFFFFF                  // RMT_INT_RAW_REG bits 0..29
// Group clock in RMT_SYS_CONF_REG
#define kHwdRmtClkNames  kHwdClk4Names
static inline uint32_t HwdRmtClkSoc(uint32_t sel) { return HwdClk4Soc(sel); }
static inline uint32_t HwdRmtSclkSel(void) { return RMT.sys_conf.sclk_sel; }
static inline uint32_t HwdRmtSclkDiv(void) { return HAL_FORCE_READ_U32_REG_FIELD(RMT.sys_conf, sclk_div_num) + 1; }
static inline bool HwdRmtMemForcePd(void) { return RMT.sys_conf.mem_force_pd; }

// I2S0/I2S1 (C3 layout, no FIFO register, data goes through GDMA)
#define HWDUMP_I2S
#include "soc/i2s_struct.h"
const char kHwdI2sClkNames[] PROGMEM = "XTAL|PLL_D2|PLL_F160M|external";  // I2S_TX/RX_CLKM_CONF_REG.clk_sel (see i2s_ll_tx_clk_set_src)
#define HwdI2sDev(n)    ((0 == (n)) ? &I2S0 : &I2S1)        // macro: no custom types in .ino function signatures
static inline bool HwdI2sOn(uint32_t n) { return periph_ll_periph_enabled(n ? PERIPH_I2S1_MODULE : PERIPH_I2S0_MODULE); }
static inline uint32_t HwdI2sClkSoc(uint32_t sel) {
  return (0 == sel) ? SOC_MOD_CLK_XTAL : (1 == sel) ? SOC_MOD_CLK_PLL_D2 : (2 == sel) ? SOC_MOD_CLK_PLL_F160M : 0;
}
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
// mclk out, mclk in, tx bck out/in, tx ws out/in, dout, dout1, rx bck out/in, rx ws out/in, din (0xFFFF = no such signal)
static const uint16_t kHwdI2sSig[SOC_I2S_NUM][13] = {
  { I2S0_MCLK_OUT_IDX, I2S0_MCLK_IN_IDX, I2S0O_BCK_OUT_IDX, I2S0O_BCK_IN_IDX, I2S0O_WS_OUT_IDX, I2S0O_WS_IN_IDX,
    I2S0O_SD_OUT_IDX, I2S0O_SD1_OUT_IDX, I2S0I_BCK_OUT_IDX, I2S0I_BCK_IN_IDX, I2S0I_WS_OUT_IDX, I2S0I_WS_IN_IDX, I2S0I_SD_IN_IDX },
  { I2S1_MCLK_OUT_IDX, I2S1_MCLK_IN_IDX, I2S1O_BCK_OUT_IDX, I2S1O_BCK_IN_IDX, I2S1O_WS_OUT_IDX, I2S1O_WS_IN_IDX,
    I2S1O_SD_OUT_IDX, 0xFFFF, I2S1I_BCK_OUT_IDX, I2S1I_BCK_IN_IDX, I2S1I_WS_OUT_IDX, I2S1I_WS_IN_IDX, I2S1I_SD_IN_IDX },
};

// SPI2 (GPSPI2/FSPI) and SPI3 (GPSPI3), SPI0/1 are used by flash/PSRAM and not dumped
// Warning: never read GPSPIn.data_buf[] while a transaction is ongoing (not dumped anyway)
#define HWDUMP_SPI
#include "soc/spi_struct.h"
const char kHwdSpiClkNames[] PROGMEM = "XTAL|APB";                        // SPI_CLK_GATE_REG.mst_clk_sel (see spi_ll_set_clk_source)
#define HWD_SPI_NUM     2
static const char * const kHwdSpiNames[HWD_SPI_NUM] = { "SPI2", "SPI3" };
#define HwdSpiDev(i)    ((0 == (i)) ? &GPSPI2 : &GPSPI3)    // macro: no custom types in .ino function signatures
#define HWD_SPI_CS_NUM  6
static inline bool HwdSpiOn(uint32_t i) { return periph_ll_periph_enabled(i ? PERIPH_SPI3_MODULE : PERIPH_SPI2_MODULE); }
static inline uint32_t HwdSpiClkSel(uint32_t i) { return HwdSpiDev(i)->clk_gate.mst_clk_sel; }
static inline uint32_t HwdSpiClkSoc(uint32_t sel) { return sel ? SOC_MOD_CLK_APB : SOC_MOD_CLK_XTAL; }
static inline uint32_t HwdSpiClkPreDiv(uint32_t i) { return 1; }                   // no module clock pre-divider
// GPIO matrix signals: sck, d (mosi), q (miso), hd, wp, cs0..cs5 - input and output signals share the same index
// 0xFFFF = no such signal (SPI3 has only 3 CS)
static const uint16_t kHwdSpiSig[HWD_SPI_NUM][5 + HWD_SPI_CS_NUM] = {
  { FSPICLK_OUT_IDX, FSPID_OUT_IDX, FSPIQ_IN_IDX, FSPIHD_OUT_IDX, FSPIWP_OUT_IDX,
    FSPICS0_OUT_IDX, FSPICS1_OUT_IDX, FSPICS2_OUT_IDX, FSPICS3_OUT_IDX, FSPICS4_OUT_IDX, FSPICS5_OUT_IDX },
  { SPI3_CLK_OUT_IDX, SPI3_D_OUT_IDX, SPI3_Q_IN_IDX, SPI3_HD_OUT_IDX, SPI3_WP_OUT_IDX,
    SPI3_CS0_OUT_IDX, SPI3_CS1_OUT_IDX, SPI3_CS2_OUT_IDX, 0xFFFF, 0xFFFF, 0xFFFF },
};
// IO_MUX function names: sck, d, q, hd, wp, cs0 (SPI3 has no IO_MUX pins)
static const char * const kHwdSpiIomux[HWD_SPI_NUM][6] = {
  { "FSPICLK", "FSPID", "FSPIQ", "FSPIHD", "FSPIWP", "FSPICS0" },
  { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr },
};

// Sleep: RTC_CNTL registers
#define HWDUMP_SLEEP
#define HWDUMP_SLEEP_EXT                                    // ext0 (RTC_IO) and ext1 (RTC_CNTL) wakeup
#include "soc/rtc.h"
#include "esp_sleep.h"
#include "esp_pm.h"
// RTC_CNTL_WAKEUP_ENA bits, from esp_hw_support/port/esp32s3/include/soc/rtc.h (RTC_xxx_TRIG_EN), index = bit number
const char kHwdWakeupNames[] PROGMEM = "ext0|ext1|gpio|timer|sdio|wifi|uart0|uart1|touch|ulp|bt|cocpu|xtal32k_dead|cocpu_trap|usb";
#define HWD_WAKEUP_BITS   17
static_assert(RTC_EXT0_TRIG_EN == BIT(0) && RTC_EXT1_TRIG_EN == BIT(1) && RTC_TIMER_TRIG_EN == BIT(3) &&
              RTC_TOUCH_TRIG_EN == BIT(8) && RTC_COCPU_TRIG_EN == BIT(11) && RTC_USB_TRIG_EN == BIT(14),
              "HwDump: unexpected RTC wakeup bits");
static inline uint32_t HwdWakeupEna(void) { return REG_GET_FIELD(RTC_CNTL_WAKEUP_STATE_REG, RTC_CNTL_WAKEUP_ENA); }
static inline bool HwdPadHold(uint32_t pin) {
  int r = HwdRtcNum(pin);
  if (r >= 0) { return REG_READ(RTC_CNTL_PAD_HOLD_REG) & rtc_io_desc[r].hold_force; }     // RTC pad, same as rtcio_ll_force_hold_enable()
  return gpio_ll_is_digital_io_hold(&GPIO, pin);           // GPIO26+: RTC_CNTL_DIG_PAD_HOLD_REG
}
#define HWDUMP_SLEEP_DG_PAD_HOLD                            // RTC_CNTL_DIG_ISO_REG pad hold bits
static inline bool HwdDgPadAutohold(void) { return REG_READ(RTC_CNTL_DIG_ISO_REG) & RTC_CNTL_DG_PAD_AUTOHOLD; }
static inline bool HwdDgPadAutoholdEn(void) { return REG_READ(RTC_CNTL_DIG_ISO_REG) & RTC_CNTL_DG_PAD_AUTOHOLD_EN; }
static inline bool HwdDgPadForceHold(void) { return REG_READ(RTC_CNTL_DIG_ISO_REG) & RTC_CNTL_DG_PAD_FORCE_HOLD; }
static inline uint32_t HwdGpioIntType(uint32_t pin) { return GPIO.pin[pin].int_type; }
static inline bool HwdGpioWakeupEn(uint32_t pin) { return GPIO.pin[pin].wakeup_enable; }
// ext0: one RTC pad, level; ext1: mask of RTC pads, any high or all low (programmed by IDF when entering sleep)
#define HWD_WAKEUP_EXT0_EN  RTC_EXT0_TRIG_EN
#define HWD_WAKEUP_EXT1_EN  RTC_EXT1_TRIG_EN
static inline int HwdRtcGpio(uint32_t r) { return rtc_io_desc[r].rtc_num; }        // RTC_IO index -> GPIO number
static inline uint32_t HwdExt0Rtc(void) { return REG_GET_FIELD(RTC_IO_EXT_WAKEUP0_REG, RTC_IO_EXT_WAKEUP0_SEL); }
static inline bool HwdExt0High(void) { return REG_READ(RTC_CNTL_EXT_WAKEUP_CONF_REG) & RTC_CNTL_EXT_WAKEUP0_LV; }
static inline uint32_t HwdExt1Mask(void) { return REG_GET_FIELD(RTC_CNTL_EXT_WAKEUP1_REG, RTC_CNTL_EXT_WAKEUP1_SEL); }
static inline bool HwdExt1AnyHigh(void) { return REG_READ(RTC_CNTL_EXT_WAKEUP_CONF_REG) & RTC_CNTL_EXT_WAKEUP1_LV; }
static inline uint32_t HwdExt1Status(void) { return REG_GET_FIELD(RTC_CNTL_EXT_WAKEUP1_STATUS_REG, RTC_CNTL_EXT_WAKEUP1_STATUS); }

#endif  // CONFIG_IDF_TARGET_ESP32S3
