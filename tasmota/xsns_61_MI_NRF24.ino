/*
  xsns_61_MI_NRF24.ino - MI-BLE-sensors via nrf24l01 support for Tasmota

  Copyright (C) 2020  Christian Baars and Theo Arends

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

  0.9.8.0 20200705  integrate - add YEE-RC, NLIGHT and MJYD2S, add NRFUSE
  ---
  0.9.7.0 20200624  integrate - fix BEARSSL-decryption, remove MBEDTLS, prepare night light sensors
  ---
  0.9.6.1 20200622  integrate - use BEARSSL-lib for decryption as default, make decryption optional
  ---
  0.9.6.0 20200618  integrate - add decryption for LYWSD03
  ---
  0.9.5.0 20200328  integrate - add dew point, multi-page-web ui, refactoring, command interface,
                                simple beacon
  ---
  0.9.4.0 20200304  integrate - sensor types can be ignored (default for LYWSD03),
                                add CGD1 (Alarm clock), correct PDU-types for LYWSD02
  ---
  0.9.3.0 20200222  integrate - use now the correct id-word instead of MAC-OUI,
                                add CGG1
  ---
  0.9.2.0 20200212  integrate - "backports" from MI-HM10, change reading pattern,
                                add missing PDU-types, renaming driver
  ---
  0.9.1.0 20200117  integrate - Added support for the LYWSD02
  ---
  0.9.0.0 20191127  started - further development by Christian Baars
                    base    - code base from cbm80amiga, floe, Dmitry.GR
                    forked  - from arendst/tasmota            - https://github.com/arendst/Tasmota

*/

#ifdef USE_SPI
#ifdef USE_NRF24
#ifdef USE_MIBLE

#ifdef DEBUG_TASMOTA_SENSOR
  #define MINRF_LOG_BUFFER(x) MINRFshowBuffer(x);
#else
  #define MINRF_LOG_BUFFER(x)
#endif

#define USE_MI_DECRYPTION
/*********************************************************************************************\
* MINRF
* BLE-Sniffer/Bridge for MIJIA/XIAOMI Temperatur/Humidity-Sensor, Mi Flora, LYWSD02, GCx, ...
*
* Usage: Configure NRF24
\*********************************************************************************************/

#define XSNS_61             61

#include <vector>
#ifdef USE_MI_DECRYPTION
#include <bearssl/bearssl_block.h>
#endif //USE_MI_DECRYPTION

#define FLORA       1
#define MJ_HT_V1    2
#define LYWSD02     3
#define LYWSD03     4
#define CGG1        5
#define CGD1        6
#define NLIGHT      7
#define MJYD2S      8
#define YEERC       9

#define MI_TYPES    9 //count this manually

#define D_CMND_NRF "NRF"

const char S_JSON_NRF_COMMAND_NVALUE[] PROGMEM = "{\"" D_CMND_NRF "%s\":%d}";
const char S_JSON_NRF_COMMAND[] PROGMEM        = "{\"" D_CMND_NRF "%s\":\"%s\"}";
const char kNRF_Commands[] PROGMEM             = "Ignore|Use|Page|Scan|Beacon|Chan|Nlight"
#ifdef USE_MI_DECRYPTION
                                                  "|Mjyd2s"
                                                  "|Key"
#endif //USE_MI_DECRYPTION
                                                  ;

enum NRF_Commands {          // commands useable in console or rules
  CMND_NRF_IGNORE,           // ignore specific sensor type (1-9) --- DEPRECATED!!!!
  CMND_NRF_USE,              // use specific sensor type (1-9)
  CMND_NRF_PAGE,             // sensor entries per web page, which will be shown alternated
  CMND_NRF_SCAN,             // simplified passive BLE adv scan
  CMND_NRF_BEACON,           // even more simplified Beacon, reports time since last sighting
  CMND_NRF_CHAN,             // ignore channel 0-2 (translates to 37-39)
  CMND_NRF_NLIGHT            // add Philips night light via MAC
#ifdef USE_MI_DECRYPTION
  ,  CMND_NRF_MJYD2S         // add MJYD2S night light via bind_key to a MAC for payload decryption
  ,  CMND_NRF_KEY            // add bind_key to a MAC for payload decryption
#endif //USE_MI_DECRYPTION
  };

const uint16_t kMINRFDeviceID[MI_TYPES]={ 0x0098, // Flora
                                          0x01aa, // MJ_HT_V1
                                          0x045b, // LYWSD02
                                          0x055b, // LYWSD03
                                          0x0347, // CGG1
                                          0x0576, // CGD1
                                          0x03dd, // NLIGHT
                                          0x07f6, // MJYD2S
                                          0x0153  // yee-rc
                                          };

const char kMINRFDeviceType1[] PROGMEM = "Flora";
const char kMINRFDeviceType2[] PROGMEM = "MJ_HT_V1";
const char kMINRFDeviceType3[] PROGMEM = "LYWSD02";
const char kMINRFDeviceType4[] PROGMEM = "LYWSD03";
const char kMINRFDeviceType5[] PROGMEM = "CGG1";
const char kMINRFDeviceType6[] PROGMEM = "CGD1";
const char kMINRFDeviceType7[] PROGMEM = "NLIGHT";
const char kMINRFDeviceType8[] PROGMEM = "MJYD2S";
const char kMINRFDeviceType9[] PROGMEM = "YEERC";
const char * kMINRFDeviceType[] PROGMEM = {kMINRFDeviceType1,kMINRFDeviceType2,kMINRFDeviceType3,kMINRFDeviceType4,kMINRFDeviceType5,kMINRFDeviceType6,kMINRFDeviceType7,kMINRFDeviceType8,kMINRFDeviceType9};

// PDU's or different channels 37-39
const uint32_t kMINRFFloPDU[3] = {0x3eaa857d,0xef3b8730,0x71da7b46};
const uint32_t kMINRFMJPDU[3]  = {0x4760cd66,0xdbcc0cd3,0x33048df5};
const uint32_t kMINRFL2PDU[3]  = {0x3eaa057d,0xef3b0730,0x71dafb46};
const uint32_t kMINRFL3PDU[3]  = {0x4760dd78,0xdbcc1ccd,0x33049deb}; //encrypted - 58 58
// const uint32_t kMINRFL3PDU[3]  = {0x4760cb78,0xdbcc0acd,0x33048beb}; //unencrypted  - 30 58
const uint32_t kMINRFCGGPDU[3]  = {0x4760cd6e,0xdbcc0cdb,0x33048dfd};
const uint32_t kMINRFCGDPDU[3]  = {0x5da0d752,0xc10c16e7,0x29c497c1};
// const uint32_t kMINRFNLIPDU[3]  = {0x4760C56E,0xDBCC04DB,0x0330485FD}; //NLIGHT
const uint32_t kMINRFYRCPDU[3]  = {0x216D63E2,0x5C3DD47E,0x0A5D0E96};  //yee-rc - 50 30

// start-LSFR for different channels 37-39
const uint8_t kMINRFlsfrList_A[3] = {0x4b,0x17,0x23};  // Flora, LYWSD02
const uint8_t kMINRFlsfrList_B[3] = {0x21,0x72,0x43};  // MJ_HT_V1, LYWSD03, CGx
const uint8_t kMINRFlsfrList_C[3] = {0x38,0x25,0x2e};  // yee-rc


#pragma pack(1)  // important!!
struct mi_beacon_t{
  uint16_t PID;
  uint8_t counter;
  uint8_t MAC[6];
  uint8_t spare; // not on MJ_HT_V1 and CGG1
  uint8_t type;
  uint8_t ten;
  uint8_t size;
  union {
    struct{ //0d
      int16_t temp;
      uint16_t hum;
    }HT;
    uint8_t bat; //0a
    uint16_t temp; //04
    uint16_t hum; //06
    uint32_t lux:24; //07
    uint8_t moist; //08
    uint16_t fert; //09
    struct{ //01
      uint16_t num;
      uint8_t longPress; 
    }Btn; 
  };
};

struct CGDPacket_t {    // related to the whole 32-byte-packet/buffer
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

struct bleAdvPacket_t { // for nRF24L01 max 32 bytes = 2+6+24
  uint8_t pduType;
  uint8_t payloadSize;
  uint8_t MAC[6];
};

#ifdef USE_MI_DECRYPTION
struct encPayload_t {
  uint8_t cipher[5];
  uint8_t ExtCnt[3];
  uint8_t tag[4];
};

struct encPacket_t{
  // the packet is longer, but this part is enough to decrypt
  uint16_t PID;
  uint8_t frameCnt;
  uint8_t MAC[6];
  encPayload_t payload;
};

struct mjysd02_Packet_t{
  uint8_t padding[11];
  uint8_t payloadSize;
  uint8_t padding3;
  uint16_t UUID;
  uint16_t frameCtrl;
  uint16_t PID;
  uint8_t frameCnt;
  uint8_t data[18];
};

union mi_bindKey_t{
  struct{
    uint8_t key[16];
    uint8_t MAC[6];
    };
  uint8_t buf[22];
};
#endif //USE_MI_DECRYPTION
union FIFO_t{
  bleAdvPacket_t bleAdv;
  mi_beacon_t miBeacon;
  CGDPacket_t CGDPacket;
  uint8_t raw[32];
};

#pragma pack(0)

struct {
  const uint8_t channel[3]   = {37,38,39};  // BLE advertisement channel number
  const uint8_t frequency[3] = { 2,26,80};  // real frequency (2400+x MHz)

  uint16_t timer;
  uint16_t ignore = 0; //bitfield: 2^sensor type
  uint8_t currentChan=0;
  uint8_t channelIgnore = 0; //bitfield: 2^channel (0=37,1=38,2=39)
  uint8_t confirmedSensors = 0;
  uint8_t packetMode; // 0 - normal BLE-advertisements, 1 - 9 "special" sensor packets
  uint8_t perPage = 4;
  uint8_t firstUsedPacketMode = 1;
  uint8_t activeLight = 0;

  FIFO_t buffer;

