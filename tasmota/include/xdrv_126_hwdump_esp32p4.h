/*
  xdrv_126_hwdump_esp32p4.h - HwDump per-target data and register accessors for ESP32-P4

  SPDX-FileCopyrightText: 2026 Stephan Hadinger

  SPDX-License-Identifier: GPL-3.0-only
*/

// Only included from tasmota_xdrv_driver/xdrv_126_hwdump.ino, after HwdPad_t, HwdSig_t and HWD_SIG() are defined.
// Defines data (not only declarations), so it must be included exactly once.
#pragma once

#if CONFIG_IDF_TARGET_ESP32P4

/*********************************************************************************************\
 * ESP32-P4
 *
 * Two silicon generations with different register headers: rev < 3.0 (soc/register/hw_ver1, CONFIG_ESP32P4_SELECTS_REV_LESS_V3)
 * and rev >= 3.0 (soc/register/hw_ver3). Differences are selected with CONFIG_ESP_REV_MIN_FULL >= 300, the same test
 * as HAL_CONFIG(CHIP_SUPPORT_MIN_REV) in the IDF LL headers (USB-Serial-JTAG field names, LP pad hold register).
 *
 * Bus clock, reset and clock selectors/dividers of HP peripherals are in HP_SYS_CLKRST (soc_clk_ctrlN = bus clocks,
 * hp_rst_enN = resets, peri_clk_ctrlN = function clock source/divider/enable), USB-Serial-JTAG reset is in LP_AON_CLKRST.
 * UART/RMT/LEDC clock selector is 0=XTAL 1=RC_FAST 2=PLL_F80M.
 * LP_IO (GPIO0-15, LP IO n = GPIO n): pad config in LP_IOMUX.pad[] (per pad mux_sel), pin config/data in LP_GPIO,
 * sleep configuration is in PMU (no RTC_CNTL).
 * LP_UART, LP_I2C, LP_I2S, LP_SPI, TWAI, MCPWM, PCNT, PARLIO, I3C, EMAC, SDMMC and USB OTG are not dumped.
\*********************************************************************************************/
#define HWDUMP_SUPPORTED
#include "hal/misc.h"                 // HAL_FORCE_READ_U32_REG_FIELD()
#include "hal/pmu_types.h"            // PMU_MODE_HP_SLEEP
#include "soc/hp_sys_clkrst_struct.h" // HP_SYS_CLKRST
#include "soc/lp_clkrst_struct.h"     // LP_AON_CLKRST
#include "soc/lp_system_struct.h"     // LP_SYS
#include "soc/lpperi_struct.h"        // LPPERI
#include "soc/lp_iomux_struct.h"      // LP_IOMUX
#include "soc/lp_gpio_struct.h"       // LP_GPIO
#include "soc/pmu_struct.h"           // PMU
#include "soc/pmu_reg.h"              // PMU_EXT_WAKEUP_xxx_REG
#include "soc/rtc_io_channel.h"       // RTCIO_CHANNEL_n_GPIO_NUM
#include "esp_efuse.h"                // esp_efuse_read_field_bit()
#include "esp_efuse_table.h"          // ESP_EFUSE_USB_PHY_SEL

// Pad names and IO_MUX functions from soc/io_mux_reg.h (FUNC_GPIOn_<function>_PAD = MCU_SEL value, "_PAD" suffix removed),
// identical for hw_ver1 and hw_ver3. Empty name = no function at this MCU_SEL value
const HwdPad_t kHwdPads[] = {
  { "GPIO0",   "GPIO0_0|GPIO0" },                              //  0
  { "GPIO1",   "GPIO1_0|GPIO1" },                              //  1
  { "GPIO2",   "MTCK|GPIO2" },                                 //  2
  { "GPIO3",   "MTDI|GPIO3" },                                 //  3
  { "GPIO4",   "MTMS|GPIO4" },                                 //  4
  { "GPIO5",   "MTDO|GPIO5" },                                 //  5
  { "GPIO6",   "GPIO6_0|GPIO6||SPI2_HOLD" },                   //  6
  { "GPIO7",   "GPIO7_0|GPIO7||SPI2_CS" },                     //  7
  { "GPIO8",   "GPIO8_0|GPIO8|UART0_RTS|SPI2_D" },             //  8
  { "GPIO9",   "GPIO9_0|GPIO9|UART0_CTS|SPI2_CK" },            //  9
  { "GPIO10",  "GPIO10_0|GPIO10|UART1_TXD|SPI2_Q" },           // 10
  { "GPIO11",  "GPIO11_0|GPIO11|UART1_RXD|SPI2_WP" },          // 11
  { "GPIO12",  "GPIO12_0|GPIO12|UART1_RTS" },                  // 12
  { "GPIO13",  "GPIO13_0|GPIO13|UART1_CTS" },                  // 13
  { "GPIO14",  "GPIO14_0|GPIO14" },                            // 14
  { "GPIO15",  "GPIO15_0|GPIO15" },                            // 15
  { "GPIO16",  "GPIO16_0|GPIO16" },                            // 16
  { "GPIO17",  "GPIO17_0|GPIO17" },                            // 17
  { "GPIO18",  "GPIO18_0|GPIO18" },                            // 18
  { "GPIO19",  "GPIO19_0|GPIO19" },                            // 19
  { "GPIO20",  "GPIO20_0|GPIO20" },                            // 20
  { "GPIO21",  "GPIO21_0|GPIO21" },                            // 21
  { "GPIO22",  "GPIO22_0|GPIO22|||DBG_PSRAM_CK" },             // 22
  { "GPIO23",  "GPIO23_0|GPIO23||REF_50M_CLK|DBG_PSRAM_CS" },  // 23
  { "GPIO24",  "GPIO24_0|GPIO24" },                            // 24
  { "GPIO25",  "GPIO25_0|GPIO25" },                            // 25
  { "GPIO26",  "GPIO26_0|GPIO26" },                            // 26
  { "GPIO27",  "GPIO27_0|GPIO27" },                            // 27
  { "GPIO28",  "GPIO28_0|GPIO28|SPI2_CS|EMAC_PHY_RXDV|DBG_PSRAM_D" }, // 28
  { "GPIO29",  "GPIO29_0|GPIO29|SPI2_D|EMAC_PHY_RXD0|DBG_PSRAM_Q" }, // 29
  { "GPIO30",  "GPIO30_0|GPIO30|SPI2_CK|EMAC_PHY_RXD1|DBG_PSRAM_WP" }, // 30
  { "GPIO31",  "GPIO31_0|GPIO31|SPI2_Q|EMAC_PHY_RXER|DBG_PSRAM_HOLD" }, // 31
  { "GPIO32",  "GPIO32_0|GPIO32|SPI2_HOLD|EMAC_RMII_CLK|DBG_PSRAM_DQ4" }, // 32
  { "GPIO33",  "GPIO33_0|GPIO33|SPI2_WP|EMAC_PHY_TXEN|DBG_PSRAM_DQ5" }, // 33
  { "GPIO34",  "GPIO34_0|GPIO34|SPI2_IO4|EMAC_PHY_TXD0|DBG_PSRAM_DQ6" }, // 34
  { "GPIO35",  "GPIO35_0|GPIO35|SPI2_IO5|EMAC_PHY_TXD1|DBG_PSRAM_DQ7" }, // 35
  { "GPIO36",  "GPIO36_0|GPIO36|SPI2_IO6|EMAC_PHY_TXER|DBG_PSRAM_DQS_0" }, // 36
  { "GPIO37",  "UART0_TXD|GPIO37|SPI2_IO7" },                  // 37
  { "GPIO38",  "UART0_RXD|GPIO38|SPI2_DQS" },                  // 38
  { "GPIO39",  "SD1_CDATA0|GPIO39|BIST|REF_50M_CLK|DBG_PSRAM_DQ8" }, // 39
  { "GPIO40",  "SD1_CDATA1|GPIO40|BIST|EMAC_PHY_TXEN|DBG_PSRAM_DQ9" }, // 40
  { "GPIO41",  "SD1_CDATA2|GPIO41|BIST|EMAC_PHY_TXD0|DBG_PSRAM_DQ10" }, // 41
  { "GPIO42",  "SD1_CDATA3|GPIO42|BIST|EMAC_PHY_TXD1|DBG_PSRAM_DQ11" }, // 42
  { "GPIO43",  "SD1_CCLK|GPIO43|BIST|EMAC_PHY_TXER|DBG_PSRAM_DQ12" }, // 43
  { "GPIO44",  "SD1_CCMD|GPIO44|BIST|EMAC_RMII_CLK|DBG_PSRAM_DQ13" }, // 44
  { "GPIO45",  "SD1_CDATA4|GPIO45|BIST|EMAC_PHY_RXDV|DBG_PSRAM_DQ14" }, // 45
  { "GPIO46",  "SD1_CDATA5|GPIO46|BIST|EMAC_PHY_RXD0|DBG_PSRAM_DQ15" }, // 46
  { "GPIO47",  "SD1_CDATA6|GPIO47|BIST|EMAC_PHY_RXD1|DBG_PSRAM_DQS_1" }, // 47
  { "GPIO48",  "SD1_CDATA7|GPIO48|BIST|EMAC_PHY_RXER" },       // 48
  { "GPIO49",  "GPIO49_0|GPIO49||EMAC_PHY_TXEN|DBG_FLASH_CS" }, // 49
  { "GPIO50",  "GPIO50_0|GPIO50||EMAC_RMII_CLK|DBG_FLASH_Q" }, // 50
  { "GPIO51",  "GPIO51_0|GPIO51||EMAC_PHY_RXDV|DBG_FLASH_WP" }, // 51
  { "GPIO52",  "GPIO52_0|GPIO52||EMAC_PHY_RXD0|DBG_FLASH_HOLD" }, // 52
  { "GPIO53",  "GPIO53_0|GPIO53||EMAC_PHY_RXD1|DBG_FLASH_CK" }, // 53
  { "GPIO54",  "GPIO54_0|GPIO54||EMAC_PHY_RXER|DBG_FLASH_D" }, // 54
};

// Sanity checks of the function indexes above against ESP-IDF definitions
static_assert(PIN_FUNC_GPIO == 1, "HwDump: unexpected PIN_FUNC_GPIO");
static_assert(FUNC_GPIO0_GPIO0_0 == 0 && FUNC_GPIO0_GPIO0 == 1, "HwDump: IO_MUX pad 0");
static_assert(FUNC_GPIO2_MTCK == 0 && FUNC_GPIO5_MTDO == 0, "HwDump: IO_MUX pads 2-5");
static_assert(FUNC_GPIO6_SPI2_HOLD_PAD == 3 && FUNC_GPIO7_SPI2_CS_PAD == 3, "HwDump: IO_MUX pads 6-7");
static_assert(FUNC_GPIO8_UART0_RTS_PAD == 2 && FUNC_GPIO8_SPI2_D_PAD == 3, "HwDump: IO_MUX pad 8");
static_assert(FUNC_GPIO11_UART1_RXD_PAD == 2 && FUNC_GPIO11_SPI2_WP_PAD == 3, "HwDump: IO_MUX pad 11");
static_assert(FUNC_GPIO13_UART1_CTS_PAD == 2, "HwDump: IO_MUX pad 13");
static_assert(FUNC_GPIO22_DBG_PSRAM_CK_PAD == 4 && FUNC_GPIO23_REF_50M_CLK_PAD == 3, "HwDump: IO_MUX pads 22-23");
static_assert(FUNC_GPIO28_SPI2_CS_PAD == 2 && FUNC_GPIO28_EMAC_PHY_RXDV_PAD == 3 && FUNC_GPIO28_DBG_PSRAM_D_PAD == 4, "HwDump: IO_MUX pad 28");
static_assert(FUNC_GPIO36_DBG_PSRAM_DQS_0_PAD == 4, "HwDump: IO_MUX pad 36");
static_assert(FUNC_GPIO37_UART0_TXD_PAD == 0 && FUNC_GPIO37_SPI2_IO7_PAD == 2, "HwDump: IO_MUX pad 37");
static_assert(FUNC_GPIO38_UART0_RXD_PAD == 0 && FUNC_GPIO38_SPI2_DQS_PAD == 2, "HwDump: IO_MUX pad 38");
static_assert(FUNC_GPIO39_SD1_CDATA0_PAD == 0 && FUNC_GPIO39_BIST_PAD == 2, "HwDump: IO_MUX pad 39");
static_assert(FUNC_GPIO48_SD1_CDATA7_PAD == 0 && FUNC_GPIO48_EMAC_PHY_RXER_PAD == 3, "HwDump: IO_MUX pad 48");
static_assert(FUNC_GPIO49_DBG_FLASH_CS_PAD == 4 && FUNC_GPIO54_DBG_FLASH_D_PAD == 4, "HwDump: IO_MUX pads 49-54");

