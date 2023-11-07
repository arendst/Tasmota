/*
  xsns_62_esp32_mi.h - MI-BLE-sensors via ESP32 support for Tasmota

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

*/
#ifdef USE_MI_ESP32
/*********************************************************************************************\
 * structs and types
\*********************************************************************************************/
#pragma pack(1)  // byte-aligned structures to read the sensor data

struct frame_crtl_t{
  uint16_t reserved1:1;
  uint16_t reserved2:1;
  uint16_t reserved3:1;
  uint16_t isEncrypted:1;
  uint16_t includesMAC:1;
  uint16_t includesCapability:1;
  uint16_t includesObj:1;
  uint16_t MESH:1;
  uint16_t registered:1;
  uint16_t solicited:1;
  uint16_t AuthMode:2;
  uint16_t version:4;
};

struct mi_payload_t{
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
    uint8_t leak; //14
    uint32_t NMT; //17
    uint8_t door; //19
    struct{ //01
      uint8_t num;
      uint8_t value;
      uint8_t type;
    }Btn;
  };
  uint8_t padding[12]; //for decryption
};

struct mi_beacon_t{
  frame_crtl_t frame;
  uint16_t productID;
  uint8_t counter;
  uint8_t MAC[6];
  uint8_t capability;
  mi_payload_t payload;
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

struct encPacket_t{
  // the packet is longer, but this part is enough to decrypt
  uint16_t PID;
  uint8_t frameCnt;
  uint8_t MAC[6];
  uint8_t payload[16]; // only a pointer to the address, size is variable
};

struct berryAdvPacket_t{
  uint8_t MAC[6];
  uint8_t addressType;
  uint8_t RSSI;
  uint8_t length;      // length of payload
  uint8_t payload[32]; // only a pointer to the address, size is 0-31 bytes
};


union mi_bindKey_t{
  struct{
    uint8_t key[16];
    uint8_t MAC[6];
    };
  uint8_t buf[22];
};

struct ATCPacket_t{ //and PVVX
  uint8_t MAC[6];
  union {
    struct{
      uint16_t temp; //sadly this is in wrong endianess
      uint8_t hum;
      uint8_t batPer;
      uint16_t batMV;
      uint8_t frameCnt;
    } A; //ATC
    struct{
      int16_t temp;
      uint16_t hum; // x 0.01 %
      uint16_t batMV;
      uint8_t batPer;
      uint8_t frameCnt;
      struct {
        uint8_t reed:1;
        uint8_t TRGval:1;
        uint8_t TRGcrtl:1;
        uint8_t tempTrig:1;
        uint8_t humTrig:1;
        uint8_t spare:3;
      };
    }P; //PVVX
  };
};

struct BLERingBufferItem_t{
  uint16_t returnCharUUID;
  uint16_t handle;
  uint32_t type;
  uint8_t length;
};

#pragma pack(0)


struct MI32connectionContextBerry_t{
  NimBLEUUID serviceUUID;
  NimBLEUUID charUUID;
  uint16_t returnCharUUID;
  uint16_t handle;
  uint8_t MAC[6];
  uint8_t * buffer;
  uint8_t operation;
  uint8_t addrType;
  int error;
  bool oneOp;
  bool response;
};

struct {
  // uint32_t period;             // set manually in addition to TELE-period, is set to TELE-period after start
  TaskHandle_t ScanTask = nullptr;
  TaskHandle_t ConnTask = nullptr;
  TaskHandle_t ServerTask = nullptr;
  MI32connectionContextBerry_t *conCtx = nullptr;
  union {
    struct {
      uint32_t init:1;
      uint32_t connected:1;
      uint32_t autoScan:1;
      // uint32_t canScan:1;
      uint32_t runningScan:1;
      uint32_t updateScan:1;
      uint32_t deleteScanTask:1;

      uint32_t canConnect:1;
      uint32_t willConnect:1;
      uint32_t readingDone:1;

      uint32_t shallTriggerTele:1;
      uint32_t triggeredTele:1;
      uint32_t shallShowStatusInfo:1; // react to amount of found sensors via RULES
      uint32_t didGetConfig:1;

      uint32_t triggerBerryAdvCB:1;
      uint32_t triggerBerryConnCB:1;
      uint32_t triggerNextConnJob:1;
      uint32_t readyForNextConnJob:1;
      uint32_t discoverAttributes:1;

