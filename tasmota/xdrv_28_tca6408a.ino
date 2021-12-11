/*
  xdrv_28_tca6408a.ino - TCA6408A I2C support for Tasmota

  Copyright (C) 2021  Stefan Bode 
  modified to support TCA6408A as faked PCF8574 - 2021 Joern Plewka 

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
#ifdef USE_PCF8574
/*********************************************************************************************\
 * TCA6408A - I2C IO Expander with unused Reset-Pin
 * for higher levels it behaves like a faked pcf8574
 *
 * I2C Address: TCA6408A  = 0x20 .. 0x21
\*********************************************************************************************/

#define XDRV_28           28
#define XI2C_02           2     // See I2CDEVICES.md

#define TCA6408A_ADDR     0x20  // TCA6408A

/* Command bytes */
#define TCA6408_INPUT                           0x00
#define TCA6408_OUTPUT                          0x01
#define TCA6408_POLARITY_INVERSION              0x02 // input only!
#define TCA6408_CONFIGURATION                   0x03 // 1 is input

struct TCA6408A {
  int error;
  uint8_t pin[64];
  uint8_t address[MAX_PCF8574];
  uint8_t pin_mask[MAX_PCF8574] = { 0 };
#ifdef USE_PCF8574_MQTTINPUT
  uint8_t last_input[MAX_PCF8574] = { 0 };
#endif
  uint8_t max_connected_ports = 0;        // Max numbers of devices comming from TCA6408A modules
  uint8_t max_devices = 0;                // Max numbers of TCA6408A modules
  char stype[9];
  bool type = false;
} Tca6408a;

uint8_t Tca6408aRead(uint8_t idx, uint8_t reg)
{
  Wire.beginTransmission(Tca6408a.address[idx]);
  Wire.write(reg);  // reg 
  Wire.requestFrom(Tca6408a.address[idx],(uint8_t)1);
  return Wire.read();
}

uint8_t Tca6408aWrite(uint8_t idx, uint8_t reg)
{
  Wire.beginTransmission(Tca6408a.address[idx]);
  Wire.write(reg); // reg 
  Wire.write(Tca6408a.pin_mask[idx]); // data
  return Wire.endTransmission();
}

uint8_t Tca6408aRead_Input(uint8_t idx)      {return(Tca6408aRead(idx, TCA6408_INPUT));}
uint8_t Tca6408aRead_Output(uint8_t idx)     {return(Tca6408aRead(idx, TCA6408_OUTPUT));}
uint8_t Tca6408aRead_Inversion(uint8_t idx)  {return(Tca6408aRead(idx, TCA6408_POLARITY_INVERSION));}
uint8_t Tca6408aRead_Direction(uint8_t idx)  {return(Tca6408aRead(idx, TCA6408_CONFIGURATION));}

uint8_t Tca6408aWrite_Input(uint8_t idx)     {return(Tca6408aWrite(idx, TCA6408_INPUT));}
uint8_t Tca6408aWrite_Output(uint8_t idx)    {return(Tca6408aWrite(idx, TCA6408_OUTPUT));}
uint8_t Tca6408aWrite_Inversion(uint8_t idx) {return(Tca6408aWrite(idx, TCA6408_POLARITY_INVERSION));}
uint8_t Tca6408aWrite_Direction(uint8_t idx) {return(Tca6408aWrite(idx, TCA6408_CONFIGURATION));}

void Tca6408aSwitchRelay(void)
{
  for (uint32_t i = 0; i < TasmotaGlobal.devices_present; i++) {
    uint8_t relay_state = bitRead(XdrvMailbox.index, i);

    if (Tca6408a.max_devices > 0 && Tca6408a.pin[i] < 99) {
      uint8_t board = Tca6408a.pin[i]>>3; // TODO: done for 8, not 2 
      uint8_t pin = Tca6408a.pin[i]&0x7;  // 8
      uint8_t oldpinmask = Tca6408a.pin_mask[board];
      uint8_t _val = bitRead(TasmotaGlobal.rel_inverted, i) ? !relay_state : relay_state;

      //AddLog(LOG_LEVEL_DEBUG, PSTR("TCA: SwitchRelay %d=%d => TCA-%d.D%d=%d"), i, relay_state, board +1, pin, _val);
      bitWrite(Tca6408a.pin_mask[board], pin, _val);
      if (oldpinmask != Tca6408a.pin_mask[board]) {
        Tca6408a.error = Tca6408aWrite_Output(board);
      }
      //else AddLog(LOG_LEVEL_DEBUG, PSTR("TCA: SwitchRelay skipped"));
    }
  }
}

