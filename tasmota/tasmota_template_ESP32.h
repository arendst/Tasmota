/*
  tasmota_template_ESP32.h - template settings for Tasmota

  Copyright (C) 2020  Theo Arends

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _TASMOTA_TEMPLATE_ESP32_H_
#define _TASMOTA_TEMPLATE_ESP32_H_

#ifdef ESP32

// mqtt
#undef MQTT_LIBRARY_TYPE
#define MQTT_LIBRARY_TYPE      MQTT_PUBSUBCLIENT
// Hardware has no ESP32
#undef USE_TUYA_DIMMER
#undef USE_PWM_DIMMER
#undef USE_EXS_DIMMER
#undef USE_ARMTRONIX_DIMMERS
#undef USE_SONOFF_RF
#undef USE_SONOFF_SC
#undef USE_SONOFF_IFAN
#undef USE_SONOFF_L1
#undef USE_SONOFF_D1
#undef USE_RF_FLASH
// not ported
#undef USE_DISCOVERY
#undef USE_ADC_VCC // Needs to be ported
#undef USE_DEEPSLEEP
#undef USE_MY92X1
#undef USE_TUYA_MCU
#undef USE_I2C
#undef USE_PS_16_DZ

/********************************************************************************************/
// Supported hardware modules
enum SupportedModules {
  WEMOS,
  ESP32_CAM,
  MAXMODULE
};

const char kModuleNames[] PROGMEM =
  "WeMos D1 ESP32|ESP32 CAM|"
  ;

// Default module settings
const uint8_t kModuleNiceList[MAXMODULE] PROGMEM = {
  WEMOS,
  ESP32_CAM
};

