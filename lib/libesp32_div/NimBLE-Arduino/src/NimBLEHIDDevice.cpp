/*
 * NimBLEHIDDevice.cpp
 *
 *  Created: on Oct 06 2020
 *      Author wakwak-koba
 *
 * Originally:
 *
 * BLEHIDDevice.cpp
 *
 *  Created on: Jan 03, 2018
 *      Author: chegewara
 */

#include "nimconfig.h"
#if defined(CONFIG_BT_ENABLED) && defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)

#include "NimBLEHIDDevice.h"
#include "NimBLE2904.h"

/**
 * @brief Construct a default NimBLEHIDDevice object.
 * @param [in] server A pointer to the server instance this HID Device will use.
 */
NimBLEHIDDevice::NimBLEHIDDevice(NimBLEServer* server) {
	/*
	 * Here we create mandatory services described in bluetooth specification
	 */
	m_deviceInfoService = server->createService(NimBLEUUID((uint16_t) 0x180a));
	m_hidService = server->createService(NimBLEUUID((uint16_t) 0x1812));
	m_batteryService = server->createService(NimBLEUUID((uint16_t) 0x180f));

	/*
	 * Mandatory characteristic for device info service
	 */
	m_pnpCharacteristic = m_deviceInfoService->createCharacteristic((uint16_t) 0x2a50, NIMBLE_PROPERTY::READ);

	/*
	 * Mandatory characteristics for HID service
	 */
	m_hidInfoCharacteristic = m_hidService->createCharacteristic((uint16_t) 0x2a4a, NIMBLE_PROPERTY::READ);
	m_reportMapCharacteristic = m_hidService->createCharacteristic((uint16_t) 0x2a4b, NIMBLE_PROPERTY::READ);
	m_hidControlCharacteristic = m_hidService->createCharacteristic((uint16_t) 0x2a4c, NIMBLE_PROPERTY::WRITE_NR);
	m_protocolModeCharacteristic = m_hidService->createCharacteristic((uint16_t) 0x2a4e, NIMBLE_PROPERTY::WRITE_NR | NIMBLE_PROPERTY::READ);

	/*
	 * Mandatory battery level characteristic with notification and presence descriptor
	 */
	m_batteryLevelCharacteristic = m_batteryService->createCharacteristic((uint16_t) 0x2a19, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);
	NimBLE2904* batteryLevelDescriptor = (NimBLE2904*)m_batteryLevelCharacteristic->createDescriptor((uint16_t) 0x2904);
	batteryLevelDescriptor->setFormat(NimBLE2904::FORMAT_UINT8);
	batteryLevelDescriptor->setNamespace(1);
	batteryLevelDescriptor->setUnit(0x27ad);

	/*
	 * This value is setup here because its default value in most usage cases, its very rare to use boot mode
	 * and we want to simplify library using as much as possible
	 */
	const uint8_t pMode[] = { 0x01 };
	protocolMode()->setValue((uint8_t*) pMode, 1);
}

NimBLEHIDDevice::~NimBLEHIDDevice() {
}

/**
 * @brief Set the report map data formatting information.
 * @param [in] map A pointer to an array with the values to set.
 * @param [in] size The number of values in the array.
 */
void NimBLEHIDDevice::reportMap(uint8_t* map, uint16_t size) {
	m_reportMapCharacteristic->setValue(map, size);
}

/**
 * @brief Start the HID device services.\n
 * This function called when all the services have been created.
 */
void NimBLEHIDDevice::startServices() {
	m_deviceInfoService->start();
	m_hidService->start();
	m_batteryService->start();
}

/**
 * @brief Create a manufacturer characteristic (this characteristic is optional).
 */
NimBLECharacteristic* NimBLEHIDDevice::manufacturer() {
	m_manufacturerCharacteristic = m_deviceInfoService->createCharacteristic((uint16_t) 0x2a29, NIMBLE_PROPERTY::READ);
	return m_manufacturerCharacteristic;
}

/**
 * @brief Set manufacturer name
 * @param [in] name The manufacturer name of this HID device.
 */
void NimBLEHIDDevice::manufacturer(std::string name) {
	m_manufacturerCharacteristic->setValue(name);
}

/**
 * @brief Sets the Plug n Play characterisc value.
 * @param [in] sig The vendor ID source number.
 * @param [in] vid The vendor ID number.
 * @param [in] pid The product ID number.
 * @param [in] version The produce version number.
 */
void NimBLEHIDDevice::pnp(uint8_t sig, uint16_t vid, uint16_t pid, uint16_t version) {
	uint8_t pnp[] = { sig, (uint8_t) (vid >> 8), (uint8_t) vid, (uint8_t) (pid >> 8), (uint8_t) pid, (uint8_t) (version >> 8), (uint8_t) version };
	m_pnpCharacteristic->setValue(pnp, sizeof(pnp));
}

/**
 * @brief Sets the HID Information characteristic value.
 * @param [in] country The country code for the device.
 * @param [in] flags The HID Class Specification release number to use.
 */
