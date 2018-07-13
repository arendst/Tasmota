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
uint8_t mcp280xx_pincount = 0;

const char MCP230XX_SENSOR_RESPONSE[] PROGMEM = "{\"Sensor29\":{\"D\":%i,\"MODE\":%i,\"PULL-UP\":%i}}";

#ifdef USE_WEBSERVER
#ifdef USE_MCP230xx_displaymain
const char HTTP_SNS_MCP230xx_GPIO[] PROGMEM = "%s{s}MCP230XX D%d{m}%d{e}";                               // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif // USE_MCP230xx_displaymain
#ifdef USE_MCP230xx_webconfig
const char HTTP_FORM_I2C_MCP230XX_T[] PROGMEM = "<table>";
const char HTTP_FORM_I2C_MCP230XX_TE[] PROGMEM = "</table>";

const char HTTP_FORM_MCP230XX[] PROGMEM =
  "<fieldset><legend><b>&nbsp;MCP230xx settings &nbsp;</b></legend><form method='post' action='sv'><input id='w' name='w' value='8' hidden>";

const char HTTP_FORM_I2C_MCP230XX[] PROGMEM =
  "<tr><td nowrap>{b0 </b> <br/></td><td nowrap><select id='{b1' name='{b1'>"
  "<option value='0'{s0>Disabled</option>"
  "<option value='1'{s1>Input</option>"
  "<option value='2'{s2>Input (Int on Change)</option>"
  "<option value='3'{s3>Input (Int when Low)</option>"
  "<option value='4'{s4>Input (Int when High)</option>"
  "</select></td>"
  "<td nowrap>Enable Pullup</td>"
  "<td nowrap><input type=checkbox name=epu{b1 value=1{b2></input></td>"
  "</tr>";

void handleMCP230xx()
{
  if (HttpUser()) {
    return;
  }

  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_CONFIGURE_MCP230XX));

  String page = FPSTR(HTTP_HEAD);

  page.replace(F("{v}"), D_CONFIGURE_MCP230XX);

  page += FPSTR(HTTP_HEAD_STYLE);
  page += FPSTR(HTTP_FORM_MCP230XX);

  page += FPSTR(HTTP_FORM_I2C_MCP230XX_T);

  for (uint8_t idx = 0; idx < mcp280xx_pincount; idx++) {
    page += FPSTR(HTTP_FORM_I2C_MCP230XX);
    page.replace(F("{b0"), "MCP230XX D" + String(idx));
    page.replace(F("{b1"), "D" + String(idx));

    // determine correct dropdown state

    uint8_t bitsetting = 0;                           // Default to disabled
    if (Settings.mcp230xx_config[idx].enable) {
      bitsetting = 1;                                 // Default to normal enable (floating without interrupt)
      if (Settings.mcp230xx_config[idx].inten) {      // Int choice
        bitsetting = 2;                               // Default to INT on Change (LOW to HIGH, and HIGH to LOW)
        if (Settings.mcp230xx_config[idx].intmode) {    // On comparison
          bitsetting = 3;                               // On comparison default to LOW
          if (Settings.mcp230xx_config[idx].intcomp) {
            bitsetting = 4;                             // On comparison default to HIGH
          }
        }
      }
    }
    switch (bitsetting) {
      case 0 : page.replace(F("{s0"), PSTR(" selected")); break;
      case 1 : page.replace(F("{s1"), PSTR(" selected")); break;
      case 2 : page.replace(F("{s2"), PSTR(" selected")); break;
      case 3 : page.replace(F("{s3"), PSTR(" selected")); break;
      case 4 : page.replace(F("{s4"), PSTR(" selected")); break;
    }
    // replace remaining unselected options - if one was replaced above it will be ignored
    page.replace(F("{s0"), PSTR(""));
    page.replace(F("{s1"), PSTR(""));
    page.replace(F("{s2"), PSTR(""));
    page.replace(F("{s3"), PSTR(""));
    page.replace(F("{s4"), PSTR(""));

    if (Settings.mcp230xx_config[idx].pullup) {
      page.replace(F("{b2"), PSTR(" checked"));
    } else {
      page.replace(F("{b2"), PSTR(""));
    }
  }

  page += FPSTR(HTTP_FORM_I2C_MCP230XX_TE);

  page += FPSTR(HTTP_FORM_END);
  page += FPSTR(HTTP_BTN_CONF);
  ShowPage(page);
}

