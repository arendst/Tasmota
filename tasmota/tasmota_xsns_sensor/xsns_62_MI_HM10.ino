/*
  xsns_62_MI_HM10.ino - MI-BLE-sensors via HM-10 support for Tasmota

  Copyright (C) 2021  Christian Baars and Theo Arends

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


  --------------------------------------------------------------------------------------------
  Version yyyymmdd  Action    Description
  --------------------------------------------------------------------------------------------
  0.9.6.0 20201127  added   - add BLOCK and OPTION command, send BLE scan via MQTT, refactoring, support negative temps
  ---
  0.9.5.0 20201101  added   - bugfixes, better advertisement parsing, beacon, HASS-fixes, CGD1 now passive readable
  ---
  0.9.4.1 20200807  added   - add ATC, some optimizations and a bit more error handling
  ---
  0.9.4.0 20200807  added   - multiple backports from the HM10-driver (NLIGHT,MJYD2S,YEERC,MHOC401,MHOC303),
                             fixing Flora values, adding realtime-bridge, better battery for LYWSD03 and MHOC401
  ---
  0.9.3.1 20200412  added   - clean ups, code shrink, battery bugfix
  ---
  0.9.3.0 20200322  added   - multi page web view, command HM10PAGE, polling for MJ_HT_V1,
                              more stable readings, internal refactoring
  ---
  0.9.2.0 20200317  added   - MiBeacon-support, add Flora, MJ_HT_V1 and CGD1, add dew point,
                              add AUTO(-scan), RULES-message
  ---
  0.9.1.0 20200209  added   - LYWSD02-support, including setting the time
  ---
  0.9.0.0 20200130  started - initial development by Christian Baars (support LYWSD03 only)
                    forked  - from arendst/tasmota            - https://github.com/arendst/Tasmota

*/
#ifdef ESP8266                     // ESP8266 only. Use define USE_MI_ESP32 for ESP32 support

#ifdef USE_HM10

#define XSNS_62                    62

#include <TasmotaSerial.h>
#include <vector>

TasmotaSerial *HM10Serial;
#define HM10_BAUDRATE              115200  // default with FW>700 is 115200

#define  HM10_MAX_TASK_NUMBER      12
uint8_t  HM10_TASK_LIST[HM10_MAX_TASK_NUMBER+1][2];   // first value: kind of task - second value: delay in x * 100ms

#define  HM10_MAX_RX_BUF           64

#pragma pack(1)  // byte-aligned structures to read the sensor data

struct LYWSD0x_HT_t{
  int16_t temp;
  uint8_t hum;
  uint16_t volt;
};
struct CGD1_HT_t{
  uint8_t spare;
  int16_t temp;
  uint16_t hum;
};
struct Flora_TLMF_t{
  int16_t temp;
  uint8_t spare;
  uint32_t lux;
  uint8_t moist;
  uint16_t fert;
  uint32_t ID;
}; // temperature, lux, moisture, fertility

struct mi_beacon_t{
  uint16_t frame;
  uint16_t productID;
  uint8_t counter;
  uint8_t MAC[6];
  uint8_t spare;
  uint8_t type;
  uint8_t ten;
  uint8_t size;
  union {
    struct{ //0d
      int16_t temp;
      uint16_t hum;
    }HT;
    uint8_t bat; //0a
    int16_t temp; //04
    uint16_t hum; //06
    uint32_t lux; //07
    uint8_t moist; //08
    uint16_t fert; //09
    uint32_t NMT; //17
    struct{ //01
      uint16_t num;
      uint8_t longPress;
    }Btn;
  };
  uint8_t padding[12];
};

struct ATCPacket_t{
  uint8_t MAC[6];
  uint16_t temp; //sadly this is in wrong endianess
  uint8_t hum;
  uint8_t batPer;
  uint16_t batMV;
  uint8_t frameCnt;
};

struct cg_packet_t {
  uint16_t frameID;
  uint8_t MAC[6];
  uint16_t mode;
  union {
    struct {
    int16_t temp;  // -9 - 59 °C
    uint16_t hum;
    };
    uint8_t bat;
  };
};

struct MiScaleV1Packet_t {
	//uint8_t		size;	// = 14
	//uint8_t		uid;	// = 0x16, 16-bit UUID
	//uint16_t	UUID;	// = 0x181D
	uint8_t status; // bit 0 lbs, 4 jin, 5, stabilized, 7, weight removed
	uint16_t weight;
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
};

struct MiScaleV2Packet_t {
	//uint8_t		size;	// = 17
	//uint8_t		uid;	// = 0x16, 16-bit UUID
	//uint16_t	UUID;	// = 0x181B
	uint8_t weight_unit;
	uint8_t status;
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint16_t impedance;
	uint16_t weight;
};

#pragma pack(0)

struct scan_entry_t {
  uint8_t MAC[6];
  uint16_t CID;
  uint16_t UUID;
  int32_t RSSI;
  uint8_t TX;
  union{
    uint16_t SVC;
    uint8_t svcData[32];
  };
};

struct generic_beacon_t {
  uint8_t MAC[6];
  uint32_t time;
  int32_t RSSI;
  uint16_t CID; // company identifier
  uint16_t UUID; // the first, if more than one exists
  uint16_t SVC;
  uint16_t TX;
  bool active = false;
};


struct mi_sensor_t{
  uint8_t type; //Flora = 1; MI-HT_V1=2; LYWSD02=3; LYWSD03=4; CGG1=5; CGD1=6
  uint8_t lastCnt; //device generated counter of the packet
  uint8_t shallSendMQTT;
  uint8_t showedUp;
  uint8_t MAC[6];
  union {
    struct {
      uint32_t temp:1;
      uint32_t hum:1;
      uint32_t tempHum:1; //every hum sensor has temp too, easier to use Tasmota dew point functions
      uint32_t lux:1;
      uint32_t moist:1;
      uint32_t fert:1;
      uint32_t bat:1;
      uint32_t NMT:1;
      uint32_t PIR:1;
      uint32_t Btn:1;
      uint32_t scale:1;
      uint32_t impedance:1;
    };
    uint32_t raw;
  } feature;
  union {
    struct {
      uint32_t temp:1;
      uint32_t hum:1;
      uint32_t tempHum:1; //can be combined from the sensor
      uint32_t lux:1;
      uint32_t moist:1;
      uint32_t fert:1;
      uint32_t bat:1;
      uint32_t NMT:1;
      uint32_t motion:1;
      uint32_t noMotion:1;
      uint32_t Btn:1;
      uint32_t scale:1;
    };
    uint32_t raw;
  } eventType;

  int rssi;
  uint32_t lastTime;
  uint32_t lux;
  float temp; //Flora, MJ_HT_V1, LYWSD0x, CGx
  union {
    struct {
      uint8_t moisture;
      uint16_t fertility;
      char firmware[6]; // actually only for FLORA but hopefully we can add for more devices
    }; // Flora
    struct {
      float hum;
    }; // MJ_HT_V1, LYWSD0x
    struct {
      uint16_t events; //"alarms" since boot
      uint32_t NMT;    // no motion time in seconds for the MJYD2S
    };
    uint16_t Btn;
    struct {
      uint8_t has_impedance;
      uint8_t impedance_stabilized;
      uint8_t weight_stabilized;
      uint8_t weight_removed;
      char weight_unit[4]; // kg, lbs, jin or empty when unknown
      float weight;
      uint16_t impedance;
      struct {
        uint16_t	year;
        uint8_t		month;
        uint8_t		day;
        uint8_t		hour;
        uint8_t		minute;
        uint8_t		second;
      } datetime;
    };
  };
  union {
      uint8_t bat; // many values seem to be hard-coded garbage (LYWSD0x, GCD1)
  };
};

struct {
  uint8_t current_task_delay;  // number of 100ms-cycles
  uint8_t last_command;
  uint16_t perPage = 4;
  uint16_t firmware;
  uint32_t period;             // set manually in addition to TELE-period, is set to TELE-period after start
  uint32_t serialSpeed;
  union {
    uint32_t time;
    uint8_t timebuf[4];
  };
  uint16_t autoScanInterval;
  struct {
    uint32_t awaiting:8;
    uint32_t init:1;
    uint32_t pending_task:1;
    uint32_t connected:1;
    uint32_t subscribed:1;
    uint32_t autoScan:1;
    uint32_t shallTriggerTele:1;
    uint32_t triggeredTele:1;
    uint32_t activeBeacon:1;
    uint32_t firstAutodiscoveryDone:1;
    uint32_t shallShowScanResult:1;
    uint32_t shallShowBlockList:1;
} mode;
  struct {
    uint8_t sensor;           // points to to the number 0...255
    uint8_t beaconScanCounter;
    // TODO: more to come
  } state;
  struct {
    uint32_t allwaysAggregate:1; // always show all known values of one sensor in brdigemode
    uint32_t noSummary:1;        // no sensor values at TELE-period
    uint32_t directBridgeMode:1; // send every received BLE-packet as a MQTT-message in real-time
    uint32_t holdBackFirstAutodiscovery:1; // allows to trigger it later
    uint32_t showRSSI:1;
    uint32_t ignoreBogusBattery:1;
    uint32_t minimalSummary:1;   // DEPRECATED!!
  } option;
  scan_entry_t rxAdvertisement;
  char *rxBuffer;
} HM10;

struct MAC_t {
  uint8_t buf[6];
};

std::vector<mi_sensor_t> MIBLEsensors;
std::array<generic_beacon_t,4> MIBLEbeacons; // we support a fixed number
std::vector<scan_entry_t> MIBLEscanResult;
std::vector<MAC_t> MIBLEBlockList;

/*********************************************************************************************\
 * constants
\*********************************************************************************************/

#define D_CMND_HM10 "HM10"

const char S_JSON_HM10_COMMAND_NVALUE[] PROGMEM = "{\"" D_CMND_HM10 "%s\":%d}";
const char S_JSON_HM10_COMMAND_SVALUE[] PROGMEM = "{\"" D_CMND_HM10 "%s%u\":\"%s\"}";
const char S_JSON_HM10_COMMAND[] PROGMEM        = "{\"" D_CMND_HM10 "%s%s\"}";
const char kHM10_Commands[] PROGMEM             = D_CMND_HM10"|"
                                                  "Scan|AT|Period|Baud|Time|Auto|Page|Beacon|Block|Option";

const char HTTP_MISCALE_WEIGHT[] PROGMEM = "{s}%s" " Weight" "{m}%*_f %s{e}";
const char HTTP_MISCALE_IMPEDANCE[] PROGMEM = "{s}%s" " Impedance" "{m}%u{e}";
const char HTTP_MISCALE_WEIGHT_REMOVED[] PROGMEM = "{s}%s" " Weight removed" "{m}%s{e}";
const char HTTP_MISCALE_STABILIZED[] PROGMEM = "{s}%s" " Stabilized" "{m}%s{e}";

void (*const HM10_Commands[])(void) PROGMEM = { &CmndHM10Scan, &CmndHM10AT, &CmndHM10Period, &CmndHM10Baud, &CmndHM10Time, &CmndHM10Auto, &CmndHM10Page, &CmndHM10Beacon, &CmndHM10Block, &CmndHM10Option };


#define HM10_FLORA        1
#define HM10_MJ_HT_V1     2
#define HM10_LYWSD02      3
#define HM10_LYWSD03MMC   4
#define HM10_CGG1         5
#define HM10_CGD1         6
#define HM10_NLIGHT       7
#define HM10_MJYD2S       8
#define HM10_YEERC        9
#define HM10_MHOC401      10
#define HM10_MHOC303      11
#define HM10_ATC          12
#define HM10_MI_SCALE_V1  13
#define HM10_MI_SCALE_V2  14

#define HM10_TYPES    14 //count this manually

const uint16_t kHM10SlaveID[HM10_TYPES]={
                                  0x0098, // Flora
                                  0x01aa, // MJ_HT_V1
                                  0x045b, // LYWSD02
                                  0x055b, // LYWSD03
                                  0x0347, // CGG1
                                  0x0576, // CGD1
                                  0x03dd, // NLIGHT
                                  0x07f6, // MJYD2S
                                  0x0153, // yee-rc
                                  0x0387, // MHO-C401
                                  0x06d3, // MHO-C303
                                  0x0a1c,  // ATC -> this is a fake ID
                                  0x181d, // Mi Scale V1
                                  0x181b  // Mi Scale V2
                                  };

const char kHM10DeviceType1[] PROGMEM = "Flora";
const char kHM10DeviceType2[] PROGMEM = "MJ_HT_V1";
const char kHM10DeviceType3[] PROGMEM = "LYWSD02";
const char kHM10DeviceType4[] PROGMEM = "LYWSD03";
const char kHM10DeviceType5[] PROGMEM = "CGG1";
const char kHM10DeviceType6[] PROGMEM = "CGD1";
const char kHM10DeviceType7[] PROGMEM = "NLIGHT";
const char kHM10DeviceType8[] PROGMEM = "MJYD2S";
const char kHM10DeviceType9[] PROGMEM = "YEERC";
const char kHM10DeviceType10[] PROGMEM ="MHOC401";
const char kHM10DeviceType11[] PROGMEM ="MHOC303";
const char kHM10DeviceType12[] PROGMEM ="ATC";
const char kHM10DeviceType13[] PROGMEM ="MSCALE1";
const char kHM10DeviceType14[] PROGMEM ="MSCALE2";

