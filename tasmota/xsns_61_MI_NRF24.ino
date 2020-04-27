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


/*********************************************************************************************\
* MINRF
* BLE-Sniffer/Bridge for MIJIA/XIAOMI Temperatur/Humidity-Sensor, Mi Flora, LYWSD02, GCx
*
* Usage: Configure NRF24
\*********************************************************************************************/

#define XSNS_61             61

#include <vector>

#define FLORA       1
#define MJ_HT_V1    2
#define LYWSD02     3
#define LYWSD03     4
#define CGG1        5
#define CGD1        6

#define D_CMND_NRF "NRF"

const char S_JSON_NRF_COMMAND_NVALUE[] PROGMEM = "{\"" D_CMND_NRF "%s\":%d}";
const char S_JSON_NRF_COMMAND[] PROGMEM        = "{\"" D_CMND_NRF "%s\":\"%s\"}";
const char kNRF_Commands[] PROGMEM             = "Ignore|Page|Scan|Beacon|Chan";

enum NRF_Commands {          // commands useable in console or rules
  CMND_NRF_IGNORE,           // ignore specific sensor type (1-6)
  CMND_NRF_PAGE,             // sensor entries per web page, which will be shown alternated
  CMND_NRF_SCAN,             // simplified passive BLE adv scan
  CMND_NRF_BEACON,           // even more simplified Beacon, reports time since last sighting
  CMND_NRF_CHAN              // ignore channel 0-2 (translates to 37-39)
  };

const uint16_t kMINRFSlaveID[6]={ 0x0098, // Flora
                                  0x01aa, // MJ_HT_V1
                                  0x045b, // LYWSD02
                                  0x055b, // LYWSD03
                                  0x0347, // CGG1
                                  0x0576  // CGD1
                                  };

const char kMINRFSlaveType1[] PROGMEM = "Flora";
const char kMINRFSlaveType2[] PROGMEM = "MJ_HT_V1";
const char kMINRFSlaveType3[] PROGMEM = "LYWSD02";
const char kMINRFSlaveType4[] PROGMEM = "LYWSD03";
const char kMINRFSlaveType5[] PROGMEM = "CGG1";
const char kMINRFSlaveType6[] PROGMEM = "CGD1";
const char * kMINRFSlaveType[] PROGMEM = {kMINRFSlaveType1,kMINRFSlaveType2,kMINRFSlaveType3,kMINRFSlaveType4,kMINRFSlaveType5,kMINRFSlaveType6};

// PDU's or different channels 37-39
const uint32_t kMINRFFloPDU[3] = {0x3eaa857d,0xef3b8730,0x71da7b46};
const uint32_t kMINRFMJPDU[3]  = {0x4760cd66,0xdbcc0cd3,0x33048df5};
const uint32_t kMINRFL2PDU[3]  = {0x3eaa057d,0xef3b0730,0x71dafb46};
// const uint32_t kMINRFL3PDU[3]  = {0x4760dd78,0xdbcc1ccd,0xffffffff}; //encrypted - 58 58
const uint32_t kMINRFL3PDU[3]  = {0x4760cb78,0xdbcc0acd,0x33048beb}; //unencrypted  - 30 58
const uint32_t kMINRFCGGPDU[3]  = {0x4760cd6e,0xdbcc0cdb,0x33048dfd};
const uint32_t kMINRFCGDPDU[3]  = {0x5da0d752,0xc10c16e7,0x29c497c1};

// start-LSFR for different channels 37-39
const uint8_t kMINRFlsfrList_A[3] = {0x4b,0x17,0x23};  // Flora, LYWSD02
const uint8_t kMINRFlsfrList_B[3] = {0x21,0x72,0x43};  // MJ_HT_V1, LYWSD03, CGx


#pragma pack(1)  // important!!
struct mi_beacon_t{
  uint16_t productID;
  uint8_t counter;
  uint8_t Mac[6];
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
  };
};

