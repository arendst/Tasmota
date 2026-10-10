/*
  xdrv_126_hwdump_esp32.h - HwDump per-target data and register accessors for ESP32

  SPDX-FileCopyrightText: 2026 Stephan Hadinger

  SPDX-License-Identifier: GPL-3.0-only
*/

// Only included from tasmota_xdrv_driver/xdrv_126_hwdump.ino, after HwdPad_t, HwdSig_t and HWD_SIG() are defined.
// Defines data (not only declarations), so it must be included exactly once.
#pragma once

#if CONFIG_IDF_TARGET_ESP32

/*********************************************************************************************\
 * ESP32
 *
 * Bus clock and reset bits are in DPORT_PERIP_CLK_EN_REG / DPORT_PERIP_RST_EN_REG, read through
 * periph_ll_periph_enabled() which uses DPORT_REG_READ (ESP32 DPORT read workaround)
\*********************************************************************************************/
#define HWDUMP_SUPPORTED
#include "hal/clk_gate_ll.h"          // periph_ll_periph_enabled()
#include "soc/rtc_io_periph.h"        // rtc_io_desc[], rtc_io_num_map[]

// Pad names and IO_MUX functions from soc/io_mux_reg.h (FUNC_<pad>_<function> = MCU_SEL value, empty = no function)
// GPIO28-31 do not exist, GPIO20 and GPIO24 are not bonded on most packages (filtered by GPIO_IS_VALID_GPIO)
const HwdPad_t kHwdPads[] = {
  { "GPIO0",      "GPIO0_0|CLK_OUT1|GPIO0|||EMAC_TX_CLK" },               //  0
  { "U0TXD",      "U0TXD|CLK_OUT3|GPIO1|||EMAC_RXD2" },                   //  1
  { "GPIO2",      "GPIO2_0|HSPIWP|GPIO2|HS2_DATA0|SD_DATA0" },            //  2
  { "U0RXD",      "U0RXD|CLK_OUT2|GPIO3" },                               //  3
  { "GPIO4",      "GPIO4_0|HSPIHD|GPIO4|HS2_DATA1|SD_DATA1|EMAC_TX_ER" }, //  4
  { "GPIO5",      "GPIO5_0|VSPICS0|GPIO5|HS1_DATA6||EMAC_RX_CLK" },       //  5
  { "SD_CLK",     "SD_CLK|SPICLK|GPIO6|HS1_CLK|U1CTS" },                  //  6
  { "SD_DATA0",   "SD_DATA0|SPIQ|GPIO7|HS1_DATA0|U2RTS" },                //  7
  { "SD_DATA1",   "SD_DATA1|SPID|GPIO8|HS1_DATA1|U2CTS" },                //  8
  { "SD_DATA2",   "SD_DATA2|SPIHD|GPIO9|HS1_DATA2|U1RXD" },               //  9
  { "SD_DATA3",   "SD_DATA3|SPIWP|GPIO10|HS1_DATA3|U1TXD" },              // 10
  { "SD_CMD",     "SD_CMD|SPICS0|GPIO11|HS1_CMD|U1RTS" },                 // 11
  { "MTDI",       "MTDI|HSPIQ|GPIO12|HS2_DATA2|SD_DATA2|EMAC_TXD3" },     // 12
  { "MTCK",       "MTCK|HSPID|GPIO13|HS2_DATA3|SD_DATA3|EMAC_RX_ER" },    // 13
  { "MTMS",       "MTMS|HSPICLK|GPIO14|HS2_CLK|SD_CLK|EMAC_TXD2" },       // 14
  { "MTDO",       "MTDO|HSPICS0|GPIO15|HS2_CMD|SD_CMD|EMAC_RXD3" },       // 15
  { "GPIO16",     "GPIO16_0||GPIO16|HS1_DATA4|U2RXD|EMAC_CLK_OUT" },      // 16
  { "GPIO17",     "GPIO17_0||GPIO17|HS1_DATA5|U2TXD|EMAC_CLK_180" },      // 17  EMAC_CLK_OUT_180 shortened to fit 15 chars
  { "GPIO18",     "GPIO18_0|VSPICLK|GPIO18|HS1_DATA7" },                  // 18
  { "GPIO19",     "GPIO19_0|VSPIQ|GPIO19|U0CTS||EMAC_TXD0" },             // 19
  { "GPIO20",     "GPIO20_0||GPIO20" },                                   // 20
  { "GPIO21",     "GPIO21_0|VSPIHD|GPIO21|||EMAC_TX_EN" },                // 21
  { "GPIO22",     "GPIO22_0|VSPIWP|GPIO22|U0RTS||EMAC_TXD1" },            // 22
  { "GPIO23",     "GPIO23_0|VSPID|GPIO23|HS1_STROBE" },                   // 23
  { "GPIO24",     "GPIO24_0||GPIO24" },                                   // 24
  { "GPIO25",     "GPIO25_0||GPIO25|||EMAC_RXD0" },                       // 25
  { "GPIO26",     "GPIO26_0||GPIO26|||EMAC_RXD1" },                       // 26
  { "GPIO27",     "GPIO27_0||GPIO27|||EMAC_RX_DV" },                      // 27
  { "-",          "" },                                                   // 28
  { "-",          "" },                                                   // 29
  { "-",          "" },                                                   // 30
  { "-",          "" },                                                   // 31
  { "32K_XP",     "GPIO32_0||GPIO32" },                                   // 32
  { "32K_XN",     "GPIO33_0||GPIO33" },                                   // 33
  { "VDET_1",     "GPIO34_0||GPIO34" },                                   // 34
  { "VDET_2",     "GPIO35_0||GPIO35" },                                   // 35
  { "SENSOR_VP",  "GPIO36_0||GPIO36" },                                   // 36
  { "SENSOR_CAPP","GPIO37_0||GPIO37" },                                   // 37
  { "SENSOR_CAPN","GPIO38_0||GPIO38" },                                   // 38
  { "SENSOR_VN",  "GPIO39_0||GPIO39" },                                   // 39
};