void NimBLEHIDDevice::hidInfo(uint8_t country, uint8_t flags) {
	uint8_t info[] = { 0x11, 0x1, country, flags };
	m_hidInfoCharacteristic->setValue(info, sizeof(info));
}

/**
 * @brief Create input report characteristic
 * @param [in] reportID input report ID, the same as in report map for input object related to the characteristic
 * @return pointer to new input report characteristic
 */
NimBLECharacteristic* NimBLEHIDDevice::inputReport(uint8_t reportID) {
	NimBLECharacteristic* inputReportCharacteristic = m_hidService->createCharacteristic((uint16_t) 0x2a4d, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY | NIMBLE_PROPERTY::READ_ENC);
	NimBLEDescriptor* inputReportDescriptor = inputReportCharacteristic->createDescriptor((uint16_t) 0x2908, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::READ_ENC);

	uint8_t desc1_val[] = { reportID, 0x01 };
	inputReportDescriptor->setValue((uint8_t*) desc1_val, 2);

	return inputReportCharacteristic;
}

/**
 * @brief Create output report characteristic
 * @param [in] reportID Output report ID, the same as in report map for output object related to the characteristic
 * @return Pointer to new output report characteristic
 */
NimBLECharacteristic* NimBLEHIDDevice::outputReport(uint8_t reportID) {
	NimBLECharacteristic* outputReportCharacteristic = m_hidService->createCharacteristic((uint16_t) 0x2a4d, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::WRITE_NR | NIMBLE_PROPERTY::READ_ENC | NIMBLE_PROPERTY::WRITE_ENC);
	NimBLEDescriptor* outputReportDescriptor = outputReportCharacteristic->createDescriptor((uint16_t) 0x2908, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::READ_ENC | NIMBLE_PROPERTY::WRITE_ENC);

	uint8_t desc1_val[] = { reportID, 0x02 };
	outputReportDescriptor->setValue((uint8_t*) desc1_val, 2);

	return outputReportCharacteristic;
}

/**
 * @brief Create feature report characteristic.
 * @param [in] reportID Feature report ID, the same as in report map for feature object related to the characteristic
 * @return Pointer to new feature report characteristic
 */
NimBLECharacteristic* NimBLEHIDDevice::featureReport(uint8_t reportID) {
	NimBLECharacteristic* featureReportCharacteristic = m_hidService->createCharacteristic((uint16_t) 0x2a4d, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::READ_ENC | NIMBLE_PROPERTY::WRITE_ENC);
	NimBLEDescriptor* featureReportDescriptor = featureReportCharacteristic->createDescriptor((uint16_t) 0x2908, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::READ_ENC | NIMBLE_PROPERTY::WRITE_ENC);

	uint8_t desc1_val[] = { reportID, 0x03 };
	featureReportDescriptor->setValue((uint8_t*) desc1_val, 2);

	return featureReportCharacteristic;
}

/**
 * @brief Creates a keyboard boot input report characteristic
 */
NimBLECharacteristic* NimBLEHIDDevice::bootInput() {
	return m_hidService->createCharacteristic((uint16_t) 0x2a22, NIMBLE_PROPERTY::NOTIFY);
}

/**
 * @brief Create a keyboard boot output report characteristic
 */
NimBLECharacteristic* NimBLEHIDDevice::bootOutput() {
	return m_hidService->createCharacteristic((uint16_t) 0x2a32, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::WRITE_NR);
}

/**
 * @brief Returns a pointer to the HID control point characteristic.
 */
NimBLECharacteristic* NimBLEHIDDevice::hidControl() {
	return m_hidControlCharacteristic;
}

/**
 * @brief Returns a pointer to the protocol mode characteristic.
 */
NimBLECharacteristic* NimBLEHIDDevice::protocolMode() {
	return m_protocolModeCharacteristic;
}

/**
 * @brief Set the battery level characteristic value.
 * @param [in] level The battery level value.
 */
void NimBLEHIDDevice::setBatteryLevel(uint8_t level) {
	m_batteryLevelCharacteristic->setValue(&level, 1);
}
/*
 * @brief Returns battery level characteristic
 * @ return battery level characteristic
 *//*
BLECharacteristic* BLEHIDDevice::batteryLevel() {
	return m_batteryLevelCharacteristic;
}



BLECharacteristic*	 BLEHIDDevice::reportMap() {
	return m_reportMapCharacteristic;
}

BLECharacteristic*	 BLEHIDDevice::pnp() {
	return m_pnpCharacteristic;
}


BLECharacteristic*	BLEHIDDevice::hidInfo() {
	return m_hidInfoCharacteristic;
}
*/

/**
 * @brief Returns a pointer to the device information service.
 */
NimBLEService* NimBLEHIDDevice::deviceInfo() {
	return m_deviceInfoService;
}

/**
 * @brief Returns a pointer to the HID service.
 */
NimBLEService* NimBLEHIDDevice::hidService() {
	return m_hidService;
}

/**
 * @brief @brief Returns a pointer to the battery service.
 */
NimBLEService* NimBLEHIDDevice::batteryService() {
	return m_batteryService;
}

#endif /* CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_PERIPHERAL */
