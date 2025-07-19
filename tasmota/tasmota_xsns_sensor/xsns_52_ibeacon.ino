/*
  xsns_52_ibeacon.ino - Support for HM17 BLE Module + ibeacon reader on Tasmota

  Copyright (C) 2021  Gerhard Mutz and Theo Arends

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

// for testing of BLE_ESP32, we remove this completely, and instead add the modified xsns_52_ibeacon_BLE_ESP32.ino
// in the future this may be more fine-grained, e.g. to allow hm17 for this, and BLE-ESP32 for other
#ifndef USE_BLE_ESP32

#ifdef USE_IBEACON

#define XSNS_52                       52

// keyfob expires after N seconds
#define IB_TIMEOUT_INTERVAL 30
// does a passive scan every N seconds
#define IB_UPDATE_TIME_INTERVAL 10

// should be in Settings
//#if 1
uint8_t ib_upd_interval,ib_tout_interval;
//#undef IB_UPDATE_TIME
//#undef IB_TIMEOUT_TIME
#define IB_UPDATE_TIME ib_upd_interval
#define IB_TIMEOUT_TIME ib_tout_interval
//#else
//#undef IB_UPDATE_TIME
//#undef IB_TIMEOUT_TIME
//#define IB_UPDATE_TIME Settings->ib_upd_interval
//#define IB_TIMEOUT_TIME Settings->ib_tout_interval
//#endif

char ib_mac[14];

#ifdef USE_IBEACON_ESP32

struct {
  union {
    struct {
      uint32_t init:1;
      uint32_t runningScan:1;
    };
    uint32_t all = 0;
  } mode;
} ESP32BLE;

#include <NimBLEDevice.h>
#include <NimBLEAdvertisedDevice.h>
#include "NimBLEEddystoneURL.h"
#include "NimBLEEddystoneTLM.h"
#include "NimBLEBeacon.h"

#define ENDIAN_CHANGE_U16(x) ((((x)&0xFF00) >> 8) + (((x)&0xFF) << 8))

BLEScan *ESP32BLEScan;

#else

#include <TasmotaSerial.h>

#define TMSBSIZ52 512

#define HM17_BAUDRATE 9600

#define IBEACON_DEBUG

// use this for Version 110
#define HM17_V110

TasmotaSerial *IBEACON_Serial = nullptr;

uint8_t hm17_found,hm17_cmd,hm17_flag;

#ifdef IBEACON_DEBUG
uint8_t hm17_debug=0;
#endif

// 78 is max serial response
#define HM17_BSIZ 128
char hm17_sbuffer[HM17_BSIZ];
uint8_t hm17_sindex,hm17_result,hm17_scanning,hm17_connecting;
uint32_t hm17_lastms;

enum {HM17_TEST,HM17_ROLE,HM17_IMME,HM17_DISI,HM17_IBEA,HM17_SCAN,HM17_DISC,HM17_RESET,HM17_RENEW,HM17_CON};
#define HM17_SUCESS 99

#endif

struct IBEACON {
  char FACID[8];
  char UID[32];
  char MAJOR[4];
  char MINOR[4];
  char PWR[2];
  char MAC[12];
  char RSSI[4];
#ifdef USE_IBEACON_ESP32
  char NAME[16];
#endif
};

#ifdef USE_IBEACON_ESP32
  #define MAX_IBEACONS 32
#else
  #define MAX_IBEACONS 16
#endif

struct IBEACON_UID {
  char MAC[12];
  char RSSI[4];
  char UID[32];
  char MAJOR[4];
  char MINOR[4];
  uint8_t FLAGS;
  uint8_t TIME;
#ifdef USE_IBEACON_ESP32
  uint8_t REPORTED;
  uint8_t REPTIME;
  char NAME[16];
#endif
} ibeacons[MAX_IBEACONS];

#ifdef USE_IBEACON_ESP32

uint32_t ibeacon_add(struct IBEACON *ib);

void ESP32BLE_ReverseStr(uint8_t _mac[], uint8_t len=6){
  uint8_t _reversedMAC[len];
  for (uint8_t i=0; i<len; i++){
    _reversedMAC[len-1-i] = _mac[i];
  }
  memcpy(_mac,_reversedMAC, sizeof(_reversedMAC));
}

void DumpHex(const unsigned char * in, size_t insz, char * out)
{
  static const char * hex = "0123456789ABCDEF";
  const unsigned char * pin = in;
  char * pout = out;
  for (; pin < in+insz; pout+=2, pin++) {
    pout[0] = hex[(pgm_read_byte(pin)>>4) & 0xF];
    pout[1] = hex[ pgm_read_byte(pin)     & 0xF];
  }
}


class ESP32BLEScanCallback : public BLEAdvertisedDeviceCallbacks
{
    void onResult(BLEAdvertisedDevice *advertisedDevice)
    {
      struct IBEACON ib;
      ESP32BLEScan->erase(advertisedDevice->getAddress());
      if (advertisedDevice->haveManufacturerData() == true) {
        std::string strManufacturerData = advertisedDevice->getManufacturerData();

        uint8_t cManufacturerData[100];
        strManufacturerData.copy((char *)cManufacturerData, strManufacturerData.length(), 0);

        int16_t     RSSI  = advertisedDevice->getRSSI();
        char sRSSI[6];
        itoa(RSSI,sRSSI,10);

        DumpHex(cManufacturerData,2,ib.FACID);

        uint8_t MAC[6];
        memcpy(MAC,advertisedDevice->getAddress().getNative(),6);
        ESP32BLE_ReverseStr(MAC,6);

        if (strManufacturerData.length() == 25 && cManufacturerData[0] == 0x4C && cManufacturerData[1] == 0x00)
        {
          BLEBeacon oBeacon = BLEBeacon();
          oBeacon.setData(strManufacturerData);

          uint8_t UUID[16];
	  memcpy(UUID,oBeacon.getProximityUUID().getNative()->u128.value,16);
          ESP32BLE_ReverseStr(UUID,16);

	  uint16_t    Major = ENDIAN_CHANGE_U16(oBeacon.getMajor());
          uint16_t    Minor = ENDIAN_CHANGE_U16(oBeacon.getMinor());

          uint8_t     PWR   = oBeacon.getSignalPower();

          AddLog(LOG_LEVEL_DEBUG, PSTR("%s: MAC: %s Major: %d Minor: %d UUID: %s Power: %d RSSI: %d"),
            "BLE",
            advertisedDevice->getAddress().toString().c_str(),
            Major, Minor,
            oBeacon.getProximityUUID().toString().c_str(),
            PWR, RSSI);

          DumpHex((const unsigned char*)&UUID,16,ib.UID);
          DumpHex((const unsigned char*)&Major,2,ib.MAJOR);
          DumpHex((const unsigned char*)&Minor,2,ib.MINOR);
          DumpHex((const unsigned char*)&PWR,1,ib.PWR);
          DumpHex((const unsigned char*)&MAC,6,ib.MAC);
          memcpy(ib.RSSI,sRSSI,4);
          memset(ib.NAME,0x0,16);

          ibeacon_add(&ib);

        } else {

          memset(ib.UID,'0',32);
          memset(ib.MAJOR,'0',4);
          memset(ib.MINOR,'0',4);
          memset(ib.PWR,'0',2);
          DumpHex((const unsigned char*)&MAC,6,ib.MAC);
          memcpy(ib.RSSI,sRSSI,4);

          if (advertisedDevice->haveName()) {
            strncpy(ib.NAME,advertisedDevice->getName().c_str(),16);
          } else {
            memset(ib.NAME,0x0,16);
          }

          ibeacon_add(&ib);
        }
      }
    }
};

void ESP32StartScanTask(){
    ESP32BLE.mode.runningScan = 1;
    xTaskCreatePinnedToCore(
    ESP32ScanTask,    /* Function to implement the task */
    "ESP32ScanTask",  /* Name of the task */
    2048,             /* Stack size in words */
    NULL,             /* Task input parameter */
    0,                /* Priority of the task */
    NULL,             /* Task handle. */
    0);               /* Core where the task should run */
    AddLog(LOG_LEVEL_DEBUG,PSTR("%s: Start scanning"),"BLE");
}