const mytmplt kModules[MAXMODULE] PROGMEM = {
  {   // "WeMos D1 ESP32",         // Any ESP32 device like WeMos and NodeMCU hardware (ESP32)
      GPIO_USER,  //0       (I)O                GPIO0, ADC2_CH1, TOUCH1, RTC_GPIO11, CLK_OUT1, EMAC_TX_CLK
      GPIO_USER,  //1       IO     TXD0         GPIO1, U0TXD, CLK_OUT3, EMAC_RXD2
      GPIO_USER,  //2       IO                  GPIO2, ADC2_CH2, TOUCH2, RTC_GPIO12, HSPIWP, HS2_DATA0, SD_DATA0
      GPIO_USER,  //3       IO     RXD0         GPIO3, U0RXD, CLK_OUT2
      GPIO_USER,  //4       IO                  GPIO4, ADC2_CH0, TOUCH0, RTC_GPIO10, HSPIHD, HS2_DATA1, SD_DATA1, EMAC_TX_ER
      GPIO_USER,  //5       IO                  GPIO5, VSPICS0, HS1_DATA6, EMAC_RX_CLK
//      0,          //6
//      0,          //7
//      0,          //8
//      0,          //9
//      0,          //10
//      0,          //11
      GPIO_USER,  //12      (I)O                GPIO12, ADC2_CH5, TOUCH5, RTC_GPIO15, MTDI, HSPIQ, HS2_DATA2, SD_DATA2, EMAC_TXD3       (If driven High, flash voltage (VDD_SDIO) is 1.8V not default 3.3V. Has internal pull-down, so unconnected = Low = 3.3V. May prevent flashing and/or booting if 3.3V flash is connected and pulled high. See ESP32 datasheet for more details.)
      GPIO_USER,  //13      IO                  GPIO13, ADC2_CH4, TOUCH4, RTC_GPIO14, MTCK, HSPID, HS2_DATA3, SD_DATA3, EMAC_RX_ER
      GPIO_USER,  //14      IO                  GPIO14, ADC2_CH6, TOUCH6, RTC_GPIO16, MTMS, HSPICLK, HS2_CLK, SD_CLK, EMAC_TXD2
      GPIO_USER,  //15      (I)O                GPIO15, ADC2_CH3, TOUCH3, MTDO, HSPICS0, RTC_GPIO13, HS2_CMD, SD_CMD, EMAC_RXD3         (If driven Low, silences boot messages from normal boot. Has internal pull-up, so unconnected = High = normal output.)
      GPIO_USER,  //16      IO                  GPIO16, HS1_DATA4, U2RXD, EMAC_CLK_OUT
      GPIO_USER,  //17      IO                  GPIO17, HS1_DATA5, U2TXD, EMAC_CLK_OUT_180
      GPIO_USER,  //18      IO                  GPIO18, VSPICLK, HS1_DATA7
      GPIO_USER,  //19      IO                  GPIO19, VSPIQ, U0CTS, EMAC_TXD0
      0,          //20
      0,          //21      IO                  GPIO21, VSPIHD, EMAC_TX_EN
      GPIO_USER,  //22      IO      LED         GPIO22, VSPIWP, U0RTS, EMAC_TXD1
      GPIO_USER,  //23      IO                  GPIO23, VSPID, HS1_STROBE
      0,          //24
      GPIO_USER,  //25      IO                  GPIO25, DAC_1, ADC2_CH8, RTC_GPIO6, EMAC_RXD0
      GPIO_USER,  //26      IO                  GPIO26, DAC_2, ADC2_CH9, RTC_GPIO7, EMAC_RXD1
      GPIO_USER,  //27      IO                  GPIO27, ADC2_CH7, TOUCH7, RTC_GPIO17, EMAC_RX_DV
      0,          //28
      0,          //29
      0,          //30
      0,          //31
      GPIO_USER,  //32      IO                  GPIO32, XTAL_32K_P (32.768 kHz crystal oscillator input), ADC1_CH4, TOUCH9, RTC_GPIO9
      GPIO_USER,  //33      IO                  GPIO33, XTAL_32K_N (32.768 kHz crystal oscillator output), ADC1_CH5, TOUCH8, RTC_GPIO8
      GPIO_USER,  //34      I   NO PULLUP       GPIO34, ADC1_CH6, RTC_GPIO4
      GPIO_USER,  //35      I   NO PULLUP       GPIO35, ADC1_CH7, RTC_GPIO5
      GPIO_USER,  //36      I   NO PULLUP       GPIO36, SENSOR_VP, ADC_H, ADC1_CH0, RTC_GPIO0
      0,          //37          NO PULLUP
      0,          //38          NO PULLUP
      GPIO_USER   //39      I   NO PULLUP       GPIO39, SENSOR_VN, ADC1_CH3, ADC_H, RTC_GPIO3
  },
  {   //"ESP32 CAM",
      GPIO_USER,  //0       (I)O                GPIO0, ADC2_CH1, TOUCH1, RTC_GPIO11, CLK_OUT1, EMAC_TX_CLK
      GPIO_USER,  //1       IO     TXD0         GPIO1, U0TXD, CLK_OUT3, EMAC_RXD2
      GPIO_USER,  //2       IO                  GPIO2, ADC2_CH2, TOUCH2, RTC_GPIO12, HSPIWP, HS2_DATA0, SD_DATA0
      GPIO_USER,  //3       IO     RXD0         GPIO3, U0RXD, CLK_OUT2
      GPIO_USER,  //4       IO                  GPIO4, ADC2_CH0, TOUCH0, RTC_GPIO10, HSPIHD, HS2_DATA1, SD_DATA1, EMAC_TX_ER
      GPIO_USER,  //5       IO                  GPIO5, VSPICS0, HS1_DATA6, EMAC_RX_CLK
//      0,          //6
//      0,          //7
//      0,          //8
//      0,          //9
//      0,          //10
//      0,          //11
      GPIO_USER,  //12      (I)O                GPIO12, ADC2_CH5, TOUCH5, RTC_GPIO15, MTDI, HSPIQ, HS2_DATA2, SD_DATA2, EMAC_TXD3       (If driven High, flash voltage (VDD_SDIO) is 1.8V not default 3.3V. Has internal pull-down, so unconnected = Low = 3.3V. May prevent flashing and/or booting if 3.3V flash is connected and pulled high. See ESP32 datasheet for more details.)
      GPIO_USER,  //13      IO                  GPIO13, ADC2_CH4, TOUCH4, RTC_GPIO14, MTCK, HSPID, HS2_DATA3, SD_DATA3, EMAC_RX_ER
      GPIO_USER,  //14      IO                  GPIO14, ADC2_CH6, TOUCH6, RTC_GPIO16, MTMS, HSPICLK, HS2_CLK, SD_CLK, EMAC_TXD2
      GPIO_USER,  //15      (I)O                GPIO15, ADC2_CH3, TOUCH3, MTDO, HSPICS0, RTC_GPIO13, HS2_CMD, SD_CMD, EMAC_RXD3         (If driven Low, silences boot messages from normal boot. Has internal pull-up, so unconnected = High = normal output.)
      GPIO_USER,  //16      IO                  GPIO16, HS1_DATA4, U2RXD, EMAC_CLK_OUT
      GPIO_USER,  //17      IO                  GPIO17, HS1_DATA5, U2TXD, EMAC_CLK_OUT_180
      GPIO_USER,  //18      IO                  GPIO18, VSPICLK, HS1_DATA7
      GPIO_USER,  //19      IO                  GPIO19, VSPIQ, U0CTS, EMAC_TXD0
      0,          //20
      0,          //21      IO                  GPIO21, VSPIHD, EMAC_TX_EN
      GPIO_USER,  //22      IO      LED         GPIO22, VSPIWP, U0RTS, EMAC_TXD1
      GPIO_USER,  //23      IO                  GPIO23, VSPID, HS1_STROBE
      0,          //24
      GPIO_USER,  //25      IO                  GPIO25, DAC_1, ADC2_CH8, RTC_GPIO6, EMAC_RXD0
      GPIO_USER,  //26      IO                  GPIO26, DAC_2, ADC2_CH9, RTC_GPIO7, EMAC_RXD1
      GPIO_USER,  //27      IO                  GPIO27, ADC2_CH7, TOUCH7, RTC_GPIO17, EMAC_RX_DV
      0,          //28
      0,          //29
      0,          //30
      0,          //31
      GPIO_USER,  //32      IO                  GPIO32, XTAL_32K_P (32.768 kHz crystal oscillator input), ADC1_CH4, TOUCH9, RTC_GPIO9
      GPIO_USER,  //33      IO                  GPIO33, XTAL_32K_N (32.768 kHz crystal oscillator output), ADC1_CH5, TOUCH8, RTC_GPIO8
      GPIO_USER,  //34      I   NO PULLUP       GPIO34, ADC1_CH6, RTC_GPIO4
      GPIO_USER,  //35      I   NO PULLUP       GPIO35, ADC1_CH7, RTC_GPIO5
      GPIO_USER,  //36      I   NO PULLUP       GPIO36, SENSOR_VP, ADC_H, ADC1_CH0, RTC_GPIO0
      0,          //37          NO PULLUP
      0,          //38          NO PULLUP
      GPIO_USER   //39      I   NO PULLUP       GPIO39, SENSOR_VN, ADC1_CH3, ADC_H, RTC_GPIO3
  }
};

#endif  // ESP32

#endif  // _TASMOTA_TEMPLATE_ESP32_H_