struct CGDPacket_t {    // related to the whole 32-byte-packet/buffer
  uint8_t serial[6];
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
  uint8_t mac[6];
};

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
  uint8_t currentChan=0;
  uint8_t ignore = 0; //bitfield: 2^sensor type
  uint8_t channelIgnore = 0; //bitfield: 2^channel (0=37,1=38,2=39)
  uint8_t confirmedSensors = 0;
  uint8_t packetMode; // 0 - normal BLE-advertisements, 1 - 6 "special" sensor packets
  uint8_t perPage = 4;
  uint8_t firstUsedPacketMode = 1;

  FIFO_t buffer;

  struct {
    uint8_t mac[6];
    uint32_t time;
    uint32_t PDU[3];
    bool active = false;
  } beacon;
  bool activeScan = false;
  bool stopScan = false;

#ifdef DEBUG_TASMOTA_SENSOR
  uint8_t streamBuffer[sizeof(buffer)]; //  raw data stream bytes
  uint8_t lsfrBuffer[sizeof(buffer)];   //  correpsonding lfsr-bytes for the buffer, probably only useful for a BLE-packet
#endif //  DEBUG_TASMOTA_SENSOR

} MINRF;

struct mi_sensor_t{
  uint8_t type; //Flora = 1; MJ_HT_V1=2; LYWSD02=3; LYWSD03=4; CGG1=5; CGD1=6
  uint8_t serial[6];
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
  };
};

struct scan_entry_t {
  uint8_t mac[6];
  uint16_t cid;
  uint16_t svc;
  uint16_t uuid;
  uint8_t showedUp;
};

std::vector<mi_sensor_t> MIBLEsensors;
std::vector<scan_entry_t> MINRFscanResult;

static union{
  scan_entry_t MINRFdummyEntry;
  uint8_t MINRFtempBuf[23];
};

/********************************************************************************************/

/**
 * @brief
 *
 * @param _mode Packet mode 0-6
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
      case 0:
      MINRFwhiten((uint8_t *)&MINRF.buffer, sizeof(MINRF.buffer),  MINRF.channel[MINRF.currentChan] | 0x40);
      break;
      case 1:
      MINRFwhiten((uint8_t *)&MINRF.buffer, sizeof(MINRF.buffer),  kMINRFlsfrList_A[MINRF.currentChan]); // "flora" mode
      break;
      case 2:
      MINRFwhiten((uint8_t *)&MINRF.buffer, sizeof(MINRF.buffer),  kMINRFlsfrList_B[MINRF.currentChan]); // "MJ_HT_V1" mode
      break;
      case 3:
      MINRFwhiten((uint8_t *)&MINRF.buffer, sizeof(MINRF.buffer),  kMINRFlsfrList_A[MINRF.currentChan]); // "LYWSD02" mode
      break;
      case 4:
      MINRFwhiten((uint8_t *)&MINRF.buffer, sizeof(MINRF.buffer),  kMINRFlsfrList_B[MINRF.currentChan]); // "LYWSD03" mode
      break;
      case 5:
      MINRFwhiten((uint8_t *)&MINRF.buffer, sizeof(MINRF.buffer),  kMINRFlsfrList_B[MINRF.currentChan]); // "CGG1" mode
      break;
      case 6:
      MINRFwhiten((uint8_t *)&MINRF.buffer, sizeof(MINRF.buffer),  kMINRFlsfrList_B[MINRF.currentChan]); // "CGD1" mode
      break;
    }
    // DEBUG_SENSOR_LOG(PSTR("MINRF: LSFR:%x"),_lsfr);
    // if (_lsfr>254) _lsfr=0;
  }
  // DEBUG_SENSOR_LOG(PSTR("MINRF: did read FIFO"));
  return true;
}

#ifdef DEBUG_TASMOTA_SENSOR
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
#endif // DEBUG_TASMOTA_SENSOR

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
                            if(e.showedUp>2) AddLog_P2(LOG_LEVEL_INFO,PSTR("MINRF: Beacon %02u: %02X%02X%02X%02X%02X%02X Cid: %04X Svc: %04X UUID: %04X"),i,e.mac[0],e.mac[1],e.mac[2],e.mac[3],e.mac[4],e.mac[5],e.cid,e.svc,e.uuid);
                            i++;
                            return ((e.showedUp < 3));
                            }),
                          MINRFscanResult.end());
    MINRF.stopScan=false;
    return;
  }

  MINRFreverseMAC(MINRF.buffer.bleAdv.mac);
  for(uint32_t i=0; i<MINRFscanResult.size(); i++){
    if(memcmp(MINRF.buffer.bleAdv.mac,MINRFscanResult[i].mac,sizeof(MINRF.buffer.bleAdv.mac))==0){
      MINRFscanResult[i].showedUp++;
      // AddLog_P2(LOG_LEVEL_INFO,PSTR("MINRF: ADVk: %02x %02x %02x %02x %02x %02x"),MINRF.buffer.bleAdv.mac[0],MINRF.buffer.bleAdv.mac[1],MINRF.buffer.bleAdv.mac[2],MINRF.buffer.bleAdv.mac[3],MINRF.buffer.bleAdv.mac[4],MINRF.buffer.bleAdv.mac[5]);
      return;
    }
  }
  if(MINRF.buffer.raw[8]!=2 && MINRF.buffer.raw[9]!=1) return; //unsupported packet
  scan_entry_t _new;
  _new.showedUp = 1;
  _new.cid = 0;
  _new.svc = 0;
  _new.uuid = 0;
  memcpy(_new.mac,MINRF.buffer.bleAdv.mac,sizeof(_new.mac));
  memcpy(MINRF.beacon.mac,MINRF.buffer.bleAdv.mac,sizeof(_new.mac));
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
  memcpy(MINRF.beacon.mac,MINRFscanResult[entry].mac,sizeof(MINRF.beacon.mac));
  AddLog_P2(LOG_LEVEL_INFO,PSTR("MINRF: Beacon activated: %02x:%02x:%02x:%02x:%02x:%02x"),MINRF.beacon.mac[0],MINRF.beacon.mac[1],MINRF.beacon.mac[2],MINRF.beacon.mac[3],MINRF.beacon.mac[4],MINRF.beacon.mac[5]);
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

  if(memcmp((uint8_t*)&_buf[2],MINRF.beacon.mac,2)==0){ // always at least 2 undestroyed bytes left
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
void MINRFbeaconCounter(void){
  if(MINRF.beacon.active) {
    MINRF.beacon.time++;
    char stemp[20];
    snprintf_P(stemp, sizeof(stemp),PSTR("{%s:{\"Beacon\": %u}}"),D_CMND_NRF, MINRF.beacon.time);
    AddLog_P2(LOG_LEVEL_DEBUG, stemp);
    RulesProcessEvent(stemp);
  }
}

/**
 * @brief compute "PDU" from MAC for each possible channel and store it globally
 *
 */