void ESP32scanEndedCB(NimBLEScanResults results);

void ESP32ScanTask(void *pvParameters){
  if (ESP32BLEScan == nullptr) ESP32BLEScan = NimBLEDevice::getScan();
  ESP32BLEScan->setInterval(70);
  ESP32BLEScan->setWindow(50);
  ESP32BLEScan->setAdvertisedDeviceCallbacks(new ESP32BLEScanCallback());
  ESP32BLEScan->setActiveScan(false);
  ESP32BLEScan->setDuplicateFilter(false);

  ESP32BLEScan->start(0, ESP32scanEndedCB, false);

  for (;;) {
    vTaskDelay(10000/ portTICK_PERIOD_MS);
    ESP32BLEScan->clearResults();
    AddLog(LOG_LEVEL_DEBUG,PSTR("%s: Clear scanning results"),"BLE");
  }

}

void ESP32scanEndedCB(NimBLEScanResults results) {
  ESP32BLE.mode.runningScan = 0;
  AddLog(LOG_LEVEL_DEBUG,PSTR("%s: Stop scanning"),"BLE");
}

void ESP32StopScanTask() {
  ESP32BLEScan->stop();
  AddLog(LOG_LEVEL_DEBUG, PSTR("%s: Pausing scanner task"),"BLE");
}