const char * kHM10DeviceType[] PROGMEM = {kHM10DeviceType1,kHM10DeviceType2,kHM10DeviceType3,kHM10DeviceType4,kHM10DeviceType5,kHM10DeviceType6,kHM10DeviceType7,kHM10DeviceType8,kHM10DeviceType9,kHM10DeviceType10,kHM10DeviceType11,kHM10DeviceType12,kHM10DeviceType13,kHM10DeviceType14};

/*********************************************************************************************\
 * enumerations
\*********************************************************************************************/

enum HM10_awaitData: uint8_t {
    none = 0,
    tempHumLY = 1,
    TLMF = 2,
    bat = 3,
    tempHumCGD1 = 4,
    discScan = 5,
    tempHumMJ = 6
    };

/*********************************************************************************************\
 * Task codes defines
\*********************************************************************************************/

#define TASK_HM10_NOTASK          0                         // nothing to be done
#define TASK_HM10_ROLE1           1                         // change role to 1
#define TASK_HM10_IMME1           2                         // change imme to 1
#define TASK_HM10_RENEW           3                         // device factory setting
#define TASK_HM10_RESET           4                         // device reset
#define TASK_HM10_DISC            5                         // device discovery scan: AT+DISA?
#define TASK_HM10_CONN            6                         // connect to given MAC
#define TASK_HM10_VERSION         7                         // query FW version
#define TASK_HM10_NAME            8                         // query device name
#define TASK_HM10_FEEDBACK        9                         // get device response
#define TASK_HM10_DISCONN         10                        // disconnect
#define TASK_HM10_SUB_L3          11                        // subscribe to service handle 37

#define TASK_HM10_SCAN9           13                        // longest discovery scan possible
#define TASK_HM10_UN_L3           14                        // unsubscribe  service handle 37

#define TASK_HM10_READ_BT_L3      16                        // read from handle 3A -> Battery
#define TASK_HM10_SUB_L2          17                        // subscribe to service handle 3C
#define TASK_HM10_UN_L2           18                        // unsubscribe  service handle 3C
#define TASK_HM10_READ_BT_L2      19                        // read from handle 43 -> Battery
#define TASK_HM10_TIME_L2         20                        // set time of LYWSD02 to system time
#define TASK_HM10_SHOW0           21                        // set verbositiy to minimum
#define TASK_HM10_READ_BF_FL      22                        // read battery and firmware from flower care
#define TASK_HM10_CALL_TLMF_FL    23                        // write 0xa01f to handle 0x33 to init sensor readings
#define TASK_HM10_READ_TLMF_FL    24                        // read temp,lux,moist and fert from flower care
#define TASK_HM10_SUB_HT_CGD1     25                        // subscribe to service handle 4b
#define TASK_HM10_UN_HT_CGD1      26                        // unsubscribe  service handle 4b
#define TASK_HM10_READ_B_CGD1     27                        // read service handle 11

#define TASK_HM10_READ_B_MJ       29                        // read service handle 18
#define TASK_HM10_SUB_HT_MJ       30                        // subscribe to service handle 0f

#define TASK_HM10_STATUS_EVENT    32                        // process status for RULES

#define TASK_HM10_DONE            99                        // used, if there was a task in the slot or just to wait

/*********************************************************************************************\
 * Helper functions
\*********************************************************************************************/

void HM10_Launchtask(uint8_t task, uint8_t slot, uint8_t delay){
                          HM10_TASK_LIST[slot][0]   = task;
                          HM10_TASK_LIST[slot][1]   = delay;
                          HM10_TASK_LIST[slot+1][0] = TASK_HM10_NOTASK;           // the tasks must always be launched in ascending order!!
                          HM10.current_task_delay   = HM10_TASK_LIST[0][1];
}

void HM10_TaskReplaceInSlot(uint8_t task, uint8_t slot){
                          HM10.last_command         = HM10_TASK_LIST[slot][0];    // save command
                          HM10_TASK_LIST[slot][0]   = task;
}

/**
 * @brief Remove all colons from null terminated char array
 *
 * @param _string Typically representing a MAC-address like AA:BB:CC:DD:EE:FF
 */
void HM10stripColon(char* _string){
  uint32_t _length = strlen(_string);
  uint32_t _index = 0;
  while (_index < _length) {
    char c = _string[_index];
    if(c==':'){
      memmove(_string+_index,_string+_index+1,_length-_index);
    }
    _index++;
  }
  _string[_index] = 0;
}

/**
 * @brief Convert string that repesents a hexadecimal number to a byte array
 *
 * @param _string input string in format: AABBCCDDEEFF or AA:BB:CC:DD:EE:FF, caseinsensitive
 * @param _mac  target byte array must match the correct size (i.e. AA:BB -> uint8_t bytes[2])
 */

void HM10HexStringToBytes(char* _string, uint8_t* _byteArray) {
  HM10stripColon(_string);
  UpperCase(_string,_string);
  uint32_t index = 0;
  uint32_t _end = strlen(_string);
  memset(_byteArray,0,_end/2);
  while (index < _end) {
      char c = _string[index];
      uint8_t value = 0;
      if(c >= '0' && c <= '9')
        value = (c - '0');
      else if (c >= 'A' && c <= 'F')
        value = (10 + (c - 'A'));
      _byteArray[(index/2)] += value << (((index + 1) % 2) * 4);
      index++;
  }
}


void HM10_ReverseMAC(uint8_t _mac[]){
  uint8_t _reversedMAC[6];
  for (uint8_t i=0; i<6; i++){
    _reversedMAC[5-i] = _mac[i];
  }
  memcpy(_mac,_reversedMAC, sizeof(_reversedMAC));
}

/*********************************************************************************************\
 * chained tasks
\*********************************************************************************************/

void HM10_Reset(void) {   HM10_Launchtask(TASK_HM10_DISCONN,0,1);       // disconnect
                          HM10_Launchtask(TASK_HM10_ROLE1,1,1);         // set role to 1
                          HM10_Launchtask(TASK_HM10_IMME1,2,1);         // set imme to 1
                          HM10_Launchtask(TASK_HM10_RESET,3,1);         // reset Device
                          HM10_Launchtask(TASK_HM10_VERSION,4,10);      // read SW Version
                          HM10_Launchtask(TASK_HM10_SCAN9,5,2);         // scan time 9 seconds
                          HM10_Launchtask(TASK_HM10_DISC,6,2);          // discovery
                          HM10_Launchtask(TASK_HM10_STATUS_EVENT,7,2);  // status
                          }

void HM10_Discovery_Scan(void) {
                          // HM10_Launchtask(TASK_HM10_DISCONN,0,1);       // disconnect
                          HM10_Launchtask(TASK_HM10_DISC,0,1);          // discovery
                          // HM10_Launchtask(TASK_HM10_STATUS_EVENT,2,1);  // status
                          }

void HM10_Read_LYWSD03(void) { //and MHO-C401
                          HM10_Launchtask(TASK_HM10_CONN,0,1);           // connect
                          HM10_Launchtask(TASK_HM10_FEEDBACK,1,35);      // get OK+CONN
                          HM10_Launchtask(TASK_HM10_SUB_L3,2,20);        // subscribe
                          HM10_Launchtask(TASK_HM10_UN_L3,3,80);         // unsubscribe
                          // HM10_Launchtask(TASK_HM10_READ_BT_L3,4,5);     // read Battery
                          HM10_Launchtask(TASK_HM10_DISCONN,4,5);        // disconnect
                          }

void HM10_Read_LYWSD02(void) { //and MHO-C303
                          HM10_Launchtask(TASK_HM10_CONN,0,1);           // connect
                          HM10_Launchtask(TASK_HM10_FEEDBACK,1,35);      // get OK+CONN
                          HM10_Launchtask(TASK_HM10_SUB_L2,2,20);        // subscribe
                          HM10_Launchtask(TASK_HM10_UN_L2,3,80);         // unsubscribe
                          HM10_Launchtask(TASK_HM10_READ_BT_L2,4,5);     // read Battery
                          HM10_Launchtask(TASK_HM10_DISCONN,5,5);        // disconnect
                          }

void HM10_Time_LYWSD02(void) {
                          HM10_Launchtask(TASK_HM10_DISCONN,0,0);        // disconnect
                          HM10_Launchtask(TASK_HM10_CONN,1,5);           // connect
                          HM10_Launchtask(TASK_HM10_FEEDBACK,2,35);      // get OK+CONN
                          HM10_Launchtask(TASK_HM10_TIME_L2,3,20);       // subscribe
                          HM10_Launchtask(TASK_HM10_DISCONN,4,5);        // disconnect
                          }

void HM10_Read_Flora(void) {
                          HM10_Launchtask(TASK_HM10_DISCONN,0,0);        // disconnect
                          HM10_Launchtask(TASK_HM10_CONN,1,1);           // connect
                          HM10_Launchtask(TASK_HM10_FEEDBACK,2,5);       // get OK+CONN
                          HM10_Launchtask(TASK_HM10_READ_BF_FL,3,20);    // read battery
                          HM10_Launchtask(TASK_HM10_CALL_TLMF_FL,4,30);  // read TLMF
                          HM10_Launchtask(TASK_HM10_DISCONN,5,10);       // disconnect
                          }

void HM10_Read_CGD1(void) {
                          HM10_Launchtask(TASK_HM10_CONN,0,1);           // connect
                          HM10_Launchtask(TASK_HM10_FEEDBACK,1,35);      // get OK+CONN
                          HM10_Launchtask(TASK_HM10_SUB_HT_CGD1,2,20);   // subscribe
                          HM10_Launchtask(TASK_HM10_UN_HT_CGD1,3,10);    // unsubscribe
                          HM10_Launchtask(TASK_HM10_READ_B_CGD1,4,5);    // read Battery
                          HM10_Launchtask(TASK_HM10_DISCONN,5,5);        // disconnect
                          }

void HM10_Read_MJ_HT_V1(void) {
                          HM10_Launchtask(TASK_HM10_CONN,0,1);           // connect
                          HM10_Launchtask(TASK_HM10_FEEDBACK,1,35);      // get OK+CONN
                          HM10_Launchtask(TASK_HM10_READ_B_MJ,2,20);     // battery
                          HM10_Launchtask(TASK_HM10_SUB_HT_MJ,3,10);     // temp hum
                          HM10_Launchtask(TASK_HM10_DISCONN,4,5);        // disconnect
                          }

/**
 * @brief Return the slot number of a known sensor or return create new sensor slot
 *
 * @param _MAC     BLE address of the sensor
 * @param _type       Type number of the sensor
 * @return uint32_t   Known or new slot in the sensors-vector
 */
uint32_t MIBLEgetSensorSlot(uint8_t (&_MAC)[6], uint16_t _type, int _rssi){

  DEBUG_SENSOR_LOG(PSTR("%s: will test ID-type: %x"),D_CMND_HM10, _type);
  bool _success = false;
  for (uint32_t i=0;i<HM10_TYPES;i++){
    if(_type == kHM10SlaveID[i]){
      DEBUG_SENSOR_LOG(PSTR("HM10: ID is type %u"), i);
      _type = i+1;
      _success = true;
    }
    else {
      DEBUG_SENSOR_LOG(PSTR("%s: ID-type is not: %x"),D_CMND_HM10,kHM10SlaveID[i]);
    }
  }
  if(!_success) return 0xff;

  DEBUG_SENSOR_LOG(PSTR("%s: vector size %u"),D_CMND_HM10, MIBLEsensors.size());
  for(uint32_t i=0; i<MIBLEsensors.size(); i++){
    if(memcmp(_MAC,MIBLEsensors[i].MAC,sizeof(_MAC))==0){
      DEBUG_SENSOR_LOG(PSTR("%s: known sensor at slot: %u"),D_CMND_HM10, i);
      MIBLEsensors[i].rssi=_rssi;
      if(MIBLEsensors[i].showedUp < 4){ // if we got an intact packet, the sensor should show up several times
        MIBLEsensors[i].showedUp++;     // count up to the above number ... now we are pretty sure
      }
      return i;
    }
    DEBUG_SENSOR_LOG(PSTR("%s: i: %x %x %x %x %x %x"),D_CMND_HM10, MIBLEsensors[i].MAC[5], MIBLEsensors[i].MAC[4],MIBLEsensors[i].MAC[3],MIBLEsensors[i].MAC[2],MIBLEsensors[i].MAC[1],MIBLEsensors[i].MAC[0]);
    DEBUG_SENSOR_LOG(PSTR("%s: n: %x %x %x %x %x %x"),D_CMND_HM10, _MAC[5], _MAC[4], _MAC[3],_MAC[2],_MAC[1],_MAC[0]);
  }
  DEBUG_SENSOR_LOG(PSTR("%s: found new sensor"),D_CMND_HM10);
  mi_sensor_t _newSensor;
  memcpy(_newSensor.MAC,_MAC, sizeof(_MAC));
  _newSensor.type = _type;
  _newSensor.eventType.raw = 0;
  _newSensor.feature.raw = 0;
  _newSensor.temp =NAN;
  _newSensor.bat=0x00;
  _newSensor.rssi=_rssi;
  _newSensor.lux = 0x00ffffff;
  switch (_type){
    case HM10_FLORA:
      _newSensor.moisture =0xff;
      _newSensor.fertility =0xffff;
      _newSensor.firmware[0]='\0';
      _newSensor.feature.temp=1;
      _newSensor.feature.moist=1;
      _newSensor.feature.fert=1;
      _newSensor.feature.lux=1;
      _newSensor.feature.bat=1;
      break;
    case HM10_NLIGHT:
      _newSensor.events=0x00;
      _newSensor.feature.PIR=1;
      _newSensor.feature.NMT=1;
      break;
    case HM10_MJYD2S:
      _newSensor.NMT=0;
      _newSensor.events=0x00;
      _newSensor.feature.PIR=1;
      _newSensor.feature.NMT=1;
      _newSensor.feature.lux=1;
      _newSensor.feature.bat=1;
      break;
    case HM10_YEERC:
      _newSensor.feature.Btn=1;
      break;
    case HM10_MI_SCALE_V1:
      _newSensor.weight=NAN;
      _newSensor.feature.scale=1;
    break;
    case HM10_MI_SCALE_V2:
      _newSensor.weight=NAN;
      _newSensor.feature.scale=1;
      _newSensor.feature.impedance=1;
      break;
    default:
      _newSensor.hum=NAN;
      _newSensor.feature.temp=1;
      _newSensor.feature.hum=1;
      _newSensor.feature.tempHum=1;
      _newSensor.feature.bat=1;
      break;
    }
  MIBLEsensors.push_back(_newSensor);
  AddLog(LOG_LEVEL_DEBUG,PSTR("%s: new %s at slot: %u"),D_CMND_HM10, kHM10DeviceType[_type-1],MIBLEsensors.size()-1);
  return MIBLEsensors.size()-1;
};

