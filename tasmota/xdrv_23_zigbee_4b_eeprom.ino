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
//     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "hydrateDeviceData data_len=%d contains %s"), data_len, ToHex_P(buf.buf(start+offset+1), data_len, hex_char, sizeof(hex_char)));
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
int32_t hydrateSingleDevice(const SBuffer & buf, size_t start, size_t len) {
  uint8_t segment_len = buf.get8(start);
  if ((segment_len < 4) || (start + segment_len > len)) {
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "invalid segment_len=%d"), segment_len);
    return -1;
  }
  // read shortaddr
  uint16_t shortaddr = buf.get16(start + 1);
  if (shortaddr >= 0xFFF0) {
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "invalid shortaddr=0x%04X"), shortaddr);
    return -1;
  }
#ifdef Z_EEPROM_DEBUG
  {
    if (segment_len > 3) {
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "ZbData 0x%04X,%*_H"), shortaddr, segment_len+1-3, buf.buf(start+3));
    }
  }
#endif
  // check if the device exists, if not skip the record
  Z_Device & device = zigbee_devices.findShortAddr(shortaddr);
  if (&device != nullptr) {

    // parse the rest
    bool ret = hydrateDeviceData(device, buf, start + 3, segment_len - 3);

    if (!ret) { return -1; }
  }
  return segment_len + 1;
}

/*********************************************************************************************\
 *
 * Hydrate data from the EEPROM
 *
\*********************************************************************************************/
// Parse the entire blob
// return true if ok
bool hydrateDevicesDataFromEEPROM(void) {
#ifdef USE_ZIGBEE_EZSP
  if (!zigbee.eeprom_ready) { return false; }
  int32_t file_length = ZFS::getLength(ZIGB_DATA2);
  if (file_length > 0) {
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "Zigbee device data in EEPROM (%d bytes)"), file_length);
  } else {
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "No Zigbee device data in EEPROM"));
    return false;
  }

  const uint16_t READ_BUFFER = 192;
  uint16_t cursor = 0x0000;         // cursor in the file
  bool read_more = true;

  SBuffer buf(READ_BUFFER);
  while (read_more) {
    buf.setLen(buf.size());         // set to max size and fill with zeros
    int32_t bytes_read = ZFS::readBytes(ZIGB_DATA2, buf.getBuffer(), buf.size(), cursor, READ_BUFFER);
// #ifdef Z_EEPROM_DEBUG
//     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "readBytes buffer_len=%d, read_start=%d, read_len=%d, actual_read=%d"), buf.size(), cursor, length, bytes_read);
// #endif
    if (bytes_read > 0) {
      buf.setLen(bytes_read);       // adjust to actual size
      int32_t segment_len = hydrateSingleDevice(buf, 0, buf.len());
// #ifdef Z_EEPROM_DEBUG
//       AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "hydrateSingleDevice segment_len=%d"), segment_len);
// #endif
      if (segment_len <= 0) { return false; }

      cursor += segment_len;
    } else {
      read_more = false;
    }
  }
  return true;
#else // USE_ZIGBEE_EZSP
  return false;
#endif // USE_ZIGBEE_EZSP
}

SBuffer hibernateDeviceData(const struct Z_Device & device, bool mqtt = false) {
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

      if (mqtt) {
        Response_P(PSTR("{\"" D_PRFX_ZB D_CMND_ZIGBEE_DATA "\":\"ZbData 0x%04X,%*_H\"}"), device.shortaddr, buf_len, buf.buf(3));
        MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, PSTR(D_PRFX_ZB D_CMND_ZIGBEE_DATA));
      } else {
        AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "ZbData 0x%04X,%*_H"), device.shortaddr, buf_len, buf.buf(3));
      }
    }
  }

  return buf;
}

/*********************************************************************************************\
 *
 * Hibernate data to the EEPROM
 *
\*********************************************************************************************/
void hibernateAllData(void) {
#ifdef USE_ZIGBEE_EZSP
  if (Rtc.utc_time < START_VALID_TIME) { return; }
  if (!zigbee.eeprom_ready) { return; }

  ZFS_Write_File write_data(ZIGB_DATA2);
  // first prefix is number of devices
  uint8_t device_num = zigbee_devices.devicesSize();

  for (const auto & device : zigbee_devices.getDevices()) {
    // allocte a buffer for a single device
    SBuffer buf = hibernateDeviceData(device, false);    // simple log, no mqtt
    if (buf.len() > 0) {
      write_data.addBytes(buf.getBuffer(), buf.len());
    }
  }
  int32_t ret = write_data.close();
#ifdef Z_EEPROM_DEBUG
  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "ZbData - %d bytes written to EEPROM"), ret);