// Sanity checks of the function indexes above against ESP-IDF definitions
static_assert(PIN_FUNC_GPIO == 2, "HwDump: unexpected PIN_FUNC_GPIO");
static_assert(FUNC_GPIO0_CLK_OUT1 == 1 && FUNC_GPIO0_EMAC_TX_CLK == 5, "HwDump: IO_MUX pad 0");
static_assert(FUNC_U0TXD_U0TXD == 0 && FUNC_U0TXD_CLK_OUT3 == 1 && FUNC_U0TXD_EMAC_RXD2 == 5, "HwDump: IO_MUX pad 1");
static_assert(FUNC_GPIO2_HSPIWP == 1 && FUNC_GPIO2_HS2_DATA0 == 3 && FUNC_GPIO2_SD_DATA0 == 4, "HwDump: IO_MUX pad 2");
static_assert(FUNC_U0RXD_U0RXD == 0 && FUNC_U0RXD_CLK_OUT2 == 1, "HwDump: IO_MUX pad 3");
static_assert(FUNC_GPIO4_HSPIHD == 1 && FUNC_GPIO4_SD_DATA1 == 4 && FUNC_GPIO4_EMAC_TX_ER == 5, "HwDump: IO_MUX pad 4");
static_assert(FUNC_GPIO5_VSPICS0 == 1 && FUNC_GPIO5_HS1_DATA6 == 3 && FUNC_GPIO5_EMAC_RX_CLK == 5, "HwDump: IO_MUX pad 5");
static_assert(FUNC_SD_CLK_SPICLK == 1 && FUNC_SD_CLK_HS1_CLK == 3 && FUNC_SD_CLK_U1CTS == 4, "HwDump: IO_MUX pad 6");
static_assert(FUNC_SD_DATA0_U2RTS == 4 && FUNC_SD_DATA1_U2CTS == 4 && FUNC_SD_DATA2_U1RXD == 4 &&
              FUNC_SD_DATA3_U1TXD == 4 && FUNC_SD_CMD_U1RTS == 4, "HwDump: IO_MUX pads 7-11");
static_assert(FUNC_MTDI_HSPIQ == 1 && FUNC_MTCK_HSPID == 1 && FUNC_MTMS_HSPICLK == 1 && FUNC_MTDO_HSPICS0 == 1, "HwDump: IO_MUX pads 12-15");
static_assert(FUNC_MTDI_SD_DATA2 == 4 && FUNC_MTDI_EMAC_TXD3 == 5 && FUNC_MTDO_EMAC_RXD3 == 5, "HwDump: IO_MUX pads 12-15");
static_assert(FUNC_GPIO16_U2RXD == 4 && FUNC_GPIO16_EMAC_CLK_OUT == 5 && FUNC_GPIO17_U2TXD == 4 &&
              FUNC_GPIO17_EMAC_CLK_OUT_180 == 5, "HwDump: IO_MUX pads 16-17");
static_assert(FUNC_GPIO18_VSPICLK == 1 && FUNC_GPIO18_HS1_DATA7 == 3 && FUNC_GPIO19_VSPIQ == 1 &&
              FUNC_GPIO19_U0CTS == 3 && FUNC_GPIO19_EMAC_TXD0 == 5, "HwDump: IO_MUX pads 18-19");
static_assert(FUNC_GPIO21_VSPIHD == 1 && FUNC_GPIO21_EMAC_TX_EN == 5 && FUNC_GPIO22_VSPIWP == 1 && FUNC_GPIO22_U0RTS == 3 &&
              FUNC_GPIO23_VSPID == 1 && FUNC_GPIO23_HS1_STROBE == 3, "HwDump: IO_MUX pads 21-23");
static_assert(FUNC_GPIO25_EMAC_RXD0 == 5 && FUNC_GPIO26_EMAC_RXD1 == 5 && FUNC_GPIO27_EMAC_RX_DV == 5, "HwDump: IO_MUX pads 25-27");

