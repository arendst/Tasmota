/*
  xdrv_23_zigbee_4a_eeprom.ino - zigbee support for Tasmota - saving configuration in I2C Eeprom of ZBBridge

  Copyright (C) 2020  Theo Arends and Stephan Hadinger

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
// uint8 - number of devices, 0=none, 0xFF=invalid entry (probably Flash was erased)
//
// [Array of devices]
// [Offset = 2]
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

void dumpZigbeeDevicesData(void) {
#ifdef USE_ZIGBEE_EZSP
  if (zigbee.eeprom_present) {
    SBuffer buf(192);

    zigbee.eeprom.readBytes(64, 192, buf.getBuffer());
    AddLogBuffer(LOG_LEVEL_INFO, buf.getBuffer(), 192);
  }
#endif // USE_ZIGBEE_EZSP
}

// returns the lenght of consumed buffer, or -1 if error
int32_t hydrateDeviceWideData(class Z_Device & device, const SBuffer & buf, size_t start, size_t len) {
  size_t segment_len = buf.get8(start);
  if ((segment_len < 6) || (segment_len > len)) {
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "invalid device wide data length=%d"), segment_len);
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

  size_t offset = 0 + ret;
  while (offset + 5 <= len) {    // each entry is at least 5 bytes
    uint8_t data_len = buf.get8(start + offset);
    Z_Data & data_elt = device.data.createFromBuffer(buf, offset + 1, data_len);
    (void)data_elt;   // avoid compiler warning
    offset += data_len + 1;
  }
  return true;
}

// negative means error
// positive is the segment length
int32_t hydrateSingleDevice(const class SBuffer & buf, size_t start, size_t len) {
  uint8_t segment_len = buf.get8(start);
  if ((segment_len < 4) || (start + segment_len > len)) {
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "invalid segment_len=%d"), segment_len);
    return -1;
  }
  // read shortaddr
  uint16_t shortaddr = buf.get16(start + 1);
  if (shortaddr >= 0xFFF0) {
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "invalid shortaddr=0x%04X"), shortaddr);
    return -1;
  }

  // check if the device exists, if not skip the record
  Z_Device & device = zigbee_devices.findShortAddr(shortaddr);
  if (&device != nullptr) {

    // parse the rest
    bool ret = hydrateDeviceData(device, buf, start + 3, segment_len - 3);

    if (!ret) { return -1; }
  }
  return segment_len + 1;
}

// Parse the entire blob
// return true if ok
bool hydrateDevicesDataBlob(const class SBuffer & buf, size_t start, size_t len) {
  // read number of devices
  uint8_t num_devices = buf.get8(start);
  if (num_devices > 0x80) {
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "wrong number of devices=%d"), num_devices);
    return false;
  }

  size_t offset = 0;
  for (uint32_t cur_dev_num = 0; (cur_dev_num < num_devices) && (offset + 4 <= len); cur_dev_num++) {
    int32_t segment_len = hydrateSingleDevice(buf, offset, len);

    // advance buffer
    if (segment_len <= 0) { return false; }
    offset += segment_len;
  }
  return true;
}

class SBuffer hibernateDeviceData(const struct Z_Device & device, bool log = false) {
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

    if (log) {
      size_t buf_len = buf.len() - 3;
      char hex[2*buf_len + 1];
      // skip first 3 bytes
      ToHex_P(buf.buf(3), buf_len, hex, sizeof(hex));

      Response_P(PSTR("{\"" D_PRFX_ZB D_CMND_ZIGBEE_DATA "\":\"ZbData 0x%04X,%s\"}"), device.shortaddr, hex);
      MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, PSTR(D_PRFX_ZB D_CMND_ZIGBEE_DATA));
    }
  }

  return buf;
}

void hibernateAllData(void) {

  // first prefix is number of devices
  uint8_t device_num = zigbee_devices.devicesSize();

  for (const auto & device : zigbee_devices.getDevices()) {
    // allocte a buffer for a single device
    SBuffer buf = hibernateDeviceData(device, true);    // log
    if (buf.len() > 0) {
      // TODO store in EEPROM
    }
  }

}

#endif // USE_ZIGBEE
