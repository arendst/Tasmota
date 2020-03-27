/*
  xdrv_28_pcf8574.ino - PCF8574 I2C support for Tasmota

  Copyright (C) 2020  Stefan Bode

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
 * PCF8574 - I2C IO Expander
 *
 * I2C Address: PCF8574  = 0x20 .. 0x27 (0x27 is not supported),
 *              PCF8574A = 0x39 .. 0x3F (0x38 is not supported)
\*********************************************************************************************/

#define XDRV_28           28
#define XI2C_02           2     // See I2CDEVICES.md

#define PCF8574_ADDR1     0x20  // PCF8574
#define PCF8574_ADDR2     0x38  // PCF8574A

struct PCF8574 {
  int error;
  uint8_t pin[64];
  uint8_t address[MAX_PCF8574];
  uint8_t pin_mask[MAX_PCF8574] = { 0 };
  uint8_t max_connected_ports = 0;        // Max numbers of devices comming from PCF8574 modules
  uint8_t max_devices = 0;                // Max numbers of PCF8574 modules
  char stype[9];
  bool type = false;
} Pcf8574;

void Pcf8574SwitchRelay(void)
{
  for (uint32_t i = 0; i < devices_present; i++) {
    uint8_t relay_state = bitRead(XdrvMailbox.index, i);

    //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("PCF: Pcf8574.max_devices %d requested pin %d"), Pcf8574.max_devices,Pcf8574.pin[i]);

    if (Pcf8574.max_devices > 0 && Pcf8574.pin[i] < 99) {
      uint8_t board = Pcf8574.pin[i]>>3;
      uint8_t oldpinmask = Pcf8574.pin_mask[board];
      uint8_t _val = bitRead(rel_inverted, i) ? !relay_state : relay_state;

      //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("PCF: Pcf8574SwitchRelay %d on pin %d"), i,state);

      if (_val) {
        Pcf8574.pin_mask[board] |= _val << (Pcf8574.pin[i]&0x7);
      } else {
        Pcf8574.pin_mask[board] &= ~(1 << (Pcf8574.pin[i]&0x7));
      }
      if (oldpinmask != Pcf8574.pin_mask[board]) {
        Wire.beginTransmission(Pcf8574.address[board]);
        Wire.write(Pcf8574.pin_mask[board]);
        Pcf8574.error = Wire.endTransmission();
      }
      //pcf8574.write(Pcf8574.pin[i]&0x7, rel_inverted[i] ? !state : state);
    }
  }
}