void MCP230xx_SaveSettings()
{
  char stemp[8];
  for (uint8_t idx = 0; idx < mcp280xx_pincount; idx++) {
    snprintf_P(stemp, sizeof(stemp), PSTR("D%d"), idx);
    uint8_t _pinvalue = (!strlen(WebServer->arg(stemp).c_str() )) ?  0 : atoi(WebServer->arg(stemp).c_str() );
    if (_pinvalue) {
      Settings.mcp230xx_config[idx].enable = 1;
      if (_pinvalue >= 2) {
        Settings.mcp230xx_config[idx].inten = 1;
        if (_pinvalue >= 3) {
          Settings.mcp230xx_config[idx].intmode = 1;
          if (_pinvalue >= 4) {
            Settings.mcp230xx_config[idx].intcomp = 1;
          } else {
            Settings.mcp230xx_config[idx].intcomp = 0;
          }
        } else {
          Settings.mcp230xx_config[idx].intmode = 0;
          Settings.mcp230xx_config[idx].intcomp = 0;
        }
      } else {
        Settings.mcp230xx_config[idx].inten = 0;
        Settings.mcp230xx_config[idx].intmode = 0;
        Settings.mcp230xx_config[idx].intcomp = 0;
      }
    } else {
      Settings.mcp230xx_config[idx].enable = 0;
      Settings.mcp230xx_config[idx].inten = 0;
      Settings.mcp230xx_config[idx].intmode = 0;
      Settings.mcp230xx_config[idx].intcomp = 0;
    }
    Settings.mcp230xx_config[idx].b5 = 0;
    Settings.mcp230xx_config[idx].b6 = 0;
    Settings.mcp230xx_config[idx].b7 = 0;
    if (Settings.mcp230xx_config[idx].enable) {
      snprintf_P(stemp, sizeof(stemp), PSTR("epuD%d"), idx);
      Settings.mcp230xx_config[idx].pullup = (!strlen(WebServer->arg(stemp).c_str() )) ?  0 : atoi(WebServer->arg(stemp).c_str() );
    } else {
      Settings.mcp230xx_config[idx].pullup = 0;
    }
  }
  MCP230xx_ApplySettings();
}

#endif // USE_MCP230xx_webconfig

#endif // USE_WEBSERVER

uint8_t MCP230xx_Type(void) {
  return mcp230xx_type;
}

uint8_t MCP230xx_readGPIO(uint8_t port) {
  return I2cRead8(mcp230xx_address, MCP230xx_GPIO + port);
}