void Tca6408aInit(void)
{
  uint8_t tca6408a_address = TCA6408A_ADDR;
  while (Tca6408a.max_devices < MAX_PCF8574 +1) {

#ifdef USE_MCP230xx_ADDR
    if (USE_MCP230xx_ADDR == tca6408a_address) {
      AddLog(LOG_LEVEL_INFO, PSTR("TCA: Address 0x%02x reserved for MCP320xx skipped"), tca6408a_address);
      tca6408a_address++;
    }
#endif

  //  AddLog(LOG_LEVEL_DEBUG, PSTR("TCA: Probing addr: 0x%x for TCA6408A"), tca6408a_address);

    if (I2cSetDevice(tca6408a_address)) {
      Tca6408a.type = true;

      Tca6408a.address[Tca6408a.max_devices] = tca6408a_address;
      Tca6408a.max_devices++;

      strcpy(Tca6408a.stype, "TCA6408A");
      I2cSetActiveFound(tca6408a_address, Tca6408a.stype);
    }

    tca6408a_address++;
  }
  if (Tca6408a.type) {
    for (uint32_t i = 0; i < sizeof(Tca6408a.pin); i++) {
      Tca6408a.pin[i] = 99;
    }
    TasmotaGlobal.devices_present = TasmotaGlobal.devices_present - Tca6408a.max_connected_ports; // reset no of devices to avoid duplicate ports on duplicate init.
    Tca6408a.max_connected_ports = 0;  // reset no of devices to avoid duplicate ports on duplicate init.
    for (uint32_t idx = 0; idx < Tca6408a.max_devices; idx++) { // support up to 2 boards TCA6408A
      
      Tca6408aWrite_Direction(~Settings->pcf8574_config[idx]); // 1 is input at TCA
      
      uint8_t gpio = Tca6408aRead_Input(idx);
      Tca6408a.pin_mask[idx] = gpio;
#ifdef USE_PCF8574_MQTTINPUT
      Tca6408a.last_input[idx] = gpio & ~Settings->pcf8574_config[idx];
#endif // #ifdef USE_PCF8574_MQTTINPUT
      //AddLog(LOG_LEVEL_DEBUG, PSTR("TCA: TCA-%d config=0x%02x, gpio=0x%02X"), idx +1, Settings->pcf8574_config[idx], gpio);

      for (uint32_t i = 0; i < 8; i++, gpio>>=1) { // 8 bit wide port
        uint8_t _result = Settings->pcf8574_config[idx] >> i &1;
        //AddLog(LOG_LEVEL_DEBUG, PSTR("TCA: I2C shift i %d: %d. Powerstate: %d, TasmotaGlobal.devices_present: %d"), i,_result, Settings->power>>i&1, TasmotaGlobal.devices_present);
        if (_result > 0) {
          Tca6408a.pin[TasmotaGlobal.devices_present] = i + 8 * idx;
          bitWrite(TasmotaGlobal.rel_inverted, TasmotaGlobal.devices_present, Settings->flag3.pcf8574_ports_inverted);  // SetOption81 - Invert all ports on TCA6408A devices
          if (!Settings->flag.save_state && !Settings->flag3.no_power_feedback) {  // SetOption63 - Don't scan relay power state at restart - #5594 and #5663
            //AddLog(LOG_LEVEL_DEBUG, PSTR("TCA: Set power from from chip state"));
            uint8_t power_state = Settings->flag3.pcf8574_ports_inverted ? 1 & ~gpio : 1 & gpio;
            bitWrite(TasmotaGlobal.power, TasmotaGlobal.devices_present, power_state);
            bitWrite(Settings->power, TasmotaGlobal.devices_present, power_state);
          }
          //else AddLog(LOG_LEVEL_DEBUG, PSTR("TCA: DON'T set power from chip state"));
          TasmotaGlobal.devices_present++;
          Tca6408a.max_connected_ports++;
        }
      }
    }
    //AddLog(LOG_LEVEL_DEBUG, PSTR("TCA: Settings->power=0x%08X, TasmotaGlobal.power=0x%08X"), Settings->power, TasmotaGlobal.power);
    AddLog(LOG_LEVEL_INFO, PSTR("TCA: Total devices %d, TCA6408A output ports %d"), Tca6408a.max_devices, Tca6408a.max_connected_ports);
  }
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

#ifdef USE_WEBSERVER

#define WEB_HANDLE_PCF8574 "pcf"

const char HTTP_BTN_MENU_PCF8574[] PROGMEM =
  "<p><form action='" WEB_HANDLE_PCF8574 "' method='get'><button>" D_CONFIGURE_PCF8574 "</button></form></p>";

const char HTTP_FORM_I2C_PCF8574_1[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_PCF8574_PARAMETERS "&nbsp;</b></legend>"
  "<form method='get' action='" WEB_HANDLE_PCF8574 "'>"
  "<p><label><input id='b1' name='b1' type='checkbox'%s><b>" D_INVERT_PORTS "</b></label></p><hr/>";

const char HTTP_FORM_I2C_PCF8574_2[] PROGMEM =
  "<tr><td><b>" D_DEVICE " %d " D_PORT " %d</b></td><td style='width:100px'><select id='i2cs%d' name='i2cs%d'>"
  "<option%s value='0'>" D_DEVICE_INPUT "</option>"
  "<option%s value='1'>" D_DEVICE_OUTPUT "</option>"
  "</select></td></tr>";

const char HTTP_SNS_PCF8574_GPIO[] PROGMEM = "{s}TCA6408A%c%d D%d{m}%d{e}"; // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>


void HandleTca6408a(void)
{
  if (!HttpCheckPriviledgedAccess()) { return; }

  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP  D_CONFIGURE_PCF8574));

  if (Webserver->hasArg("save")) {
    Tca6408aSaveSettings();
    WebRestart(1);
    return;
  }

  WSContentStart_P(D_CONFIGURE_PCF8574);
  WSContentSendStyle();
  WSContentSend_P(HTTP_FORM_I2C_PCF8574_1, (Settings->flag3.pcf8574_ports_inverted) ? PSTR(" checked") : "");  // SetOption81 - Invert all ports on TCA6408A devices
  WSContentSend_P(HTTP_TABLE100);
  for (uint32_t idx = 0; idx < Tca6408a.max_devices; idx++) {
    for (uint32_t idx2 = 0; idx2 < 8; idx2++) {  // 8 ports on TCA6408A
      uint8_t helper = 1 << idx2;
      WSContentSend_P(HTTP_FORM_I2C_PCF8574_2,
        idx +1, idx2,
        idx2 + 8*idx,
        idx2 + 8*idx,
        ((helper & Settings->pcf8574_config[idx]) >> idx2 == 0) ? PSTR(" selected ") : " ",
        ((helper & Settings->pcf8574_config[idx]) >> idx2 == 1) ? PSTR(" selected ") : " "
      );
    }
  }
  WSContentSend_P(PSTR("</table>"));
  WSContentSend_P(HTTP_FORM_END);
  WSContentSpaceButton(BUTTON_CONFIGURATION);
  WSContentStop();
}