void Pcf8574Init(void)
{
  uint8_t pcf8574_address = PCF8574_ADDR1;
  while ((Pcf8574.max_devices < MAX_PCF8574) && (pcf8574_address < PCF8574_ADDR2 +8)) {

#ifdef USE_MCP230xx_ADDR
    if (USE_MCP230xx_ADDR == pcf8574_address) {
      AddLog_P2(LOG_LEVEL_INFO, PSTR("PCF: Address 0x%02x reserved for MCP320xx skipped"), pcf8574_address);
      pcf8574_address++;
      if ((PCF8574_ADDR1 +7) == pcf8574_address) {  // Support I2C addresses 0x20 to 0x26 and 0x39 to 0x3F
        pcf8574_address = PCF8574_ADDR2 +1;
      }
    }
#endif

  //  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("PCF: Probing addr: 0x%x for PCF8574"), pcf8574_address);

    if (I2cSetDevice(pcf8574_address)) {
      Pcf8574.type = true;

      Pcf8574.address[Pcf8574.max_devices] = pcf8574_address;
      Pcf8574.max_devices++;

      strcpy(Pcf8574.stype, "PCF8574");
      if (pcf8574_address >= PCF8574_ADDR2) {
        strcpy(Pcf8574.stype, "PCF8574A");
      }
      I2cSetActiveFound(pcf8574_address, Pcf8574.stype);
    }

    pcf8574_address++;
    if ((PCF8574_ADDR1 +7) == pcf8574_address) {  // Support I2C addresses 0x20 to 0x26 and 0x39 to 0x3F
      pcf8574_address = PCF8574_ADDR2 +1;
    }
  }
  if (Pcf8574.type) {
    for (uint32_t i = 0; i < sizeof(Pcf8574.pin); i++) {
      Pcf8574.pin[i] = 99;
    }
    devices_present = devices_present - Pcf8574.max_connected_ports; // reset no of devices to avoid duplicate ports on duplicate init.
    Pcf8574.max_connected_ports = 0;  // reset no of devices to avoid duplicate ports on duplicate init.
    for (uint32_t idx = 0; idx < Pcf8574.max_devices; idx++) { // suport up to 8 boards PCF8574

      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("PCF: Device %d config 0x%02x"), idx +1, Settings.pcf8574_config[idx]);

      for (uint32_t i = 0; i < 8; i++) {
        uint8_t _result = Settings.pcf8574_config[idx] >> i &1;
        //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("PCF: I2C shift i %d: %d. Powerstate: %d, devices_present: %d"), i,_result, Settings.power>>i&1, devices_present);
        if (_result > 0) {
          Pcf8574.pin[devices_present] = i + 8 * idx;
          bitWrite(rel_inverted, devices_present, Settings.flag3.pcf8574_ports_inverted);  // SetOption81 - Invert all ports on PCF8574 devices
          devices_present++;
          Pcf8574.max_connected_ports++;
        }
      }
    }
    AddLog_P2(LOG_LEVEL_INFO, PSTR("PCF: Total devices %d, PCF8574 output ports %d"), Pcf8574.max_devices, Pcf8574.max_connected_ports);
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
  "<p><input id='b1' name='b1' type='checkbox'%s><b>" D_INVERT_PORTS "</b></p><hr/>";

const char HTTP_FORM_I2C_PCF8574_2[] PROGMEM =
  "<tr><td><b>" D_DEVICE " %d " D_PORT " %d</b></td><td style='width:100px'><select id='i2cs%d' name='i2cs%d'>"
  "<option%s value='0'>" D_DEVICE_INPUT "</option>"
  "<option%s value='1'>" D_DEVICE_OUTPUT "</option>"
  "</select></td></tr>";

void HandlePcf8574(void)
{
  if (!HttpCheckPriviledgedAccess()) { return; }

  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP  D_CONFIGURE_PCF8574));

  if (WebServer->hasArg("save")) {
    Pcf8574SaveSettings();
    WebRestart(1);
    return;
  }

  WSContentStart_P(D_CONFIGURE_PCF8574);
  WSContentSendStyle();
  WSContentSend_P(HTTP_FORM_I2C_PCF8574_1, (Settings.flag3.pcf8574_ports_inverted) ? " checked" : "");  // SetOption81 - Invert all ports on PCF8574 devices
  WSContentSend_P(HTTP_TABLE100);
  for (uint32_t idx = 0; idx < Pcf8574.max_devices; idx++) {
    for (uint32_t idx2 = 0; idx2 < 8; idx2++) {  // 8 ports on PCF8574
      uint8_t helper = 1 << idx2;
      WSContentSend_P(HTTP_FORM_I2C_PCF8574_2,
        idx +1, idx2,
        idx2 + 8*idx,
        idx2 + 8*idx,
        ((helper & Settings.pcf8574_config[idx]) >> idx2 == 0) ? " selected " : " ",
        ((helper & Settings.pcf8574_config[idx]) >> idx2 == 1) ? " selected " : " "
      );
    }
  }
  WSContentSend_P(PSTR("</table>"));
  WSContentSend_P(HTTP_FORM_END);
  WSContentSpaceButton(BUTTON_CONFIGURATION);
  WSContentStop();
}

void Pcf8574SaveSettings(void)
{
  char stemp[7];
  char tmp[100];

  //AddLog_P(LOG_LEVEL_DEBUG, PSTR("PCF: Start working on Save arguements: inverted:%d")), WebServer->hasArg("b1");

  Settings.flag3.pcf8574_ports_inverted = WebServer->hasArg("b1");  // SetOption81 - Invert all ports on PCF8574 devices
  for (byte idx = 0; idx < Pcf8574.max_devices; idx++) {
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
        Pcf8574.max_connected_ports++;
      } else {
        Settings.pcf8574_config[idx] = Settings.pcf8574_config[idx] & ~(1 << i );
      }
    }
    //Settings.pcf8574_config[0] = (!strlen(webServer->arg("i2cs0").c_str())) ?  0 : atoi(webServer->arg("i2cs0").c_str());
    //AddLog_P2(LOG_LEVEL_INFO, PSTR("PCF: I2C Board: %d, Config: %2x")),  idx, Settings.pcf8574_config[idx];

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
    Pcf8574Init();
  }
  else if (Pcf8574.type) {
    switch (function) {
      case FUNC_SET_POWER:
        Pcf8574SwitchRelay();
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_ADD_BUTTON:
        WSContentSend_P(HTTP_BTN_MENU_PCF8574);
        break;
      case FUNC_WEB_ADD_HANDLER:
        WebServer->on("/" WEB_HANDLE_PCF8574, HandlePcf8574);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_PCF8574
#endif  // USE_I2C
