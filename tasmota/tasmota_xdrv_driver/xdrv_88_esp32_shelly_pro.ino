/*
  xdrv_88_esp32_shelly_pro.ino - Shelly pro family support for Tasmota

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
#ifdef USE_MCP23XXX_DRV
#ifdef USE_SHELLY_PRO
/*********************************************************************************************\
 * Shelly Pro support
 *
 * {"NAME":"Shelly Pro 1","GPIO":[0,1,0,1,768,0,0,0,672,704,736,0,0,0,5600,6214,0,0,0,5568,0,0,0,0,0,0,0,0,0,0,0,32,4736,0,160,0],"FLAG":0,"BASE":1,"CMND":"AdcParam1 2,5600,4700,3350"}
 * {"NAME":"Shelly Pro 1PM","GPIO":[9568,1,9472,1,768,0,0,0,672,704,736,0,0,0,5600,6214,0,0,0,5568,0,0,0,0,0,0,0,0,3459,0,0,32,4736,0,160,0],"FLAG":0,"BASE":1,"CMND":"AdcParam1 2,5600,4700,3350"}
 * {"NAME":"Shelly Pro 2","GPIO":[0,1,0,1,768,0,0,0,672,704,736,0,0,0,5600,6214,0,0,0,5568,0,0,0,0,0,0,0,0,0,0,0,32,4736,4737,160,161],"FLAG":0,"BASE":1,"CMND":"AdcParam1 2,5600,4700,3350;AdcParam2 2,5600,4700,3350"}
 * {"NAME":"Shelly Pro 2PM","GPIO":[9568,1,9472,1,768,0,0,0,672,704,736,9569,0,0,5600,6214,0,0,0,5568,0,0,0,0,0,0,0,0,3460,0,0,32,4736,4737,160,161],"FLAG":0,"BASE":1,"CMND":"AdcParam1 2,5600,4700,3350;AdcParam2 2,5600,4700,3350"}
 * {"NAME":"Shelly Pro 4PM","GPIO":[0,6210,0,6214,9568,0,0,0,0,0,9569,0,10272,0,5600,0,0,0,0,5568,0,0,0,0,0,0,0,0,736,704,3461,10240,4736,0,0,672],"FLAG":0,"BASE":1,"CMND":"AdcParam1 2,5600,4700,3350;rule3 on file#mcp23x.dat do {\"NAME\":\"MCP23S17 Shelly Pro 4PM\",\"GPIO\":[194,193,65,66,3840,64,192,0,224,0,0,0,227,225,226,195]} endon"}
 *
 * Shelly Pro 1/2 uses SPI to control one 74HC595 for relays/leds and one ADE7953 (1PM) or two ADE7953 (2PM) for energy monitoring
 * Shelly Pro 4 uses an SPI to control one MCP23S17 for buttons/switches/relays/leds and two ADE7953 for energy monitoring and a second SPI for the display
 *   To use display enable defines USE_DISPLAY, USE_UNIVERSAL_DISPLAY and SHOW_SPLASH. Load file ST7735S_Pro4PM_display.ini as display.ini
\*********************************************************************************************/

#define XDRV_88                        88

#define SHELLY_PRO_PIN_LAN8720_RESET   5

struct SPro {
  uint32_t last_update;
  uint16_t input_state;
  int8_t switch_offset;
  int8_t button_offset;
  uint8_t pin_register_cs;
  uint8_t ledlink;
  uint8_t power;
  bool init_done;
  uint8_t detected;
} SPro;

/*********************************************************************************************\
 * Shelly Pro 4
\*********************************************************************************************/

void ShellyPro4Init(void) {
  /*
  Shelly Pro 4PM MCP23S17 registers
   bit 0 = input, inverted - Switch3
   bit 1 = input, inverted - Switch2
   bit 2 = input - Button Down
   bit 3 = input - Button OK
   bit 4 = output - Reset, display, ADE7953
   bit 5 = input - Button Up
   bit 6 = input, inverted - Switch1
   bit 7
   bit 8 = output - Relay O1
   bit 9
   bit 10
   bit 11
   bit 12 = output - Relay O4
   bit 13 = output - Relay O2
   bit 14 = output - Relay O3
   bit 15 = input, inverted - Switch4
  */
}

void ShellyPro4Reset(void) {
  MCP23xPinMode(4, OUTPUT);                         // Safeguard as also performed by MCP23x.dat template as Output_Hi
  MCP23xDigitalWrite(4, 0);                         // Reset pin display, ADE7953
  delay(1);                                         // To initiate a hardware reset, this pin must be brought low for a minimum of 10 μs.
  MCP23xDigitalWrite(4, 1);
}

bool ShellyProButton(void) {
  if (SPro.detected != 4) { return false; }         // Only support Shelly Pro 4

  uint32_t button_index = XdrvMailbox.index - SPro.button_offset;
  if (button_index > 2) { return false; }           // Only support Up, Down, Ok

  uint32_t button = XdrvMailbox.payload;
  uint32_t last_state = (XdrvMailbox.command_code & 0xFF);
  if ((PRESSED == button) && (NOT_PRESSED == last_state)) {  // Button pressed

    AddLog(LOG_LEVEL_DEBUG, PSTR("SHP: Button %d pressed"), button_index +1);

    // Do something with the Up,Down,Ok button
    switch (button_index) {
      case 0:  // Up
        break;
      case 1:  // Down
        break;
      case 2:  // Ok
        break;
    }
  }
  return true;                                      // Disable further button processing
}