/*********************************************************************************************\
 * init serial
 * define serial rx/tx port fixed with 115200 baud
\*********************************************************************************************/

void HM10SerialInit(void) {
  HM10.mode.init = false;
  HM10.serialSpeed = HM10_BAUDRATE;
  HM10Serial = new TasmotaSerial(Pin(GPIO_HM10_RX), Pin(GPIO_HM10_TX), 1, 0, HM10_MAX_RX_BUF);
  if (HM10Serial->begin(HM10.serialSpeed)) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("%s start serial communication fixed to 115200 baud"),D_CMND_HM10);
    if (HM10Serial->hardwareSerial()) {
      ClaimSerial();
      DEBUG_SENSOR_LOG(PSTR("%s: claim HW"),D_CMND_HM10);
    }
    HM10_Reset();
    HM10.mode.pending_task = 1;
    HM10.mode.init = 1;
    HM10.period = Settings->tele_period;
    DEBUG_SENSOR_LOG(PSTR("%s_TASK_LIST initialized, now return to main loop"),D_CMND_HM10);

    //test section for options
    HM10.option.allwaysAggregate = 1;
    HM10.option.noSummary = 0;
    HM10.option.minimalSummary = 0;
    HM10.option.directBridgeMode = 0;
    HM10.option.showRSSI = 1;
    HM10.option.ignoreBogusBattery = 1; // from advertisements
    HM10.option.holdBackFirstAutodiscovery = 1;
    HM10.mode.autoScan = 1;

    HM10.rxBuffer = new char[HM10_MAX_RX_BUF];
  }
  return;
}

/*********************************************************************************************\
 * parse the response
\*********************************************************************************************/

void HM10parseMiBeacon(char * _buf, uint32_t _slot){
  float _tempFloat;
  mi_beacon_t _beacon;
  if (MIBLEsensors[_slot].type==HM10_MJ_HT_V1 || MIBLEsensors[_slot].type==HM10_CGG1 || MIBLEsensors[_slot].type==HM10_YEERC){
    memcpy((uint8_t*)&_beacon+1,(uint8_t*)_buf, sizeof(_beacon)-1); // shift by one byte for the MJ_HT_V1
    memcpy((uint8_t*)&_beacon.MAC,(uint8_t*)&_beacon.MAC+1,6);    // but shift back the MAC
  }
  else{
    memcpy((void*)&_beacon,(void*)_buf, sizeof(_beacon));
  }
  HM10_ReverseMAC(_beacon.MAC);
  // if(memcmp(_beacon.MAC,MIBLEsensors[_slot].MAC,sizeof(_beacon.MAC))!=0){
  //   if (MIBLEsensors[_slot].showedUp>3) return; // probably false alarm from a damaged packet
  //   AddLog(LOG_LEVEL_DEBUG, PSTR("%s: remove garbage sensor"),D_CMND_HM10);
  //   DEBUG_SENSOR_LOG(PSTR("%s i: %x %x %x %x %x %x"),D_CMND_HM10, MIBLEsensors[_slot].MAC[5], MIBLEsensors[_slot].MAC[4],MIBLEsensors[_slot].MAC[3],MIBLEsensors[_slot].MAC[2],MIBLEsensors[_slot].MAC[1],MIBLEsensors[_slot].MAC[0]);
  //   DEBUG_SENSOR_LOG(PSTR("%s n: %x %x %x %x %x %x"),D_CMND_HM10, _beacon.MAC[5], _beacon.MAC[4], _beacon.MAC[3],_beacon.MAC[2],_beacon.MAC[1],_beacon.MAC[0]);
  //   MIBLEsensors.erase(MIBLEsensors.begin()+_slot);
  //   return;
  // }
  // if (MIBLEsensors[_slot].showedUp<4) MIBLEsensors[_slot].showedUp++;

  DEBUG_SENSOR_LOG(PSTR("MiBeacon type:%02x: %02x %02x %02x %02x %02x %02x %02x %02x"),_beacon.type, (uint8_t)_buf[0],(uint8_t)_buf[1],(uint8_t)_buf[2],(uint8_t)_buf[3],(uint8_t)_buf[4],(uint8_t)_buf[5],(uint8_t)_buf[6],(uint8_t)_buf[7]);
  DEBUG_SENSOR_LOG(PSTR("         type:%02x: %02x %02x %02x %02x %02x %02x %02x %02x"),_beacon.type, (uint8_t)_buf[8],(uint8_t)_buf[9],(uint8_t)_buf[10],(uint8_t)_buf[11],(uint8_t)_buf[12],(uint8_t)_buf[13],(uint8_t)_buf[14],(uint8_t)_buf[15]);

  // MIBLEsensors[_slot].rssi = _rssi;
  if(MIBLEsensors[_slot].type==HM10_LYWSD03MMC || MIBLEsensors[_slot].type==HM10_CGD1 || MIBLEsensors[_slot].type==HM10_MHOC401){
    DEBUG_SENSOR_LOG(PSTR("LYWSD03 and CGD1 no support for MiBeacon, type %u"),MIBLEsensors[_slot].type);
    return;
  }
  AddLog(LOG_LEVEL_DEBUG, PSTR("%s: %s mibeacon type: %x"),D_CMND_HM10, kHM10DeviceType[MIBLEsensors[_slot].type-1], _beacon.type);

  DEBUG_SENSOR_LOG(PSTR("%s at slot %u"), kHM10DeviceType[MIBLEsensors[_slot].type-1],_slot);
  switch(_beacon.type){
    case 0x01:
      MIBLEsensors[_slot].Btn=_beacon.Btn.num + (_beacon.Btn.longPress/2)*6;
      MIBLEsensors[_slot].eventType.Btn = 1;
      HM10.mode.shallTriggerTele = 1;
    DEBUG_SENSOR_LOG(PSTR("Mode 1: U16:  %u Button"), MIBLEsensors[_slot].Btn );
    break;
    case 0x04:
    _tempFloat=(float)(_beacon.temp)/10.0f;
    if(_tempFloat<60){
        MIBLEsensors[_slot].temp=_tempFloat;
        DEBUG_SENSOR_LOG(PSTR("Mode 4: temp updated"));
        MIBLEsensors[_slot].eventType.temp = 1;
    }
    DEBUG_SENSOR_LOG(PSTR("Mode 4: U16:  %u Temp"), _beacon.temp );
    break;
    case 0x06:
    _tempFloat=(float)(_beacon.hum)/10.0f;
    if(_tempFloat<101){
        MIBLEsensors[_slot].hum=_tempFloat;
        DEBUG_SENSOR_LOG(PSTR("Mode 6: hum updated"));
        MIBLEsensors[_slot].eventType.hum = 1;
    }
    DEBUG_SENSOR_LOG(PSTR("Mode 6: U16:  %u Hum"), _beacon.hum);
    break;
    case 0x07:
    if(MIBLEsensors[_slot].type==HM10_MJYD2S){
      MIBLEsensors[_slot].eventType.noMotion  = 1;
    }
    MIBLEsensors[_slot].lux=_beacon.lux & 0x00ffffff;
    DEBUG_SENSOR_LOG(PSTR("Mode 7: U24: %u Lux"), _beacon.lux & 0x00ffffff);
    break;
    case 0x08:
    if(_beacon.moist<101){
        MIBLEsensors[_slot].moisture=_beacon.moist;
        DEBUG_SENSOR_LOG(PSTR("Mode 8: moisture updated"));
        MIBLEsensors[_slot].eventType.moist = 1;
    }
    DEBUG_SENSOR_LOG(PSTR("Mode 8: U8: %u Moisture"), _beacon.moist);
    break;
    case 0x09:
    if(_beacon.fert<65535){
        MIBLEsensors[_slot].fertility=_beacon.fert;
        DEBUG_SENSOR_LOG(PSTR("Mode 9: fertility updated"));
        MIBLEsensors[_slot].eventType.fert = 1;
    }
    DEBUG_SENSOR_LOG(PSTR("Mode 9: U16: %u Fertility"), _beacon.fert);
    break;
    case 0x0a:
    if(_beacon.bat<101){
      MIBLEsensors[_slot].bat = _beacon.bat;
      DEBUG_SENSOR_LOG(PSTR("Mode a: bat updated"));
      MIBLEsensors[_slot].eventType.bat = 1;
      }
    DEBUG_SENSOR_LOG(PSTR("Mode a: U8: %u %%"), _beacon.bat);
    break;
    case 0x0d:
    _tempFloat=(float)(_beacon.HT.temp)/10.0f;
    if(_tempFloat<60){
        MIBLEsensors[_slot].temp = _tempFloat;
        DEBUG_SENSOR_LOG(PSTR("Mode d: temp updated"));
     }
    _tempFloat=(float)(_beacon.HT.hum)/10.0f;
    if(_tempFloat<100){
        MIBLEsensors[_slot].hum = _tempFloat;
        DEBUG_SENSOR_LOG(PSTR("Mode d: hum updated"));
    }
    MIBLEsensors[_slot].eventType.tempHum = 1;
    DEBUG_SENSOR_LOG(PSTR("Mode d: U16:  %x Temp U16: %x Hum"), _beacon.HT.temp,  _beacon.HT.hum);
    break;
  }
  if(MIBLEsensors[_slot].eventType.raw == 0) return;
  MIBLEsensors[_slot].shallSendMQTT = 1;
  if(HM10.option.directBridgeMode) HM10.mode.shallTriggerTele = 1;
}

void HM10parseATC(char * _buf, uint32_t _slot){
  ATCPacket_t *_packet = (ATCPacket_t*)_buf;
  if(memcmp(_packet->MAC,MIBLEsensors.at(_slot).MAC,6)!=0) return; // data corruption
  MIBLEsensors.at(_slot).temp = (float)(int16_t(__builtin_bswap16(_packet->temp)))/10.0f;
  MIBLEsensors.at(_slot).hum = (float)_packet->hum;
  MIBLEsensors.at(_slot).bat = _packet->batPer;
  MIBLEsensors[_slot].shallSendMQTT = 1;
  if(HM10.option.directBridgeMode) HM10.mode.shallTriggerTele = 1;
}

void HM10parseCGD1Packet(char * _buf, uint32_t _slot){ // no MiBeacon
  cg_packet_t *_packet = (cg_packet_t*)_buf;
  switch (_packet->mode){
    case 0x0401:
      float _tempFloat;
      _tempFloat=(float)(_packet->temp)/10.0f;
      if(_tempFloat<60){
          MIBLEsensors.at(_slot).temp = _tempFloat;
          MIBLEsensors[_slot].eventType.temp  = 1;
          DEBUG_SENSOR_LOG(PSTR("CGD1: temp updated"));
      }
      _tempFloat=(float)(_packet->hum)/10.0f;
      if(_tempFloat<100){
          MIBLEsensors.at(_slot).hum = _tempFloat;
          MIBLEsensors[_slot].eventType.hum  = 1;
          DEBUG_SENSOR_LOG(PSTR("CGD1: hum updated"));
      }
      DEBUG_SENSOR_LOG(PSTR("CGD1: U16:  %x Temp U16: %x Hum"), _packet->temp,  _packet->hum);
      break;
    case 0x0102:
      if(_packet->bat<101){
      MIBLEsensors.at(_slot).bat = _packet->bat;
      MIBLEsensors[_slot].eventType.bat  = 1;
      DEBUG_SENSOR_LOG(PSTR("Mode a: bat updated"));
      }
      break;
    default:
      DEBUG_SENSOR_LOG(PSTR("HM10: unexpected CGD1-packet"));
  }
  if(MIBLEsensors[_slot].eventType.raw == 0) return;
  MIBLEsensors[_slot].shallSendMQTT = 1;
  if(HM10.option.directBridgeMode) HM10.mode.shallTriggerTele = 1;
}