void ESP32ResumeScanTask() {
  ESP32BLE.mode.runningScan = 1;
  ESP32BLEScan->start(0, ESP32scanEndedCB, false);
  AddLog(LOG_LEVEL_DEBUG, PSTR("%s: Resumed scanner task"),"BLE");
}

void ESP32Init() {

  if (TasmotaGlobal.global_state.wifi_down) { return; }

  TasmotaGlobal.wifi_stay_asleep = true;
  if (WiFi.getSleep() == false) {
    AddLog(LOG_LEVEL_DEBUG,PSTR("%s: Put WiFi modem in sleep mode"),"BLE");
    WiFi.setSleep(true); // Sleep
  }

  AddLog(LOG_LEVEL_DEBUG,PSTR("%s: Initializing Bluetooth..."),"BLE");

  if (!ESP32BLE.mode.init) {
    NimBLEDevice::init("");

    ESP32BLE.mode.init = 1;

    ESP32StartScanTask(); // Let's get started !!

    IB_UPDATE_TIME=IB_UPDATE_TIME_INTERVAL;
    IB_TIMEOUT_TIME=IB_TIMEOUT_INTERVAL;
  }

}

#endif

void IBEACON_Init() {

#ifdef USE_IBEACON_ESP32

  ESP32BLE.mode.init = false;
  ESP32BLE.mode.runningScan = false;

#else

  hm17_found=0;

// actually doesnt work reliably with software serial
  if (PinUsed(GPIO_IBEACON_RX) && PinUsed(GPIO_IBEACON_TX)) {
    IBEACON_Serial = new TasmotaSerial(Pin(GPIO_IBEACON_RX), Pin(GPIO_IBEACON_TX),1,0,TMSBSIZ52);
    if (IBEACON_Serial->begin(HM17_BAUDRATE)) {
      if (IBEACON_Serial->hardwareSerial()) {
        ClaimSerial();
      }
#ifdef ESP32
      AddLog(LOG_LEVEL_DEBUG, PSTR("BLE: Serial UART%d"), IBEACON_Serial->getUart());
#endif
      hm17_sendcmd(HM17_TEST);
      hm17_lastms=millis();
      // in case of using Settings this has to be moved
      IB_UPDATE_TIME=IB_UPDATE_TIME_INTERVAL;
      IB_TIMEOUT_TIME=IB_TIMEOUT_INTERVAL;
    }
  }

#endif

}

#ifdef USE_IBEACON_ESP32

void esp32_every_second(void) {

  if (!ESP32BLE.mode.init) { return; }

  if (TasmotaGlobal.ota_state_flag) {
    if (ESP32BLE.mode.runningScan) {
      AddLog(LOG_LEVEL_DEBUG,PSTR("%s: Upgrade procedure started"),"BLE");
      ESP32StopScanTask();
    }
  } else {
    if (!ESP32BLE.mode.runningScan) {
      AddLog(LOG_LEVEL_DEBUG,PSTR("%s: Resuming scan"),"BLE");
      ESP32ResumeScanTask();
    }
  }

  for (uint32_t cnt=0;cnt<MAX_IBEACONS;cnt++) {
    if (ibeacons[cnt].FLAGS) {
      ibeacons[cnt].TIME++;
      ibeacons[cnt].REPTIME++;
      if (ibeacons[cnt].TIME>IB_TIMEOUT_TIME) {
        ibeacons[cnt].FLAGS=0;
        ibeacon_mqtt(ibeacons[cnt].MAC,"0000",ibeacons[cnt].UID,ibeacons[cnt].MAJOR,ibeacons[cnt].MINOR,ibeacons[cnt].NAME);
      }
    }
  }
}

#else

void hm17_every_second(void) {
  if (!IBEACON_Serial) return;

  if (hm17_found) {
    if (IB_UPDATE_TIME && (TasmotaGlobal.uptime%IB_UPDATE_TIME==0)) {
      if (hm17_cmd!=99) {
        if (hm17_flag&2) {
          ib_sendbeep();
        } else {
          if (!hm17_connecting) {
            hm17_sendcmd(HM17_DISI);
          }
        }
      }
    }
    for (uint32_t cnt=0;cnt<MAX_IBEACONS;cnt++) {
      if (ibeacons[cnt].FLAGS) {
        ibeacons[cnt].TIME++;
        if (ibeacons[cnt].TIME>IB_TIMEOUT_TIME) {
          ibeacons[cnt].FLAGS=0;
          ibeacon_mqtt(ibeacons[cnt].MAC,"0000",ibeacons[cnt].UID,ibeacons[cnt].MAJOR,ibeacons[cnt].MINOR);
        }
      }
    }
  } else {
    if (TasmotaGlobal.uptime%20==0) {
      hm17_sendcmd(HM17_TEST);
    }
  }
}

