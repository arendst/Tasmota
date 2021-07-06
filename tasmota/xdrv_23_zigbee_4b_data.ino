/*
  xdrv_23_zigbee_4a_eeprom.ino - zigbee support for Tasmota - saving configuration in I2C Eeprom of ZBBridge

  Copyright (C) 2021  Theo Arends and Stephan Hadinger

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

#ifdef USE_ZIGBEE


// =======================
// ZbData v1
// File structure:
//
// [Array of devices]
// uint8  - length of device record (excluding the length byte)
// uint16 - short address
//
// [Device specific data first]
// uint8 - length of structure (excluding the length byte)
// uint8[] - device wide data
//
// [Array of data structures]
// uint8  - length of structure
// uint8[] - list of data
//

// returns the lenght of consumed buffer, or -1 if error
int32_t hydrateDeviceWideData(class Z_Device & device, const SBuffer & buf, size_t start, size_t len) {
  size_t segment_len = buf.get8(start);
  if ((segment_len < 6) || (segment_len > len)) {
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "invalid device wide data length=%d"), segment_len);
    return -1;
  }
  device.last_seen = buf.get32(start+1);
  device.lqi = buf.get8(start + 5);
  device.batterypercent = buf.get8(start + 6);
  return segment_len + 1;
}

// return true if success
bool hydrateDeviceData(class Z_Device & device, const SBuffer & buf, size_t start, size_t len) {
  // First hydrate device wide data
  int32_t ret = hydrateDeviceWideData(device, buf, start, len);
  if (ret < 0) { return false; }

  size_t offset = ret;
  while (offset + 5 <= len) {    // each entry is at least 5 bytes
    uint8_t data_len = buf.get8(start + offset);
// #ifdef Z_EEPROM_DEBUG
//   {
//     char hex_char[((data_len+1) * 2) + 2];
//     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "hydrateDeviceData data_len=%d contains %s"), data_len, ToHex_P(buf.buf(start+offset+1), data_len, hex_char, sizeof(hex_char)));
//   }
// #endif
    Z_Data & data_elt = device.data.createFromBuffer(buf, start + offset + 1, data_len);
    (void)data_elt;   // avoid compiler warning
    offset += data_len + 1;
  }
  return true;
}

// negative means error
// positive is the segment length
int32_t hydrateSingleDeviceData(const SBuffer & buf) {
  uint8_t segment_len = buf.len();
  if (segment_len < 4) {
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "invalid segment_len=%d"), segment_len);
    return -1;
  }
  // read shortaddr
  uint16_t shortaddr = buf.get16(0);
  if (shortaddr >= 0xFFF0) {
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "invalid shortaddr=0x%04X"), shortaddr);
    return -1;
  }
#ifdef Z_EEPROM_DEBUG
  {
    if (segment_len > 3) {
      AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "ZbData 0x%04X,%*_H"), shortaddr, buf.buf(2), buf.len() - 2);
    }
  }
#endif
  // check if the device exists, if not skip the record
  Z_Device & device = zigbee_devices.findShortAddr(shortaddr);
  if (&device != nullptr) {

    // parse the rest
    bool ret = hydrateDeviceData(device, buf, 2, segment_len - 2);

    if (!ret) { return -1; }
  }
  return segment_len;
}

SBuffer hibernateDeviceData(const struct Z_Device & device) {
  SBuffer buf(192);

  // If we have zero information about the device, just skip ir
  if (device.validLqi() ||
      device.validBatteryPercent() ||
      device.validLastSeen() ||
      !device.data.isEmpty()) {

    buf.add8(0x00);     // overall length, will be updated later
    buf.add16(device.shortaddr);

    // device wide data
    buf.add8(6);        // 6 bytes
    buf.add32(device.last_seen);
    buf.add8(device.lqi);
    buf.add8(device.batterypercent);

    for (const auto & data_elt : device.data) {
      size_t item_len = data_elt.DataTypeToLength(data_elt.getType());
      buf.add8(item_len);      // place-holder for length
      buf.addBuffer((uint8_t*) &data_elt, item_len);
    }

    // update overall length
    buf.set8(0, buf.len() - 1);

    {
      // skip first 3 bytes
      size_t buf_len = buf.len() - 3;
      Response_P(PSTR("{\"" D_PRFX_ZB D_CMND_ZIGBEE_DATA "\":\"ZbData 0x%04X,%*_H\"}"), device.shortaddr, buf_len, buf.buf(3));
      MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, PSTR(D_PRFX_ZB D_CMND_ZIGBEE_DATA));
    }
  }

  return buf;
}

/*********************************************************************************************\
 *
 * Hydrate data from the EEPROM
 *
\*********************************************************************************************/
// Parse the entire blob
// return true if ok
bool hydrateDevicesData(void) {
  Univ_Read_File f;   // universal reader
  const char * storage_class = PSTR("");
#ifdef USE_ZIGBEE_EEPROM
  if (zigbee.eeprom_ready) {
    f.init(ZIGB_DATA2);
    storage_class = PSTR("EEPROM");
  }
#endif // USE_ZIGBEE_EEPROM

#ifdef USE_UFILESYS
  File file;
  if (!f.valid() && dfsp) {
    file = dfsp->open(TASM_FILE_ZIGBEE_DATA, "r");
    if (file) {
      f.init(&file);
      storage_class = PSTR("File System");
    }
  }
#endif // USE_UFILESYS

  if (!f.valid() || f.len <= 0) {
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "No Zigbee device data"));
    return false;
  }

  uint32_t file_len = f.len;

  if (file_len > 0) {
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "Zigbee device data in %s (%d bytes)"), storage_class, file_len);
  } else {
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "No Zigbee device data in %s"), storage_class);
    f.close();
    return false;
  }

  while (1) {
    uint8_t dev_record_len = 0;
    int32_t ret = f.readBytes(&dev_record_len, sizeof(dev_record_len));
    if (ret <= 0) {
      break;    // finished
    }

    SBuffer buf(dev_record_len);
    buf.setLen(dev_record_len);
    
    ret = f.readBytes(buf.getBuffer(), dev_record_len);
    if (ret != dev_record_len) {
      AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "Invalid device data information, aborting"));
      f.close();
      return false;
    }
    int32_t segment_len = hydrateSingleDeviceData(buf);
    if (segment_len <= 0) {
      AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "Invalid device data information, aborting"));
      f.close();
      return false;
    }
  }
  f.close();
  return true;
}

