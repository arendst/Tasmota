/*
 * Copyright 2020-2025 Ryan Powell <ryan@nable-embedded.io> and
 * esp-nimble-cpp, NimBLE-Arduino contributors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef NIMBLE_CPP_HIDDEVICE_H_
#define NIMBLE_CPP_HIDDEVICE_H_

#include "nimconfig.h"
#if CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_BROADCASTER && CONFIG_BT_NIMBLE_ROLE_PERIPHERAL

# include <stdint.h>
# include <string>

# define GENERIC_HID     0x03C0
# define HID_KEYBOARD    0x03C1
# define HID_MOUSE       0x03C2
# define HID_JOYSTICK    0x03C3
# define HID_GAMEPAD     0x03C4
# define HID_TABLET      0x03C5
# define HID_CARD_READER 0x03C6
# define HID_DIGITAL_PEN 0x03C7
# define HID_BARCODE     0x03C8

# define PNPVersionField(MajorVersion, MinorVersion, PatchVersion) \
     ((MajorVersion << 16) & 0xFF00) | ((MinorVersion << 8) & 0x00F0) | (PatchVersion & 0x000F)

class NimBLEServer;
class NimBLEService;
class NimBLECharacteristic;

/**
 * @brief A model of a BLE Human Interface Device.
 */
class NimBLEHIDDevice {
  public:
    NimBLEHIDDevice(NimBLEServer* server);

    void                  setReportMap(uint8_t* map, uint16_t);
    void                  startServices();
    bool                  setManufacturer(const std::string& name);
    void                  setPnp(uint8_t sig, uint16_t vid, uint16_t pid, uint16_t version);
    void                  setHidInfo(uint8_t country, uint8_t flags);
    void                  setBatteryLevel(uint8_t level, bool notify = false);
    NimBLECharacteristic* getBatteryLevel();
    NimBLECharacteristic* getReportMap();
    NimBLECharacteristic* getHidControl();
    NimBLECharacteristic* getInputReport(uint8_t reportId);
    NimBLECharacteristic* getOutputReport(uint8_t reportId);
    NimBLECharacteristic* getFeatureReport(uint8_t reportId);
    NimBLECharacteristic* getProtocolMode();
    NimBLECharacteristic* getBootInput();
    NimBLECharacteristic* getBootOutput();
    NimBLECharacteristic* getPnp();
    NimBLECharacteristic* getHidInfo();
    NimBLEService*        getDeviceInfoService();
    NimBLEService*        getHidService();
    NimBLEService*        getBatteryService();

  private:
    NimBLEService* m_deviceInfoSvc{nullptr}; // 0x180a
    NimBLEService* m_hidSvc{nullptr};        // 0x1812
    NimBLEService* m_batterySvc{nullptr};    // 0x180f

    NimBLECharacteristic* m_manufacturerChr{nullptr}; // 0x2a29
    NimBLECharacteristic* m_pnpChr{nullptr};          // 0x2a50
    NimBLECharacteristic* m_hidInfoChr{nullptr};      // 0x2a4a
    NimBLECharacteristic* m_reportMapChr{nullptr};    // 0x2a4b
    NimBLECharacteristic* m_hidControlChr{nullptr};   // 0x2a4c
    NimBLECharacteristic* m_protocolModeChr{nullptr}; // 0x2a4e
    NimBLECharacteristic* m_batteryLevelChr{nullptr}; // 0x2a19

    NimBLECharacteristic* locateReportCharacteristicByIdAndType(uint8_t reportId, uint8_t reportType);
};

#endif // CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_BROADCASTER && CONFIG_BT_NIMBLE_ROLE_PERIPHERAL
#endif // NIMBLE_CPP_HIDDEVICE_H_
