/*
   EddystoneTLM beacon for NimBLE by BeeGee based on https://github.com/pcbreflux/espressif/blob/master/esp32/arduino/sketchbook/ESP32_Eddystone_TLM_deepsleep/ESP32_Eddystone_TLM_deepsleep.ino
   EddystoneTLM frame specification https://github.com/google/eddystone/blob/master/eddystone-tlm/tlm-plain.md
*/

/*
   Create a BLE server that will send periodic Eddystone URL frames.
   The design of creating the BLE server is:
   1. Create a BLE Server
   2. Create advertising data
   3. Start advertising.
   4. wait
   5. Stop advertising.
   6. deep sleep
   
*/

#include "NimBLEDevice.h"
#include "NimBLEBeacon.h"
#include "NimBLEAdvertising.h"
#include "NimBLEEddystoneURL.h"

#include "sys/time.h"
#include "esp_sleep.h"

#define GPIO_DEEP_SLEEP_DURATION 10     // sleep x seconds and then wake up

// UUID 1 128-Bit (may use linux tool uuidgen or random numbers via https://www.uuidgenerator.net/)
#define BEACON_UUID "8ec76ea3-6668-48da-9866-75be8bc86f4d" 

RTC_DATA_ATTR static time_t last;    // remember last boot in RTC Memory
RTC_DATA_ATTR static uint32_t bootcount; // remember number of boots in RTC Memory

BLEAdvertising *pAdvertising;
struct timeval nowTimeStruct;

time_t lastTenth;

// Check
// https://github.com/google/eddystone/blob/master/eddystone-tlm/tlm-plain.md
// and http://www.hugi.scene.org/online/coding/hugi%2015%20-%20cmtadfix.htm
// for the temperature value. It is a 8.8 fixed-point notation
void setBeacon()
{
  char beacon_data[25];
  uint16_t beconUUID = 0xFEAA;
  uint16_t volt = random(2800, 3700); // 3300mV = 3.3V
  float tempFloat = random(2000, 3100) / 100.0f;
  Serial.printf("Random temperature is %.2fC\n", tempFloat);
  int temp = (int)(tempFloat * 256); //(uint16_t)((float)23.00);
  Serial.printf("Converted to 8.8 format %0X%0X\n", (temp >> 8), (temp & 0xFF));

  BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();
  BLEAdvertisementData oScanResponseData = BLEAdvertisementData();

  oScanResponseData.setFlags(0x06); // GENERAL_DISC_MODE 0x02 | BR_EDR_NOT_SUPPORTED 0x04
  oScanResponseData.setCompleteServices(BLEUUID(beconUUID));

  beacon_data[0] = 0x20;                // Eddystone Frame Type (Unencrypted Eddystone-TLM)
  beacon_data[1] = 0x00;                // TLM version
  beacon_data[2] = (volt >> 8);           // Battery voltage, 1 mV/bit i.e. 0xCE4 = 3300mV = 3.3V
  beacon_data[3] = (volt & 0xFF);           //
  beacon_data[4] = (temp >> 8);           // Beacon temperature
  beacon_data[5] = (temp & 0xFF);           //
  beacon_data[6] = ((bootcount & 0xFF000000) >> 24);  // Advertising PDU count
  beacon_data[7] = ((bootcount & 0xFF0000) >> 16);  //
  beacon_data[8] = ((bootcount & 0xFF00) >> 8);   //
  beacon_data[9] = (bootcount & 0xFF);        //
  beacon_data[10] = ((lastTenth & 0xFF000000) >> 24); // Time since power-on or reboot as 0.1 second resolution counter
  beacon_data[11] = ((lastTenth & 0xFF0000) >> 16);   //
  beacon_data[12] = ((lastTenth & 0xFF00) >> 8);    //
  beacon_data[13] = (lastTenth & 0xFF);       //

  oScanResponseData.setServiceData(BLEUUID(beconUUID), std::string(beacon_data, 14));
  oAdvertisementData.setName("TLMBeacon");
  pAdvertising->setAdvertisementData(oAdvertisementData);
  pAdvertising->setScanResponseData(oScanResponseData);
}

void setup()
{

  Serial.begin(115200);
  gettimeofday(&nowTimeStruct, NULL);

  Serial.printf("start ESP32 %d\n", bootcount++);

  Serial.printf("deep sleep (%lds since last reset, %lds since last boot)\n", nowTimeStruct.tv_sec, nowTimeStruct.tv_sec - last);

  last = nowTimeStruct.tv_sec;
  lastTenth = nowTimeStruct.tv_sec * 10; // Time since last reset as 0.1 second resolution counter

  // Create the BLE Device
  BLEDevice::init("TLMBeacon");

  BLEDevice::setPower(ESP_PWR_LVL_N12);

  pAdvertising = BLEDevice::getAdvertising();

  setBeacon();
  // Start advertising
  pAdvertising->start();
  Serial.println("Advertizing started for 10s ...");
  delay(10000);
  pAdvertising->stop();
  Serial.printf("enter deep sleep for 10s\n");
  esp_deep_sleep(1000000LL * GPIO_DEEP_SLEEP_DURATION);
  Serial.printf("in deep sleep\n");
}

void loop()
{
}
