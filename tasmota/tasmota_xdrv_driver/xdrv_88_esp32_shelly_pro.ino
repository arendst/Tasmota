/*
  xdrv_88_shelly_pro.ino - Shelly pro family support for Tasmota

  Copyright (C) 2022  Theo Arends

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

#ifdef ESP32
#ifdef USE_SPI
#ifdef USE_SHELLY_PRO
/*********************************************************************************************\
 * Shelly Pro support
 *
 * {"NAME":"Shelly Pro 1","GPIO":[0,0,0,0,768,0,0,0,672,704,736,0,0,0,5600,6214,0,0,0,5568,0,0,0,0,0,0,0,0,0,0,0,32,0,0,160,0],"FLAG":0,"BASE":1}
 * {"NAME":"Shelly Pro 2","GPIO":[0,0,0,0,768,0,0,0,672,704,736,0,0,0,5600,6214,0,0,0,5568,0,0,0,0,0,0,0,0,0,0,0,32,0,0,160,161],"FLAG":0,"BASE":1}
 *
 * Shelly Pro uses SPI to control one 74HC595 for relays/leds and one ADE7953 (1PM) or two ADE7953 (2PM) for energy monitoring
\*********************************************************************************************/

#define XDRV_88           88

struct SPro {
  uint8_t pin_shift595_rclk;
  uint8_t ledlink;
  uint8_t power;
  bool detected;
} SPro;

void ShellyProUpdate(void) {
  // Shelly Pro 595 register
  // bit 0 = relay/led 1
  // bit 1 = relay/led 2
  // bit 2 = wifi led blue
  // bit 3 = wifi led green
  // bit 4 = wifi led red
  // bit 5 - 7 = nc
  uint32_t val = SPro.power | SPro.ledlink;
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  SPI.transfer(val);                            // Write 74HC595 shift register
  SPI.endTransaction();

  digitalWrite(SPro.pin_shift595_rclk, 1);
  delayMicroseconds(200);                       // Shelly 10mS
  digitalWrite(SPro.pin_shift595_rclk, 0);
}

void ShellyProPreInit(void) {
  if ((SPI_MOSI_MISO == TasmotaGlobal.spi_enabled) &&
      PinUsed(GPIO_SPI_CS) &&
      TasmotaGlobal.gpio_optiona.shelly_pro) {  // Option_A7

    if (PinUsed(GPIO_SWT1)) {
      TasmotaGlobal.devices_present++;          // Shelly Pro 1
      if (PinUsed(GPIO_SWT1, 1)) {
        TasmotaGlobal.devices_present++;        // Shelly Pro 2
      }

      SPro.pin_shift595_rclk = Pin(GPIO_SPI_CS);
      pinMode(SPro.pin_shift595_rclk, OUTPUT);
      digitalWrite(SPro.pin_shift595_rclk, 0);
      // Does nothing if SPI is already initiated (by ADE7953) so no harm done
      SPI.begin(Pin(GPIO_SPI_CLK), Pin(GPIO_SPI_MISO), Pin(GPIO_SPI_MOSI), -1);

      SPro.power = TasmotaGlobal.power &3;      // Restore power
      SPro.ledlink = 0x1C;                      // All leds off
      ShellyProUpdate();

      SPro.detected = true;
    }
  }
}

void ShellyProPower(void) {
  SPro.power = XdrvMailbox.index &3;
  ShellyProUpdate();
}

/*********************************************************************************************\
 * External called functions
\*********************************************************************************************/

bool ShellyProLedLink(uint32_t state) {
  if (SPro.detected) {
    // bit 2 = blue, 3 = green, 4 = red
    SPro.ledlink = (state) ? 0x18 : 0x1C;       // Blue on (wifi link) or all off
    ShellyProUpdate();
    return true;
  }
  return false;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv88(uint8_t function) {
  bool result = false;

  if (FUNC_PRE_INIT == function) {
    ShellyProPreInit();
    } else if (SPro.detected) {
      switch (function) {
        case FUNC_SET_POWER:
          ShellyProPower();
          break;
      }
    }
  return result;
}

#endif  // USE_SHELLY_PRO
#endif  // USE_SPI
#endif  // ESP32