void hm17_sbclr(void) {
  memset(hm17_sbuffer,0,HM17_BSIZ);
  hm17_sindex=0;
  //IBEACON_Serial->flush();
}

void hm17_sendcmd(uint8_t cmd) {
  hm17_sbclr();
  hm17_cmd=cmd;
#ifdef IBEACON_DEBUG
  if (hm17_debug) AddLog(LOG_LEVEL_INFO, PSTR("hm17cmd %d"),cmd);
#endif
  switch (cmd) {
    case HM17_TEST:
      IBEACON_Serial->write("AT");
      break;
    case HM17_ROLE:
      IBEACON_Serial->write("AT+ROLE1");
      break;
    case HM17_IMME:
      IBEACON_Serial->write("AT+IMME1");
      break;
    case HM17_DISI:
      IBEACON_Serial->write("AT+DISI?");
      hm17_scanning=1;
      break;
    case HM17_IBEA:
      IBEACON_Serial->write("AT+IBEA1");
      break;
    case HM17_RESET:
      IBEACON_Serial->write("AT+RESET");
      break;
    case HM17_RENEW:
      IBEACON_Serial->write("AT+RENEW");
      break;
    case HM17_SCAN:
      IBEACON_Serial->write("AT+SCAN5");
      break;
    case HM17_DISC:
      IBEACON_Serial->write("AT+DISC?");
      hm17_scanning=1;
      break;
    case HM17_CON:
      IBEACON_Serial->write((const uint8_t*)"AT+CON",6);
      IBEACON_Serial->write((const uint8_t*)ib_mac,12);
      hm17_connecting=1;
      break;
  }
}

#endif

uint32_t ibeacon_add(struct IBEACON *ib) {
/*  if (!strncmp(ib->MAJOR,"4B1C",4)) {
    return 0;
  }
  */
  if (!strncmp(ib->RSSI,"0",1)) {
    return 0;
  }

  // keyfob starts with ffff, ibeacon has valid facid
  if (!strncmp(ib->MAC,"FFFF",4) || strncmp(ib->FACID,"00000000",8)) {
    for (uint32_t cnt=0;cnt<MAX_IBEACONS;cnt++) {
      if (ibeacons[cnt].FLAGS) {
        if (!strncmp_P(ib->UID,PSTR("00000000000000000000000000000000"),32)) {
          if (!strncmp(ibeacons[cnt].MAC,ib->MAC,12)) {
            // exists
            memcpy(ibeacons[cnt].RSSI,ib->RSSI,4);
            ibeacons[cnt].TIME=0;
#ifdef USE_IBEACON_ESP32
            if (ibeacons[cnt].REPTIME >= IB_UPDATE_TIME) {
              ibeacons[cnt].REPTIME = 0;
              ibeacons[cnt].REPORTED = 0;
            }
#endif
            return 1;
          }
        } else {
          if (!strncmp(ibeacons[cnt].UID,ib->UID,32)) {
            // exists
            memcpy(ibeacons[cnt].RSSI,ib->RSSI,4);
            ibeacons[cnt].TIME=0;
#ifdef USE_IBEACON_ESP32
            if (ibeacons[cnt].REPTIME >= IB_UPDATE_TIME) {
              ibeacons[cnt].REPTIME = 0;
              ibeacons[cnt].REPORTED = 0;
            }
#endif
            return 1;
          }
        }
      }
    }
    for (uint32_t cnt=0;cnt<MAX_IBEACONS;cnt++) {
      if (!ibeacons[cnt].FLAGS) {
        memcpy(ibeacons[cnt].MAC,ib->MAC,12);
        memcpy(ibeacons[cnt].RSSI,ib->RSSI,4);
        memcpy(ibeacons[cnt].UID,ib->UID,32);
        memcpy(ibeacons[cnt].MAJOR,ib->MAJOR,4);
        memcpy(ibeacons[cnt].MINOR,ib->MINOR,4);
        ibeacons[cnt].FLAGS=1;
        ibeacons[cnt].TIME=0;
#ifdef USE_IBEACON_ESP32
        memcpy(ibeacons[cnt].NAME,ib->NAME,16);
        ibeacons[cnt].REPTIME = 0;
        ibeacons[cnt].REPORTED = 0;
#endif
        return 1;
      }
    }
  }
  return 0;
}

#ifndef USE_IBEACON_ESP32