      uint32_t triggerNextServerJob:1;
      uint32_t readyForNextServerJob:1;
      uint32_t triggerBerryServerCB:1;
      uint32_t deleteServerTask:1;
    };
    uint32_t all = 0;
  } mode;
  struct {
    uint8_t sensor;             // points to to the number 0...255
  } state;
  struct {
    uint32_t allwaysAggregate:1; // always show all known values of one sensor in brdigemode
    uint32_t noSummary:1;        // no sensor values at TELE-period
    uint32_t directBridgeMode:1; // send every received BLE-packet as a MQTT-message in real-time
    uint32_t activeScan:1;
    uint32_t ignoreBogusBattery:1;
    uint32_t minimalSummary:1;   // DEPRECATED!!
  } option;
#ifdef USE_MI_EXT_GUI
  uint32_t widgetSlot;
#ifdef USE_ENERGY_SENSOR
  uint8_t *energy_history;
#endif //USE_ENERGY_SENSOR
#endif //USE_MI_EXT_GUI

  void *beConnCB;
  void *beAdvCB;
  void *beServerCB;
  uint8_t *beAdvBuf;
  uint8_t infoMsg = 0;
} MI32;

struct mi_sensor_t{
  uint8_t type; //Flora = 1; MI-HT_V1=2; LYWSD02=3; LYWSD03=4; CGG1=5; CGD1=6
  uint8_t lastCnt; //device generated counter of the packet
  uint8_t shallSendMQTT;
  uint8_t MAC[6];
  uint16_t PID;
  uint8_t *key = nullptr;
  char *name = nullptr;
  union {
    struct {
      uint32_t needsKey:1;
      uint32_t temp:1;
      uint32_t hum:1;
      uint32_t tempHum:1; //every hum sensor has temp too, easier to use Tasmota dew point functions
      uint32_t lux:1;
      uint32_t moist:1;
      uint32_t fert:1;
      uint32_t bat:1;
      uint32_t NMT:1;
      uint32_t motion:1;
      uint32_t Btn:1;
      uint32_t knob:1;
      uint32_t door:1;
      uint32_t leak:1;
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
      uint32_t knob:1;
      uint32_t longpress:1; //needs no extra feature bit, because knob is sufficient
      uint32_t door:1;
      uint32_t leak:1;
    };
    uint32_t raw;
  } eventType;
  union{
    struct{
      uint8_t hasWrongKey:1;
      uint8_t isUnbounded:1;
    };
    uint8_t raw;
  } status;