  struct {
    uint8_t MAC[6];
    uint32_t time;
    uint32_t PDU[3];
    bool active = false;
  } beacon;
  bool activeScan = false;
  bool stopScan = false;
  bool triggeredTELE = false;

#ifdef DEBUG_TASMOTA_SENSOR
  uint8_t streamBuffer[sizeof(buffer)]; //  raw data stream bytes
  uint8_t lsfrBuffer[sizeof(buffer)];   //  correpsonding lfsr-bytes for the buffer, probably only useful for a BLE-packet
#endif //  DEBUG_TASMOTA_SENSOR

} MINRF;

struct mi_sensor_t{
  uint8_t type; //Flora = 1; MJ_HT_V1=2; LYWSD02=3; LYWSD03=4; CGG1=5; CGD1=6; YEERC=9
  uint8_t MAC[6];
  uint8_t showedUp;
  float temp; //Flora, MJ_HT_V1, LYWSD0x, CGx
  union {
    struct {
      float moisture;
      float fertility;
      uint32_t lux;
    }; // Flora
    struct {
      float hum;
      uint8_t bat;
    }; // MJ_HT_V1, LYWSD0x, CGx
    struct {
      uint8_t btn;
      uint8_t shallSendMQTT;
      uint8_t lastCnt;
    }; // yee-rc
  };
};

struct mi_light_t{
  uint8_t MAC[6];
  uint32_t PDU[3];
  uint8_t type; // NLIGHT=7, MJYD2S=8
  uint8_t bat;
  struct {
    uint16_t events; //"alarms" since boot
    uint8_t lastCnt; //device generated counter of the packet
    uint8_t shallSendMQTT;
  };
  uint32_t NMT; // no motion time in seconds for the MJYD2S
  uint32_t lastTime;
  uint8_t lux; //1 or 64 for the MJYD2S
  uint8_t eventType; //internal type of actual event for the MJYD2S

};

struct scan_entry_t {
  uint8_t MAC[6];
  uint16_t cid;
  uint16_t svc;
  uint16_t uuid;
  uint8_t showedUp;
};

std::vector<mi_sensor_t> MIBLEsensors;
std::vector<scan_entry_t> MINRFscanResult;
#ifdef USE_MI_DECRYPTION
std::vector<mi_bindKey_t> MIBLEbindKeys;
#endif //USE_MI_DECRYPTION
std::vector<mi_light_t> MIBLElights;

static union{
  scan_entry_t MINRFdummyEntry;
  uint8_t MINRFtempBuf[23];
};

/********************************************************************************************/

/**
 * @brief
 *
 * @param _mode Packet mode 0-9
 * @return true  If no error occured
 * @return false  If NRF24L01 is not connected
 */
bool MINRFinitBLE(uint8_t _mode)
{
  if (MINRF.timer%1000 == 0){ // only re-init every 20 seconds
    NRF24radio.begin(Pin(GPIO_SPI_CS),Pin(GPIO_SPI_DC));
    NRF24radio.setAutoAck(false);
    NRF24radio.setDataRate(RF24_1MBPS);
    NRF24radio.disableCRC();
    NRF24radio.setChannel( MINRF.frequency[MINRF.currentChan] );
    NRF24radio.setRetries(0,0);
    NRF24radio.setPALevel(RF24_PA_MIN); // we only receive
    NRF24radio.setAddressWidth(4);
    // NRF24radio.openReadingPipe(0,0x6B7D9171); // advertisement address: 0x8E89BED6 (bit-reversed -> 0x6B7D9171)
    // NRF24radio.openWritingPipe(  0x6B7D9171); // not used ATM
    NRF24radio.powerUp();
  }
  if(NRF24radio.isChipConnected()){
    // DEBUG_SENSOR_LOG(PSTR("MINRF chip connected"));
    MINRFchangePacketModeTo(_mode);
    return true;
  }
  // DEBUG_SENSOR_LOG(PSTR("MINRF chip NOT !!!! connected"));
  return false;
}

/**
 * @brief cycle through the channels 37-39, skip ignored channel
 *
 */
void MINRFhopChannel()
{
  for (uint32_t i = 0; i<3;i++){
    MINRF.currentChan++;
    if(bitRead(MINRF.channelIgnore,MINRF.currentChan)) continue;
    if(MINRF.currentChan >= sizeof(MINRF.channel)) {
      MINRF.currentChan = 0;
      if(bitRead(MINRF.channelIgnore,MINRF.currentChan)) continue;
    }
    break;
  }
  NRF24radio.setChannel( MINRF.frequency[MINRF.currentChan] );
}

/**
 * @brief Read out FIFO-buffer, swap buffer and whiten
 *
 * @return true - If something is in the buffer
 * @return false - Nothing is in the buffer
 */
bool MINRFreceivePacket(void)
{
  if(!NRF24radio.available()) {
    return false;
  }
  while(NRF24radio.available()) {
    // static uint8_t _lsfr = 0; //-> for testing out suitable lsfr-start-values for yet unknown packets
    // _lsfr++;
    NRF24radio.read( &MINRF.buffer, sizeof(MINRF.buffer) );
#ifdef DEBUG_TASMOTA_SENSOR
    memcpy(&MINRF.streamBuffer, &MINRF.buffer, sizeof(MINRF.buffer));
#endif // DEBUG_TASMOTA_SENSOR
    MINRFswapbuf((uint8_t*)&MINRF.buffer, sizeof(MINRF.buffer) );
    // MINRF_LOG_BUFFER();

    // AddLog_P2(LOG_LEVEL_INFO,PSTR("MINRF: _lsfrlist: %x, chan: %u, mode: %u"),_lsfrlist[MINRF.currentChan],MINRF.currentChan, MINRF.packetMode);
    switch (MINRF.packetMode) {
      case 0: case 7: case 8: 
      MINRFwhiten((uint8_t *)&MINRF.buffer, sizeof(MINRF.buffer),  MINRF.channel[MINRF.currentChan] | 0x40); // "BEACON" mode, "NLIGHT" mode, "MJYD2S" mode
      break;
      case 1: case 3:
      MINRFwhiten((uint8_t *)&MINRF.buffer, sizeof(MINRF.buffer),  kMINRFlsfrList_A[MINRF.currentChan]); // "flora" mode, "LYWSD02" mode
      break;
      case 2: case 4: case 5: case 6:
      MINRFwhiten((uint8_t *)&MINRF.buffer, sizeof(MINRF.buffer),  kMINRFlsfrList_B[MINRF.currentChan]); // "MJ_HT_V1" mode, LYWSD03" mode, "CGG1" mode, "CGD1" mode
      break;
      case 9:
      MINRFwhiten((uint8_t *)&MINRF.buffer, sizeof(MINRF.buffer),  kMINRFlsfrList_C[MINRF.currentChan]); // "YEE-RC" mode
      break;
    }
    // DEBUG_SENSOR_LOG(PSTR("MINRF: LSFR:%x"),_lsfr);
    // if (_lsfr>254) _lsfr=0;
  }
  // DEBUG_SENSOR_LOG(PSTR("MINRF: did read FIFO"));
  return true;
}

// #ifdef DEBUG_TASMOTA_SENSOR
void MINRFshowBuffer(uint8_t (&buf)[32]){ // we use this only for the 32-byte-FIFO-buffer, so 32 is hardcoded
  // DEBUG_SENSOR_LOG(PSTR("MINRF: Buffer: %c %c %c %c %c %c %c %c"
  //                                       " %c %c %c %c %c %c %c %c"
  //                                       " %c %c %c %c %c %c %c %c"
  //                                       " %c %c %c %c %c %c %c %c")
  DEBUG_SENSOR_LOG(PSTR("MINRF: Buffer: %02x %02x %02x %02x %02x %02x %02x %02x "
                                        "%02x %02x %02x %02x %02x %02x %02x %02x "
                                        "%02x %02x %02x %02x %02x %02x %02x %02x "
                                        "%02x %02x %02x %02x %02x %02x %02x %02x ")
  ,buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7],buf[8],buf[9],buf[10],buf[11],
  buf[12],buf[13],buf[14],buf[15],buf[16],buf[17],buf[18],buf[19],buf[20],buf[21],buf[22],buf[23],
  buf[24],buf[25],buf[26],buf[27],buf[28],buf[29],buf[30],buf[31]
  );
}
// #endif // DEBUG_TASMOTA_SENSOR

/**
 * @brief change lsfrBuffer content to "wire bit order"
 *
 * @param len Buffer lenght (could be hardcoded to 32)
 */
void MINRFswapbuf(uint8_t *buf, uint8_t len)
{
  // uint8_t* buf = (uint8_t*)&MINRF.buffer;
  while(len--) {
    uint8_t a = *buf;
    uint8_t v = 0;
    if (a & 0x80) v |= 0x01;
    if (a & 0x40) v |= 0x02;
    if (a & 0x20) v |= 0x04;
    if (a & 0x10) v |= 0x08;
    if (a & 0x08) v |= 0x10;
    if (a & 0x04) v |= 0x20;
    if (a & 0x02) v |= 0x40;
    if (a & 0x01) v |= 0x80;
    *(buf++) = v;
  }
}

/**
 * @brief Whiten the packet buffer
 *
 * @param buf   The packet buffer
 * @param len   Lenght of the packet buffer
 * @param lfsr  Start lsfr-byte
 */
void MINRFwhiten(uint8_t *buf, uint8_t len, uint8_t lfsr)
{
  while(len--) {
    uint8_t res = 0;
    // LFSR in "wire bit order"
    for (uint8_t i = 1; i; i <<= 1) {
      if (lfsr & 0x01) {
        lfsr ^= 0x88;
        res |= i;
      }
      lfsr >>= 1;
    }
    *(buf++) ^= res;
#ifdef DEBUG_TASMOTA_SENSOR
    MINRF.lsfrBuffer[31-len] = lfsr;
#endif //DEBUG_TASMOTA_SENSOR
  }
}

/*********************************************************************************************\
 * Beacon functions
\*********************************************************************************************/
bool MINRFhandleBeacon(scan_entry_t * entry, uint32_t offset);

/**
 * @brief handle a generic BLE-packet in the scan process
 *
 */
void MINRFhandleScan(void){
  if(MINRFscanResult.size()>20 || MINRF.stopScan) {
     MINRF.activeScan=false;
     MINRFcomputefirstUsedPacketMode();
    uint32_t i = 0; // pass counter as reference to lambda
    MINRFscanResult.erase(std::remove_if(MINRFscanResult.begin(),
                          MINRFscanResult.end(),
                          [&i](scan_entry_t e) {
                            if(e.showedUp>2) AddLog_P2(LOG_LEVEL_INFO,PSTR("MINRF: Beacon %02u: %02X%02X%02X%02X%02X%02X Cid: %04X Svc: %04X UUID: %04X"),i,e.MAC[0],e.MAC[1],e.MAC[2],e.MAC[3],e.MAC[4],e.MAC[5],e.cid,e.svc,e.uuid);
                            i++;
                            return ((e.showedUp < 3));
                            }),
                          MINRFscanResult.end());
    MINRF.stopScan=false;
    return;
  }

  MINRFreverseMAC(MINRF.buffer.bleAdv.MAC);
  for(uint32_t i=0; i<MINRFscanResult.size(); i++){
    if(memcmp(MINRF.buffer.bleAdv.MAC,MINRFscanResult[i].MAC,sizeof(MINRF.buffer.bleAdv.MAC))==0){
      MINRFscanResult[i].showedUp++;
      // AddLog_P2(LOG_LEVEL_INFO,PSTR("MINRF: ADVk: %02x %02x %02x %02x %02x %02x"),MINRF.buffer.bleAdv.MAC[0],MINRF.buffer.bleAdv.MAC[1],MINRF.buffer.bleAdv.MAC[2],MINRF.buffer.bleAdv.MAC[3],MINRF.buffer.bleAdv.MAC[4],MINRF.buffer.bleAdv.MAC[5]);
      return;
    }
  }
  if(MINRF.buffer.raw[8]!=2 && MINRF.buffer.raw[9]!=1) return; //unsupported packet
  scan_entry_t _new;
  _new.showedUp = 1;
  _new.cid = 0;
  _new.svc = 0;
  _new.uuid = 0;
  memcpy(_new.MAC,MINRF.buffer.bleAdv.MAC,sizeof(_new.MAC));
  memcpy(MINRF.beacon.MAC,MINRF.buffer.bleAdv.MAC,sizeof(_new.MAC));
  if (MINRFhandleBeacon(&_new,0)){
    MINRFscanResult.push_back(_new);
  }
}