void MINRFcomputeBeaconPDU(void){
  for (uint32_t i = 0; i<3; i++){
    bleAdvPacket_t packet;
    memcpy((uint8_t *)&packet.mac, (uint8_t *)&MINRF.beacon.mac, sizeof(packet.mac));
    MINRFreverseMAC(packet.mac);
    MINRFwhiten((uint8_t *)&packet, sizeof(packet), MINRF.channel[i] | 0x40);
    MINRFswapbuf((uint8_t*)&packet,sizeof(packet));
    uint32_t pdu = packet.mac[0]<<24 | packet.mac[1]<<16 | packet.mac[2]<<8 | packet.mac[3];
    MINRF.beacon.PDU[i] = pdu;
  }
}

/*********************************************************************************************\
 * helper functions
\*********************************************************************************************/

/**
 * @brief reverse 6-byte-array, hard-coded size of 6
 *
 * @param _mac pass an uint_t[6]
 */
void MINRFreverseMAC(uint8_t _mac[]){
  uint8_t _reversedMAC[6];
  for (uint8_t i=0; i<6; i++){
    _reversedMAC[5-i] = _mac[i];
  }
  memcpy(_mac,_reversedMAC, sizeof(_reversedMAC));
}

/**
 * @brief
 *
 * @param _string input string in format: AABBCCDDEEFF (upper case!)
 * @param _mac  target byte array with fixed size of 6
 */
void MINRFMACStringToBytes(char* _string, uint8_t _mac[]) { //uppercase
    uint32_t index = 0;
    while (index < 12) {
        char c = _string[index];
        uint8_t value = 0;
        if(c >= '0' && c <= '9')
          value = (c - '0');
        else if (c >= 'A' && c <= 'F')
          value = (10 + (c - 'A'));
        _mac[(index/2)] += value << (((index + 1) % 2) * 4);
        index++;
    }
    // DEBUG_SENSOR_LOG(PSTR("MINRF:  %s to MAC-array: %02X%02X%02X%02X%02X%02X"),_string,_mac[0],_mac[1],_mac[2],_mac[3],_mac[4],_mac[5]);
}

