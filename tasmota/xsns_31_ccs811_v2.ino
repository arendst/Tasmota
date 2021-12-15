/*
  xsns_31_ccs811.ino - CCS811 gas and air quality sensor support for Tasmota

  Copyright (C) 2021  Gerhard Mutz and Theo Arends

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
#ifdef USE_CCS811_V2
/*********************************************************************************************\
 * CCS811 - Gas (TVOC - Total Volatile Organic Compounds) and Air Quality (CO2)
 *
 * Source: Adafruit
 *
 * This driver supports one to two devices at a time at
 * addressses 0x5A or/and 0x5B
 *  - for I2C address 0x5A, connect ADDR to GND
 *  - for I2C address 0x5B, connect ADDR to VCC
 * NOTE:
 * - Wake must be connected to GND (no sleep mode supported!)
 * - depending on the breakout board, SDA & SCL may require
 *   pull-ups to VCC, e.g. 4k7R
 *
\*********************************************************************************************/

#define XSNS_31             31
#define XI2C_24             24  // See I2CDEVICES.md

#define EVERYNSECONDS 5
#define RESETCOUNT    6

#include "Adafruit_CCS811.h"

uint8_t CCS811_addresses[] = { CCS811_ADDRESS, (CCS811_ADDRESS + 1) };
#define MAXDEVICECOUNT (sizeof( CCS811_addresses) / sizeof(uint8_t))

typedef struct {
  uint8_t address;
  uint8_t device_found;
  uint8_t device_index;
  uint8_t device_ready;
  Adafruit_CCS811 ccsinstance;
  uint16_t eCO2;
  uint16_t TVOC;
  uint8_t refresh_count;
  uint8_t reset_count;
} CCS811DATA;

uint8_t CCS811_devices_found = 0;
CCS811DATA ccsd[ MAXDEVICECOUNT];
CCS811DATA * pccsd;
uint32_t i;

#define D_PRFX_CCS811 "CCS811"
#define D_CMND_HWVERSION "HW"
#define D_CMND_FWAPPVERSION "FWApp"
#define D_CMND_BASELINE "Baseline"

const char kCCS811Commands[] PROGMEM = D_PRFX_CCS811 "|"  // Prefix
  D_CMND_HWVERSION "|"
  D_CMND_FWAPPVERSION "|"
  D_CMND_BASELINE;

void (* const CCS811Command[])(void) PROGMEM = {
  &CmndCCS811HwVersion,
  &CmndCCS811FwAppVersion,
  &CmndCCS811Baseline
};

/********************************************************************************************/

void CCS811Detect(void)
{
  if (!CCS811_devices_found) {
    memset( ccsd, 0, sizeof( ccsd));
  }
  int active_index = 1;
  for (i = 0, pccsd = ccsd; i < MAXDEVICECOUNT; i++, pccsd++) {
    pccsd->address = CCS811_addresses[ i];
    if (!I2cSetDevice( pccsd->address)) { continue; }
    if (!pccsd->ccsinstance.begin(pccsd->address)) {
      pccsd->device_found = 1;
      CCS811_devices_found += 1;
      I2cSetActiveFound( pccsd->address, "CCS811");
      pccsd->device_index = active_index;
      active_index++;
    }
  }
}