// GPIO matrix output signals (GPIO_FUNCn_OUT_SEL), from soc/gpio_sig_map.h (xxx_PAD_OUT_IDX, "_PAD" removed from the name)
// SIG_IN_FUNC250..255 are listed as outputs (same as other targets)
const HwdSig_t kHwdSigOut[] = {
  { SD_CARD_CCLK_2_PAD_OUT_IDX, "SD_CARD_CCLK_2_OUT" }, { SD_CARD_CCMD_2_PAD_OUT_IDX, "SD_CARD_CCMD_2_OUT" },
  { SD_CARD_CDATA0_2_PAD_OUT_IDX, "SD_CARD_CDATA0_2_OUT" }, { SD_CARD_CDATA1_2_PAD_OUT_IDX, "SD_CARD_CDATA1_2_OUT" },
  { SD_CARD_CDATA2_2_PAD_OUT_IDX, "SD_CARD_CDATA2_2_OUT" }, { SD_CARD_CDATA3_2_PAD_OUT_IDX, "SD_CARD_CDATA3_2_OUT" },
  { SD_CARD_CDATA4_2_PAD_OUT_IDX, "SD_CARD_CDATA4_2_OUT" }, { SD_CARD_CDATA5_2_PAD_OUT_IDX, "SD_CARD_CDATA5_2_OUT" },
  { SD_CARD_CDATA6_2_PAD_OUT_IDX, "SD_CARD_CDATA6_2_OUT" }, { SD_CARD_CDATA7_2_PAD_OUT_IDX, "SD_CARD_CDATA7_2_OUT" },
  { UART0_TXD_PAD_OUT_IDX, "UART0_TXD_OUT" }, { UART0_RTS_PAD_OUT_IDX, "UART0_RTS_OUT" },
  { UART0_DTR_PAD_OUT_IDX, "UART0_DTR_OUT" }, { UART1_TXD_PAD_OUT_IDX, "UART1_TXD_OUT" },
  { UART1_RTS_PAD_OUT_IDX, "UART1_RTS_OUT" }, { UART1_DTR_PAD_OUT_IDX, "UART1_DTR_OUT" },
  { UART2_TXD_PAD_OUT_IDX, "UART2_TXD_OUT" }, { UART2_RTS_PAD_OUT_IDX, "UART2_RTS_OUT" },
  { UART2_DTR_PAD_OUT_IDX, "UART2_DTR_OUT" }, { UART3_TXD_PAD_OUT_IDX, "UART3_TXD_OUT" },
  { UART3_RTS_PAD_OUT_IDX, "UART3_RTS_OUT" }, { UART3_DTR_PAD_OUT_IDX, "UART3_DTR_OUT" },
  { UART4_TXD_PAD_OUT_IDX, "UART4_TXD_OUT" }, { UART4_RTS_PAD_OUT_IDX, "UART4_RTS_OUT" },
  { UART4_DTR_PAD_OUT_IDX, "UART4_DTR_OUT" }, { I2S0_O_BCK_PAD_OUT_IDX, "I2S0_O_BCK_OUT" },
  { I2S0_MCLK_PAD_OUT_IDX, "I2S0_MCLK_OUT" }, { I2S0_O_WS_PAD_OUT_IDX, "I2S0_O_WS_OUT" },
  { I2S0_O_SD_PAD_OUT_IDX, "I2S0_O_SD_OUT" }, { I2S0_I_BCK_PAD_OUT_IDX, "I2S0_I_BCK_OUT" },
  { I2S0_I_WS_PAD_OUT_IDX, "I2S0_I_WS_OUT" }, { I2S1_O_BCK_PAD_OUT_IDX, "I2S1_O_BCK_OUT" },
  { I2S1_MCLK_PAD_OUT_IDX, "I2S1_MCLK_OUT" }, { I2S1_O_WS_PAD_OUT_IDX, "I2S1_O_WS_OUT" },
  { I2S1_O_SD_PAD_OUT_IDX, "I2S1_O_SD_OUT" }, { I2S1_I_BCK_PAD_OUT_IDX, "I2S1_I_BCK_OUT" },
  { I2S1_I_WS_PAD_OUT_IDX, "I2S1_I_WS_OUT" }, { I2S2_O_BCK_PAD_OUT_IDX, "I2S2_O_BCK_OUT" },
  { I2S2_MCLK_PAD_OUT_IDX, "I2S2_MCLK_OUT" }, { I2S2_O_WS_PAD_OUT_IDX, "I2S2_O_WS_OUT" },
  { I2S2_O_SD_PAD_OUT_IDX, "I2S2_O_SD_OUT" }, { I2S2_I_BCK_PAD_OUT_IDX, "I2S2_I_BCK_OUT" },
  { I2S2_I_WS_PAD_OUT_IDX, "I2S2_I_WS_OUT" }, { I2S0_O_SD1_PAD_OUT_IDX, "I2S0_O_SD1_OUT" },
  { SPI2_DQS_PAD_OUT_IDX, "SPI2_DQS_OUT" }, { SPI3_CS2_PAD_OUT_IDX, "SPI3_CS2_OUT" },
  { SPI3_CS1_PAD_OUT_IDX, "SPI3_CS1_OUT" }, { SPI3_CK_PAD_OUT_IDX, "SPI3_CK_OUT" },
  { SPI3_QO_PAD_OUT_IDX, "SPI3_QO_OUT" }, { SPI3_D_PAD_OUT_IDX, "SPI3_D_OUT" },
  { SPI3_HOLD_PAD_OUT_IDX, "SPI3_HOLD_OUT" }, { SPI3_WP_PAD_OUT_IDX, "SPI3_WP_OUT" },
  { SPI3_CS_PAD_OUT_IDX, "SPI3_CS_OUT" }, { SPI2_CK_PAD_OUT_IDX, "SPI2_CK_OUT" },
  { SPI2_Q_PAD_OUT_IDX, "SPI2_Q_OUT" }, { SPI2_D_PAD_OUT_IDX, "SPI2_D_OUT" },
  { SPI2_HOLD_PAD_OUT_IDX, "SPI2_HOLD_OUT" }, { SPI2_WP_PAD_OUT_IDX, "SPI2_WP_OUT" },
  { SPI2_IO4_PAD_OUT_IDX, "SPI2_IO4_OUT" }, { SPI2_IO5_PAD_OUT_IDX, "SPI2_IO5_OUT" },
  { SPI2_IO6_PAD_OUT_IDX, "SPI2_IO6_OUT" }, { SPI2_IO7_PAD_OUT_IDX, "SPI2_IO7_OUT" },
  { SPI2_CS_PAD_OUT_IDX, "SPI2_CS_OUT" }, { SPI2_CS1_PAD_OUT_IDX, "SPI2_CS1_OUT" },
  { SPI2_CS2_PAD_OUT_IDX, "SPI2_CS2_OUT" }, { SPI2_CS3_PAD_OUT_IDX, "SPI2_CS3_OUT" },
  { SPI2_CS4_PAD_OUT_IDX, "SPI2_CS4_OUT" }, { SPI2_CS5_PAD_OUT_IDX, "SPI2_CS5_OUT" },
  { I2C0_SCL_PAD_OUT_IDX, "I2C0_SCL_OUT" }, { I2C0_SDA_PAD_OUT_IDX, "I2C0_SDA_OUT" },
  { I2C1_SCL_PAD_OUT_IDX, "I2C1_SCL_OUT" }, { I2C1_SDA_PAD_OUT_IDX, "I2C1_SDA_OUT" },
  { GPIO_SD0_OUT_IDX, "GPIO_SD0_OUT" }, { GPIO_SD1_OUT_IDX, "GPIO_SD1_OUT" }, { GPIO_SD2_OUT_IDX, "GPIO_SD2_OUT" },
  { GPIO_SD3_OUT_IDX, "GPIO_SD3_OUT" }, { GPIO_SD4_OUT_IDX, "GPIO_SD4_OUT" }, { GPIO_SD5_OUT_IDX, "GPIO_SD5_OUT" },
  { GPIO_SD6_OUT_IDX, "GPIO_SD6_OUT" }, { GPIO_SD7_OUT_IDX, "GPIO_SD7_OUT" },
  { TWAI0_TX_PAD_OUT_IDX, "TWAI0_TX_OUT" }, { TWAI0_BUS_OFF_ON_PAD_OUT_IDX, "TWAI0_BUS_OFF_ON_OUT" },
  { TWAI0_CLKOUT_PAD_OUT_IDX, "TWAI0_CLKOUT_OUT" }, { TWAI1_TX_PAD_OUT_IDX, "TWAI1_TX_OUT" },
  { TWAI1_BUS_OFF_ON_PAD_OUT_IDX, "TWAI1_BUS_OFF_ON_OUT" }, { TWAI1_CLKOUT_PAD_OUT_IDX, "TWAI1_CLKOUT_OUT" },
  { TWAI2_TX_PAD_OUT_IDX, "TWAI2_TX_OUT" }, { TWAI2_BUS_OFF_ON_PAD_OUT_IDX, "TWAI2_BUS_OFF_ON_OUT" },
  { TWAI2_CLKOUT_PAD_OUT_IDX, "TWAI2_CLKOUT_OUT" }, { PWM0_CH0_A_PAD_OUT_IDX, "PWM0_CH0_A_OUT" },
  { PWM0_CH0_B_PAD_OUT_IDX, "PWM0_CH0_B_OUT" }, { PWM0_CH1_A_PAD_OUT_IDX, "PWM0_CH1_A_OUT" },
  { PWM0_CH1_B_PAD_OUT_IDX, "PWM0_CH1_B_OUT" }, { PWM0_CH2_A_PAD_OUT_IDX, "PWM0_CH2_A_OUT" },
  { PWM0_CH2_B_PAD_OUT_IDX, "PWM0_CH2_B_OUT" }, { PWM1_CH0_A_PAD_OUT_IDX, "PWM1_CH0_A_OUT" },
  { PWM1_CH0_B_PAD_OUT_IDX, "PWM1_CH0_B_OUT" }, { PWM1_CH1_A_PAD_OUT_IDX, "PWM1_CH1_A_OUT" },
  { PWM1_CH1_B_PAD_OUT_IDX, "PWM1_CH1_B_OUT" }, { PWM1_CH2_A_PAD_OUT_IDX, "PWM1_CH2_A_OUT" },
  { PWM1_CH2_B_PAD_OUT_IDX, "PWM1_CH2_B_OUT" }, { TWAI0_STANDBY_PAD_OUT_IDX, "TWAI0_STANDBY_OUT" },
  { TWAI1_STANDBY_PAD_OUT_IDX, "TWAI1_STANDBY_OUT" }, { TWAI2_STANDBY_PAD_OUT_IDX, "TWAI2_STANDBY_OUT" },
  { MII_MDC_PAD_OUT_IDX, "MII_MDC_OUT" }, { MII_MDO_PAD_OUT_IDX, "MII_MDO_OUT" },
  { USB_SRP_DISCHRGVBUS_PAD_OUT_IDX, "USB_SRP_DISCHRGVBUS_OUT" },
  { USB_OTG11_IDPULLUP_PAD_OUT_IDX, "USB_OTG11_IDPULLUP_OUT" },
  { USB_OTG11_DPPULLDOWN_PAD_OUT_IDX, "USB_OTG11_DPPULLDOWN_OUT" },
  { USB_OTG11_DMPULLDOWN_PAD_OUT_IDX, "USB_OTG11_DMPULLDOWN_OUT" },
  { USB_OTG11_DRVVBUS_PAD_OUT_IDX, "USB_OTG11_DRVVBUS_OUT" },
  { USB_SRP_CHRGVBUS_PAD_OUT_IDX, "USB_SRP_CHRGVBUS_OUT" }, { RNG_CHAIN_CLK_PAD_OUT_IDX, "RNG_CHAIN_CLK_OUT" },
  { HP_PROBE_TOP_OUT0_IDX, "HP_PROBE_TOP_OUT0" }, { HP_PROBE_TOP_OUT1_IDX, "HP_PROBE_TOP_OUT1" },
  { HP_PROBE_TOP_OUT2_IDX, "HP_PROBE_TOP_OUT2" }, { HP_PROBE_TOP_OUT3_IDX, "HP_PROBE_TOP_OUT3" },
  { HP_PROBE_TOP_OUT4_IDX, "HP_PROBE_TOP_OUT4" }, { HP_PROBE_TOP_OUT5_IDX, "HP_PROBE_TOP_OUT5" },
  { HP_PROBE_TOP_OUT6_IDX, "HP_PROBE_TOP_OUT6" }, { HP_PROBE_TOP_OUT7_IDX, "HP_PROBE_TOP_OUT7" },
  { LEDC_LS_SIG_OUT_PAD_OUT0_IDX, "LEDC_LS_SIG_OUT0" }, { LEDC_LS_SIG_OUT_PAD_OUT1_IDX, "LEDC_LS_SIG_OUT1" },
  { LEDC_LS_SIG_OUT_PAD_OUT2_IDX, "LEDC_LS_SIG_OUT2" }, { LEDC_LS_SIG_OUT_PAD_OUT3_IDX, "LEDC_LS_SIG_OUT3" },
  { LEDC_LS_SIG_OUT_PAD_OUT4_IDX, "LEDC_LS_SIG_OUT4" }, { LEDC_LS_SIG_OUT_PAD_OUT5_IDX, "LEDC_LS_SIG_OUT5" },
  { LEDC_LS_SIG_OUT_PAD_OUT6_IDX, "LEDC_LS_SIG_OUT6" }, { LEDC_LS_SIG_OUT_PAD_OUT7_IDX, "LEDC_LS_SIG_OUT7" },
  { I3C_MST_SCL_PAD_OUT_IDX, "I3C_MST_SCL_OUT" }, { I3C_MST_SDA_PAD_OUT_IDX, "I3C_MST_SDA_OUT" },
  { I3C_SLV_SCL_PAD_OUT_IDX, "I3C_SLV_SCL_OUT" }, { I3C_SLV_SDA_PAD_OUT_IDX, "I3C_SLV_SDA_OUT" },
  { I3C_MST_SCL_PULLUP_EN_PAD_OUT_IDX, "I3C_MST_SCL_PULLUP_EN_OUT" },
  { I3C_MST_SDA_PULLUP_EN_PAD_OUT_IDX, "I3C_MST_SDA_PULLUP_EN_OUT" },
  { USB_JTAG_TDI_BRIDGE_PAD_OUT_IDX, "USB_JTAG_TDI_BRIDGE_OUT" },
  { USB_JTAG_TMS_BRIDGE_PAD_OUT_IDX, "USB_JTAG_TMS_BRIDGE_OUT" },
  { USB_JTAG_TCK_BRIDGE_PAD_OUT_IDX, "USB_JTAG_TCK_BRIDGE_OUT" },
  { USB_JTAG_TRST_BRIDGE_PAD_OUT_IDX, "USB_JTAG_TRST_BRIDGE_OUT" }, { LCD_CS_PAD_OUT_IDX, "LCD_CS_OUT" },
  { LCD_DC_PAD_OUT_IDX, "LCD_DC_OUT" }, { SD_RST_N_1_PAD_OUT_IDX, "SD_RST_N_1_OUT" },
  { SD_RST_N_2_PAD_OUT_IDX, "SD_RST_N_2_OUT" }, { SD_CCMD_OD_PULLUP_EN_N_PAD_OUT_IDX, "SD_CCMD_OD_PULLUP_EN_N_OUT" },
  { LCD_PCLK_PAD_OUT_IDX, "LCD_PCLK_OUT" }, { CAM_CLK_PAD_OUT_IDX, "CAM_CLK_OUT" },
  { LCD_H_ENABLE_PAD_OUT_IDX, "LCD_H_ENABLE_OUT" }, { LCD_H_SYNC_PAD_OUT_IDX, "LCD_H_SYNC_OUT" },
  { LCD_V_SYNC_PAD_OUT_IDX, "LCD_V_SYNC_OUT" }, { LCD_DATA_OUT_PAD_OUT0_IDX, "LCD_DATA_OUT_OUT0" },
  { LCD_DATA_OUT_PAD_OUT1_IDX, "LCD_DATA_OUT_OUT1" }, { LCD_DATA_OUT_PAD_OUT2_IDX, "LCD_DATA_OUT_OUT2" },
  { LCD_DATA_OUT_PAD_OUT3_IDX, "LCD_DATA_OUT_OUT3" }, { LCD_DATA_OUT_PAD_OUT4_IDX, "LCD_DATA_OUT_OUT4" },
  { LCD_DATA_OUT_PAD_OUT5_IDX, "LCD_DATA_OUT_OUT5" }, { LCD_DATA_OUT_PAD_OUT6_IDX, "LCD_DATA_OUT_OUT6" },
  { LCD_DATA_OUT_PAD_OUT7_IDX, "LCD_DATA_OUT_OUT7" }, { LCD_DATA_OUT_PAD_OUT8_IDX, "LCD_DATA_OUT_OUT8" },
  { LCD_DATA_OUT_PAD_OUT9_IDX, "LCD_DATA_OUT_OUT9" }, { LCD_DATA_OUT_PAD_OUT10_IDX, "LCD_DATA_OUT_OUT10" },
  { LCD_DATA_OUT_PAD_OUT11_IDX, "LCD_DATA_OUT_OUT11" }, { LCD_DATA_OUT_PAD_OUT12_IDX, "LCD_DATA_OUT_OUT12" },
  { LCD_DATA_OUT_PAD_OUT13_IDX, "LCD_DATA_OUT_OUT13" }, { LCD_DATA_OUT_PAD_OUT14_IDX, "LCD_DATA_OUT_OUT14" },
  { LCD_DATA_OUT_PAD_OUT15_IDX, "LCD_DATA_OUT_OUT15" }, { LCD_DATA_OUT_PAD_OUT16_IDX, "LCD_DATA_OUT_OUT16" },
  { LCD_DATA_OUT_PAD_OUT17_IDX, "LCD_DATA_OUT_OUT17" }, { LCD_DATA_OUT_PAD_OUT18_IDX, "LCD_DATA_OUT_OUT18" },
  { LCD_DATA_OUT_PAD_OUT19_IDX, "LCD_DATA_OUT_OUT19" }, { LCD_DATA_OUT_PAD_OUT20_IDX, "LCD_DATA_OUT_OUT20" },
  { LCD_DATA_OUT_PAD_OUT21_IDX, "LCD_DATA_OUT_OUT21" }, { LCD_DATA_OUT_PAD_OUT22_IDX, "LCD_DATA_OUT_OUT22" },
  { LCD_DATA_OUT_PAD_OUT23_IDX, "LCD_DATA_OUT_OUT23" }, { EMAC_PHY_TXEN_PAD_OUT_IDX, "EMAC_PHY_TXEN_OUT" },
  { EMAC_PHY_TXD0_PAD_OUT_IDX, "EMAC_PHY_TXD0_OUT" }, { EMAC_PHY_TXD1_PAD_OUT_IDX, "EMAC_PHY_TXD1_OUT" },
  { EMAC_PHY_TXD2_PAD_OUT_IDX, "EMAC_PHY_TXD2_OUT" }, { EMAC_PHY_TXD3_PAD_OUT_IDX, "EMAC_PHY_TXD3_OUT" },
  { EMAC_PHY_TXER_PAD_OUT_IDX, "EMAC_PHY_TXER_OUT" }, { DBG_CH0_CLK_IDX, "DBG_CH0_CLK" },
  { DBG_CH1_CLK_IDX, "DBG_CH1_CLK" }, { PARLIO_RX_CLK_PAD_OUT_IDX, "PARLIO_RX_CLK_OUT" },
  { PARLIO_TX_CLK_PAD_OUT_IDX, "PARLIO_TX_CLK_OUT" }, { PARLIO_TX_DATA0_PAD_OUT_IDX, "PARLIO_TX_DATA0_OUT" },
  { PARLIO_TX_DATA1_PAD_OUT_IDX, "PARLIO_TX_DATA1_OUT" }, { PARLIO_TX_DATA2_PAD_OUT_IDX, "PARLIO_TX_DATA2_OUT" },
  { PARLIO_TX_DATA3_PAD_OUT_IDX, "PARLIO_TX_DATA3_OUT" }, { PARLIO_TX_DATA4_PAD_OUT_IDX, "PARLIO_TX_DATA4_OUT" },
  { PARLIO_TX_DATA5_PAD_OUT_IDX, "PARLIO_TX_DATA5_OUT" }, { PARLIO_TX_DATA6_PAD_OUT_IDX, "PARLIO_TX_DATA6_OUT" },
  { PARLIO_TX_DATA7_PAD_OUT_IDX, "PARLIO_TX_DATA7_OUT" }, { PARLIO_TX_DATA8_PAD_OUT_IDX, "PARLIO_TX_DATA8_OUT" },
  { PARLIO_TX_DATA9_PAD_OUT_IDX, "PARLIO_TX_DATA9_OUT" }, { PARLIO_TX_DATA10_PAD_OUT_IDX, "PARLIO_TX_DATA10_OUT" },
  { PARLIO_TX_DATA11_PAD_OUT_IDX, "PARLIO_TX_DATA11_OUT" }, { PARLIO_TX_DATA12_PAD_OUT_IDX, "PARLIO_TX_DATA12_OUT" },
  { PARLIO_TX_DATA13_PAD_OUT_IDX, "PARLIO_TX_DATA13_OUT" }, { PARLIO_TX_DATA14_PAD_OUT_IDX, "PARLIO_TX_DATA14_OUT" },
  { PARLIO_TX_DATA15_PAD_OUT_IDX, "PARLIO_TX_DATA15_OUT" }, { HP_PROBE_TOP_OUT8_IDX, "HP_PROBE_TOP_OUT8" },
  { HP_PROBE_TOP_OUT9_IDX, "HP_PROBE_TOP_OUT9" }, { HP_PROBE_TOP_OUT10_IDX, "HP_PROBE_TOP_OUT10" },
  { HP_PROBE_TOP_OUT11_IDX, "HP_PROBE_TOP_OUT11" }, { HP_PROBE_TOP_OUT12_IDX, "HP_PROBE_TOP_OUT12" },
  { HP_PROBE_TOP_OUT13_IDX, "HP_PROBE_TOP_OUT13" }, { HP_PROBE_TOP_OUT14_IDX, "HP_PROBE_TOP_OUT14" },
  { HP_PROBE_TOP_OUT15_IDX, "HP_PROBE_TOP_OUT15" }, { CONSTANT0_PAD_OUT_IDX, "CONSTANT0_OUT" },
  { CONSTANT1_PAD_OUT_IDX, "CONSTANT1_OUT" }, { CORE_GPIO_OUT_PAD_OUT0_IDX, "CORE_GPIO_OUT_OUT0" },
  { CORE_GPIO_OUT_PAD_OUT1_IDX, "CORE_GPIO_OUT_OUT1" }, { CORE_GPIO_OUT_PAD_OUT2_IDX, "CORE_GPIO_OUT_OUT2" },
  { CORE_GPIO_OUT_PAD_OUT3_IDX, "CORE_GPIO_OUT_OUT3" }, { CORE_GPIO_OUT_PAD_OUT4_IDX, "CORE_GPIO_OUT_OUT4" },
  { CORE_GPIO_OUT_PAD_OUT5_IDX, "CORE_GPIO_OUT_OUT5" }, { CORE_GPIO_OUT_PAD_OUT6_IDX, "CORE_GPIO_OUT_OUT6" },
  { CORE_GPIO_OUT_PAD_OUT7_IDX, "CORE_GPIO_OUT_OUT7" }, { CORE_GPIO_OUT_PAD_OUT8_IDX, "CORE_GPIO_OUT_OUT8" },
  { CORE_GPIO_OUT_PAD_OUT9_IDX, "CORE_GPIO_OUT_OUT9" }, { CORE_GPIO_OUT_PAD_OUT10_IDX, "CORE_GPIO_OUT_OUT10" },
  { CORE_GPIO_OUT_PAD_OUT11_IDX, "CORE_GPIO_OUT_OUT11" }, { CORE_GPIO_OUT_PAD_OUT12_IDX, "CORE_GPIO_OUT_OUT12" },
  { CORE_GPIO_OUT_PAD_OUT13_IDX, "CORE_GPIO_OUT_OUT13" }, { CORE_GPIO_OUT_PAD_OUT14_IDX, "CORE_GPIO_OUT_OUT14" },
  { CORE_GPIO_OUT_PAD_OUT15_IDX, "CORE_GPIO_OUT_OUT15" }, { CORE_GPIO_OUT_PAD_OUT16_IDX, "CORE_GPIO_OUT_OUT16" },
  { CORE_GPIO_OUT_PAD_OUT17_IDX, "CORE_GPIO_OUT_OUT17" }, { CORE_GPIO_OUT_PAD_OUT18_IDX, "CORE_GPIO_OUT_OUT18" },
  { CORE_GPIO_OUT_PAD_OUT19_IDX, "CORE_GPIO_OUT_OUT19" }, { CORE_GPIO_OUT_PAD_OUT20_IDX, "CORE_GPIO_OUT_OUT20" },
  { CORE_GPIO_OUT_PAD_OUT21_IDX, "CORE_GPIO_OUT_OUT21" }, { CORE_GPIO_OUT_PAD_OUT22_IDX, "CORE_GPIO_OUT_OUT22" },
  { CORE_GPIO_OUT_PAD_OUT23_IDX, "CORE_GPIO_OUT_OUT23" }, { CORE_GPIO_OUT_PAD_OUT24_IDX, "CORE_GPIO_OUT_OUT24" },
  { CORE_GPIO_OUT_PAD_OUT25_IDX, "CORE_GPIO_OUT_OUT25" }, { CORE_GPIO_OUT_PAD_OUT26_IDX, "CORE_GPIO_OUT_OUT26" },
  { CORE_GPIO_OUT_PAD_OUT27_IDX, "CORE_GPIO_OUT_OUT27" }, { PARLIO_TX_CS_PAD_OUT_IDX, "PARLIO_TX_CS_OUT" },
  { EMAC_PTP_PPS_PAD_OUT_IDX, "EMAC_PTP_PPS_OUT" }, { ANA_COMP0_OUT_IDX, "ANA_COMP0_OUT" },
  { ANA_COMP1_OUT_IDX, "ANA_COMP1_OUT" }, { RMT_SIG_PAD_OUT0_IDX, "RMT_SIG_OUT0" },
  { RMT_SIG_PAD_OUT1_IDX, "RMT_SIG_OUT1" }, { RMT_SIG_PAD_OUT2_IDX, "RMT_SIG_OUT2" },
  { RMT_SIG_PAD_OUT3_IDX, "RMT_SIG_OUT3" }, { SIG_IN_FUNC250_IDX, "SIG_IN_FUNC250" },
  { SIG_IN_FUNC251_IDX, "SIG_IN_FUNC251" }, { SIG_IN_FUNC252_IDX, "SIG_IN_FUNC252" },
  { SIG_IN_FUNC253_IDX, "SIG_IN_FUNC253" }, { SIG_IN_FUNC254_IDX, "SIG_IN_FUNC254" },
  { SIG_IN_FUNC255_IDX, "SIG_IN_FUNC255" },
  { SIG_GPIO_OUT_IDX, "GPIO" },                   // simple GPIO output (GPIO_OUT register)
};