/**
 * @brief start beacon mode, can co-exist with Mijia-sniffing
 *
 * @param entry number of entry in scan list
 */
void MINRFstartBeacon(uint16_t entry){
  memcpy(MINRF.beacon.MAC,MINRFscanResult[entry].MAC,sizeof(MINRF.beacon.MAC));
  AddLog_P2(LOG_LEVEL_INFO,PSTR("MINRF: Beacon activated: %02x:%02x:%02x:%02x:%02x:%02x"),MINRF.beacon.MAC[0],MINRF.beacon.MAC[1],MINRF.beacon.MAC[2],MINRF.beacon.MAC[3],MINRF.beacon.MAC[4],MINRF.beacon.MAC[5]);
  MINRF.beacon.time = 0;
  MINRF.beacon.active = true;
}

/**
 * @brief semi-generic BLE-ADV-parser
 *
 * @param entry Entry of scan list
 * @param offset Depends on the reading mode: 0->regular BLE-ADV, 6->"cutted" BLE-ADV with MAC as PDU
 * @return true - when name, cid, uuid or svc is found with any value
 * @return false  - name, cid, uuid and svc are not found
 */
bool MINRFhandleBeacon(scan_entry_t * entry, uint32_t offset){
  bool success = false;
  uint8_t _buf[32+offset];
  MINRFwhiten((uint8_t *)&MINRF.buffer, sizeof(MINRF.buffer), MINRF.channel[MINRF.currentChan] | 0x40);
  MINRFswapbuf((uint8_t*)&MINRF.buffer,sizeof(MINRF.buffer));
  memcpy((uint8_t*)&_buf+offset,MINRF.buffer.raw,32);
  MINRFswapbuf((uint8_t*)&_buf,sizeof(_buf));
  MINRFwhiten((uint8_t *)&_buf, sizeof(_buf), MINRF.channel[MINRF.currentChan] | 0x40);
  if (offset == 6) MINRFreverseMAC((uint8_t*)&_buf[2]);

  if(memcmp((uint8_t*)&_buf[2],MINRF.beacon.MAC,2)==0){ // always at least 2 undestroyed bytes left
    if(_buf[8]!=2 && _buf[9]!=1){
      DEBUG_SENSOR_LOG(PSTR("MINRF: unsupported ADV %02x %02x"), _buf[8],_buf[9]);
      return success;
    }
    AddLog_P2(LOG_LEVEL_DEBUG,PSTR("MINRF: Beacon:____________"));
    for (uint32_t i = 8; i<32+offset;i++){
      uint32_t size = _buf[i];
      if (size>30) break;
      uint32_t ADtype = _buf[i+1];
      // AddLog_P2(LOG_LEVEL_DEBUG,PSTR("MINRF: Size: %u AD: %x i:%u"), size, ADtype,i);
      if (size+i>32+offset) size=32-i+offset-2;
      if (size>30) break;
      char _stemp[(size*2)];
      uint32_t backupSize;
      switch(ADtype){
        case 0x01:
          AddLog_P2(LOG_LEVEL_DEBUG,PSTR("MINRF: Flags: %02x"), _buf[i+2]);
          break;
        case 0x02: case 0x03:
          entry->uuid = _buf[i+3]*256 + _buf[i+2];
          AddLog_P2(LOG_LEVEL_DEBUG,PSTR("MINRF: UUID: %04x"), entry->uuid);
          success = true;
          break;
        case 0x08: case 0x09:
          backupSize = _buf[i+size+1];
          _buf[i+size+1] = 0;
          AddLog_P2(LOG_LEVEL_DEBUG,PSTR("MINRF: Name: %s"), (char*)&_buf[i+2]);
          success = true;
          _buf[i+size+1] = backupSize;
          break;
        case 0x0a:
          AddLog_P2(LOG_LEVEL_DEBUG,PSTR("MINRF: TxPow: %02u"), _buf[i+2]);
          break;
        case 0xff:
          entry->cid = _buf[i+3]*256 + _buf[i+2];
          AddLog_P2(LOG_LEVEL_DEBUG,PSTR("MINRF: Cid: %04x"), entry->cid);
          ToHex_P((unsigned char*)&_buf+i+4,size-3,_stemp,(size*2));
          AddLog_P2(LOG_LEVEL_DEBUG,PSTR("%s"),_stemp);
          success = true;
          break;
        case 0x16:
          entry->svc = _buf[i+3]*256 + _buf[i+2];
          AddLog_P2(LOG_LEVEL_DEBUG,PSTR("MINRF: Svc: %04x"), entry->svc);
          ToHex_P((unsigned char*)&_buf+i+4,size-3,_stemp,(size*2));
          AddLog_P2(LOG_LEVEL_DEBUG,PSTR("%s"),_stemp);
          success = true;
          break;
        default:
          ToHex_P((unsigned char*)&_buf+i+2,size-1,_stemp,(size*2));
          AddLog_P2(LOG_LEVEL_DEBUG,PSTR("%s"),_stemp);
      }
      i+=size;
    }
    MINRF.beacon.time = 0;
  }
  return success;
}

/**
 * @brief increase beacon timer every second and process the result
 *
 */
void MINRFbeaconCounter(void) {
  if (MINRF.beacon.active) {
    MINRF.beacon.time++;
/*
    char stemp[20];
    snprintf_P(stemp, sizeof(stemp),PSTR("{%s:{\"Beacon\": %u}}"),D_CMND_NRF, MINRF.beacon.time);
    AddLog_P2(LOG_LEVEL_DEBUG, stemp);
    RulesProcessEvent(stemp);
*/
    Response_P(PSTR("{%s:{\"Beacon\":%u}}"), D_CMND_NRF, MINRF.beacon.time);
    XdrvRulesProcess();
  }
}

/**
 * @brief compute "PDU" from MAC for each possible channel and store it globally
 *
 */
void MINRFcomputeBeaconPDU(uint8_t (&_MAC)[6], uint32_t (&PDU)[3], uint32_t offset){
  uint32_t _PDU[3];
  for (uint32_t i = 0; i<3; i++){
    bleAdvPacket_t packet;
    memcpy((uint8_t *)&packet.MAC, (uint8_t *)&_MAC, sizeof(packet.MAC));
    MINRFreverseMAC(packet.MAC);
    MINRFwhiten((uint8_t *)&packet, sizeof(packet), MINRF.channel[i] | 0x40);
    MINRFswapbuf((uint8_t*)&packet,sizeof(packet));
    uint32_t pdu = packet.MAC[0+offset]<<24 | packet.MAC[1+offset]<<16 | packet.MAC[2+offset]<<8 | packet.MAC[3+offset];
    _PDU[i] = pdu;
  }
  memcpy(PDU,_PDU,sizeof(_PDU));
}

#ifdef USE_MI_DECRYPTION
int MINRFdecryptPacket(char *_buf){
  encPacket_t *packet = (encPacket_t*)_buf;
  // AddLog_P2(LOG_LEVEL_DEBUG,PSTR("to decrypt: %02x %02x %02x %02x %02x %02x %02x %02x"),(uint8_t)_buf[0],(uint8_t)_buf[1],(uint8_t)_buf[2],(uint8_t)_buf[3],(uint8_t)_buf[4],(uint8_t)_buf[5],(uint8_t)_buf[6],(uint8_t)_buf[7]);
  // AddLog_P2(LOG_LEVEL_DEBUG,PSTR("          : %02x %02x %02x %02x %02x %02x %02x %02x"),(uint8_t)_buf[8],(uint8_t)_buf[9],(uint8_t)_buf[10],(uint8_t)_buf[11],(uint8_t)_buf[12],(uint8_t)_buf[13],(uint8_t)_buf[14],(uint8_t)_buf[15]);
  // AddLog_P2(LOG_LEVEL_DEBUG,PSTR("          : %02x %02x %02x %02x %02x               "),(uint8_t)_buf[16],(uint8_t)_buf[17],(uint8_t)_buf[18],(uint8_t)_buf[19],(uint8_t)_buf[20]);

  int ret = 0;
  unsigned char output[16] = {0};
  uint8_t nonce[12];
  const unsigned char authData[1] = {0x11};

  // nonce: device MAC, device type, frame cnt, ext. cnt
  for (uint32_t i = 0; i<6; i++){
    nonce[i] = packet->MAC[5-i];
  }
  memcpy((uint8_t*)&nonce+6,(uint8_t*)&packet->PID,2);
  nonce[8] = packet->frameCnt;
  memcpy((uint8_t*)&nonce+9,(uint8_t*)&packet->payload.ExtCnt,3);

  uint8_t _bindkey[16] = {0x0};
  for(uint32_t i=0; i<MIBLEbindKeys.size(); i++){
    if(memcmp(packet->MAC,MIBLEbindKeys[i].MAC,sizeof(packet->MAC))==0){
      // AddLog_P2(LOG_LEVEL_DEBUG,PSTR("have key"));
      memcpy(_bindkey,MIBLEbindKeys[i].key,sizeof(_bindkey));
      break;
    }
    // else{
    // AddLog_P2(LOG_LEVEL_DEBUG,PSTR("MAC in packet: %02x  %02x  %02x  %02x  %02x  %02x"), packet->MAC[0], packet->MAC[1], packet->MAC[2], packet->MAC[3], packet->MAC[4], packet->MAC[5]);
    // AddLog_P2(LOG_LEVEL_DEBUG,PSTR("MAC in vector: %02x  %02x  %02x  %02x  %02x  %02x"), MIBLEbindKeys[i].MAC[0], MIBLEbindKeys[i].MAC[1], MIBLEbindKeys[i].MAC[2], MIBLEbindKeys[i].MAC[3], MIBLEbindKeys[i].MAC[4], MIBLEbindKeys[i].MAC[5]);
    // }
  }

  memcpy(output,packet->payload.cipher, sizeof(packet->payload.cipher));

  br_aes_small_ctrcbc_keys keyCtx;
  br_aes_small_ctrcbc_init(&keyCtx, _bindkey, sizeof(_bindkey));

  br_ccm_context ctx;
  br_ccm_init(&ctx, &keyCtx.vtable);
  br_ccm_reset(&ctx, nonce, sizeof(nonce), sizeof(authData),sizeof(packet->payload.cipher),sizeof(packet->payload.tag));
	br_ccm_aad_inject(&ctx, authData, sizeof(authData));
	br_ccm_flip(&ctx);
  br_ccm_run(&ctx, 0, output, sizeof(packet->payload.cipher));

  ret = br_ccm_check_tag(&ctx, packet->payload.tag);
  AddLog_P2(LOG_LEVEL_DEBUG,PSTR("BEARSSL: Err:%i, Decrypted : %02x  %02x  %02x  %02x  %02x "), ret, output[0],output[1],output[2],output[3],output[4]);
  memcpy((uint8_t*)(packet->payload.cipher)+1,output,sizeof(packet->payload.cipher));
  return ret;
}

