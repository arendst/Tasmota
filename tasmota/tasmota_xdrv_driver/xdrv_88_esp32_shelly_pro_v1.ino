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
#ifdef USE_SHELLY_PRO_V1
/*********************************************************************************************\
 * Shelly Pro support
 *
 * {"NAME":"Shelly Pro 1","GPIO":[0,1,0,1,768,0,0,0,672,704,736,0,0,0,5600,6214,0,0,0,5568,0,0,0,0,0,0,0,0,0,0,0,32,4736,0,160,0],"FLAG":0,"BASE":1,"CMND":"AdcParam1 2,10000,10000,3350"}
 * {"NAME":"Shelly Pro 1PM","GPIO":[9568,1,9472,1,768,0,0,0,672,704,736,0,0,0,5600,6214,0,0,0,5568,0,0,0,0,0,0,0,0,3459,0,0,32,4736,0,160,0],"FLAG":0,"BASE":1,"CMND":"AdcParam1 2,10000,10000,3350"}
 * {"NAME":"Shelly Pro 2","GPIO":[0,1,0,1,768,0,0,0,672,704,736,0,0,0,5600,6214,0,0,0,5568,0,0,0,0,0,0,0,0,0,0,0,32,4736,4737,160,161],"FLAG":0,"BASE":1,"CMND":"AdcParam1 2,10000,10000,3350;AdcParam2 2,10000,10000,3350"}
 * {"NAME":"Shelly Pro 2PM","GPIO":[9568,1,9472,1,768,0,0,0,672,704,736,9569,0,0,5600,6214,0,0,0,5568,0,0,0,0,0,0,0,0,3460,0,0,32,4736,4737,160,161],"FLAG":0,"BASE":1,"CMND":"AdcParam1 2,10000,10000,3350;AdcParam2 2,10000,10000,3350"}
 *
 * Shelly Pro uses SPI to control one 74HC595 for relays/leds and one ADE7953 (1PM) or two ADE7953 (2PM) for energy monitoring
\*********************************************************************************************/

#define XDRV_88           88

struct SPro {
  uint32_t last_update;
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
  // OE is connected to Gnd with 470 ohm resistor R62 AND a capacitor C81 to 3V3
  // - this inhibits output of signals (also relay state) during power on for a few seconds
  uint8_t val = SPro.power | SPro.ledlink;
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  SPI.transfer(val);                            // Write 74HC595 shift register
  SPI.endTransaction();
//  delayMicroseconds(2);                         // Wait for SPI clock to stop
  digitalWrite(SPro.pin_shift595_rclk, 1);      // Latch data
  delayMicroseconds(1);                         // Shelly 10mS
  digitalWrite(SPro.pin_shift595_rclk, 0);
}

void ShellyProPreInit(void) {
  if ((SPI_MOSI_MISO == TasmotaGlobal.spi_enabled) &&
      PinUsed(GPIO_SPI_CS) &&
      TasmotaGlobal.gpio_optiona.shelly_pro) {  // Option_A7

    if (PinUsed(GPIO_SWT1) || PinUsed(GPIO_KEY1)) {
      UpdateDevicesPresent(1);                  // Shelly Pro 1

      if (PinUsed(GPIO_SWT1, 1) || PinUsed(GPIO_KEY1, 1)) {
        UpdateDevicesPresent(1);                // Shelly Pro 2
      }

      SPro.pin_shift595_rclk = Pin(GPIO_SPI_CS);
      digitalWrite(SPro.pin_shift595_rclk, 0);
      pinMode(SPro.pin_shift595_rclk, OUTPUT);
      // Does nothing if SPI is already initiated (by ADE7953) so no harm done
      SPI.begin(Pin(GPIO_SPI_CLK), Pin(GPIO_SPI_MISO), Pin(GPIO_SPI_MOSI), -1);

      SPro.ledlink = 0x18;                      // Blue led on - set by first call ShellyProPower()
      SPro.detected = true;
    }
  }
}

void ShellyProInit(void) {
  int pin_lan_reset = 5;                        // GPIO5 = LAN8720 nRST
//  delay(30);                                    // (t-purstd) This pin must be brought low for a minimum of 25 mS after power on
  digitalWrite(pin_lan_reset, 0);
  pinMode(pin_lan_reset, OUTPUT);
  delay(1);                                     // (t-rstia) This pin must be brought low for a minimum of 100 uS
  digitalWrite(pin_lan_reset, 1);

  AddLog(LOG_LEVEL_INFO, PSTR("HDW: Shelly Pro %d%s initialized"),
    TasmotaGlobal.devices_present, (PinUsed(GPIO_ADE7953_CS))?"PM":"");
}

void ShellyProPower(void) {
  SPro.power = XdrvMailbox.index &3;
  ShellyProUpdate();
}

void ShellyProUpdateLedLink(uint32_t ledlink) {
  if (ledlink != SPro.ledlink) {
    SPro.ledlink = ledlink;
    ShellyProUpdate();
  }
}

void ShellyProLedLink(void) {
  /*
  bit 2 = blue, 3 = green, 4 = red
  Shelly Pro documentation
  - Blue light indicator will be on if in AP mode.
  - Red light indicator will be on if in STA mode and not connected to a Wi-Fi network.
  - Yellow light indicator will be on if in STA mode and connected to a Wi-Fi network.
  - Green light indicator will be on if in STA mode and connected to a Wi-Fi network and to the Shelly Cloud.
  - The light indicator will be flashing Red/Blue if OTA update is in progress.
  Tasmota behaviour
  - Blue light indicator will blink if no wifi or mqtt.
  - Green light indicator will be on if in STA mode and connected to a Wi-Fi network.
  */
  SPro.last_update = TasmotaGlobal.uptime;
  uint32_t ledlink = 0x1C;                      // All leds off
  if (XdrvMailbox.index) {
    ledlink &= 0xFB;                            // Blue blinks if wifi/mqtt lost
  }
  else if (!TasmotaGlobal.global_state.wifi_down) {
    ledlink &= 0xF7;                            // Green On
  }

  ShellyProUpdateLedLink(ledlink);
}

void ShellyProLedLinkWifiOff(void) {
  /*
  bit 2 = blue, 3 = green, 4 = red
  - Green light indicator will be on if in STA mode and connected to a Wi-Fi network.
  */
  if (SPro.last_update +1 < TasmotaGlobal.uptime) {
    ShellyProUpdateLedLink((TasmotaGlobal.global_state.wifi_down) ? 0x1C : 0x14);  // Green off if wifi OFF
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv88(uint32_t function) {
  bool result = false;

  if (FUNC_PRE_INIT == function) {
    ShellyProPreInit();
  } else if (SPro.detected) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        ShellyProLedLinkWifiOff();
        break;
      case FUNC_SET_POWER:
        ShellyProPower();
        break;
      case FUNC_LED_LINK:
        ShellyProLedLink();
        break;
      case FUNC_INIT:
        ShellyProInit();
        break;
      case FUNC_ACTIVE:
        result = true;
        break;
    }
  }
  return result;
}

#endif  // USE_SHELLY_PRO
#endif  // USE_SPI
#endif  // ESP32