// GPIO matrix input signals (GPIO_FUNCm_IN_SEL_CFG), from soc/gpio_sig_map.h (xxx_PAD_IN_IDX, "_PAD" removed from the name)
const HwdSig_t kHwdSigIn[] = {
  { SD_CARD_CCMD_2_PAD_IN_IDX, "SD_CARD_CCMD_2_IN" }, { SD_CARD_CDATA0_2_PAD_IN_IDX, "SD_CARD_CDATA0_2_IN" },
  { SD_CARD_CDATA1_2_PAD_IN_IDX, "SD_CARD_CDATA1_2_IN" }, { SD_CARD_CDATA2_2_PAD_IN_IDX, "SD_CARD_CDATA2_2_IN" },
  { SD_CARD_CDATA3_2_PAD_IN_IDX, "SD_CARD_CDATA3_2_IN" }, { SD_CARD_CDATA4_2_PAD_IN_IDX, "SD_CARD_CDATA4_2_IN" },
  { SD_CARD_CDATA5_2_PAD_IN_IDX, "SD_CARD_CDATA5_2_IN" }, { SD_CARD_CDATA6_2_PAD_IN_IDX, "SD_CARD_CDATA6_2_IN" },
  { SD_CARD_CDATA7_2_PAD_IN_IDX, "SD_CARD_CDATA7_2_IN" }, { UART0_RXD_PAD_IN_IDX, "UART0_RXD_IN" },
  { UART0_CTS_PAD_IN_IDX, "UART0_CTS_IN" }, { UART0_DSR_PAD_IN_IDX, "UART0_DSR_IN" },
  { UART1_RXD_PAD_IN_IDX, "UART1_RXD_IN" }, { UART1_CTS_PAD_IN_IDX, "UART1_CTS_IN" },
  { UART1_DSR_PAD_IN_IDX, "UART1_DSR_IN" }, { UART2_RXD_PAD_IN_IDX, "UART2_RXD_IN" },
  { UART2_CTS_PAD_IN_IDX, "UART2_CTS_IN" }, { UART2_DSR_PAD_IN_IDX, "UART2_DSR_IN" },
  { UART3_RXD_PAD_IN_IDX, "UART3_RXD_IN" }, { UART3_CTS_PAD_IN_IDX, "UART3_CTS_IN" },
  { UART3_DSR_PAD_IN_IDX, "UART3_DSR_IN" }, { UART4_RXD_PAD_IN_IDX, "UART4_RXD_IN" },
  { UART4_CTS_PAD_IN_IDX, "UART4_CTS_IN" }, { UART4_DSR_PAD_IN_IDX, "UART4_DSR_IN" },
  { I2S0_O_BCK_PAD_IN_IDX, "I2S0_O_BCK_IN" }, { I2S0_MCLK_PAD_IN_IDX, "I2S0_MCLK_IN" },
  { I2S0_O_WS_PAD_IN_IDX, "I2S0_O_WS_IN" }, { I2S0_I_SD_PAD_IN_IDX, "I2S0_I_SD_IN" },
  { I2S0_I_BCK_PAD_IN_IDX, "I2S0_I_BCK_IN" }, { I2S0_I_WS_PAD_IN_IDX, "I2S0_I_WS_IN" },
  { I2S1_O_BCK_PAD_IN_IDX, "I2S1_O_BCK_IN" }, { I2S1_MCLK_PAD_IN_IDX, "I2S1_MCLK_IN" },
  { I2S1_O_WS_PAD_IN_IDX, "I2S1_O_WS_IN" }, { I2S1_I_SD_PAD_IN_IDX, "I2S1_I_SD_IN" },
  { I2S1_I_BCK_PAD_IN_IDX, "I2S1_I_BCK_IN" }, { I2S1_I_WS_PAD_IN_IDX, "I2S1_I_WS_IN" },
  { I2S2_O_BCK_PAD_IN_IDX, "I2S2_O_BCK_IN" }, { I2S2_MCLK_PAD_IN_IDX, "I2S2_MCLK_IN" },
  { I2S2_O_WS_PAD_IN_IDX, "I2S2_O_WS_IN" }, { I2S2_I_SD_PAD_IN_IDX, "I2S2_I_SD_IN" },
  { I2S2_I_BCK_PAD_IN_IDX, "I2S2_I_BCK_IN" }, { I2S2_I_WS_PAD_IN_IDX, "I2S2_I_WS_IN" },
  { I2S0_I_SD1_PAD_IN_IDX, "I2S0_I_SD1_IN" }, { I2S0_I_SD2_PAD_IN_IDX, "I2S0_I_SD2_IN" },
  { I2S0_I_SD3_PAD_IN_IDX, "I2S0_I_SD3_IN" }, { SPI3_CK_PAD_IN_IDX, "SPI3_CK_IN" },
  { SPI3_Q_PAD_IN_IDX, "SPI3_Q_IN" }, { SPI3_D_PAD_IN_IDX, "SPI3_D_IN" }, { SPI3_HOLD_PAD_IN_IDX, "SPI3_HOLD_IN" },
  { SPI3_WP_PAD_IN_IDX, "SPI3_WP_IN" }, { SPI3_CS_PAD_IN_IDX, "SPI3_CS_IN" }, { SPI2_CK_PAD_IN_IDX, "SPI2_CK_IN" },
  { SPI2_Q_PAD_IN_IDX, "SPI2_Q_IN" }, { SPI2_D_PAD_IN_IDX, "SPI2_D_IN" }, { SPI2_HOLD_PAD_IN_IDX, "SPI2_HOLD_IN" },
  { SPI2_WP_PAD_IN_IDX, "SPI2_WP_IN" }, { SPI2_IO4_PAD_IN_IDX, "SPI2_IO4_IN" },
  { SPI2_IO5_PAD_IN_IDX, "SPI2_IO5_IN" }, { SPI2_IO6_PAD_IN_IDX, "SPI2_IO6_IN" },
  { SPI2_IO7_PAD_IN_IDX, "SPI2_IO7_IN" }, { SPI2_CS_PAD_IN_IDX, "SPI2_CS_IN" },
  { PCNT_RST_PAD_IN0_IDX, "PCNT_RST_IN0" }, { PCNT_RST_PAD_IN1_IDX, "PCNT_RST_IN1" },
  { PCNT_RST_PAD_IN2_IDX, "PCNT_RST_IN2" }, { PCNT_RST_PAD_IN3_IDX, "PCNT_RST_IN3" },
  { I2C0_SCL_PAD_IN_IDX, "I2C0_SCL_IN" }, { I2C0_SDA_PAD_IN_IDX, "I2C0_SDA_IN" },
  { I2C1_SCL_PAD_IN_IDX, "I2C1_SCL_IN" }, { I2C1_SDA_PAD_IN_IDX, "I2C1_SDA_IN" },
  { UART0_SLP_CLK_PAD_IN_IDX, "UART0_SLP_CLK_IN" }, { UART1_SLP_CLK_PAD_IN_IDX, "UART1_SLP_CLK_IN" },
  { UART2_SLP_CLK_PAD_IN_IDX, "UART2_SLP_CLK_IN" }, { UART3_SLP_CLK_PAD_IN_IDX, "UART3_SLP_CLK_IN" },
  { UART4_SLP_CLK_PAD_IN_IDX, "UART4_SLP_CLK_IN" }, { TWAI0_RX_PAD_IN_IDX, "TWAI0_RX_IN" },
  { TWAI1_RX_PAD_IN_IDX, "TWAI1_RX_IN" }, { TWAI2_RX_PAD_IN_IDX, "TWAI2_RX_IN" },
  { PWM0_SYNC0_PAD_IN_IDX, "PWM0_SYNC0_IN" }, { PWM0_SYNC1_PAD_IN_IDX, "PWM0_SYNC1_IN" },
  { PWM0_SYNC2_PAD_IN_IDX, "PWM0_SYNC2_IN" }, { PWM0_F0_PAD_IN_IDX, "PWM0_F0_IN" },
  { PWM0_F1_PAD_IN_IDX, "PWM0_F1_IN" }, { PWM0_F2_PAD_IN_IDX, "PWM0_F2_IN" },
  { PWM0_CAP0_PAD_IN_IDX, "PWM0_CAP0_IN" }, { PWM0_CAP1_PAD_IN_IDX, "PWM0_CAP1_IN" },
  { PWM0_CAP2_PAD_IN_IDX, "PWM0_CAP2_IN" }, { PWM1_SYNC0_PAD_IN_IDX, "PWM1_SYNC0_IN" },
  { PWM1_SYNC1_PAD_IN_IDX, "PWM1_SYNC1_IN" }, { PWM1_SYNC2_PAD_IN_IDX, "PWM1_SYNC2_IN" },
  { PWM1_F0_PAD_IN_IDX, "PWM1_F0_IN" }, { PWM1_F1_PAD_IN_IDX, "PWM1_F1_IN" }, { PWM1_F2_PAD_IN_IDX, "PWM1_F2_IN" },
  { PWM1_CAP0_PAD_IN_IDX, "PWM1_CAP0_IN" }, { PWM1_CAP1_PAD_IN_IDX, "PWM1_CAP1_IN" },
  { PWM1_CAP2_PAD_IN_IDX, "PWM1_CAP2_IN" }, { MII_MDI_PAD_IN_IDX, "MII_MDI_IN" },
  { EMAC_PHY_COL_PAD_IN_IDX, "EMAC_PHY_COL_IN" }, { EMAC_PHY_CRS_PAD_IN_IDX, "EMAC_PHY_CRS_IN" },
  { USB_OTG11_IDDIG_PAD_IN_IDX, "USB_OTG11_IDDIG_IN" }, { USB_OTG11_AVALID_PAD_IN_IDX, "USB_OTG11_AVALID_IN" },
  { USB_SRP_BVALID_PAD_IN_IDX, "USB_SRP_BVALID_IN" }, { USB_OTG11_VBUSVALID_PAD_IN_IDX, "USB_OTG11_VBUSVALID_IN" },
  { USB_SRP_SESSEND_PAD_IN_IDX, "USB_SRP_SESSEND_IN" }, { ULPI_CLK_PAD_IN_IDX, "ULPI_CLK_IN" },
  { USB_HSPHY_REFCLK_IN_IDX, "USB_HSPHY_REFCLK_IN" }, { SD_CARD_DETECT_N_1_PAD_IN_IDX, "SD_CARD_DETECT_N_1_IN" },
  { SD_CARD_DETECT_N_2_PAD_IN_IDX, "SD_CARD_DETECT_N_2_IN" }, { SD_CARD_INT_N_1_PAD_IN_IDX, "SD_CARD_INT_N_1_IN" },
  { SD_CARD_INT_N_2_PAD_IN_IDX, "SD_CARD_INT_N_2_IN" }, { SD_CARD_WRITE_PRT_1_PAD_IN_IDX, "SD_CARD_WRITE_PRT_1_IN" },
  { SD_CARD_WRITE_PRT_2_PAD_IN_IDX, "SD_CARD_WRITE_PRT_2_IN" },
  { SD_DATA_STROBE_1_PAD_IN_IDX, "SD_DATA_STROBE_1_IN" }, { SD_DATA_STROBE_2_PAD_IN_IDX, "SD_DATA_STROBE_2_IN" },
  { I3C_MST_SCL_PAD_IN_IDX, "I3C_MST_SCL_IN" }, { I3C_MST_SDA_PAD_IN_IDX, "I3C_MST_SDA_IN" },
  { I3C_SLV_SCL_PAD_IN_IDX, "I3C_SLV_SCL_IN" }, { I3C_SLV_SDA_PAD_IN_IDX, "I3C_SLV_SDA_IN" },
  { USB_JTAG_TDO_BRIDGE_PAD_IN_IDX, "USB_JTAG_TDO_BRIDGE_IN" }, { PCNT_SIG_CH0_PAD_IN0_IDX, "PCNT_SIG_CH0_IN0" },
  { PCNT_SIG_CH0_PAD_IN1_IDX, "PCNT_SIG_CH0_IN1" }, { PCNT_SIG_CH0_PAD_IN2_IDX, "PCNT_SIG_CH0_IN2" },
  { PCNT_SIG_CH0_PAD_IN3_IDX, "PCNT_SIG_CH0_IN3" }, { PCNT_SIG_CH1_PAD_IN0_IDX, "PCNT_SIG_CH1_IN0" },
  { PCNT_SIG_CH1_PAD_IN1_IDX, "PCNT_SIG_CH1_IN1" }, { PCNT_SIG_CH1_PAD_IN2_IDX, "PCNT_SIG_CH1_IN2" },
  { PCNT_SIG_CH1_PAD_IN3_IDX, "PCNT_SIG_CH1_IN3" }, { PCNT_CTRL_CH0_PAD_IN0_IDX, "PCNT_CTRL_CH0_IN0" },
  { PCNT_CTRL_CH0_PAD_IN1_IDX, "PCNT_CTRL_CH0_IN1" }, { PCNT_CTRL_CH0_PAD_IN2_IDX, "PCNT_CTRL_CH0_IN2" },
  { PCNT_CTRL_CH0_PAD_IN3_IDX, "PCNT_CTRL_CH0_IN3" }, { PCNT_CTRL_CH1_PAD_IN0_IDX, "PCNT_CTRL_CH1_IN0" },
  { PCNT_CTRL_CH1_PAD_IN1_IDX, "PCNT_CTRL_CH1_IN1" }, { PCNT_CTRL_CH1_PAD_IN2_IDX, "PCNT_CTRL_CH1_IN2" },
  { PCNT_CTRL_CH1_PAD_IN3_IDX, "PCNT_CTRL_CH1_IN3" }, { CAM_PCLK_PAD_IN_IDX, "CAM_PCLK_IN" },
  { CAM_H_ENABLE_PAD_IN_IDX, "CAM_H_ENABLE_IN" }, { CAM_H_SYNC_PAD_IN_IDX, "CAM_H_SYNC_IN" },
  { CAM_V_SYNC_PAD_IN_IDX, "CAM_V_SYNC_IN" }, { CAM_DATA_IN_PAD_IN0_IDX, "CAM_DATA_IN_IN0" },
  { CAM_DATA_IN_PAD_IN1_IDX, "CAM_DATA_IN_IN1" }, { CAM_DATA_IN_PAD_IN2_IDX, "CAM_DATA_IN_IN2" },
  { CAM_DATA_IN_PAD_IN3_IDX, "CAM_DATA_IN_IN3" }, { CAM_DATA_IN_PAD_IN4_IDX, "CAM_DATA_IN_IN4" },
  { CAM_DATA_IN_PAD_IN5_IDX, "CAM_DATA_IN_IN5" }, { CAM_DATA_IN_PAD_IN6_IDX, "CAM_DATA_IN_IN6" },
  { CAM_DATA_IN_PAD_IN7_IDX, "CAM_DATA_IN_IN7" }, { CAM_DATA_IN_PAD_IN8_IDX, "CAM_DATA_IN_IN8" },
  { CAM_DATA_IN_PAD_IN9_IDX, "CAM_DATA_IN_IN9" }, { CAM_DATA_IN_PAD_IN10_IDX, "CAM_DATA_IN_IN10" },
  { CAM_DATA_IN_PAD_IN11_IDX, "CAM_DATA_IN_IN11" }, { CAM_DATA_IN_PAD_IN12_IDX, "CAM_DATA_IN_IN12" },
  { CAM_DATA_IN_PAD_IN13_IDX, "CAM_DATA_IN_IN13" }, { CAM_DATA_IN_PAD_IN14_IDX, "CAM_DATA_IN_IN14" },
  { CAM_DATA_IN_PAD_IN15_IDX, "CAM_DATA_IN_IN15" }, { EMAC_PHY_RXDV_PAD_IN_IDX, "EMAC_PHY_RXDV_IN" },
  { EMAC_PHY_RXD0_PAD_IN_IDX, "EMAC_PHY_RXD0_IN" }, { EMAC_PHY_RXD1_PAD_IN_IDX, "EMAC_PHY_RXD1_IN" },
  { EMAC_PHY_RXD2_PAD_IN_IDX, "EMAC_PHY_RXD2_IN" }, { EMAC_PHY_RXD3_PAD_IN_IDX, "EMAC_PHY_RXD3_IN" },
  { EMAC_PHY_RXER_PAD_IN_IDX, "EMAC_PHY_RXER_IN" }, { EMAC_RX_CLK_PAD_IN_IDX, "EMAC_RX_CLK_IN" },
  { EMAC_TX_CLK_PAD_IN_IDX, "EMAC_TX_CLK_IN" }, { PARLIO_RX_CLK_PAD_IN_IDX, "PARLIO_RX_CLK_IN" },
  { PARLIO_TX_CLK_PAD_IN_IDX, "PARLIO_TX_CLK_IN" }, { PARLIO_RX_DATA0_PAD_IN_IDX, "PARLIO_RX_DATA0_IN" },
  { PARLIO_RX_DATA1_PAD_IN_IDX, "PARLIO_RX_DATA1_IN" }, { PARLIO_RX_DATA2_PAD_IN_IDX, "PARLIO_RX_DATA2_IN" },
  { PARLIO_RX_DATA3_PAD_IN_IDX, "PARLIO_RX_DATA3_IN" }, { PARLIO_RX_DATA4_PAD_IN_IDX, "PARLIO_RX_DATA4_IN" },
  { PARLIO_RX_DATA5_PAD_IN_IDX, "PARLIO_RX_DATA5_IN" }, { PARLIO_RX_DATA6_PAD_IN_IDX, "PARLIO_RX_DATA6_IN" },
  { PARLIO_RX_DATA7_PAD_IN_IDX, "PARLIO_RX_DATA7_IN" }, { PARLIO_RX_DATA8_PAD_IN_IDX, "PARLIO_RX_DATA8_IN" },
  { PARLIO_RX_DATA9_PAD_IN_IDX, "PARLIO_RX_DATA9_IN" }, { PARLIO_RX_DATA10_PAD_IN_IDX, "PARLIO_RX_DATA10_IN" },
  { PARLIO_RX_DATA11_PAD_IN_IDX, "PARLIO_RX_DATA11_IN" }, { PARLIO_RX_DATA12_PAD_IN_IDX, "PARLIO_RX_DATA12_IN" },
  { PARLIO_RX_DATA13_PAD_IN_IDX, "PARLIO_RX_DATA13_IN" }, { PARLIO_RX_DATA14_PAD_IN_IDX, "PARLIO_RX_DATA14_IN" },
  { PARLIO_RX_DATA15_PAD_IN_IDX, "PARLIO_RX_DATA15_IN" }, { CORE_GPIO_IN_PAD_IN0_IDX, "CORE_GPIO_IN_IN0" },
  { CORE_GPIO_IN_PAD_IN1_IDX, "CORE_GPIO_IN_IN1" }, { CORE_GPIO_IN_PAD_IN2_IDX, "CORE_GPIO_IN_IN2" },
  { CORE_GPIO_IN_PAD_IN3_IDX, "CORE_GPIO_IN_IN3" }, { CORE_GPIO_IN_PAD_IN4_IDX, "CORE_GPIO_IN_IN4" },
  { CORE_GPIO_IN_PAD_IN5_IDX, "CORE_GPIO_IN_IN5" }, { CORE_GPIO_IN_PAD_IN6_IDX, "CORE_GPIO_IN_IN6" },
  { CORE_GPIO_IN_PAD_IN7_IDX, "CORE_GPIO_IN_IN7" }, { CORE_GPIO_IN_PAD_IN8_IDX, "CORE_GPIO_IN_IN8" },
  { CORE_GPIO_IN_PAD_IN9_IDX, "CORE_GPIO_IN_IN9" }, { CORE_GPIO_IN_PAD_IN10_IDX, "CORE_GPIO_IN_IN10" },
  { CORE_GPIO_IN_PAD_IN11_IDX, "CORE_GPIO_IN_IN11" }, { CORE_GPIO_IN_PAD_IN12_IDX, "CORE_GPIO_IN_IN12" },
  { CORE_GPIO_IN_PAD_IN13_IDX, "CORE_GPIO_IN_IN13" }, { CORE_GPIO_IN_PAD_IN14_IDX, "CORE_GPIO_IN_IN14" },
  { CORE_GPIO_IN_PAD_IN15_IDX, "CORE_GPIO_IN_IN15" }, { CORE_GPIO_IN_PAD_IN16_IDX, "CORE_GPIO_IN_IN16" },
  { CORE_GPIO_IN_PAD_IN17_IDX, "CORE_GPIO_IN_IN17" }, { CORE_GPIO_IN_PAD_IN18_IDX, "CORE_GPIO_IN_IN18" },
  { CORE_GPIO_IN_PAD_IN19_IDX, "CORE_GPIO_IN_IN19" }, { CORE_GPIO_IN_PAD_IN20_IDX, "CORE_GPIO_IN_IN20" },
  { CORE_GPIO_IN_PAD_IN21_IDX, "CORE_GPIO_IN_IN21" }, { CORE_GPIO_IN_PAD_IN22_IDX, "CORE_GPIO_IN_IN22" },
  { CORE_GPIO_IN_PAD_IN23_IDX, "CORE_GPIO_IN_IN23" }, { CORE_GPIO_IN_PAD_IN24_IDX, "CORE_GPIO_IN_IN24" },
  { CORE_GPIO_IN_PAD_IN25_IDX, "CORE_GPIO_IN_IN25" }, { CORE_GPIO_IN_PAD_IN26_IDX, "CORE_GPIO_IN_IN26" },
  { CORE_GPIO_IN_PAD_IN27_IDX, "CORE_GPIO_IN_IN27" }, { CORE_GPIO_IN_PAD_IN28_IDX, "CORE_GPIO_IN_IN28" },
  { CORE_GPIO_IN_PAD_IN29_IDX, "CORE_GPIO_IN_IN29" }, { CORE_GPIO_IN_PAD_IN30_IDX, "CORE_GPIO_IN_IN30" },
  { CORE_GPIO_IN_PAD_IN31_IDX, "CORE_GPIO_IN_IN31" }, { RMT_SIG_PAD_IN0_IDX, "RMT_SIG_IN0" },
  { RMT_SIG_PAD_IN1_IDX, "RMT_SIG_IN1" }, { RMT_SIG_PAD_IN2_IDX, "RMT_SIG_IN2" },
  { RMT_SIG_PAD_IN3_IDX, "RMT_SIG_IN3" },
};

