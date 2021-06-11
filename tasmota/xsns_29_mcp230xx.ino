/*
  xsns_29_mcp230xx.ino - Support for I2C MCP23008/MCP23017 GPIO Expander on Tasmota

  Copyright (C) 2021  Andre Thomas and Theo Arends

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

   I2C Address: 0x20 - 0x26 (0x27 is not supported)
\*********************************************************************************************/

#define XSNS_29                   29
#define XI2C_22                   22  // See I2CDEVICES.md

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
#ifdef USE_MCP230xx_OUTPUT
uint8_t MCP230xx_OLAT           = 0x0A;
#endif // USE_MCP230xx_OUTPUT

uint8_t mcp230xx_type = 0;
uint8_t mcp230xx_pincount = 0;
uint8_t mcp230xx_outpincount = 0;
#ifdef USE_MCP230xx_OUTPUT
uint8_t mcp230xx_outpinmapping[16];
uint8_t mcp230xx_keepout_no_toggle = 0;
#endif
uint8_t mcp230xx_int_en = 0;
uint8_t mcp230xx_int_prio_counter = 0;
uint8_t mcp230xx_int_counter_en = 0;
uint8_t mcp230xx_int_retainer_en = 0;
uint8_t mcp230xx_int_sec_counter = 0;

uint8_t mcp230xx_int_report_defer_counter[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

uint16_t mcp230xx_int_counter[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

uint8_t mcp230xx_int_retainer[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // Used to store if an interrupt occured that needs to be retained until teleperiod

unsigned long int_millis[16]; // To keep track of millis() since last interrupt

const char MCP230XX_SENSOR_RESPONSE[] PROGMEM = "{\"Sensor29_D%i\":{\"MODE\":%i,\"PULL_UP\":\"%s\",\"INT_MODE\":\"%s\",\"STATE\":\"%s\"}}";

const char MCP230XX_INTCFG_RESPONSE[] PROGMEM = "{\"MCP230xx_INT%s\":{\"D_%i\":%i}}";

#ifdef USE_MCP230xx_OUTPUT
const char MCP230XX_CMND_RESPONSE[] PROGMEM = "{\"S29cmnd_D%i\":{\"COMMAND\":\"%s\",\"STATE\":\"%s\"}}";
const char MCP230XX_OUTPUT_RESPONSE[] PROGMEM = "{\"Sensor29_D%i\":{\"MODE\":%i,\"START_UP\":\"%s\",\"STATE\":\"%s\"}}";
#endif // USE_MCP230xx_OUTPUT

void MCP230xx_CheckForIntCounter(void) {
  uint8_t en = 0;
  for (uint32_t ca=0;ca<16;ca++) {
    if (Settings->mcp230xx_config[ca].int_count_en) {
      en=1;
    }
  }
  if (!Settings->mcp230xx_int_timer) en=0;
  mcp230xx_int_counter_en=en;
  if (!mcp230xx_int_counter_en) { // Interrupt counters are disabled, so we clear all the counters
    for (uint32_t ca=0;ca<16;ca++) {
      mcp230xx_int_counter[ca] = 0;
    }
  }
}

void MCP230xx_CheckForIntRetainer(void) {
  uint8_t en = 0;
  for (uint32_t ca=0;ca<16;ca++) {
    if (Settings->mcp230xx_config[ca].int_retain_flag) {
      en=1;
    }
  }
  mcp230xx_int_retainer_en=en;
  if (!mcp230xx_int_retainer_en) { // Interrupt counters are disabled, so we clear all the counters
    for (uint32_t ca=0;ca<16;ca++) {
      mcp230xx_int_retainer[ca] = 0;
    }
  }
}

#ifdef USE_MCP230xx_OUTPUT
const char* ConvertNumTxt(uint8_t statu, uint8_t pinmod=0, uint8_t config=0) {
#else // not USE_MCP230xx_OUTPUT
const char* ConvertNumTxt(uint8_t statu, uint8_t pinmod=0) {
#endif // USE_MCP230xx_OUTPUT
#ifdef USE_MCP230xx_OUTPUT
  if ((6 == pinmod) && (statu < 2)) { statu = 1-statu; }
  if ((config) && (Settings->flag.save_state)) {
    return "SAVED";
  }
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
#ifdef USE_MCP230xx_OUTPUT
      if (config) {
        return "DEVICE";
      } else {
#endif // USE_MCP230xx_OUTPUT
        return "TOGGLE";
#ifdef USE_MCP230xx_OUTPUT
      }
#endif // USE_MCP230xx_OUTPUT
      break;
#endif // USE_MCP230xx_OUTPUT
  }
  return "";
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
  }
  return "";
}

uint8_t MCP230xx_readGPIO(uint8_t port) {
  return I2cRead8(USE_MCP230xx_ADDR, MCP230xx_GPIO + port);
}

void MCP230xx_ApplySettings(void)
{
#ifdef USE_MCP230xx_OUTPUT
  TasmotaGlobal.devices_present -= mcp230xx_outpincount;
  mcp230xx_outpincount = 0;
  for (uint32_t idx = 0; idx < mcp230xx_pincount; idx++) {
    if (Settings->mcp230xx_config[idx].pinmode >= 5) {
      mcp230xx_outpinmapping[mcp230xx_outpincount] = idx;
      mcp230xx_outpincount++;
    }
    int_millis[idx]=millis();
  }
  TasmotaGlobal.devices_present += mcp230xx_outpincount;
#endif // USE_MCP230xx_OUTPUT
  uint8_t int_en = 0;
  uint8_t reg_portpins[mcp230xx_type];
  for (uint32_t mcp230xx_port = 0; mcp230xx_port < mcp230xx_type; mcp230xx_port++) {
    uint8_t reg_gppu = 0;
    uint8_t reg_gpinten = 0;
    uint8_t reg_iodir = 0xFF;
#ifdef USE_MCP230xx_OUTPUT
    reg_portpins[mcp230xx_port] = 0x00;
    uint8_t reg_readpins = I2cRead8(USE_MCP230xx_ADDR, MCP230xx_OLAT + mcp230xx_port);
#endif // USE_MCP230xx_OUTPUT
    for (uint32_t idx = 0; idx < 8; idx++) {
      switch (Settings->mcp230xx_config[idx+(mcp230xx_port*8)].pinmode) {
        case 0 ... 1:
          reg_iodir |= (1 << idx);
          break;
        case 2 ... 4:
          reg_iodir |= (1 << idx);
          reg_gpinten |= (1 << idx);
          int_en = 1;
          break;
#ifdef USE_MCP230xx_OUTPUT
        case 5 ... 6:
          reg_iodir &= ~(1 << idx);
          if (Settings->flag.save_state) {  // SetOption0 - Save power state and use after restart - Firmware configuration wants us to use the last pin state
            reg_portpins[mcp230xx_port] |= (Settings->mcp230xx_config[idx+(mcp230xx_port*8)].saved_state << idx);
          } else {
            if (Settings->mcp230xx_config[idx+(mcp230xx_port*8)].keep_output) { // Read the value to use from the MCP230xx
              reg_portpins[mcp230xx_port] |= reg_readpins & (1 << idx);
              mcp230xx_keepout_no_toggle++;
            }
            else if (Settings->mcp230xx_config[idx+(mcp230xx_port*8)].pullup) {
              reg_portpins[mcp230xx_port] |= (1 << idx);
            }
          }
          break;
#endif // USE_MCP230xx_OUTPUT
        default:
          break;
      }
#ifdef USE_MCP230xx_OUTPUT
      if ((Settings->mcp230xx_config[idx+(mcp230xx_port*8)].pullup) && (Settings->mcp230xx_config[idx+(mcp230xx_port*8)].pinmode < 5)) {
        reg_gppu |= (1 << idx);
      }
#else // not USE_MCP230xx_OUTPUT
      if (Settings->mcp230xx_config[idx+(mcp230xx_port*8)].pullup) {
        reg_gppu |= (1 << idx);
      }
#endif // USE_MCP230xx_OUTPUT
    }
    I2cWrite8(USE_MCP230xx_ADDR, MCP230xx_GPPU+mcp230xx_port, reg_gppu);
    I2cWrite8(USE_MCP230xx_ADDR, MCP230xx_GPINTEN+mcp230xx_port, reg_gpinten);
#ifdef USE_MCP230xx_OUTPUT
    I2cWrite8(USE_MCP230xx_ADDR, MCP230xx_IODIR+mcp230xx_port, reg_iodir);
#endif // USE_MCP230xx_OUTPUT
  }
#ifdef USE_MCP230xx_OUTPUT
  for (uint32_t mcp230xx_port = 0; mcp230xx_port < mcp230xx_type; mcp230xx_port++) {
    for (uint32_t idx = 0; idx < mcp230xx_outpincount; idx++) {
      if (mcp230xx_port ? mcp230xx_outpinmapping[idx] > 7 : mcp230xx_outpinmapping[idx] < 8) {
        uint8_t relay_no = TasmotaGlobal.devices_present - mcp230xx_outpincount + idx + 1;
        if (mcp230xx_keepout_no_toggle >0) mcp230xx_keepout_no_toggle--;
        ExecuteCommandPower(relay_no, (reg_portpins[mcp230xx_port] >> (mcp230xx_outpinmapping[idx] & 7)) & 1, SRC_IGNORE);
      }
    }
  }
#endif // USE_MCP230xx_OUTPUT
  mcp230xx_int_en = int_en;
  MCP230xx_CheckForIntCounter();  // update register on whether or not we should be counting interrupts
  MCP230xx_CheckForIntRetainer(); // update register on whether or not we should be retaining interrupt events for teleperiod
}

void MCP230xx_Detect(void)
{
  if (I2cActive(USE_MCP230xx_ADDR)) { return; }

  uint8_t buffer;

  I2cWrite8(USE_MCP230xx_ADDR, MCP230xx_IOCON, 0x80); // attempt to set bank mode - this will only work on MCP23017, so its the best way to detect the different chips 23008 vs 23017
  if (I2cValidRead8(&buffer, USE_MCP230xx_ADDR, MCP230xx_IOCON)) {
    if (0x00 == buffer) {
      mcp230xx_type = 1; // We have a MCP23008
      I2cSetActiveFound(USE_MCP230xx_ADDR, "MCP23008");
      mcp230xx_pincount = 8;
      MCP230xx_ApplySettings();
    } else {
      if (0x80 == buffer) {
        mcp230xx_type = 2; // We have a MCP23017
        I2cSetActiveFound(USE_MCP230xx_ADDR, "MCP23017");
        mcp230xx_pincount = 16;
        // Reset bank mode to 0
        I2cWrite8(USE_MCP230xx_ADDR, MCP230xx_IOCON, 0x00);
        // Update register locations for MCP23017
        MCP230xx_GPINTEN        = 0x04;
        MCP230xx_GPPU           = 0x0C;
        MCP230xx_INTF           = 0x0E;
        MCP230xx_INTCAP         = 0x10;
        MCP230xx_GPIO           = 0x12;
#ifdef USE_MCP230xx_OUTPUT
        MCP230xx_OLAT           = 0x14;
#endif // USE_MCP230xx_OUTPUT
        MCP230xx_ApplySettings();
      }
    }
  }
}

void MCP230xx_CheckForInterrupt(void) {
  uint8_t intf;
  uint8_t mcp230xx_intcap = 0;
  uint8_t report_int;
  for (uint32_t mcp230xx_port = 0; mcp230xx_port < mcp230xx_type; mcp230xx_port++) {
    if (I2cValidRead8(&intf,USE_MCP230xx_ADDR,MCP230xx_INTF+mcp230xx_port)) {
      if (intf > 0) {
        if (I2cValidRead8(&mcp230xx_intcap, USE_MCP230xx_ADDR, MCP230xx_INTCAP+mcp230xx_port)) {
          for (uint32_t intp = 0; intp < 8; intp++) {
            if ((intf >> intp) & 0x01) { // we know which pin caused interrupt
              report_int = 0;
              if (Settings->mcp230xx_config[intp+(mcp230xx_port*8)].pinmode > 1) {
                switch (Settings->mcp230xx_config[intp+(mcp230xx_port*8)].pinmode) {
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
                // Check for interrupt counter
                if ((mcp230xx_int_counter_en) && (report_int)) { // We may have some counting to do
                  if (Settings->mcp230xx_config[intp+(mcp230xx_port*8)].int_count_en) { // Indeed, for this pin
                    mcp230xx_int_counter[intp+(mcp230xx_port*8)]++;
                  }
                }
                // check for interrupt defer on this pin
                if (report_int) {
                  if (Settings->mcp230xx_config[intp+(mcp230xx_port*8)].int_report_defer) {
                    mcp230xx_int_report_defer_counter[intp+(mcp230xx_port*8)]++;
                    if (mcp230xx_int_report_defer_counter[intp+(mcp230xx_port*8)] >= Settings->mcp230xx_config[intp+(mcp230xx_port*8)].int_report_defer) {
                      mcp230xx_int_report_defer_counter[intp+(mcp230xx_port*8)]=0;
                    } else {
                      report_int = 0; // defer int report for now
                    }
                  }
                }
                // check if interrupt retain is used, if it is for this pin then we do not report immediately as it will be reported in teleperiod
                if (report_int) {
                  if (Settings->mcp230xx_config[intp+(mcp230xx_port*8)].int_retain_flag) {
                    mcp230xx_int_retainer[intp+(mcp230xx_port*8)] = 1;
                    report_int = 0; // do not report for now
                  }
                }
                if (Settings->mcp230xx_config[intp+(mcp230xx_port*8)].int_count_en) { // We do not want to report via tele or event if counting is enabled
                  report_int = 0;
                }
                if (report_int) {
                  bool int_tele = false;
                  bool int_event = false;
                  unsigned long millis_now = millis();
                  unsigned long millis_since_last_int = millis_now - int_millis[intp+(mcp230xx_port*8)];
                  int_millis[intp+(mcp230xx_port*8)]=millis_now;
                  switch (Settings->mcp230xx_config[intp+(mcp230xx_port*8)].int_report_mode) {
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
                    ResponseTime_P(PSTR(",\"MCP230XX_INT\":{\"D%i\":%i,\"MS\":%lu}}"),
                      intp+(mcp230xx_port*8), ((mcp230xx_intcap >> intp) & 0x01),millis_since_last_int);
                    MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, PSTR("MCP230XX_INT"));
                    if (Settings->flag3.hass_tele_on_power) {  // SetOption59 - Send tele/%topic%/SENSOR in addition to stat/%topic%/RESULT
                        MqttPublishSensor();
                    }
                  }
                  if (int_event) {
                    char command[19]; // Theoretical max = 'event MCPINT_D16=1' so 18 + 1 (for the \n)
                    sprintf(command,"event MCPINT_D%i=%i",intp+(mcp230xx_port*8),((mcp230xx_intcap >> intp) & 0x01));
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
}

void MCP230xx_Show(bool json)
{
  if (json) {
    uint8_t gpio = MCP230xx_readGPIO(0);
    ResponseAppend_P(PSTR(",\"MCP230XX\":{\"D0\":%i,\"D1\":%i,\"D2\":%i,\"D3\":%i,\"D4\":%i,\"D5\":%i,\"D6\":%i,\"D7\":%i"),
                (gpio>>0)&1,(gpio>>1)&1,(gpio>>2)&1,(gpio>>3)&1,(gpio>>4)&1,(gpio>>5)&1,(gpio>>6)&1,(gpio>>7)&1);
    uint8_t gpiob = 0;
    if (2 == mcp230xx_type) {
      gpiob = MCP230xx_readGPIO(1);
      ResponseAppend_P(PSTR(",\"D8\":%i,\"D9\":%i,\"D10\":%i,\"D11\":%i,\"D12\":%i,\"D13\":%i,\"D14\":%i,\"D15\":%i"),
                  (gpiob>>0)&1, (gpiob>>1)&1, (gpiob>>2)&1, (gpiob>>3)&1, (gpiob>>4)&1, (gpiob>>5)&1, (gpiob>>6)&1, (gpiob>>7)&1);
    }

#ifdef USE_MCP230xx_OUTPUT
    uint8_t outputcount = 0;
    for (uint32_t pinx = 0; pinx < mcp230xx_pincount; pinx++) {
      if (Settings->mcp230xx_config[pinx].pinmode >= 5) { outputcount++; }
    }
    if (outputcount) {
      uint16_t gpiototal = ((uint16_t)gpiob << 8) | gpio;
      ResponseAppend_P(PSTR(",\"OUT\":{"));
      char stt[7];
      bool first = true;
      for (uint32_t pinx = 0; pinx < mcp230xx_pincount; pinx++) {
        if (Settings->mcp230xx_config[pinx].pinmode >= 5) {
          sprintf(stt, ConvertNumTxt(((gpiototal>>pinx)&1), Settings->mcp230xx_config[pinx].pinmode));
          ResponseAppend_P(PSTR("%s\"OUT_D%i\":\"%s\""), (first) ? "" : ",", pinx, stt);
          first = false;
        }
      }
      ResponseAppend_P(PSTR("}"));
    }
#endif  // USE_MCP230xx_OUTPUT
    ResponseJsonEnd();
  }
}

#ifdef USE_MCP230xx_OUTPUT

void MCP230xx_SetOutPin(uint8_t pin,uint8_t pinstate) {
  uint8_t portpins;
  uint8_t port = 0;
  uint8_t pinmo = Settings->mcp230xx_config[pin].pinmode;
  uint8_t interlock = Settings->flag.interlock;  // CMND_INTERLOCK - Enable/disable interlock
  int pinadd = (pin % 2)+1-(3*(pin % 2)); //check if pin is odd or even and convert to 1 (if even) or -1 (if odd)
  char cmnd[7], stt[4];
  if (pin > 7) { port = 1; }
  portpins = MCP230xx_readGPIO(port);

  if (pinstate < 2) {
    if (pinstate) portpins |= (1 << (pin-(port*8))); else portpins &= ~(1 << (pin-(port*8)));
  } else {
    portpins ^= (1 << (pin-(port*8)));
  }

  I2cWrite8(USE_MCP230xx_ADDR, MCP230xx_GPIO + port, portpins);
  if (Settings->flag.save_state) {  // SetOption0 - Save power state and use after restart - Firmware configured to save last known state in settings
    Settings->mcp230xx_config[pin].saved_state=(portpins>>(pin-(port*8))&1)^(pinmo-5);
    Settings->mcp230xx_config[pin+pinadd].saved_state=(portpins>>(pin+pinadd-(port*8))&1)^(pinmo-5);
  }
  sprintf(cmnd,ConvertNumTxt(pinstate, pinmo));
  sprintf(stt,ConvertNumTxt((portpins >> (pin-(port*8))&1), pinmo));
  if (interlock && (pinmo == Settings->mcp230xx_config[pin+pinadd].pinmode)) {
    char stt1[4];
    sprintf(stt1,ConvertNumTxt((portpins >> (pin+pinadd-(port*8))&1), pinmo));
    Response_P(PSTR("{\"S29cmnd_D%i\":{\"COMMAND\":\"%s\",\"STATE\":\"%s\"},\"S29cmnd_D%i\":{\"STATE\":\"%s\"}}"),pin, cmnd, stt, pin+pinadd, stt1);
  } else {
    Response_P(MCP230XX_CMND_RESPONSE, pin, cmnd, stt);
  }
}

#endif // USE_MCP230xx_OUTPUT

void MCP230xx_Reset(uint8_t pinmode) {
  uint8_t pullup = 0;
  if ((pinmode > 1) && (pinmode < 5)) { pullup=1; }
  for (uint32_t pinx=0;pinx<16;pinx++) {
    Settings->mcp230xx_config[pinx].pinmode=pinmode;
    Settings->mcp230xx_config[pinx].pullup=pullup;
    Settings->mcp230xx_config[pinx].saved_state=0;
    if ((pinmode > 1) && (pinmode < 5)) {
      Settings->mcp230xx_config[pinx].int_report_mode=0; // Enabled for ALL by default
    } else {
      Settings->mcp230xx_config[pinx].int_report_mode=3; // Disabled for pinmode 1, 5 and 6 (No interrupts there)
    }
    Settings->mcp230xx_config[pinx].int_report_defer=0; // Disabled
    Settings->mcp230xx_config[pinx].int_count_en=0;     // Disabled by default
    Settings->mcp230xx_config[pinx].int_retain_flag=0;  // Disabled by default
    Settings->mcp230xx_config[pinx].keep_output=0;      // Disabled by default
    Settings->mcp230xx_config[pinx].spare14=0;
    Settings->mcp230xx_config[pinx].spare15=0;
  }
  Settings->mcp230xx_int_prio = 0; // Once per FUNC_EVERY_50_MSECOND callback
  Settings->mcp230xx_int_timer = 0;
  MCP230xx_ApplySettings();
#ifdef USE_MCP230xx_OUTPUT
  if (pinmode > 4) {
    char startuptxt[7];
    sprintf(startuptxt,ConvertNumTxt(pullup, 0, 1));
    Response_P(MCP230XX_OUTPUT_RESPONSE,99,pinmode,startuptxt,startuptxt);
  } else {
#endif // USE_MCP230xx_OUTPUT
    char pulluptxt[7];
    char intmodetxt[9];
    sprintf(pulluptxt,ConvertNumTxt(pullup));
    uint8_t intmode = 3;
    if ((pinmode > 1) && (pinmode < 5)) { intmode = 0; }
    sprintf(intmodetxt,IntModeTxt(intmode));
    Response_P(MCP230XX_SENSOR_RESPONSE,99,pinmode,pulluptxt,intmodetxt,"");
#ifdef USE_MCP230xx_OUTPUT
  }
#endif // USE_MCP230xx_OUTPUT
}

bool MCP230xx_Command(void)
{
  bool serviced = true;
  bool validpin = false;
  uint8_t paramcount = 0;
  if (XdrvMailbox.data_len > 0) {
    paramcount=1;
  } else {
    serviced = false;
    return serviced;
  }
  char argument[XdrvMailbox.data_len];
  for (uint32_t ca=0;ca<XdrvMailbox.data_len;ca++) {
    if ((' ' == XdrvMailbox.data[ca]) || ('=' == XdrvMailbox.data[ca])) { XdrvMailbox.data[ca] = ','; }
    if (',' == XdrvMailbox.data[ca]) { paramcount++; }
  }
  UpperCase(XdrvMailbox.data,XdrvMailbox.data);
  if (!strcmp(ArgV(argument, 1),"RESET"))  {  MCP230xx_Reset(1); return serviced; }
  if (!strcmp(ArgV(argument, 1),"RESET1")) {  MCP230xx_Reset(1); return serviced; }
  if (!strcmp(ArgV(argument, 1),"RESET2")) {  MCP230xx_Reset(2); return serviced; }
  if (!strcmp(ArgV(argument, 1),"RESET3")) {  MCP230xx_Reset(3); return serviced; }
  if (!strcmp(ArgV(argument, 1),"RESET4")) {  MCP230xx_Reset(4); return serviced; }
#ifdef USE_MCP230xx_OUTPUT
  if (!strcmp(ArgV(argument, 1),"RESET5")) {  MCP230xx_Reset(5); return serviced; }
  if (!strcmp(ArgV(argument, 1),"RESET6")) {  MCP230xx_Reset(6); return serviced; }
#endif // USE_MCP230xx_OUTPUT

  if (!strcmp(ArgV(argument, 1),"INTPRI")) {
    if (paramcount > 1) {
      uint8_t intpri = atoi(ArgV(argument, 2));
      if ((intpri >= 0) && (intpri <= 20)) {
        Settings->mcp230xx_int_prio = intpri;
        Response_P(MCP230XX_INTCFG_RESPONSE,"PRI",99,Settings->mcp230xx_int_prio);  // "{\"MCP230xx_INT%s\":{\"D_%i\":%i}}";
        return serviced;
      }
    } else { // No parameter was given for INTPRI so we return the current configured value
      Response_P(MCP230XX_INTCFG_RESPONSE,"PRI",99,Settings->mcp230xx_int_prio);  // "{\"MCP230xx_INT%s\":{\"D_%i\":%i}}";
      return serviced;
    }
  }

  if (!strcmp(ArgV(argument, 1),"INTTIMER")) {
    if (paramcount > 1) {
      uint8_t inttim = atoi(ArgV(argument, 2));
      if ((inttim >= 0) && (inttim <= 3600)) {
        Settings->mcp230xx_int_timer = inttim;
        MCP230xx_CheckForIntCounter(); // update register on whether or not we should be counting interrupts
        Response_P(MCP230XX_INTCFG_RESPONSE,"TIMER",99,Settings->mcp230xx_int_timer);  // "{\"MCP230xx_INT%s\":{\"D_%i\":%i}}";
        return serviced;
      }
    } else { // No parameter was given for INTTIM so we return the current configured value
      Response_P(MCP230XX_INTCFG_RESPONSE,"TIMER",99,Settings->mcp230xx_int_timer);  // "{\"MCP230xx_INT%s\":{\"D_%i\":%i}}";
      return serviced;
    }
  }

  if (!strcmp(ArgV(argument, 1),"INTDEF")) {
    if (paramcount > 1) {
      uint8_t pin = atoi(ArgV(argument, 2));
      if (pin < mcp230xx_pincount) {
        if (pin == 0) {
          if (!strcmp(ArgV(argument, 2), "0")) validpin=true;
        } else {
          validpin = true;
        }
      }
      if (validpin) {
        if (paramcount > 2) {
          uint8_t intdef = atoi(ArgV(argument, 3));
          if ((intdef >= 0) && (intdef <= 15)) {
            Settings->mcp230xx_config[pin].int_report_defer=intdef;
            if (Settings->mcp230xx_config[pin].int_count_en) {
              Settings->mcp230xx_config[pin].int_count_en=0;
              MCP230xx_CheckForIntCounter();
              AddLog(LOG_LEVEL_INFO, PSTR("*** WARNING *** - Disabled INTCNT for pin D%i"),pin);
            }
            Response_P(MCP230XX_INTCFG_RESPONSE,"DEF",pin,Settings->mcp230xx_config[pin].int_report_defer);  // "{\"MCP230xx_INT%s\":{\"D_%i\":%i}}";
            return serviced;
          } else {
            serviced=false;
            return serviced;
          }
        } else {
          Response_P(MCP230XX_INTCFG_RESPONSE,"DEF",pin,Settings->mcp230xx_config[pin].int_report_defer);  // "{\"MCP230xx_INT%s\":{\"D_%i\":%i}}";
          return serviced;
        }
      }
      serviced = false;
      return serviced;
    } else {
      serviced = false;
      return serviced;
    }
  }

  if (!strcmp(ArgV(argument, 1),"INTCNT")) {
    if (paramcount > 1) {
      uint8_t pin = atoi(ArgV(argument, 2));
      if (pin < mcp230xx_pincount) {
        if (pin == 0) {
          if (!strcmp(ArgV(argument, 2), "0")) validpin=true;
        } else {
          validpin = true;
        }
      }
      if (validpin) {
        if (paramcount > 2) {
          uint8_t intcnt = atoi(ArgV(argument, 3));
          if ((intcnt >= 0) && (intcnt <= 1)) {
            Settings->mcp230xx_config[pin].int_count_en=intcnt;
            if (Settings->mcp230xx_config[pin].int_report_defer) {
              Settings->mcp230xx_config[pin].int_report_defer=0;
              AddLog(LOG_LEVEL_INFO, PSTR("*** WARNING *** - Disabled INTDEF for pin D%i"),pin);
            }
            if (Settings->mcp230xx_config[pin].int_report_mode < 3) {
              Settings->mcp230xx_config[pin].int_report_mode=3;
              AddLog(LOG_LEVEL_INFO, PSTR("*** WARNING *** - Disabled immediate interrupt/telemetry reporting for pin D%i"),pin);
            }
            if ((Settings->mcp230xx_config[pin].int_count_en) && (!Settings->mcp230xx_int_timer)) {
              AddLog(LOG_LEVEL_INFO, PSTR("*** WARNING *** - INTCNT enabled for pin D%i but global INTTIMER is disabled!"),pin);
            }
            MCP230xx_CheckForIntCounter(); // update register on whether or not we should be counting interrupts
            Response_P(MCP230XX_INTCFG_RESPONSE,"CNT",pin,Settings->mcp230xx_config[pin].int_count_en);  // "{\"MCP230xx_INT%s\":{\"D_%i\":%i}}";
            return serviced;
          } else {
            serviced=false;
            return serviced;
          }
        } else {
          Response_P(MCP230XX_INTCFG_RESPONSE,"CNT",pin,Settings->mcp230xx_config[pin].int_count_en);  // "{\"MCP230xx_INT%s\":{\"D_%i\":%i}}";
          return serviced;
        }
      }
      serviced = false;
      return serviced;
    } else {
      serviced = false;
      return serviced;
    }
  }

  if (!strcmp(ArgV(argument, 1),"INTRETAIN")) {
    if (paramcount > 1) {
      uint8_t pin = atoi(ArgV(argument, 2));
      if (pin < mcp230xx_pincount) {
        if (pin == 0) {
          if (!strcmp(ArgV(argument, 2), "0")) validpin=true;
        } else {
          validpin = true;
        }
      }
      if (validpin) {
        if (paramcount > 2) {
          uint8_t int_retain = atoi(ArgV(argument, 3));
          if ((int_retain >= 0) && (int_retain <= 1)) {
            Settings->mcp230xx_config[pin].int_retain_flag=int_retain;
            Response_P(MCP230XX_INTCFG_RESPONSE,"INT_RETAIN",pin,Settings->mcp230xx_config[pin].int_retain_flag);
            MCP230xx_CheckForIntRetainer();
            return serviced;
          } else {
            serviced=false;
            return serviced;
          }
        } else {
          Response_P(MCP230XX_INTCFG_RESPONSE,"INT_RETAIN",pin,Settings->mcp230xx_config[pin].int_retain_flag);
          return serviced;
        }
      }
      serviced = false;
      return serviced;
    } else {
      serviced = false;
      return serviced;
    }
  }

  uint8_t pin = atoi(ArgV(argument, 1));

  if (pin < mcp230xx_pincount) {
    if (0 == pin) {
      if (!strcmp(ArgV(argument, 1), "0")) validpin=true;
    } else {
      validpin=true;
    }
  }
  if (validpin && (paramcount > 1)) {
    if (!strcmp(ArgV(argument, 2), "?")) {
      uint8_t port = 0;
      if (pin > 7) { port = 1; }
      uint8_t portdata = MCP230xx_readGPIO(port);
      char pulluptxtr[7],pinstatustxtr[7];
      uint8_t pinmod = Settings->mcp230xx_config[pin].pinmode;
#ifdef USE_MCP230xx_OUTPUT
      if (pinmod > 4) {
        sprintf(pulluptxtr,ConvertNumTxt(Settings->mcp230xx_config[pin].pullup | (Settings->mcp230xx_config[pin].keep_output << 1), 0, 1));
        sprintf(pinstatustxtr,ConvertNumTxt(portdata>>(pin-(port*8))&1,pinmod));
        Response_P(MCP230XX_OUTPUT_RESPONSE,pin,pinmod,pulluptxtr,pinstatustxtr);
      } else {
#endif //USE_MCP230xx_OUTPUT
        char intmodetxt[9];
        sprintf(intmodetxt,IntModeTxt(Settings->mcp230xx_config[pin].int_report_mode));
        sprintf(pulluptxtr,ConvertNumTxt(Settings->mcp230xx_config[pin].pullup | (Settings->mcp230xx_config[pin].keep_output << 1)));
        sprintf(pinstatustxtr,ConvertNumTxt(portdata>>(pin-(port*8))&1));
        Response_P(MCP230XX_SENSOR_RESPONSE,pin,pinmod,pulluptxtr,intmodetxt,pinstatustxtr);
#ifdef USE_MCP230xx_OUTPUT
      }
#endif //USE_MCP230xx_OUTPUT
      return serviced;
    }
#ifdef USE_MCP230xx_OUTPUT
    if (Settings->mcp230xx_config[pin].pinmode >= 5 && paramcount == 2) { // Changing output value
      uint8_t pincmd = Settings->mcp230xx_config[pin].pinmode - 5;
      uint8_t relay_no = 0;
      for (relay_no = 0; relay_no < mcp230xx_pincount ; relay_no ++) {
        if ( mcp230xx_outpinmapping[relay_no] == pin) break;
      }
      relay_no = TasmotaGlobal.devices_present - mcp230xx_outpincount + relay_no +1;
      if ((!strcmp(ArgV(argument, 2), "ON")) || (!strcmp(ArgV(argument, 2), "1"))) {
        ExecuteCommandPower(relay_no, 1, SRC_IGNORE);
        return serviced;
      }
      if ((!strcmp(ArgV(argument, 2), "OFF")) || (!strcmp(ArgV(argument, 2), "0"))) {
        ExecuteCommandPower(relay_no, 0, SRC_IGNORE);
        return serviced;
      }
      if ((!strcmp(ArgV(argument, 2), "T")) || (!strcmp(ArgV(argument, 2), "2")))  {
        ExecuteCommandPower(relay_no, 2, SRC_IGNORE);
        return serviced;
      }
    }
#endif // USE_MCP230xx_OUTPUT
    uint8_t pinmode = 0;
    uint8_t pullup = 0;
    uint8_t intmode = 0;
    if (paramcount > 1) {
      pinmode = atoi(ArgV(argument, 2));
    }
    if (paramcount > 2) {
      pullup = atoi(ArgV(argument, 3));
    }
    if (paramcount > 3) {
      intmode = atoi(ArgV(argument, 4));
    }
#ifdef USE_MCP230xx_OUTPUT
    if ((pin < mcp230xx_pincount) && (((pinmode > 0) && (pinmode < 5) && (pullup < 2)) || ((pinmode > 4) && (pinmode < 7) && (pullup < 3))) && (paramcount > 2)) {
#else // not use OUTPUT
    if ((pin < mcp230xx_pincount) && (pinmode > 0) && (pinmode < 5) && (pullup < 2) && (paramcount > 2)) {
#endif // USE_MCP230xx_OUTPUT
      Settings->mcp230xx_config[pin].pinmode=pinmode;
      Settings->mcp230xx_config[pin].pullup=pullup & 1;
      Settings->mcp230xx_config[pin].keep_output=pullup >> 1;
      if ((pinmode > 1) && (pinmode < 5)) {
        if ((intmode >= 0) && (intmode <= 3)) {
          Settings->mcp230xx_config[pin].int_report_mode=intmode;
        }
      } else {
        Settings->mcp230xx_config[pin].int_report_mode=3; // Int mode not valid for pinmodes other than 2 through 4
      }
      MCP230xx_ApplySettings();
      uint8_t port = 0;
      if (pin > 7) { port = 1; }
      uint8_t portdata = MCP230xx_readGPIO(port);
      char pulluptxtr[7],pinstatustxtr[7];
      uint8_t pinmod = Settings->mcp230xx_config[pin].pinmode;
#ifdef USE_MCP230xx_OUTPUT
      if (pinmod > 4) {
        sprintf(pulluptxtr,ConvertNumTxt(Settings->mcp230xx_config[pin].pullup | (Settings->mcp230xx_config[pin].keep_output << 1), 0, 1));
        sprintf(pinstatustxtr,ConvertNumTxt(portdata>>(pin-(port*8))&1,pinmod));
        Response_P(MCP230XX_OUTPUT_RESPONSE,pin,pinmod,pulluptxtr,pinstatustxtr);
      } else {
#endif //USE_MCP230xx_OUTPUT
        char intmodetxt[9];
        sprintf(intmodetxt,IntModeTxt(Settings->mcp230xx_config[pin].int_report_mode));
        sprintf(pulluptxtr,ConvertNumTxt(Settings->mcp230xx_config[pin].pullup | (Settings->mcp230xx_config[pin].keep_output << 1)));
        sprintf(pinstatustxtr,ConvertNumTxt(portdata>>(pin-(port*8))&1));
        Response_P(MCP230XX_SENSOR_RESPONSE,pin,pinmod,pulluptxtr,intmodetxt,pinstatustxtr);
#ifdef USE_MCP230xx_OUTPUT
      }
#endif //USE_MCP230xx_OUTPUT




/*      char pulluptxtc[7], pinstatustxtc[7];
      char intmodetxt[9];
      sprintf(pulluptxtc,ConvertNumTxt(pullup));
      sprintf(intmodetxt,IntModeTxt(Settings->mcp230xx_config[pin].int_report_mode));
#ifdef USE_MCP230xx_OUTPUT
      sprintf(pinstatustxtc,ConvertNumTxt(portdata>>(pin-(port*8))&1,Settings->mcp230xx_config[pin].pinmode));
#else  // not USE_MCP230xx_OUTPUT
      sprintf(pinstatustxtc,ConvertNumTxt(portdata>>(pin-(port*8))&1));
#endif // USE_MCP230xx_OUTPUT
      Response_P(MCP230XX_SENSOR_RESPONSE,pin,pinmode,pulluptxtc,intmodetxt,pinstatustxtc); */
      return serviced;
    }
  } else {
    serviced=false; // no valid pin was used
    return serviced;
  }
  return serviced;
}

#ifdef USE_MCP230xx_OUTPUT
#ifdef USE_MCP230xx_DISPLAYOUTPUT

const char HTTP_SNS_MCP230xx_OUTPUT[] PROGMEM = "{s}MCP230XX D%d{m}%s{e}"; // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>

void MCP230xx_UpdateWebData(void)
{
  uint8_t gpio1 = MCP230xx_readGPIO(0);
  uint8_t gpio2 = 0;
  if (2 == mcp230xx_type) {
    gpio2 = MCP230xx_readGPIO(1);
  }
  uint16_t gpio = (gpio2 << 8) + gpio1;
  for (uint32_t pin = 0; pin < mcp230xx_pincount; pin++) {
    if (Settings->mcp230xx_config[pin].pinmode >= 5) {
      char stt[7];
      sprintf(stt,ConvertNumTxt((gpio>>pin)&1,Settings->mcp230xx_config[pin].pinmode));
      WSContentSend_PD(HTTP_SNS_MCP230xx_OUTPUT, pin, stt);
    }
  }
}

#endif // USE_MCP230xx_DISPLAYOUTPUT
#endif // USE_MCP230xx_OUTPUT

/*
#ifdef USE_MCP230xx_OUTPUT

void MCP230xx_OutputTelemetry(void)
{
  uint8_t outputcount = 0;
  uint16_t gpiototal = 0;
  uint8_t gpioa = 0;
  uint8_t gpiob = 0;
  gpioa=MCP230xx_readGPIO(0);
  if (2 == mcp230xx_type) { gpiob=MCP230xx_readGPIO(1); }
  gpiototal=((uint16_t)gpiob << 8) | gpioa;
  for (uint32_t pinx = 0;pinx < mcp230xx_pincount;pinx++) {
    if (Settings->mcp230xx_config[pinx].pinmode >= 5) outputcount++;
  }
  if (outputcount) {
    char stt[7];
    ResponseTime_P(PSTR(",\"MCP230_OUT\":{"));
    for (uint32_t pinx = 0;pinx < mcp230xx_pincount;pinx++) {
      if (Settings->mcp230xx_config[pinx].pinmode >= 5) {
        sprintf(stt,ConvertNumTxt(((gpiototal>>pinx)&1),Settings->mcp230xx_config[pinx].pinmode));
        ResponseAppend_P(PSTR("\"OUT_D%i\":\"%s\","),pinx,stt);
      }
    }
    ResponseAppend_P(PSTR("\"END\":1}}"));
    MqttPublishTeleSensor();
  }
}

#endif // USE_MCP230xx_OUTPUT
*/

void MCP230xx_Interrupt_Counter_Report(void) {
  ResponseTime_P(PSTR(",\"MCP230_INTTIMER\":{"));
  bool first = true;
  for (uint32_t pinx = 0;pinx < mcp230xx_pincount;pinx++) {
    if (Settings->mcp230xx_config[pinx].int_count_en) { // Counting is enabled for this pin so we add to report
      ResponseAppend_P(PSTR("%s\"INTCNT_D%i\":%i,"), (first) ? "" : "?", pinx, mcp230xx_int_counter[pinx]);
      first = false;
      mcp230xx_int_counter[pinx]=0;
    }
  }
  ResponseAppend_P(PSTR("}}"));
  MqttPublishTeleSensor();
  mcp230xx_int_sec_counter = 0;
}

void MCP230xx_Interrupt_Retain_Report(void) {
  uint16_t retainresult = 0;
  ResponseTime_P(PSTR(",\"MCP_INTRETAIN\":{"));
  for (uint32_t pinx = 0;pinx < mcp230xx_pincount;pinx++) {
    if (Settings->mcp230xx_config[pinx].int_retain_flag) {
      ResponseAppend_P(PSTR("\"D%i\":%i,"),pinx,mcp230xx_int_retainer[pinx]);
      retainresult |= (((mcp230xx_int_retainer[pinx])&1) << pinx);
      mcp230xx_int_retainer[pinx]=0;
    }
  }
  ResponseAppend_P(PSTR("\"Value\":%u}}"),retainresult);
  MqttPublishTeleSensor();
}

#ifdef USE_MCP230xx_OUTPUT
void MCP230xx_SwitchRelay() {
  for (uint32_t i = TasmotaGlobal.devices_present - mcp230xx_outpincount; i < TasmotaGlobal.devices_present; i++) {
    uint8_t pin = mcp230xx_outpinmapping[i - (TasmotaGlobal.devices_present - mcp230xx_outpincount)];
    uint8_t pincmd = Settings->mcp230xx_config[pin].pinmode - 5;
    uint8_t relay_state = bitRead(XdrvMailbox.index, i);
    if (mcp230xx_keepout_no_toggle == 0 || !Settings->mcp230xx_config[pin].keep_output) {
      switch (relay_state) {
      case 1:
        MCP230xx_SetOutPin(pin,1-pincmd);
        break;
      case 0:
        MCP230xx_SetOutPin(pin,pincmd);
        break;
      }
    }
  }
}
#endif // USE_MCP230xx_OUTPUT

/*********************************************************************************************\
   Interface
\*********************************************************************************************/

bool Xsns29(uint8_t function)
{
  if (!I2cEnabled(XI2C_22)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
      MCP230xx_Detect();
  }
  else if (mcp230xx_type) {
    switch (function) {
      case FUNC_EVERY_50_MSECOND:
        if (mcp230xx_int_en) { // Only check for interrupts if its enabled on one of the pins
          mcp230xx_int_prio_counter++;
          if ((mcp230xx_int_prio_counter) >= (Settings->mcp230xx_int_prio)) {
            MCP230xx_CheckForInterrupt();
            mcp230xx_int_prio_counter=0;
          }
        }
        break;
      case FUNC_EVERY_SECOND:
        if (mcp230xx_int_counter_en) {
          mcp230xx_int_sec_counter++;
          if (mcp230xx_int_sec_counter >= Settings->mcp230xx_int_timer) { // Interrupt counter interval reached, lets report
            MCP230xx_Interrupt_Counter_Report();
          }
        }
        if (TasmotaGlobal.tele_period == 0) {
          if (mcp230xx_int_retainer_en) { // We have pins configured for interrupt retain reporting
            MCP230xx_Interrupt_Retain_Report();
          }
/*
#ifdef USE_MCP230xx_OUTPUT
          MCP230xx_OutputTelemetry();
#endif // USE_MCP230xx_OUTPUT
*/
        }
        break;
#ifdef USE_MCP230xx_OUTPUT
      case FUNC_SET_POWER:
          MCP230xx_SwitchRelay();
          break;
#endif // USE_MCP230xx_OUTPUT
      case FUNC_JSON_APPEND:
        MCP230xx_Show(1);
        break;
      case FUNC_COMMAND_SENSOR:
        if (XSNS_29 == XdrvMailbox.index) {
          result = MCP230xx_Command();
        }
        break;
#ifdef USE_WEBSERVER
#ifdef USE_MCP230xx_OUTPUT
#ifdef USE_MCP230xx_DISPLAYOUTPUT
      case FUNC_WEB_SENSOR:
        MCP230xx_UpdateWebData();
        break;
#endif // USE_MCP230xx_DISPLAYOUTPUT
#endif // USE_MCP230xx_OUTPUT
#endif // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_MCP230xx
#endif  // USE_I2C