int MINRFdecryptMJYD2SPacket(char *_buf, uint8_t _light, char* _output){
  int ret = 0;
  uint8_t nonce[12];
  const unsigned char authData[1] = {0x11};
  uint8_t tag[4];
  mjysd02_Packet_t *packet = (mjysd02_Packet_t*)_buf;

  // nonce: device MAC, device type, frame cnt, ext. cnt
  for (uint32_t i = 0; i<6; i++){
    nonce[i] = MIBLElights[_light-1].MAC[5-i];
  }
  memcpy((uint8_t*)&nonce+6,(uint8_t*)&packet->PID,2);
  nonce[8] = packet->frameCnt;
  memcpy((uint8_t*)&nonce+9,(uint8_t*)&packet->padding[0] + packet->payloadSize + 5, 3);
  // AddLog_P2(LOG_LEVEL_DEBUG,PSTR("nonce: %02x  %02x  %02x  %02x  %02x  %02x %02x  %02x  %02x  %02x  %02x  %02x"), nonce[0], nonce[1], nonce[2], nonce[3], nonce[4], nonce[5], nonce[6], nonce[7], nonce[8], nonce[9], nonce[10], nonce[11]);

  uint8_t _bindkey[16];
  for(uint32_t i=0; i<MIBLEbindKeys.size(); i++){
    if(memcmp(MIBLElights[_light-1].MAC,MIBLEbindKeys[i].MAC,sizeof(MIBLElights[_light-1].MAC))==0){
      AddLog_P2(LOG_LEVEL_DEBUG,PSTR("have key"));
      memcpy(_bindkey,MIBLEbindKeys[i].key,sizeof(_bindkey));
      break;
    }
    // else{
    // AddLog_P2(LOG_LEVEL_DEBUG,PSTR("MAC in packet: %02x  %02x  %02x  %02x  %02x  %02x"), packet->MAC[0], packet->MAC[1], packet->MAC[2], packet->MAC[3], packet->MAC[4], packet->MAC[5]);
    // AddLog_P2(LOG_LEVEL_DEBUG,PSTR("MAC in vector: %02x  %02x  %02x  %02x  %02x  %02x"), MIBLEbindKeys[i].MAC[0], MIBLEbindKeys[i].MAC[1], MIBLEbindKeys[i].MAC[2], MIBLEbindKeys[i].MAC[3], MIBLEbindKeys[i].MAC[4], MIBLEbindKeys[i].MAC[5]);
    // }
  }

  // AddLog_P2(LOG_LEVEL_DEBUG,PSTR("size %u"),packet->payloadSize);
  uint32_t _size;
  int32_t _offset;
  uint32_t _tagSize;
  switch (packet->payloadSize){
  case 22:
    _size = 7;
    _offset = 2;
    _tagSize = 4;
    break;
  case 25:
    _size = packet->payloadSize - 21;
    _offset = -1;
    _tagSize = 4;
    break;
  case 27:
    _size = packet->payloadSize - 21;
    _offset = 1;
    _tagSize = 3;
    break;
  default:
    return 0;
    break;
  }
  // AddLog_P2(LOG_LEVEL_DEBUG,PSTR("size %u , offset %u"),_size,_offset);
  memcpy(_output,(uint8_t*)&packet->padding[0] + packet->payloadSize - _offset, _size);
  // AddLog_P2(LOG_LEVEL_DEBUG,PSTR("BEARSSL: Output : %02x  %02x  %02x  %02x  %02x %02x  %02x"), _output[0], _output[1],_output[2],_output[3],_output[4],_output[5],_output[6]);

  br_aes_small_ctrcbc_keys keyCtx;
  br_aes_small_ctrcbc_init(&keyCtx, _bindkey, sizeof(_bindkey));

  br_ccm_context ctx;
  br_ccm_init(&ctx, &keyCtx.vtable);
  br_ccm_reset(&ctx, nonce, sizeof(nonce), sizeof(authData),_size,4);
	br_ccm_aad_inject(&ctx, authData, sizeof(authData));
	br_ccm_flip(&ctx);
  br_ccm_run(&ctx, 0, _output, _size);
  // AddLog_P2(LOG_LEVEL_DEBUG,PSTR("BEARSSL: Err:%i, Decrypted : %02x  %02x  %02x  %02x  %02x %02x  %02x"), ret, _output[0], _output[1],_output[2],_output[3],_output[4],_output[5],_output[6]);

  br_ccm_get_tag(&ctx, tag);
  ret = memcmp(tag,(uint8_t*)&packet->padding[0] + packet->payloadSize + 8, _tagSize);
  return ret;
}
#endif //USE_MI_DECRYPTION

/*********************************************************************************************\
 * helper functions
\*********************************************************************************************/

/**
 * @brief reverse 6-byte-array, hard-coded size of 6
 *
 * @param _MAC pass an uint_t[6]
 */
void MINRFreverseMAC(uint8_t _MAC[]){
  uint8_t _reversedMAC[6];
  for (uint8_t i=0; i<6; i++){
    _reversedMAC[5-i] = _MAC[i];
  }
  memcpy(_MAC,_reversedMAC, sizeof(_reversedMAC));
}
#ifdef USE_MI_DECRYPTION
void MINRFAddKey(char* payload){
  mi_bindKey_t keyMAC;
  memset(keyMAC.buf,0,sizeof(keyMAC));
  MINRFKeyMACStringToBytes(payload,keyMAC.buf);
  bool unknownKey = true;
  for(uint32_t i=0; i<MIBLEbindKeys.size(); i++){
    if(memcmp(keyMAC.MAC,MIBLEbindKeys[i].MAC,sizeof(keyMAC.MAC))==0){
      DEBUG_SENSOR_LOG(PSTR("Known MAC for key"));
      unknownKey=false;
    }
  }
  if(unknownKey){
    DEBUG_SENSOR_LOG(PSTR("Key for new MAC"));
    MIBLEbindKeys.push_back(keyMAC);
  }
}

/**
 * @brief Convert combined key-MAC-string to
 *
 * @param _string input string in format: AABBCCDDEEFF... (upper case!), must be 44 chars!!
 * @param _MAC  target byte array with fixed size of 16 + 6
 */
void MINRFKeyMACStringToBytes(char* _string,uint8_t _keyMAC[]) { //uppercase
    uint32_t index = 0;
    while (index < 44) {
        char c = _string[index];
        uint8_t value = 0;
        if(c >= '0' && c <= '9')
          value = (c - '0');
        else if (c >= 'A' && c <= 'F')
          value = (10 + (c - 'A'));
        _keyMAC[(index/2)] += value << (((index + 1) % 2) * 4);
        index++;
    }
    DEBUG_SENSOR_LOG(PSTR("MINRF:  %s to:"),_string);
    DEBUG_SENSOR_LOG(PSTR("MINRF:  key-array: %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X"),_keyMAC[0],_keyMAC[1],_keyMAC[2],_keyMAC[3],_keyMAC[4],_keyMAC[5],_keyMAC[6],_keyMAC[7],_keyMAC[8],_keyMAC[9],_keyMAC[10],_keyMAC[11],_keyMAC[12],_keyMAC[13],_keyMAC[14],_keyMAC[15]);
    DEBUG_SENSOR_LOG(PSTR("MINRF: MAC-array: %02X%02X%02X%02X%02X%02X"),_keyMAC[16],_keyMAC[17],_keyMAC[18],_keyMAC[19],_keyMAC[20],_keyMAC[21]);
}
#endif //USE_MI_DECRYPTION
/**
 * @brief
 *
 * @param _string input string in format: AABBCCDDEEFF (upper case!)
 * @param _MAC  target byte array with fixed size of 6
 */
void MINRFMACStringToBytes(char* _string, uint8_t _MAC[]) { //uppercase
    uint32_t index = 0;
    while (index < 12) {
        char c = _string[index];
        uint8_t value = 0;
        if(c >= '0' && c <= '9')
          value = (c - '0');
        else if (c >= 'A' && c <= 'F')
          value = (10 + (c - 'A'));
        _MAC[(index/2)] += value << (((index + 1) % 2) * 4);
        index++;
    }
    // DEBUG_SENSOR_LOG(PSTR("MINRF:  %s to MAC-array: %02X%02X%02X%02X%02X%02X"),_string,_MAC[0],_MAC[1],_MAC[2],_MAC[3],_MAC[4],_MAC[5]);
}

/**
 * @brief helper function, to avoid to start with an ignored sensor type
 *
 */
void MINRFcomputefirstUsedPacketMode(void){
  for (uint32_t i = 0; i<MI_TYPES; i++){
    if (!bitRead(MINRF.ignore,i+1)) {
      DEBUG_SENSOR_LOG(PSTR("MINRF: FPM: %u"),i+1);
      MINRF.firstUsedPacketMode = i+1;
      if(MINRF.firstUsedPacketMode>MI_TYPES) MINRF.firstUsedPacketMode=0;
      break;
    }
  }
}

/**
 * @brief Recalculates the receive buffer with an offset in relation to a standard BLE advertisement.
 *        Used for custom PDU, typically based on a MAC
 * 
 * @param _buf - The receive buffer
 * @param offset - in bytes
 */

