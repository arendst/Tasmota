/*
  xdrv_23_zigbee_1_headers.ino - zigbee support for Tasmota

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

// contains some definitions for functions used before their declarations

class ZigbeeZCLSendMessage {
public:
  uint16_t shortaddr;
  uint16_t groupaddr;
  uint16_t clusterId;
  uint8_t endpoint;
  uint8_t cmdId;
  uint16_t manuf;
  bool clusterSpecific;
  bool needResponse;
  uint8_t transacId;    // ZCL transaction number
  const uint8_t *msg;
  size_t len;
};

void ZigbeeZCLSend_Raw(const ZigbeeZCLSendMessage &zcl);
bool ZbAppendWriteBuf(SBuffer & buf, const Z_attribute & attr, bool prepend_status_ok = false);

uint32_t parseHex(const char **data, size_t max_len = 8) {
  uint32_t ret = 0;
  for (uint32_t i = 0; i < max_len; i++) {
    int8_t v = hexValue(**data);
    if (v < 0) { break; }     // non hex digit, we stop parsing
    ret = (ret << 4) | v;
    *data += 1;
  }
  return ret;
}

#endif // USE_ZIGBEE