// Target specific register fields (names differ between SoCs), GPIO32-54 are in the second output register
static inline uint32_t HwdOutLevel(uint32_t pin) { return (pin < 32) ? ((GPIO.out.val >> pin) & 1) : ((GPIO.out1.val >> (pin - 32)) & 1); }
static inline bool HwdOutInv(uint32_t pin) { return GPIO.func_out_sel_cfg[pin].out_inv_sel; }
static inline bool HwdInInv(uint32_t sig) { return GPIO.func_in_sel_cfg[sig].in_inv_sel; }

// Common 2-bit clock selector used by HP_SYS_CLKRST reg_uartn_clk_src_sel, reg_rmt_clk_src_sel and reg_ledc_clk_src_sel:
// 0=XTAL 1=RC_FAST 2=PLL_F80M (see uart_ll_get_sclk, rmt_ll_get_group_clock_src, ledc_ll_get_slow_clk_sel)
const char kHwdClk3Names[] PROGMEM = "XTAL|RC_FAST|PLL_F80M|?";
static inline uint32_t HwdClk3Soc(uint32_t sel) { return (0 == sel) ? SOC_MOD_CLK_XTAL : (1 == sel) ? SOC_MOD_CLK_RC_FAST : (2 == sel) ? SOC_MOD_CLK_PLL_F80M : 0; }