void hm17_decode(void) {
  struct IBEACON ib;
  switch (hm17_cmd) {
    case HM17_TEST:
      if (!strncmp(hm17_sbuffer,"OK",2)) {
#ifdef IBEACON_DEBUG
        if (hm17_debug) AddLog(LOG_LEVEL_INFO, PSTR("AT OK"));
#endif
        hm17_sbclr();
        hm17_result=HM17_SUCESS;
        hm17_found=1;
      }
      break;
    case HM17_ROLE:
      if (!strncmp(hm17_sbuffer,"OK+Set:1",8)) {
#ifdef IBEACON_DEBUG
        if (hm17_debug) AddLog(LOG_LEVEL_INFO, PSTR("ROLE OK"));
#endif
        hm17_sbclr();
        hm17_result=HM17_SUCESS;
      }
      break;
    case HM17_IMME:
      if (!strncmp(hm17_sbuffer,"OK+Set:1",8)) {
#ifdef IBEACON_DEBUG
        if (hm17_debug) AddLog(LOG_LEVEL_INFO, PSTR("IMME OK"));
#endif
        hm17_sbclr();
        hm17_result=HM17_SUCESS;
      }
      break;
    case HM17_IBEA:
      if (!strncmp(hm17_sbuffer,"OK+Set:1",8)) {
#ifdef IBEACON_DEBUG
        if (hm17_debug) AddLog(LOG_LEVEL_INFO, PSTR("IBEA OK"));
#endif
        hm17_sbclr();
        hm17_result=HM17_SUCESS;
      }
      break;
    case HM17_SCAN:
        if (!strncmp(hm17_sbuffer,"OK+Set:5",8)) {
#ifdef IBEACON_DEBUG
          if (hm17_debug) AddLog(LOG_LEVEL_INFO, PSTR("SCAN OK"));
#endif
          hm17_sbclr();
          hm17_result=HM17_SUCESS;
        }
        break;
    case HM17_RESET:
      if (!strncmp(hm17_sbuffer,"OK+RESET",8)) {
#ifdef IBEACON_DEBUG
        if (hm17_debug) AddLog(LOG_LEVEL_INFO, PSTR("RESET OK"));
#endif
        hm17_sbclr();
        hm17_result=HM17_SUCESS;
      }
      break;
    case HM17_RENEW:
      if (!strncmp(hm17_sbuffer,"OK+RENEW",8)) {
#ifdef IBEACON_DEBUG
        if (hm17_debug) AddLog(LOG_LEVEL_INFO, PSTR("RENEW OK"));
#endif
        hm17_sbclr();
        hm17_result=HM17_SUCESS;
      }
      break;
    case HM17_CON:
      if (!strncmp(hm17_sbuffer,"OK+CONNA",8)) {
        hm17_sbclr();
#ifdef IBEACON_DEBUG
        if (hm17_debug) AddLog(LOG_LEVEL_INFO, PSTR("CONNA OK"));
#endif
        hm17_connecting=2;
        break;
      }
      if (!strncmp(hm17_sbuffer,"OK+CONNE",8)) {
        hm17_sbclr();
#ifdef IBEACON_DEBUG
        if (hm17_debug) AddLog(LOG_LEVEL_INFO, PSTR("CONNE ERROR"));
#endif
        break;
      }
      if (!strncmp(hm17_sbuffer,"OK+CONNF",8)) {
        hm17_sbclr();
#ifdef IBEACON_DEBUG
        if (hm17_debug) AddLog(LOG_LEVEL_INFO, PSTR("CONNF ERROR"));
#endif
        break;
      }
      if (hm17_connecting==2 && !strncmp(hm17_sbuffer,"OK+CONN",7)) {
        hm17_sbclr();
#ifdef IBEACON_DEBUG
        if (hm17_debug) AddLog(LOG_LEVEL_INFO, PSTR("CONN OK"));
#endif
        hm17_connecting=3;
        hm17_sendcmd(HM17_TEST);
        hm17_connecting=0;
        break;
      }
      break;

    case HM17_DISI:
    case HM17_DISC:
      if (!strncmp(hm17_sbuffer,"OK+DISCS",8)) {
        hm17_sbclr();
        hm17_result=1;
#ifdef IBEACON_DEBUG
        if (hm17_debug) AddLog(LOG_LEVEL_INFO, PSTR("DISCS OK"));
#endif
        break;
      }
      if (!strncmp(hm17_sbuffer,"OK+DISIS",8)) {
        hm17_sbclr();
        hm17_result=1;
#ifdef IBEACON_DEBUG
        if (hm17_debug) AddLog(LOG_LEVEL_INFO, PSTR("DISIS OK"));
#endif
        break;
      }
      if (!strncmp(hm17_sbuffer,"OK+DISCE",8)) {
        hm17_sbclr();
        hm17_result=HM17_SUCESS;
#ifdef IBEACON_DEBUG
        if (hm17_debug) AddLog(LOG_LEVEL_INFO, PSTR("DISCE OK"));
#endif
        hm17_scanning=0;
        break;
      }
      if (!strncmp(hm17_sbuffer,"OK+NAME:",8)) {
        if (hm17_sbuffer[hm17_sindex-1]=='\n') {
          hm17_result=HM17_SUCESS;
#ifdef IBEACON_DEBUG
          if (hm17_debug) {
            AddLog(LOG_LEVEL_INFO, PSTR("NAME OK"));
            AddLog(LOG_LEVEL_INFO, PSTR(">>%s"),&hm17_sbuffer[8]);
          }
#endif
          hm17_sbclr();
        }
        break;
      }
      if (!strncmp(hm17_sbuffer,"OK+DIS0:",8)) {
        if (hm17_cmd==HM17_DISI) {
#ifdef HM17_V110
          goto hm17_v110;
#endif
        } else {
          if (hm17_sindex==20) {
            hm17_result=HM17_SUCESS;
#ifdef IBEACON_DEBUG
            if (hm17_debug) {
              AddLog(LOG_LEVEL_INFO, PSTR("DIS0 OK"));
              AddLog(LOG_LEVEL_INFO, PSTR(">>%s"),&hm17_sbuffer[8]);
            }
#endif
            hm17_sbclr();
          }
        }
        break;
      }
      if (!strncmp(hm17_sbuffer,"OK+DISC:",8)) {
hm17_v110:
        if (hm17_cmd==HM17_DISI) {
          if (hm17_sindex==78) {
#ifdef IBEACON_DEBUG
            if (hm17_debug) {
              AddLog(LOG_LEVEL_INFO, PSTR("DISC: OK"));
              //OK+DISC:4C 000C0E:003 A9144081A8 3B16849611 862EC1005: 0B1CE7485D :4DB4E940F C0E:-078
              AddLog(LOG_LEVEL_INFO, PSTR(">>%s"),&hm17_sbuffer[8]);
            }
#endif
            memcpy(ib.FACID,&hm17_sbuffer[8],8);
            memcpy(ib.UID,&hm17_sbuffer[8+8+1],32);
            memcpy(ib.MAJOR,&hm17_sbuffer[8+8+1+32+1],4);
            memcpy(ib.MINOR,&hm17_sbuffer[8+8+1+32+1+4],4);
            memcpy(ib.PWR,&hm17_sbuffer[8+8+1+32+1+4+4],2);
            memcpy(ib.MAC,&hm17_sbuffer[8+8+1+32+1+4+4+2+1],12);
            memcpy(ib.RSSI,&hm17_sbuffer[8+8+1+32+1+4+4+2+1+12+1],4);

            if (ibeacon_add(&ib)) {
              ibeacon_mqtt(ib.MAC,ib.RSSI,ib.UID,ib.MAJOR,ib.MINOR);
            }
            hm17_sbclr();
            hm17_result=1;
          }
        } else {
#ifdef IBEACON_DEBUG
          if (hm17_debug) AddLog(LOG_LEVEL_INFO, PSTR(">->%s"),&hm17_sbuffer[8]);
#endif
        }
        break;
      }
  }
}