void CCS811Update(void)  // Perform every n second
{
  for (i = 0, pccsd = ccsd; i < MAXDEVICECOUNT; i++, pccsd++) {

    if (!pccsd->device_found)
      continue;

    pccsd->refresh_count++;
    if (pccsd->refresh_count >= EVERYNSECONDS) {
      pccsd->refresh_count = 0;
      pccsd->device_ready = 0;
      if (pccsd->ccsinstance.available()) {
        if (!pccsd->ccsinstance.readData()){
          pccsd->TVOC = pccsd->ccsinstance.getTVOC();
          pccsd->eCO2 = pccsd->ccsinstance.geteCO2();
          pccsd->device_ready = 1;
          if ((TasmotaGlobal.global_update) &&
              (TasmotaGlobal.humidity > 0) &&
              (!isnan(TasmotaGlobal.temperature_celsius))) {
            pccsd->ccsinstance.setEnvironmentalData((uint8_t)TasmotaGlobal.humidity,
                                                    TasmotaGlobal.temperature_celsius);
          }
          pccsd->reset_count = 0;
        }
      } else {
        // failed, count up
        pccsd->reset_count++;
        if (pccsd->reset_count > RESETCOUNT) {
          // after 30 seconds, restart
          pccsd->ccsinstance.begin( pccsd->address);
          pccsd->reset_count = 0;
        }
      }
    }
  }
}


// no methods available in Adafruit library to read version data or
// read/set the baseline value, so we need to emulate the private methods

void CCS811ReadMailboxValue( uint8_t address, uint8_t mailbox, byte * pbuf, uint8_t buflen)
{
  Wire.beginTransmission(address);
  Wire.write(mailbox);
  Wire.endTransmission();
  Wire.requestFrom(address, buflen);
  for (uint8_t i = 0; i < buflen; i++) {
    *(pbuf + i) = Wire.read();
#ifdef CCS811_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR( D_LOG_DEBUG D_PRFX_CCS811 " reading byte %u: 0%02x / %u"), i, *(pbuf + i), *(pbuf + i));
#endif
  }
}

void CCS811WriteMailboxValue(uint8_t address, uint8_t mailbox, byte * pbuf, uint8_t buflen)
{
#ifdef CCS811_DEBUG
  for (uint8_t i = 0; i < buflen; i++) {
    AddLog(LOG_LEVEL_DEBUG, PSTR( D_LOG_DEBUG D_PRFX_CCS811 " writing byte %u: 0%02x / %u"), i, *(pbuf + i), *(pbuf + i));
  }
#endif
	Wire.beginTransmission(address);
	Wire.write((uint8_t)mailbox);
	Wire.write(pbuf, buflen);
	Wire.endTransmission();
}

/*********************************************************************************************\
 * Command Sensor31
\*********************************************************************************************/

CCS811DATA * CmndCCS811SelectDeviceFromIndex(void) {
  CCS811DATA * pccsd_command = NULL;
  if (XdrvMailbox.index <= CCS811_devices_found) {
    // select device data matching the index
    for (i = 0, pccsd = ccsd; i < MAXDEVICECOUNT; i++, pccsd++) {
      if  (pccsd->device_index == XdrvMailbox.index) {
        pccsd_command = pccsd;
#ifdef CCS811_DEBUG
        AddLog(LOG_LEVEL_DEBUG, PSTR( D_LOG_DEBUG D_PRFX_CCS811 " I2C Address: 0%02x"), pccsd_command->address);
#endif
        break;
      }
    }
  }
  return pccsd_command;
}


void CmndCCS811HwVersion(void) {
  CCS811DATA * pccsd = CmndCCS811SelectDeviceFromIndex();
  if (pccsd) {
    byte CCS811_hw_version;
    CCS811ReadMailboxValue( pccsd->address,
                            CCS811_HW_VERSION,
                            &CCS811_hw_version,
                            sizeof(CCS811_hw_version));
    ResponseCmndIdxNumber(CCS811_hw_version);
  }
}

void CmndCCS811FwAppVersion(void) {
  pccsd = CmndCCS811SelectDeviceFromIndex();
  if (pccsd) {
    byte bCCS811_fw_app_version[2];
    char CCS811_fw_app_version[16];
    CCS811ReadMailboxValue( pccsd->address,
                            CCS811_FW_APP_VERSION,
                            bCCS811_fw_app_version,
                            (sizeof(bCCS811_fw_app_version) / sizeof(byte)));
    sprintf( CCS811_fw_app_version,
             PSTR( "%x.%x.%x"),
             (bCCS811_fw_app_version[0] >> 4),  // major
             (bCCS811_fw_app_version[0] & 0xF), // minor
             bCCS811_fw_app_version[1]);        // build

    ResponseCmndIdxChar(CCS811_fw_app_version);
  }
}

