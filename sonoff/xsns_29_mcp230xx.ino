/*
  xsns_29_mcp230xx.ino - Support for I2C MCP23008/MCP23017 GPIO Expander (INPUT ONLY!)

  Copyright (C) 2018  Andre Thomas and Theo Arends

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

#ifdef USE_I2C
#ifdef USE_MCP230xx

/*********************************************************************************************\
   MCP23008/17 - I2C GPIO EXPANDER

   Docs at https://www.microchip.com/wwwproducts/en/MCP23008
           https://www.microchip.com/wwwproducts/en/MCP23017

   I2C Address: 0x20 - 0x27
  \*********************************************************************************************/

#define XSNS_29                   29

#define MCP230xx_ADDRESS1         0x20
#define MCP230xx_ADDRESS2         0x21
#define MCP230xx_ADDRESS3         0x22
#define MCP230xx_ADDRESS4         0x23
#define MCP230xx_ADDRESS5         0x24
#define MCP230xx_ADDRESS6         0x25
#define MCP230xx_ADDRESS7         0x26
#define MCP230xx_ADDRESS8         0x27

/*
   Default register locations for MCP23008 - They change for MCP23017 in default bank mode
*/

uint8_t MCP230xx_IODIR          = 0x00;
uint8_t MCP230xx_GPINTEN        = 0x02;
uint8_t MCP230xx_IOCON          = 0x05;
uint8_t MCP230xx_GPPU           = 0x06;
uint8_t MCP230xx_INTF           = 0x07;
uint8_t MCP230xx_INTCAP         = 0x08;
uint8_t MCP230xx_GPIO           = 0x09;

uint8_t mcp230xx_type = 0;
uint8_t mcp230xx_address;
uint8_t mcp230xx_addresses[] = { MCP230xx_ADDRESS1, MCP230xx_ADDRESS2, MCP230xx_ADDRESS3, MCP230xx_ADDRESS4, MCP230xx_ADDRESS5, MCP230xx_ADDRESS6, MCP230xx_ADDRESS7, MCP230xx_ADDRESS8 };
uint8_t mcp230xx_pincount = 0;
uint8_t mcp230xx_int_en = 0;

const char MCP230XX_SENSOR_RESPONSE[] PROGMEM = "{\"Sensor29\":{\"D\":%i,\"MODE\":%i,\"PULL-UP\":%i,\"STATE\":%i}}";

uint8_t MCP230xx_readGPIO(uint8_t port) {
  return I2cRead8(mcp230xx_address, MCP230xx_GPIO + port);
}

void MCP230xx_ApplySettings(void) {
  uint8_t int_en = 0;
  for (uint8_t mcp230xx_port=0;mcp230xx_port<mcp230xx_type;mcp230xx_port++) {
    uint8_t reg_gppu = 0;
    uint8_t reg_gpinten = 0;
    uint8_t reg_iodir = 0xFF;
    for (uint8_t idx = 0; idx < 8; idx++) {
      switch (Settings.mcp230xx_config[idx+(mcp230xx_port*8)].pinmode) {
        case 0 ... 1:
          reg_iodir |= (1 << idx);
          break;
        case 2 ... 4:
          reg_iodir |= (1 << idx);
          reg_gpinten |= (1 << idx);
          int_en=1;
          break;
        default:
          break;
      }
      if (Settings.mcp230xx_config[idx+(mcp230xx_port*8)].pullup) {
        reg_gppu |= (1 << idx);
      }
    }
    I2cWrite8(mcp230xx_address, MCP230xx_GPPU+mcp230xx_port, reg_gppu);
    I2cWrite8(mcp230xx_address, MCP230xx_GPINTEN+mcp230xx_port, reg_gpinten);
    I2cWrite8(mcp230xx_address, MCP230xx_IODIR+mcp230xx_port, reg_iodir);
  }
  mcp230xx_int_en=int_en;
}

