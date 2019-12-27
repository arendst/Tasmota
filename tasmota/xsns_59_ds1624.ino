/*
  xsns_59_ds1624.ino - Support for I2C DS1624 Temperature Sensor

  Copyright (C) 2019 Leonid Myravjev <asm@asm.pp.ru>

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
#ifdef USE_DS1624

/*********************************************************************************************\
 * DS1624,DS1621 - Temperature
 * 
 * I2C Address: 0x48 - 0x4F
\*********************************************************************************************/

#define XSNS_59                 59
#define XI2C_42                 42  // See I2CDEVICES.md

#define DS1624_MEM_REGISTER    0x17  //only for ds1624, don't exists on 1621
#define DS1624_CONF_REGISTER   0xAC
#define DS1624_TEMP_REGISTER   0xAA
#define DS1624_START_REGISTER  0xEE
#define DS1624_STOP_REGISTER   0x22

#define DS1621_COUNTER_REGISTER   0xA8 //exists on 1621 and 1624(undocumented)
#define DS1621_SLOPE_REGISTER     0xA9 //exists on 1624 and 1624(undocumented)

enum {
  DS1624_TYPE_DS1624,
  DS1624_TYPE_DS1621
};

#define DS1624_MAX_SENSORS     8
bool ds1624_init=false;
struct {
  bool  valid;
  float value;
  int   type;
  char  name[9];
} ds1624_sns[DS1624_MAX_SENSORS];

uint32_t DS1624_Idx2Addr(int idx) {
  return 0x48 + idx;
}

void DS1624_HotPlugUp(int idx) {
  uint32_t addr;
  uint8_t config,tmp;

  addr = DS1624_Idx2Addr(idx);
  if (I2cActive(addr)) return;
  if (!I2cSetDevice(addr)) return;
  if (I2cValidRead8(&config, addr, DS1624_CONF_REGISTER)) {
    if (I2cValidRead8(&tmp, addr, DS1624_MEM_REGISTER))
         ds1624_sns[idx].type=DS1624_TYPE_DS1624;
    else ds1624_sns[idx].type=DS1624_TYPE_DS1621;
    if (ds1624_sns[idx].type==DS1624_TYPE_DS1621)
          snprintf_P(ds1624_sns[idx].name, sizeof(ds1624_sns[idx].name), PSTR("DS1621_%d"), idx);
    else  snprintf_P(ds1624_sns[idx].name, sizeof(ds1624_sns[idx].name), PSTR("DS1624_%d"), idx);
    I2cSetActiveFound(addr, ds1624_sns[idx].name);
    ds1624_sns[idx].valid=true;
    if ((config & 1) == 1) {
      config &= 0xfe;
      I2cWrite8(addr, DS1624_CONF_REGISTER, config); //1show off
      delay(10);// by spec after writing
    }
    I2cValidRead(addr, DS1624_START_REGISTER, 1); //FIXME 0 must read, but 0 isn't work for tasmota
    AddLog_P2(LOG_LEVEL_INFO, "Hot Plug %s addr %x config: %x", ds1624_sns[idx].name, addr, config);
  }
}

void DS1624_HotPlugDown(int idx) {
  uint32_t addr=DS1624_Idx2Addr(idx);
  if (I2cActive(addr)==false) return; 
  I2cResetActive(addr);
  ds1624_sns[idx].valid=false;
  AddLog_P2(LOG_LEVEL_INFO, "Hot UnPlug %s", ds1624_sns[idx].name);
}

bool DS1624GetTemp(float *value, int idx)
{
  uint16_t t;
  bool     result;
  uint32_t addr=DS1624_Idx2Addr(idx);

  if (!I2cValidRead16(&t, DS1624_Idx2Addr(idx), DS1624_TEMP_REGISTER)) return false;
  *value = ConvertTemp(((float)(int8_t)(t>>8)) + ((t>>4)&0xf)*0.0625);
  if (ds1624_sns[idx].type==DS1624_TYPE_DS1621) { // Higher resolution
    uint8_t perc,remain;
    if (I2cValidRead8(&remain, addr, DS1621_COUNTER_REGISTER)==false) return true;
    if (I2cValidRead8(&perc, addr, DS1621_SLOPE_REGISTER)==false) return true;
    *value += ((float)perc - (float)remain)/((float)perc) - 0.25;
  }
  return true;
}

void DS1624HotPlugScan(void)
{
  uint16_t t;

  for (int idx = 0; idx < DS1624_MAX_SENSORS; idx++) {
    uint32_t addr=DS1624_Idx2Addr(idx);
    if ((I2cActive(addr)==true) && (ds1624_sns[idx].valid==false))
      continue; // is busy by another driver
    if (!I2cValidRead16(&t, DS1624_Idx2Addr(idx), DS1624_TEMP_REGISTER)) {
      DS1624_HotPlugDown(idx);
      continue; }
    DS1624_HotPlugUp(idx);
  }
}

void DS1624EverySecond(void)
{
  float t;
  for (int i = 0; i < DS1624_MAX_SENSORS; i++) {
    if (ds1624_sns[i].valid==false) continue;
    if (!DS1624GetTemp(&t, i)) continue;
    ds1624_sns[i].value=t;
  }
}

void DS1624Show(bool json)
{
  int  i;
  char temperature[33];

  for (i = 0; i < DS1624_MAX_SENSORS; i++) {
    if (ds1624_sns[i].valid==false) continue;

    dtostrfd(ds1624_sns[i].value, Settings.flag2.temperature_resolution, temperature);
    if (json) {
      ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_TEMPERATURE "\":%s}"), ds1624_sns[i].name, temperature);
      if (0 == tele_period) { //FIXME 0==i WTF?
  #ifdef USE_DOMOTICZ
        DomoticzSensor(DZ_TEMP, temperature);
  #endif  // USE_DOMOTICZ
  #ifdef USE_KNX
        KnxSensor(KNX_TEMPERATURE, temperature);
  #endif // USE_KNX
      }
  #ifdef USE_WEBSERVER
    } else {
      WSContentSend_PD(HTTP_SNS_TEMP, ds1624_sns[i].name, temperature, TempUnit());
  #endif  // USE_WEBSERVER
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define FUNC_HOTPLUG_SCAN 999 // FIXME remove it after HOTPLUG supported

bool Xsns59(uint8_t function)
{
  if (!I2cEnabled(XI2C_20)) { return false; }

  bool result = false;
  if (FUNC_INIT == function) {
    if (ds1624_init==false) {
      memset(ds1624_sns, 0, sizeof(ds1624_sns));
      ds1624_init=true;
      DS1624HotPlugScan(); //If HotPlug is off
    }
  }
  switch (function) {
    case FUNC_HOTPLUG_SCAN:
      DS1624HotPlugScan();
      break;
    case FUNC_EVERY_SECOND:
      DS1624EverySecond();
      break;
    case FUNC_JSON_APPEND:
      DS1624Show(1);
      break;
  #ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      DS1624Show(0);
      break;
  #endif  // USE_WEBSERVER
  }
  return result;
}

#endif  // USE_DS1624
#endif  // USE_I2C