void HM10ParseMiScalePacket(char * _buf, uint32_t _slot, uint16_t _type){
//void MI32ParseMiScalePacket(const uint8_t * _buf, uint32_t length, const uint8_t *addr, int RSSI, int UUID){

  uint8_t impedance_stabilized = 0;
  uint8_t weight_stabilized = 0;
  uint8_t weight_removed = 0;

  if (isnan(MIBLEsensors[_slot].weight))
  {
    //First recieved packet, need to set default values to prevent memory garbage display
    // For other packets only stable values will be displayed (or bridgeMode will send all packets)
    MIBLEsensors[_slot].eventType.scale = 1;
    MIBLEsensors[_slot].has_impedance = 0;
    MIBLEsensors[_slot].weight_stabilized = 0;
    MIBLEsensors[_slot].impedance_stabilized = 0;
    MIBLEsensors[_slot].weight_removed = 0;
    MIBLEsensors[_slot].weight = 0.0f;
    MIBLEsensors[_slot].impedance = 0;
    strcpy(MIBLEsensors[_slot].weight_unit, PSTR("kg"));
  }

  // Mi Scale V1
  if (_type == 0x181d){ // 14-1-1-2
    MiScaleV1Packet_t *_packetV1 = (MiScaleV1Packet_t*)_buf;
    if ((_slot >= 0) && (_slot < MIBLEsensors.size())){
      DEBUG_SENSOR_LOG(PSTR("HM10: %s: at slot %u"), kHM10DeviceType[MIBLEsensors[_slot].type-1],_slot);

      MIBLEsensors[_slot].eventType.scale = 1;

      weight_stabilized = (_packetV1->status & (1 << 5)) ? 1 : 0;
      weight_removed = (_packetV1->status & (1 << 7)) ? 1 : 0;

      // Set sensor values for every packet in BridgedMode and for every packet with stable weight
      if (HM10.option.directBridgeMode || (weight_stabilized && !weight_removed))
      {
        MIBLEsensors[_slot].weight_stabilized = weight_stabilized;
        MIBLEsensors[_slot].weight_removed = weight_removed;

        if (_packetV1->status & (1 << 0))
        {
          strcpy(MIBLEsensors[_slot].weight_unit, PSTR("lbs"));
          MIBLEsensors[_slot].weight = (float)_packetV1->weight / 100.0f;
        }
        else if (_packetV1->status & (1 << 4))
        {
          strcpy(MIBLEsensors[_slot].weight_unit, PSTR("jin"));
          MIBLEsensors[_slot].weight = (float)_packetV1->weight / 100.0f;
        }
        else
        {
          strcpy(MIBLEsensors[_slot].weight_unit, PSTR("kg"));
          MIBLEsensors[_slot].weight = (float)_packetV1->weight / 200.0f;
        }

        if (MIBLEsensors[_slot].weight_removed)
        {
          MIBLEsensors[_slot].weight = 0.0f;
        }
        // Can be changed to memcpy or smthng else ?
        MIBLEsensors[_slot].datetime.year = _packetV1->year;
        MIBLEsensors[_slot].datetime.month = _packetV1->month;
        MIBLEsensors[_slot].datetime.day = _packetV1->day;
        MIBLEsensors[_slot].datetime.hour = _packetV1->hour;
        MIBLEsensors[_slot].datetime.minute = _packetV1->minute;
        MIBLEsensors[_slot].datetime.second = _packetV1->second;

        MIBLEsensors[_slot].shallSendMQTT = 1;

        HM10.mode.shallTriggerTele = 1;
      }
    }
  }

  // Mi Scale V2
  // Scales measure weight at first step, after weight is stable - measure impedance.
  //  So measurement (packet) can contain 'weight' or 'weight + impedance'

  else if (_type == 0x181b){ // 17-1-1-2
    MiScaleV2Packet_t *_packetV2 = (MiScaleV2Packet_t*)_buf;
    if ((_slot >= 0) && (_slot < MIBLEsensors.size())){
      DEBUG_SENSOR_LOG(PSTR("HM10: %s: at slot %u"), kHM10DeviceType[MIBLEsensors[_slot].type-1],_slot);

      weight_stabilized = (_packetV2->status & (1 << 5)) ? 1 : 0;
      weight_removed = (_packetV2->status & (1 << 7)) ? 1 : 0;
      impedance_stabilized = (_packetV2->status & (1 << 1)) ? 1 : 0;

      //AddLog(LOG_LEVEL_DEBUG, PSTR("%s: MSCALE: WS %u, WR: %u, IS: %u "),D_CMND_HM10,weight_stabilized,weight_removed,impedance_stabilized);

      // Set sensor values for every packet in BridgedMode and for every packet with stable weight
      if (HM10.option.directBridgeMode || (weight_stabilized && !weight_removed))
      {
        MIBLEsensors[_slot].has_impedance = (_packetV2->status & (1 << 1)) ? 1 : 0;
        MIBLEsensors[_slot].weight_stabilized = weight_stabilized;
        MIBLEsensors[_slot].impedance_stabilized = impedance_stabilized;
        MIBLEsensors[_slot].weight_removed = weight_removed;
        if (_packetV2->weight_unit & (1 << 4))
        {
          strcpy(MIBLEsensors[_slot].weight_unit, PSTR("jin"));
          MIBLEsensors[_slot].weight = (float)_packetV2->weight / 100.0f;
        }
        else if (_packetV2->weight_unit == 3)
        {
          strcpy(MIBLEsensors[_slot].weight_unit, PSTR("lbs"));
          MIBLEsensors[_slot].weight = (float)_packetV2->weight / 100.0f;
        }
        else if (_packetV2->weight_unit == 2)
        {
          strcpy(MIBLEsensors[_slot].weight_unit, PSTR("kg"));
          MIBLEsensors[_slot].weight = (float)_packetV2->weight / 200.0f;
        }
        else
        {
          strcpy(MIBLEsensors[_slot].weight_unit, PSTR(""));
          MIBLEsensors[_slot].weight = (float)_packetV2->weight / 100.0f;
        }

        if (MIBLEsensors[_slot].weight_removed)
        {
          MIBLEsensors[_slot].weight = 0.0f;

        }
        //Set impedance to zero after every stable weight measurement
        MIBLEsensors[_slot].impedance = 0;
        // If impedance stable or BridgeMode - set value
        if (HM10.option.directBridgeMode || impedance_stabilized)
        {
          MIBLEsensors[_slot].impedance = MIBLEsensors[_slot].has_impedance ? _packetV2->impedance: 0;
        }
        // Can be changed to memcpy or smthng else ?
        MIBLEsensors[_slot].datetime.year = _packetV2->year;
        MIBLEsensors[_slot].datetime.month = _packetV2->month;
        MIBLEsensors[_slot].datetime.day = _packetV2->day;
        MIBLEsensors[_slot].datetime.hour = _packetV2->hour;
        MIBLEsensors[_slot].datetime.minute = _packetV2->minute;
        MIBLEsensors[_slot].datetime.second = _packetV2->second;

        MIBLEsensors[_slot].shallSendMQTT = 1;
        //Trigger in all cases - BridgeMode or weight_stabilized
        HM10.mode.shallTriggerTele = 1;
      }

    }
  }
}


void HM10ParseResponse(char *buf, uint16_t bufsize) {
  if (!strncmp(buf,"HMSoft",6)) { //8
    const char* _fw = "000";
    memcpy((void *)_fw,(void *)(buf+8),3);
    HM10.firmware = atoi(_fw);
    DEBUG_SENSOR_LOG(PSTR("%s: Firmware: %d"),D_CMND_HM10, HM10.firmware);
    }
  else if (strstr(buf, "LOST")){
    HM10.current_task_delay = 0;
    HM10.mode.connected = false;
  }
  else if (strstr(buf, "CONNF")){
    HM10.mode.connected = false;
    HM10.current_task_delay = 0;
  }
  else if (strstr(buf, "CONN")){
    HM10.current_task_delay = 0;
  }
  else {
    DEBUG_SENSOR_LOG(PSTR("%s: empty response"),D_CMND_HM10);
  }
}

void HM10readHT_LY(char *_buf){
  // AddLogBuffer(LOG_LEVEL_DEBUG, (uint8_t*)_buf,7);
  if(_buf[0]==0x4f && _buf[1]==0x4b) return; // "OK"
  if(_buf[0] != 0 && _buf[1] != 0){
    LYWSD0x_HT_t *packet = (LYWSD0x_HT_t*)_buf;
    AddLog(LOG_LEVEL_DEBUG, PSTR("%s: T * 100: %u, H: %u"),D_CMND_HM10,packet->temp,packet->hum);
    uint32_t _slot = HM10.state.sensor;

    DEBUG_SENSOR_LOG(PSTR("MIBLE: Sensor slot: %u"), _slot);
    static float _tempFloat;
    _tempFloat=(float)(packet->temp)/100.0f;
    if(_tempFloat<60){
        MIBLEsensors[_slot].temp=_tempFloat;
        HM10.mode.awaiting = none;
        HM10.current_task_delay = 0;
        MIBLEsensors[_slot].showedUp=255; // this sensor is real
    }
    _tempFloat=(float)packet->hum;
    if(_tempFloat<100){
      MIBLEsensors[_slot].hum = _tempFloat;
      DEBUG_SENSOR_LOG(PSTR("LYWSD0x: hum updated"));
    }
    MIBLEsensors[_slot].eventType.tempHum = 1;
    if (MIBLEsensors[_slot].type == HM10_LYWSD03MMC || MIBLEsensors[_slot].type == HM10_MHOC401){
      MIBLEsensors[_slot].bat = ((float)packet->volt-2100.0f)/12.0f;
      MIBLEsensors[_slot].eventType.bat  = 1;
    }
  MIBLEsensors[_slot].shallSendMQTT = 1;
  if(HM10.option.directBridgeMode) HM10.mode.shallTriggerTele = 1;
  }
}

void HM10readHT_CGD1(char *_buf){
  DEBUG_SENSOR_LOG(PSTR("%s: raw data: %x%x%x%x%x%x%x"),D_CMND_HM10,_buf[0],_buf[1],_buf[2],_buf[3],_buf[4],_buf[5],_buf[6]);
  if(_buf[0]==0x4f && _buf[1]==0x4b) return; // "OK"
  if(_buf[0] == 0){
    if(_buf[1]==0 && _buf[2]==0 && _buf[3]==0 && _buf[4]==0) return;
    CGD1_HT_t *_packet = (CGD1_HT_t*)_buf;
    AddLog(LOG_LEVEL_DEBUG, PSTR("%s: T * 100: %u, H * 100: %u"),D_CMND_HM10,_packet->temp,_packet->hum);
    uint32_t _slot = HM10.state.sensor;

    DEBUG_SENSOR_LOG(PSTR("MIBLE: Sensor slot: %u"), _slot);
    static float _tempFloat;
    _tempFloat=(float)(_packet->temp)/100.0f;
    if(_tempFloat<60){
        MIBLEsensors[_slot].temp=_tempFloat;
        HM10.mode.awaiting = none;
        HM10.current_task_delay = 0;
        MIBLEsensors[_slot].showedUp=255; // this sensor is real
    }
    _tempFloat=(float)_packet->hum/100.0f;
    if(_tempFloat<100){
      MIBLEsensors[_slot].hum = _tempFloat;
      DEBUG_SENSOR_LOG(PSTR("CGD1: hum updated"));
    }
    MIBLEsensors[_slot].eventType.tempHum = 1;
    MIBLEsensors[_slot].shallSendMQTT = 1;
    if(HM10.option.directBridgeMode) HM10.mode.shallTriggerTele = 1;
  }
}

void HM10readHT_MJ_HT_V1(char *_buf){
  DEBUG_SENSOR_LOG(PSTR("%s: raw data: %x%x%x%x%x%x%x"),D_CMND_HM10,_buf[0],_buf[1],_buf[2],_buf[3],_buf[4],_buf[5],_buf[6]);
  if(_buf[0]!=0x54 && _buf[1]!=0x3d) return; //"T="
  // T=22.7 H=42.2 (response as ASCII)
  // 0123456789012
  uint32_t _temp = (atoi(_buf+2) * 10) + atoi(_buf+5);
  uint32_t _hum = (atoi(_buf+9) * 10) + atoi(_buf+12);
  AddLog(LOG_LEVEL_DEBUG, PSTR("%s: T * 10: %u, H * 10: %u"),D_CMND_HM10,_temp,_hum);
  uint32_t _slot = HM10.state.sensor;

  DEBUG_SENSOR_LOG(PSTR("MIBLE: Sensor slot: %u"), _slot);
  static float _tempFloat;
  _tempFloat=(float)_temp/10.0f;
  if(_tempFloat<60){
      MIBLEsensors[_slot].temp=_tempFloat;
      HM10.mode.awaiting = none;
      HM10.current_task_delay = 0;
      MIBLEsensors[_slot].showedUp=255; // this sensor is real
  }
  _tempFloat=(float)_hum/10.0f;
  if(_tempFloat<100){
    MIBLEsensors[_slot].hum = _tempFloat;
    DEBUG_SENSOR_LOG(PSTR("MJ_HT_V1: hum updated"));
  }
  MIBLEsensors[_slot].eventType.tempHum = 1;
  MIBLEsensors[_slot].shallSendMQTT = 1;
  if(HM10.option.directBridgeMode) HM10.mode.shallTriggerTele = 1;
}

void HM10readTLMF(char *_buf){
  AddLogBuffer(LOG_LEVEL_DEBUG, (uint8_t*)_buf,16);
  Flora_TLMF_t *_packet = (Flora_TLMF_t*)_buf;
  if(_packet->ID==0xFB003C02){ // this is a magic word ... hopefully independent of FW version
    AddLog(LOG_LEVEL_DEBUG, PSTR("%s: T * 10: %u, L: %u, M: %u, F: %u"),D_CMND_HM10,_packet->temp,_packet->lux,_packet->moist,_packet->fert);
    uint32_t _slot = HM10.state.sensor;
    DEBUG_SENSOR_LOG(PSTR("MIBLE: Sensor slot: %u"), _slot);
    MIBLEsensors[_slot].showedUp=255; // this sensor is real

    static float _tempFloat;
    _tempFloat=(float)(_packet->temp)/10.0f;
    MIBLEsensors[_slot].temp=_tempFloat;

    MIBLEsensors[_slot].lux = _packet->lux;
    MIBLEsensors[_slot].moisture = _packet->moist;
    MIBLEsensors[_slot].fertility = _packet->fert;
    MIBLEsensors[_slot].eventType.temp = 1;
    MIBLEsensors[_slot].eventType.lux = 1;
    MIBLEsensors[_slot].eventType.moist = 1;
    MIBLEsensors[_slot].eventType.fert = 1;
    MIBLEsensors[_slot].shallSendMQTT = 1;
    if(HM10.option.directBridgeMode) HM10.mode.shallTriggerTele = 1;
    HM10.mode.awaiting = none;
    HM10.current_task_delay = 0;
  }
}