// LP_IO: GPIO0-15 are LP IO 0-15 (soc/rtc_io_channel.h), LP_IOMUX.pad[n].mux_sel switches a pad to LP_IO,
// then IO_MUX/GPIO matrix settings do not apply (see rtcio_ll_function_select)
#define HWDUMP_LPIO
static_assert(SOC_RTCIO_PIN_COUNT == 16 && RTCIO_CHANNEL_0_GPIO_NUM == 0 && RTCIO_CHANNEL_15_GPIO_NUM == 15, "HwDump: unexpected LP_IO pads");
// LP IO_MUX bus clock and reset, plus the HP IOMUX APB clock that also gates lp_io (see rtcio_ll_function_select),
// LP_IOMUX / LP_GPIO are only read when enabled
static inline bool HwdLpIoOn(void) {
  return HP_SYS_CLKRST.soc_clk_ctrl3.reg_iomux_apb_clk_en && LPPERI.clk_en.ck_en_lp_iomux && !LPPERI.reset_en.rst_en_lp_iomux;
}
// Per pad mux select (1 = pad connected to LP_IO), returned as a bit mask
static inline uint32_t HwdLpIoMuxSel(void) {
  uint32_t mask = 0;
  if (!HwdLpIoOn()) { return 0; }
  for (uint32_t r = 0; r < SOC_RTCIO_PIN_COUNT; r++) {
    if (LP_IOMUX.pad[r].mux_sel) { mask |= BIT(r); }
  }
  return mask;
}
// LP_IO register layout: pad config in LP_IOMUX.pad[], pin config in LP_GPIO.pin[]
// LP_IOMUX pad field names are mapped one by one (shared names: mcu_sel fun_ie fun_wpu fun_wpd fun_drv slp_sel mcu_ie mcu_oe)
#define HWD_LPIO_MUX(r)       (LP_IOMUX.pad[r])             // LP_IOMUX_PADn_REG
#define HWD_LPIO_PIN(r)       (LP_GPIO.pin[r])              // LP_GPIO_PINn_REG
#define HWD_LPIO_MUX_F(f)     HWD_P4_LPIO_##f
#define HWD_P4_LPIO_mcu_sel   fun_sel                       // LP IO_MUX function
#define HWD_P4_LPIO_fun_ie    fun_ie
#define HWD_P4_LPIO_fun_wpu   rue                           // pull-up (see rtcio_ll_pullup_enable)
#define HWD_P4_LPIO_fun_wpd   rde                           // pull-down (see rtcio_ll_pulldown_enable)
#define HWD_P4_LPIO_fun_drv   drv
#define HWD_P4_LPIO_slp_sel   slp_sel
#define HWD_P4_LPIO_mcu_ie    slp_ie                        // input enable in sleep
#define HWD_P4_LPIO_mcu_oe    slp_oe                        // output enable in sleep
#define HWD_LPIO_PIN_F(f)     f
#define HWD_LPIO_SLP_PULL     0                             // no sleep pull-up/pull-down/drive fields in LP_IOMUX.pad[]
#define HWD_LPIO_FUNC_GPIO    1                             // LP IO_MUX function of LP_GPIO (RTCIO_LL_PIN_FUNC)
static inline uint32_t HwdLpIoOe(void) { return LP_GPIO.enable.val & 0xFFFF; }
static inline uint32_t HwdLpIoOut(void) { return LP_GPIO.out.val & 0xFFFF; }
static inline uint32_t HwdLpIoIn(void) { return LP_GPIO.in.val & 0xFFFF; }
static inline bool HwdLpIoIntClk(void) { return LP_GPIO.clk_en.reg_clk_en; }           // see rtcio_ll_enable_io_clock
// LP pad hold bits (bit n = GPIOn), see rtcio_ll_force_hold_enable: LP_SYS on rev >= 3.0, LP_IOMUX on rev < 3.0
static inline uint32_t HwdLpIoHoldMask(void) {
#if CONFIG_ESP_REV_MIN_FULL >= 300
  return LP_SYS.pad_rtc_hold_ctrl0.pad_rtc_hold_ctrl0 & 0xFFFF;
#else
  return HwdLpIoOn() ? (HAL_FORCE_READ_U32_REG_FIELD(LP_IOMUX.lp_pad_hold, reg_lp_gpio_hold) & 0xFFFF) : 0;
#endif
}
static inline int HwdRtcGpio(uint32_t r) { return (int)r; }                         // LP IO index -> GPIO number
static inline bool HwdPinIsRtc(uint32_t pin) { return pin < SOC_RTCIO_PIN_COUNT; }
static inline bool HwdPadRtcMux(uint32_t pin) { return HwdPinIsRtc(pin) && (HwdLpIoMuxSel() & BIT(pin)); }