void MINRFrecalcBuffer(uint8_t *_buf, uint32_t offset){
  MINRFwhiten((uint8_t *)&MINRF.buffer, sizeof(MINRF.buffer), MINRF.channel[MINRF.currentChan] | 0x40);
  MINRFswapbuf((uint8_t*)&MINRF.buffer,sizeof(MINRF.buffer));
  memcpy(_buf+offset,MINRF.buffer.raw,32);
  MINRFswapbuf(_buf,32+offset);
  MINRFwhiten(_buf, 32+offset, MINRF.channel[MINRF.currentChan] | 0x40);
}

/**
 * @brief Set packet mode and fitting PDU-type of the NRF24L01
 *
 * @param _mode The internal packet mode number
 */
void MINRFchangePacketModeTo(uint8_t _mode) {
  uint32_t (_nextchannel) = MINRF.currentChan+1;
  if (_nextchannel>2) _nextchannel=0;

  switch(_mode){
    case 0: // normal BLE advertisement
      NRF24radio.openReadingPipe(0,0x6B7D9171); // advertisement address: 0x8E89BED6 (bit-reversed -> 0x6B7D9171)
    break;
    case 1: // special flora packet
      NRF24radio.openReadingPipe(0,kMINRFFloPDU[_nextchannel]); // 95 fe 71 20 -> flora
    break;
    case 2: // special MJ_HT_V1 packet
      NRF24radio.openReadingPipe(0,kMINRFMJPDU[_nextchannel]); // 95 fe 50 20 -> MJ_HT_V1
    break;
    case 3: // special LYWSD02 packet
      NRF24radio.openReadingPipe(0,kMINRFL2PDU[_nextchannel]);// 95 fe 70 20 -> LYWSD02
    break;
    case 4: // special LYWSD03 packet
      NRF24radio.openReadingPipe(0,kMINRFL3PDU[_nextchannel]);// 95 fe 58 58 -> LYWSD03 (= encrypted data message)
    break;
    case 5: // special CGG1 packet
      NRF24radio.openReadingPipe(0,kMINRFCGGPDU[_nextchannel]); // 95 fe 50 30 -> CGG1
    break;
    case 6: // special CGD1 packet
      NRF24radio.openReadingPipe(0,kMINRFCGDPDU[_nextchannel]); // cd fd 08 0c -> CGD1
    break;
    case 7: case 8:// MAC based LIGHT packet
      if (MIBLElights.size()==0) break;
      NRF24radio.openReadingPipe(0,MIBLElights[MINRF.activeLight].PDU[_nextchannel]); // computed from MAC -> NLIGHT and MJYSD2S
      MINRF.activeLight++;
    break;
    case 9: // YEE-RC packet 
      NRF24radio.openReadingPipe(0,kMINRFYRCPDU[_nextchannel]);// 95 fe 50 30 -> YEE-RC
    break;
  }
  // DEBUG_SENSOR_LOG(PSTR("MINRF: Change Mode to %u"),_mode);
  MINRF.packetMode = _mode;
}

/**
 * @brief Return the slot number of a known sensor or return create new sensor slot
 *
 * @param _MAC     BLE address of the sensor
 * @param _type       Type number of the sensor
 * @return uint32_t   Known or new slot in the sensors-vector
 */
uint32_t MINRFgetSensorSlot(uint8_t (&_MAC)[6], uint16_t _type){

  DEBUG_SENSOR_LOG(PSTR("MINRF: will test ID-type: %x"), _type);
  bool _success = false;
  for (uint32_t i=0;i<MI_TYPES;i++){ // i < sizeof(kMINRFDeviceID) gives compiler warning
    if(_type == kMINRFDeviceID[i]){
      DEBUG_SENSOR_LOG(PSTR("MINRF: ID is type %u"), i);
      _type = i+1;
      _success = true;
    }
    else {
      DEBUG_SENSOR_LOG(PSTR("MINRF: ID-type is not: %x"),kMINRFDeviceID[i]);
    }
  }
  if(!_success) return 0xff;

  DEBUG_SENSOR_LOG(PSTR("MINRF: vector size %u"), MIBLEsensors.size());
  for(uint32_t i=0; i<MIBLEsensors.size(); i++){
    if(memcmp(_MAC,MIBLEsensors[i].MAC,sizeof(_MAC))==0){
      DEBUG_SENSOR_LOG(PSTR("MINRF: known sensor at slot: %u"), i);
        if(MIBLEsensors[i].showedUp < 3){ // if we got an intact packet, the sensor should show up several times
          MIBLEsensors[i].showedUp++;     // count up to the above number ... now we are pretty sure
          DEBUG_SENSOR_LOG(PSTR("MINRF: showed up %u"),MIBLEsensors[i].showedUp);
          MINRFconfirmSensors();
      }
      return i;
    }
    DEBUG_SENSOR_LOG(PSTR("MINRF i: %x %x %x %x %x %x"), MIBLEsensors[i].MAC[5], MIBLEsensors[i].MAC[4],MIBLEsensors[i].MAC[3],MIBLEsensors[i].MAC[2],MIBLEsensors[i].MAC[1],MIBLEsensors[i].MAC[0]);
    DEBUG_SENSOR_LOG(PSTR("MINRF n: %x %x %x %x %x %x"), _MAC[5], _MAC[4], _MAC[3],_MAC[2],_MAC[1],_MAC[0]);
  }

  DEBUG_SENSOR_LOG(PSTR("MINRF: found new sensor"));
  mi_sensor_t _newSensor;
  memcpy(_newSensor.MAC,_MAC, sizeof(_MAC));
  _newSensor.type = _type;
  _newSensor.showedUp = 1;
  _newSensor.temp =NAN;
  switch (_type)
    {
    case 1:
      _newSensor.moisture =NAN;
      _newSensor.fertility =NAN;
      _newSensor.lux = 0xffffffff;
      break;
    case 2: case 3: case 4: case 5: case 6:
      _newSensor.hum=NAN;
      _newSensor.bat=0x00;
      break;
    default:
      break;
    }
  MIBLEsensors.push_back(_newSensor);
  AddLog_P2(LOG_LEVEL_INFO,PSTR("MINRF: new %s at slot: %u"),kMINRFDeviceType[_type-1], MIBLEsensors.size()-1);
  return (MIBLEsensors.size()-1);
};

/**
 * @brief Remove "zombie" sensors after a certain amount of time.
 *        If they showed up less than 3 times, they are probably
 *        a product of data corruption.
 */
void MINRFpurgeFakeSensors(void){
  for(uint32_t i=0; i<MIBLEsensors.size(); i++){
    DEBUG_SENSOR_LOG(PSTR("MINRF: remove FAKE %s at slot: %u"),kMINRFDeviceType[MIBLEsensors[i].type-1], i);
    MIBLEsensors.erase(std::remove_if(MIBLEsensors.begin(),
                      MIBLEsensors.end(),
                      [](mi_sensor_t i) { return ((i.showedUp < 3 || bitRead(MINRF.ignore,i.type))); }),
                      MIBLEsensors.end());
  }
  MINRFconfirmSensors();
}

/**
 * @brief count the sensors, that have sended data multiple times
 *        these are very likely real and not the result of corrupted data
 */
void MINRFconfirmSensors(void){
  MINRF.confirmedSensors = 0;
  for(uint32_t i=0; i<MIBLEsensors.size(); i++){
    if(MIBLEsensors[i].showedUp > 2){
      MINRF.confirmedSensors++;
    }
  }
}

/**
 * @brief trigger real-time message for PIR or RC
 * 
 */
void MINRFtriggerTele(void){
    MINRF.triggeredTELE = true;
    mqtt_data[0] = '\0';
    if (MqttShowSensor()) {
      MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR), Settings.flag.mqtt_sensor_retain);
  #ifdef USE_RULES
      RulesTeleperiod();  // Allow rule based HA messages
  #endif  // USE_RULES
    }
}

/**
 * @brief generic MiBeacon parser
 *
 */
void MINRFhandleMiBeaconPacket(void){
  MINRFreverseMAC(MINRF.buffer.miBeacon.MAC);
  uint32_t _slot = MINRFgetSensorSlot(MINRF.buffer.miBeacon.MAC, MINRF.buffer.miBeacon.PID);
  if(_slot==0xff) return;
  DEBUG_SENSOR_LOG(PSTR("MINRF: slot %u, size vector: %u %u"),_slot,MIBLEsensors.size());
  mi_sensor_t *_sensorVec = &MIBLEsensors.at(_slot);
  DEBUG_SENSOR_LOG(PSTR("MINRF: %u %u %u"),_slot,_sensorVec->type,MINRF.buffer.miBeacon.type);
  float _tempFloat;
  int decryptRet;

  switch(_sensorVec->type){
    case MJ_HT_V1: case CGG1: case YEERC:
    memcpy(MINRFtempBuf,(uint8_t*)&MINRF.buffer.miBeacon.spare, 32-9); // shift by one byte for the MJ_HT_V1 and CGG1
    memcpy((uint8_t*)&MINRF.buffer.miBeacon.type,MINRFtempBuf, 32-9);  // shift by one byte for the MJ_HT_V1 and CGG1
    break;
#ifdef USE_MI_DECRYPTION
    case LYWSD03:
    decryptRet = MINRFdecryptPacket((char*)&MINRF.buffer); //start with PID
    if(decryptRet==1) _sensorVec->showedUp=255; // if decryption worked, this must be a valid sensor
    break;
#endif //USE_MI_DECRYPTION
  }

  DEBUG_SENSOR_LOG(PSTR("%s at slot %u"), kNRFSlaveType[_sensorVec->type-1],_slot);
  switch(MINRF.buffer.miBeacon.type){
    case 0x1:
    if(MINRF.buffer.miBeacon.counter==_sensorVec->lastCnt) break;
    // AddLog_P2(LOG_LEVEL_INFO,PSTR("MINRF: YEE-RC button: %u Long: %u"), MINRF.buffer.miBeacon.Btn.num, MINRF.buffer.miBeacon.Btn.longPress);
    _sensorVec->lastCnt=MINRF.buffer.miBeacon.counter;
    _sensorVec->btn=MINRF.buffer.miBeacon.Btn.num + (MINRF.buffer.miBeacon.Btn.longPress/2)*6;
    _sensorVec->shallSendMQTT = 1;
    MINRFtriggerTele();
    break;
    case 0x04:
    _tempFloat=(float)(MINRF.buffer.miBeacon.temp)/10.0f;
    if(_tempFloat<60){
        _sensorVec->temp=_tempFloat;
        DEBUG_SENSOR_LOG(PSTR("Mode 4: temp updated"));
    }
    DEBUG_SENSOR_LOG(PSTR("Mode 4: U16:  %u Temp"), MINRF.buffer.miBeacon.temp );
    break;
    case 0x06:
    _tempFloat=(float)(MINRF.buffer.miBeacon.hum)/10.0f;
    if(_tempFloat<101){
        _sensorVec->hum=_tempFloat;
        DEBUG_SENSOR_LOG(PSTR("Mode 6: hum updated"));
    }
    DEBUG_SENSOR_LOG(PSTR("Mode 6: U16:  %u Hum"), MINRF.buffer.miBeacon.hum);
    break;
    case 0x07:
    _sensorVec->lux=MINRF.buffer.miBeacon.lux & 0x00ffffff;
    DEBUG_SENSOR_LOG(PSTR("Mode 7: U24: %u Lux"), MINRF.buffer.miBeacon.lux & 0x00ffffff);
    break;
    case 0x08:
    _tempFloat =(float)MINRF.buffer.miBeacon.moist;
    if(_tempFloat<100){
        _sensorVec->moisture=_tempFloat;
        DEBUG_SENSOR_LOG(PSTR("Mode 8: moisture updated"));
    }
    DEBUG_SENSOR_LOG(PSTR("Mode 8: U8: %u Moisture"), MINRF.buffer.miBeacon.moist);
    break;
    case 0x09:
    _tempFloat=(float)(MINRF.buffer.miBeacon.fert);
    if(_tempFloat<65535){ // ???
        _sensorVec->fertility=_tempFloat;
        DEBUG_SENSOR_LOG(PSTR("Mode 9: fertility updated"));
    }
    DEBUG_SENSOR_LOG(PSTR("Mode 9: U16: %u Fertility"), MINRF.buffer.miBeacon.fert);
    break;
    case 0x0a:
    if(MINRF.buffer.miBeacon.bat<101){
      _sensorVec->bat = MINRF.buffer.miBeacon.bat;
      DEBUG_SENSOR_LOG(PSTR("Mode a: bat updated"));
      }
    DEBUG_SENSOR_LOG(PSTR("Mode a: U8: %u %%"), MINRF.buffer.miBeacon.bat);
    break;
    case 0x0d:
    _tempFloat=(float)(MINRF.buffer.miBeacon.HT.temp)/10.0f;
    if(_tempFloat<60){
        _sensorVec->temp = _tempFloat;
        DEBUG_SENSOR_LOG(PSTR("Mode d: temp updated"));
     }
    _tempFloat=(float)(MINRF.buffer.miBeacon.HT.hum)/10.0f;
    if(_tempFloat<100){
        _sensorVec->hum = _tempFloat;
        DEBUG_SENSOR_LOG(PSTR("Mode d: hum updated"));
    }
    DEBUG_SENSOR_LOG(PSTR("Mode d: U16:  %x Temp U16: %x Hum"), MINRF.buffer.miBeacon.HT.temp,  MINRF.buffer.miBeacon.HT.hum);
    break;
  }
}