// GPIO matrix output signals (GPIO_FUNCn_OUT_SEL), from soc/gpio_sig_map.h
const HwdSig_t kHwdSigOut[] = {
  HWD_SIG(SPICLK_OUT), HWD_SIG(SPIQ_OUT), HWD_SIG(SPID_OUT), HWD_SIG(SPIHD_OUT), HWD_SIG(SPIWP_OUT),
  HWD_SIG(SPICS0_OUT), HWD_SIG(SPICS1_OUT), HWD_SIG(SPICS2_OUT),
  HWD_SIG(HSPICLK_OUT), HWD_SIG(HSPIQ_OUT), HWD_SIG(HSPID_OUT), HWD_SIG(HSPICS0_OUT), HWD_SIG(HSPIHD_OUT), HWD_SIG(HSPIWP_OUT),
  HWD_SIG(U0TXD_OUT), HWD_SIG(U0RTS_OUT), HWD_SIG(U0DTR_OUT), HWD_SIG(U1TXD_OUT), HWD_SIG(U1RTS_OUT),
  HWD_SIG(I2CM_SCL_O), HWD_SIG(I2CM_SDA_O), HWD_SIG(EXT_I2C_SCL_O), HWD_SIG(EXT_I2C_SDA_O),
  HWD_SIG(I2S0O_BCK_OUT), HWD_SIG(I2S1O_BCK_OUT), HWD_SIG(I2S0O_WS_OUT), HWD_SIG(I2S1O_WS_OUT),
  HWD_SIG(I2S0I_BCK_OUT), HWD_SIG(I2S0I_WS_OUT),
  HWD_SIG(I2CEXT0_SCL_OUT), HWD_SIG(I2CEXT0_SDA_OUT),
  HWD_SIG(SDIO_TOHOST_INT_OUT),
  HWD_SIG(PWM0_OUT0A), HWD_SIG(PWM0_OUT0B), HWD_SIG(PWM0_OUT1A), HWD_SIG(PWM0_OUT1B), HWD_SIG(PWM0_OUT2A), HWD_SIG(PWM0_OUT2B),
  HWD_SIG(GPIO_WLAN_ACTIVE),
  HWD_SIG(BB_DIAG0), HWD_SIG(BB_DIAG1), HWD_SIG(BB_DIAG2), HWD_SIG(BB_DIAG3), HWD_SIG(BB_DIAG4),
  HWD_SIG(BB_DIAG5), HWD_SIG(BB_DIAG6), HWD_SIG(BB_DIAG7), HWD_SIG(BB_DIAG8), HWD_SIG(BB_DIAG9),
  HWD_SIG(BB_DIAG10), HWD_SIG(BB_DIAG11), HWD_SIG(BB_DIAG12), HWD_SIG(BB_DIAG13), HWD_SIG(BB_DIAG14),
  HWD_SIG(BB_DIAG15), HWD_SIG(BB_DIAG16), HWD_SIG(BB_DIAG17), HWD_SIG(BB_DIAG18), HWD_SIG(BB_DIAG19),
  HWD_SIG(HSPICS1_OUT), HWD_SIG(HSPICS2_OUT),
  HWD_SIG(VSPICLK_OUT), HWD_SIG(VSPIQ_OUT), HWD_SIG(VSPID_OUT), HWD_SIG(VSPIHD_OUT), HWD_SIG(VSPIWP_OUT),
  HWD_SIG(VSPICS0_OUT), HWD_SIG(VSPICS1_OUT), HWD_SIG(VSPICS2_OUT),
  HWD_SIG(LEDC_HS_SIG_OUT0), HWD_SIG(LEDC_HS_SIG_OUT1), HWD_SIG(LEDC_HS_SIG_OUT2), HWD_SIG(LEDC_HS_SIG_OUT3),
  HWD_SIG(LEDC_HS_SIG_OUT4), HWD_SIG(LEDC_HS_SIG_OUT5), HWD_SIG(LEDC_HS_SIG_OUT6), HWD_SIG(LEDC_HS_SIG_OUT7),
  HWD_SIG(LEDC_LS_SIG_OUT0), HWD_SIG(LEDC_LS_SIG_OUT1), HWD_SIG(LEDC_LS_SIG_OUT2), HWD_SIG(LEDC_LS_SIG_OUT3),
  HWD_SIG(LEDC_LS_SIG_OUT4), HWD_SIG(LEDC_LS_SIG_OUT5), HWD_SIG(LEDC_LS_SIG_OUT6), HWD_SIG(LEDC_LS_SIG_OUT7),
  HWD_SIG(RMT_SIG_OUT0), HWD_SIG(RMT_SIG_OUT1), HWD_SIG(RMT_SIG_OUT2), HWD_SIG(RMT_SIG_OUT3),
  HWD_SIG(RMT_SIG_OUT4), HWD_SIG(RMT_SIG_OUT5), HWD_SIG(RMT_SIG_OUT6), HWD_SIG(RMT_SIG_OUT7),
  HWD_SIG(I2CEXT1_SCL_OUT), HWD_SIG(I2CEXT1_SDA_OUT),
  HWD_SIG(HOST_CCMD_OD_PULLUP_EN_N), HWD_SIG(HOST_RST_N_1), HWD_SIG(HOST_RST_N_2),
  HWD_SIG(GPIO_SD0_OUT), HWD_SIG(GPIO_SD1_OUT), HWD_SIG(GPIO_SD2_OUT), HWD_SIG(GPIO_SD3_OUT),
  HWD_SIG(GPIO_SD4_OUT), HWD_SIG(GPIO_SD5_OUT), HWD_SIG(GPIO_SD6_OUT), HWD_SIG(GPIO_SD7_OUT),
  HWD_SIG(PWM1_OUT0A), HWD_SIG(PWM1_OUT0B), HWD_SIG(PWM1_OUT1A), HWD_SIG(PWM1_OUT1B), HWD_SIG(PWM1_OUT2A), HWD_SIG(PWM1_OUT2B),
  HWD_SIG(TWAI_TX), HWD_SIG(TWAI_BUS_OFF_ON), HWD_SIG(TWAI_CLKOUT),
  HWD_SIG(SPID4_OUT), HWD_SIG(SPID5_OUT), HWD_SIG(SPID6_OUT), HWD_SIG(SPID7_OUT),
  HWD_SIG(HSPID4_OUT), HWD_SIG(HSPID5_OUT), HWD_SIG(HSPID6_OUT), HWD_SIG(HSPID7_OUT),
  HWD_SIG(VSPID4_OUT), HWD_SIG(VSPID5_OUT), HWD_SIG(VSPID6_OUT), HWD_SIG(VSPID7_OUT),
  HWD_SIG(I2S0O_DATA_OUT0), HWD_SIG(I2S0O_DATA_OUT1), HWD_SIG(I2S0O_DATA_OUT2), HWD_SIG(I2S0O_DATA_OUT3),
  HWD_SIG(I2S0O_DATA_OUT4), HWD_SIG(I2S0O_DATA_OUT5), HWD_SIG(I2S0O_DATA_OUT6), HWD_SIG(I2S0O_DATA_OUT7),
  HWD_SIG(I2S0O_DATA_OUT8), HWD_SIG(I2S0O_DATA_OUT9), HWD_SIG(I2S0O_DATA_OUT10), HWD_SIG(I2S0O_DATA_OUT11),
  HWD_SIG(I2S0O_DATA_OUT12), HWD_SIG(I2S0O_DATA_OUT13), HWD_SIG(I2S0O_DATA_OUT14), HWD_SIG(I2S0O_DATA_OUT15),
  HWD_SIG(I2S0O_DATA_OUT16), HWD_SIG(I2S0O_DATA_OUT17), HWD_SIG(I2S0O_DATA_OUT18), HWD_SIG(I2S0O_DATA_OUT19),
  HWD_SIG(I2S0O_DATA_OUT20), HWD_SIG(I2S0O_DATA_OUT21), HWD_SIG(I2S0O_DATA_OUT22), HWD_SIG(I2S0O_DATA_OUT23),
  HWD_SIG(I2S1I_BCK_OUT), HWD_SIG(I2S1I_WS_OUT),
  HWD_SIG(I2S1O_DATA_OUT0), HWD_SIG(I2S1O_DATA_OUT1), HWD_SIG(I2S1O_DATA_OUT2), HWD_SIG(I2S1O_DATA_OUT3),
  HWD_SIG(I2S1O_DATA_OUT4), HWD_SIG(I2S1O_DATA_OUT5), HWD_SIG(I2S1O_DATA_OUT6), HWD_SIG(I2S1O_DATA_OUT7),
  HWD_SIG(I2S1O_DATA_OUT8), HWD_SIG(I2S1O_DATA_OUT9), HWD_SIG(I2S1O_DATA_OUT10), HWD_SIG(I2S1O_DATA_OUT11),
  HWD_SIG(I2S1O_DATA_OUT12), HWD_SIG(I2S1O_DATA_OUT13), HWD_SIG(I2S1O_DATA_OUT14), HWD_SIG(I2S1O_DATA_OUT15),
  HWD_SIG(I2S1O_DATA_OUT16), HWD_SIG(I2S1O_DATA_OUT17), HWD_SIG(I2S1O_DATA_OUT18), HWD_SIG(I2S1O_DATA_OUT19),
  HWD_SIG(I2S1O_DATA_OUT20), HWD_SIG(I2S1O_DATA_OUT21), HWD_SIG(I2S1O_DATA_OUT22), HWD_SIG(I2S1O_DATA_OUT23),
  HWD_SIG(U2TXD_OUT), HWD_SIG(U2RTS_OUT),
  HWD_SIG(EMAC_MDC_O), HWD_SIG(EMAC_MDO_O), HWD_SIG(EMAC_CRS_O), HWD_SIG(EMAC_COL_O),
  HWD_SIG(BT_AUDIO0_IRQ), HWD_SIG(BT_AUDIO1_IRQ), HWD_SIG(BT_AUDIO2_IRQ),
  HWD_SIG(BLE_AUDIO0_IRQ), HWD_SIG(BLE_AUDIO1_IRQ), HWD_SIG(BLE_AUDIO2_IRQ),
  HWD_SIG(PCMFSYNC_OUT), HWD_SIG(PCMCLK_OUT), HWD_SIG(PCMDOUT),
  HWD_SIG(BLE_AUDIO_SYNC0_P), HWD_SIG(BLE_AUDIO_SYNC1_P), HWD_SIG(BLE_AUDIO_SYNC2_P),
  HWD_SIG(ANT_SEL0), HWD_SIG(ANT_SEL1), HWD_SIG(ANT_SEL2), HWD_SIG(ANT_SEL3),
  HWD_SIG(ANT_SEL4), HWD_SIG(ANT_SEL5), HWD_SIG(ANT_SEL6), HWD_SIG(ANT_SEL7),
  HWD_SIG(SIG_IN_FUNC224), HWD_SIG(SIG_IN_FUNC225), HWD_SIG(SIG_IN_FUNC226), HWD_SIG(SIG_IN_FUNC227), HWD_SIG(SIG_IN_FUNC228),
  { SIG_GPIO_OUT_IDX, "GPIO" },                   // simple GPIO output (GPIO_OUT register)
};