#if defined(USE_PCF8574_SENSOR) || defined(USE_PCF8574_DISPLAYINPUT)
void Tca6408aShow(bool json)
{
#ifdef USE_PCF8574_SENSOR
  if (json) {
    for (int idx = 0 ; idx < Tca6408a.max_devices ; idx++)
    {
      uint8_t gpio = Tca6408aRead_Input(idx);
      ResponseAppend_P(PSTR(",\"TCA6408A%c%d\":{\"D0\":%i,\"D1\":%i,\"D2\":%i,\"D3\":%i,\"D4\":%i,\"D5\":%i,\"D6\":%i,\"D7\":%i}"),
        IndexSeparator(), idx +1,
        (gpio>>0)&1,(gpio>>1)&1,(gpio>>2)&1,(gpio>>3)&1,(gpio>>4)&1,(gpio>>5)&1,(gpio>>6)&1,(gpio>>7)&1);
    }
  }
#endif // #ifdef USE_PCF8574_SENSOR
#if defined(USE_WEBSERVER) && defined(USE_PCF8574_DISPLAYINPUT)
  if(!json) {
    for (int idx = 0 ; idx < Tca6408a.max_devices ; idx++)
    {
      uint8_t input_mask = ~Settings->pcf8574_config[idx]; //invert to 1 = input
      uint8_t gpio = Tca6408aRead_Input(idx);
      for (int pin = 0 ; pin < 8 ; ++pin, input_mask>>=1, gpio>>=1)
      {
        if (input_mask & 1)
          WSContentSend_P(HTTP_SNS_PCF8574_GPIO, IndexSeparator(), idx +1, pin, gpio & 1);
      }
    }
  }
#endif // defined(USE_WEBSERVER) && defined(USE_PCF8574_DISPLAYINPUT)
}
#endif // #if defined(USE_PCF8574_SENSOR) || defined(USE_PCF8574_DISPLAYINPUT)


