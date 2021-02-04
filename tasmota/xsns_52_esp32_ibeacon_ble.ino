/*
  xsns_52_esp32_ibeacon_ble.ino
   - Support for BLE_ESP32 ibeacon reader on Tasmota

  Copyright (C) 2020  Gerhard Mutz and Theo Arends and Simon Hailes

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

////////////////////////////////////////
// Commands:
// iBeacon 0/1 - Enable 
// iBeaconOnlyAliased 0/1/2 - 
//   0 = all BLE devices, 
//   1 = only devices with any BLEAlias
//   2 = only devices which have BLEAlias starting "iB" 
// iBeaconClear - clear list NOW
// iBeaconPeriod (sec) - update period - default 10s
// iBeaconTimeout (sec) - timeout period - default 30s
//
// posts MQTT about each beacon every iBeaconPeriod (default 10s)
// MQTT like
// tele/tasmota_E89E98/SENSOR = {"Time":"2021-02-04T23:20:22","IBEACON":{"MAC":"FFFFA0003B19","NAME":"iB1","RSSI":-81,"STATE":"ON","PERSEC":10}}
//  Always present:
//   Time: time of MQTT send
//   IBEACON.MAC
//   IBEACON.RSSI
//   IBEACON.STATE - ON - present, OFF - last MQTT you will get for now (device removed)
//  Optional:
//   IBEACON.NAME - name if in scan, or BLEAlias if set - only present if NAME present
//   IBEACON.PERSEC - count of adverts per sec.  USeful for detecting button press?
//   IBEACON.MAJOR - some iBeacon related term? - only present for some
//   IBEACON.MINOR - some iBeacon related term? - only present for some
////////////////////////////////////////


// for testing of BLE_ESP32, we remove xsns_52_ibeacon.ino completely, and instead add this modified xsns_52_ibeacon_BLE_ESP32.ino
// in the future this may be more fine-grained, e.g. to allow hm17 for this, and BLE-ESP32 for other
#if CONFIG_IDF_TARGET_ESP32
#ifdef USE_BLE_ESP32

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
//#define IB_UPDATE_TIME Settings.ib_upd_interval
//#define IB_TIMEOUT_TIME Settings.ib_tout_interval
//#endif

#define ENDIAN_CHANGE_U16(x) ((((x)&0xFF00) >> 8) + (((x)&0xFF) << 8))

struct IBEACON {
  char FACID[8];
  char UID[32];
  char MAJOR[4];
  char MINOR[4];
  char PWR[2];
  char MAC[12];
  char RSSI[4];
  char NAME[16];
};

#define MAX_IBEACONS 32

struct IBEACON_UID {
  char MAC[12];
  char RSSI[4];
  char UID[32];
  char MAJOR[4];
  char MINOR[4];
  uint8_t FLAGS;
  uint8_t TIME;
  uint8_t REPORTED;
  uint8_t REPTIME;
  uint8_t count; // count of adverts in REPTIME (reptime default = 10s)
  uint32_t lastmqtt; // last time we sent something
  char NAME[16];
} ibeacons[MAX_IBEACONS];


void CmndiBeacon(void);
void CmndiBeaconOnlyAliased(void);
void CmndiBeaconClear(void);
void CmndiBeaconPeriod(void);
void CmndiBeaconTimeout(void);

#define D_CMND_IBEACON "IBEACON"

const char kiBeacon_Commands[] PROGMEM = D_CMND_IBEACON "|"
  "|"
  "onlyaliased|"
  "clear|"
  "period|"
  "timeout";

void (*const iBeacon_Commands[])(void) PROGMEM = {
  &CmndiBeacon,
  &CmndiBeaconOnlyAliased,
  &CmndiBeaconClear,
  &CmndiBeaconPeriod,
  &CmndiBeaconTimeout
};

uint8_t iBeaconOnlyAliased = 0;
uint8_t iBeaconEnable = 0;


void CmndiBeacon(void){
  if (XdrvMailbox.data_len > 0) {
    iBeaconEnable = XdrvMailbox.payload;
  }
  ResponseCmndNumber(iBeaconEnable);
}

void CmndiBeaconOnlyAliased(void){
  if (XdrvMailbox.data_len > 0) {
    iBeaconOnlyAliased = XdrvMailbox.payload;
  }
  ResponseCmndNumber(iBeaconOnlyAliased);
}

void CmndiBeaconClear(void){
  uint32_t count = 0;
  for (uint32_t cnt=0; cnt<MAX_IBEACONS; cnt++) {
    if (ibeacons[cnt].FLAGS) count++;
    ibeacons[cnt].FLAGS=0;
  }
  ResponseCmndNumber(count);
}

void CmndiBeaconPeriod(void){
  if (XdrvMailbox.data_len > 0) {
    IB_UPDATE_TIME = XdrvMailbox.payload;
  }
  ResponseCmndNumber(IB_UPDATE_TIME);
}

void CmndiBeaconTimeout(void){
  if (XdrvMailbox.data_len > 0) {
    IB_TIMEOUT_TIME = XdrvMailbox.payload;
  }
  ResponseCmndNumber(IB_TIMEOUT_TIME);
}


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

int advertismentCallback(BLE_ESP32::ble_advertisment_t *pStruct)
{
  struct IBEACON ib;
  if (!iBeaconEnable) return 0;

  BLEAdvertisedDevice *advertisedDevice = pStruct->advertisedDevice;

  char sRSSI[6];
  itoa(pStruct->RSSI,sRSSI,10);
  const uint8_t *MAC = pStruct->addr;

  const char *alias = BLE_ESP32::getAlias(MAC);
  if (iBeaconOnlyAliased){
    // ignore unless we have an alias.
    if (!alias || !(*alias)){
      return 0;
    }
  }
  if (!alias) alias = "";

  if (iBeaconOnlyAliased == 2){
    if (strncmp(alias, "iB", 2)){
      return 0;
    }
  }

  int manufacturerDataLen = 0;
  std::string data;
  if (advertisedDevice->haveManufacturerData()){
    data = advertisedDevice->getManufacturerData();
    manufacturerDataLen = data.length();
  }
  if (manufacturerDataLen){
    const uint8_t *manufacturerData = (const uint8_t *)data.data();
    DumpHex(manufacturerData, 2, ib.FACID);
    if (manufacturerDataLen == 25 &&
        manufacturerData[0] == 0x4C &&
        manufacturerData[1] == 0x00)
    {
      BLEBeacon oBeacon = BLEBeacon();
      oBeacon.setData(std::string((char *)manufacturerData, manufacturerDataLen));
      uint8_t UUID[16];
      memcpy(UUID,oBeacon.getProximityUUID().getNative()->u128.value,16);
      ESP32BLE_ReverseStr(UUID,16);

      uint16_t    Major = ENDIAN_CHANGE_U16(oBeacon.getMajor());
      uint16_t    Minor = ENDIAN_CHANGE_U16(oBeacon.getMinor());
      uint8_t     PWR   = oBeacon.getSignalPower();

      DumpHex((const unsigned char*)&UUID,16,ib.UID);
      DumpHex((const unsigned char*)&Major,2,ib.MAJOR);
      DumpHex((const unsigned char*)&Minor,2,ib.MINOR);
      DumpHex((const unsigned char*)&PWR,1,ib.PWR);
      DumpHex((const unsigned char*)MAC,6,ib.MAC);
      memcpy(ib.RSSI,sRSSI,4);
      memset(ib.NAME,0x0,16);
      if (*alias){
        strncpy(ib.NAME, alias, 16);
      }

      // if we added it
      if (ibeacon_add(&ib) == 1){
        AddLog(LOG_LEVEL_DEBUG, PSTR("%s: MAC: %s Major: %d Minor: %d UUID: %s Power: %d RSSI: %d"),
          "iBeacon",
          advertisedDevice->getAddress().toString().c_str(),
          Major, Minor,
          oBeacon.getProximityUUID().toString().c_str(),
          PWR, pStruct->RSSI);
      }
      return 0;
    }
  }

  // no manufacturer data, or not recognised.
  // still have an RSSi....
  memset(ib.UID,'0',32);
  memset(ib.MAJOR,'0',4);
  memset(ib.MINOR,'0',4);
  memset(ib.PWR,'0',2);
  DumpHex((const unsigned char*)MAC,6,ib.MAC);
  memcpy(ib.RSSI,sRSSI,4);

  if (advertisedDevice->haveName()) {
    strncpy(ib.NAME,advertisedDevice->getName().c_str(),16);
  } else {
    memset(ib.NAME,0x0,16);
  }

  if (*alias){
    strncpy(ib.NAME, alias,16);
  }

  ibeacon_add(&ib);
  return 0;
}


void IBEACON_Init() {
  AddLog(LOG_LEVEL_INFO, PSTR("iBeacon register for advert callbacks"));
  BLE_ESP32::registerForAdvertismentCallbacks((const char *)"iBeacon", advertismentCallback);
  IB_UPDATE_TIME=IB_UPDATE_TIME_INTERVAL;
  IB_TIMEOUT_TIME=IB_TIMEOUT_INTERVAL;
}

void esp32_every_second(void) {
  for (uint32_t cnt=0; cnt < MAX_IBEACONS; cnt++) {
    if (ibeacons[cnt].FLAGS) {
      uint8_t mac[6];
      char tmp[13];
      memcpy(tmp, ibeacons[cnt].MAC, 12);
      tmp[12] = 0;
      BLE_ESP32::fromHex(mac, tmp, 6);
      // use global device timeouts from BLE_ESP32.

      uint32_t ageS = BLE_ESP32::devicePresent(mac);

      // if device not present at all or past local timeout.
      if (!ageS || (ageS > IB_TIMEOUT_TIME)){
        //AddLog(LOG_LEVEL_INFO, PSTR("iBeacon no device %s %02x%02x%02x%02x%02x%02x"),tmp, mac[0],mac[1], mac[2],mac[3], mac[4],mac[5]);
        ibeacons[cnt].FLAGS=0;
        uint64_t now = esp_timer_get_time();
        uint32_t nowms = now/1000;
        float countspermssec = ((float)ibeacons[cnt].count)/(((float)(nowms - ibeacons[cnt].lastmqtt))/1000.0);
        ibeacon_mqtt(ibeacons[cnt].MAC,"0000",ibeacons[cnt].UID,ibeacons[cnt].MAJOR,ibeacons[cnt].MINOR,ibeacons[cnt].NAME, (int)(countspermssec));
        ibeacons[cnt].count = 0;
        ibeacons[cnt].lastmqtt = nowms;
      } else {
        //AddLog(LOG_LEVEL_INFO, PSTR("iBeacon device %s %02x%02x%02x%02x%02x%02x"),tmp, mac[0],mac[1], mac[2],mac[3], mac[4],mac[5]);
      }
      //ibeacons[cnt].TIME++;
      ibeacons[cnt].REPTIME++; // counter used to send mqtt for a dev regularly
    }
  }
}


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
            strncpy(ibeacons[cnt].NAME,ib->NAME,sizeof(ibeacons[cnt].NAME));
            memcpy(ibeacons[cnt].RSSI,ib->RSSI,4);
            ibeacons[cnt].TIME=0;
            if (ibeacons[cnt].REPTIME >= IB_UPDATE_TIME) {
              ibeacons[cnt].REPTIME = 0;
              ibeacons[cnt].REPORTED = 0;
            }
            ibeacons[cnt].count++;
            return 2;
          }
        } else {
          if (!strncmp(ibeacons[cnt].UID,ib->UID,32)) {
            // exists
            strncpy(ibeacons[cnt].NAME,ib->NAME,sizeof(ibeacons[cnt].NAME));
            memcpy(ibeacons[cnt].RSSI,ib->RSSI,4);
            ibeacons[cnt].TIME=0;
            if (ibeacons[cnt].REPTIME >= IB_UPDATE_TIME) {
              ibeacons[cnt].REPTIME = 0;
              ibeacons[cnt].REPORTED = 0;
            }
            ibeacons[cnt].count++;
            return 2;
          }
        }
      }
    }
    for (uint32_t cnt=0;cnt<MAX_IBEACONS;cnt++) {
      if (!ibeacons[cnt].FLAGS) {
        strncpy(ibeacons[cnt].NAME,ib->NAME,sizeof(ibeacons[cnt].NAME));
        memcpy(ibeacons[cnt].MAC,ib->MAC,12);
        memcpy(ibeacons[cnt].RSSI,ib->RSSI,4);
        memcpy(ibeacons[cnt].UID,ib->UID,32);
        memcpy(ibeacons[cnt].MAJOR,ib->MAJOR,4);
        memcpy(ibeacons[cnt].MINOR,ib->MINOR,4);
        ibeacons[cnt].FLAGS=1;
        ibeacons[cnt].TIME=0;
        memcpy(ibeacons[cnt].NAME,ib->NAME,16);
        ibeacons[cnt].REPTIME = 0;
        ibeacons[cnt].REPORTED = 0;
        ibeacons[cnt].count = 0;
        uint64_t now = esp_timer_get_time();
        uint32_t nowms = now/1000;
        ibeacons[cnt].lastmqtt = nowms;
        return 1;
      }
    }
  }
  return 0;
}


void IBEACON_loop() {
  //TasAutoMutex localmutex(&beaconmutex, "iBeacLoop");
  for (uint32_t cnt=0; cnt<MAX_IBEACONS; cnt++) {
    if (ibeacons[cnt].FLAGS && (!ibeacons[cnt].REPORTED || ibeacons[cnt].count > 10)) {
      uint64_t now = esp_timer_get_time();
      uint32_t nowms = now/1000;
      float countspermssec = ((float)ibeacons[cnt].count)/(((float)(nowms - ibeacons[cnt].lastmqtt))/1000.0);
      // squash if it only just appeared
      if (ibeacons[cnt].count < 2) countspermssec = 0.0;
      ibeacon_mqtt(ibeacons[cnt].MAC,ibeacons[cnt].RSSI,ibeacons[cnt].UID,ibeacons[cnt].MAJOR,ibeacons[cnt].MINOR,ibeacons[cnt].NAME, (int)(countspermssec));
      ibeacons[cnt].count = 0;
      ibeacons[cnt].lastmqtt = nowms;
      ibeacons[cnt].REPORTED=1;
      ibeacons[cnt].REPTIME = 0;
    }
  }
}

#ifdef USE_WEBSERVER
const char HTTP_IBEACON_HL[] PROGMEM = "{s}<hr>{m}<hr>{e}";
const char HTTP_IBEACON_mac[] PROGMEM =
 "{s}IBEACON-MAC : %s" " {m} RSSI : %s" "{e}";
const char HTTP_IBEACON_uid[] PROGMEM =
 "{s}IBEACON-UID : %s" " {m} RSSI : %s" "{e}";
const char HTTP_IBEACON_name[] PROGMEM =
 "{s}IBEACON-NAME : %s (%s)" " {m} RSSI : %s" "{e}";
void IBEACON_Show(void) {
  char mac[14];
  char rssi[6];
  char uid[34];
  char name[18];
  //TasAutoMutex localmutex(&beaconmutex, "iBeacShow");
  int total = 0;

  for (uint32_t cnt=0;cnt<MAX_IBEACONS;cnt++) {
    if (ibeacons[cnt].FLAGS) {
      total++;
      memcpy(mac,ibeacons[cnt].MAC,12);
      mac[12]=0;
      memcpy(rssi,ibeacons[cnt].RSSI,4);
      rssi[4]=0;
      memcpy(uid,ibeacons[cnt].UID,32);
      uid[32]=0;
      memcpy(name,ibeacons[cnt].NAME,16);
      name[16]=0;
      if (!strncmp_P(uid,PSTR("00000000000000000000000000000000"),32)) {
        if (name[0]) {
          WSContentSend_PD(HTTP_IBEACON_name,name,mac,rssi);
        } else {
          WSContentSend_PD(HTTP_IBEACON_mac,mac,rssi);
        }
      } else {
        WSContentSend_PD(HTTP_IBEACON_uid,uid,rssi);
      }
    }
  }
  // add a break line after us, if we showed anything.
  if (total) WSContentSend_PD(HTTP_IBEACON_HL);

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
c  = clears sensor list
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
      } else if (*cp=='c') {
        for (uint32_t cnt=0;cnt<MAX_IBEACONS;cnt++) ibeacons[cnt].FLAGS=0;
        Response_P(S_JSON_IBEACON1, XSNS_52,"clr list","");
      }
  } else {
    serviced=false;
  }
  return serviced;
}

void ibeacon_mqtt(const char *mac,const char *rssi,const char *uid,const char *major,const char *minor, const char *name, int count) {
  char s_mac[14];
  char s_uid[34];
  char s_major[6];
  char s_minor[6];
  char s_rssi[6];
  char *s_state;
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
  if  (n_rssi) {
    s_state=(char *)"ON";
  } else {
    s_state=(char *)"OFF";
  }
  memcpy(s_name,name,16);
  s_name[16]=0;

  // if uid == all zeros, take mac
  if (!strncmp_P(s_uid,PSTR("00000000000000000000000000000000"),32)) {
    if (name[0]) {
      ResponseTime_P(PSTR(",\"" D_CMND_IBEACON "\":{\"MAC\":\"%s\",\"NAME\":\"%s\",\"RSSI\":%d,\"STATE\":\"%s\",\"PERSEC\":%d}}"),s_mac,s_name,n_rssi,s_state, count);
    } else {
      ResponseTime_P(PSTR(",\"" D_CMND_IBEACON "\":{\"MAC\":\"%s\",\"RSSI\":%d,\"STATE\":\"%s\",\"PERSEC\":%d}}"),s_mac,n_rssi,s_state, count);
    }
  } else {
    if (name[0]) {
      ResponseTime_P(PSTR(",\"" D_CMND_IBEACON "\":{\"MAC\":\"%s\",\"NAME\":\"%s\",\"MAJOR\":\"%s\",\"MINOR\":\"%s\",\"MAC\":\"%s\",\"RSSI\":%d,\"STATE\":\"%s\",\"PERSEC\":%d}}"),s_uid,s_name,s_major,s_minor,s_mac,n_rssi,s_state,count);
    } else {
      ResponseTime_P(PSTR(",\"" D_CMND_IBEACON "\":{\"UID\":\"%s\",\"MAJOR\":\"%s\",\"MINOR\":\"%s\",\"MAC\":\"%s\",\"RSSI\":%d,\"STATE\":\"%s\",\"PERSEC\":%d}}"),s_uid,s_major,s_minor,s_mac,n_rssi,s_state,count);
    }
  }

  MqttPublishTeleSensor();
}


/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns52(byte function)
{
  bool result = false;

    switch (function) {
      case FUNC_INIT:
        IBEACON_Init();
        break;
      case FUNC_LOOP:
        if (iBeaconEnable) IBEACON_loop();
        break;
      case FUNC_EVERY_SECOND:
        if (iBeaconEnable) esp32_every_second();
        break;
      case FUNC_COMMAND:
        result = DecodeCommand(kiBeacon_Commands, iBeacon_Commands);
        break;

      case FUNC_COMMAND_SENSOR:
        if (XSNS_52 == XdrvMailbox.index) {
          result = xsns52_cmd();
        }
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        if (iBeaconEnable) IBEACON_Show();
        break;
#endif  // USE_WEBSERVER
    }
    return result;
}

#endif // USE_BLE_ESP32
#endif  // CONFIG_IDF_TARGET_ESP32
