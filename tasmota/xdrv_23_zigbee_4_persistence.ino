/*
  xdrv_23_zigbee.ino - zigbee support for Tasmota

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

// Ensure persistence of devices into Flash
//
// Structure:
// (from file info):
// uint16 - start address in Flash (offset)
// uint16 - length in bytes (makes sure parsing stops)
//
// First byte:
// 0x00 - Empty or V3 format
// 0x01-0xFE - Legacy format
// 0xFF - invalid
//
//
// V1 Legacy
// =========
// File structure:
// uint8 - number of devices, 0=none, 0xFF=invalid entry (probably Flash was erased)
//
// [Array of devices]
// [Offset = 2]
// uint8  - length of device record
// uint16 - short address
// uint64 - long IEEE address
// uint8  - number of endpoints
// [Array of endpoints]
// uint8  - endpoint number
// uint16 - profileID of the endpoint
// Array of uint8 - clusters In  codes, 0xFF end marker
// Array of uint8 - clusters Out codes, 0xFF end marker
//
// str    - ModelID (null terminated C string, 32 chars max)
// str    - Manuf   (null terminated C string, 32 chars max)
// str    - FriendlyName   (null terminated C string, 32 chars max)
// reserved for extensions
//  -- V2 --
// int8_t - zigbee profile of the device
//
// =======================
// v3 with version number
// File structure:
//
// uint8 - number of devices, 0=none, 0xFF=invalid entry (probably Flash was erased)
//
// [Array of devices]
// [Offset = 2]
// uint8  - length of device record
// uint16 - short address
// uint64 - long IEEE address
//
// str    - ModelID (null terminated C string, 32 chars max)
// str    - Manuf   (null terminated C string, 32 chars max)
// str    - FriendlyName   (null terminated C string, 32 chars max)
//
// [Array of endpoints]
// uint8  - endpoint number, 0xFF marks the end of endpoints
// uint8[] - list of configuration bytes, 0xFF marks the end
// i.e. 0xFF-0xFF marks the end of the array of endpoints
//


// Memory footprint
#ifdef ESP8266
const static uint16_t z_spi_start_sector = 0xFF;  // Force last bank of first MB
const static uint8_t* z_spi_start    = (uint8_t*) 0x402FF000;  // 0x402FF000
const static uint8_t* z_dev_start    = z_spi_start + 0x0800;   // 0x402FF800 - 2KB
const static size_t   z_spi_len      = 0x1000;   // 4kb blocks
const static size_t   z_block_offset = 0x0800;
const static size_t   z_block_len    = 0x0800;   // 2kb
#endif  // ESP8266
#ifdef ESP32
uint8_t* z_dev_start;
const static size_t   z_spi_len      = 0x1000;   // 4kb blocks
const static size_t   z_block_offset = 0x0000;   // No offset needed
const static size_t   z_block_len    = 0x1000;   // 4kb
#endif  // ESP32

// Each entry consumes 8 bytes
class Z_Flashentry {
public:
  uint32_t name;    // simple 4 letters name. Currently 'zig1', 'zig2'. 0xFFFFFFFF if not entry
  uint16_t len;     // len of object in bytes, 0xFFFF if no entry
  uint16_t start;   // address of start, 0xFFFF if empty, must be aligned on 128 bytes boundaries
};

class Z_Flashdirectory {
public:
  // 8 bytes header
  uint32_t magic;         // magic value 'Tsmt' to check that the block is initialized
  uint32_t clock;         // clock vector to discard entries that are made before this one. This should be incremented by 1 for each new entry (future anti-weavering)
  // entries, 14*8 = 112 bytes
  Z_Flashentry entries[14];
  uint32_t name;    // simple 4 letters name. Currently 'skey', 'crt ', 'crt1', 'crt2'
  uint16_t len;     // len of object
  uint16_t reserved; // align on 4 bytes boundary
  // link to next entry, none for now, but may be used for anti-weavering
  uint16_t next_dir;  // 0xFFFF if none
  uint16_t reserved1; // must be 0xFFFF
  uint32_t reserved2; // must be 0xFFFFFFFF
};


const static uint32_t ZIGB_NAME1 = 0x3167697A; // 'zig1' little endian
const static uint32_t ZIGB_NAME2 = 0x3267697A; // 'zig2' little endian, v2
const static uint32_t ZIGB_DATA2 = 0x32746164; // 'dat2' little endian, v2
const static size_t   Z_MAX_FLASH = z_block_len - sizeof(Z_Flashentry);  // 2040

bool hibernateDeviceConfiguration(SBuffer & buf, const class Z_Data_Set & data, uint8_t endpoint) {
  bool found = false;
  for (auto & elt : data) {
    if (endpoint == elt.getEndpoint()) {
      buf.add8(elt.getConfigByte());
      found = true;
    }
  }
  return found;
}

SBuffer hibernateDevicev2(const struct Z_Device &device) {
  SBuffer buf(128);

  buf.add8(0x00);     // overall length, will be updated later
  buf.add16(device.shortaddr);
  buf.add64(device.longaddr);

  char *names[3] = { device.modelId, device.manufacturerId, device.friendlyName };

  for (uint32_t i=0; i<nitems(names); i++) {
    char *p = names[i];
    if (p) {
      size_t len = strlen(p);
      if (len > 32) { len = 32; }       // max 32 chars
      buf.addBuffer(p, len);
    }
    buf.add8(0x00);     // end of string marker
  }

  // check if we need to write fake endpoint 0x00
  buf.add8(0x00);
  if (hibernateDeviceConfiguration(buf, device.data, 0)) {
    buf.add8(0xFF); // end of configuration
  } else {
    buf.setLen(buf.len()-1);    // remove 1 byte header
  }
  // scan endpoints
  for (uint32_t i=0; i<endpoints_max; i++) {
    uint8_t endpoint = device.endpoints[i];
    if (0x00 == endpoint) { break; }
    buf.add8(endpoint);
    hibernateDeviceConfiguration(buf, device.data, endpoint);
    buf.add8(0xFF); // end of configuration
  }
  buf.add8(0xFF); // end of endpoints

  // update overall length
  buf.set8(0, buf.len());

  return buf;
}

SBuffer hibernateDevices(void) {
  SBuffer buf(2048);

  size_t devices_size = zigbee_devices.devicesSize();
  if (devices_size > 32) { devices_size = 32; }         // arbitrarily limit to 32 devices, for now
  buf.add8(devices_size);    // number of devices

  for (uint32_t i = 0; i < devices_size; i++) {
    const Z_Device & device = zigbee_devices.devicesAt(i);
    const SBuffer buf_device = hibernateDevicev2(device);
    buf.addBuffer(buf_device);
  }

  return buf;
}

// parse a single string from the saved data
// if something wrong happens, returns nullptr to ignore the string
// Index d is incremented to just after the string
const char * hydrateSingleString(const SBuffer & buf, uint32_t *d) {
  size_t s_len = buf.strlen(*d);
  const char * ptr = s_len ? buf.charptr(*d) : "";
  *d += s_len + 1;
  return ptr;
}

void hydrateSingleDevice(const SBuffer & buf_d, uint32_t version = 2) {
  uint32_t d = 1;   // index in device buffer
  uint16_t shortaddr = buf_d.get16(d);  d += 2;
  uint64_t longaddr  = buf_d.get64(d);  d += 8;
  size_t   buf_len = buf_d.len();
  Z_Device & device = zigbee_devices.updateDevice(shortaddr, longaddr);   // update device's addresses

  if (1 == version) {
    uint32_t endpoints = buf_d.get8(d++);
    for (uint32_t j = 0; j < endpoints; j++) {
      uint8_t ep = buf_d.get8(d++);
      // uint16_t ep_profile = buf_d.get16(d);  d += 2;
      device.addEndpoint(ep);

      // in clusters
      while (d < buf_len) {      // safe guard against overflow
        uint8_t ep_cluster = buf_d.get8(d++);
        if (0xFF == ep_cluster) { break; }   // end of block
        // ignore
      }
      // out clusters
      while (d < buf_len) {      // safe guard against overflow
        uint8_t ep_cluster = buf_d.get8(d++);
        if (0xFF == ep_cluster) { break; }   // end of block
        // ignore
      }
    }
  }

  // ModelId
  device.setModelId(hydrateSingleString(buf_d, &d));

  // ManufID
  device.setManufId(hydrateSingleString(buf_d, &d));

  // FriendlyName
  device.setFriendlyName(hydrateSingleString(buf_d, &d));

  if (d >= buf_len) { return; }

  // Hue bulbtype - if present
  if (1 == version) {
    device.setLightChannels(buf_d.get8(d));
    d++;
  } else if (2 == version) {
    // v2 parser
    while (d < buf_len) {
      uint8_t ep = buf_d.get8(d++);
      if (0xFF == ep) { break; }        // ep 0xFF marks the end of the endpoints
      if (ep > 240) { ep = 0xFF; }      // ep == 0xFF means ignore
      device.addEndpoint(ep);           // it will ignore invalid endpoints
      while (d < buf_len) {
        uint8_t config_type = buf_d.get8(d++);
        if (0xFF == config_type) { break; }                                // 0xFF marks the end of congiguration
        uint8_t config = config_type & 0x0F;
        Z_Data_Type type = (Z_Data_Type) (config_type >> 4);
        // set the configuration
        if (ep != 0xFF) {
          Z_Data & z_data = device.data.getByType(type, ep);
          if (&z_data != nullptr) {
            z_data.setConfig(config);
            Z_Data_Set::updateData(z_data);
          }
        }
      }
    }
  }
}

void hydrateDevices(const SBuffer &buf, uint32_t version) {
  uint32_t buf_len = buf.len();
  if (buf_len <= 10) { return; }

  uint32_t k = 0;   // byte index in global buffer
  uint32_t num_devices = buf.get8(k++);
  for (uint32_t i = 0; (i < num_devices) && (k < buf_len); i++) {
    uint32_t dev_record_len = buf.get8(k);

    SBuffer buf_d = buf.subBuffer(k, dev_record_len);
    hydrateSingleDevice(buf_d, version);

    // next iteration
    k += dev_record_len;
  }
}

// dump = true, only dump to logs, don't actually load
void loadZigbeeDevices(bool dump_only = false) {
#ifdef USE_ZIGBEE_EZSP
  if (loadZigbeeDevicesFromEEPROM()) {
    return;       // we succesfully loaded from EEPROM, skip the read from Flash
  }
#endif
#ifdef ESP32
  // first copy SPI buffer into ram
  uint8_t *spi_buffer = (uint8_t*) malloc(z_spi_len);
  if (!spi_buffer) {
    AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_ZIGBEE "Cannot allocate 4KB buffer"));
    return;
  }
#ifdef USE_UFILESYS
  TfsLoadFile(TASM_FILE_ZIGBEE, spi_buffer, z_spi_len);
#endif
  z_dev_start = spi_buffer;
#endif  // ESP32
  Z_Flashentry flashdata;
  memcpy_P(&flashdata, z_dev_start, sizeof(Z_Flashentry));
//  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_ZIGBEE "Memory %d"), ESP_getFreeHeap());
  // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_ZIGBEE "Zigbee signature in Flash: %08X - %d"), flashdata.name, flashdata.len);

  // Check the signature
  if ( ((flashdata.name == ZIGB_NAME1) || (flashdata.name == ZIGB_NAME2))
      && (flashdata.len > 0)) {
    uint16_t buf_len = flashdata.len;
    uint32_t version = (flashdata.name == ZIGB_NAME2) ? 2 : 1;
    // parse what seems to be a valid entry
    SBuffer buf(buf_len);
    buf.addBuffer(z_dev_start + sizeof(Z_Flashentry), buf_len);
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "Zigbee device information in %s (%d bytes)"), PSTR("Flash"), buf_len);

    if (dump_only) {
      size_t buf_len = buf.len();
      if (buf_len > 192) { buf_len = 192; }
      AddLogBuffer(LOG_LEVEL_INFO, buf.getBuffer(), buf_len);
      // Serial.printf(">> Buffer=");
      // for (uint32_t i=0; i<buf.len(); i++) Serial.printf("%02X ", buf.get8(i));
      // Serial.printf("\n");
    } else {
      hydrateDevices(buf, version);
      zigbee_devices.clean();   // don't write back to Flash what we just loaded
    }
  } else {
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "No Zigbee device information in %s"), PSTR("Flash"));
  }
#ifdef ESP32
  free(spi_buffer);
#endif  // ESP32
}

void saveZigbeeDevices(void) {
#ifdef USE_ZIGBEE_EZSP
  if (zigbee.eeprom_ready) {
    if (hibernateDevicesInEEPROM()) {
      return;   // saved in EEPROM successful, non need to write in Flash
    }
  }
#endif
  SBuffer buf = hibernateDevices();
  size_t buf_len = buf.len();
  if (buf_len > 2040) {
    AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_ZIGBEE "Buffer too big to fit in Flash (%d bytes)"), buf_len);
    return;
  }

  // first copy SPI buffer into ram
  uint8_t *spi_buffer = (uint8_t*) malloc(z_spi_len);
  if (!spi_buffer) {
    AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_ZIGBEE "Cannot allocate 4KB buffer"));
    return;
  }
  // copy the flash into RAM to make local change, and write back the whole buffer
#ifdef ESP8266
  ESP.flashRead(z_spi_start_sector * SPI_FLASH_SEC_SIZE, (uint32_t*) spi_buffer, SPI_FLASH_SEC_SIZE);
#endif  // ESP8266
#ifdef ESP32
#ifdef USE_UFILESYS
  TfsLoadFile(TASM_FILE_ZIGBEE, spi_buffer, z_spi_len);
#endif
#endif  // ESP32

  Z_Flashentry *flashdata = (Z_Flashentry*)(spi_buffer + z_block_offset);
  flashdata->name = ZIGB_NAME2;     // v2
  flashdata->len = buf_len;
  flashdata->start = 0;

  memcpy(spi_buffer + z_block_offset + sizeof(Z_Flashentry), buf.getBuffer(), buf_len);

  // buffer is now ready, write it back
#ifdef ESP8266
  if (ESP.flashEraseSector(z_spi_start_sector)) {
    ESP.flashWrite(z_spi_start_sector * SPI_FLASH_SEC_SIZE, (uint32_t*) spi_buffer, SPI_FLASH_SEC_SIZE);
  }
  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "Zigbee Devices Data store in Flash (0x%08X - %d bytes)"), z_dev_start, buf_len);
#endif  // ESP8266
#ifdef ESP32
#ifdef USE_UFILESYS
  TfsSaveFile(TASM_FILE_ZIGBEE, spi_buffer, z_spi_len);
#endif
  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "Zigbee Devices Data saved in %s (%d bytes)"), PSTR("Flash"), buf_len);
#endif  // ESP32
  free(spi_buffer);
}

// Erase the flash area containing the ZigbeeData
void eraseZigbeeDevices(void) {
  zigbee_devices.clean();     // avoid writing data to flash after erase
#ifdef USE_ZIGBEE_EZSP
  ZFS_Erase();
#endif // USE_ZIGBEE_EZSP
#ifdef ESP8266
  // first copy SPI buffer into ram
  uint8_t *spi_buffer = (uint8_t*) malloc(z_spi_len);
  if (!spi_buffer) {
    AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_ZIGBEE "Cannot allocate 4KB buffer"));
    return;
  }
  // copy the flash into RAM to make local change, and write back the whole buffer
  ESP.flashRead(z_spi_start_sector * SPI_FLASH_SEC_SIZE, (uint32_t*) spi_buffer, SPI_FLASH_SEC_SIZE);

  // Fill the Zigbee area with 0xFF
  memset(spi_buffer + z_block_offset, 0xFF, z_block_len);

  // buffer is now ready, write it back
  if (ESP.flashEraseSector(z_spi_start_sector)) {
    ESP.flashWrite(z_spi_start_sector * SPI_FLASH_SEC_SIZE, (uint32_t*) spi_buffer, SPI_FLASH_SEC_SIZE);
  }

  free(spi_buffer);
  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "Zigbee Devices Data erased in %s"), PSTR("Flash"));
#endif  // ESP8266
#ifdef ESP32
#ifdef USE_UFILESYS
  TfsInitFile(TASM_FILE_ZIGBEE, z_block_len, 0xFF);
#endif
  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "Zigbee Devices Data erased (%d bytes)"), z_block_len);
#endif  // ESP32
}

void restoreDumpAllDevices(void) {
  for (const auto & device : zigbee_devices.getDevices()) {
    const SBuffer buf = hibernateDevicev2(device);
    if (buf.len() > 0) {
      Response_P(PSTR("{\"" D_PRFX_ZB D_CMND_ZIGBEE_RESTORE "\":\"ZbRestore %_B\"}"), &buf);
      MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, PSTR(D_PRFX_ZB D_CMND_ZIGBEE_DATA));
    }
  }
}

#endif // USE_ZIGBEE