bool HM10readBat(char *_buf){
  DEBUG_SENSOR_LOG(PSTR("%s: raw data: %x%x%x%x%x%x%x"),D_CMND_HM10,_buf[0],_buf[1],_buf[2],_buf[3],_buf[4],_buf[5],_buf[6]);
  if(_buf[0]==0x4f && _buf[1]==0x4b) return false; // "OK"
  uint32_t _slot = HM10.state.sensor;
  // if(HM10.option.ignoreBogusBattery){
  //   if (MIBLEsensors[_slot].type == HM10_LYWSD03MMC || MIBLEsensors[_slot].type == HM10_MHOC401) return true;
  // }
  if(_buf[0] != 0){
    AddLog(LOG_LEVEL_DEBUG,PSTR("%s: Battery: %u"),D_CMND_HM10,_buf[0]);
    DEBUG_SENSOR_LOG(PSTR("MIBLE: Sensor slot: %u"), _slot);
    if(_buf[0]<101){
        MIBLEsensors[_slot].bat=_buf[0];
        MIBLEsensors[_slot].showedUp=255; // this sensor is real
        MIBLEsensors[_slot].eventType.bat = 1;
        MIBLEsensors[_slot].shallSendMQTT = 1;
        if(HM10.option.directBridgeMode) HM10.mode.shallTriggerTele = 1;
        return true;
    }
  }
  return false;
}

/*********************************************************************************************\
 * beacon functions
\*********************************************************************************************/

/**
 * @brief Handle a generic BLE advertisment in a running scan or to check a beacon
 *
*
 */
void HM10HandleGenericBeacon(void){
  if(HM10.state.beaconScanCounter==0){ //handle beacon
    for(auto &_beacon : MIBLEbeacons){
      if(memcmp(HM10.rxAdvertisement.MAC,_beacon.MAC,6)==0){
        _beacon.time = 0;
        _beacon.RSSI = HM10.rxAdvertisement.RSSI;
        _beacon.SVC = HM10.rxAdvertisement.SVC;
        _beacon.CID = HM10.rxAdvertisement.CID;
        _beacon.UUID = HM10.rxAdvertisement.UUID;
        _beacon.TX = HM10.rxAdvertisement.TX;
        return;
      }
    }
    return;
  }
  // else handle scan
  if(MIBLEscanResult.size()>19) {
    AddLog(LOG_LEVEL_INFO,PSTR("HM10: Scan buffer full"));
    HM10.state.beaconScanCounter = 1;
    return;
  }
  for(auto _scanResult : MIBLEscanResult){
    if(memcmp(HM10.rxAdvertisement.MAC,_scanResult.MAC,6)==0){
      // AddLog(LOG_LEVEL_INFO,PSTR("HM10: known device"));
      return;
    }
  }
  MIBLEscanResult.push_back(HM10.rxAdvertisement);
}

/**
 * @brief Add a beacon defined by its MAC-address, if only zeros are given, the beacon will be deactivated
 *
 * @param index 1-4 beacons are currently supported
 * @param data  null terminated char array representing a MAC-address in hex
 */
void HM10addBeacon(uint8_t index, char* data){
  auto &_new = MIBLEbeacons[index-1]; //TODO: check
  HM10HexStringToBytes(data,_new.MAC);
  char _MAC[18];
  ToHex_P(MIBLEbeacons[index-1].MAC,6,_MAC,18,':');
  char _empty[6] = {0};
  _new.time = 0;
  if(memcmp(_empty,_new.MAC,6) == 0){
    _new.active = false;
    AddLog(LOG_LEVEL_INFO,PSTR("HM10: beacon%u deactivated"), index);
  }
  else{
    _new.active = true;
    HM10.mode.activeBeacon = 1;
    AddLog(LOG_LEVEL_INFO,PSTR("HM10: beacon added with MAC: %s"), _MAC);
  }
}

/**
 * @brief Present BLE scan in the console, after that deleting the scan data
 *
 */
void HM10showScanResults(){
  size_t _size = MIBLEscanResult.size();
  ResponseAppend_P(PSTR(",\"BLEScan\":{\"Found\":%u,\"Devices\":["), _size);
  bool add_comma = false;
  for(auto _scanResult : MIBLEscanResult){
    char _MAC[18];
    ToHex_P(_scanResult.MAC,6,_MAC,18,':');
    ResponseAppend_P(PSTR("%s{\"MAC\":\"%s\",\"CID\":\"0x%04x\",\"SVC\":\"0x%04x\",\"UUID\":\"0x%04x\",\"RSSI\":%d}"),
      (add_comma)?",":"", _MAC, _scanResult.CID, _scanResult.SVC, _scanResult.UUID, _scanResult.RSSI);
    add_comma = true;
  }
  ResponseAppend_P(PSTR("]}"));
  MIBLEscanResult.clear();
  HM10.mode.shallShowScanResult = 0;
}

void HM10showBlockList(){
  ResponseAppend_P(PSTR(",\"Block\":["));
  bool add_comma = false;
  for(auto _scanResult : MIBLEBlockList){
    char _MAC[18];
    ToHex_P(_scanResult.buf,6,_MAC,18,':');
    ResponseAppend_P(PSTR("%s\"%s\""), (add_comma)?",":"", _MAC);
    add_comma = true;
  }
  ResponseAppend_P(PSTR("]"));
  HM10.mode.shallShowBlockList = 0;
}

bool HM10isInBlockList(uint8_t* MAC){
  bool isBlocked = false;
  for(auto &_blockedMAC : MIBLEBlockList){
    if(memcmp(_blockedMAC.buf,MAC,6) == 0) isBlocked = true;
  }
  return isBlocked;
}

void HM10removeMIBLEsensor(uint8_t* MAC){
  MIBLEsensors.erase( std::remove_if( MIBLEsensors.begin() , MIBLEsensors.end(), [MAC]( mi_sensor_t _sensor )->bool
  { return (memcmp(_sensor.MAC,MAC,6) == 0); }
  ), end( MIBLEsensors ) );
}
/*********************************************************************************************\
 * handle the return value from the HM10
\*********************************************************************************************/

bool HM10SerialHandleFeedback(){                  // every 50 milliseconds
  bool success    = false;
  uint32_t i       = 0;
  uint32_t _targetsize = 64; //set to some save value
  bool _isPotentialAdv = false;
  bool _isValidAdv = false;
  char* _rx = HM10.rxBuffer; // we start always with the buffer
  uint32_t _nextStep = 2; // we can expect length and AD type at the first two positions anyway
  uint8_t length_type[64]; // length,AD-type,buffer - buffer should be too large
  while(HM10Serial->available() && (_targetsize!=i)) {
    *_rx= HM10Serial->read();
    if(i==18){
      if(memcmp(HM10.rxBuffer+4,"ISA:",4)==0){ //last 4 bytes of "OK+DISA:" should be safe enough
        // AddLog(LOG_LEVEL_DEBUG, PSTR("%s packet size: %u"),D_CMND_HM10,HM10.rxBuffer[16]);
        _targetsize = HM10.rxBuffer[16] + 19; // this is the size byte according to HM-10 docs
        if(_targetsize>64) _targetsize=64;
        memcpy(HM10.rxAdvertisement.MAC,HM10.rxBuffer+8,6);
        HM10_ReverseMAC(HM10.rxAdvertisement.MAC);
        HM10.rxAdvertisement.RSSI = (256 - HM10.rxBuffer[15]) * -1;
        length_type[0] = HM10.rxBuffer[17]; //length
        length_type[1] = HM10.rxBuffer[18]; //AD-type
        _isPotentialAdv = true;
      }
    }

    if(_isPotentialAdv){ // we will change the pointer now according to the AD-type and data length
      if(_nextStep>length_type[0]){
        if(length_type[1]==0xff){ //we only want the CID from the custom data 0xff ...
          memcpy((uint8_t*)&HM10.rxAdvertisement.CID,length_type+2,2); // ... and leave the rest unused in the buffer
        }
        _nextStep=0;
        _rx = (char*)&length_type - 1;
      }
      else if(_nextStep == 2){
        switch(length_type[1]){ //AD type
          case 0x02: case 0x03:
            _rx = (char*)&HM10.rxAdvertisement.UUID - 1;
            break;
          case 0x0a:
            if(length_type[0] == 0xd) {
              if(_targetsize-i == 1)_isValidAdv = true; // expected trailing bytes for a valid packet
            }
            _rx = (char*)&HM10.rxAdvertisement.TX - 1;
            break;
          case 0x16:
            _rx = (char*)HM10.rxAdvertisement.svcData - 1;
            break;
        }
    }
    _nextStep++;
    }
    i++;
    _rx++;
    success = true;
  }

  if(i==0){
    if(HM10.mode.shallTriggerTele){ // let us use the spare time for other things
      HM10.mode.shallTriggerTele=0;
      HM10triggerTele();
    }
    return success;
  }

  switch (HM10.mode.awaiting){
    case bat:
      if (HM10.mode.connected) {
        if (HM10readBat(HM10.rxBuffer)){
          HM10.mode.awaiting = none;
          HM10.current_task_delay = 0;
        }
      }
      break;
    case tempHumLY:
      if (HM10.mode.connected) HM10readHT_LY(HM10.rxBuffer);
      break;
    case tempHumCGD1:
      if (HM10.mode.connected) HM10readHT_CGD1(HM10.rxBuffer);
      break;
    case TLMF:
      if (HM10.mode.connected) HM10readTLMF(HM10.rxBuffer);
      break;
    case discScan:
      if(_isValidAdv) {
          if(HM10.state.beaconScanCounter!=0 || HM10.mode.activeBeacon){
            HM10HandleGenericBeacon();
          }
          uint16_t _type = (uint8_t)HM10.rxAdvertisement.svcData[5]*256 + (uint8_t)HM10.rxAdvertisement.svcData[4];
          // AddLog(LOG_LEVEL_DEBUG, PSTR("%04x %02x %04x %04x %04x"),HM10.rxAdvertisement.UUID,HM10.rxAdvertisement.TX,HM10.rxAdvertisement.CID,HM10.rxAdvertisement.SVC, _type);
          DEBUG_SENSOR_LOG(PSTR("HM10: UUID %04x, TX: %02x, CID: %04x, SVC: %04x"), HM10.rxAdvertisement.UUID,HM10.rxAdvertisement.TX,HM10.rxAdvertisement.CID,HM10.rxAdvertisement.SVC);
          if(HM10.rxAdvertisement.SVC==0x181a) _type = 0xa1c;
          else if(HM10.rxAdvertisement.SVC==0xfdcd) _type = 0x0576;
          else if(HM10.rxAdvertisement.SVC==0x181b || HM10.rxAdvertisement.SVC==0x181d) _type = 0x181b;
          uint16_t _slot = MIBLEgetSensorSlot(HM10.rxAdvertisement.MAC, _type, HM10.rxAdvertisement.RSSI);
          if(_slot!=0xff){
            if (_type==0xa1c) HM10parseATC((char*)HM10.rxAdvertisement.svcData+2,_slot);
            else if (_type==0x0576) HM10parseCGD1Packet((char*)HM10.rxAdvertisement.svcData+2,_slot);
            else if (_type==0x181b) HM10ParseMiScalePacket((char*)HM10.rxAdvertisement.svcData+2,_slot, HM10.rxAdvertisement.SVC);
            else HM10parseMiBeacon((char*)HM10.rxAdvertisement.svcData+2,_slot);
          }
          else{
            // AddLogBuffer(LOG_LEVEL_INFO,(uint8_t*)HM10.rxAdvertisement.svcData,16);
          }
      }
    break;
    case tempHumMJ:
      if (HM10.mode.connected) HM10readHT_MJ_HT_V1(HM10.rxBuffer);
      break;
    case none:
      if(success) {
        AddLog(LOG_LEVEL_DEBUG, PSTR("%s: response: %s"),D_CMND_HM10, (char *)HM10.rxBuffer);
        // for(uint32_t j = 0; j<i+1; j+=8){
        //   AddLog(LOG_LEVEL_DEBUG, PSTR("%02x %02x %02x %02x %02x %02x %02x %02x"),(uint8_t)ret[j],(uint8_t)ret[j+1],(uint8_t)ret[j+2],(uint8_t)ret[j+3],(uint8_t)ret[j+4],(uint8_t)ret[j+5],(uint8_t)ret[j+6],(uint8_t)ret[j+7]);
        // }
        HM10ParseResponse(HM10.rxBuffer,i);
      }
    break;
  }
  memset(HM10.rxBuffer,0,i); // wipe away the recent bytes
  if(_isPotentialAdv){
    memset((void*)&HM10.rxAdvertisement,0,sizeof(HM10.rxAdvertisement));
  }
  return success;
}

/*********************************************************************************************\
 * execute the next Task
\*********************************************************************************************/

