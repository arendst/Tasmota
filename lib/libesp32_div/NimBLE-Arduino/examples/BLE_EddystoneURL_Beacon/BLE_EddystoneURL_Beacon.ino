/*
   EddystoneURL beacon for NimBLE by BeeGee
   EddystoneURL frame specification https://github.com/google/eddystone/blob/master/eddystone-url/README.md

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
#include "NimBLEEddystoneURL.h"

#include "sys/time.h"
#include "esp_sleep.h"

#define GPIO_DEEP_SLEEP_DURATION 10     // sleep x seconds and then wake up

// UUID 1 128-Bit (may use linux tool uuidgen or random numbers via https://www.uuidgenerator.net/)
#define BEACON_UUID "8ec76ea3-6668-48da-9866-75be8bc86f4d" 

RTC_DATA_ATTR static time_t last;    // remember last boot in RTC Memory
RTC_DATA_ATTR static uint32_t bootcount; // remember number of boots in RTC Memory

BLEAdvertising *pAdvertising;
struct timeval now;

static const char *eddystone_url_prefix_subs[] = {
  "http://www.",
  "https://www.",
  "http://",
  "https://",
  "urn:uuid:",
  NULL
};

static const char *eddystone_url_suffix_subs[] = {
  ".com/",
  ".org/",
  ".edu/",
  ".net/",
  ".info/",
  ".biz/",
  ".gov/",
  ".com",
  ".org",
  ".edu",
  ".net",
  ".info",
  ".biz",
  ".gov",
  NULL
};

static int string_begin_with(const char *str, const char *prefix)
{
  int prefix_len = strlen(prefix);
  if (strncmp(prefix, str, prefix_len) == 0)
  {
    return prefix_len;
  }
  return 0;
}

void setBeacon()
{
  BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();
  BLEAdvertisementData oScanResponseData = BLEAdvertisementData();

  const char url[] = "https://d.giesecke.tk";

  int scheme_len, ext_len = 1, i, idx, url_idx;
  char *ret_data;
  int url_len = strlen(url);

  ret_data = (char *)calloc(1, url_len + 13);

  ret_data[0] = 2;   // Len
  ret_data[1] = 0x01;  // Type Flags
  ret_data[2] = 0x06;  // GENERAL_DISC_MODE 0x02 | BR_EDR_NOT_SUPPORTED 0x04
  ret_data[3] = 3;   // Len
  ret_data[4] = 0x03;  // Type 16-Bit UUID
  ret_data[5] = 0xAA;  // Eddystone UUID 2 -> 0xFEAA LSB
  ret_data[6] = 0xFE;  // Eddystone UUID 1 MSB
  ret_data[7] = 19; // Length of Beacon Data
  ret_data[8] = 0x16;  // Type Service Data
  ret_data[9] = 0xAA;  // Eddystone UUID 2 -> 0xFEAA LSB
  ret_data[10] = 0xFE; // Eddystone UUID 1 MSB
  ret_data[11] = 0x10; // Eddystone Frame Type
  ret_data[12] = 0xF4; // Beacons TX power at 0m

  i = 0, idx = 13, url_idx = 0;

  //replace prefix
  scheme_len = 0;
  while (eddystone_url_prefix_subs[i] != NULL)
  {
    if ((scheme_len = string_begin_with(url, eddystone_url_prefix_subs[i])) > 0)
    {
      ret_data[idx] = i;
      idx++;
      url_idx += scheme_len;
      break;
    }
    i++;
  }
  while (url_idx < url_len)
  {
    i = 0;
    ret_data[idx] = url[url_idx];
    ext_len = 1;
    while (eddystone_url_suffix_subs[i] != NULL)
    {
      if ((ext_len = string_begin_with(&url[url_idx], eddystone_url_suffix_subs[i])) > 0)
      {
        ret_data[idx] = i;
        break;
      }
      else
      {
        ext_len = 1; //inc 1
      }
      i++;
    }
    url_idx += ext_len;
    idx++;
  }
  ret_data[7] = idx - 8;

  Serial.printf("struct size %d url size %d reported len %d\n",
                url_len + 13,
                url_len, ret_data[7]);

  Serial.printf("URL in data %s\n", &ret_data[13]);

  std::string eddyStoneData(ret_data);

  oAdvertisementData.addData(eddyStoneData);
  oScanResponseData.setName("MeBeacon");
  pAdvertising->setAdvertisementData(oAdvertisementData);
  pAdvertising->setScanResponseData(oScanResponseData);
}

void setup()
{

  Serial.begin(115200);
  gettimeofday(&now, NULL);

  Serial.printf("start ESP32 %d\n", bootcount++);

  Serial.printf("deep sleep (%lds since last reset, %lds since last boot)\n", now.tv_sec, now.tv_sec - last);

  last = now.tv_sec;

  // Create the BLE Device
  BLEDevice::init("MeBeacon");

  BLEDevice::setPower(ESP_PWR_LVL_N12);

  pAdvertising = BLEDevice::getAdvertising();

  setBeacon();
  // Start advertising
  pAdvertising->start();
  Serial.println("Advertizing started...");
  delay(10000);
  pAdvertising->stop();
  Serial.printf("enter deep sleep\n");
  esp_deep_sleep(1000000LL * GPIO_DEEP_SLEEP_DURATION);
  Serial.printf("in deep sleep\n");
}

void loop()
{
}