/**
 * @brief parse the Cleargrass-packet
 *        Note: battery section is based on "internet data" -> not confirmed yet
 */
void MINRFhandleCGD1Packet(void){ // no MiBeacon
  MINRFreverseMAC(MINRF.buffer.CGDPacket.MAC);
  uint32_t _slot = MINRFgetSensorSlot(MINRF.buffer.CGDPacket.MAC, 0x0576); // This must be hard-coded, no object-id in Cleargrass-packet
  DEBUG_SENSOR_LOG(PSTR("MINRF: Sensor slot: %u"), _slot);
  if(_slot==0xff) return;

  switch (MINRF.buffer.CGDPacket.mode){
    case 0x0401:
      float _tempFloat;
      _tempFloat=(float)(MINRF.buffer.CGDPacket.temp)/10.0f;
      if(_tempFloat<60){
          MIBLEsensors.at(_slot).temp = _tempFloat;
          DEBUG_SENSOR_LOG(PSTR("CGD1: temp updated"));
      }
      _tempFloat=(float)(MINRF.buffer.CGDPacket.hum)/10.0f;
      if(_tempFloat<100){
          MIBLEsensors.at(_slot).hum = _tempFloat;
          DEBUG_SENSOR_LOG(PSTR("CGD1: hum updated"));
      }
      DEBUG_SENSOR_LOG(PSTR("CGD1: U16:  %x Temp U16: %x Hum"), MINRF.buffer.CGDPacket.temp,  MINRF.buffer.CGDPacket.hum);
      break;
    case 0x0102:
      if(MINRF.buffer.CGDPacket.bat<101){
      MIBLEsensors.at(_slot).bat = MINRF.buffer.CGDPacket.bat;
      DEBUG_SENSOR_LOG(PSTR("Mode a: bat updated"));
      }
      break;
    default:
      DEBUG_SENSOR_LOG(PSTR("MINRF: unexpected CGD1-packet"));
      MINRF_LOG_BUFFER(MINRF.buffer.raw);
  }
}

void MINRFhandleNlightPacket(void){ // no MiBeacon
  uint32_t offset = 6;
  uint8_t _buf[32+offset];
  MINRFrecalcBuffer((uint8_t*)&_buf,offset);
  // AddLog_P2(LOG_LEVEL_INFO,PSTR("MINRF: NLIGHT: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x"),_buf[0],_buf[1],_buf[2],_buf[3],_buf[4],_buf[5],_buf[6],_buf[7],_buf[8],_buf[9],_buf[10],_buf[11],_buf[12],_buf[13],_buf[14],_buf[15],_buf[16],_buf[17],_buf[18]);
  uint32_t _frame_PID = _buf[15]<<24 | _buf[16]<<16 | _buf[17]<<8 | _buf[18];
  if(_frame_PID!=0x4030dd03) return; // invalid packet
  // AddLog_P2(LOG_LEVEL_INFO,PSTR("MINRF: NLIGHT:%x"),_frame_PID);
  uint32_t _idx = MINRF.activeLight-1;
  if((millis() - MIBLElights[_idx].lastTime)<1500) return;
  if(_buf[19]!=MIBLElights[_idx].lastCnt){
    MIBLElights[_idx].lastCnt = _buf[19];
    MIBLElights[_idx].events++;
    MIBLElights[_idx].shallSendMQTT = 1;
    MIBLElights[_idx].lastTime = millis();
    AddLog_P2(LOG_LEVEL_DEBUG,PSTR("MINRF: NLIGHT %u: events: %u, Cnt:%u"), _idx,MIBLElights[_idx].events, MIBLElights[_idx].lastCnt);
  }
}

void MINRFhandleMJYD2SPacket(void){ // no MiBeacon
  uint32_t offset = 8;
  uint8_t _buf[32+offset];
  MINRFrecalcBuffer((uint8_t*)&_buf,offset);
  mjysd02_Packet_t *_packet = (mjysd02_Packet_t*)&_buf;
  if(_packet->PID!=0x07f6) return; // invalid packet
  // AddLog_P2(LOG_LEVEL_INFO,PSTR("MINRF: MJYD2S: %02u %04x %04x %04x %02x"),_packet->payloadSize,_packet->UUID,_packet->frameCtrl,_packet->PID,_packet->frameCnt);
  // AddLog_P2(LOG_LEVEL_INFO,PSTR("MINRF: PAYLOAD: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x"),_packet->data[0],_packet->data[1],_packet->data[2],_packet->data[3],_packet->data[4],_packet->data[5],_packet->data[6],_packet->data[7],_packet->data[8],_packet->data[9],_packet->data[10],_packet->data[11],_packet->data[12],_packet->data[13],_packet->data[14],_packet->data[15],_packet->data[16],_packet->data[17]);
  uint32_t _idx = MINRF.activeLight-1;
  switch(_packet->frameCtrl){
    case 0x5910:
      if(_packet->frameCnt!=MIBLElights[_idx].lastCnt){
        // AddLog_P2(LOG_LEVEL_INFO,PSTR("MINRF: MJYD2S after motion:%x"),_packet->frameCnt);
        MIBLElights[_idx].lastCnt = _packet->frameCnt;
        if(millis()-MIBLElights[_idx].lastTime>120000){
          MIBLElights[_idx].eventType = 1;
          MIBLElights[_idx].events++;
          MIBLElights[_idx].shallSendMQTT = 1;
          MIBLElights[_idx].lastTime = millis();
          AddLog_P2(LOG_LEVEL_DEBUG,PSTR("MINRF: MJYD2S secondary PIR"));
        }
      }
      break;
    case 0x5948: case 0x5958:
      uint8_t output[16];
      if(_packet->frameCnt==MIBLElights[_idx].lastCnt) break;
      int32_t ret = MINRFdecryptMJYD2SPacket((char*)&_buf, MINRF.activeLight,(char*)&output);
      if(ret==0){
        MIBLElights[_idx].lastCnt = _packet->frameCnt;
        switch(output[0]){
          case 0x0f:
            if(output[1] == 0){
              if(millis()-MIBLElights[_idx].lastTime>1000){
                MIBLElights[_idx].eventType = 1; //PIR
                MIBLElights[_idx].shallSendMQTT = 1;
                AddLog_P2(LOG_LEVEL_DEBUG,PSTR("MINRF: MJYD2S primary PIR"));
                MIBLElights[_idx].events++;
              }
              MIBLElights[_idx].lastTime = millis();
              MIBLElights[_idx].lux = output[3];
            }
            break;
          case 0x07:
            if(output[1] == 0x10){
              MIBLElights[_idx].eventType = 2; //No PIR
              MIBLElights[_idx].lux = output[3];
              MIBLElights[_idx].shallSendMQTT = 1;
            }
            break;
          case 0x0a:
            MIBLElights[_idx].bat = output[3];
          break;
          case 0x17:
            MIBLElights[_idx].NMT = output[6]<<24 | output[5]<<16 | output[4]<<8 | output[3];
            MIBLElights[_idx].eventType = 3; // NMT  0, 120, 300, 600, 1800, ... seconds
            MIBLElights[_idx].shallSendMQTT = 1;
            // AddLog_P2(LOG_LEVEL_DEBUG,PSTR("MINRF: MJYD2S NMT: %u"), MIBLElights[_idx].NMT );
            break;
        }
      }
  }
  // AddLog_P2(LOG_LEVEL_INFO,PSTR("MINRF: NLIGHT:%x"),_frame_PID);
}


void MINRFhandleLightPacket(void){
  switch(MIBLElights[MINRF.activeLight-1].type){
    case NLIGHT:
        // AddLog_P2(LOG_LEVEL_INFO,PSTR("MINRF: NLIGHT!!"));
        MINRFhandleNlightPacket();
        break;
    case MJYD2S:
        // AddLog_P2(LOG_LEVEL_INFO,PSTR("MINRF: MJYD2S !!"));
        MINRFhandleMJYD2SPacket();
        break;
  }
  if(MIBLElights[MINRF.activeLight-1].shallSendMQTT==1) MINRFtriggerTele();
}