/*********************************************************************************************\
 * Shelly Pro 1/2
\*********************************************************************************************/

void ShellyProUpdate(void) {
  /*
  Shelly Pro 1/2/PM 74HC595 register
   bit 0 = relay/led 1
   bit 1 = relay/led 2
   bit 2 = wifi led blue
   bit 3 = wifi led green
   bit 4 = wifi led red
   bit 5 - 7 = nc
   OE is connected to Gnd with 470 ohm resistor R62 AND a capacitor C81 to 3V3
   - this inhibits output of signals (also relay state) during power on for a few seconds
  */
  uint8_t val = SPro.power | SPro.ledlink;
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  SPI.transfer(val);                                // Write 74HC595 shift register
  SPI.endTransaction();
//  delayMicroseconds(2);                             // Wait for SPI clock to stop
  digitalWrite(SPro.pin_register_cs, 1);            // Latch data
  delayMicroseconds(1);                             // Shelly 10mS
  digitalWrite(SPro.pin_register_cs, 0);
}

/*********************************************************************************************\
 * Shelly Pro
\*********************************************************************************************/

void ShellyProPreInit(void) {
  if ((SPI_MOSI_MISO == TasmotaGlobal.spi_enabled) &&
      (PinUsed(GPIO_SPI_CS) || PinUsed(GPIO_MCP23SXX_CS)) && // 74HC595 rclk / MCP23S17
      TasmotaGlobal.gpio_optiona.shelly_pro) {      // Option_A7

    if (PinUsed(GPIO_SWT1) || PinUsed(GPIO_KEY1)) {
      SPro.detected = 1;                            // Shelly Pro 1
      if (PinUsed(GPIO_SWT1, 1) || PinUsed(GPIO_KEY1, 1)) {
        SPro.detected = 2;                          // Shelly Pro 2
      }
      UpdateDevicesPresent(SPro.detected);          // Shelly Pro 1/2
      SPro.pin_register_cs = Pin(GPIO_SPI_CS);
      digitalWrite(SPro.pin_register_cs, 0);        // Prep 74HC595 rclk
      pinMode(SPro.pin_register_cs, OUTPUT);
      SPro.ledlink = 0x18;                          // Blue led on - set by first call ShellyProPower() - Shelly 1/2
      // Does nothing if SPI is already initiated (by ADE7953) so no harm done
      SPI.begin(Pin(GPIO_SPI_CLK), Pin(GPIO_SPI_MISO), Pin(GPIO_SPI_MOSI), -1);
    }
    if (PinUsed(GPIO_MCP23SXX_CS)) {
      SPro.detected = 4;                            // Shelly Pro 4PM (No SWT or KEY)
    }
  }
}

void ShellyProInit(void) {
  int pin_lan_reset = SHELLY_PRO_PIN_LAN8720_RESET;  // GPIO5 = LAN8720 nRST
//  delay(30);                                        // (t-purstd) This pin must be brought low for a minimum of 25 mS after power on
  digitalWrite(pin_lan_reset, 0);
  pinMode(pin_lan_reset, OUTPUT);
  delay(1);                                         // (t-rstia) This pin must be brought low for a minimum of 100 uS
  digitalWrite(pin_lan_reset, 1);

  AddLog(LOG_LEVEL_INFO, PSTR("HDW: Shelly Pro %d%s initialized"),
    SPro.detected, (PinUsed(GPIO_ADE7953_CS))?"PM":"");

  SPro.init_done = true;
}

void ShellyProPower(void) {
  if (SPro.detected != 4) {
    SPro.power = XdrvMailbox.index &3;
    ShellyProUpdate();
  }
}

void ShellyProUpdateLedLink(uint32_t ledlink) {
  if (ledlink != SPro.ledlink) {
    SPro.ledlink = ledlink;
    ShellyProUpdate();
  }
}

void ShellyProLedLink(void) {
  if (!SPro.init_done) { return; }  // Block write before first power update
  if (SPro.detected != 4) {
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
    uint32_t ledlink = 0x1C;                        // All leds off
    if (XdrvMailbox.index) {
      ledlink &= 0xFB;                              // Blue blinks if wifi/mqtt lost
    }
    else if (!TasmotaGlobal.global_state.wifi_down) {
      ledlink &= 0xF7;                              // Green On
    }
    ShellyProUpdateLedLink(ledlink);
  }
}

void ShellyProLedLinkWifiOff(void) {
  if (!SPro.init_done) { return; }
  if (SPro.detected != 4) {
    /*
    bit 2 = blue, 3 = green, 4 = red
    - Green light indicator will be on if in STA mode and connected to a Wi-Fi network.
    */
    if (SPro.last_update +1 < TasmotaGlobal.uptime) {
      ShellyProUpdateLedLink((TasmotaGlobal.global_state.wifi_down) ? 0x1C : 0x14);  // Green off if wifi OFF
    }
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
/*
      case FUNC_BUTTON_PRESSED:
        result = ShellyProButton();
        break;
*/
      case FUNC_EVERY_SECOND:
        ShellyProLedLinkWifiOff();
        break;
      case FUNC_SET_POWER:
        ShellyProPower();
        break;
      case FUNC_INIT:
        ShellyProInit();
        break;
      case FUNC_LED_LINK:
        ShellyProLedLink();
        break;
      case FUNC_ACTIVE:
        result = true;
        break;
    }
  }
  return result;
}

#endif  // USE_SHELLY_PRO
#endif  // USE_MCP23XXX_DRV
#endif  // USE_SPI
#endif  // ESP32