void MCP230xx_Detect()
{
  uint8_t buffer;

  if (mcp230xx_type) {
    return;
  }

  for (byte i = 0; i < sizeof(mcp230xx_addresses); i++) {
    mcp230xx_address = mcp230xx_addresses[i];
    I2cWrite8(mcp230xx_address, MCP230xx_IOCON, 0x80); // attempt to set bank mode - this will only work on MCP23017, so its the best way to detect the different chips 23008 vs 23017
    if (I2cValidRead8(&buffer, mcp230xx_address, MCP230xx_IOCON)) {
      if (buffer == 0x00) {
        mcp230xx_type = 1; // We have a MCP23008
        snprintf_P(log_data, sizeof(log_data), S_LOG_I2C_FOUND_AT, "MCP23008", mcp230xx_address);
        AddLog(LOG_LEVEL_DEBUG);
        mcp230xx_pincount = 8;
        MCP230xx_ApplySettings();
      } else {
        if (buffer == 0x80) {
          mcp230xx_type = 2; // We have a MCP23017
          snprintf_P(log_data, sizeof(log_data), S_LOG_I2C_FOUND_AT, "MCP23017", mcp230xx_address);
          AddLog(LOG_LEVEL_DEBUG);
          mcp230xx_pincount = 16;
          // Reset bank mode to 0
          I2cWrite8(mcp230xx_address, MCP230xx_IOCON, 0x00);
          // Update register locations for MCP23017
          MCP230xx_GPINTEN        = 0x04;
          MCP230xx_GPPU           = 0x0C;
          MCP230xx_INTF           = 0x0E;
          MCP230xx_INTCAP         = 0x10;
          MCP230xx_GPIO           = 0x12;
          MCP230xx_ApplySettings();
        }
      }
      break;
    }
  }
}

bool MCP230xx_CheckForInterrupt(void) {
  uint8_t intf;
  uint8_t mcp230xx_intcap = 0;
  uint8_t report_int;
  for (uint8_t mcp230xx_port=0;mcp230xx_port<mcp230xx_type;mcp230xx_port++) {
    if (I2cValidRead8(&intf,mcp230xx_address,MCP230xx_INTF+mcp230xx_port)) {
      if (intf > 0) {
        if (I2cValidRead8(&mcp230xx_intcap, mcp230xx_address, MCP230xx_INTCAP+mcp230xx_port)) {
          for (uint8_t intp = 0; intp < 8; intp++) {
            if ((intf >> intp) & 0x01) { // we know which pin caused interrupt
              report_int = 0;
              if (Settings.mcp230xx_config[intp+(mcp230xx_port*8)].pinmode > 1) {
                switch (Settings.mcp230xx_config[intp+(mcp230xx_port*8)].pinmode) {
                  case 2:
                    report_int = 1;
                    break;
                  case 3:
                    if (((mcp230xx_intcap >> intp) & 0x01) == 0) report_int = 1; // Int on LOW
                    break;
                  case 4:
                    if (((mcp230xx_intcap >> intp) & 0x01) == 1) report_int = 1; // Int on HIGH
                    break;
                  default:
                    break;
                }
                if (report_int) {
                  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_JSON_TIME "\":\"%s\""), GetDateAndTime(DT_LOCAL).c_str());
                  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"MCP230XX_INT\":{\"D%i\":%i}"), mqtt_data, intp+(mcp230xx_port*8), ((mcp230xx_intcap >> intp) & 0x01));
                  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}"), mqtt_data);
                  MqttPublishPrefixTopic_P(RESULT_OR_STAT, mqtt_data);
                  char command[18];
                  sprintf(command,"event MCPINTD%i=%i",intp+(mcp230xx_port*8),((mcp230xx_intcap >> intp) & 0x01));
                  ExecuteCommand(command, SRC_RULE);
                }
              }
            }
          }
        }
      }
    }
  }
}

void MCP230xx_Show(boolean json)
{
  if (mcp230xx_type) {
    if (json) {
      if (mcp230xx_type > 0) { // we have at least 8 pins
        uint8_t gpio = MCP230xx_readGPIO(0);
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"MCP230XX\":{\"D0\":%i,\"D1\":%i,\"D2\":%i,\"D3\":%i,\"D4\":%i,\"D5\":%i,\"D6\":%i,\"D7\":%i"),
                   mqtt_data,(gpio>>0)&1,(gpio>>1)&1,(gpio>>2)&1,(gpio>>3)&1,(gpio>>4)&1,(gpio>>5)&1,(gpio>>6)&1,(gpio>>7)&1);
        if (mcp230xx_type == 2) {
          gpio = MCP230xx_readGPIO(1);
          snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"D8\":%i,\"D9\":%i,\"D10\":%i,\"D11\":%i,\"D12\":%i,\"D13\":%i,\"D14\":%i,\"D15\":%i"),
                     mqtt_data,(gpio>>0)&1,(gpio>>1)&1,(gpio>>2)&1,(gpio>>3)&1,(gpio>>4)&1,(gpio>>5)&1,(gpio>>6)&1,(gpio>>7)&1);
        }
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s%s"),mqtt_data,"}");
      }
    }
  }
}

