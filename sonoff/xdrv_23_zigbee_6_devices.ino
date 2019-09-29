/*
  xdrv_23_zigbee.ino - zigbee support for Sonoff-Tasmota

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

#include <vector>
#include <map>

typedef struct Z_Device {
  uint16_t              shortaddr;
  uint64_t              longaddr;       // 0x00 means unspecified
  std::vector<uint8_t>  endpoints;
  std::vector<uint32_t> clusters_in;    // encoded as high 16 bits is endpoint, low 16 bits is cluster number
  std::vector<uint32_t> clusters_out;   // encoded as high 16 bits is endpoint, low 16 bits is cluster number
} Z_Device;

std::map<uint16_t, Z_Device> zigbee_devices = {};


template < typename T>
bool findInVector(const std::vector<T>  & vecOfElements, const T  & element) {
	// Find given element in vector
	auto it = std::find(vecOfElements.begin(), vecOfElements.end(), element);

	if (it != vecOfElements.end()) {
		return true;
	} else {
		return false;
	}
}

// insert an entry when it is known it is missing
void Z_InsertShortAddrEntry(uint16_t shortaddr, uint64_t longaddr) {
  Z_Device device = { shortaddr, longaddr,
                      std::vector<uint8_t>(),
                      std::vector<uint32_t>(),
                      std::vector<uint32_t>() };
  zigbee_devices[shortaddr] = device;
}

void Z_AddDeviceLongAddr(uint16_t shortaddr, uint64_t longaddr) {
  // is the short address already recorded?
  if (0 == zigbee_devices.count(shortaddr)) {
    // No, add an entry
    Z_InsertShortAddrEntry(shortaddr, longaddr);
  } else {
    // Yes, update the longaddr if necessary
    Z_Device &device = zigbee_devices[shortaddr];
    uint64_t prev_longaddr = device.longaddr;
    if (prev_longaddr != longaddr) {
      // new device, i.e. collision
      device.longaddr = longaddr;
      device.endpoints.clear();
      device.clusters_in.clear();
      device.clusters_out.clear();
    }
  }
}

void Z_AddDeviceEndpoint(uint16_t shortaddr, uint8_t endpoint) {
  if (0 == zigbee_devices.count(shortaddr)) {
    // No entry
    Z_InsertShortAddrEntry(shortaddr, 0);
  }
  Z_Device &device = zigbee_devices[shortaddr];
  if (!findInVector(device.endpoints, endpoint)) {
    device.endpoints.push_back(endpoint);
  }
}

void Z_AddDeviceCluster(uint16_t shortaddr, uint8_t endpoint, uint16_t cluster, bool out) {
  if (0 == zigbee_devices.count(shortaddr)) {
    // No entry
    Z_InsertShortAddrEntry(shortaddr, 0);
  }
  Z_Device &device = zigbee_devices[shortaddr];
  if (!findInVector(device.endpoints, endpoint)) {
    device.endpoints.push_back(endpoint);
  }
  uint32_t ep_cluster = (endpoint << 16) | cluster;
  if (!out) {
    if (!findInVector(device.clusters_in, ep_cluster)) {
      device.clusters_in.push_back(ep_cluster);
    }
  } else { // out
    if (!findInVector(device.clusters_out, ep_cluster)) {
      device.clusters_out.push_back(ep_cluster);
    }
  }
}

String Z_DumpDevices(void) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  JsonObject& devices = json.createNestedObject(F("ZigbeeDevices"));

  for (std::map<uint16_t, Z_Device>::iterator it = zigbee_devices.begin(); it != zigbee_devices.end(); ++it) {
    uint16_t shortaddr = it->first;
    Z_Device& device = it->second;
    char hex[20];

    snprintf_P(hex, sizeof(hex), PSTR("0x%04X"), shortaddr);
    JsonObject& dev = devices.createNestedObject(hex);
    dev[F("ShortAddr")] = hex;

    Uint64toHex(device.longaddr, hex, 64);
    dev[F("IEEEAddr")] = hex;

    JsonArray& dev_endpoints = dev.createNestedArray(F("Endpoints"));
    for (std::vector<uint8_t>::iterator ite = device.endpoints.begin() ; ite != device.endpoints.end(); ++ite) {
      uint8_t endpoint = *ite;

      snprintf_P(hex, sizeof(hex), PSTR("0x%02X"), endpoint);
      dev_endpoints.add(hex);
    }

    JsonObject& dev_clusters_in = dev.createNestedObject(F("Clusters_in"));
    for (std::vector<uint32_t>::iterator itc = device.clusters_in.begin() ; itc != device.clusters_in.end(); ++itc) {
      uint16_t cluster = *itc & 0xFFFF;
      uint8_t  endpoint = (*itc >> 16) & 0xFF;

      snprintf_P(hex, sizeof(hex), PSTR("0x%02X"), endpoint);
      if (!dev_clusters_in.containsKey(hex)) {
        dev_clusters_in.createNestedArray(hex);
      }
      JsonArray &cluster_arr = dev_clusters_in[hex];

      snprintf_P(hex, sizeof(hex), PSTR("0x%04X"), cluster);
      cluster_arr.add(hex);
    }

    JsonObject& dev_clusters_out = dev.createNestedObject(F("Clusters_out"));
    for (std::vector<uint32_t>::iterator itc = device.clusters_out.begin() ; itc != device.clusters_out.end(); ++itc) {
      uint16_t cluster = *itc & 0xFFFF;
      uint8_t  endpoint = (*itc >> 16) & 0xFF;

      snprintf_P(hex, sizeof(hex), PSTR("0x%02X"), endpoint);
      if (!dev_clusters_out.containsKey(hex)) {
        dev_clusters_out.createNestedArray(hex);
      }
      JsonArray &cluster_arr = dev_clusters_out[hex];

      snprintf_P(hex, sizeof(hex), PSTR("0x%04X"), cluster);
      cluster_arr.add(hex);
    }
  }
  String payload = "";
  payload.reserve(200);
  json.printTo(payload);
  return payload;
}

#endif // USE_ZIGBEE