void MCP230xx_ApplySettings(void) {
  uint8_t reg_gppu = 0;
  uint8_t reg_gpinten = 0;
  for (uint8_t idx = 0; idx < 8; idx++) {
    if (Settings.mcp230xx_config[idx].enable) {
      if (Settings.mcp230xx_config[idx].inten) { // Int is enabled in some form or another
        reg_gpinten |= (1 << idx);
      }
      if (Settings.mcp230xx_config[idx].pullup) {
        reg_gppu |= (1 << idx);
      }
    }
  }
  I2cWrite8(mcp230xx_address, MCP230xx_GPPU, reg_gppu);
  I2cWrite8(mcp230xx_address, MCP230xx_GPINTEN, reg_gpinten);
  if (mcp230xx_type == 2) { // We have a MCP23017
    reg_gppu = 0;
    reg_gpinten = 0;
    for (uint8_t idx = 8; idx < 16; idx++) {
      if (Settings.mcp230xx_config[idx].enable) {
        if (Settings.mcp230xx_config[idx].inten) { // Int is enabled in some form or another
          reg_gpinten |= (1 << idx - 8);
        }
        if (Settings.mcp230xx_config[idx].pullup) {
          reg_gppu |= (1 << idx - 8);
        }
      }
    }
    I2cWrite8(mcp230xx_address, MCP230xx_GPPU + 1, reg_gppu);
    I2cWrite8(mcp230xx_address, MCP230xx_GPINTEN + 1, reg_gpinten);
  }
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
        snprintf_P(log_data, sizeof(log_data), S_LOG_I2C_FOUND_AT, PSTR("MCP23008"), mcp230xx_address);
        AddLog(LOG_LEVEL_DEBUG);
        mcp280xx_pincount = 8;
        MCP230xx_ApplySettings();
      } else {
        if (buffer == 0x80) {
          mcp230xx_type = 2; // We have a MCP23017
          snprintf_P(log_data, sizeof(log_data), S_LOG_I2C_FOUND_AT, PSTR("MCP23017"), mcp230xx_address);
          AddLog(LOG_LEVEL_DEBUG);
          mcp280xx_pincount = 16;
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

  if (I2cValidRead8(&intf, mcp230xx_address, MCP230xx_INTF)) {
    if (intf > 0) {
      if (I2cValidRead8(&mcp230xx_intcap, mcp230xx_address, MCP230xx_INTCAP)) {
        for (uint8_t intp = 0; intp < 8; intp++) {
          if ((intf >> intp) & 0x01) { // we know which pin caused interrupt
            report_int = 0;
            if (Settings.mcp230xx_config[intp].intmode) { // change on INT
              if (((mcp230xx_intcap >> intp) & 0x01) == Settings.mcp230xx_config[intp].intcomp) report_int = 1;
            } else {
              report_int = 1;
            }
            if (report_int) {
              snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_JSON_TIME "\":\"%s\""), GetDateAndTime(DT_LOCAL).c_str());
              snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"MCP230XX_INT\":{\"Pin\":\"D%i\", \"State\":%i}"), mqtt_data, intp, ((mcp230xx_intcap >> intp) & 0x01));
              snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}"), mqtt_data);
              MqttPublishPrefixTopic_P(RESULT_OR_STAT, mqtt_data);
            }
          }
        }
      }
    }
  }
  if (mcp230xx_type == 2) { // We have a MCP23017 so we need to check the other 8 bits also
    if (I2cValidRead8(&intf, mcp230xx_address, MCP230xx_INTF+1)) {
      if (intf > 0) {
        if (I2cValidRead8(&mcp230xx_intcap, mcp230xx_address, MCP230xx_INTCAP+1)) {
          for (uint8_t intp = 0; intp < 8; intp++) {
            if ((intf >> intp) & 0x01) { // we know which pin caused interrupt
              report_int = 0;
              if (Settings.mcp230xx_config[intp+8].intmode) { // change on INT
                if (((mcp230xx_intcap >> intp) & 0x01) == Settings.mcp230xx_config[intp+8].intcomp) report_int = 1;
              } else {
                report_int = 1;
              }
              if (report_int) {
                snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_JSON_TIME "\":\"%s\""), GetDateAndTime(DT_LOCAL).c_str());
                snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"MCP230XX_INT\":{\"Pin\":\"D%i\", \"State\":%i}"), mqtt_data, intp+8, ((mcp230xx_intcap >> intp) & 0x01));
                snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}"), mqtt_data);
                MqttPublishPrefixTopic_P(RESULT_OR_STAT, mqtt_data);
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
      if (mcp230xx_type == 1) {
        uint8_t gpio = MCP230xx_readGPIO(0);
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"MCP23008\":{\"D0\":%i,\"D1\":%i,\"D2\":%i,\"D3\":%i,\"D4\":%i,\"D5\":%i,\"D6\":%i,\"D7\":%i}"),
                   mqtt_data,(gpio>>0)&1,(gpio>>1)&1,(gpio>>2)&1,(gpio>>3)&1,(gpio>>4)&1,(gpio>>5)&1,(gpio>>6)&1,(gpio>>7)&1);
      }
      if (mcp230xx_type == 2) {
        uint8_t gpio1 = MCP230xx_readGPIO(0);
        uint8_t gpio2 = MCP230xx_readGPIO(1);
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"MCP23017\":{\"D0\":%i,\"D1\":%i,\"D2\":%i,\"D3\":%i,\"D4\":%i,\"D5\":%i,\"D6\":%i,\"D7\":%i,\"D8\":%i,\"D9\":%i,\"D10\":%i,\"D11\":%i,\"D12\":%i,\"D13\":%i,\"D14\":%i,\"D15\":%i}"),
                   mqtt_data, (gpio1>>0)&1,(gpio1>>1)&1,(gpio1>>2)&1,(gpio1>>3)&1,(gpio1>>4)&1,(gpio1>>5)&1,(gpio1>>6)&1,(gpio1>>7)&1,(gpio2>>0)&1,(gpio2>>1)&1,(gpio2>>2)&1,(gpio2>>3)&1,(gpio2>>4)&1,(gpio2>>5)&1,(gpio2>>6)&1,(gpio2>>7)&1);
      }
      