void CmndCCS811Baseline(void) {
  pccsd = CmndCCS811SelectDeviceFromIndex();
  if (pccsd) {
    byte CCS811_baseline[2];
    if (XdrvMailbox.data_len > 0) {
      CCS811_baseline[0] = (XdrvMailbox.payload & 0xFF00) >> 8;
      CCS811_baseline[1] =  XdrvMailbox.payload & 0xFF;
      CCS811WriteMailboxValue( pccsd->address,
                               CCS811_BASELINE,
                               CCS811_baseline,
                               (sizeof(CCS811_baseline) / sizeof(byte)));
    } else {
      CCS811ReadMailboxValue( pccsd->address,
                              CCS811_BASELINE,
                              CCS811_baseline,
                              (sizeof(CCS811_baseline) / sizeof(byte)));
    }
    ResponseCmndIdxNumber(((CCS811_baseline[0] << 8) + CCS811_baseline[1]));
  }
}

// -----------------------------------------------------------------------------

const char HTTP_SNS_CCS811[] PROGMEM =
  "{s}%s " D_ECO2 "{m}%d " D_UNIT_PARTS_PER_MILLION "{e}"                // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
  "{s}%s " D_TVOC "{m}%d " D_UNIT_PARTS_PER_BILLION "{e}";

const char * devicenamelist[] PROGMEM = { "CCS811", "CCS811_1", "CCS811_2" };

void CCS811Show(bool json)
{
  uint8_t ready_count = 0;
  for (i = 0, pccsd = ccsd; i < MAXDEVICECOUNT; i++, pccsd++) {
    if ((pccsd->device_found) && (pccsd->device_ready)) {
      ready_count += 1;
    }
  }
  if (!ready_count) {
    return;
  }

  // in upcoming loops use either one device name
  // with no index or or two names with index
  const char ** pdevicename;
  const char ** pdevicename_first = devicenamelist;
  if (ready_count > 1) {
    pdevicename_first++;
  }

  if (json) {
    for (i = 0, pccsd = ccsd, pdevicename = pdevicename_first; i < MAXDEVICECOUNT; i++, pccsd++) {
      if (pccsd->device_ready) {
        ResponseAppend_P( PSTR(",\"%s\":{\"" D_JSON_ECO2 "\":%d,\"" D_JSON_TVOC "\":%d}"),
                          *pdevicename,
                          pccsd->eCO2,
                          pccsd->TVOC);
        pdevicename++;
      }
    }
#ifdef USE_DOMOTICZ
    if (0 == TasmotaGlobal.tele_period) {
      if (pccsd->device_ready) {
        pccsd = ccsd;
        DomoticzSensor(DZ_AIRQUALITY, pccsd->eCO2);
      }
    }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
  } else {
    for (i = 0, pccsd = ccsd, pdevicename = pdevicename_first; i < MAXDEVICECOUNT; i++, pccsd++) {
      if (pccsd->device_ready) {
        WSContentSend_PD( HTTP_SNS_CCS811,
                          *pdevicename, pccsd->eCO2,
                          *pdevicename, pccsd->TVOC);
        pdevicename++;
      }
    }
#endif
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns31(uint8_t function)
{
  if (!I2cEnabled(XI2C_24)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    CCS811Detect();
  }
  else if (CCS811_devices_found) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        CCS811Update();
        break;
      case FUNC_COMMAND:
        result = DecodeCommand( kCCS811Commands, CCS811Command);
        break;
      case FUNC_JSON_APPEND:
        CCS811Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        CCS811Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_CCS811_V2
#endif  // USE_I2C