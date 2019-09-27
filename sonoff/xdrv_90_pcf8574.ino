/*
Copyright (c) 2017 Stefan Bode.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/


#ifdef USE_I2C
#ifdef USE_PCF8574

#define PFC8574_ADDR1        0x38

uint8_t pcf8574_pin[64] = {99}, pcf8574addr[8], pcf8574type = 0;
char pcf8574stype[8];
uint8_t _pcf8574pinMask[8] = {0};
int _error;


#ifdef USE_WEBSERVER

#define WEB_HANDLE_PCF8574 "pcf"

const char HTTP_BTN_MENU_PCF8574[] PROGMEM =
  "<p><form action='" WEB_HANDLE_PCF8574 "' method='get'><button>" D_CONFIGURE_PCF8574 "</button></form></p>";

const char HTTP_FORM_I2C_PCF8574_1[] PROGMEM =
  "<fieldset><legend><b>&nbsp;PCF8674 parameters &nbsp;</b></legend>"
  "<form method='get' action='" WEB_HANDLE_PCF8574 "'>"
  "<br/><input id='b1' name='b1' type='checkbox'%s><b>Reverse Relays</b><br/><br/><hr/>";
const char HTTP_FORM_I2C_PCF8574_2[] PROGMEM =
  "<br/><b>Board %d " D_CHANNEL " %d</b> <br/><select id='i2cs%d' name='i2cs%d'>"
  "<option%s value='0'>Input</option>"
  "<option%s value='1'>Output</option>"
  "</select></br>";




void handleI2C()
{
  if (!HttpCheckPriviledgedAccess()) { return; }
  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP  D_CONFIGURE_PCF8574));

  if (WebServer->hasArg("save")) {
    pcf8574_saveSettings();
    WebRestart(1);
    return;
  }

  WSContentStart_P(D_CONFIGURE_PCF8574);
  WSContentSendStyle();

  //page += FPSTR(HTTP_HEAD_STYLE);
  WSContentSend_P(HTTP_FORM_I2C_PCF8574_1, (Settings.all_relays_inverted) ? " checked" : "");
  //page += FPSTR(HTTP_FORM_I2C_PCF8574_1);
  //page.replace("{r1", (Settings.all_relays_inverted) ? " checked" : "");
  //AddLog(LOG_LEVEL_INFO);
  for (uint8_t idx = 0; idx < max_pcf8574_devices; idx++) {
    //page.replace("{b1", String(idx));
    for (uint8_t idx2 = 0; idx2 < 8; idx2++) {
      uint8_t helper = 1 << idx2;
      WSContentSend_P(HTTP_FORM_I2C_PCF8574_2,
        idx , idx2,
        idx2+8*idx,
        idx2+8*idx,
        ((helper & Settings.pcf8574_config[idx]) >> idx2 == 0) ? " selected " : " ",
        ((helper & Settings.pcf8574_config[idx]) >> idx2 == 1) ? " selected " : " "
      );
    }
  }
  WSContentSend_P(HTTP_FORM_END);
  WSContentSpaceButton(BUTTON_CONFIGURATION);
  WSContentStop();
}

void pcf8574_saveSettings()
{
  char stemp[7];
  char tmp[100];
  //AddLog_P(LOG_LEVEL_DEBUG, PSTR("Start working on Save arguements: inverted:%d")), WebServer->hasArg("ab1");
  Settings.all_relays_inverted = WebServer->hasArg("b1");
  for (byte idx = 0; idx < max_pcf8574_devices; idx++) {
    byte count=0;
    byte n = Settings.pcf8574_config[idx];
    while(n!=0) {
      n = n&(n-1);
      count++;
    }
    if (count <= devices_present) {
      devices_present = devices_present - count;
    }
    for (byte i = 0; i < 8; i++) {
      snprintf_P(stemp, sizeof(stemp), PSTR("i2cs%d"), i+8*idx);
      WebGetArg(stemp, tmp, sizeof(tmp));
      byte _value = (!strlen(tmp)) ?  0 : atoi(tmp);
      if (_value) {
        Settings.pcf8574_config[idx] = Settings.pcf8574_config[idx] | 1 << i;
        devices_present++;
        max_pcf8574_connected_ports++;
      } else {
        Settings.pcf8574_config[idx] = Settings.pcf8574_config[idx] & ~(1 << i );
      }
    }
    //Settings.pcf8574_config[0] = (!strlen(webServer->arg("i2cs0").c_str())) ?  0 : atoi(webServer->arg("i2cs0").c_str());
    //AddLog_P2(LOG_LEVEL_INFO, PSTR("HTTP: I2C Board: %d, Config: %2x")),  idx, Settings.pcf8574_config[idx];

  }
}
#endif // USE_WEBSERVER


void pcf8574_switchrelay()
{
  uint8_t relay_state;

  for (uint8_t i = 0; i < devices_present; i++) {
    relay_state = bitRead(XdrvMailbox.index, i);
    //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("RSLT: max_pcf8574_devices %d requested pin %d"), max_pcf8574_devices,pcf8574_pin[i]);
    if (max_pcf8574_devices > 0 && pcf8574_pin[i] < 99) {
      uint8_t board = pcf8574_pin[i]>>3;
      uint8_t oldpinmask = _pcf8574pinMask[board];
      uint8_t _val = bitRead(rel_inverted, i) ? !relay_state : relay_state;
      //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("RSLT: pcf8574_switchrelay %d on pin %d"), i,state);

      if(_val) _pcf8574pinMask[board] |= _val << (pcf8574_pin[i]&0x7);
      else _pcf8574pinMask[board] &= ~(1 << (pcf8574_pin[i]&0x7));
      if (oldpinmask != _pcf8574pinMask[board]) {
        Wire.beginTransmission(pcf8574addr[board]);
        Wire.write(_pcf8574pinMask[board]);
        _error = Wire.endTransmission();
      }

      //pcf8574.write(pcf8574_pin[i]&0x7, rel_inverted[i] ? !state : state);
    }
  }
}

void  pcf8574_Init()
{
  for (byte i=0;i<64;i++) {
    pcf8574_pin[i]=99;
  }
  if (max_pcf8574_devices==0 && (pin[GPIO_I2C_SCL] < 99) && (pin[GPIO_I2C_SDA] < 99)) {
    pcf8574_detect();
    AddLog_P2(LOG_LEVEL_INFO, PSTR("RSLT: pcf8574 %d boards"), max_pcf8574_devices);
  }
  devices_present = devices_present - max_pcf8574_connected_ports; // reset no of devices to avoid duplicate ports on duplicate init.
  max_pcf8574_connected_ports = 0;  // reset no of devices to avoid duplicate ports on duplicate init.
  for (byte idx = 0; idx < max_pcf8574_devices; idx++) { // suport up to 8 boards PCF8574
   AddLog_P2(LOG_LEVEL_DEBUG, PSTR("RSLT: I2C Config: %d"), Settings.pcf8574_config[idx]);
    for (byte i = 0; i < 8; i++) {
      uint8_t _result = Settings.pcf8574_config[idx]>>i&1;
      //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("RSLT: I2C shift i %d: %d. Powerstate: %d, devices_present: %d"), i,_result, Settings.power>>i&1, devices_present);
      if (_result > 0) {
        pcf8574_pin[devices_present] = i + 8*idx;
        bitWrite(rel_inverted, devices_present, Settings.all_relays_inverted);
        devices_present++;
        max_pcf8574_connected_ports++;
      }
    }
  }
  AddLog_P2(LOG_LEVEL_INFO, PSTR("RSLT: Final max devices: %d, PCF8574 devices %d"), devices_present, max_pcf8574_connected_ports);
}

bool pcf8574_detect()
{
  if (pcf8574type) return true;
  bool success = false;

  for (byte i = 0; i < 8; i++) {
  //  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Probing addr: 0x%x for PCF8574"), PFC8574_ADDR1 + i);
    Wire.beginTransmission(PFC8574_ADDR1 + i);
    int16_t val = -1;
    val = Wire.endTransmission();
    if (val != -1 && !success) {
      success = true;
      pcf8574type = 1;
      strcpy(pcf8574stype, "PCF8574");
    }
    if (!val) {
      pcf8574addr[max_pcf8574_devices] = PFC8574_ADDR1 + i;
      max_pcf8574_devices++;
      AddLog_P2(LOG_LEVEL_DEBUG, S_LOG_I2C_FOUND_AT, pcf8574stype,  PFC8574_ADDR1 + i);
    }

  }
  return success;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XDRV_90 90

bool Xdrv90(uint8_t function)
{
  bool result = false;

  if (i2c_flg) {
    switch (function) {
      case FUNC_WEB_ADD_BUTTON:
        WSContentSend_P(HTTP_BTN_MENU_PCF8574);
        break;
      case FUNC_WEB_ADD_HANDLER:
        WebServer->on("/" WEB_HANDLE_PCF8574, handleI2C);
        break;
      case FUNC_PRE_INIT:
         pcf8574_Init();
        break;
      case FUNC_SET_POWER:
          pcf8574_switchrelay();
          break;
    }
  }

  return result;
}


#endif // USE_PCF8574
#endif // USE_I2C