void MINRFaddLight(uint8_t _MAC[], uint8_t _type){ // no MiBeacon
  for(uint32_t i=0; i<MIBLElights.size(); i++){
    if(memcmp(_MAC,MIBLElights[i].MAC,sizeof(MIBLElights[i].MAC))==0){
      // AddLog_P2(LOG_LEVEL_INFO,PSTR("MINRF: NLIGHT: Known MAC!!"));
      return;
    }
  }
  mi_light_t _light;
  memcpy(_light.MAC,_MAC,sizeof(_light.MAC));
  uint32_t offset = (_type == 7)? 0 : 2; // for the MJYD2S we transform the end of the MAC to a PDU
  MINRFcomputeBeaconPDU(_light.MAC,_light.PDU, offset);
  _light.type=_type;
  _light.events=0;
  _light.lastCnt=0;
  _light.bat=0;
  _light.lux=0;
  MIBLElights.push_back(_light);
  AddLog_P2(LOG_LEVEL_INFO,PSTR("MINRF: new %s at slot: %u"),kMINRFDeviceType[_type-1], MIBLElights.size()-1);
}

/*********************************************************************************************\
 * Main loop of the driver
\*********************************************************************************************/

void MINRF_EVERY_50_MSECOND() { // Every 50mseconds

  if(MINRF.timer>6000){ // happens every 6000/20 = 300 seconds
    DEBUG_SENSOR_LOG(PSTR("MINRF: check for FAKE sensors"));
    MINRFpurgeFakeSensors();
    MINRF.timer=0;
  }
  MINRF.timer++;

  if (!MINRFreceivePacket()){
    // DEBUG_SENSOR_LOG(PSTR("MINRF: nothing received"));
  }

  else {
    switch (MINRF.packetMode) {
      case 0:
        if (MINRF.beacon.active){
          MINRFhandleBeacon(&MINRFdummyEntry,6);
        }
        else MINRFhandleScan();
        break;
      case FLORA: case MJ_HT_V1: case LYWSD02: case CGG1: case LYWSD03: case YEERC:
        MINRFhandleMiBeaconPacket();
        break;
      case CGD1:
        MINRFhandleCGD1Packet();
        break;
      case NLIGHT: //case MJYD2S:
        MINRFhandleLightPacket();
        break;
      default:
        break;
    }
  }
  if (MINRF.beacon.active || MINRF.activeScan) {
    MINRF.firstUsedPacketMode=0;
  }

  if(MINRF.packetMode==NLIGHT){
    if(MINRF.activeLight+1>MIBLElights.size()){
      MINRF.activeLight=0;
      MINRF.packetMode+=2;
    }
    else MINRF.packetMode+=2;
  }
  else{  
    MINRF.packetMode = (MINRF.packetMode+1>MI_TYPES) ? MINRF.firstUsedPacketMode : MINRF.packetMode+1;
    for (uint32_t i = MINRF.packetMode; i<MI_TYPES+1; i++){
      if (bitRead(MINRF.ignore,i)) {
        MINRF.packetMode++;
      }
      else break;
    }
  }

  if (MINRF.activeScan) MINRF.packetMode=0;

  MINRFinitBLE(MINRF.packetMode);

  MINRFhopChannel();

  if (MINRF.beacon.active) {
      if (MINRF.packetMode==0) NRF24radio.openReadingPipe(0,MINRF.beacon.PDU[MINRF.currentChan]);
  }

  NRF24radio.startListening();
}
/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

bool NRFCmd(void) {
  char command[CMDSZ];
  bool serviced = true;
  uint8_t disp_len = strlen(D_CMND_NRF);

  if (!strncasecmp_P(XdrvMailbox.topic, PSTR(D_CMND_NRF), disp_len)) {  // prefix
    uint32_t command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic + disp_len, kNRF_Commands);
    switch (command_code) {
      case CMND_NRF_PAGE:
        if (XdrvMailbox.data_len > 0) {
            if (XdrvMailbox.payload == 0) XdrvMailbox.payload = MINRF.perPage; // ignore 0
            MINRF.perPage = XdrvMailbox.payload;
          }
        else XdrvMailbox.payload = MINRF.perPage;
        Response_P(S_JSON_NRF_COMMAND_NVALUE, command, XdrvMailbox.payload);
        break;
      case CMND_NRF_IGNORE:
        if (XdrvMailbox.data_len > 0) {
            if (XdrvMailbox.payload == 0){
              MINRF.ignore = 0;
              MINRF.firstUsedPacketMode = 1;
            }
            else if (XdrvMailbox.payload < MI_TYPES+1) {
              bitSet(MINRF.ignore,XdrvMailbox.payload);
              MINRFcomputefirstUsedPacketMode();
              MINRF.timer = 5900;
              Response_P(S_JSON_NRF_COMMAND, command, kMINRFDeviceType[XdrvMailbox.payload-1]);
            }
            else if (XdrvMailbox.payload == 65535) {
              MINRF.ignore = 65535;
            }
          }
        Response_P(S_JSON_NRF_COMMAND_NVALUE, command, MINRF.ignore);
        break;
      case CMND_NRF_USE:
        if (XdrvMailbox.data_len > 0) {
            if (XdrvMailbox.payload == 0){
              MINRF.ignore = 65535;
              MINRF.firstUsedPacketMode = 1;
            }
            else if (XdrvMailbox.payload < MI_TYPES+1) {
              bitClear(MINRF.ignore,XdrvMailbox.payload);
              MINRFcomputefirstUsedPacketMode();
              MINRF.timer = 5900;
              Response_P(S_JSON_NRF_COMMAND, command, kMINRFDeviceType[XdrvMailbox.payload-1]);
            }
            else if (XdrvMailbox.payload == 65535) {
              MINRF.ignore = 0;
            }
          }
        Response_P(S_JSON_NRF_COMMAND_NVALUE, command, MINRF.ignore);
        break;
      case CMND_NRF_SCAN:
      if (XdrvMailbox.data_len > 0) {
        MINRF.beacon.active = false;
        switch(XdrvMailbox.payload){
          case 0: // new scan
            MINRF.activeScan = true;
            MINRF.stopScan = false;
            MINRFscanResult.erase(std::remove_if(MINRFscanResult.begin(),
                                  MINRFscanResult.end(),
                                  [](scan_entry_t&) { return true; }),
                                  MINRFscanResult.end());
            break;
          case 1: // append scan
            MINRF.activeScan = true;
            MINRF.stopScan = false;
            break;
          case 2: // stop scan
            MINRF.stopScan = true;
            break;
        }
        Response_P(S_JSON_NRF_COMMAND_NVALUE, command, XdrvMailbox.payload);
      }
        break;
      case CMND_NRF_BEACON:
          if (XdrvMailbox.data_len > 0) {
            if(XdrvMailbox.data_len<3){ // a list entry
              if (XdrvMailbox.payload < MINRFscanResult.size()) {
                MINRFstartBeacon(XdrvMailbox.payload);
                Response_P(S_JSON_NRF_COMMAND_NVALUE, command, XdrvMailbox.payload);
              }
            }
            if (XdrvMailbox.data_len==12){  // a MAC-string
              memset(MINRF.beacon.MAC,0,sizeof(MINRF.beacon.MAC));
              MINRFMACStringToBytes(XdrvMailbox.data, MINRF.beacon.MAC);
              MINRF.beacon.time=0;
              MINRF.beacon.active=true;
              Response_P(S_JSON_NRF_COMMAND, command, XdrvMailbox.data);
            }
            MINRFcomputeBeaconPDU(MINRF.beacon.MAC,MINRF.beacon.PDU,0);
          }
        break;
      case CMND_NRF_NLIGHT:
          if (XdrvMailbox.data_len > 0) {
            if (XdrvMailbox.data_len==12){  // a MAC-string
              uint8_t _MAC[6] = {0};
              MINRFMACStringToBytes(XdrvMailbox.data, _MAC);
              Response_P(S_JSON_NRF_COMMAND, command, XdrvMailbox.data);         
              MINRFaddLight(_MAC, 7);
            }
          }
        break;
        case CMND_NRF_CHAN:
          if (XdrvMailbox.data_len == 1) {
            switch(XdrvMailbox.payload){
              case 0: case 1: case 2:
                bitRead(MINRF.channelIgnore,XdrvMailbox.payload) == 0 ? bitSet(MINRF.channelIgnore,XdrvMailbox.payload) : bitClear(MINRF.channelIgnore,XdrvMailbox.payload);
                break;
              }
          }
          Response_P(S_JSON_NRF_COMMAND_NVALUE, command, MINRF.channelIgnore);
        break;
#ifdef USE_MI_DECRYPTION
      case CMND_NRF_MJYD2S:
        if (XdrvMailbox.data_len==44){  // a KEY-MAC-string
          MINRFAddKey(XdrvMailbox.data);
          uint8_t _MAC[6] = {0};
          MINRFMACStringToBytes((XdrvMailbox.data)+32, _MAC);
          MINRFaddLight(_MAC, 8);
          Response_P(S_JSON_NRF_COMMAND, command, XdrvMailbox.data);
        }
        break;

      case CMND_NRF_KEY:
        if (XdrvMailbox.data_len==44){  // a KEY-MAC-string
          MINRFAddKey(XdrvMailbox.data);
          Response_P(S_JSON_NRF_COMMAND, command, XdrvMailbox.data);
        }
        break;
#endif //USE_MI_DECRYPTION
      default:
        // else for Unknown command
        serviced = false;
      break;
    }
  } else {
    return false;
  }
  return serviced;
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

const char HTTP_BATTERY[] PROGMEM = "{s}%s" " Battery" "{m}%u%%{e}";
const char HTTP_MINRF_MAC[] PROGMEM = "{s}%s %s{m}%02x:%02x:%02x:%02x:%02x:%02x%{e}";
const char HTTP_MINRF_FLORA_DATA[] PROGMEM =  "{s}%s" " Fertility" "{m}%dus/cm{e}";
const char HTTP_MINRF_HL[] PROGMEM = "{s}<hr>{m}<hr>{e}";
const char HTTP_NRF24NEW[] PROGMEM = "{s}%sL01%c{m}%u%s / %u{e}";