void HM10_TaskEvery100ms(){
  if (HM10.current_task_delay == 0)  {
    uint8_t i = 0;
    bool runningTaskLoop = true;
    while (runningTaskLoop) {                                        // always iterate through the whole task list
      switch(HM10_TASK_LIST[i][0]) {                                 // handle the kind of task
        case TASK_HM10_ROLE1:
          AddLog(LOG_LEVEL_DEBUG, PSTR("%s: set role to 1"),D_CMND_HM10);
          HM10.current_task_delay = 5;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write("AT+ROLE1");
          break;
        case TASK_HM10_IMME1:
          AddLog(LOG_LEVEL_DEBUG, PSTR("%s: set imme to 1"),D_CMND_HM10);
          HM10.current_task_delay = 5;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write("AT+IMME1");
          break;
        case TASK_HM10_DISC:
          AddLog(LOG_LEVEL_DEBUG, PSTR("%s: start discovery"),D_CMND_HM10);
          HM10.current_task_delay = 90;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10.mode.awaiting = discScan;
          HM10Serial->write("AT+DISA?");
          break;
        case TASK_HM10_VERSION:
          AddLog(LOG_LEVEL_DEBUG, PSTR("%s: read version"),D_CMND_HM10);
          HM10.current_task_delay = 5;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write("AT+VERR?");
          break;
        case TASK_HM10_NAME:
          AddLog(LOG_LEVEL_DEBUG, PSTR("%s: read name"),D_CMND_HM10);
          HM10.current_task_delay = 5;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write("AT+NAME?");
          break;
        case TASK_HM10_CONN:
          char _con[20];
          sprintf_P(_con,"AT+CON%02x%02x%02x%02x%02x%02x",MIBLEsensors[HM10.state.sensor].MAC[0],MIBLEsensors[HM10.state.sensor].MAC[1],MIBLEsensors[HM10.state.sensor].MAC[2],MIBLEsensors[HM10.state.sensor].MAC[3],MIBLEsensors[HM10.state.sensor].MAC[4],MIBLEsensors[HM10.state.sensor].MAC[5]);
          AddLog(LOG_LEVEL_DEBUG, PSTR("%s: %s connect %s"),D_CMND_HM10,kHM10DeviceType[MIBLEsensors[HM10.state.sensor].type-1],_con);
          HM10.current_task_delay = 2;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write(_con);
          HM10.mode.awaiting = none;
          HM10.mode.connected = true;
          break;
        case TASK_HM10_DISCONN:
          AddLog(LOG_LEVEL_DEBUG, PSTR("%s: disconnect"),D_CMND_HM10);
          HM10.current_task_delay = 5;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write("AT");
          break;
        case TASK_HM10_RESET:
          AddLog(LOG_LEVEL_DEBUG, PSTR("%s: Reset Device"),D_CMND_HM10);
          HM10Serial->write("AT+RESET");
          HM10.current_task_delay = 5;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          break;
        case TASK_HM10_SUB_L3:
          AddLog(LOG_LEVEL_DEBUG, PSTR("%s: subscribe"),D_CMND_HM10);
          HM10.current_task_delay = 25;                   // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          HM10.mode.awaiting = tempHumLY;
          runningTaskLoop = false;
          HM10Serial->write("AT+NOTIFY_ON0037");
          break;
        case TASK_HM10_UN_L3:
          AddLog(LOG_LEVEL_DEBUG, PSTR("%s: un-subscribe"),D_CMND_HM10);
          HM10.current_task_delay = 5;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10.mode.awaiting = none;
          HM10Serial->write("AT+NOTIFYOFF0037");
          break;
        case TASK_HM10_SUB_L2:
          AddLog(LOG_LEVEL_DEBUG, PSTR("%s: subscribe"),D_CMND_HM10);
          HM10.current_task_delay = 85;                   // set task delay
          HM10.mode.awaiting = tempHumLY;
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          if(MIBLEsensors[HM10.state.sensor].type == HM10_LYWSD02) HM10Serial->write("AT+NOTIFY_ON003C");
          else HM10Serial->write("AT+NOTIFY_ON004B"); //MHO-C303
          break;
        case TASK_HM10_UN_L2:
          AddLog(LOG_LEVEL_DEBUG, PSTR("%s: un-subscribe"),D_CMND_HM10);
          HM10.current_task_delay = 5;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10.mode.awaiting = none;
          if(MIBLEsensors[HM10.state.sensor].type == HM10_LYWSD02) HM10Serial->write("AT+NOTIFY_OFF003C");
          else HM10Serial->write("AT+NOTIFY_OFF004B"); //MHO-C303
          break;
        case TASK_HM10_TIME_L2:
          AddLog(LOG_LEVEL_DEBUG, PSTR("%s: set time"),D_CMND_HM10);
          HM10.current_task_delay = 5;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10.time = Rtc.utc_time;
          HM10Serial->write("AT+SEND_DATAWR002F");
          HM10Serial->write(HM10.timebuf,4);
          HM10Serial->write(Rtc.time_timezone / 60);
          AddLog(LOG_LEVEL_DEBUG,PSTR("%s Time-string: %x%x%x%x%x"),D_CMND_HM10, HM10.timebuf[0],HM10.timebuf[1],HM10.timebuf[2],HM10.timebuf[3],(Rtc.time_timezone /60));
          break;
        // case TASK_HM10_READ_BT_L3:
        //   AddLog(LOG_LEVEL_DEBUG, PSTR("%s: read handle 003A"),D_CMND_HM10);
        //   HM10.current_task_delay = 2;                    // set task delay
        //   HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
        //   runningTaskLoop = false;
        //   HM10Serial->write("AT+READDATA003A?");
        //   HM10.mode.awaiting = bat;
        //   break;
        case TASK_HM10_READ_BT_L2:
          AddLog(LOG_LEVEL_DEBUG, PSTR("%s: read handle 0043"),D_CMND_HM10);
          HM10.current_task_delay = 2;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          if(MIBLEsensors[HM10.state.sensor].type == HM10_LYWSD02) HM10Serial->write("AT+READDATA0043?");
          else HM10Serial->write("AT+READDATA0052?"); //MHO-C303
          HM10.mode.awaiting = bat;
          break;
        case TASK_HM10_READ_BF_FL:
          AddLog(LOG_LEVEL_DEBUG, PSTR("%s: read handle 0038"),D_CMND_HM10);
          HM10.current_task_delay = 2;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write("AT+READDATA0038?");
          HM10.mode.awaiting = bat;
          break;
        case TASK_HM10_CALL_TLMF_FL:
          AddLog(LOG_LEVEL_DEBUG, PSTR("%s: write to handle 0033"),D_CMND_HM10);
          HM10.current_task_delay = 5;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_READ_TLMF_FL,i);
          runningTaskLoop = false;
          HM10Serial->write("AT+SEND_DATAWR0033");
          HM10Serial->write(0xa0);
          HM10Serial->write(0x1f);
          HM10.mode.awaiting = none;
          break;
        case TASK_HM10_READ_TLMF_FL:
          AddLog(LOG_LEVEL_DEBUG, PSTR("%s: read handle 0035"),D_CMND_HM10);
          HM10.current_task_delay = 2;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write("AT+READDATA0035?");
          HM10.mode.awaiting = TLMF;
          break;
        case TASK_HM10_READ_B_CGD1:
          AddLog(LOG_LEVEL_DEBUG, PSTR("%s: read handle 0011"),D_CMND_HM10);
          HM10.current_task_delay = 2;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write("AT+READDATA0011?");
          HM10.mode.awaiting = bat;
          break;
        case TASK_HM10_SUB_HT_CGD1:
          AddLog(LOG_LEVEL_DEBUG, PSTR("%s: subscribe 4b"),D_CMND_HM10);
          HM10.current_task_delay = 5;                   // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10.mode.awaiting = tempHumCGD1;
          HM10Serial->write("AT+NOTIFY_ON004b");
          break;
        case TASK_HM10_UN_HT_CGD1:
          AddLog(LOG_LEVEL_DEBUG, PSTR("%s: un-subscribe 4b"),D_CMND_HM10);
          HM10.current_task_delay = 5;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10.mode.awaiting = none;
          HM10Serial->write("AT+NOTIFYOFF004b");
          break;
        case TASK_HM10_SCAN9:
          AddLog(LOG_LEVEL_DEBUG, PSTR("%s: scan time to 9"),D_CMND_HM10);
          HM10.current_task_delay = 2;                  // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write("AT+SCAN9");
          break;
        case TASK_HM10_READ_B_MJ:
          AddLog(LOG_LEVEL_DEBUG, PSTR("%s: read handle 0x18"),D_CMND_HM10);
          HM10.current_task_delay = 2;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write("AT+READDATA0018?");
          HM10.mode.awaiting = bat;
          break;
        case TASK_HM10_SUB_HT_MJ:
          AddLog(LOG_LEVEL_DEBUG, PSTR("%s: subscribe to 0x0f"),D_CMND_HM10);
          HM10.current_task_delay = 10;                   // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write("AT+NOTIFY_ON000F");
          HM10.mode.awaiting = tempHumMJ;
          break;
        case TASK_HM10_FEEDBACK:
          AddLog(LOG_LEVEL_DEBUG, PSTR("%s: get response"),D_CMND_HM10);
          HM10SerialHandleFeedback();
          HM10.current_task_delay = HM10_TASK_LIST[i+1][1];;     // set task delay
          HM10_TASK_LIST[i][0] = TASK_HM10_DONE;                 // no feedback for reset
          runningTaskLoop = false;
          break;
        case TASK_HM10_STATUS_EVENT:
          AddLog(LOG_LEVEL_DEBUG, PSTR("%s: show status"),D_CMND_HM10);
          HM10StatusInfo();
          HM10.current_task_delay = HM10_TASK_LIST[i+1][1];;     // set task delay
          HM10_TASK_LIST[i][0] = TASK_HM10_DONE;                 // no feedback for reset
          runningTaskLoop = false;
          break;
        case TASK_HM10_DONE:                                    // this entry was already handled
          // AddLog(LOG_LEVEL_DEBUG, PSTR("%sFound done HM10_TASK"),D_CMND_HM10);
          // AddLog(LOG_LEVEL_DEBUG, PSTR("%snext slot:%u, i: %u"),D_CMND_HM10, HM10_TASK_LIST[i+1][0],i);
          if(HM10_TASK_LIST[i+1][0] == TASK_HM10_NOTASK) {             // check the next entry and if there is none
            DEBUG_SENSOR_LOG(PSTR("%s: no Tasks left"),D_CMND_HM10);
            DEBUG_SENSOR_LOG(PSTR("%s: HM10_TASK_DONE current slot %u"),D_CMND_HM10, i);
            for (uint8_t j = 0; j < HM10_MAX_TASK_NUMBER+1; j++) {   // do a clean-up:
              DEBUG_SENSOR_LOG(PSTR("%s: HM10_TASK cleanup slot %u"),D_CMND_HM10, j);
              HM10_TASK_LIST[j][0] = TASK_HM10_NOTASK;                // reset all task entries
              HM10_TASK_LIST[j][1] = 0;                              // reset all delays
            }
            runningTaskLoop = false;                                  // return to main loop
            HM10.mode.pending_task = 0;                               // back to main loop control
            break;
          }
      }
      i++;
    }
  }
  else {
    HM10.current_task_delay--;               // count down every 100 ms
  }
}

void HM10StatusInfo() {
/*
  char stemp[20];
  snprintf_P(stemp, sizeof(stemp),PSTR("{%s:{\"found\": %u}}"),D_CMND_HM10, MIBLEsensors.size());
  AddLog(LOG_LEVEL_INFO, stemp);
  RulesProcessEvent(stemp);
*/
  Response_P(PSTR("{\"%s\":{\"found\":%u}}"), D_CMND_HM10, MIBLEsensors.size());
  XdrvRulesProcess(0);
}

/**
 * @brief Main loop of the driver, "high level"-loop
 *
 */

void HM10EverySecond(bool restart){
  static uint32_t _counter = 0;
  static uint32_t _nextSensorSlot = 0;
  static uint32_t _lastDiscovery = 0;

  if(restart){
    _counter = 0;
    _lastDiscovery = 0;
    return;
  }

  uint32_t _idx = 0;
  uint32_t _activeBeacons = 0;
  for (auto &_beacon : MIBLEbeacons){
    _idx++;
    if(_beacon.active == false) continue;
    _activeBeacons++;
    _beacon.time++;
    Response_P(PSTR("{\"Beacon%u\":{\"Time\":%u}}"), _idx, _beacon.time);
    XdrvRulesProcess(0);
  }
  if(_activeBeacons==0) HM10.mode.activeBeacon = 0;

  if(HM10.state.beaconScanCounter!=0){
    HM10.state.beaconScanCounter--;
    if(HM10.state.beaconScanCounter==0){
      HM10.mode.shallShowScanResult = 1;
      HM10triggerTele();
    }
  }

  if(HM10.firmware == 0) return;
  if(HM10.mode.pending_task == 1) return;
  if(MIBLEsensors.size()==0 && !HM10.mode.autoScan) return;

  if((HM10.period-_counter)>15 && HM10.mode.autoScan) {
    if(_counter-_lastDiscovery>HM10.autoScanInterval){
      HM10_Discovery_Scan();
      HM10.mode.pending_task = 1;
      _counter+=12;
      _lastDiscovery = _counter;
      return;
    }
  }

  if(_counter==0) {
    if(MIBLEsensors.size() == 0){
      _counter++;
      return;
    }
    HM10.state.sensor = _nextSensorSlot;
    _nextSensorSlot++;
    HM10.mode.pending_task = 1;
    switch(MIBLEsensors[HM10.state.sensor].type){
      case HM10_FLORA:
        HM10_Read_Flora();
        break;
      case HM10_MJ_HT_V1: case HM10_CGG1:
        HM10_Read_MJ_HT_V1();
        break;
      case HM10_LYWSD02: case HM10_MHOC303:
        HM10_Read_LYWSD02();
        break;
      case HM10_LYWSD03MMC: case HM10_MHOC401:
        HM10_Read_LYWSD03();
        break;
      case HM10_CGD1:
        HM10_Read_CGD1();
        break;
      default:
        HM10.mode.pending_task = 0;
    }
    if (HM10.state.sensor==MIBLEsensors.size()-1) {
      _nextSensorSlot= 0;
      _counter++;
    }
    DEBUG_SENSOR_LOG(PSTR("%s: active sensor now: %u"),D_CMND_HM10, HM10.state.sensor);
  }
  else _counter++;
  if (_counter>HM10.period) {
    _counter = 0;
    _lastDiscovery = 0;
  }
}