#endif

void IBEACON_loop() {

#ifdef USE_IBEACON_ESP32

  for (uint32_t cnt=0;cnt<MAX_IBEACONS;cnt++) {
    if (ibeacons[cnt].FLAGS && ! ibeacons[cnt].REPORTED) {
      ibeacon_mqtt(ibeacons[cnt].MAC,ibeacons[cnt].RSSI,ibeacons[cnt].UID,ibeacons[cnt].MAJOR,ibeacons[cnt].MINOR,ibeacons[cnt].NAME);
      ibeacons[cnt].REPORTED=1;
    }
  }

#else

  if (!IBEACON_Serial) return;

uint32_t difftime=millis()-hm17_lastms;

  while (IBEACON_Serial->available()) {
    hm17_lastms=millis();
    // shift in
    if (hm17_sindex<HM17_BSIZ) {
      hm17_sbuffer[hm17_sindex]=IBEACON_Serial->read();
      hm17_sindex++;
      hm17_decode();
    } else {
      hm17_sindex=0;
      break;
    }
  }

  if (hm17_cmd==99) {
   if (hm17_sindex>=HM17_BSIZ-2 || (hm17_sindex && (difftime>100))) {
     AddLog(LOG_LEVEL_INFO, PSTR("%s"),hm17_sbuffer);
     hm17_sbclr();
   }
  }

#endif

}

#ifdef USE_WEBSERVER
const char HTTP_IBEACON_mac[] PROGMEM =
 "{s}IBEACON-MAC : %s" " - RSSI : %s" "{m}{e}";
const char HTTP_IBEACON_uid[] PROGMEM =
 "{s}IBEACON-UID : %s" " - RSSI : %s" "{m}{e}";
#ifdef USE_IBEACON_ESP32
const char HTTP_IBEACON_name[] PROGMEM =
 "{s}IBEACON-NAME : %s (%s)" " - RSSI : %s" "{m}{e}";