// LEDC (low speed mode only, 4 timers, 8 channels), fields not covered by ledc_ll getters
#define HWDUMP_LEDC
#define HWDUMP_LEDC_CLK_EN                                  // HP_SYS_CLKRST_PERI_CLK_CTRL22_REG.reg_ledc_clk_en
#define HWD_LEDC_CLK_LABEL    "clk"                         // global clock shared by all timers
#define HWD_LEDC_MODE_NUM     1
static const uint8_t kHwdLedcModes[HWD_LEDC_MODE_NUM] = { LEDC_LOW_SPEED_MODE };
static_assert(LEDC_LS_SIG_OUT_PAD_OUT7_IDX == LEDC_LS_SIG_OUT_PAD_OUT0_IDX + 7, "HwDump: LEDC output signals must be contiguous");
// HP_SYS_CLKRST_PERI_CLK_CTRL22_REG.reg_ledc_clk_src_sel: 0=XTAL 1=RC_FAST 2=PLL_F80M (see ledc_ll_set_slow_clk_sel)
// Note: ledc_ll_get_slow_clk_sel() calls abort() on an invalid selector, so read the field directly
#define kHwdLedcClkNames  kHwdClk3Names
static inline bool HwdLedcBusClk(void) { return HP_SYS_CLKRST.soc_clk_ctrl3.reg_ledc_apb_clk_en && !HP_SYS_CLKRST.hp_rst_en1.reg_rst_en_ledc; }
static inline uint32_t HwdLedcClkSel(void) { return HP_SYS_CLKRST.peri_clk_ctrl22.reg_ledc_clk_src_sel; }
static inline bool HwdLedcClkEn(void) { return HP_SYS_CLKRST.peri_clk_ctrl22.reg_ledc_clk_en; }   // see ledc_ll_enable_clock
static inline uint32_t HwdLedcClkHz(uint32_t sel) {
  uint32_t hz = 0;
  uint32_t soc_clk = HwdClk3Soc(sel);
  if (soc_clk) { esp_clk_tree_src_get_freq_hz((soc_module_clk_t)soc_clk, ESP_CLK_TREE_SRC_FREQ_PRECISION_APPROX, &hz); }
  return hz;
}
// No per-timer clock mux (see ledc_ll_get_clock_source)
static inline uint32_t HwdLedcTimerClkHz(uint32_t mode, uint32_t t, uint32_t clk_hz) { return clk_hz; }
static inline bool HwdLedcTimerPaused(uint32_t mode, uint32_t t) { return LEDC.timer_group[mode].timer[t].conf.pause; }
static inline bool HwdLedcTimerRst(uint32_t mode, uint32_t t) { return LEDC.timer_group[mode].timer[t].conf.rst; }
static inline uint32_t HwdLedcTimerCnt(uint32_t mode, uint32_t t) { return LEDC.timer_group[mode].timer[t].value.cnt; }
static inline bool HwdLedcChOutEn(uint32_t mode, uint32_t ch) { return LEDC.channel_group[mode].channel[ch].conf0.sig_out_en; }
static inline uint32_t HwdLedcChIdle(uint32_t mode, uint32_t ch) { return LEDC.channel_group[mode].channel[ch].conf0.idle_lv; }
static inline uint32_t HwdLedcSig(uint32_t mode, uint32_t ch) { return LEDC_LS_SIG_OUT_PAD_OUT0_IDX + ch; }

// USB-Serial-JTAG controller on internal USB FS PHY 0 (GPIO24/25) or PHY 1 (GPIO26/27)
// Warning: never read USB_SERIAL_JTAG.ep1 (USB_SERIAL_JTAG_EP1_REG), reading it pops a byte from the RX FIFO
#define HWDUMP_USB_SERIAL_JTAG
#include "soc/usb_serial_jtag_struct.h"
static_assert(USB_INT_PHY0_DM_GPIO_NUM == 24 && USB_INT_PHY0_DP_GPIO_NUM == 25 &&
              USB_INT_PHY1_DM_GPIO_NUM == 26 && USB_INT_PHY1_DP_GPIO_NUM == 27, "HwDump: unexpected USB pins");
// Same test as usb_serial_jtag_ll_module_is_enabled()
static inline bool HwdUsjBusClk(void) { return HP_SYS_CLKRST.soc_clk_ctrl2.reg_usb_device_apb_clk_en && !LP_AON_CLKRST.hp_usb_clkrst_ctrl1.rst_en_usb_device; }
// FS PHY used by USJ: LP_SYS software override (see usb_serial_jtag_ll_phy_select), else efuse USB_PHY_SEL (0 = PHY 0)
static inline uint32_t HwdUsjPhy(void) {
  if (LP_SYS.usb_ctrl.sw_hw_usb_phy_sel) { return LP_SYS.usb_ctrl.sw_usb_phy_sel; }
  return esp_efuse_read_field_bit(ESP_EFUSE_USB_PHY_SEL) ? 1 : 0;
}
#define HWD_USJ_DM_GPIO     (HwdUsjPhy() ? USB_INT_PHY1_DM_GPIO_NUM : USB_INT_PHY0_DM_GPIO_NUM)    // USB D- / D+ pads
#define HWD_USJ_DP_GPIO     (HwdUsjPhy() ? USB_INT_PHY1_DP_GPIO_NUM : USB_INT_PHY0_DP_GPIO_NUM)
#if CONFIG_ESP_REV_MIN_FULL >= 300
#define HWD_USJ_DATE_REG    USB_DEVICE_DATE_REG             // renamed in hw_ver3
#define HWD_USJ_F(f)        serial_jtag_##f                 // hw_ver3 status field names: in_fifo_cnt -> serial_jtag_in_fifo_cnt
#else
#define HWD_USJ_DATE_REG    USB_SERIAL_JTAG_DATE_REG
#define HWD_USJ_F(f)        f
#endif

// UART (UART0-4, LP_UART is not dumped) - Warning: never read UARTn.fifo (UART_FIFO_REG), reading it pops a byte from the RX FIFO
// The HP_SYS_CLKRST sclk pre-divider (reg_uartn_sclk_div_num) is applied by uart_ll_get_baudrate()
#define HWDUMP_UART
#include "hal/uart_ll.h"
#define HwdUartDev(n)   UART_LL_GET_HW(n)                   // macro: no custom types in .ino function signatures
#define HWD_UART_CONF0      conf0_sync                      // UART_CONF0_SYNC_REG member
#define HWD_UART_CLKDIV     clkdiv_sync                     // UART_CLKDIV_SYNC_REG member and its integer / fractional fields
#define HWD_UART_DIV_INT    clkdiv
#define HWD_UART_DIV_FRAG   clkdiv_frag
static inline uint32_t HwdUartClkSel(uint32_t n) {          // see uart_ll_get_sclk
  switch (n) {
    case 0: return HP_SYS_CLKRST.peri_clk_ctrl110.reg_uart0_clk_src_sel;
    case 1: return HP_SYS_CLKRST.peri_clk_ctrl111.reg_uart1_clk_src_sel;
    case 2: return HP_SYS_CLKRST.peri_clk_ctrl112.reg_uart2_clk_src_sel;
    case 3: return HP_SYS_CLKRST.peri_clk_ctrl113.reg_uart3_clk_src_sel;
    case 4: return HP_SYS_CLKRST.peri_clk_ctrl114.reg_uart4_clk_src_sel;
  }
  return 0;
}
#define kHwdUartClkNames  kHwdClk3Names
static inline uint32_t HwdUartClkSoc(uint32_t sel) { return HwdClk3Soc(sel); }
// GPIO matrix signals: tx, rx, rts, cts and IO_MUX function names (nullptr if none)
static const uint16_t kHwdUartSig[SOC_UART_HP_NUM][4] = {
  { UART0_TXD_PAD_OUT_IDX, UART0_RXD_PAD_IN_IDX, UART0_RTS_PAD_OUT_IDX, UART0_CTS_PAD_IN_IDX },
  { UART1_TXD_PAD_OUT_IDX, UART1_RXD_PAD_IN_IDX, UART1_RTS_PAD_OUT_IDX, UART1_CTS_PAD_IN_IDX },
  { UART2_TXD_PAD_OUT_IDX, UART2_RXD_PAD_IN_IDX, UART2_RTS_PAD_OUT_IDX, UART2_CTS_PAD_IN_IDX },
  { UART3_TXD_PAD_OUT_IDX, UART3_RXD_PAD_IN_IDX, UART3_RTS_PAD_OUT_IDX, UART3_CTS_PAD_IN_IDX },
  { UART4_TXD_PAD_OUT_IDX, UART4_RXD_PAD_IN_IDX, UART4_RTS_PAD_OUT_IDX, UART4_CTS_PAD_IN_IDX },
};
static const char * const kHwdUartIomux[SOC_UART_HP_NUM][4] = {
  { "UART0_TXD", "UART0_RXD", "UART0_RTS", "UART0_CTS" },
  { "UART1_TXD", "UART1_RXD", "UART1_RTS", "UART1_CTS" },
  { nullptr, nullptr, nullptr, nullptr },
  { nullptr, nullptr, nullptr, nullptr },
  { nullptr, nullptr, nullptr, nullptr },
};