#ifdef USE_PCF8574_MQTTINPUT
void Tca6408aCheckForInputChange(void)
{
    for (int idx = 0 ; idx < Tca6408a.max_devices ; idx++)
    {
      uint8_t input_mask = ~Settings->pcf8574_config[idx]; //invert to 1 = input
      uint8_t input = Tca6408aRead_Input(idx) & input_mask;
      uint8_t last_input = Tca6408a.last_input[idx];
      if (input != last_input) { // don't scan bits if no change (EVERY_50_MS !)
        for (uint8_t pin = 0 ; pin < 8 ; ++pin) {
          if (bitRead(input_mask,pin) && bitRead(input,pin) != bitRead(last_input,pin)) {
            ResponseTime_P(PSTR(",\"TCA6408A%c%d_INP\":{\"D%i\":%i}}"), IndexSeparator(), idx +1, pin, bitRead(input,pin));
            MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, PSTR("TCA6408A_INP"));
            if (Settings->flag3.hass_tele_on_power) {  // SetOption59 - Send tele/%topic%/SENSOR in addition to stat/%topic%/RESULT
                MqttPublishSensor();
            }
          }
          Tca6408a.last_input[idx] =  input;
        }
      }
    }
}
#endif //#ifdef USE_PCF8574_MQTTINPUT

void Tca6408aSaveSettings(void)
{
  char stemp[7];
  char tmp[100];

  //AddLog(LOG_LEVEL_DEBUG, PSTR("TCA: Start working on Save arguements: inverted:%d")), Webserver->hasArg("b1");

  Settings->flag3.pcf8574_ports_inverted = Webserver->hasArg("b1");  // SetOption81 - Invert all ports on TCA6408A devices
  for (byte idx = 0; idx < Tca6408a.max_devices; idx++) {
    byte count=0;
    byte n = Settings->pcf8574_config[idx];
    while(n!=0) {
      n = n&(n-1);
      count++;
    }
    if (count <= TasmotaGlobal.devices_present) {
      TasmotaGlobal.devices_present = TasmotaGlobal.devices_present - count;
    }
    for (byte i = 0; i < 8; i++) {
      snprintf_P(stemp, sizeof(stemp), PSTR("i2cs%d"), i+8*idx);
      WebGetArg(stemp, tmp, sizeof(tmp));
      byte _value = (!strlen(tmp)) ?  0 : atoi(tmp);
      if (_value) {
        Settings->pcf8574_config[idx] = Settings->pcf8574_config[idx] | 1 << i;
        TasmotaGlobal.devices_present++;
        Tca6408a.max_connected_ports++;
      } else {
        Settings->pcf8574_config[idx] = Settings->pcf8574_config[idx] & ~(1 << i );
      }
    }
    //Settings->pcf8574_config[0] = (!strlen(webServer->arg("i2cs0").c_str())) ?  0 : atoi(webServer->arg("i2cs0").c_str());
    //AddLog(LOG_LEVEL_INFO, PSTR("TCA: I2C Board: %d, Config: %2x")),  idx, Settings->pcf8574_config[idx];

  }
}
#endif // USE_WEBSERVER

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv28(uint8_t function)
{
  if (!I2cEnabled(XI2C_02)) { return false; }

  bool result = false;

  if (FUNC_PRE_INIT == function) {
    Tca6408aInit();
  }
  else if (Tca6408a.type) {
    switch (function) {
      case FUNC_SET_POWER:
        Tca6408aSwitchRelay();
        break;
#ifdef USE_PCF8574_MQTTINPUT
      case FUNC_EVERY_50_MSECOND:
        Tca6408aCheckForInputChange();
        break;
#endif // #ifdef USE_PCF8574_MQTTINPUT
#ifdef USE_PCF8574_SENSOR
      case FUNC_JSON_APPEND:
        Tca6408aShow(1);
        break;
#endif // #ifdef USE_PCF8574_SENSOR
#ifdef USE_WEBSERVER
      case FUNC_WEB_ADD_BUTTON:
        WSContentSend_P(HTTP_BTN_MENU_PCF8574);
        break;
      case FUNC_WEB_ADD_HANDLER:
        WebServer_on(PSTR("/" WEB_HANDLE_PCF8574), HandleTca6408a);
        break;
#ifdef USE_PCF8574_DISPLAYINPUT
      case FUNC_WEB_SENSOR:
        Tca6408aShow(0);
        break;
#endif // #ifdef USE_PCF8574_DISPLAYINPUT
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_PCF8574
#endif  // USE_I2C