#endif
void IBEACON_Show(void) {
char mac[14];
char rssi[6];
char uid[34];
#ifdef USE_IBEACON_ESP32
char name[18];
#endif

  for (uint32_t cnt=0;cnt<MAX_IBEACONS;cnt++) {
    if (ibeacons[cnt].FLAGS) {
      memcpy(mac,ibeacons[cnt].MAC,12);
      mac[12]=0;
      memcpy(rssi,ibeacons[cnt].RSSI,4);
      rssi[4]=0;
      memcpy(uid,ibeacons[cnt].UID,32);
      uid[32]=0;
#ifdef USE_IBEACON_ESP32
      memcpy(name,ibeacons[cnt].NAME,16);
      name[16]=0;
#endif
      if (!strncmp_P(uid,PSTR("00000000000000000000000000000000"),32)) {
#ifdef USE_IBEACON_ESP32
        if (name[0]) {
          WSContentSend_PD(HTTP_IBEACON_name,name,mac,rssi);
        } else {
          WSContentSend_PD(HTTP_IBEACON_mac,mac,rssi);
        }
#else
        WSContentSend_PD(HTTP_IBEACON_mac,mac,rssi);
#endif
      } else {
        WSContentSend_PD(HTTP_IBEACON_uid,uid,rssi);
      }
    }
  }

}
#endif  // USE_WEBSERVER

/*
commands sensor52
to initialyze hm17 the cmds
sensor52 1
sensor52 2
must be given only once (stored in module)

uT = sets update interval in seconds (scan tags every T seonds) default=10
tT = sets timeout interval in seconds (after T seconds if tag is not detected send rssi=0) default=30
sending IBEACON_FFFF3D1B1E9D_RSSI with data 99 causes tag to beep (ID to be replaced with actual ID)

*** debugging
dx = sets debug mode to 0,1 (shows hm17 cmds + reactions in console)
c  = clears sensor list
s AT+xxx  = send native cmds to module
e.g. s AT+CONFFFF3D1B1E9D connects to module with ID, then send s AT to disconnect which activates the beeper in the TAG
*/



bool xsns52_cmd(void) {
  bool serviced = true;
  const char S_JSON_IBEACON[] = "{\"" D_CMND_SENSOR "%d\":%s:%d}";
  const char S_JSON_IBEACON1[] = "{\"" D_CMND_SENSOR "%d\":%s:%s}";
  uint16_t len=XdrvMailbox.data_len;
  if (len > 0) {
      char *cp=XdrvMailbox.data;
      if (*cp=='u') {
        cp++;
        if (*cp) IB_UPDATE_TIME=atoi(cp);
        Response_P(S_JSON_IBEACON, XSNS_52,"uintv",IB_UPDATE_TIME);
      } else if (*cp=='t') {
        cp++;
        if (*cp) IB_TIMEOUT_TIME=atoi(cp);
        Response_P(S_JSON_IBEACON, XSNS_52,"lintv",IB_TIMEOUT_TIME);
      } else if (*cp=='c') {
        for (uint32_t cnt=0;cnt<MAX_IBEACONS;cnt++) ibeacons[cnt].FLAGS=0;
        Response_P(S_JSON_IBEACON1, XSNS_52,"clr list","");
      }
#ifndef USE_IBEACON_ESP32
      else if (*cp>='0' && *cp<='8') {
        hm17_sendcmd(*cp&7);
        Response_P(S_JSON_IBEACON, XSNS_52,"hm17cmd",*cp&7);
      } else if (*cp=='s') {
        cp++;
        len--;
        while (*cp==' ') {
          len--;
          cp++;
        }
        IBEACON_Serial->write((uint8_t*)cp,len);
        hm17_cmd=99;
        Response_P(S_JSON_IBEACON1, XSNS_52,"hm17cmd",cp);
      }
#endif
#ifdef IBEACON_DEBUG
      else if (*cp=='d') {
        cp++;
        hm17_debug=atoi(cp);
        Response_P(S_JSON_IBEACON, XSNS_52,"debug",hm17_debug);
      }
#endif
  } else {
    serviced=false;
  }
  return serviced;
}

#define D_CMND_IBEACON "IBEACON"

#ifndef USE_IBEACON_ESP32
//"IBEACON_FFFF3D1B1E9D_RSSI", Data "99" causes TAG to beep
bool ibeacon_cmd(void) {
  ib_mac[0]=0;
  int16_t rssi=0;
  const char S_JSON_IBEACON[] = "{\"" D_CMND_IBEACON "_%s_RSSI\":%d}";
  uint8_t cmd_len = strlen(D_CMND_IBEACON);
  if (!strncasecmp_P(XdrvMailbox.topic, PSTR(D_CMND_IBEACON), cmd_len)) {
    // IBEACON prefix
    rssi = XdrvMailbox.payload;
    if (rssi==99) {
      memcpy(ib_mac,XdrvMailbox.topic+cmd_len+1,12);
      ib_mac[12]=0;
      if (hm17_scanning) {
        // postpone sendbeep
        hm17_flag|=2;
      } else {
        ib_sendbeep();
      }
    }
    Response_P(S_JSON_IBEACON,ib_mac,rssi);
    return true;
  }
  return false;
}