/**
 * @brief helper function, to avoid to start with an ignored sensor type
 *
 */
void MINRFcomputefirstUsedPacketMode(void){
  for (uint32_t i = 0; i<CGD1; i++){
    if (!bitRead(MINRF.ignore,i+1)) {
      DEBUG_SENSOR_LOG(PSTR("MINRF: FPM: %u"),i+1);
      MINRF.firstUsedPacketMode = i+1;
      if(MINRF.firstUsedPacketMode>CGD1) MINRF.firstUsedPacketMode=0;
      break;
    }
  }
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
      NRF24radio.openReadingPipe(0,kMINRFL3PDU[_nextchannel]);// 95 fe 58 30 -> LYWSD03 (= no data message)
    break;
    case 5: // special CGG1 packet
      NRF24radio.openReadingPipe(0,kMINRFCGGPDU[_nextchannel]); // 95 fe 50 30 -> CGG1
    break;
    case 6: // special CGD1 packet
      NRF24radio.openReadingPipe(0,kMINRFCGDPDU[_nextchannel]); // cd fd 08 0c -> CGD1
    break;
  }
  // DEBUG_SENSOR_LOG(PSTR("MINRF: Change Mode to %u"),_mode);
  MINRF.packetMode = _mode;
}

/**
 * @brief Return the slot number of a known sensor or return create new sensor slot
 *
 * @param _serial     BLE address of the sensor
 * @param _type       Type number of the sensor
 * @return uint32_t   Known or new slot in the sensors-vector
 */
uint32_t MINRFgetSensorSlot(uint8_t (&_serial)[6], uint16_t _type){

  DEBUG_SENSOR_LOG(PSTR("MINRF: will test ID-type: %x"), _type);
  bool _success = false;
  for (uint32_t i=0;i<6;i++){ // i < sizeof(kMINRFSlaveID) gives compiler warning
    if(_type == kMINRFSlaveID[i]){
      DEBUG_SENSOR_LOG(PSTR("MINRF: ID is type %u"), i);
      _type = i+1;
      _success = true;
    }
    else {
      DEBUG_SENSOR_LOG(PSTR("MINRF: ID-type is not: %x"),kMINRFSlaveID[i]);
    }
  }
  if(!_success) return 0xff;

  DEBUG_SENSOR_LOG(PSTR("MINRF: vector size %u"), MIBLEsensors.size());
  for(uint32_t i=0; i<MIBLEsensors.size(); i++){
    if(memcmp(_serial,MIBLEsensors[i].serial,sizeof(_serial))==0){
      DEBUG_SENSOR_LOG(PSTR("MINRF: known sensor at slot: %u"), i);
        if(MIBLEsensors[i].showedUp < 3){ // if we got an intact packet, the sensor should show up several times
          MIBLEsensors[i].showedUp++;     // count up to the above number ... now we are pretty sure
          DEBUG_SENSOR_LOG(PSTR("MINRF: showed up %u"),MIBLEsensors[i].showedUp);
          MINRFconfirmSensors();
      }
      return i;
    }
    DEBUG_SENSOR_LOG(PSTR("MINRF i: %x %x %x %x %x %x"), MIBLEsensors[i].serial[5], MIBLEsensors[i].serial[4],MIBLEsensors[i].serial[3],MIBLEsensors[i].serial[2],MIBLEsensors[i].serial[1],MIBLEsensors[i].serial[0]);
    DEBUG_SENSOR_LOG(PSTR("MINRF n: %x %x %x %x %x %x"), _serial[5], _serial[4], _serial[3],_serial[2],_serial[1],_serial[0]);
  }

  DEBUG_SENSOR_LOG(PSTR("MINRF: found new sensor"));
  mi_sensor_t _newSensor;
  memcpy(_newSensor.serial,_serial, sizeof(_serial));
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
  AddLog_P2(LOG_LEVEL_INFO,PSTR("MINRF: new %s at slot: %u"),kMINRFSlaveType[_type-1], MIBLEsensors.size()-1);
  return (MIBLEsensors.size()-1);
};

/**
 * @brief Remove "zombie" sensors after a certain amount of time.
 *        If they showed up less than 3 times, they are probably
 *        a product of data corruption.
 */
