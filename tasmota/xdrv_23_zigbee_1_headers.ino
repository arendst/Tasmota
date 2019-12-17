/*
  xdrv_23_zigbee_1_headers.ino - zigbee support for Tasmota

  Copyright (C) 2019  Theo Arends and Stephan Hadinger

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

void ZigbeeZCLSend(uint16_t dtsAddr, uint16_t clusterId, uint8_t endpoint, uint8_t cmdId, bool clusterSpecific, const uint8_t *msg, size_t len, bool disableDefResp = true, uint8_t transacId = 1);

#endif // USE_ZIGBEE
