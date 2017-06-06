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

#ifdef USE_PCF8574

#include <pcf8574_esp.h>

#define PFC8574_ADDR1        0x38

uint8_t pcf8574_pin[64], pcf8574addr[8], pcf8574type = 0;
uint8_t max_pcf8574_devices = 0;
char pcf8574stype[8];


#ifdef USE_WEBSERVER
const char HTTP_FORM_I2C_PCF8574_1[] PROGMEM =
  "<fieldset><legend><b>&nbsp;PCF8674 parameters &nbsp;</b></legend><form method='post' action='sv'>"
  "<input id='w' name='w' value='7' hidden><input id='r' name='r' value='1' hidden>"
  "<br/><input style='width:10%;float:left' id='b1' name='b1' type='checkbox'{r1}><b>Reverse Relays</b><br/>";
const char HTTP_FORM_I2C_PCF8574_2[] PROGMEM =
  "<br/><b>{b0} IN/OUT</b> <br/><select id='{b2}' name='{b2}'>"
  "<option{a0value='0'>0 Input</option>"
  "<option{a1value='1'>1 Output</option>"
  "</select></br>";

void handleI2C()
{
  if (httpUser()) return;
  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Configure PCF8574"));

  String page = FPSTR(HTTP_HEAD);

  page.replace("{v}", "Configure I2C");
  page += FPSTR(HTTP_FORM_I2C_PCF8574_1);
  page.replace("{r1}", (sysCfg.all_relays_inverted) ? " checked" : "");
  for (byte idx = 0; idx < max_pcf8574_devices; idx++) {
    page.replace("{b1}", String(idx));
    for (byte idx2 = 0; idx2 < 8; idx2++) {
      page += FPSTR(HTTP_FORM_I2C_PCF8574_2);
      page.replace("{b0}", "Board: "+  String(idx) + "  I2C P" + String(idx2));
      page.replace("{b2}", "i2cs" + String(idx2+8*idx));
      for (byte i = 0; i < 2; i++) {
        char log[LOGSZ];
        byte helper = 1 << idx2;
        page.replace("{a" + String(i), ((helper & sysCfg.pcf8574_config[idx]) >> idx2 == i) ? " selected " : " ");
      }
    }
  }
  page += FPSTR(HTTP_FORM_END);

  if (_httpflag == HTTP_MANAGER) {
    _httpflag = HTTP_ADMIN;
  } else {
    page += FPSTR(HTTP_BTN_CONF);
  }
  showPage(page);
}

void pcf8574_saveSettings()
{
  char stemp[7];
  sysCfg.all_relays_inverted = webServer->hasArg("b1");
  for (byte idx = 0; idx < max_pcf8574_devices; idx++) {
    byte count=0;
    byte n = sysCfg.pcf8574_config[idx];
    while(n!=0) {
      n = n&(n-1);
      count++;
    }
    if (count <= Maxdevice) {Maxdevice = Maxdevice - count;}
    for (byte i = 0; i < 8; i++) {
      snprintf_P(stemp, sizeof(stemp), PSTR("i2cs%d"), i+8*idx);
      byte _value = (!strlen(webServer->arg(stemp).c_str() )) ?  0 : atoi(webServer->arg(stemp).c_str() );
      if (_value) {
        sysCfg.pcf8574_config[idx] = sysCfg.pcf8574_config[idx] | 1 << i;
        Maxdevice++;
      } else {
        sysCfg.pcf8574_config[idx] = sysCfg.pcf8574_config[idx] & ~(1 << i );
      }
    }
    //sysCfg.pcf8574_config[0] = (!strlen(webServer->arg("i2cs0").c_str())) ?  0 : atoi(webServer->arg("i2cs0").c_str());
    //snprintf_P(log, sizeof(log), PSTR("HTTP: I2C Board: %d, Config: %2x"),  idx, sysCfg.pcf8574_config[idx]);
    //addLog(LOG_LEVEL_INFO, log);
  }
}
#endif // USE_WEBSERVER


void pcf8574_switchrelay(byte i, uint8_t state)
{
  //char log[TOPSZ];
  if (max_pcf8574_devices > 0) {
    uint8_t board = pcf8574_pin[i]>>3;
    PCF857x pcf8574(pcf8574addr[board], &Wire);
    //snprintf_P(log, sizeof(log), PSTR("RSLT: Setting I2C PCF8574 Relay Number %d (pin P%d, board %d,0x%2x, boardpin %d) to state %d, inverted %d"), i, pcf8574_pin[i], board,pcf8574addr[board],pcf8574_pin[i]&0x7, state, rel_inverted[i]);
    //addLog(LOG_LEVEL_INFO, log);
    pcf8574.write(pcf8574_pin[i]&0x7, rel_inverted[i] ? !state : state);
  }
}

void  pcf8574_Init()
{
  char log[TOPSZ + MESSZ];
  //for (byte i=0;i<64;i++) pcf8574_pin[i]=99;
  if (max_pcf8574_devices==0 && (pin[GPIO_I2C_SCL] < 99) && (pin[GPIO_I2C_SDA] < 99)) {
    pcf8574_detect();
    snprintf_P(log, sizeof(log), PSTR("RSLT: pcf8574 %d boards"), max_pcf8574_devices);
    addLog(LOG_LEVEL_INFO, log);
  }
  for (byte idx = 0; idx < max_pcf8574_devices; idx++) { // suport up to 8 boards PCF8574
    PCF857x pcf8574(pcf8574addr[idx], &Wire);
    //snprintf_P(log, sizeof(log), PSTR("RSLT: I2C Config: %d"), sysCfg.pcf8574_config[idx]);
    //addLog(LOG_LEVEL_INFO, log);
    for (byte i = 0; i < 8; i++) {
      uint8_t _result = sysCfg.pcf8574_config[idx]>>i&1;
      //snprintf_P(log, sizeof(log), PSTR("RSLT: I2C shift i %d: %d. Powerstate: %d"), i,_result, sysCfg.power>>i&1);
      //addLog(LOG_LEVEL_INFO, log);
      if (_result > 0) {
        pcf8574_pin[Maxdevice] = i + 8*idx;
        rel_inverted[Maxdevice] = sysCfg.all_relays_inverted;
        Maxdevice++;
      }
    }
  }
  snprintf_P(log, sizeof(log), PSTR("RSLT: Final max devices: %d"), Maxdevice);
  addLog(LOG_LEVEL_INFO, log);
}

boolean pcf8574_detect()
{
  if (pcf8574type) return true;
  boolean success = false;

  for (byte i = 0; i < 8; i++) {
    PCF857x _pcf8574(PFC8574_ADDR1 + i, &Wire);
    int16_t val = -1;
    _pcf8574.begin();

    val = _pcf8574.lastError();
    if (val != -1 && !success) {
      success = true;
      pcf8574type = 1;
      strcpy(pcf8574stype, "PFC8574");
    }
    if (!val) {
      pcf8574addr[max_pcf8574_devices] = PFC8574_ADDR1 + i;
      max_pcf8574_devices++;
    }

  }
  return success;
}

#endif // USE_PCF8574