// I2C (I2C0/1, LP_I2C is not dumped) - Warning: never read I2Cn.data (I2C_DATA_REG), reading it pops a byte from the RX FIFO
// Source clock and divider are in HP_SYS_CLKRST_PERI_CLK_CTRL10/11_REG (see i2c_ll_set_source_clk, i2c_ll_set_bus_timing)
#define HWDUMP_I2C
#include "hal/i2c_ll.h"
const char kHwdI2cClkNames[] PROGMEM = "XTAL|RC_FAST";                    // reg_i2cn_clk_src_sel
#define HwdI2cDev(n)        I2C_LL_GET_HW(n)                // macro: no custom types in .ino function signatures
#define HwdI2cClkSel(hw)    (((hw) == &I2C0) ? HP_SYS_CLKRST.peri_clk_ctrl10.reg_i2c0_clk_src_sel : HP_SYS_CLKRST.peri_clk_ctrl10.reg_i2c1_clk_src_sel)
#define HwdI2cClkDiv(hw)    ((((hw) == &I2C0) ? HAL_FORCE_READ_U32_REG_FIELD(HP_SYS_CLKRST.peri_clk_ctrl10, reg_i2c0_clk_div_num) \
                                              : HAL_FORCE_READ_U32_REG_FIELD(HP_SYS_CLKRST.peri_clk_ctrl11, reg_i2c1_clk_div_num)) + 1)
#define HwdI2cIsMaster(hw)  i2c_ll_is_master_mode(hw)
#define HWD_I2C_SR(hw)      ((hw)->sr)                      // I2C_SR_REG
#define HWD_I2C_RXFIFO_CNT  rxfifo_cnt                      // I2C_SR_REG field names
#define HWD_I2C_TXFIFO_CNT  txfifo_cnt
#define HWD_I2C_INT_MASK    0x7FFFF                         // I2C_INT_RAW_REG bits 0..18
#define HWD_I2C_SCL_ADJ     0                               // i2c_ll_get_scl_timing() already returns the full high and low periods
static inline bool HwdI2cOn(uint32_t n) {
  return n ? (HP_SYS_CLKRST.soc_clk_ctrl2.reg_i2c1_apb_clk_en && !HP_SYS_CLKRST.hp_rst_en1.reg_rst_en_i2c1)
           : (HP_SYS_CLKRST.soc_clk_ctrl2.reg_i2c0_apb_clk_en && !HP_SYS_CLKRST.hp_rst_en1.reg_rst_en_i2c0);
}
static inline uint32_t HwdI2cClkSoc(uint32_t sel) { return sel ? SOC_MOD_CLK_RC_FAST : SOC_MOD_CLK_XTAL; }
static const uint16_t kHwdI2cSig[SOC_HP_I2C_NUM][4] = {       // scl_out, scl_in, sda_out, sda_in
  { I2C0_SCL_PAD_OUT_IDX, I2C0_SCL_PAD_IN_IDX, I2C0_SDA_PAD_OUT_IDX, I2C0_SDA_PAD_IN_IDX },
  { I2C1_SCL_PAD_OUT_IDX, I2C1_SCL_PAD_IN_IDX, I2C1_SDA_PAD_OUT_IDX, I2C1_SDA_PAD_IN_IDX },
};

// RMT (4 tx channels 0-3, 4 rx channels 4-7, C3 layout with "_chn" / "_chm" field suffixes)
// Warning: never read RMT.chndata[] / RMT.chmdata[] (RMT_CHnDATA_REG / RMT_CHmDATA_REG), they are the channel FIFO access ports
#define HWDUMP_RMT
#include "hal/rmt_ll.h"
static inline bool HwdRmtOn(void) { return HP_SYS_CLKRST.soc_clk_ctrl2.reg_rmt_sys_clk_en && !HP_SYS_CLKRST.hp_rst_en1.reg_rst_en_rmt; }
static_assert(RMT_SIG_PAD_OUT3_IDX == RMT_SIG_PAD_OUT0_IDX + 3 && RMT_SIG_PAD_IN3_IDX == RMT_SIG_PAD_IN0_IDX + 3, "HwDump: RMT signals must be contiguous");
#define HWD_RMT_SIG_OUT0        RMT_SIG_PAD_OUT0_IDX        // GPIO matrix signals of tx channel 0 / rx channel 0
#define HWD_RMT_SIG_IN0         RMT_SIG_PAD_IN0_IDX
#define HWD_RMT_TX_CONF(ch)     (RMT.chnconf0[ch])          // RMT_CHnCONF0_REG
#define HWD_RMT_TX_STATUS(ch)   (RMT.chnstatus[ch])         // RMT_CHnSTATUS_REG
#define HWD_RMT_RX_CONF(i)      (RMT.chmconf[i])            // RMT_CHmCONF0_REG / RMT_CHmCONF1_REG as .conf0 / .conf1
#define HWD_RMT_RX_STATUS(i)    (RMT.chmstatus[i])          // RMT_CHmSTATUS_REG
#define HWD_RMT_TX_F(f)         f##_chn                     // tx field names: mem_size -> mem_size_chn
#define HWD_RMT_RX_F(f)         f##_chm                     // rx field names: mem_size -> mem_size_chm
#define HWD_RMT_INT_MASK        0x3FFFFFFF                  // RMT_INT_RAW_REG bits 0..29
// Group clock in HP_SYS_CLKRST_PERI_CLK_CTRL22_REG (see rmt_ll_set_group_clock_src)
#define kHwdRmtClkNames  kHwdClk3Names
static inline uint32_t HwdRmtClkSoc(uint32_t sel) { return HwdClk3Soc(sel); }
static inline uint32_t HwdRmtSclkSel(void) { return HP_SYS_CLKRST.peri_clk_ctrl22.reg_rmt_clk_src_sel; }
static inline uint32_t HwdRmtSclkDiv(void) { return HAL_FORCE_READ_U32_REG_FIELD(HP_SYS_CLKRST.peri_clk_ctrl22, reg_rmt_clk_div_num) + 1; }
static inline bool HwdRmtMemForcePd(void) { return RMT.sys_conf.mem_force_pd; }        // see rmt_ll_is_mem_force_powered_down

// I2S0/1/2 (C3 layout, no FIFO register, data goes through GDMA), tx/rx clocks in HP_SYS_CLKRST_PERI_CLK_CTRL11..19_REG
#define HWDUMP_I2S
#include "soc/i2s_struct.h"
// reg_i2sn_tx/rx_clk_src_sel (see i2s_ll_get_clk_src): 0=XTAL 1=APLL 2=external 3=PLL_F160M (rev >= 3.0 only)
const char kHwdI2sClkNames[] PROGMEM = "XTAL|APLL|external|PLL_F160M";
#define HwdI2sDev(n)    (((n) == 0) ? &I2S0 : ((n) == 1) ? &I2S1 : &I2S2)   // macro: no custom types in .ino function signatures
static inline bool HwdI2sOn(uint32_t n) {
  switch (n) {
    case 0: return HP_SYS_CLKRST.soc_clk_ctrl2.reg_i2s0_apb_clk_en && !HP_SYS_CLKRST.hp_rst_en2.reg_rst_en_i2s0_apb;
    case 1: return HP_SYS_CLKRST.soc_clk_ctrl2.reg_i2s1_apb_clk_en && !HP_SYS_CLKRST.hp_rst_en2.reg_rst_en_i2s1_apb;
    case 2: return HP_SYS_CLKRST.soc_clk_ctrl2.reg_i2s2_apb_clk_en && !HP_SYS_CLKRST.hp_rst_en2.reg_rst_en_i2s2_apb;
  }
  return false;
}
static inline uint32_t HwdI2sClkSoc(uint32_t sel) {
  return (0 == sel) ? SOC_MOD_CLK_XTAL : (1 == sel) ? SOC_MOD_CLK_APLL : (3 == sel) ? SOC_MOD_CLK_PLL_F160M : 0;
}
#define HWD_I2S_BCK(c, c1)  (c)                             // tx/rx_bck_div_num is in I2S_TX/RX_CONF_REG
// No I2S core clock gate on P4 (i2s_ll_enable_core_clock is a no-op), report the APB bus clock instead
static inline bool HwdI2sClkEn(uint32_t u) { return HwdI2sOn(u); }
// reg_i2sn_mst_clk_sel: 1 = MCLK from tx clock, 0 = from rx clock (see i2s_ll_mclk_bind_to_tx_clk), returns 1 for rx
static inline uint32_t HwdI2sMclkSel(uint32_t u) {
  uint32_t mst = (0 == u) ? HP_SYS_CLKRST.peri_clk_ctrl14.reg_i2s0_mst_clk_sel :
                 (1 == u) ? HP_SYS_CLKRST.peri_clk_ctrl17.reg_i2s1_mst_clk_sel : HP_SYS_CLKRST.peri_clk_ctrl19.reg_i2s2_mst_clk_sel;
  return mst ? 0 : 1;
}
// Clock selector, enable, integer divider and fractional divider fields (see i2s_ll_tx/rx_clk_set_src, i2s_ll_tx/rx_set_raw_clk_div)
// Each register is read once into a local copy, fields are spread over consecutive peri_clk_ctrl registers
static inline void HwdI2sClkGet(uint32_t u, bool tx, uint32_t *sel, uint32_t *active, uint32_t *n,
                                uint32_t *x, uint32_t *y, uint32_t *z, uint32_t *yn1) {
  if (0 == u) {
    __typeof__(HP_SYS_CLKRST.peri_clk_ctrl11) c11;  c11.val = HP_SYS_CLKRST.peri_clk_ctrl11.val;
    __typeof__(HP_SYS_CLKRST.peri_clk_ctrl12) c12;  c12.val = HP_SYS_CLKRST.peri_clk_ctrl12.val;
    __typeof__(HP_SYS_CLKRST.peri_clk_ctrl13) c13;  c13.val = HP_SYS_CLKRST.peri_clk_ctrl13.val;
    __typeof__(HP_SYS_CLKRST.peri_clk_ctrl14) c14;  c14.val = HP_SYS_CLKRST.peri_clk_ctrl14.val;
    if (tx) {
      *sel = c13.reg_i2s0_tx_clk_src_sel; *active = c13.reg_i2s0_tx_clk_en; *n = c13.reg_i2s0_tx_div_n;
      *x = c13.reg_i2s0_tx_div_x; *y = c14.reg_i2s0_tx_div_y; *z = c14.reg_i2s0_tx_div_z; *yn1 = c14.reg_i2s0_tx_div_yn1;
    } else {
      *sel = c11.reg_i2s0_rx_clk_src_sel; *active = c11.reg_i2s0_rx_clk_en; *n = c12.reg_i2s0_rx_div_n;
      *x = c12.reg_i2s0_rx_div_x; *y = c12.reg_i2s0_rx_div_y; *z = c13.reg_i2s0_rx_div_z; *yn1 = c13.reg_i2s0_rx_div_yn1;
    }
  } else if (1 == u) {
    __typeof__(HP_SYS_CLKRST.peri_clk_ctrl14) c14;  c14.val = HP_SYS_CLKRST.peri_clk_ctrl14.val;
    __typeof__(HP_SYS_CLKRST.peri_clk_ctrl15) c15;  c15.val = HP_SYS_CLKRST.peri_clk_ctrl15.val;
    __typeof__(HP_SYS_CLKRST.peri_clk_ctrl16) c16;  c16.val = HP_SYS_CLKRST.peri_clk_ctrl16.val;
    __typeof__(HP_SYS_CLKRST.peri_clk_ctrl17) c17;  c17.val = HP_SYS_CLKRST.peri_clk_ctrl17.val;
    if (tx) {
      *sel = c15.reg_i2s1_tx_clk_src_sel; *active = c15.reg_i2s1_tx_clk_en; *n = c16.reg_i2s1_tx_div_n;
      *x = c16.reg_i2s1_tx_div_x; *y = c16.reg_i2s1_tx_div_y; *z = c17.reg_i2s1_tx_div_z; *yn1 = c17.reg_i2s1_tx_div_yn1;
    } else {
      *sel = c14.reg_i2s1_rx_clk_src_sel; *active = c14.reg_i2s1_rx_clk_en; *n = c14.reg_i2s1_rx_div_n;
      *x = c15.reg_i2s1_rx_div_x; *y = c15.reg_i2s1_rx_div_y; *z = c15.reg_i2s1_rx_div_z; *yn1 = c15.reg_i2s1_rx_div_yn1;
    }
  } else {
    __typeof__(HP_SYS_CLKRST.peri_clk_ctrl17) c17;  c17.val = HP_SYS_CLKRST.peri_clk_ctrl17.val;
    __typeof__(HP_SYS_CLKRST.peri_clk_ctrl18) c18;  c18.val = HP_SYS_CLKRST.peri_clk_ctrl18.val;
    __typeof__(HP_SYS_CLKRST.peri_clk_ctrl19) c19;  c19.val = HP_SYS_CLKRST.peri_clk_ctrl19.val;
    if (tx) {
      *sel = c18.reg_i2s2_tx_clk_src_sel; *active = c18.reg_i2s2_tx_clk_en; *n = c18.reg_i2s2_tx_div_n;
      *x = c19.reg_i2s2_tx_div_x; *y = c19.reg_i2s2_tx_div_y; *z = c19.reg_i2s2_tx_div_z; *yn1 = c19.reg_i2s2_tx_div_yn1;
    } else {
      *sel = c17.reg_i2s2_rx_clk_src_sel; *active = c17.reg_i2s2_rx_clk_en; *n = c17.reg_i2s2_rx_div_n;
      *x = c17.reg_i2s2_rx_div_x; *y = c18.reg_i2s2_rx_div_y; *z = c18.reg_i2s2_rx_div_z; *yn1 = c18.reg_i2s2_rx_div_yn1;
    }
  }
}
// mclk out, mclk in, tx bck out/in, tx ws out/in, dout, dout1, rx bck out/in, rx ws out/in, din (0xFFFF = no such signal)
static const uint16_t kHwdI2sSig[SOC_I2S_NUM][13] = {
  { I2S0_MCLK_PAD_OUT_IDX, I2S0_MCLK_PAD_IN_IDX, I2S0_O_BCK_PAD_OUT_IDX, I2S0_O_BCK_PAD_IN_IDX, I2S0_O_WS_PAD_OUT_IDX, I2S0_O_WS_PAD_IN_IDX,
    I2S0_O_SD_PAD_OUT_IDX, I2S0_O_SD1_PAD_OUT_IDX, I2S0_I_BCK_PAD_OUT_IDX, I2S0_I_BCK_PAD_IN_IDX, I2S0_I_WS_PAD_OUT_IDX, I2S0_I_WS_PAD_IN_IDX,
    I2S0_I_SD_PAD_IN_IDX },
  { I2S1_MCLK_PAD_OUT_IDX, I2S1_MCLK_PAD_IN_IDX, I2S1_O_BCK_PAD_OUT_IDX, I2S1_O_BCK_PAD_IN_IDX, I2S1_O_WS_PAD_OUT_IDX, I2S1_O_WS_PAD_IN_IDX,
    I2S1_O_SD_PAD_OUT_IDX, 0xFFFF, I2S1_I_BCK_PAD_OUT_IDX, I2S1_I_BCK_PAD_IN_IDX, I2S1_I_WS_PAD_OUT_IDX, I2S1_I_WS_PAD_IN_IDX,
    I2S1_I_SD_PAD_IN_IDX },
  { I2S2_MCLK_PAD_OUT_IDX, I2S2_MCLK_PAD_IN_IDX, I2S2_O_BCK_PAD_OUT_IDX, I2S2_O_BCK_PAD_IN_IDX, I2S2_O_WS_PAD_OUT_IDX, I2S2_O_WS_PAD_IN_IDX,
    I2S2_O_SD_PAD_OUT_IDX, 0xFFFF, I2S2_I_BCK_PAD_OUT_IDX, I2S2_I_BCK_PAD_IN_IDX, I2S2_I_WS_PAD_OUT_IDX, I2S2_I_WS_PAD_IN_IDX,
    I2S2_I_SD_PAD_IN_IDX },
};

