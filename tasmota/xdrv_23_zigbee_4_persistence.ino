/*
  xdrv_23_zigbee.ino - zigbee support for Tasmota

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

// Ensure persistence of devices into Flash
//
// Structure:
// (from file info):
// uint16 - start address in Flash (offset)
// uint16 - length in bytes (makes sure parsing stops)
//
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
// int8_t - bulbtype

// Memory footprint
#ifdef ESP8266
const static uint16_t z_spi_start_sector = 0xFF;  // Force last bank of first MB
const static uint8_t* z_spi_start    = (uint8_t*) 0x402FF000;  // 0x402FF000
const static uint8_t* z_dev_start    = z_spi_start + 0x0800;   // 0x402FF800 - 2KB
const static size_t   z_spi_len      = 0x1000;   // 4kb blocks
const static size_t   z_block_offset = 0x0800;
const static size_t   z_block_len    = 0x0800;   // 2kb
#else  // ESP32
uint8_t* z_dev_start;
const static size_t   z_spi_len      = 0x1000;   // 4kb blocks
const static size_t   z_block_offset = 0x0000;   // No offset needed
const static size_t   z_block_len    = 0x1000;   // 4kb
#endif

class z_flashdata_t {
public:
  uint32_t name;    // simple 4 letters name. Currently 'skey', 'crt ', 'crt1', 'crt2'
  uint16_t len;     // len of object
  uint16_t reserved; // align on 4 bytes boundary
};

const static uint32_t ZIGB_NAME = 0x3167697A; // 'zig1' little endian
const static size_t   Z_MAX_FLASH = z_block_len - sizeof(z_flashdata_t);  // 2040


class SBuffer hibernateDevice(const struct Z_Device &device) {
  SBuffer buf(128);

  buf.add8(0x00);     // overall length, will be updated later
  buf.add16(device.shortaddr);
  buf.add64(device.longaddr);

  uint32_t endpoints_count = 0;
  for (endpoints_count = 0; endpoints_count < endpoints_max; endpoints_count++) {
    if (0x00 == device.endpoints[endpoints_count]) { break; }
  }

  buf.add8(endpoints_count);
  // iterate on endpoints
  for (uint32_t i = 0; i < endpoints_max; i++) {
    uint8_t endpoint = device.endpoints[i];
    if (0x00 == endpoint) { break; }      // stop

    buf.add8(endpoint);
    buf.add16(0x0000);   // profile_id, not used anymore

    // removed clusters_in
    buf.add8(0xFF);      // end of endpoint marker

    // no more storage of clusters_out
    buf.add8(0xFF);      // end of endpoint marker
  }

  // ModelID
  if (device.modelId) {
    size_t model_len = strlen(device.modelId);
    if (model_len > 32) { model_len = 32; }       // max 32 chars
    buf.addBuffer(device.modelId, model_len);
  }
  buf.add8(0x00);     // end of string marker

  // ManufID
  if (device.manufacturerId) {
    size_t manuf_len = strlen(device.manufacturerId);
    if (manuf_len > 32) { manuf_len = 32; }       // max 32 chars
    buf.addBuffer(device.manufacturerId, manuf_len);
  }
  buf.add8(0x00);     // end of string marker

  // FriendlyName
  if (device.friendlyName) {
    size_t frname_len = strlen(device.friendlyName);
    if (frname_len > 32) {frname_len = 32; }       // max 32 chars
    buf.addBuffer(device.friendlyName, frname_len);
  }
  buf.add8(0x00);     // end of string marker

  // Hue Bulbtype
  buf.add8(device.bulbtype);

  // update overall length
  buf.set8(0, buf.len());

  return buf;
}

class SBuffer hibernateDevices(void) {
  SBuffer buf(2048);

  size_t devices_size = zigbee_devices.devicesSize();
  if (devices_size > 32) { devices_size = 32; }         // arbitrarily limit to 32 devices, for now
  buf.add8(devices_size);    // number of devices

  for (uint32_t i = 0; i < devices_size; i++) {
    const Z_Device & device = zigbee_devices.devicesAt(i);
    const SBuffer buf_device = hibernateDevice(device);
    buf.addBuffer(buf_device);
  }

  size_t buf_len = buf.len();
  if (buf_len > 2040) {
    AddLog_P2(LOG_LEVEL_ERROR, PSTR(D_LOG_ZIGBEE "Devices list too big to fit in Flash (%d)"), buf_len);
  }

  // Log
  char *hex_char = (char*) malloc((buf_len * 2) + 2);
  if (hex_char) {
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_ZIGBEE "ZbFlashStore %s"),
                                    ToHex_P(buf.getBuffer(), buf_len, hex_char, (buf_len * 2) + 2));
    free(hex_char);
  }

  return buf;
}

void hydrateDevices(const SBuffer &buf) {
  uint32_t buf_len = buf.len();
  if (buf_len <= 10) { return; }

  uint32_t k = 0;
  uint32_t num_devices = buf.get8(k++);
//size_t before = 0;
  for (uint32_t i = 0; (i < num_devices) && (k < buf_len); i++) {
    uint32_t dev_record_len = buf.get8(k);

    SBuffer buf_d = buf.subBuffer(k, dev_record_len);

    uint32_t d = 1;   // index in device buffer
    uint16_t shortaddr = buf_d.get16(d);  d += 2;
    uint64_t longaddr  = buf_d.get64(d);  d += 8;
    zigbee_devices.updateDevice(shortaddr, longaddr);   // update device's addresses

    uint32_t endpoints = buf_d.get8(d++);
    for (uint32_t j = 0; j < endpoints; j++) {
      uint8_t ep = buf_d.get8(d++);
      uint16_t ep_profile = buf_d.get16(d);  d += 2;
      zigbee_devices.addEndpoint(shortaddr, ep);

      // in clusters
      while (d < dev_record_len) {      // safe guard against overflow
        uint8_t ep_cluster = buf_d.get8(d++);
        if (0xFF == ep_cluster) { break; }   // end of block
        // ignore
      }
      // out clusters
      while (d < dev_record_len) {      // safe guard against overflow
        uint8_t ep_cluster = buf_d.get8(d++);
        if (0xFF == ep_cluster) { break; }   // end of block
        // ignore
      }
    }
//AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "Device 0x%04X Memory3.shrink = %d"), shortaddr, ESP_getFreeHeap());

    // parse 3 strings
    char empty[] = "";

    // ManufID
    uint32_t s_len = buf_d.strlen_s(d);
    char *ptr = s_len ? buf_d.charptr(d) : empty;
    zigbee_devices.setModelId(shortaddr, ptr);
    d += s_len + 1;

    // ManufID
    s_len = buf_d.strlen_s(d);
    ptr = s_len ? buf_d.charptr(d) : empty;
    zigbee_devices.setManufId(shortaddr, ptr);
    d += s_len + 1;

    // FriendlyName
    s_len = buf_d.strlen_s(d);
    ptr = s_len ? buf_d.charptr(d) : empty;
    zigbee_devices.setFriendlyName(shortaddr, ptr);
    d += s_len + 1;

    // Hue bulbtype - if present
    if (d < dev_record_len) {
      zigbee_devices.setHueBulbtype(shortaddr, buf_d.get8(d));
      d++;
    }

    // next iteration
    k += dev_record_len;
//AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "Device %d After  Memory = %d"), i, ESP_getFreeHeap());
  }
}

void loadZigbeeDevices(void) {
#ifdef ESP32
  // first copy SPI buffer into ram
  uint8_t *spi_buffer = (uint8_t*) malloc(z_spi_len);
  if (!spi_buffer) {
    AddLog_P2(LOG_LEVEL_ERROR, PSTR(D_LOG_ZIGBEE "Cannot allocate 4KB buffer"));
    return;
  }
  ZigbeeRead(&spi_buffer, z_spi_len);
  z_dev_start = spi_buffer;
#endif  // ESP32
  z_flashdata_t flashdata;
  memcpy_P(&flashdata, z_dev_start, sizeof(z_flashdata_t));
//  AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_ZIGBEE "Memory %d"), ESP_getFreeHeap());
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_ZIGBEE "Zigbee signature in Flash: %08X - %d"), flashdata.name, flashdata.len);

  // Check the signature
  if ((flashdata.name == ZIGB_NAME) && (flashdata.len > 0)) {
    uint16_t buf_len = flashdata.len;
    // parse what seems to be a valid entry
    SBuffer buf(buf_len);
    buf.addBuffer(z_dev_start + sizeof(z_flashdata_t), buf_len);
    AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "Zigbee devices data in Flash (%d bytes)"), buf_len);
    hydrateDevices(buf);
    zigbee_devices.clean();   // don't write back to Flash what we just loaded
  } else {
    AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "No zigbee devices data in Flash"));
  }
//  AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_ZIGBEE "Memory %d"), ESP_getFreeHeap());
#ifdef ESP32
  free(spi_buffer);
#endif  // ESP32
}

void saveZigbeeDevices(void) {
  SBuffer buf = hibernateDevices();
  size_t buf_len = buf.len();
  if (buf_len > Z_MAX_FLASH) {
    AddLog_P2(LOG_LEVEL_ERROR, PSTR(D_LOG_ZIGBEE "Buffer too big to fit in Flash (%d bytes)"), buf_len);
    return;
  }

  // first copy SPI buffer into ram
  uint8_t *spi_buffer = (uint8_t*) malloc(z_spi_len);
  if (!spi_buffer) {
    AddLog_P2(LOG_LEVEL_ERROR, PSTR(D_LOG_ZIGBEE "Cannot allocate 4KB buffer"));
    return;
  }
  // copy the flash into RAM to make local change, and write back the whole buffer
#ifdef ESP8266
  ESP.flashRead(z_spi_start_sector * SPI_FLASH_SEC_SIZE, (uint32_t*) spi_buffer, SPI_FLASH_SEC_SIZE);
#else  // ESP32
  ZigbeeRead(&spi_buffer, z_spi_len);
#endif  // ESP8266 - ESP32

  z_flashdata_t *flashdata = (z_flashdata_t*)(spi_buffer + z_block_offset);
  flashdata->name = ZIGB_NAME;
  flashdata->len = buf_len;
  flashdata->reserved = 0;

  memcpy(spi_buffer + z_block_offset + sizeof(z_flashdata_t), buf.getBuffer(), buf_len);

  // buffer is now ready, write it back
#ifdef ESP8266
  if (ESP.flashEraseSector(z_spi_start_sector)) {
    ESP.flashWrite(z_spi_start_sector * SPI_FLASH_SEC_SIZE, (uint32_t*) spi_buffer, SPI_FLASH_SEC_SIZE);
  }
  AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "Zigbee Devices Data store in Flash (0x%08X - %d bytes)"), z_dev_start, buf_len);
#else  // ESP32
  ZigbeeWrite(&spi_buffer, z_spi_len);
  AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "Zigbee Devices Data saved (%d bytes)"), buf_len);
#endif  // ESP8266 - ESP32
  free(spi_buffer);
}

// Erase the flash area containing the ZigbeeData
void eraseZigbeeDevices(void) {
  zigbee_devices.clean();     // avoid writing data to flash after erase
#ifdef ESP8266
  // first copy SPI buffer into ram
  uint8_t *spi_buffer = (uint8_t*) malloc(z_spi_len);
  if (!spi_buffer) {
    AddLog_P2(LOG_LEVEL_ERROR, PSTR(D_LOG_ZIGBEE "Cannot allocate 4KB buffer"));
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
  AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "Zigbee Devices Data erased (0x%08X - %d bytes)"), z_dev_start, z_block_len);
#else  // ESP32
  ZigbeeErase();
  AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "Zigbee Devices Data erased (%d bytes)"), z_block_len);
#endif  // ESP8266 - ESP32
}

#endif // USE_ZIGBEE
