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

#include "NimBLEHIDDevice.h"
#if CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_BROADCASTER && CONFIG_BT_NIMBLE_ROLE_PERIPHERAL

# include "NimBLEServer.h"
# include "NimBLEService.h"
# include "NimBLE2904.h"

static constexpr uint16_t deviceInfoSvcUuid = 0x180a;
static constexpr uint16_t hidSvcUuid        = 0x1812;
static constexpr uint16_t batterySvcUuid    = 0x180f;

static constexpr uint16_t pnpCharUuid           = 0x2a50;
static constexpr uint16_t hidInfoCharUuid       = 0x2a4a;
static constexpr uint16_t reportMapCharUuid     = 0x2a4b;
static constexpr uint16_t hidControlCharUuid    = 0x2a4c;
static constexpr uint16_t inputReportChrUuid    = 0x2a4d;
static constexpr uint16_t protocolModeCharUuid  = 0x2a4e;
static constexpr uint16_t batteryLevelCharUuid  = 0x2a19;
static constexpr uint16_t batteryLevelDscUuid   = 0x2904;
static constexpr uint16_t featureReportDscUuid  = 0x2908;
static constexpr uint16_t m_manufacturerChrUuid = 0x2a29;
static constexpr uint16_t bootInputChrUuid      = 0x2a22;
static constexpr uint16_t bootOutputChrUuid     = 0x2a32;

/**
 * @brief Construct a default NimBLEHIDDevice object.
 * @param [in] server A pointer to the server instance this HID Device will use.
 */
NimBLEHIDDevice::NimBLEHIDDevice(NimBLEServer* server) {
    // Here we create mandatory services described in bluetooth specification
    m_deviceInfoSvc = server->createService(deviceInfoSvcUuid);
    m_hidSvc        = server->createService(hidSvcUuid);
    m_batterySvc    = server->createService(batterySvcUuid);

    // Mandatory characteristic for device info service
    m_pnpChr = m_deviceInfoSvc->createCharacteristic(pnpCharUuid, NIMBLE_PROPERTY::READ);

    // Mandatory characteristics for HID service
    m_hidInfoChr    = m_hidSvc->createCharacteristic(hidInfoCharUuid, NIMBLE_PROPERTY::READ);
    m_reportMapChr  = m_hidSvc->createCharacteristic(reportMapCharUuid, NIMBLE_PROPERTY::READ);
    m_hidControlChr = m_hidSvc->createCharacteristic(hidControlCharUuid, NIMBLE_PROPERTY::WRITE_NR);
    m_protocolModeChr =
        m_hidSvc->createCharacteristic(protocolModeCharUuid, NIMBLE_PROPERTY::WRITE_NR | NIMBLE_PROPERTY::READ);

    // Mandatory battery level characteristic with notification and presence descriptor
    m_batteryLevelChr =
        m_batterySvc->createCharacteristic(batteryLevelCharUuid, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);
    NimBLE2904* batteryLevelDescriptor = m_batteryLevelChr->create2904();
    batteryLevelDescriptor->setFormat(NimBLE2904::FORMAT_UINT8);
    batteryLevelDescriptor->setUnit(0x27ad); // percentage

    // This value is setup here because its default value in most usage cases, it's very rare to use boot mode
    m_protocolModeChr->setValue(static_cast<uint8_t>(0x01));
} // NimBLEHIDDevice

/**
 * @brief Set the report map data formatting information.
 * @param [in] map A pointer to an array with the values to set.
 * @param [in] size The number of values in the array.
 */
void NimBLEHIDDevice::setReportMap(uint8_t* map, uint16_t size) {
    m_reportMapChr->setValue(map, size);
} // setReportMap

/**
 * @brief Start the HID device services.
 * This function called when all the services have been created.
 */
void NimBLEHIDDevice::startServices() {
    m_deviceInfoSvc->start();
    m_hidSvc->start();
    m_batterySvc->start();
} // startServices

/**
 * @brief Get the manufacturer characteristic (this characteristic is optional).
 * @details The characteristic will be created if not already existing.
 * @returns True if the name was set and/or the characteristic created.
 */
bool NimBLEHIDDevice::setManufacturer(const std::string& name) {
    if (m_manufacturerChr == nullptr) {
        m_manufacturerChr = m_deviceInfoSvc->createCharacteristic(m_manufacturerChrUuid, NIMBLE_PROPERTY::READ);
    }

    if (m_manufacturerChr) {
        m_manufacturerChr->setValue(name);
        return true;
    }

    return false;
} // setManufacturer

/**
 * @brief Sets the Plug n Play characteristic value.
 * @param [in] sig The vendor ID source number.
 * @param [in] vid The vendor ID number.
 * @param [in] pid The product ID number.
 * @param [in] version The produce version number.
 */