// GPIO matrix input signals (GPIO_FUNCm_IN_SEL_CFG), from soc/gpio_sig_map.h
const HwdSig_t kHwdSigIn[] = {
  HWD_SIG(SPICLK_IN), HWD_SIG(SPIQ_IN), HWD_SIG(SPID_IN), HWD_SIG(SPIHD_IN), HWD_SIG(SPIWP_IN),
  HWD_SIG(SPICS0_IN), HWD_SIG(SPICS1_IN), HWD_SIG(SPICS2_IN),
  HWD_SIG(HSPICLK_IN), HWD_SIG(HSPIQ_IN), HWD_SIG(HSPID_IN), HWD_SIG(HSPICS0_IN), HWD_SIG(HSPIHD_IN), HWD_SIG(HSPIWP_IN),
  HWD_SIG(U0RXD_IN), HWD_SIG(U0CTS_IN), HWD_SIG(U0DSR_IN), HWD_SIG(U1RXD_IN), HWD_SIG(U1CTS_IN),
  HWD_SIG(I2CM_SDA_I), HWD_SIG(EXT_I2C_SDA_I),
  HWD_SIG(I2S0O_BCK_IN), HWD_SIG(I2S1O_BCK_IN), HWD_SIG(I2S0O_WS_IN), HWD_SIG(I2S1O_WS_IN),
  HWD_SIG(I2S0I_BCK_IN), HWD_SIG(I2S0I_WS_IN),
  HWD_SIG(I2CEXT0_SCL_IN), HWD_SIG(I2CEXT0_SDA_IN),
  HWD_SIG(PWM0_SYNC0_IN), HWD_SIG(PWM0_SYNC1_IN), HWD_SIG(PWM0_SYNC2_IN),
  HWD_SIG(PWM0_F0_IN), HWD_SIG(PWM0_F1_IN), HWD_SIG(PWM0_F2_IN),
  HWD_SIG(GPIO_BT_ACTIVE), HWD_SIG(GPIO_BT_PRIORITY),
  HWD_SIG(PCNT_SIG_CH0_IN0), HWD_SIG(PCNT_SIG_CH1_IN0), HWD_SIG(PCNT_CTRL_CH0_IN0), HWD_SIG(PCNT_CTRL_CH1_IN0),
  HWD_SIG(PCNT_SIG_CH0_IN1), HWD_SIG(PCNT_SIG_CH1_IN1), HWD_SIG(PCNT_CTRL_CH0_IN1), HWD_SIG(PCNT_CTRL_CH1_IN1),
  HWD_SIG(PCNT_SIG_CH0_IN2), HWD_SIG(PCNT_SIG_CH1_IN2), HWD_SIG(PCNT_CTRL_CH0_IN2), HWD_SIG(PCNT_CTRL_CH1_IN2),
  HWD_SIG(PCNT_SIG_CH0_IN3), HWD_SIG(PCNT_SIG_CH1_IN3), HWD_SIG(PCNT_CTRL_CH0_IN3), HWD_SIG(PCNT_CTRL_CH1_IN3),
  HWD_SIG(PCNT_SIG_CH0_IN4), HWD_SIG(PCNT_SIG_CH1_IN4), HWD_SIG(PCNT_CTRL_CH0_IN4), HWD_SIG(PCNT_CTRL_CH1_IN4),
  HWD_SIG(HSPICS1_IN), HWD_SIG(HSPICS2_IN),
  HWD_SIG(VSPICLK_IN), HWD_SIG(VSPIQ_IN), HWD_SIG(VSPID_IN), HWD_SIG(VSPIHD_IN), HWD_SIG(VSPIWP_IN),
  HWD_SIG(VSPICS0_IN), HWD_SIG(VSPICS1_IN), HWD_SIG(VSPICS2_IN),
  HWD_SIG(PCNT_SIG_CH0_IN5), HWD_SIG(PCNT_SIG_CH1_IN5), HWD_SIG(PCNT_CTRL_CH0_IN5), HWD_SIG(PCNT_CTRL_CH1_IN5),
  HWD_SIG(PCNT_SIG_CH0_IN6), HWD_SIG(PCNT_SIG_CH1_IN6), HWD_SIG(PCNT_CTRL_CH0_IN6), HWD_SIG(PCNT_CTRL_CH1_IN6),
  HWD_SIG(PCNT_SIG_CH0_IN7), HWD_SIG(PCNT_SIG_CH1_IN7), HWD_SIG(PCNT_CTRL_CH0_IN7), HWD_SIG(PCNT_CTRL_CH1_IN7),
  HWD_SIG(RMT_SIG_IN0), HWD_SIG(RMT_SIG_IN1), HWD_SIG(RMT_SIG_IN2), HWD_SIG(RMT_SIG_IN3),
  HWD_SIG(RMT_SIG_IN4), HWD_SIG(RMT_SIG_IN5), HWD_SIG(RMT_SIG_IN6), HWD_SIG(RMT_SIG_IN7),
  HWD_SIG(EXT_ADC_START), HWD_SIG(TWAI_RX),
  HWD_SIG(I2CEXT1_SCL_IN), HWD_SIG(I2CEXT1_SDA_IN),
  HWD_SIG(HOST_CARD_DETECT_N_1), HWD_SIG(HOST_CARD_DETECT_N_2), HWD_SIG(HOST_CARD_WRITE_PRT_1), HWD_SIG(HOST_CARD_WRITE_PRT_2),
  HWD_SIG(HOST_CARD_INT_N_1), HWD_SIG(HOST_CARD_INT_N_2),
  HWD_SIG(PWM1_SYNC0_IN), HWD_SIG(PWM1_SYNC1_IN), HWD_SIG(PWM1_SYNC2_IN),
  HWD_SIG(PWM1_F0_IN), HWD_SIG(PWM1_F1_IN), HWD_SIG(PWM1_F2_IN),
  HWD_SIG(PWM0_CAP0_IN), HWD_SIG(PWM0_CAP1_IN), HWD_SIG(PWM0_CAP2_IN),
  HWD_SIG(PWM1_CAP0_IN), HWD_SIG(PWM1_CAP1_IN), HWD_SIG(PWM1_CAP2_IN),
  HWD_SIG(SPID4_IN), HWD_SIG(SPID5_IN), HWD_SIG(SPID6_IN), HWD_SIG(SPID7_IN),
  HWD_SIG(HSPID4_IN), HWD_SIG(HSPID5_IN), HWD_SIG(HSPID6_IN), HWD_SIG(HSPID7_IN),
  HWD_SIG(VSPID4_IN), HWD_SIG(VSPID5_IN), HWD_SIG(VSPID6_IN), HWD_SIG(VSPID7_IN),
  HWD_SIG(I2S0I_DATA_IN0), HWD_SIG(I2S0I_DATA_IN1), HWD_SIG(I2S0I_DATA_IN2), HWD_SIG(I2S0I_DATA_IN3),
  HWD_SIG(I2S0I_DATA_IN4), HWD_SIG(I2S0I_DATA_IN5), HWD_SIG(I2S0I_DATA_IN6), HWD_SIG(I2S0I_DATA_IN7),
  HWD_SIG(I2S0I_DATA_IN8), HWD_SIG(I2S0I_DATA_IN9), HWD_SIG(I2S0I_DATA_IN10), HWD_SIG(I2S0I_DATA_IN11),
  HWD_SIG(I2S0I_DATA_IN12), HWD_SIG(I2S0I_DATA_IN13), HWD_SIG(I2S0I_DATA_IN14), HWD_SIG(I2S0I_DATA_IN15),
  HWD_SIG(I2S1I_BCK_IN), HWD_SIG(I2S1I_WS_IN),
  HWD_SIG(I2S1I_DATA_IN0), HWD_SIG(I2S1I_DATA_IN1), HWD_SIG(I2S1I_DATA_IN2), HWD_SIG(I2S1I_DATA_IN3),
  HWD_SIG(I2S1I_DATA_IN4), HWD_SIG(I2S1I_DATA_IN5), HWD_SIG(I2S1I_DATA_IN6), HWD_SIG(I2S1I_DATA_IN7),
  HWD_SIG(I2S1I_DATA_IN8), HWD_SIG(I2S1I_DATA_IN9), HWD_SIG(I2S1I_DATA_IN10), HWD_SIG(I2S1I_DATA_IN11),
  HWD_SIG(I2S1I_DATA_IN12), HWD_SIG(I2S1I_DATA_IN13), HWD_SIG(I2S1I_DATA_IN14), HWD_SIG(I2S1I_DATA_IN15),
  HWD_SIG(I2S0I_H_SYNC), HWD_SIG(I2S0I_V_SYNC), HWD_SIG(I2S0I_H_ENABLE),
  HWD_SIG(I2S1I_H_SYNC), HWD_SIG(I2S1I_V_SYNC), HWD_SIG(I2S1I_H_ENABLE),
  HWD_SIG(U2RXD_IN), HWD_SIG(U2CTS_IN),
  HWD_SIG(EMAC_MDC_I), HWD_SIG(EMAC_MDI_I), HWD_SIG(EMAC_CRS_I), HWD_SIG(EMAC_COL_I),
  HWD_SIG(PCMFSYNC_IN), HWD_SIG(PCMCLK_IN), HWD_SIG(PCMDIN),
};