/**
 * @brief trigger real-time message
 *
 */
void HM10triggerTele(void){
  HM10.mode.triggeredTele = 1;
  MqttPublishTeleperiodSensor();
}


/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void CmndHM10Period() {
  if (XdrvMailbox.data_len > 0) {
    if (XdrvMailbox.payload==1) {
      HM10EverySecond(true);
      XdrvMailbox.payload = HM10.period;
      }
    else {
      HM10.period = XdrvMailbox.payload;
    }
  }
  else {
    XdrvMailbox.payload = HM10.period;
  }
  ResponseCmndNumber(HM10.period);
}

void CmndHM10Auto() {
  if (XdrvMailbox.data_len > 0) {
    if (XdrvMailbox.payload>0) {
      HM10.mode.autoScan = 1;
      HM10.autoScanInterval = XdrvMailbox.payload;
    }
    else {
      HM10.mode.autoScan = 0;
      HM10.autoScanInterval = 0;
    }
  }
  else {
    XdrvMailbox.payload = HM10.autoScanInterval;
  }
  ResponseCmndNumber(HM10.autoScanInterval);
}

void CmndHM10Baud() {
  if (XdrvMailbox.data_len > 0) {
      HM10.serialSpeed = XdrvMailbox.payload;
      HM10Serial->begin(HM10.serialSpeed);
  }
  else {
    XdrvMailbox.payload = HM10.serialSpeed;
  }
  ResponseCmndNumber(HM10.serialSpeed);
}

void CmndHM10Time() {
  if (XdrvMailbox.data_len > 0) {
    if(MIBLEsensors.size()>XdrvMailbox.payload){
      if(MIBLEsensors[XdrvMailbox.payload].type == HM10_LYWSD02){
        HM10.state.sensor = XdrvMailbox.payload;
        HM10_Time_LYWSD02();
        }
      }
    }
  ResponseCmndNumber(XdrvMailbox.payload);
}

void CmndHM10Page() {
  if (XdrvMailbox.data_len > 0) {
      if (XdrvMailbox.payload == 0) XdrvMailbox.payload = HM10.perPage; // ignore 0
      HM10.perPage = XdrvMailbox.payload;
    }
  else XdrvMailbox.payload = HM10.perPage;
  ResponseCmndNumber(HM10.perPage);
}

void CmndHM10AT() {
  HM10Serial->write("AT");               // without an argument this will disconnect
  if (strlen(XdrvMailbox.data)!=0) {
    HM10Serial->write("+");
    HM10Serial->write(XdrvMailbox.data); // pass everything without checks
    Response_P(S_JSON_HM10_COMMAND, ":AT+",XdrvMailbox.data);
  }
  else Response_P(S_JSON_HM10_COMMAND, ":AT",XdrvMailbox.data);
}

void CmndHM10Scan() {
  HM10_Discovery_Scan();
  ResponseCmndDone();
}

void CmndHM10Beacon() {
  if (XdrvMailbox.data_len == 0) {
    switch(XdrvMailbox.index){
      case 0:
      HM10.state.beaconScanCounter = 8;
      ResponseCmndIdxChar(PSTR("Scanning..."));
      break;
      case 1: case 2: case 3: case 4:
      char _MAC[18];
      ResponseCmndIdxChar(ToHex_P(MIBLEbeacons[XdrvMailbox.index-1].MAC, 6, _MAC, 18, ':'));
      break;
    }
  } else {
    if(XdrvMailbox.data_len == 12 || XdrvMailbox.data_len == 17){ // MAC-string without or with colons
      switch(XdrvMailbox.index){
        case 1: case 2: case 3: case 4:
        HM10addBeacon(XdrvMailbox.index,XdrvMailbox.data);
        break;
      }
    }
  ResponseCmndIdxChar(XdrvMailbox.data);
  }
}

void CmndHM10Block(void){
  if (XdrvMailbox.data_len == 0) {
    switch (XdrvMailbox.index) {
      case 0:
        MIBLEBlockList.clear();
        // AddLog(LOG_LEVEL_INFO,PSTR("HM10: size of ilist: %u"), MIBLEBlockList.size());
        ResponseCmndIdxChar(PSTR("block list cleared"));
        break;
      case 1:
        ResponseCmndIdxChar(PSTR("show block list"));
        break;
    }
  }
  else {
    MAC_t _MACasBytes;
    HM10HexStringToBytes(XdrvMailbox.data,_MACasBytes.buf);
    switch (XdrvMailbox.index) {
      case 0:
        MIBLEBlockList.erase( std::remove_if( begin( MIBLEBlockList ), end( MIBLEBlockList ), [_MACasBytes]( MAC_t& _entry )->bool
          { return (memcmp(_entry.buf,_MACasBytes.buf,6) == 0); }
          ), end( MIBLEBlockList ) );
        ResponseCmndIdxChar(PSTR("MAC not blocked anymore"));
        break;
      case 1:
        bool _notYetInList = true;
        for (auto &_entry : MIBLEBlockList) {
          if (memcmp(_entry.buf,_MACasBytes.buf,6) == 0){
            _notYetInList = false;
          }
        }
        if (_notYetInList) {
          MIBLEBlockList.push_back(_MACasBytes);
          ResponseCmndIdxChar(XdrvMailbox.data);
          HM10removeMIBLEsensor(_MACasBytes.buf);
        }
        // AddLog(LOG_LEVEL_INFO,PSTR("HM10: size of ilist: %u"), MIBLEBlockList.size());
        break;
    }
  }
  HM10.mode.shallShowBlockList = 1;
  HM10triggerTele();
}