void NimBLEHIDDevice::setPnp(uint8_t sig, uint16_t vid, uint16_t pid, uint16_t version) {
    uint8_t pnp[] = {sig,
                     static_cast<uint8_t>(vid & 0xFF),
                     static_cast<uint8_t>((vid >> 8) & 0xFF),
                     static_cast<uint8_t>(pid & 0xFF),
                     static_cast<uint8_t>((pid >> 8) & 0xFF),
                     static_cast<uint8_t>(version & 0xFF),
                     static_cast<uint8_t>((version >> 8) & 0xFF)};

    m_pnpChr->setValue(pnp, sizeof(pnp));
} // setPnp

/**
 * @brief Sets the HID Information characteristic value.
 * @param [in] country The country code for the device.
 * @param [in] flags The HID Class Specification release number to use.
 */
void NimBLEHIDDevice::setHidInfo(uint8_t country, uint8_t flags) {
    uint8_t info[] = {0x11, 0x1, country, flags};
    m_hidInfoChr->setValue(info, sizeof(info));
} // setHidInfo

/**
 * @brief Set the battery level characteristic value.
 * @param [in] level The battery level value.
 * @param [in] notify If true sends a notification to the peer device, otherwise not. default = false
 */
void NimBLEHIDDevice::setBatteryLevel(uint8_t level, bool notify) {
    m_batteryLevelChr->setValue(&level, 1);
    if (notify) {
        m_batteryLevelChr->notify();
    }
} // setBatteryLevel

/**
 * @brief Locate the characteristic for a report ID and a report type.
 *
 * @param [in] reportId Report identifier to locate.
 * @param [in] reportType Type of report (input/output/feature).
 * @return NimBLECharacteristic* The characteristic.
 * @return nullptr If the characteristic does not exist.
 */
NimBLECharacteristic* NimBLEHIDDevice::locateReportCharacteristicByIdAndType(uint8_t reportId, uint8_t reportType) {
    NimBLECharacteristic* candidate = m_hidSvc->getCharacteristic(inputReportChrUuid, 0);
    for (uint16_t i = 1; (candidate != nullptr) && (i != 0); i++) {
        NimBLEDescriptor* dsc           = candidate->getDescriptorByUUID(featureReportDscUuid);
        NimBLEAttValue    desc1_val_att = dsc->getValue();
        const uint8_t*    desc1_val     = desc1_val_att.data();
        if ((desc1_val[0] == reportId) && (desc1_val[1] == reportType)) return candidate;
        candidate = m_hidSvc->getCharacteristic(inputReportChrUuid, i);
    }
    return nullptr;
}

/**
 * @brief Get the input report characteristic.
 * @param [in] reportId Input report ID, the same as in report map for input object related to the characteristic.
 * @return NimBLECharacteristic* A pointer to the input report characteristic.
 *                               Store this value to avoid computational overhead.
 * @details This will create the characteristic if not already created.
 */
NimBLECharacteristic* NimBLEHIDDevice::getInputReport(uint8_t reportId) {
    NimBLECharacteristic* inputReportChr = locateReportCharacteristicByIdAndType(reportId, 0x01);
    if (inputReportChr == nullptr) {
        inputReportChr =
            m_hidSvc->createCharacteristic(inputReportChrUuid,
                                           NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY | NIMBLE_PROPERTY::READ_ENC);
        NimBLEDescriptor* inputReportDsc =
            inputReportChr->createDescriptor(featureReportDscUuid, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::READ_ENC);

        uint8_t desc1_val[] = {reportId, 0x01};
        inputReportDsc->setValue(desc1_val, 2);
    }

    return inputReportChr;
} // getInputReport

/**
 * @brief Get the output report characteristic.
 * @param [in] reportId Output report ID, the same as in report map for output object related to the characteristic.
 * @return NimBLECharacteristic* A pointer to the output report characteristic.
 *                               Store this value to avoid computational overhead.
 * @details This will create the characteristic if not already created.
 */
NimBLECharacteristic* NimBLEHIDDevice::getOutputReport(uint8_t reportId) {
    NimBLECharacteristic* outputReportChr = locateReportCharacteristicByIdAndType(reportId, 0x02);
    if (outputReportChr == nullptr) {
        outputReportChr =
            m_hidSvc->createCharacteristic(inputReportChrUuid,
                                           NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::WRITE_NR |
                                               NIMBLE_PROPERTY::READ_ENC | NIMBLE_PROPERTY::WRITE_ENC);
        NimBLEDescriptor* outputReportDsc = outputReportChr->createDescriptor(
            featureReportDscUuid,
            NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::READ_ENC | NIMBLE_PROPERTY::WRITE_ENC);
        uint8_t desc1_val[] = {reportId, 0x02};
        outputReportDsc->setValue(desc1_val, 2);
    }

    return outputReportChr;
} // getOutputReport