  int RSSI;
  uint32_t lastTime;
  uint32_t lux;
  uint8_t *lux_history;
  float temp; //Flora, MJ_HT_V1, LYWSD0x, CGx
  uint8_t *temp_history;
  union {
    struct {
      uint8_t moisture;
      uint16_t fertility;
      char firmware[6]; // actually only for FLORA but hopefully we can add for more devices
    }; // Flora
    struct {
      float hum;
      uint8_t *hum_history;
    }; // MJ_HT_V1, LYWSD0x
    struct {
      uint16_t events; //"alarms" since boot
      uint32_t NMT;    // no motion time in seconds for the MJYD2S and NLIGHT
    };
    struct {
      uint8_t Btn;     // number starting with 0
      uint8_t BtnType; // 0 -single, 1 - double, 2 - hold
      uint8_t leak; // the leak sensor is the only non-RC device so far with a button fuctionality, so we handle it here
      int8_t dimmer;
      uint8_t pressed; // dimmer knob pressed while rotating
      uint8_t longpress; // dimmer knob pressed without rotating
    };
    uint8_t door;
  };
  union {
      uint8_t bat; // many values seem to be hard-coded garbage (LYWSD0x, GCD1)
  };
};

/*********************************************************************************************\
 * constants
\*********************************************************************************************/

#define D_CMND_MI32 "MI32"

const char kMI32_Commands[] PROGMEM = D_CMND_MI32 "|Key|Name|Cfg|Option";

void (*const MI32_Commands[])(void) PROGMEM = {&CmndMi32Key, &CmndMi32Name,&CmndMi32Cfg, &CmndMi32Option };

#define UNKNOWN_MI  0
#define FLORA       1
#define MJ_HT_V1    2
#define LYWSD02     3
#define LYWSD03MMC  4
#define CGG1        5
#define CGD1        6
#define NLIGHT      7
#define MJYD2S      8
#define YLYK01      9
#define MHOC401     10
#define MHOC303     11
#define ATC         12
#define MCCGQ02     13
#define SJWS01L     14
#define PVVX        15
#define YLKG08      16
#define YLAI003     17

#define MI32_TYPES    17 //count this manually

const uint16_t kMI32DeviceID[MI32_TYPES]={ 0x0098, // Flora
                                  0x01aa, // MJ_HT_V1
                                  0x045b, // LYWSD02
                                  0x055b, // LYWSD03
                                  0x0347, // CGG1
                                  0x0576, // CGD1
                                  0x03dd, // NLIGHT
                                  0x07f6, // MJYD2S
                                  0x0153, // YLYK01, old name yee-rc
                                  0x0387, // MHO-C401
                                  0x06d3, // MHO-C303
                                  0x0a1c, // ATC -> this is a fake ID
                                  0x098b, // MCCGQ02
                                  0x0863, // SJWS01L
                                  0x944a, // PVVX -> this is a fake ID
                                  0x03b6, // YLKG08 and YLKG07 - version w/wo mains
                                  0x07bf, // YLAI003
                                  };

const char kMI32DeviceType[] PROGMEM = {"Flora|MJ_HT_V1|LYWSD02|LYWSD03|CGG1|CGD1|NLIGHT|MJYD2S|YLYK01|MHOC401|MHOC303|ATC|MCCGQ02|SJWS01L|PVVX|YLKG08|YLAI003"};

const char kMI32_ConnErrorMsg[] PROGMEM = "no Error|could not connect|did disconnect|got no service|got no characteristic|can not read|can not notify|can not write|did not write|notify time out";

const char kMI32_BLEInfoMsg[] PROGMEM = "Scan ended|Got Notification|Did connect|Did disconnect|Still connected|Start passive scanning|Start active scanning|Server characteristic set|Server advertisement set|Server scan response set|Server client did connect|Server client did disconnect";

const char kMI32_ButtonMsg[] PROGMEM = "Single|Double|Hold"; //mapping: in Tasmota: 1,2,3 ; for HomeKit and Xiaomi 0,1,2
/*********************************************************************************************\
 * enumerations
\*********************************************************************************************/

enum MI32_Commands {          // commands useable in console or rules
  CMND_MI32_KEY,              // add bind key to a mac for packet decryption
  CMND_MI32_CFG,              // save config file as JSON with all sensors w/o keys to mi32cfg
  CMND_MI32_OPTION            // change driver options at runtime
  };

enum MI32_TASK {
  MI32_TASK_SCAN = 0,
  MI32_TASK_CONN = 1,
  MI32_TASK_SERV = 2,
};

enum BLE_CLIENT_OP {
BLE_OP_READ = 1,
BLE_OP_WRITE,
BLE_OP_SUBSCRIBE,
BLE_OP_UNSUBSCRIBE, //maybe used later
BLE_OP_DISCONNECT,
BLE_OP_GET_NOTIFICATION = 103,
};

enum BLE_SERVER_OP {
//commands
BLE_OP_SET_ADV = 201,
BLE_OP_SET_SCAN_RESP,
BLE_OP_SET_CHARACTERISTIC = 211,
//response
BLE_OP_ON_READ = 221,
BLE_OP_ON_WRITE,
BLE_OP_ON_UNSUBSCRIBE,
BLE_OP_ON_SUBSCRIBE_TO_NOTIFICATIONS,
BLE_OP_ON_SUBSCRIBE_TO_INDICATIONS,
BLE_OP_ON_SUBSCRIBE_TO_NOTIFICATIONS_AND_INDICATIONS,
BLE_OP_ON_CONNECT,
BLE_OP_ON_DISCONNECT,
BLE_OP_ON_STATUS,
};

enum MI32_ConnErrorMsg {
  MI32_CONN_NO_ERROR = 0,
  MI32_CONN_NO_CONNECT,
  MI32_CONN_DID_DISCCONNECT,
  MI32_CONN_NO_SERVICE,
  MI32_CONN_NO_CHARACTERISTIC,
  MI32_CONN_CAN_NOT_READ,
  MI32_CONN_CAN_NOT_NOTIFY,
  MI32_CONN_CAN_NOT_WRITE,
  MI32_CONN_DID_NOT_WRITE,
  MI32_CONN_NOTIFY_TIMEOUT
};

enum MI32_BLEInfoMsg {
  MI32_SCAN_ENDED = 1,
  MI32_GOT_NOTIFICATION,
  MI32_DID_CONNECT,
  MI32_DID_DISCONNECT,
  MI32_STILL_CONNECTED,
  MI32_START_SCANNING_PASSIVE,
  MI32_START_SCANNING_ACTIVE,
  MI32_SERV_CHARACTERISTIC_ADDED,
  MI32_SERV_ADVERTISEMENT_ADDED,
  MI32_SERV_SCANRESPONSE_ADDED,
  MI32_SERV_CLIENT_CONNECTED,
  MI32_SERV_CLIENT_DISCONNECTED
};

/*********************************************************************************************\
 * extended web gui
\*********************************************************************************************/

#ifdef USE_WEBSERVER
#ifdef USE_MI_EXT_GUI
const char HTTP_BTN_MENU_MI32[] PROGMEM = "<p><form action='m32' method='get'><button>Mi Dashboard</button></form></p>";

const char HTTP_MI32_SCRIPT_1[] PROGMEM =
  "function setUp(){setInterval(countUp,1000); setInterval(update,1000);}"
  "function countUp(){let ti=document.querySelectorAll('.Ti');"
  "for(const el of ti){var t=parseInt(el.innerText);el.innerText=t+1;}}"
  "function update(){"
    "fetch('/m32?wi=1').then(r=>r.text())"
    ".then((r)=>{"
      // console.log(r); // optional
      "if(r.length>0){"
        "var d=document.createElement('div');"
        "d.innerHTML=r.trim();"
        "var old=eb(d.firstChild.id);"
        "old.parentNode.replaceChild(d.firstChild, old);"
      "}"
    "})"
    //".catch((e) => {console.error(e);});" //optional
  "};"
;

const char HTTP_MI32_STYLE[] PROGMEM =
  "<style onload=setTimeout(setUp,500)>.parent {display: flex;flex-wrap: wrap;justify-content: center;}svg{float:inline-end;}"
  ".box {flex: 0 1 335px;margin: 5px;padding: 5px;border-radius: 0.8rem;background-color: rgba(221, 221, 221, 0.2);}</style>";

const char HTTP_MI32_STYLE_SVG[] PROGMEM =
  "<svg height='0'><defs><linearGradient id='grd%u'  x1='0%%' y1='0%%' x2='0%%' y2='15%%'>"
  "<stop offset='0' stop-color='rgba(%u, %u, %u, 0.5)'/>"
  "<stop offset='1' stop-color='rgba(%u, %u, %u, 0)'/></linearGradient></defs></svg>"
  ;

const char HTTP_MI32_PARENT_START[] PROGMEM =
  "<div class='parent'>"
      "<div class='box'><h2>MI32 Bridge</h2>"
          "Observing <span id='numDev'>%u</span> devices<br>"
          "Uptime: <span class='Ti'>%u</span> seconds<br>"
          "Free Heap: %u kB"
      "</div>";

const char HTTP_MI32_WIDGET[] PROGMEM =
  "<div class='box' id='box%u' style='opacity:%u.5;'>MAC:%s RSSI:%d %s<br>"
  "<small>&zwnj;%s</small>"
  "<h2 style='margin-top:0em;'>%s"
  "<svg height='24' width='24' style='float:inline-end;'>"
  "<circle cx='11' cy='11' r='0' fill='#90ee90' opacity='0'>"
  "<animate attributeName='r' from='11' to='0' dur='9s' repeatCount='1'></animate>"
  "<animate attributeName='opacity' from='1' to='0' dur='9s' repeatCount='1'></animate></circle></svg>"
  "</h2>";

const char HTTP_MI32_GRAPH[] PROGMEM =
    "<svg height='20' width='150'>"
    "<polyline points='%s'"
    "style='stroke:rgb(%u, %u, %u);fill:none;'></polyline>"
    "<polyline points='%s138,20 0,20'"
    "style='stroke:none;fill:url(#grd%u);'></polyline>"
    "</svg>";
      //rgb(185, 124, 124) - red, rgb(185, 124, 124) - blue, rgb(242, 240, 176) - yellow

#ifdef USE_MI_ESP32_ENERGY
const char HTTP_MI32_POWER_WIDGET[] PROGMEM =
  "<div class='box' id='box%u'>"
   "<h2 style='margin-top:0em;'>Energy"
  "</h2>"
  "<p>" D_VOLTAGE ": %.1f " D_UNIT_VOLT "</p>"
  "<p>" D_CURRENT ": %.3f " D_UNIT_AMPERE "</p>";
#endif //USE_MI_ESP32_ENERGY

#endif //USE_MI_EXT_GUI
#endif // USE_WEBSERVER

#endif //USE_MI_ESP32