// Target specific register fields (names differ between SoCs)
static inline uint32_t HwdOutLevel(uint32_t pin) { return (pin < 32) ? ((GPIO.out >> pin) & 1) : ((GPIO.out1.val >> (pin - 32)) & 1); }
static inline bool HwdOutInv(uint32_t pin) { return GPIO.func_out_sel_cfg[pin].inv_sel; }
static inline bool HwdInInv(uint32_t sig) { return GPIO.func_in_sel_cfg[sig].sig_in_inv; }

// RTC_IO: 18 pads can be switched to the RTC_IO controller (rtc_io_desc[].mux), then IO_MUX/GPIO matrix settings do not apply
#define HWDUMP_RTCIO
#include "soc/rtc_io_struct.h"
#include "soc/rtc_io_reg.h"
#include "soc/rtc_cntl_reg.h"
#define HWD_RTCIO_HOLD_FORCE_REG  RTC_CNTL_HOLD_FORCE_REG     // rtc_io_desc[].hold_force bits (see rtcio_ll_force_hold_enable)
static inline int HwdRtcNum(uint32_t pin) { return (pin < SOC_GPIO_PIN_COUNT) ? rtc_io_num_map[pin] : -1; }
static inline bool HwdPinIsRtc(uint32_t pin) { return HwdRtcNum(pin) >= 0; }
static inline bool HwdPadRtcMux(uint32_t pin) {
  int r = HwdRtcNum(pin);
  return (r >= 0) && rtc_io_desc[r].reg && (REG_READ(rtc_io_desc[r].reg) & rtc_io_desc[r].mux);
}

