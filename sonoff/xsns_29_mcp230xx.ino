/*
  xsns_29_mcp230xx.ino - Support for I2C MCP23008/MCP23017 GPIO Expander

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

unsigned long int_millis[16]; // To keep track of millis() since last interrupt

const char MCP230XX_SENSOR_RESPONSE[] PROGMEM = "{\"Sensor29_D%i\":{\"MODE\":%i,\"PULL_UP\":\"%s\",\"INT_MODE\":\"%s\",\"STATE\":\"%s\"}}";

#ifdef USE_MCP230xx_OUTPUT
const char MCP230XX_CMND_RESPONSE[] PROGMEM = "{\"S29cmnd_D%i\":{\"COMMAND\":\"%s\",\"STATE\":\"%s\"}}";
#endif // USE_MCP230xx_OUTPUT

const char* ConvertNumTxt(uint8_t statu, uint8_t pinmod=0) {
#ifdef USE_MCP230xx_OUTPUT
if (pinmod == 6 && statu < 2) statu = abs(statu-1);
#endif // USE_MCP230xx_OUTPUT
  switch (statu) {
    case 0:
      return "OFF";
      break;
    case 1:
      return "ON";
      break;
#ifdef USE_MCP230xx_OUTPUT
    case 2:
      return "TOGGLE";
      break;
#endif // USE_MCP230xx_OUTPUT
    default:
      break;
  }
}

const char* IntModeTxt(uint8_t intmo) {
  switch (intmo) {
    case 0:
      return "ALL";
      break;
    case 1:
      return "EVENT";
      break;
    case 2:
      return "TELE";
      break;
    case 3:
      return "DISABLED";
      break;
    default:
      return "UNKNOWN";
      break;
  }
}

uint8_t MCP230xx_readGPIO(uint8_t port) {
  return I2cRead8(mcp230xx_address, MCP230xx_GPIO + port);
}

void MCP230xx_ApplySettings(void) {
  uint8_t int_en = 0;
  for (uint8_t mcp230xx_port=0;mcp230xx_port<mcp230xx_type;mcp230xx_port++) {
    uint8_t reg_gppu = 0;
    uint8_t reg_gpinten = 0;
    uint8_t reg_iodir = 0xFF;
#ifdef USE_MCP230xx_OUTPUT
    uint8_t reg_portpins = 0x00;
#endif // USE_MCP230xx_OUTPUT
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
#ifdef USE_MCP230xx_OUTPUT
        case 5 ... 6:
          reg_iodir &= ~(1 << idx);
          if (Settings.flag.save_state) { // Firmware configuration wants us to use the last pin state
            reg_portpins |= (Settings.mcp230xx_config[idx+(mcp230xx_port*8)].saved_state << idx);
          } else {
            if (Settings.mcp230xx_config[idx+(mcp230xx_port*8)].pullup) {
              reg_portpins |= (1 << idx);
            }
          }
          break;
#endif // USE_MCP230xx_OUTPUT
        default:
          break;
      }
#ifdef USE_MCP230xx_OUTPUT
      if (Settings.mcp230xx_config[idx+(mcp230xx_port*8)].pullup && (Settings.mcp230xx_config[idx+(mcp230xx_port*8)].pinmode < 5)) {
        reg_gppu |= (1 << idx);
      }
#else // not USE_MCP230xx_OUTPUT
      if (Settings.mcp230xx_config[idx+(mcp230xx_port*8)].pullup) {
        reg_gppu |= (1 << idx);
      }
#endif // USE_MCP230xx_OUTPUT
    }
    I2cWrite8(mcp230xx_address, MCP230xx_GPPU+mcp230xx_port, reg_gppu);
    I2cWrite8(mcp230xx_address, MCP230xx_GPINTEN+mcp230xx_port, reg_gpinten);
    I2cWrite8(mcp230xx_address, MCP230xx_IODIR+mcp230xx_port, reg_iodir);
#ifdef USE_MCP230xx_OUTPUT
    I2cWrite8(mcp230xx_address, MCP230xx_GPIO+mcp230xx_port, reg_portpins);
#endif // USE_MCP230xx_OUTPUT
  }
  for (uint8_t idx=0;idx<mcp230xx_pincount;idx++) {
    int_millis[idx]=millis();
  }
  mcp230xx_int_en=int_en;
}

void MCP230xx_Detect()
{
  if (mcp230xx_type) {
    return;
  }

  uint8_t buffer;

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
                  bool int_tele = false;
                  bool int_event = false;
                  unsigned long millis_since_last_int = millis() - int_millis[intp+(mcp230xx_port*8)];
                  switch (Settings.mcp230xx_config[intp+(mcp230xx_port*8)].int_report_mode) {
                    case 0:
                      int_tele=true;
                      int_event=true;
                      break;
                    case 1:
                      int_event=true;
                      break;
                    case 2:
                      int_tele=true;
                      break;
                  }
                  if (int_tele) {
                    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_JSON_TIME "\":\"%s\""), GetDateAndTime(DT_LOCAL).c_str());
                    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"MCP230XX_INT\":{\"D%i\":%i,\"MS\":%lu}"), mqtt_data, intp+(mcp230xx_port*8), ((mcp230xx_intcap >> intp) & 0x01),millis_since_last_int);
                    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}"), mqtt_data);
                    MqttPublishPrefixTopic_P(RESULT_OR_STAT, mqtt_data);
                  }
                  if (int_event) {
                    char command[18];
                    sprintf(command,"event MCPINT_D%i=%i",intp+(mcp230xx_port*8),((mcp230xx_intcap >> intp) & 0x01));
                    ExecuteCommand(command, SRC_RULE);
                  }
                  int_millis[intp+(mcp230xx_port*8)]=millis();
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

#ifdef USE_MCP230xx_OUTPUT

void MCP230xx_SetOutPin(uint8_t pin,uint8_t pinstate) {
  uint8_t portpins;
  uint8_t port = 0;
  uint8_t pinmo = Settings.mcp230xx_config[pin].pinmode;
  uint8_t interlock = Settings.flag.interlock;
  int pinadd = (pin % 2)+1-(3*(pin % 2)); //check if pin is odd or even and convert to 1 (if even) or -1 (if odd)
  char cmnd[7], stt[4];
  if (pin > 7) port=1;
  portpins = MCP230xx_readGPIO(port);
  if (interlock && pinmo == Settings.mcp230xx_config[pin+pinadd].pinmode) {
    if (pinstate < 2) {
      if (pinmo == 6) {
        if (pinstate) portpins |= (1 << pin-(port*8)); else portpins |= (1 << pin+pinadd-(port*8)),portpins &= ~(1 << pin-(port*8));
      } else {
        if (pinstate) portpins &= ~(1 << pin+pinadd-(port*8)),portpins |= (1 << pin-(port*8)); else portpins &= ~(1 << pin-(port*8));
      }
    } else {
      if (pinmo == 6) {
      portpins |= (1 << pin+pinadd-(port*8)),portpins ^= (1 << pin-(port*8));
      } else {
      portpins &= ~(1 << pin+pinadd-(port*8)),portpins ^= (1 << pin-(port*8));
      }
    }
  } else {
    if (pinstate < 2) {
      if (pinstate) portpins |= (1 << pin-(port*8)); else portpins &= ~(1 << pin-(port*8));
    } else {
      portpins ^= (1 << pin-(port*8));
    }
  }
  I2cWrite8(mcp230xx_address, MCP230xx_GPIO + port, portpins);
  if (Settings.flag.save_state) { // Firmware configured to save last known state in settings
    Settings.mcp230xx_config[pin].saved_state=portpins>>(pin-(port*8))&1;
    Settings.mcp230xx_config[pin+pinadd].saved_state=portpins>>(pin+pinadd-(port*8))&1;
  }
  sprintf(cmnd,ConvertNumTxt(pinstate, pinmo));
  sprintf(stt,ConvertNumTxt((portpins >> (pin-(port*8))&1), pinmo));
  if (interlock && pinmo == Settings.mcp230xx_config[pin+pinadd].pinmode) {
    char stt1[4];
    sprintf(stt1,ConvertNumTxt((portpins >> (pin+pinadd-(port*8))&1), pinmo));
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"S29cmnd_D%i\":{\"COMMAND\":\"%s\",\"STATE\":\"%s\"},\"S29cmnd_D%i\":{\"STATE\":\"%s\"}}"),pin, cmnd, stt, pin+pinadd, stt1);
  } else {
    snprintf_P(mqtt_data, sizeof(mqtt_data), MCP230XX_CMND_RESPONSE, pin, cmnd, stt);
  }
}

#endif // USE_MCP230xx_OUTPUT

void MCP230xx_Reset(uint8_t pinmode) {
  uint8_t pullup = 0;
  if ((pinmode > 1) && (pinmode < 5)) pullup=1;
  for (uint8_t pinx=0;pinx<16;pinx++) {
    Settings.mcp230xx_config[pinx].pinmode=pinmode;
    Settings.mcp230xx_config[pinx].pullup=pullup;
    Settings.mcp230xx_config[pinx].saved_state=0;
    if (pinmode > 1 && pinmode < 5) {
      Settings.mcp230xx_config[pinx].int_report_mode=0; // Enabled for ALL by default
    } else {
      Settings.mcp230xx_config[pinx].int_report_mode=3; // Disabled for pinmode 1, 5 and 6 (No interrupts there)
    }
    Settings.mcp230xx_config[pinx].int_report_defer=0; // Disabled
    Settings.mcp230xx_config[pinx].int_count_sec; // Disabled
    Settings.mcp230xx_config[pinx].int_count_min; // Disabled
    Settings.mcp230xx_config[pinx].spare13=0;
    Settings.mcp230xx_config[pinx].spare14=0;
    Settings.mcp230xx_config[pinx].spare15=0;
  }
  Settings.mcp230xx_int_prio=0; // Once per FUNC_EVERY_50_MSECOND callback
  MCP230xx_ApplySettings();
  char pulluptxt[7];
  char intmodetxt[9];
  sprintf(pulluptxt,ConvertNumTxt(pullup));
  uint8_t intmode = 3;
  if (pinmode > 1 && pinmode < 5) intmode=0;
  sprintf(intmodetxt,IntModeTxt(intmode));
  snprintf_P(mqtt_data, sizeof(mqtt_data), MCP230XX_SENSOR_RESPONSE,99,pinmode,pulluptxt,intmodetxt,"");
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
#ifdef USE_MCP230xx_OUTPUT
  if (data == "RESET5") { MCP230xx_Reset(5); return serviced; }
  if (data == "RESET6") { MCP230xx_Reset(6); return serviced; }
#endif // USE_MCP230xx_OUTPUT

  _a = data.indexOf(",");
  pin = data.substring(0, _a).toInt();
  if (pin < mcp230xx_pincount) {
    String cmnd = data.substring(_a+1);
    if (cmnd == "?") {
      uint8_t port = 0;
      if (pin > 7) port = 1;
      uint8_t portdata = MCP230xx_readGPIO(port);
      char pulluptxtr[7],pinstatustxtr[7];
      char intmodetxt[9];
      sprintf(intmodetxt,IntModeTxt(Settings.mcp230xx_config[pin].int_report_mode));
      sprintf(pulluptxtr,ConvertNumTxt(Settings.mcp230xx_config[pin].pullup));
#ifdef USE_MCP230xx_OUTPUT
      uint8_t pinmod = Settings.mcp230xx_config[pin].pinmode;
      sprintf(pinstatustxtr,ConvertNumTxt(portdata>>(pin-(port*8))&1,pinmod));
      snprintf_P(mqtt_data, sizeof(mqtt_data), MCP230XX_SENSOR_RESPONSE,pin,pinmod,pulluptxtr,intmodetxt,pinstatustxtr);
#else // not USE_MCP230xx_OUTPUT
      sprintf(pinstatustxtr,ConvertNumTxt(portdata>>(pin-(port*8))&1));
      snprintf_P(mqtt_data, sizeof(mqtt_data), MCP230XX_SENSOR_RESPONSE,pin,Settings.mcp230xx_config[pin].pinmode,pulluptxtr,intmodetxt,pinstatustxtr);
#endif //USE_MCP230xx_OUTPUT
      return serviced;
    }
#ifdef USE_MCP230xx_OUTPUT
    if (Settings.mcp230xx_config[pin].pinmode >= 5) {
      uint8_t pincmd = Settings.mcp230xx_config[pin].pinmode - 5;
      if (cmnd == "ON") {
        MCP230xx_SetOutPin(pin,abs(pincmd-1));
        return serviced;
      }
      if (cmnd == "OFF") {
        MCP230xx_SetOutPin(pin,pincmd);
        return serviced;
      }
      if (cmnd == "T")   {
        MCP230xx_SetOutPin(pin,2);
        return serviced;
      }
    }
#endif // USE_MCP230xx_OUTPUT
  }
  _b = data.indexOf(",", _a + 1);
  if (_a < XdrvMailbox.data_len) {
    if (_b < XdrvMailbox.data_len) {
      // Lets see if we have a 4th parameter for interrupt mode
      uint8_t _c = data.indexOf(",", _b + 1);
      if (_c > XdrvMailbox.data_len) _c=XdrvMailbox.data_len;
      pinmode = data.substring(_a+1, _b).toInt();
      pullup = data.substring(_b+1, _c).toInt();
      uint8_t intmode = data.substring(_c+1, XdrvMailbox.data_len).toInt();
#ifdef USE_MCP230xx_OUTPUT
      if ((pin < mcp230xx_pincount) && (pinmode < 7) && (pullup < 2)) {
#else  // not USE_MCP230xx_OUTPUT
      if ((pin < mcp230xx_pincount) && (pinmode < 5) && (pullup < 2)) {
#endif // USE_MCP230xx_OUTPUT
        Settings.mcp230xx_config[pin].pinmode=pinmode;
        Settings.mcp230xx_config[pin].pullup=pullup;
        if (pinmode > 1 && pinmode < 5) {
          if (intmode >= 0 && intmode <= 3) {
            Settings.mcp230xx_config[pin].int_report_mode=intmode;
          }
        } else {
          Settings.mcp230xx_config[pin].int_report_mode=3; // Int mode not valid for pinmodes other than 2 through 4
        }
        MCP230xx_ApplySettings();
        uint8_t port = 0;
        if (pin > 7) port = 1;
        uint8_t portdata = MCP230xx_readGPIO(port);
        char pulluptxtc[7], pinstatustxtc[7];
        char intmodetxt[9];
        sprintf(pulluptxtc,ConvertNumTxt(pullup));
        sprintf(intmodetxt,IntModeTxt(Settings.mcp230xx_config[pin].int_report_mode));
#ifdef USE_MCP230xx_OUTPUT
        sprintf(pinstatustxtc,ConvertNumTxt(portdata>>(pin-(port*8))&1,Settings.mcp230xx_config[pin].pinmode));
#else  // not USE_MCP230xx_OUTPUT
        sprintf(pinstatustxtc,ConvertNumTxt(portdata>>(pin-(port*8))&1));
#endif // USE_MCP230xx_OUTPUT
        snprintf_P(mqtt_data, sizeof(mqtt_data), MCP230XX_SENSOR_RESPONSE,pin,pinmode,pulluptxtc,intmodetxt,pinstatustxtc);
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

#ifdef USE_MCP230xx_DISPLAYOUTPUT

const char HTTP_SNS_MCP230xx_OUTPUT[] PROGMEM = "%s{s}MCP230XX D%d{m}%s{e}"; // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>

void MCP230xx_UpdateWebData(void) {
  uint8_t gpio1 = MCP230xx_readGPIO(0);
  uint8_t gpio2 = 0;
  if (mcp230xx_type == 2) {
    gpio2=MCP230xx_readGPIO(1);
  }
  uint16_t gpio = (gpio2 << 8) + gpio1;
  for (uint8_t pin = 0; pin < mcp230xx_pincount; pin++) {
    if (Settings.mcp230xx_config[pin].pinmode >= 5) {
      char stt[7];
      sprintf(stt,ConvertNumTxt((gpio>>pin)&1,Settings.mcp230xx_config[pin].pinmode));
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_MCP230xx_OUTPUT, mqtt_data, pin, stt);
    }
  }
}

#endif // USE_MCP230xx_DISPLAYOUTPUT

#ifdef USE_MCP230xx_OUTPUT

void MCP230xx_OutputTelemetry(void) {
  if (mcp230xx_type == 0) return; // We do not do this if the MCP has not been detected
  uint8_t outputcount = 0;
  uint16_t gpiototal = 0;
  uint8_t gpioa = 0;
  uint8_t gpiob = 0;
  gpioa=MCP230xx_readGPIO(0);
  if (mcp230xx_type == 2) gpiob=MCP230xx_readGPIO(1);
  gpiototal=((uint16_t)gpiob<<8) | gpioa;
  for (uint8_t pinx = 0;pinx < mcp230xx_pincount;pinx++) {
    if (Settings.mcp230xx_config[pinx].pinmode >= 5) outputcount++;
  }
  if (outputcount) {
    char stt[7];
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_JSON_TIME "\":\"%s\",\"MCP230_OUT\": {"), GetDateAndTime(DT_LOCAL).c_str());
    for (uint8_t pinx = 0;pinx < mcp230xx_pincount;pinx++) {
      if (Settings.mcp230xx_config[pinx].pinmode >= 5) {
        sprintf(stt,ConvertNumTxt(((gpiototal>>pinx)&1),Settings.mcp230xx_config[pinx].pinmode));
        snprintf_P(mqtt_data,sizeof(mqtt_data), PSTR("%s\"OUT_D%i\":\"%s\","),mqtt_data,pinx,stt);
      }
    }
    snprintf_P(mqtt_data,sizeof(mqtt_data),PSTR("%s\"END\":1}}"),mqtt_data);
    MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR), Settings.flag.mqtt_sensor_retain);
  }
}

#endif // USE_MCP230xx_OUTPUT


/*********************************************************************************************\
   Interface
\*********************************************************************************************/

boolean Xsns29(byte function)
{
  boolean result = false;

  if (i2c_flg) {
    switch (function) {
      case FUNC_MQTT_DATA:
        break;
      case FUNC_EVERY_SECOND:
        MCP230xx_Detect();
#ifdef USE_MCP230xx_OUTPUT
        if (tele_period == 0) {
          MCP230xx_OutputTelemetry();
        }
#endif // USE_MCP230xx_OUTPUT
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
#ifdef USE_WEBSERVER
#ifdef USE_MCP230xx_OUTPUT
#ifdef USE_MCP230xx_DISPLAYOUTPUT
      case FUNC_WEB_APPEND:
        MCP230xx_UpdateWebData();
        break;
#endif // USE_MCP230xx_DISPLAYOUTPUT
#endif // USE_MCP230xx_OUTPUT
#endif // USE_WEBSERVER
      default:
        break;
    }
  }
  return result;
}

#endif  // USE_MCP230xx
#endif  // USE_I2C