// SPI2 / SPI3 (GPSPI2 / GPSPI3), SPI0/1 are used by flash/PSRAM and not dumped
// Warning: never read GPSPIn.data_buf[] while a transaction is ongoing (not dumped anyway)
// Module clock source and pre-dividers in HP_SYS_CLKRST_PERI_CLK_CTRL116/117_REG (see spi_ll_set_clk_source, spi_ll_clk_source_pre_div)
#define HWDUMP_SPI
#include "soc/spi_struct.h"
const char kHwdSpiClkNames[] PROGMEM = "XTAL|RC_FAST|?|?|SPLL";          // reg_gpspin_clk_src_sel
#define HWD_SPI_NUM     2
static const char * const kHwdSpiNames[HWD_SPI_NUM] = { "SPI2", "SPI3" };
#define HwdSpiDev(i)    ((i) ? &GPSPI3 : &GPSPI2)           // macro: no custom types in .ino function signatures
#define HWD_SPI_CS_NUM  6
// Bus clocks (sys + apb) and reset, see _spi_ll_enable_bus_clock / spi_ll_reset_register
static inline bool HwdSpiOn(uint32_t i) {
  return i ? (HP_SYS_CLKRST.soc_clk_ctrl1.reg_gpspi3_sys_clk_en && HP_SYS_CLKRST.soc_clk_ctrl2.reg_gpspi3_apb_clk_en && !HP_SYS_CLKRST.hp_rst_en2.reg_rst_en_spi3)
           : (HP_SYS_CLKRST.soc_clk_ctrl1.reg_gpspi2_sys_clk_en && HP_SYS_CLKRST.soc_clk_ctrl2.reg_gpspi2_apb_clk_en && !HP_SYS_CLKRST.hp_rst_en2.reg_rst_en_spi2);
}
static inline uint32_t HwdSpiClkSel(uint32_t i) { return i ? HP_SYS_CLKRST.peri_clk_ctrl116.reg_gpspi3_clk_src_sel : HP_SYS_CLKRST.peri_clk_ctrl116.reg_gpspi2_clk_src_sel; }
static inline uint32_t HwdSpiClkSoc(uint32_t sel) { return (0 == sel) ? SOC_MOD_CLK_XTAL : (1 == sel) ? SOC_MOD_CLK_RC_FAST : (4 == sel) ? SOC_MOD_CLK_SPLL : 0; }
// Functional clock = source / hs_div / mst_div (both dividers are div_num + 1)
static inline uint32_t HwdSpiClkPreDiv(uint32_t i) {
  if (i) {
    return (HAL_FORCE_READ_U32_REG_FIELD(HP_SYS_CLKRST.peri_clk_ctrl117, reg_gpspi3_hs_clk_div_num) + 1) *
           (HAL_FORCE_READ_U32_REG_FIELD(HP_SYS_CLKRST.peri_clk_ctrl117, reg_gpspi3_mst_clk_div_num) + 1);
  }
  return (HAL_FORCE_READ_U32_REG_FIELD(HP_SYS_CLKRST.peri_clk_ctrl116, reg_gpspi2_hs_clk_div_num) + 1) *
         (HAL_FORCE_READ_U32_REG_FIELD(HP_SYS_CLKRST.peri_clk_ctrl116, reg_gpspi2_mst_clk_div_num) + 1);
}
// GPIO matrix signals: sck, d (mosi), q (miso), hd, wp, cs0..cs5 (0xFFFF = no such signal, SPI3 has 3 CS)
// Note: the IDF name of the SPI3 Q output is SPI3_QO_PAD_OUT_IDX
static const uint16_t kHwdSpiSig[HWD_SPI_NUM][5 + HWD_SPI_CS_NUM] = {
  { SPI2_CK_PAD_OUT_IDX, SPI2_D_PAD_OUT_IDX, SPI2_Q_PAD_IN_IDX, SPI2_HOLD_PAD_OUT_IDX, SPI2_WP_PAD_OUT_IDX,
    SPI2_CS_PAD_OUT_IDX, SPI2_CS1_PAD_OUT_IDX, SPI2_CS2_PAD_OUT_IDX, SPI2_CS3_PAD_OUT_IDX, SPI2_CS4_PAD_OUT_IDX, SPI2_CS5_PAD_OUT_IDX },
  { SPI3_CK_PAD_OUT_IDX, SPI3_D_PAD_OUT_IDX, SPI3_Q_PAD_IN_IDX, SPI3_HOLD_PAD_OUT_IDX, SPI3_WP_PAD_OUT_IDX,
    SPI3_CS_PAD_OUT_IDX, SPI3_CS1_PAD_OUT_IDX, SPI3_CS2_PAD_OUT_IDX, 0xFFFF, 0xFFFF, 0xFFFF },
};
// IO_MUX function names: sck, d, q, hd, wp, cs0 (SPI2 on GPIO6-11 or GPIO28-33, SPI3 has no IO_MUX function)
static const char * const kHwdSpiIomux[HWD_SPI_NUM][6] = {
  { "SPI2_CK", "SPI2_D", "SPI2_Q", "SPI2_HOLD", "SPI2_WP", "SPI2_CS" },
  { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr },
};

// Sleep: PMU (wakeup enable, ext1, sleep mode pad settings) registers, no RTC_CNTL
#define HWDUMP_SLEEP
#define HWDUMP_SLEEP_EXT                                    // ext1 (PMU) wakeup, no ext0 on P4
#define HWDUMP_SLEEP_PMU_PAD_HOLD                           // pad hold in LP_SYS / HP_SYSTEM / PMU
#include "esp_sleep.h"
#include "esp_pm.h"
// PMU_SLP_WAKEUP_CNTL2_REG bits, from esp_hw_support/port/esp32p4/private_include/pmu_bit_defs.h (PMU_xxx_WAKEUP_EN)
// index = bit number
const char kHwdWakeupNames[] PROGMEM =
  "sdio|lp_core|gpio|usb|uart4|uart3|uart2|uart1|uart0|lp_gpio|lp_uart|touch|ext1|lp_timer|bod|vbat_uv|lp_core_trap|etm|lp_timer1|lp_i2s";
#define HWD_WAKEUP_BITS   20
#define HWD_WAKEUP_EXT1_EN  BIT(12)                         // PMU_EXT1_WAKEUP_EN
static inline uint32_t HwdWakeupEna(void) { return PMU.wakeup.cntl2.wakeup_ena; }   // see pmu_ll_hp_set_wakeup_enable
// GPIO0-15: LP pad hold, GPIO16-54: gpio_ll_is_digital_io_hold() (it aborts on GPIO0-15)
static inline bool HwdPadHold(uint32_t pin) {
  if (pin < SOC_RTCIO_PIN_COUNT) { return (HwdLpIoHoldMask() >> pin) & 1; }
  return gpio_ll_is_digital_io_hold(&GPIO, pin);
}
static inline uint64_t HwdPadHoldMask(void) {
  uint64_t mask = 0;
  for (uint32_t pin = 0; pin < SOC_GPIO_PIN_COUNT; pin++) {
    if (GPIO_IS_VALID_GPIO((int)pin) && HwdPadHold(pin)) { mask |= BIT64(pin); }
  }
  return mask;
}
static inline bool HwdSlpHpPadHoldAll(void) { return PMU.hp_sys[PMU_MODE_HP_SLEEP].syscntl.hp_pad_hold_all; }
static inline bool HwdSlpLpPadHoldAll(void) { return PMU.hp_sys[PMU_MODE_HP_SLEEP].syscntl.lp_pad_hold_all; }
static inline bool HwdSlpDigPadSlpSel(void) { return PMU.hp_sys[PMU_MODE_HP_SLEEP].syscntl.dig_pad_slp_sel; }
static inline uint32_t HwdGpioIntType(uint32_t pin) { return GPIO.pin[pin].int_type; }
static inline bool HwdGpioWakeupEn(uint32_t pin) { return GPIO.pin[pin].wakeup_enable; }
// Deep-sleep GPIO wakeup is the LP_GPIO pad wakeup (see rtcio_ll_wakeup_enable), "clk" = LP_GPIO clock gate
static inline bool HwdDeepSleepWakeEn(uint32_t pin) { return HwdLpIoOn() && HwdPinIsRtc(pin) && LP_GPIO.pin[pin].wakeup_enable; }
static inline uint32_t HwdDeepSleepWakeType(uint32_t pin) { return (HwdLpIoOn() && HwdPinIsRtc(pin)) ? LP_GPIO.pin[pin].int_type : 0; }
static inline bool HwdDeepSleepWakeClk(void) { return HwdLpIoOn() && LP_GPIO.clk_en.reg_clk_en; }
// ext1: mask of LP pads, one trigger level per pad (1 = high), programmed by IDF when entering sleep
// (see pmu_ll_ext1_set_wakeup_pins, pmu_ll_ext1_get_wakeup_status)
static inline uint32_t HwdExt1Mask(void) { return REG_READ(PMU_EXT_WAKEUP_SEL_REG) & 0xFFFF; }
static inline uint32_t HwdExt1HighMask(void) { return REG_READ(PMU_EXT_WAKEUP_LV_REG) & 0xFFFF; }
static inline uint32_t HwdExt1Status(void) { return REG_READ(PMU_EXT_WAKEUP_ST_REG) & 0xFFFF; }

#endif  // CONFIG_IDF_TARGET_ESP32P4