// LEDC: high speed and low speed groups, fields not covered by ledc_ll getters
#define HWDUMP_LEDC
#define HWD_LEDC_CLK_LABEL    "slow_clk"                    // LEDC_CONF_REG.slow_clk_sel, used by low speed timers with tick_sel=1
#define HWD_LEDC_MODE_NUM     2
static const uint8_t kHwdLedcModes[HWD_LEDC_MODE_NUM] = { LEDC_HIGH_SPEED_MODE, LEDC_LOW_SPEED_MODE };
const char kHwdLedcModeNames[] PROGMEM = "high speed|low speed";             // indexed like kHwdLedcModes
static_assert(LEDC_HS_SIG_OUT7_IDX == LEDC_HS_SIG_OUT0_IDX + 7 && LEDC_LS_SIG_OUT7_IDX == LEDC_LS_SIG_OUT0_IDX + 7,
              "HwDump: LEDC output signals must be contiguous");
const char kHwdLedcClkNames[] PROGMEM = "RC_FAST|APB";                      // LEDC_CONF_REG.slow_clk_sel (see ledc_ll_get_slow_clk_sel)
static inline bool HwdLedcBusClk(void) { return periph_ll_periph_enabled(PERIPH_LEDC_MODULE); }
static inline uint32_t HwdLedcClkSel(void) { return LEDC.conf.slow_clk_sel; }
static inline uint32_t HwdLedcClkHz(uint32_t sel) {
  uint32_t hz = 0;
  esp_clk_tree_src_get_freq_hz(sel ? SOC_MOD_CLK_APB : SOC_MOD_CLK_RC_FAST, ESP_CLK_TREE_SRC_FREQ_PRECISION_APPROX, &hz);
  return hz;
}
// Timer clock (see ledc_ll_get_clock_source): tick_sel=0 REF_TICK, tick_sel=1 APB (high speed) or slow_clk (low speed)
static inline uint32_t HwdLedcTimerClkHz(uint32_t mode, uint32_t t, uint32_t slow_hz) {
  uint32_t hz = 0;
  if (!LEDC.timer_group[mode].timer[t].conf.tick_sel) {
    esp_clk_tree_src_get_freq_hz(SOC_MOD_CLK_REF_TICK, ESP_CLK_TREE_SRC_FREQ_PRECISION_APPROX, &hz);
  } else if (LEDC_HIGH_SPEED_MODE == mode) {
    esp_clk_tree_src_get_freq_hz(SOC_MOD_CLK_APB, ESP_CLK_TREE_SRC_FREQ_PRECISION_APPROX, &hz);
  } else {
    hz = slow_hz;
  }
  return hz;
}
static inline bool HwdLedcTimerPaused(uint32_t mode, uint32_t t) { return LEDC.timer_group[mode].timer[t].conf.pause; }
static inline bool HwdLedcTimerRst(uint32_t mode, uint32_t t) { return LEDC.timer_group[mode].timer[t].conf.rst; }
static inline uint32_t HwdLedcTimerCnt(uint32_t mode, uint32_t t) { return LEDC.timer_group[mode].timer[t].value.timer_cnt; }
static inline bool HwdLedcChOutEn(uint32_t mode, uint32_t ch) { return LEDC.channel_group[mode].channel[ch].conf0.sig_out_en; }
static inline uint32_t HwdLedcChIdle(uint32_t mode, uint32_t ch) { return LEDC.channel_group[mode].channel[ch].conf0.idle_lv; }
static inline uint32_t HwdLedcSig(uint32_t mode, uint32_t ch) {
  return ((LEDC_HIGH_SPEED_MODE == mode) ? LEDC_HS_SIG_OUT0_IDX : LEDC_LS_SIG_OUT0_IDX) + ch;
}

