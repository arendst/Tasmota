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
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "nimconfig.h"
#if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)

#include "NimBLEHIDDevice.h"
#include "NimBLE2904.h"

NimBLEHIDDevice::NimBLEHIDDevice(NimBLEServer* server) {
	/*
	 * Here we create mandatory services described in bluetooth specification
	 */
	m_deviceInfoService = server->createService(NimBLEUUID((uint16_t) 0x180a));
	m_hidService = server->createService(NimBLEUUID((uint16_t) 0x1812), 40);
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

/*
 * @brief
 */
void NimBLEHIDDevice::reportMap(uint8_t* map, uint16_t size) {
	m_reportMapCharacteristic->setValue(map, size);
}

/*
 * @brief This function suppose to be called at the end, when we have created all characteristics we need to build HID service
 */
void NimBLEHIDDevice::startServices() {
	m_deviceInfoService->start();
	m_hidService->start();
	m_batteryService->start();
}

/*
 * @brief Create manufacturer characteristic (this characteristic is optional)
 */
NimBLECharacteristic* NimBLEHIDDevice::manufacturer() {
	m_manufacturerCharacteristic = m_deviceInfoService->createCharacteristic((uint16_t) 0x2a29, NIMBLE_PROPERTY::READ);
	return m_manufacturerCharacteristic;
}

/*
 * @brief Set manufacturer name
 * @param [in] name manufacturer name
 */
void NimBLEHIDDevice::manufacturer(std::string name) {
	m_manufacturerCharacteristic->setValue(name);
}

/*
 * @brief
 */
void NimBLEHIDDevice::pnp(uint8_t sig, uint16_t vid, uint16_t pid, uint16_t version) {
	uint8_t pnp[] = { sig, (uint8_t) (vid >> 8), (uint8_t) vid, (uint8_t) (pid >> 8), (uint8_t) pid, (uint8_t) (version >> 8), (uint8_t) version };
	m_pnpCharacteristic->setValue(pnp, sizeof(pnp));
}

/*
 * @brief
 */
void NimBLEHIDDevice::hidInfo(uint8_t country, uint8_t flags) {
	uint8_t info[] = { 0x11, 0x1, country, flags };
	m_hidInfoCharacteristic->setValue(info, sizeof(info));
}

/*
 * @brief Create input report characteristic that need to be saved as new characteristic object so can be further used
 * @param [in] reportID input report ID, the same as in report map for input object related to created characteristic
 * @return pointer to new input report characteristic
 */
NimBLECharacteristic* NimBLEHIDDevice::inputReport(uint8_t reportID) {
	NimBLECharacteristic* inputReportCharacteristic = m_hidService->createCharacteristic((uint16_t) 0x2a4d, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);
	NimBLEDescriptor* inputReportDescriptor = inputReportCharacteristic->createDescriptor((uint16_t) 0x2908);

	uint8_t desc1_val[] = { reportID, 0x01 };
	inputReportDescriptor->setValue((uint8_t*) desc1_val, 2);

	return inputReportCharacteristic;
}

/*
 * @brief Create output report characteristic that need to be saved as new characteristic object so can be further used
 * @param [in] reportID Output report ID, the same as in report map for output object related to created characteristic
 * @return Pointer to new output report characteristic
 */
NimBLECharacteristic* NimBLEHIDDevice::outputReport(uint8_t reportID) {
	NimBLECharacteristic* outputReportCharacteristic = m_hidService->createCharacteristic((uint16_t) 0x2a4d, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::WRITE_NR | NIMBLE_PROPERTY::READ_ENC | NIMBLE_PROPERTY::WRITE_ENC);
	NimBLEDescriptor* outputReportDescriptor = outputReportCharacteristic->createDescriptor((uint16_t) 0x2908);

	uint8_t desc1_val[] = { reportID, 0x02 };
	outputReportDescriptor->setValue((uint8_t*) desc1_val, 2);

	return outputReportCharacteristic;
}

/*
 * @brief Create feature report characteristic that need to be saved as new characteristic object so can be further used
 * @param [in] reportID Feature report ID, the same as in report map for feature object related to created characteristic
 * @return Pointer to new feature report characteristic
 */
NimBLECharacteristic* NimBLEHIDDevice::featureReport(uint8_t reportID) {
	NimBLECharacteristic* featureReportCharacteristic = m_hidService->createCharacteristic((uint16_t) 0x2a4d, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::READ_ENC | NIMBLE_PROPERTY::WRITE_ENC);
	NimBLEDescriptor* featureReportDescriptor = featureReportCharacteristic->createDescriptor((uint16_t) 0x2908);

	uint8_t desc1_val[] = { reportID, 0x03 };
	featureReportDescriptor->setValue((uint8_t*) desc1_val, 2);

	return featureReportCharacteristic;
}

/*
 * @brief
 */
NimBLECharacteristic* NimBLEHIDDevice::bootInput() {
	return m_hidService->createCharacteristic((uint16_t) 0x2a22, NIMBLE_PROPERTY::NOTIFY);
}

/*
 * @brief
 */
NimBLECharacteristic* NimBLEHIDDevice::bootOutput() {
	return m_hidService->createCharacteristic((uint16_t) 0x2a32, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::WRITE_NR);
}

/*
 * @brief
 */
NimBLECharacteristic* NimBLEHIDDevice::hidControl() {
	return m_hidControlCharacteristic;
}

/*
 * @brief
 */
NimBLECharacteristic* NimBLEHIDDevice::protocolMode() {
	return m_protocolModeCharacteristic;
}

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
/*
 * @brief
 */
NimBLEService* NimBLEHIDDevice::deviceInfo() {
	return m_deviceInfoService;
}

/*
 * @brief
 */
NimBLEService* NimBLEHIDDevice::hidService() {
	return m_hidService;
}

/*
 * @brief
 */
NimBLEService* NimBLEHIDDevice::batteryService() {
	return m_batteryService;
}

#endif // #if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)
#endif // #if defined(CONFIG_BT_ENABLED)