void ib_sendbeep(void) {
  hm17_flag=0;
  hm17_sendcmd(HM17_CON);
}

#endif

#ifdef USE_IBEACON_ESP32
void ibeacon_mqtt(const char *mac,const char *rssi,const char *uid,const char *major,const char *minor, const char *name) {
#else
void ibeacon_mqtt(const char *mac,const char *rssi,const char *uid,const char *major,const char *minor) {
#endif
  char s_mac[14];
  char s_uid[34];
  char s_major[6];
  char s_minor[6];
  char s_rssi[6];
#ifdef USE_IBEACON_ESP32
  char *s_state;
#endif
  char s_name[18];
  memcpy(s_mac,mac,12);
  s_mac[12]=0;
  memcpy(s_uid,uid,32);
  s_uid[32]=0;
  memcpy(s_major,major,4);
  s_major[4]=0;
  memcpy(s_minor,minor,4);
  s_minor[4]=0;
  memcpy(s_rssi,rssi,4);
  s_rssi[4]=0;
  int16_t n_rssi=atoi(s_rssi);
#ifdef USE_IBEACON_ESP32
  if  (n_rssi) {
    s_state=(char *)"ON";
  } else {
    s_state=(char *)"OFF";
  }
#endif
  // if uid == all zeros, take mac
  if (!strncmp_P(s_uid,PSTR("00000000000000000000000000000000"),32)) {
#ifdef USE_IBEACON_ESP32
    if (name[0]) {
      memcpy(s_name,name,16);
      s_name[16]=0;
      ResponseTime_P(PSTR(",\"" D_CMND_IBEACON "\":{\"MAC\":\"%s\",\"NAME\":\"%s\",\"RSSI\":%d,\"STATE\":\"%s\"}}"),s_mac,s_name,n_rssi,s_state);
    } else {
      ResponseTime_P(PSTR(",\"" D_CMND_IBEACON "\":{\"MAC\":\"%s\",\"RSSI\":%d,\"STATE\":\"%s\"}}"),s_mac,n_rssi,s_state);
    }
#else
    ResponseTime_P(PSTR(",\"" D_CMND_IBEACON "\":{\"MAC\":\"%s\",\"UID\":\"%s\",\"MAJOR\":\"%s\",\"MINOR\":\"%s\",\"RSSI\":%d}}"),s_mac,s_uid,s_major,s_minor,n_rssi);
#endif
  } else {
#ifdef USE_IBEACON_ESP32
    ResponseTime_P(PSTR(",\"" D_CMND_IBEACON "\":{\"UID\":\"%s\",\"MAJOR\":\"%s\",\"MINOR\":\"%s\",\"MAC\":\"%s\",\"RSSI\":%d,\"STATE\":\"%s\"}}"),s_uid,s_major,s_minor,s_mac,n_rssi,s_state);
#else
    ResponseTime_P(PSTR(",\"" D_CMND_IBEACON "\":{\"UID\":\"%s\",\"MAJOR\":\"%s\",\"MINOR\":\"%s\",\"MAC\":\"%s\",\"RSSI\":%d}}"),s_uid,s_major,s_minor,s_mac,n_rssi);
#endif
  }

  MqttPublishTeleSensor();
}


/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns52(uint32_t function)
{
  bool result = false;

    switch (function) {
      case FUNC_INIT:
        IBEACON_Init();
        break;
#ifdef USE_IBEACON_ESP32
      case FUNC_EVERY_250_MSECOND:
        if (!ESP32BLE.mode.init) {
          ESP32Init();
        }
        break;
#endif
      case FUNC_LOOP:
        IBEACON_loop();
        break;
      case FUNC_EVERY_SECOND:
#ifdef USE_IBEACON_ESP32
        esp32_every_second();
#else
        hm17_every_second();
#endif
        break;
      case FUNC_COMMAND_SENSOR:
        if (XSNS_52 == XdrvMailbox.index) {
          result = xsns52_cmd();
        }
        break;
#ifndef USE_IBEACON_ESP32
      case FUNC_COMMAND:
        result=ibeacon_cmd();
        break;
#endif
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
#ifndef USE_IBEACON_ESP32
        if (hm17_found) IBEACON_Show();
#else
        IBEACON_Show();
#endif
        break;
#endif  // USE_WEBSERVER
    }
    return result;
}

#endif  // USE_IBEACON

#endif  // USE_BLE_ESP32