// UART - Warning: never read UARTn.fifo (UART_FIFO_REG), reading it pops a byte from the RX FIFO
#define HWDUMP_UART
#include "hal/uart_ll.h"
#define HwdUartDev(n)   UART_LL_GET_HW(n)                   // macro: no custom types in .ino function signatures
#define HWD_UART_CONF0      conf0                           // UART_CONF0_REG member
#define HWD_UART_CLKDIV     clk_div                         // UART_CLKDIV_REG member and its integer / fractional fields
#define HWD_UART_DIV_INT    div_int
#define HWD_UART_DIV_FRAG   div_frag
// UART_CONF0_REG.tick_ref_always_on: 0=REF_TICK 1=APB (see uart_ll_get_sclk)
const char kHwdUartClkNames[] PROGMEM = "REF_TICK|APB";
static inline uint32_t HwdUartClkSel(uint32_t n) { return HwdUartDev(n)->conf0.tick_ref_always_on; }
static inline uint32_t HwdUartClkSoc(uint32_t sel) { return sel ? SOC_MOD_CLK_APB : SOC_MOD_CLK_REF_TICK; }
// GPIO matrix signals: tx, rx, rts, cts and IO_MUX function names (nullptr if none)
static const uint16_t kHwdUartSig[SOC_UART_HP_NUM][4] = {
  { U0TXD_OUT_IDX, U0RXD_IN_IDX, U0RTS_OUT_IDX, U0CTS_IN_IDX },
  { U1TXD_OUT_IDX, U1RXD_IN_IDX, U1RTS_OUT_IDX, U1CTS_IN_IDX },
  { U2TXD_OUT_IDX, U2RXD_IN_IDX, U2RTS_OUT_IDX, U2CTS_IN_IDX },
};
static const char * const kHwdUartIomux[SOC_UART_HP_NUM][4] = {
  { "U0TXD", "U0RXD", "U0RTS", "U0CTS" },
  { "U1TXD", "U1RXD", "U1RTS", "U1CTS" },
  { "U2TXD", "U2RXD", "U2RTS", "U2CTS" },
};

// I2C - Warning: never read I2Cn.fifo_data (I2C_DATA_REG), reading it pops a byte from the RX FIFO
#define HWDUMP_I2C
#include "hal/i2c_ll.h"
const char kHwdI2cClkNames[] PROGMEM = "APB";                             // fixed APB clock
#define HwdI2cDev(n)        I2C_LL_GET_HW(n)                // macro: no custom types in .ino function signatures
#define HwdI2cClkSel(hw)    0
#define HwdI2cClkDiv(hw)    1
#define HwdI2cIsMaster(hw)  ((hw)->ctr.ms_mode)
#define HWD_I2C_SR(hw)      ((hw)->status_reg)              // I2C_SR_REG
#define HWD_I2C_RXFIFO_CNT  rx_fifo_cnt                     // I2C_SR_REG field names
#define HWD_I2C_TXFIFO_CNT  tx_fifo_cnt
#define HWD_I2C_INT_MASK    0x1FFF
#define HWD_I2C_SCL_ADJ     8                               // low = period + 1, high = period + 7 (see i2c_ll_master_set_bus_timing)
static inline bool HwdI2cOn(uint32_t n) { return periph_ll_periph_enabled(n ? PERIPH_I2C1_MODULE : PERIPH_I2C0_MODULE); }
static inline uint32_t HwdI2cClkSoc(uint32_t sel) { return SOC_MOD_CLK_APB; }
static const uint16_t kHwdI2cSig[SOC_I2C_NUM][4] = {          // scl_out, scl_in, sda_out, sda_in
  { I2CEXT0_SCL_OUT_IDX, I2CEXT0_SCL_IN_IDX, I2CEXT0_SDA_OUT_IDX, I2CEXT0_SDA_IN_IDX },
  { I2CEXT1_SCL_OUT_IDX, I2CEXT1_SCL_IN_IDX, I2CEXT1_SDA_OUT_IDX, I2CEXT1_SDA_IN_IDX },
};

// RMT (8 bidirectional channels) - Warning: never read RMT.data_ch[] (RMT_CHnDATA_REG), it is the channel FIFO access port
#define HWDUMP_RMT_ESP32
#include "soc/rmt_struct.h"
#include "soc/rmt_reg.h"
static inline bool HwdRmtOn(void) { return periph_ll_periph_enabled(PERIPH_RMT_MODULE); }
static_assert(RMT_SIG_OUT7_IDX == RMT_SIG_OUT0_IDX + 7 && RMT_SIG_IN7_IDX == RMT_SIG_IN0_IDX + 7, "HwDump: RMT signals must be contiguous");
#define HWD_RMT_F(f)        f                               // conf0/conf1 field names
#define HWD_RMT_INT_MASK    0xFFFFFFFF
static inline bool HwdRmtFifoMask(void) { return RMT.apb_conf.fifo_mask; }
static inline bool HwdRmtMemPd(void) { return RMT.conf_ch[0].conf0.mem_pd; }                  // only channel 0 has mem_pd
static inline uint32_t HwdRmtState(uint32_t ch) { return (RMT.status_ch[ch] & RMT_STATE_CH0_M) >> RMT_STATE_CH0_S; }