void MCP230xx_Reset(uint8_t pinmode) {
  uint8_t pullup = 0;
  if ((pinmode > 1) && (pinmode < 5)) pullup=1;
  for (uint8_t pinx=0;pinx<16;pinx++) {
    Settings.mcp230xx_config[pinx].pinmode=pinmode;
    Settings.mcp230xx_config[pinx].pullup=pullup;
    Settings.mcp230xx_config[pinx].b4=0;
    Settings.mcp230xx_config[pinx].b5=0;
    Settings.mcp230xx_config[pinx].b6=0;
    Settings.mcp230xx_config[pinx].b7=0;
  }
  MCP230xx_ApplySettings();
  snprintf_P(mqtt_data, sizeof(mqtt_data), MCP230XX_SENSOR_RESPONSE,99,pinmode,pullup,99);
}

bool MCP230xx_Command(void) {
  boolean serviced = true;
  uint8_t _a, _b = 0;
  uint8_t pin, pinmode, pullup = 0;
  String data = XdrvMailbox.data;
  data.toUpperCase();
  if (data == "RESET") { MCP230xx_Reset(1); return serviced; }
  if (data == "RESET1") { MCP230xx_Reset(1); return serviced; }
  if (data == "RESET2") { MCP230xx_Reset(2); return serviced; }
  if (data == "RESET3") { MCP230xx_Reset(3); return serviced; }
  if (data == "RESET4") { MCP230xx_Reset(4); return serviced; }
  _a = data.indexOf(",");
  pin = data.substring(0, _a).toInt();
  if (pin < mcp230xx_pincount) {
    String cmnd = data.substring(_a+1);
    if (cmnd == "?") {
      uint8_t port = 0;
      if (pin > 7) port = 1;
      uint8_t portdata = MCP230xx_readGPIO(port);
      snprintf_P(mqtt_data, sizeof(mqtt_data), MCP230XX_SENSOR_RESPONSE,pin,Settings.mcp230xx_config[pin].pinmode,Settings.mcp230xx_config[pin].pullup,portdata>>(pin-(port*8))&1);
      return serviced;
    }
  }
  _b = data.indexOf(",", _a + 1);
  if (_a < XdrvMailbox.data_len) {
    if (_b < XdrvMailbox.data_len) {
      pinmode = data.substring(_a+1, _b).toInt();
      pullup = data.substring(_b+1, XdrvMailbox.data_len).toInt();
      if ((pin < mcp230xx_pincount) && (pinmode < 5) && (pullup < 2)) {
        Settings.mcp230xx_config[pin].pinmode=pinmode;
        Settings.mcp230xx_config[pin].pullup=pullup;
        MCP230xx_ApplySettings();
        uint8_t port = 0;
        if (pin > 7) port = 1;
        uint8_t portdata = MCP230xx_readGPIO(port);
        snprintf_P(mqtt_data, sizeof(mqtt_data), MCP230XX_SENSOR_RESPONSE,pin,pinmode,pullup,portdata>>(pin-(port*8))&1);
      } else {
        serviced = false;
      }
    } else {
      serviced = false;
    }
  } else {
    serviced = false;
  }
  return serviced;
}

/*********************************************************************************************\
   Interface
  \*********************************************************************************************/

boolean Xsns29(byte function)
{
  boolean result = false;

  if (i2c_flg) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        MCP230xx_Detect();
        break;
      case FUNC_EVERY_50_MSECOND:
        if (mcp230xx_int_en) {          // Only check for interrupts if its enabled on one of the pins
          MCP230xx_CheckForInterrupt();
        }
        break;
      case FUNC_JSON_APPEND:
        MCP230xx_Show(1);
        break;
      case FUNC_COMMAND:
        if (XSNS_29 == XdrvMailbox.index) {
          result = MCP230xx_Command();
        }
        break;
      default:
        break;
    }
  }
  return result;
}

#endif  // USE_MCP230xx
#endif  // USE_I2C