/**
 * @brief Get the feature report characteristic.
 * @param [in] reportId Feature report ID, the same as in report map for feature object related to the characteristic.
 * @return NimBLECharacteristic* A pointer to feature report characteristic.
 *                               Store this value to avoid computational overhead.
 * @details This will create the characteristic if not already created.
 */
NimBLECharacteristic* NimBLEHIDDevice::getFeatureReport(uint8_t reportId) {
    NimBLECharacteristic* featureReportChr = locateReportCharacteristicByIdAndType(reportId, 0x03);
    if (featureReportChr == nullptr) {
        featureReportChr = m_hidSvc->createCharacteristic(
            inputReportChrUuid,
            NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::READ_ENC | NIMBLE_PROPERTY::WRITE_ENC);
        NimBLEDescriptor* featureReportDsc = featureReportChr->createDescriptor(
            featureReportDscUuid,
            NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::READ_ENC | NIMBLE_PROPERTY::WRITE_ENC);

        uint8_t desc1_val[] = {reportId, 0x03};
        featureReportDsc->setValue(desc1_val, 2);
    }

    return featureReportChr;
} // getFeatureReport

/**
 * @brief Get a keyboard boot input report characteristic.
 * @returns A pointer to the boot input report characteristic, or nullptr on error.
 * @details This will create the characteristic if not already created.
 */
NimBLECharacteristic* NimBLEHIDDevice::getBootInput() {
    NimBLECharacteristic* bootInputChr = m_hidSvc->getCharacteristic(bootInputChrUuid);
    if (bootInputChr) {
        return bootInputChr;
    }

    return m_hidSvc->createCharacteristic(bootInputChrUuid, NIMBLE_PROPERTY::NOTIFY);
} // getBootInput

/**
 * @brief Create a keyboard boot output report characteristic
 * @returns A pointer to the boot output report characteristic, or nullptr on error.
 * @details This will create the characteristic if not already created.
 */
NimBLECharacteristic* NimBLEHIDDevice::getBootOutput() {
    NimBLECharacteristic* bootOutputChr = m_hidSvc->getCharacteristic(bootOutputChrUuid);
    if (bootOutputChr) {
        return bootOutputChr;
    }

    return m_hidSvc->createCharacteristic(bootOutputChrUuid,
                                          NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::WRITE_NR);
} // getBootOutput

/**
 * @brief Get the HID control point characteristic.
 * @returns A pointer to the HID control point characteristic.
 */
NimBLECharacteristic* NimBLEHIDDevice::getHidControl() {
    return m_hidControlChr;
} // getHidControl

/**
 * @brief Get the HID protocol mode characteristic.
 * @returns a pointer to the protocol mode characteristic.
 */
NimBLECharacteristic* NimBLEHIDDevice::getProtocolMode() {
    return m_protocolModeChr;
} // getProtocolMode

/**
 * @brief Get the battery level characteristic
 * @returns A pointer to the battery level characteristic
 */
NimBLECharacteristic* NimBLEHIDDevice::getBatteryLevel() {
    return m_batteryLevelChr;
} // getBatteryLevel

/**
 * @brief Get the report map characteristic.
 * @returns A pointer to the report map characteristic.
 */
NimBLECharacteristic* NimBLEHIDDevice::getReportMap() {
    return m_reportMapChr;
} // getReportMap

/**
 * @brief Get the PnP characteristic.
 * @returns A pointer to the PnP characteristic.
 */
NimBLECharacteristic* NimBLEHIDDevice::getPnp() {
    return m_pnpChr;
} // getPnp

/**
 * @brief Get the HID information characteristic.
 * @returns A pointer to the HID information characteristic.
 */
NimBLECharacteristic* NimBLEHIDDevice::getHidInfo() {
    return m_hidInfoChr;
} // hidInfo

/**
 * @brief Get the manufacturer characteristic.
 * @returns A pointer to the manufacturer characteristic.
 */
NimBLEService* NimBLEHIDDevice::getDeviceInfoService() {
    return m_deviceInfoSvc;
} // getDeviceInfoService

/**
 * @brief Get the HID service.
 * @returns A pointer to the HID service.
 */
NimBLEService* NimBLEHIDDevice::getHidService() {
    return m_hidSvc;
} // getHidService

/**
 * @brief Get the battery service.
 * @returns A pointer to the battery service.
 */
NimBLEService* NimBLEHIDDevice::getBatteryService() {
    return m_batterySvc;
} // getBatteryService

#endif // CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_PERIPHERAL