void MINRFShow(bool json)
{
  if (json) {
    for (uint32_t i = 0; i < MIBLEsensors.size(); i++) {
      if(MIBLEsensors[i].showedUp < 3){
        DEBUG_SENSOR_LOG(PSTR("MINRF: sensor not fully registered yet"));
        if(MIBLEsensors[i].type != YEERC) break; // send every RC code, even if there is a potentially false MAC 
        }
      switch(MIBLEsensors[i].type){
        case YEERC:
          if(MIBLEsensors[i].shallSendMQTT==0) continue;
          break;
        default:
          if(MINRF.triggeredTELE) continue;
          break;
        }
      ResponseAppend_P(PSTR(",\"%s-%02x%02x%02x\":{"),kMINRFDeviceType[MIBLEsensors[i].type-1],MIBLEsensors[i].MAC[3],MIBLEsensors[i].MAC[4],MIBLEsensors[i].MAC[5]);
      switch(MIBLEsensors[i].type){
        case FLORA:
          if(MINRF.triggeredTELE) {
            ResponseJsonEnd();
            break;
          }
          char stemp[FLOATSZ];
          dtostrfd(MIBLEsensors[i].temp, Settings.flag2.temperature_resolution, stemp);
          ResponseAppend_P(PSTR("\"" D_JSON_TEMPERATURE "\":%s"), stemp);

          if(MIBLEsensors[i].lux!=0xffffffff){ // this is the error code -> no lux
            ResponseAppend_P(PSTR(",\"" D_JSON_ILLUMINANCE "\":%u"), MIBLEsensors[i].lux);
          }
          if(!isnan(MIBLEsensors[i].moisture)){
            dtostrfd(MIBLEsensors[i].moisture, 0, stemp);
            ResponseAppend_P(PSTR(",\"" D_JSON_MOISTURE "\":%s"), stemp);
          }
          if(!isnan(MIBLEsensors[i].fertility)){
            dtostrfd(MIBLEsensors[i].fertility, 0, stemp);
            ResponseAppend_P(PSTR(",\"Fertility\":%s"), stemp);
          }
          ResponseJsonEnd();
          break;
        case YEERC:
          if(MIBLEsensors[i].shallSendMQTT == 1){
            ResponseAppend_P(PSTR("\"Btn\":%u"), MIBLEsensors[i].btn);
            MIBLEsensors[i].shallSendMQTT = 0;
          }
          ResponseJsonEnd();
          break;
        default:  
          if(MINRF.triggeredTELE) {
            ResponseJsonEnd();
            break;
          }
          if(!isnan(MIBLEsensors[i].temp) && !isnan(MIBLEsensors[i].hum)){
              ResponseAppendTHD(MIBLEsensors[i].temp,MIBLEsensors[i].hum);
          }
          if(MIBLEsensors[i].bat!=0x00){ // this is the error code -> no battery
            ResponseAppend_P(PSTR(",\"Battery\":%u"), MIBLEsensors[i].bat);
          }
          ResponseJsonEnd();
          break;
      }
    }
    for(uint32_t i=0; i<MIBLElights.size(); i++){
      if(MINRF.triggeredTELE && MIBLElights[i].shallSendMQTT==0) continue;
      ResponseAppend_P(PSTR(",\"%s-%02x%02x%02x\":{"),kMINRFDeviceType[MIBLElights[i].type-1],MIBLElights[i].MAC[3],MIBLElights[i].MAC[4],MIBLElights[i].MAC[5]);
      if(MIBLElights[i].shallSendMQTT == 1){
        switch(MIBLElights[i].type){
          case NLIGHT:
            ResponseAppend_P(PSTR("\"PIR\":1,\"Events\":%u"),MIBLElights[i].events);
            break;
          case MJYD2S:
            switch(MIBLElights[i].eventType){
              case 1: //PIR
                ResponseAppend_P(PSTR("\"PIR\":1,\"" D_JSON_ILLUMINANCE "\":%u,\"Events\":%u"), MIBLElights[i].lux,MIBLElights[i].events);
                break;
              case 2: //No PIR
                ResponseAppend_P(PSTR("\"PIR\":0,\"" D_JSON_ILLUMINANCE "\":%u"), MIBLElights[i].lux);
                break;
              case 3: // contiguous time without motion
                ResponseAppend_P(PSTR("\"NMT\":%u"), MIBLElights[i].NMT);
                break;
            }
        }
        MIBLElights[i].eventType=0;
        MIBLElights[i].shallSendMQTT = 0;
      }
      else{
        if(MIBLElights[i].type==MJYD2S){
          if(MIBLElights[i].bat!=0) ResponseAppend_P(PSTR("\"Battery\":%u,\"" D_JSON_ILLUMINANCE "\":%u,"), MIBLElights[i].bat, MIBLElights[i].lux);
        }
        ResponseAppend_P(PSTR("\"Events\":%u"),MIBLElights[i].events);
      }
      ResponseJsonEnd();
    }
    if(MINRF.beacon.active){
    ResponseAppend_P(PSTR(",\"Beacon\":{\"Timer\":%u}"),MINRF.beacon.time);
    }
    if(MINRF.triggeredTELE) MINRF.triggeredTELE = false;
    // ResponseJsonEnd();
#ifdef USE_WEBSERVER
    } else {
      static  uint32_t _page = 0;
      static uint32_t counter = 0;
      int32_t i = _page * MINRF.perPage;
      uint32_t j = i + MINRF.perPage;

      if (j+1>MINRF.confirmedSensors){
        j = MINRF.confirmedSensors;
      }
      char stemp[5] ={0};
      if (MINRF.confirmedSensors-(_page*MINRF.perPage)>1 && MINRF.perPage!=1) {
        sprintf_P(stemp,"-%u",j);
      }
      if (MINRF.confirmedSensors==0) i=-1; // only for the GUI

      WSContentSend_PD(HTTP_NRF24NEW, NRF24type, NRF24.chipType, i+1,stemp,MINRF.confirmedSensors);
      for (i ; i<j; i++) {
        if(MIBLEsensors[i].showedUp < 3){
          DEBUG_SENSOR_LOG(PSTR("MINRF: sensor not fully registered yet"));
          j++;
          continue;
          }
        WSContentSend_PD(HTTP_MINRF_HL);
        WSContentSend_PD(HTTP_MINRF_MAC, kMINRFDeviceType[MIBLEsensors[i].type-1], D_MAC_ADDRESS, MIBLEsensors[i].MAC[0], MIBLEsensors[i].MAC[1],MIBLEsensors[i].MAC[2],MIBLEsensors[i].MAC[3],MIBLEsensors[i].MAC[4],MIBLEsensors[i].MAC[5]);
        if (MIBLEsensors[i].type==YEERC) continue;
        if (MIBLEsensors[i].type==FLORA){
          if(!isnan(MIBLEsensors[i].temp)){
            char temperature[FLOATSZ];
            dtostrfd(MIBLEsensors[i].temp, Settings.flag2.temperature_resolution, temperature);
            WSContentSend_PD(HTTP_SNS_TEMP, kMINRFDeviceType[MIBLEsensors[i].type-1], temperature, TempUnit());
          }
          if(MIBLEsensors[i].lux!=0xffffffff){ // this is the error code -> no valid value
            WSContentSend_PD(HTTP_SNS_ILLUMINANCE, kMINRFDeviceType[MIBLEsensors[i].type-1], MIBLEsensors[i].lux);
          }
          if(!isnan(MIBLEsensors[i].moisture)){ // this is the error code -> no valid value
            WSContentSend_PD(HTTP_SNS_MOISTURE, kMINRFDeviceType[MIBLEsensors[i].type-1], MIBLEsensors[i].moisture);
          }
          if(!isnan(MIBLEsensors[i].fertility)){ // this is the error code -> no valid value
            WSContentSend_PD(HTTP_MINRF_FLORA_DATA, kMINRFDeviceType[MIBLEsensors[i].type-1], MIBLEsensors[i].fertility);
          }
        }
        if (MIBLEsensors[i].type>FLORA){ // everything "above" Flora
          WSContentSend_THD(kMINRFDeviceType[MIBLEsensors[i].type-1], MIBLEsensors[i].temp, MIBLEsensors[i].hum);
          if(MIBLEsensors[i].bat!=0x00){ // without "juice" nothing can be done
            WSContentSend_PD(HTTP_BATTERY, kMINRFDeviceType[MIBLEsensors[i].type-1], MIBLEsensors[i].bat);
          }
        }
      }
      if(MINRF.beacon.active){
        WSContentSend_PD(HTTP_MINRF_HL);
        WSContentSend_PD(HTTP_MINRF_HL);
        WSContentSend_PD(HTTP_MINRF_MAC, F("Beacon"), D_MAC_ADDRESS, MINRF.beacon.MAC[0], MINRF.beacon.MAC[1],MINRF.beacon.MAC[2],MINRF.beacon.MAC[3],MINRF.beacon.MAC[4],MINRF.beacon.MAC[5]);
        WSContentSend_PD(PSTR("{s}Beacon Time{m}%u seconds{e}"),MINRF.beacon.time);
      }

      for(uint32_t i=0; i<MIBLElights.size(); i++){
        WSContentSend_PD(HTTP_MINRF_HL);
        if(i==0) WSContentSend_PD(HTTP_MINRF_HL);
        WSContentSend_PD(HTTP_MINRF_MAC, kMINRFDeviceType[MIBLElights[i].type-1], D_MAC_ADDRESS, MIBLElights[i].MAC[0], MIBLElights[i].MAC[1],MIBLElights[i].MAC[2],MIBLElights[i].MAC[3],MIBLElights[i].MAC[4],MIBLElights[i].MAC[5]);
        WSContentSend_PD(PSTR("{s}%s Events {m}%u (PC: %u){e}"),kMINRFDeviceType[MIBLElights[i].type-1],MIBLElights[i].events, MIBLElights[i].lastCnt);
        if(MIBLElights[i].type==MJYD2S){
          if(MIBLElights[i].bat>0){
            WSContentSend_PD(HTTP_BATTERY, kMINRFDeviceType[MIBLElights[i].type-1], MIBLElights[i].bat);
          }
          if(MIBLElights[i].lux>0){
            WSContentSend_PD(HTTP_SNS_ILLUMINANCE, kMINRFDeviceType[MIBLElights[i].type-1], MIBLElights[i].lux);
          }
        }
      }

      if(counter>3) {
        _page++;
        counter = 0;
      }
      counter++;
      if(MINRF.confirmedSensors%MINRF.perPage==0 && _page==MINRF.confirmedSensors/MINRF.perPage) _page=0;
      if(_page>MINRF.confirmedSensors/MINRF.perPage) _page=0;
#endif  // USE_WEBSERVER
    }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns61(uint8_t function)
{
  bool result = false;
  if (NRF24.chipType) {
    switch (function) {
      case FUNC_INIT:
        MINRFinitBLE(1);
        AddLog_P2(LOG_LEVEL_INFO,PSTR("MINRF: started"));
        break;
      case FUNC_EVERY_50_MSECOND:
        MINRF_EVERY_50_MSECOND();
        break;
      case FUNC_EVERY_SECOND:
        MINRFbeaconCounter();
        break;
      case FUNC_COMMAND:
        result = NRFCmd();
        break;
      case FUNC_JSON_APPEND:
        MINRFShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        MINRFShow(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_MIBLE
#endif  // USE_NRF24
#endif  // USE_SPI