void CmndHM10Option(void){
  bool onOff = atoi(XdrvMailbox.data);
  switch(XdrvMailbox.index) {
    case 0:
      HM10.option.allwaysAggregate = onOff;
      break;
    case 1:
      HM10.option.noSummary = onOff;
      break;
    case 2:
      HM10.option.directBridgeMode = onOff;
      break;
  }
  ResponseCmndDone();
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

const char HTTP_HM10[] PROGMEM = "{s}HM10 FW%u   V0960{m}%u%s / %u{e}";
const char HTTP_HM10_MAC[] PROGMEM = "{s}%s %s{m}%s{e}";
const char HTTP_BATTERY[] PROGMEM = "{s}%s" " Battery" "{m}%u%%{e}";
const char HTTP_RSSI[] PROGMEM = "{s}%s " D_RSSI "{m}%d dBm{e}";
const char HTTP_HM10_FLORA_DATA[] PROGMEM = "{s}%s" " Fertility" "{m}%u us/cm{e}";
const char HTTP_HM10_HL[] PROGMEM = "{s}<hr>{m}<hr>{e}";

void HM10ShowContinuation(bool *commaflg) {
  if (*commaflg) {
    ResponseAppend_P(PSTR(","));
  } else {
    *commaflg = true;
  }
}

void HM10Show(bool json)
{
  if (json) {
    if(HM10.mode.shallShowScanResult) {
      return HM10showScanResults();
    }
    else if(HM10.mode.shallShowBlockList) {
      return HM10showBlockList();
    }
#ifdef USE_HOME_ASSISTANT
    bool _noSummarySave = HM10.option.noSummary;
    bool _minimalSummarySave = HM10.option.minimalSummary;
    if(hass_mode==2){
      if(HM10.option.holdBackFirstAutodiscovery){
        if(!HM10.mode.firstAutodiscoveryDone){
          HM10.mode.firstAutodiscoveryDone = 1;
          return;
        }
      }
      HM10.option.noSummary = false;
      HM10.option.minimalSummary = false;
    }
#endif //USE_HOME_ASSISTANT
    if(!HM10.mode.triggeredTele){
      if(HM10.option.noSummary) return; // no message at TELEPERIOD
      }

    for (uint32_t i = 0; i < MIBLEsensors.size(); i++) {
      if(HM10.mode.triggeredTele && MIBLEsensors[i].eventType.raw == 0) continue;
      if(HM10.mode.triggeredTele && MIBLEsensors[i].shallSendMQTT==0) continue;

      bool commaflg = false;
      ResponseAppend_P(PSTR(",\"%s-%02x%02x%02x\":{"),
        kHM10DeviceType[MIBLEsensors[i].type-1],
        MIBLEsensors[i].MAC[3], MIBLEsensors[i].MAC[4], MIBLEsensors[i].MAC[5]);

      if((!HM10.mode.triggeredTele && !HM10.option.minimalSummary)||HM10.mode.triggeredTele){
        bool tempHumSended = false;
        if(MIBLEsensors[i].feature.tempHum){
          if(MIBLEsensors[i].eventType.tempHum || !HM10.mode.triggeredTele || HM10.option.allwaysAggregate){
            if (!isnan(MIBLEsensors[i].hum) && !isnan(MIBLEsensors[i].temp)
#ifdef USE_HOME_ASSISTANT
              ||(hass_mode!=-1)
#endif //USE_HOME_ASSISTANT
            ) {
              HM10ShowContinuation(&commaflg);
              ResponseAppendTHD(MIBLEsensors[i].temp, MIBLEsensors[i].hum);
              tempHumSended = true;
            }
          }
        }
        if(MIBLEsensors[i].feature.temp && !tempHumSended){
          if(MIBLEsensors[i].eventType.temp || !HM10.mode.triggeredTele || HM10.option.allwaysAggregate) {
            if (!isnan(MIBLEsensors[i].temp)
#ifdef USE_HOME_ASSISTANT
              ||(hass_mode!=-1)
#endif //USE_HOME_ASSISTANT
            ) {
              HM10ShowContinuation(&commaflg);
              ResponseAppend_P(PSTR("\"" D_JSON_TEMPERATURE "\":%*_f"),
                Settings->flag2.temperature_resolution, &MIBLEsensors[i].temp);
            }
          }
        }
        if(MIBLEsensors[i].feature.hum && !tempHumSended){
          if(MIBLEsensors[i].eventType.hum || !HM10.mode.triggeredTele || HM10.option.allwaysAggregate) {
            if (!isnan(MIBLEsensors[i].hum)
#ifdef USE_HOME_ASSISTANT
              ||(hass_mode!=-1)
#endif //USE_HOME_ASSISTANT
            ) {
              char hum[FLOATSZ];
              dtostrfd(MIBLEsensors[i].hum, Settings->flag2.humidity_resolution, hum);
              HM10ShowContinuation(&commaflg);
              ResponseAppend_P(PSTR("\"" D_JSON_HUMIDITY "\":%s"), hum);
            }
          }
        }
        if (MIBLEsensors[i].feature.lux){
          if(MIBLEsensors[i].eventType.lux || !HM10.mode.triggeredTele || HM10.option.allwaysAggregate){
#ifdef USE_HOME_ASSISTANT
            if ((hass_mode != -1) && (MIBLEsensors[i].lux == 0x0ffffff)) {
              HM10ShowContinuation(&commaflg);
              ResponseAppend_P(PSTR("\"" D_JSON_ILLUMINANCE "\":null"));
            } else
#endif //USE_HOME_ASSISTANT
            if ((MIBLEsensors[i].lux != 0x0ffffff)
#ifdef USE_HOME_ASSISTANT
              || (hass_mode != -1)
#endif //USE_HOME_ASSISTANT
            ) { // this is the error code -> no lux
              HM10ShowContinuation(&commaflg);
              ResponseAppend_P(PSTR("\"" D_JSON_ILLUMINANCE "\":%u"), MIBLEsensors[i].lux);
            }
          }
        }
        if (MIBLEsensors[i].feature.moist){
          if(MIBLEsensors[i].eventType.moist || !HM10.mode.triggeredTele || HM10.option.allwaysAggregate){
#ifdef USE_HOME_ASSISTANT
            if ((hass_mode != -1) && (MIBLEsensors[i].moisture == 0xff)) {
              HM10ShowContinuation(&commaflg);
              ResponseAppend_P(PSTR("\"" D_JSON_MOISTURE "\":null"));
            } else
#endif //USE_HOME_ASSISTANT
            if ((MIBLEsensors[i].moisture != 0xff)
#ifdef USE_HOME_ASSISTANT
              || (hass_mode != -1)
#endif //USE_HOME_ASSISTANT
            ) {
              HM10ShowContinuation(&commaflg);
              ResponseAppend_P(PSTR("\"" D_JSON_MOISTURE "\":%u"), MIBLEsensors[i].moisture);
            }
          }
        }
        if (MIBLEsensors[i].feature.fert){
          if(MIBLEsensors[i].eventType.fert || !HM10.mode.triggeredTele || HM10.option.allwaysAggregate){
#ifdef USE_HOME_ASSISTANT
            if ((hass_mode != -1) && (MIBLEsensors[i].fertility == 0xffff)) {
              HM10ShowContinuation(&commaflg);
              ResponseAppend_P(PSTR("\"Fertility\":null"));
            } else
#endif //USE_HOME_ASSISTANT
            if ((MIBLEsensors[i].fertility != 0xffff)
#ifdef USE_HOME_ASSISTANT
              || (hass_mode != -1)
#endif //USE_HOME_ASSISTANT
            ) {
              HM10ShowContinuation(&commaflg);
              ResponseAppend_P(PSTR("\"Fertility\":%u"), MIBLEsensors[i].fertility);
            }
          }
        }
        if (MIBLEsensors[i].feature.Btn){
          if(MIBLEsensors[i].eventType.Btn){
            HM10ShowContinuation(&commaflg);
            ResponseAppend_P(PSTR("\"Btn\":%u"),MIBLEsensors[i].Btn);
          }
        }
      } // minimal summary
      if (MIBLEsensors[i].feature.PIR){
        if(MIBLEsensors[i].eventType.motion || !HM10.mode.triggeredTele){
          if(HM10.mode.triggeredTele) {
            HM10ShowContinuation(&commaflg);
            ResponseAppend_P(PSTR("\"PIR\":1")); // only real-time
          }
          HM10ShowContinuation(&commaflg);
          ResponseAppend_P(PSTR("\"Events\":%u"),MIBLEsensors[i].events);
        }
        else if(MIBLEsensors[i].eventType.noMotion && HM10.mode.triggeredTele){
          HM10ShowContinuation(&commaflg);
          ResponseAppend_P(PSTR("\"PIR\":0"));
        }
      }

      if (MIBLEsensors[i].type == HM10_FLORA && !HM10.mode.triggeredTele) {
        if (MIBLEsensors[i].firmware[0] != '\0') { // this is the error code -> no firmware
          HM10ShowContinuation(&commaflg);
          ResponseAppend_P(PSTR("\"Firmware\":\"%s\""), MIBLEsensors[i].firmware);
        }
      }

      if (MIBLEsensors[i].feature.NMT || !HM10.mode.triggeredTele){
        if(MIBLEsensors[i].eventType.NMT){
          HM10ShowContinuation(&commaflg);
          ResponseAppend_P(PSTR("\"NMT\":%u"), MIBLEsensors[i].NMT);
        }
      }
      if (MIBLEsensors[i].feature.bat){
        if(MIBLEsensors[i].eventType.bat || !HM10.mode.triggeredTele || HM10.option.allwaysAggregate){
#ifdef USE_HOME_ASSISTANT
          if ((hass_mode != -1) && (MIBLEsensors[i].bat == 0x00)) {
            HM10ShowContinuation(&commaflg);
            ResponseAppend_P(PSTR("\"Battery\":null"));
          } else
#endif //USE_HOME_ASSISTANT
          if ((MIBLEsensors[i].bat != 0x00)
#ifdef USE_HOME_ASSISTANT
            || (hass_mode != -1)
#endif //USE_HOME_ASSISTANT
          ) {
            HM10ShowContinuation(&commaflg);
            ResponseAppend_P(PSTR("\"Battery\":%u"), MIBLEsensors[i].bat);
          }
        }
      }
      //Scale
      if (MIBLEsensors[i].feature.scale){
        if(MIBLEsensors[i].eventType.scale || !HM10.mode.triggeredTele || HM10.option.allwaysAggregate
  #ifdef USE_HOME_ASSISTANT
            ||(hass_mode==2)
  #endif //USE_HOME_ASSISTANT
        ){
          HM10ShowContinuation(&commaflg);
          ResponseAppend_P(PSTR("\"wgh_removed\":%u"), MIBLEsensors[i].weight_removed);
          HM10ShowContinuation(&commaflg);
          ResponseAppend_P(PSTR("\"wgh_stabilized\":%u"), MIBLEsensors[i].weight_stabilized);
          HM10ShowContinuation(&commaflg);
          ResponseAppend_P(PSTR("\"wgh_unit\":\"%s\""), MIBLEsensors[i].weight_unit);
          HM10ShowContinuation(&commaflg);
          ResponseAppend_P(PSTR("\"" D_JSON_WEIGHT "\":%*_f"),Settings->flag2.weight_resolution, &MIBLEsensors[i].weight);
          HM10ShowContinuation(&commaflg);
          ResponseAppend_P(PSTR("\"dtime\":\"%02u/%02u/%04u %02u:%02u:%02u\"")
                  , MIBLEsensors[i].datetime.day
                  , MIBLEsensors[i].datetime.month
                  , MIBLEsensors[i].datetime.year
                  , MIBLEsensors[i].datetime.hour
                  , MIBLEsensors[i].datetime.minute
                  , MIBLEsensors[i].datetime.second
          );
        }
      }
      if (MIBLEsensors[i].feature.impedance){
          HM10ShowContinuation(&commaflg);
          ResponseAppend_P(PSTR("\"impedance\":%u"), MIBLEsensors[i].has_impedance ? MIBLEsensors[i].impedance : 0);
          HM10ShowContinuation(&commaflg);
          ResponseAppend_P(PSTR("\"imp_stabilized\":%u"), MIBLEsensors[i].impedance_stabilized);
      }

      if (HM10.option.showRSSI) {
        HM10ShowContinuation(&commaflg);
        ResponseAppend_P(PSTR("\"RSSI\":%d"), MIBLEsensors[i].rssi);
      }
      ResponseJsonEnd();

      MIBLEsensors[i].eventType.raw = 0;
      if(MIBLEsensors[i].shallSendMQTT==1){
        MIBLEsensors[i].shallSendMQTT = 0;
        continue;
      }
    }
    HM10.mode.triggeredTele = 0;
// add beacons
    uint32_t _idx = 0;
    for (auto _beacon : MIBLEbeacons){
      _idx++;
      if(!_beacon.active) continue;
      char _MAC[18];
      ToHex_P(_beacon.MAC,6,_MAC,18,':');
      ResponseAppend_P(PSTR(",\"Beacon%u\":{\"MAC\":\"%s\",\"CID\":\"0x%04x\",\"SVC\":\"0x%04x\","
                            "\"UUID\":\"0x%04x\",\"Time\":%u,\"RSSI\":%d,\"TX\":%u}"),
                            _idx,_MAC,_beacon.CID,_beacon.SVC,_beacon.UUID,_beacon.time,_beacon.RSSI,_beacon.TX);
    }
#ifdef USE_HOME_ASSISTANT
    if(hass_mode==2){
      HM10.option.noSummary = _noSummarySave;
      HM10.option.minimalSummary = _minimalSummarySave;
    }
#endif //USE_HOME_ASSISTANT
#ifdef USE_WEBSERVER
    } else {
      static  uint16_t _page = 0;
      static  uint16_t _counter = 0;
      int32_t i = _page * HM10.perPage;
      uint32_t j = i + HM10.perPage;
      if (j+1>MIBLEsensors.size()){
        j = MIBLEsensors.size();
      }
      char stemp[5] ={0};
      if (MIBLEsensors.size()-(_page*HM10.perPage)>1 && HM10.perPage!=1) {
        sprintf_P(stemp,"-%u",j);
      }
      if (MIBLEsensors.size()==0) i=-1; // only for the GUI

      WSContentSend_PD(HTTP_HM10, HM10.firmware, i+1,stemp,MIBLEsensors.size());
      for (i; i<j; i++) {
        WSContentSend_PD(HTTP_HM10_HL);
        char _MAC[18];
        ToHex_P(MIBLEsensors[i].MAC,6,_MAC,18,':');
        WSContentSend_PD(HTTP_HM10_MAC, kHM10DeviceType[MIBLEsensors[i].type-1], D_MAC_ADDRESS, _MAC);
        if (MIBLEsensors[i].type==HM10_FLORA){
          if(!isnan(MIBLEsensors[i].temp)){
            WSContentSend_Temp(kHM10DeviceType[MIBLEsensors[i].type-1], MIBLEsensors[i].temp);
          }
          if(MIBLEsensors[i].lux!=0x00ffffff){ // this is the error code -> no valid value
            WSContentSend_PD(HTTP_SNS_ILLUMINANCE, kHM10DeviceType[MIBLEsensors[i].type-1], MIBLEsensors[i].lux);
          }
          if(MIBLEsensors[i].moisture!=0xff){
            WSContentSend_PD(HTTP_SNS_MOISTURE, kHM10DeviceType[MIBLEsensors[i].type-1], MIBLEsensors[i].moisture);
          }
          if(MIBLEsensors[i].fertility!=0xffff){
            WSContentSend_PD(HTTP_HM10_FLORA_DATA, kHM10DeviceType[MIBLEsensors[i].type-1], MIBLEsensors[i].fertility);
          }
        }
        if (MIBLEsensors[i].type==HM10_MI_SCALE_V1 || MIBLEsensors[i].type==HM10_MI_SCALE_V2){

          if (MIBLEsensors[i].feature.scale){
              WSContentSend_PD(HTTP_MISCALE_WEIGHT, kHM10DeviceType[MIBLEsensors[i].type-1], Settings->flag2.weight_resolution, &MIBLEsensors[i].weight, MIBLEsensors[i].weight_unit);
              WSContentSend_PD(HTTP_MISCALE_WEIGHT_REMOVED, kHM10DeviceType[MIBLEsensors[i].type-1], MIBLEsensors[i].weight_removed ? PSTR("yes") : PSTR("no"));
              WSContentSend_PD(HTTP_MISCALE_STABILIZED, kHM10DeviceType[MIBLEsensors[i].type-1], MIBLEsensors[i].weight_stabilized ? PSTR("yes") : PSTR("no"));
          }
          if (MIBLEsensors[i].feature.impedance){
              WSContentSend_PD(HTTP_MISCALE_IMPEDANCE, kHM10DeviceType[MIBLEsensors[i].type-1], MIBLEsensors[i].has_impedance ? MIBLEsensors[i].impedance : 0);
          }
        }
        else if (MIBLEsensors[i].type>HM10_FLORA){ // everything "above" Flora
          if(!isnan(MIBLEsensors[i].hum) && !isnan(MIBLEsensors[i].temp)){
            WSContentSend_THD(kHM10DeviceType[MIBLEsensors[i].type-1], MIBLEsensors[i].temp, MIBLEsensors[i].hum);
          }
        }
        if(MIBLEsensors[i].bat!=0x00){
          WSContentSend_PD(HTTP_BATTERY, kHM10DeviceType[MIBLEsensors[i].type-1], MIBLEsensors[i].bat);
        }
        WSContentSend_PD(HTTP_RSSI, kHM10DeviceType[MIBLEsensors[i].type-1], MIBLEsensors[i].rssi);
      }
      _counter++;
      if(_counter>3) {
        _page++;
        _counter=0;
      }
      if(MIBLEsensors.size()%HM10.perPage==0 && _page==MIBLEsensors.size()/HM10.perPage) _page=0;
      if(_page>MIBLEsensors.size()/HM10.perPage) _page=0;
    //always at the bottom of the page
    uint32_t _idx=0;
    if(HM10.mode.activeBeacon){
      WSContentSend_PD(HTTP_HM10_HL);
      char _sbeacon[] = "Beacon1";
      for (auto &_beacon : MIBLEbeacons){
        _idx++;
        if(!_beacon.active) continue;
        WSContentSend_PD(HTTP_HM10_HL);
        _sbeacon[6] = _idx + 0x30;
        char _MAC[18];
        ToHex_P(_beacon.MAC,6,_MAC,18,':');
        WSContentSend_PD(HTTP_HM10_MAC, _sbeacon, D_MAC_ADDRESS, _MAC);
        WSContentSend_PD(HTTP_RSSI, _sbeacon, _beacon.RSSI);
        if(_beacon.CID!=0) WSContentSend_PD(PSTR("{s}Beacon%u CID{m}0x%04X{e}"),_idx, _beacon.CID);
        if(_beacon.SVC!=0) WSContentSend_PD(PSTR("{s}Beacon%u SVC{m}0x%04X{e}"),_idx, _beacon.SVC);
        if(_beacon.UUID!=0) WSContentSend_PD(PSTR("{s}Beacon%u UUID{m}0x%04X{e}"),_idx, _beacon.UUID);
        if(_beacon.TX!=0) WSContentSend_PD(PSTR("{s}Beacon%u TX{m}%u{e}"),_idx, _beacon.TX);
        WSContentSend_PD(PSTR("{s}Beacon%u Time{m}%u seconds{e}"),_idx, _beacon.time);
      }
    }
#endif  // USE_WEBSERVER
    }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns62(uint32_t function)
{
  bool result = false;

  if (PinUsed(GPIO_HM10_RX) && PinUsed(GPIO_HM10_TX)) {
    switch (function) {
      case FUNC_INIT:
        HM10SerialInit();                                  // init and start communication
        break;
      case FUNC_EVERY_50_MSECOND:
        HM10SerialHandleFeedback();                        // check for device feedback very often
       break;
      case FUNC_EVERY_100_MSECOND:
        if (HM10_TASK_LIST[0][0] != TASK_HM10_NOTASK) {
          HM10_TaskEvery100ms();                           // something has to be done, we'll check in the next step
        }
        break;
      case FUNC_EVERY_SECOND:
        HM10EverySecond(false);
        break;
      case FUNC_COMMAND:
        result = DecodeCommand(kHM10_Commands, HM10_Commands);
        break;
      case FUNC_JSON_APPEND:
        HM10Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        HM10Show(0);
        break;
#endif  // USE_WEBSERVER
      }
  }
  return result;
}
#endif  // USE_HM10
#endif  // ESP8266