void MINRFpurgeFakeSensors(void){
  for(uint32_t i=0; i<MIBLEsensors.size(); i++){
    DEBUG_SENSOR_LOG(PSTR("MINRF: remove FAKE %s at slot: %u"),kMINRFSlaveType[MIBLEsensors[i].type-1], i);
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
 * @brief generic MiBeacon parser
 *
 */
void MINRFhandleMiBeaconPacket(void){
  MINRFreverseMAC(MINRF.buffer.miBeacon.Mac);
  uint32_t _slot = MINRFgetSensorSlot(MINRF.buffer.miBeacon.Mac, MINRF.buffer.miBeacon.productID);
  if(_slot==0xff) return;
  DEBUG_SENSOR_LOG(PSTR("MINRF: slot %u, size vector: %u %u"),_slot,MIBLEsensors.size());

  mi_sensor_t *_sensorVec = &MIBLEsensors.at(_slot);
  float _tempFloat;

  if (_sensorVec->type==MJ_HT_V1 || _sensorVec->type==CGG1){
    memcpy(MINRFtempBuf,(uint8_t*)&MINRF.buffer.miBeacon.spare, 32-9); // shift by one byte for the MJ_HT_V1 and CGG1
    memcpy((uint8_t*)&MINRF.buffer.miBeacon.type,MINRFtempBuf, 32-9);  // shift by one byte for the MJ_HT_V1 and CGG1
  }

  DEBUG_SENSOR_LOG(PSTR("%s at slot %u"), kNRFSlaveType[_sensorVec->type-1],_slot);
  switch(MINRF.buffer.miBeacon.type){
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
 * @brief more or less a placeholder, at least it is technically possible to really decrypt data, but
 *        the bind_key must be retrieved with 3rd-party-tools -> TODO
 */
void MINRFhandleLYWSD03Packet(void){
  // not much to do ATM, just show the sensor without data
  MINRFreverseMAC(MINRF.buffer.miBeacon.Mac);
  uint32_t _slot = MINRFgetSensorSlot(MINRF.buffer.miBeacon.Mac, MINRF.buffer.miBeacon.productID);
  DEBUG_SENSOR_LOG(PSTR("MINRF: Sensor slot: %u"), _slot);
    if(_slot==0xff) return;

  MINRF_LOG_BUFFER(MINRF.streamBuffer);
  MINRF_LOG_BUFFER(MINRF.lsfrBuffer);
  MINRF_LOG_BUFFER(MINRF.buffer.raw);
}

/**
 * @brief parse the Cleargrass-packet
 *        Note: battery section is based on "internet data" -> not confirmed yet
 */
void MINRFhandleCGD1Packet(void){ // no MiBeacon
  MINRFreverseMAC(MINRF.buffer.CGDPacket.serial);
  uint32_t _slot = MINRFgetSensorSlot(MINRF.buffer.CGDPacket.serial, 0x0576); // This must be hard-coded, no object-id in Cleargrass-packet
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
      case FLORA: case MJ_HT_V1: case LYWSD02: case CGG1:
        MINRFhandleMiBeaconPacket();
        break;
      case LYWSD03:
        MINRFhandleLYWSD03Packet();
        break;
      case CGD1:
        MINRFhandleCGD1Packet();
        break;
      default:
        break;
    }
  }
  if (MINRF.beacon.active || MINRF.activeScan) {
    MINRF.firstUsedPacketMode=0;
  }

  MINRF.packetMode = (MINRF.packetMode+1>CGD1) ? MINRF.firstUsedPacketMode : MINRF.packetMode+1;
  for (uint32_t i = MINRF.packetMode; i<CGD1+1; i++){
    if (bitRead(MINRF.ignore,i)) {
      MINRF.packetMode++;
    }
    else break;
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
            }
            else if (XdrvMailbox.payload < CGD1+1) {
              bitSet(MINRF.ignore,XdrvMailbox.payload);
              MINRFcomputefirstUsedPacketMode();
              MINRF.timer = 5900;
              Response_P(S_JSON_NRF_COMMAND, command, kMINRFSlaveType[XdrvMailbox.payload-1]);
            }
            else if (XdrvMailbox.payload == 255) {
              MINRF.ignore = 255;
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
              memset(MINRF.beacon.mac,0,sizeof(MINRF.beacon.mac));
              MINRFMACStringToBytes(XdrvMailbox.data, MINRF.beacon.mac);
              MINRF.beacon.time=0;
              MINRF.beacon.active=true;
              Response_P(S_JSON_NRF_COMMAND, command, XdrvMailbox.data);
            }
            MINRFcomputeBeaconPDU();
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
        break;
        }
      ResponseAppend_P(PSTR(",\"%s-%02x%02x%02x\":{"),kMINRFSlaveType[MIBLEsensors[i].type-1],MIBLEsensors[i].serial[3],MIBLEsensors[i].serial[4],MIBLEsensors[i].serial[5]);
      if (MIBLEsensors[i].type==FLORA && !isnan(MIBLEsensors[i].temp)){
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
      }
      if (MIBLEsensors[i].type>FLORA){
        if(!isnan(MIBLEsensors[i].temp) && !isnan(MIBLEsensors[i].hum)){
            ResponseAppendTHD(MIBLEsensors[i].temp,MIBLEsensors[i].hum);
        }
        if(MIBLEsensors[i].bat!=0x00){ // this is the error code -> no battery
          ResponseAppend_P(PSTR(",\"Battery\":%u"), MIBLEsensors[i].bat);
        }
        ResponseJsonEnd();
      }
    }
    if(MINRF.beacon.active){
    ResponseAppend_P(PSTR(",\"Beacon\":{\"Timer\":%u}"),MINRF.beacon.time);
    }
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
        WSContentSend_PD(HTTP_MINRF_MAC, kMINRFSlaveType[MIBLEsensors[i].type-1], D_MAC_ADDRESS, MIBLEsensors[i].serial[0], MIBLEsensors[i].serial[1],MIBLEsensors[i].serial[2],MIBLEsensors[i].serial[3],MIBLEsensors[i].serial[4],MIBLEsensors[i].serial[5]);
        if (MIBLEsensors[i].type==FLORA){
          if(!isnan(MIBLEsensors[i].temp)){
            char temperature[FLOATSZ];
            dtostrfd(MIBLEsensors[i].temp, Settings.flag2.temperature_resolution, temperature);
            WSContentSend_PD(HTTP_SNS_TEMP, kMINRFSlaveType[MIBLEsensors[i].type-1], temperature, TempUnit());
          }
          if(MIBLEsensors[i].lux!=0xffffffff){ // this is the error code -> no valid value
            WSContentSend_PD(HTTP_SNS_ILLUMINANCE, kMINRFSlaveType[MIBLEsensors[i].type-1], MIBLEsensors[i].lux);
          }
          if(!isnan(MIBLEsensors[i].moisture)){ // this is the error code -> no valid value
            WSContentSend_PD(HTTP_SNS_MOISTURE, kMINRFSlaveType[MIBLEsensors[i].type-1], MIBLEsensors[i].moisture);
          }
          if(!isnan(MIBLEsensors[i].fertility)){ // this is the error code -> no valid value
            WSContentSend_PD(HTTP_MINRF_FLORA_DATA, kMINRFSlaveType[MIBLEsensors[i].type-1], MIBLEsensors[i].fertility);
          }
        }
        if (MIBLEsensors[i].type>FLORA){ // everything "above" Flora
          WSContentSend_THD(kMINRFSlaveType[MIBLEsensors[i].type-1], MIBLEsensors[i].temp, MIBLEsensors[i].hum);
          if(MIBLEsensors[i].bat!=0x00){ // without "juice" nothing can be done
            WSContentSend_PD(HTTP_BATTERY, kMINRFSlaveType[MIBLEsensors[i].type-1], MIBLEsensors[i].bat);
          }
        }
      }
      if(MINRF.beacon.active){
        WSContentSend_PD(HTTP_MINRF_HL);
        WSContentSend_PD(HTTP_MINRF_HL);
        WSContentSend_PD(HTTP_MINRF_MAC, F("Beacon"), D_MAC_ADDRESS, MINRF.beacon.mac[0], MINRF.beacon.mac[1],MINRF.beacon.mac[2],MINRF.beacon.mac[3],MINRF.beacon.mac[4],MINRF.beacon.mac[5]);
        WSContentSend_PD(PSTR("{s}Beacon Time{m}%u seconds{e}"),MINRF.beacon.time);
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