// I2S0/I2S1 - Warning: never read I2Sn.fifo_rd (offset 0x04), it pops data from the RX FIFO
#define HWDUMP_I2S_ESP32
#include "soc/i2s_struct.h"
const char kHwdI2sClkNames[] PROGMEM = "PLL_F160M|APLL";                  // I2S_CLKM_CONF_REG.clka_en (see i2s_ll_tx_clk_set_src)
#define HwdI2sDev(n)    ((0 == (n)) ? &I2S0 : &I2S1)        // macro: no custom types in .ino function signatures
static inline bool HwdI2sOn(uint32_t n) { return periph_ll_periph_enabled(n ? PERIPH_I2S1_MODULE : PERIPH_I2S0_MODULE); }
static inline uint32_t HwdI2sClkSoc(uint32_t sel) { return sel ? SOC_MOD_CLK_APLL : SOC_MOD_CLK_PLL_F160M; }
#define HwdI2sClkSel(clkm)  ((clkm).clka_en)                // clock source field of the I2S_CLKM_CONF_REG copy
#define HWD_I2S_PDM                                         // I2S_PDM_CONF_REG exists
#define HWD_I2S_INT_MASK    0x1FFFF
// Standard mode signals, same as i2s_periph_signal[] (data out on DATA_OUT23, data in on DATA_IN15):
//   tx bck master/slave, tx ws master/slave, dout, rx bck master/slave, rx ws master/slave, din
static const uint16_t kHwdI2sSig[SOC_I2S_NUM][10] = {
  { I2S0O_BCK_OUT_IDX, I2S0O_BCK_IN_IDX, I2S0O_WS_OUT_IDX, I2S0O_WS_IN_IDX, I2S0O_DATA_OUT23_IDX,
    I2S0I_BCK_OUT_IDX, I2S0I_BCK_IN_IDX, I2S0I_WS_OUT_IDX, I2S0I_WS_IN_IDX, I2S0I_DATA_IN15_IDX },
  { I2S1O_BCK_OUT_IDX, I2S1O_BCK_IN_IDX, I2S1O_WS_OUT_IDX, I2S1O_WS_IN_IDX, I2S1O_DATA_OUT23_IDX,
    I2S1I_BCK_OUT_IDX, I2S1I_BCK_IN_IDX, I2S1I_WS_OUT_IDX, I2S1I_WS_IN_IDX, I2S1I_DATA_IN15_IDX },
};

// SPI2 (HSPI) and SPI3 (VSPI), SPI0/1 are used by flash and not dumped
// Warning: never read SPIn.data_buf[] while a transaction is ongoing (not dumped anyway)
#define HWDUMP_SPI_ESP32
#include "soc/spi_struct.h"
#define HWD_SPI_NUM     2
static const char * const kHwdSpiNames[HWD_SPI_NUM] = { "SPI2", "SPI3" };
#define HwdSpiDev(i)    ((0 == (i)) ? &SPI2 : &SPI3)        // macro: no custom types in .ino function signatures
#define HWD_SPI_PIN(hw)           ((hw)->pin)               // SPI_PIN_REG: ck_idle_edge, cs0_dis..cs2_dis
#define HWD_SPI_CS_NUM            3
#define HwdSpiTransInten(slv)     ((slv).trans_inten)
#define HWD_SPI_DMA_INT_MASK      0x1FF
static inline bool HwdSpiOn(uint32_t i) { return periph_ll_periph_enabled(i ? PERIPH_VSPI_MODULE : PERIPH_HSPI_MODULE); }
// GPIO matrix signals: sck, d (mosi), q (miso), hd, wp, cs0, cs1, cs2 - input and output signals share the same index
static const uint16_t kHwdSpiSig[HWD_SPI_NUM][5 + HWD_SPI_CS_NUM] = {
  { HSPICLK_OUT_IDX, HSPID_OUT_IDX, HSPIQ_OUT_IDX, HSPIHD_OUT_IDX, HSPIWP_OUT_IDX, HSPICS0_OUT_IDX, HSPICS1_OUT_IDX, HSPICS2_OUT_IDX },
  { VSPICLK_OUT_IDX, VSPID_OUT_IDX, VSPIQ_OUT_IDX, VSPIHD_OUT_IDX, VSPIWP_OUT_IDX, VSPICS0_OUT_IDX, VSPICS1_OUT_IDX, VSPICS2_OUT_IDX },
};
static_assert(HSPICLK_IN_IDX == HSPICLK_OUT_IDX && HSPID_IN_IDX == HSPID_OUT_IDX && HSPIQ_IN_IDX == HSPIQ_OUT_IDX &&
              HSPICS0_IN_IDX == HSPICS0_OUT_IDX && VSPICLK_IN_IDX == VSPICLK_OUT_IDX && VSPID_IN_IDX == VSPID_OUT_IDX &&
              VSPIQ_IN_IDX == VSPIQ_OUT_IDX && VSPICS0_IN_IDX == VSPICS0_OUT_IDX, "HwDump: SPI in/out signal index mismatch");
// IO_MUX function names: sck, d, q, hd, wp, cs0
static const char * const kHwdSpiIomux[HWD_SPI_NUM][6] = {
  { "HSPICLK", "HSPID", "HSPIQ", "HSPIHD", "HSPIWP", "HSPICS0" },
  { "VSPICLK", "VSPID", "VSPIQ", "VSPIHD", "VSPIWP", "VSPICS0" },
};

// Sleep: RTC_CNTL registers
#define HWDUMP_SLEEP
#define HWDUMP_SLEEP_EXT                                    // ext0 (RTC_IO) and ext1 (RTC_CNTL) wakeup
#include "soc/rtc.h"
#include "esp_sleep.h"
#include "esp_pm.h"
// RTC_CNTL_WAKEUP_ENA bits, from esp_hw_support/port/esp32/include/soc/rtc.h (RTC_xxx_TRIG_EN), index = bit number
const char kHwdWakeupNames[] PROGMEM = "ext0|ext1|gpio|timer|sdio|wifi|uart0|uart1|touch|ulp|bt";
#define HWD_WAKEUP_BITS   11
static_assert(RTC_EXT0_TRIG_EN == BIT(0) && RTC_EXT1_TRIG_EN == BIT(1) && RTC_TIMER_TRIG_EN == BIT(3) &&
              RTC_TOUCH_TRIG_EN == BIT(8) && RTC_BT_TRIG_EN == BIT(10), "HwDump: unexpected RTC wakeup bits");
static inline uint32_t HwdWakeupEna(void) { return REG_GET_FIELD(RTC_CNTL_WAKEUP_STATE_REG, RTC_CNTL_WAKEUP_ENA); }
static inline bool HwdPadHold(uint32_t pin) {
  int r = HwdRtcNum(pin);
  if (r >= 0) {                                             // RTC pad: hold bit in the pad register or RTC_CNTL force hold
    return (REG_READ(rtc_io_desc[r].reg) & rtc_io_desc[r].hold) || (REG_READ(RTC_CNTL_HOLD_FORCE_REG) & rtc_io_desc[r].hold_force);
  }
  return gpio_ll_is_digital_io_hold(&GPIO, pin);
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

#endif  // CONFIG_IDF_TARGET_ESP32