/*********************************************************************************************\
 *
 * Hibernate data to the EEPROM
 *
\*********************************************************************************************/
void hibernateAllData(void) {
  if (Rtc.utc_time < START_VALID_TIME) { return; }
  if (zigbee_devices.devicesSize() == 0) { return; }    // safe-guard, if data is empty, don't save anything
  Univ_Write_File f;
  const char * storage_class = PSTR("");

#ifdef USE_ZIGBEE_EEPROM
  if (!f.valid() && zigbee.eeprom_ready) {
    f.init(ZIGB_DATA2);
    storage_class = PSTR("EEPROM");
  }
#endif

#ifdef USE_UFILESYS
  File file;
  if (!f.valid() && dfsp) {
    file = dfsp->open(TASM_FILE_ZIGBEE_DATA, "w");
    if (file) {
      f.init(&file);
      storage_class = PSTR("File System");
    }
  }
#endif

  if (f.valid()) {
    for (const auto & device : zigbee_devices.getDevices()) {
      // allocte a buffer for a single device
      SBuffer buf = hibernateDeviceData(device);
      if (buf.len() > 0) {
        f.writeBytes(buf.getBuffer(), buf.len());
      }
    }
    size_t buf_len = f.getCursor();
    f.close();
    
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "ZbData - %d bytes written to %s"), buf_len, storage_class);
  }
}

/*********************************************************************************************\
 * Timer to save every 60 minutes
\*********************************************************************************************/
const uint32_t Z_SAVE_DATA_TIMER = 60 * 60 * 1000;       // save data every 60 minutes (in ms)

//
// Callback for setting the timer to save Zigbee Data in x seconds
//
int32_t Z_Set_Save_Data_Timer(uint8_t value) {
  zigbee_devices.setTimer(0x0000, 0, Z_SAVE_DATA_TIMER, 0, 0, Z_CAT_ALWAYS, 0 /* value */, &Z_SaveDataTimer);
  return 0;                              // continue
}

void Z_SaveDataTimer(uint16_t shortaddr, uint16_t groupaddr, uint16_t cluster, uint8_t endpoint, uint32_t value) {
  hibernateAllData();
  Z_Set_Save_Data_Timer(0);     // set a new timer
}

#ifdef USE_ZIGBEE_EEPROM
void ZFS_Erase(void) {
  if (zigbee.eeprom_present) {
    ZFS::erase();
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "Zigbee Devices Data erased in %s"), PSTR("EEPROM"));
  }
}

#endif // USE_ZIGBEE_EEPROM

#endif // USE_ZIGBEE