#ifdef USE_WEBSERVER
#ifdef USE_MCP230xx_displaymain
    } else {
      uint8_t gpio1 = MCP230xx_readGPIO(0);
      uint8_t gpio2 = 0;
      if (mcp230xx_type == 2) {
        gpio2=MCP230xx_readGPIO(1);
      }
      uint16_t gpio = (gpio2 << 8) + gpio1;
      
      for (uint8_t pin = 0; pin < mcp280xx_pincount; pin++) {
        if (Settings.mcp230xx_config[pin].enable) {
          snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_MCP230xx_GPIO, mqtt_data, pin, (gpio>>pin)&1);
        }
      }
#endif // USE_MCP230xx_displaymain      
#endif  // USE_WEBSERVER
    }
  }
}

bool MCP230xx_Command(void) {
  boolean serviced = true;
  uint8_t _a, _b = 0;
  uint8_t pin, pinmode, pullup = 0;
  String data = XdrvMailbox.data;
  _a = data.indexOf(",");
  _b = data.indexOf(",", _a + 1);
  if (_a < XdrvMailbox.data_len) {
    if (_b < XdrvMailbox.data_len) {
      pin = data.substring(0, _a).toInt();
      pinmode = data.substring(_a+1, _b).toInt();
      pullup = data.substring(_b+1, XdrvMailbox.data_len).toInt();
      if (pinmode) {
        Settings.mcp230xx_config[pin].enable = 1;
        if (pinmode >= 2) {
          Settings.mcp230xx_config[pin].inten = 1;
          if (pinmode >= 3) {
            Settings.mcp230xx_config[pin].intmode = 1;
            if (pinmode >= 4) {
              Settings.mcp230xx_config[pin].intcomp = 1;
            } else {
              Settings.mcp230xx_config[pin].intcomp = 0;
            }
          } else {
            Settings.mcp230xx_config[pin].intmode = 0;
            Settings.mcp230xx_config[pin].intcomp = 0;
          }
        } else {
          Settings.mcp230xx_config[pin].inten = 0;
          Settings.mcp230xx_config[pin].intmode = 0;
          Settings.mcp230xx_config[pin].intcomp = 0;
        }
      } else {
        Settings.mcp230xx_config[pin].enable = 0;
        Settings.mcp230xx_config[pin].inten = 0;
        Settings.mcp230xx_config[pin].intmode = 0;
        Settings.mcp230xx_config[pin].intcomp = 0;
      }
      Settings.mcp230xx_config[pin].b5 = 0;
      Settings.mcp230xx_config[pin].b6 = 0;
      Settings.mcp230xx_config[pin].b7 = 0;
      if (Settings.mcp230xx_config[pin].enable) {
        Settings.mcp230xx_config[pin].pullup = pullup;
      } else {
        Settings.mcp230xx_config[pin].pullup = 0;
      }
      MCP230xx_ApplySettings();
      snprintf_P(mqtt_data, sizeof(mqtt_data), MCP230XX_SENSOR_RESPONSE,pin,pinmode,pullup);
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
      case FUNC_PREP_BEFORE_TELEPERIOD:
        MCP230xx_Detect();
        break;
      case FUNC_EVERY_50_MSECOND:
        MCP230xx_CheckForInterrupt();
        break;
      case FUNC_JSON_APPEND:
        MCP230xx_Show(1);
        break;
      case FUNC_COMMAND:
        if (XSNS_29 == XdrvMailbox.index) {
          result = MCP230xx_Command();
        }
        break;        
#ifdef USE_WEBSERVER
#ifdef USE_MCP230xx_displaymain
      case FUNC_WEB_APPEND:
        MCP230xx_Show(0);
        break;
#endif // USE_MCP230xx_displaymain        
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_MCP230xx
#endif  // USE_I2C