#endif
#endif // USE_ZIGBEE_EZSP
}

/*********************************************************************************************\
 * Timer to save every 60 minutes
\*********************************************************************************************/
const uint32_t Z_SAVE_DATA_TIMER = 60 * 60 * 1000;       // save data every 60 minutes (in ms)

//
// Callback for setting the timer to save Zigbee Data in x seconds
//
int32_t Z_Set_Save_Data_Timer_EEPROM(uint8_t value) {
  zigbee_devices.setTimer(0x0000, 0, Z_SAVE_DATA_TIMER, 0, 0, Z_CAT_ALWAYS, 0 /* value */, &Z_SaveDataTimer);
  return 0;                              // continue
}

void Z_SaveDataTimer(uint16_t shortaddr, uint16_t groupaddr, uint16_t cluster, uint8_t endpoint, uint32_t value) {
  hibernateAllData();
  Z_Set_Save_Data_Timer_EEPROM(0);     // set a new timer
}

#ifdef USE_ZIGBEE_EZSP
/*********************************************************************************************\
 * Write Devices in EEPROM
\*********************************************************************************************/
// EEPROM variant that writes one item at a time and is not limited to 2KB
bool hibernateDevicesInEEPROM(void) {
  if (Rtc.utc_time < START_VALID_TIME) { return false; }
  if (!zigbee.eeprom_ready) { return false; }

  ZFS_Write_File write_data(ZIGB_NAME2);

  // first prefix is number of devices
  uint8_t devices_size = zigbee_devices.devicesSize();
  if (devices_size > 64) { devices_size = 64; }         // arbitrarily limit to 64 devices in EEPROM instead of 32 in Flash
  write_data.addBytes(&devices_size, sizeof(devices_size));

  for (const auto & device : zigbee_devices.getDevices()) {
    const SBuffer buf = hibernateDevicev2(device);
    if (buf.len() > 0) {
      write_data.addBytes(buf.getBuffer(), buf.len());
    }
  }
  int32_t ret = write_data.close();

  if (ret < 0) {
    AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_ZIGBEE "Error writing Devices to EEPROM"));
    return false;
  } else {
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "Zigbee Devices Data saved in %s (%d bytes)"), PSTR("EEPROM"), ret);
  }
  return true;
}


// dump = true, only dump to logs, don't actually load
bool loadZigbeeDevicesFromEEPROM(void) {
  if (!zigbee.eeprom_ready) { return false; }
  uint16_t file_len = ZFS::getLength(ZIGB_NAME2);

  uint8_t num_devices = 0;
  ZFS::readBytes(ZIGB_NAME2, &num_devices, sizeof(num_devices), 0, sizeof(num_devices));

  if ((file_len < 10) || (num_devices == 0x00) || (num_devices == 0xFF)) {             // No data
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "No Zigbee device information in %s"), PSTR("EEPROM"));
    return false;
  }
  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "Zigbee device information in %s (%d bytes)"), PSTR("EEPROM"), file_len);

  uint32_t k = 1;   // byte index in global buffer
  for (uint32_t i = 0; (i < num_devices) && (k < file_len); i++) {
    uint8_t dev_record_len = 0;
    int32_t ret = ZFS::readBytes(ZIGB_NAME2, &dev_record_len, 1, k, 1);
    SBuffer buf(dev_record_len);
    buf.setLen(dev_record_len);
    ret = ZFS::readBytes(ZIGB_NAME2, buf.getBuffer(), dev_record_len, k, dev_record_len);
    if (ret != dev_record_len) {
      AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "File too short when reading EEPROM"));
      return false;
    }

    hydrateSingleDevice(buf, 2);

    // next iteration
    k += dev_record_len;
  }

  zigbee_devices.clean();   // don't write back to Flash what we just loaded
  return true;
}

void ZFS_Erase(void) {
  if (zigbee.eeprom_present) {
    ZFS::erase();
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "Zigbee Devices Data erased in %s"), PSTR("EEPROM"));
  }
}

#endif // USE_ZIGBEE_EZSP

#endif // USE_ZIGBEE
