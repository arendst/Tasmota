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
* BLE-Sniffer/Bridge for MIJIA/XIAOMI Temperatur/Humidity-Sensor, Mi Flora, LYWSD02
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

const uint16_t kMINRFSlaveID[5]={ 0x0098, // Flora
                                  0x01aa, // MJ_HT_V1
                                  0x045b, // LYWSD02
                                  0x055b, // LYWSD03
                                  0x0347  // CGG1
                                  };

const char kMINRFSlaveType1[] PROGMEM = "Flora";
const char kMINRFSlaveType2[] PROGMEM = "MJ_HT_V1";
const char kMINRFSlaveType3[] PROGMEM = "LYWSD02";
const char kMINRFSlaveType4[] PROGMEM = "LYWSD03";
const char kMINRFSlaveType5[] PROGMEM = "CGG1";
const char * kMINRFSlaveType[] PROGMEM = {kMINRFSlaveType1,kMINRFSlaveType2,kMINRFSlaveType3,kMINRFSlaveType4,kMINRFSlaveType5};

// PDU's or different channels 37-39
const uint32_t kMINRFFloPDU[3] = {0x3eaa857d,0xef3b8730,0x71da7b46};
const uint32_t kMINRFMJPDU[3]  = {0x4760cd66,0xdbcc0cd3,0x33048df5};
const uint32_t kMINRFL2PDU[3]  = {0x3eaa057d,0xef3b0730,0x71da7646}; // 1 and 3 unsure
// const uint32_t kMINRFL3PDU[3]  = {0x4760dd78,0xdbcc1ccd,0xffffffff}; //encrypted - 58 58
const uint32_t kMINRFL3PDU[3]  = {0x4760cb78,0xdbcc0acd,0x33048beb}; //unencrypted  - 30 58
const uint32_t kMINRFCGPDU[3]  = {0x4760cd6e,0xdbcc0cdb,0x33048dfd};

// start-LSFR for different channels 37-39
const uint8_t kMINRFlsfrList_A[3] = {0x4b,0x17,0x23};  // Flora, LYWSD02
const uint8_t kMINRFlsfrList_B[3] = {0x21,0x72,0x43};  // MJ_HT_V1, LYWSD03, CGG1


#pragma pack(1)  // important!!
struct MJ_HT_V1Header_t {// related to the payload
  uint8_t padding[3];
  uint8_t mesSize;       // 3
  uint8_t padding2;
  uint16_t uuid;         // 5,6 -> 0xFE95
  uint16_t type;         //  7,8 -> 0x2050 MI-TH-V1
  uint8_t padding3[2];
  uint8_t counter;       // 11 - counts up with every sent record
  uint8_t serial[6];     // 12 - 17
  uint8_t mode;          // 18
  uint8_t padding5;
  uint8_t effectiveDataLength;
  };

struct FlowerHeader_t {        // related to the payload
  uint8_t padding[4];
  uint8_t padding2;
  uint16_t uuid;         // 5,6 -> 0xFE95
  uint8_t mesSize;
  uint8_t padding22;
  uint16_t uuid2;        // 9,10 -> 0xFE95
  uint16_t type;         //  11,12 -> 0x7120 Flowercare
  uint8_t padding3[2];
  uint8_t counter;       // 15 - counts up with every sent record
  uint8_t serial[6];     // 16 - 21
  uint8_t padding4;      //22
  uint8_t mode;          // 23
  };

union floraPacket_t {    // related to the whole 32-byte-packet/buffer
  struct {
      uint16_t idWord;
      uint8_t padding;
      uint8_t serial[6];
      uint8_t padding4;
      uint8_t mode;
      uint8_t valueTen;
      uint8_t effectiveDataLength; // 1
      uint16_t data;
  } T;    // mode 04
  struct {
      uint16_t idWord;
      uint8_t padding;
      uint8_t serial[6];
      uint8_t padding4;
      uint8_t mode;
      uint8_t valueTen;
      uint8_t effectiveDataLength; // 3
      uint32_t data:24; //  it is probably a real uint24_t
  } L;    // mode 07
  struct {
      uint8_t padding[3];
      uint8_t serial[6];
      uint8_t padding4;
      uint8_t mode;
      uint8_t valueTen;
      uint8_t effectiveDataLength; // 1
      uint8_t data;
  } M;    // mode 08
  struct {
      uint8_t padding[3];
      uint8_t serial[6];
      uint8_t padding4;
      uint8_t mode;
      uint8_t valueTen;
      uint8_t effectiveDataLength; // 2
      uint16_t data;
  } F;    // mode 09
};

union MJ_HT_V1Packet_t {    // related to the whole 32-byte-packet/buffer
  struct {
      uint16_t idWord;
      uint8_t padding;
      uint8_t serial[6];
      uint8_t mode;
      uint8_t valueTen;
      uint8_t effectiveDataLength; // 4
      uint16_t temp;
      uint16_t hum;
  } TH;   // mode 0d
      struct {
      uint8_t padding[3];
      uint8_t serial[6];
      uint8_t mode;
      uint8_t valueTen;
      uint8_t effectiveDataLength; // 1
      uint8_t battery;
  } B;    // mode 0a
  // We do NOT need the isolated T and H packet
};

union LYWSD02Packet_t {    // related to the whole 32-byte-packet/buffer
  struct {
      uint16_t idWord;
      uint8_t padding;
      uint8_t serial[6];
      uint8_t padding4;
      uint8_t mode;
      uint8_t valueTen;
      uint8_t effectiveDataLength;
      uint16_t data;
  } TH;    // mode 04 or 06
};

struct bleAdvPacket_t { // for nRF24L01 max 32 bytes = 2+6+24
  uint8_t pduType;
  uint8_t payloadSize;
  uint8_t mac[6];
  union {
    uint8_t payload[24];
    MJ_HT_V1Header_t header;
    FlowerHeader_t flowerHeader;
    struct {
        uint8_t padding[21];
        uint16_t temp;
        uint8_t hum_lb; // the high byte does not fit into the RX_buffer
    } TH;   // mode 0d
    struct {
        uint8_t padding[21];
        uint16_t temp;
    } T;    // mode 04
    struct {
        uint8_t padding[21];
        uint16_t hum;
    } H;    // mode 06
    struct {
        uint8_t padding[21];
        uint8_t battery;
    } B;    // mode 0a
    struct {
        uint8_t padding[2];
        uint8_t mode;
        uint16_t size; // 2
        uint16_t data;
    } F_T;    // mode 04
    struct {
        uint8_t padding[2];
        uint8_t mode;
        uint16_t size; // 3
        uint16_t data;
        uint8_t data2; // unknown meaning, maybe it is a real uint24_t (data with data2)
    } F_L;    // mode 07
    struct {
        uint8_t padding[2];
        uint8_t mode;
        uint16_t size; // 1
        uint8_t data;
    } F_M;    // mode 08
    struct {
        uint8_t padding[2];
        uint8_t mode;
        uint16_t size; // 2
        uint16_t data;
    } F_F;    // mode 09
  };
};

union FIFO_t{
  bleAdvPacket_t bleAdv;
  floraPacket_t floraPacket;
  MJ_HT_V1Packet_t MJ_HT_V1Packet;
  LYWSD02Packet_t LYWSD02Packet;
  uint8_t raw[32];
};

#pragma pack(0)

struct {
  const uint8_t channel[3]   = {37,38,39};  // BLE advertisement channel number
  const uint8_t frequency[3] = { 2,26,80};  // real frequency (2400+x MHz)

  uint16_t timer;
  uint8_t currentChan=0;
  FIFO_t buffer;
  uint8_t packetMode; // 0 - normal BLE-advertisements, 1 - special "flora"-packet, 2 - special "MJ_HT_V1"-packet

#ifdef DEBUG_TASMOTA_SENSOR
  uint8_t streamBuffer[sizeof(buffer)]; //  raw data stream bytes
  uint8_t lsfrBuffer[sizeof(buffer)];   //  correpsonding lfsr-bytes for the buffer, probably only useful for a BLE-packet
#endif //  DEBUG_TASMOTA_SENSOR

} MINRF;

struct mi_sensor_t{
  uint8_t type; //Flora = 1; MJ_HT_V1=2; LYWSD02=3; LYWSD03=4; ; CGG1=5
  uint8_t serial[6];
  uint8_t showedUp;
  float temp; //Flora, MJ_HT_V1, LYWSD0x
  union {
    struct {
      float moisture;
      float fertility;
      uint32_t lux;
    }; // Flora
    struct {
      float hum;
      uint8_t bat;
    }; // MJ_HT_V1, LYWSD0x
  };
};

std::vector<mi_sensor_t> MIBLEsensors;

/********************************************************************************************/


bool MINRFinitBLE(uint8_t _mode)
{
  if (MINRF.timer%1000 == 0){ // only re-init every 20 seconds
    NRF24radio.begin(pin[GPIO_SPI_CS],pin[GPIO_SPI_DC]);
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

void MINRFhopChannel()
{
  MINRF.currentChan++;
  if(MINRF.currentChan >= sizeof(MINRF.channel)) {
    MINRF.currentChan = 0;
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
    MINRFswapbuf( sizeof(MINRF.buffer) );
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
void MINRFswapbuf(uint8_t len)
{
  uint8_t* buf = (uint8_t*)&MINRF.buffer;
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

void MINRFreverseMAC(uint8_t _mac[]){
  uint8_t _reversedMAC[6];
  for (uint8_t i=0; i<6; i++){
    _reversedMAC[5-i] = _mac[i];
  }
  memcpy(_mac,_reversedMAC, sizeof(_reversedMAC));
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
      if(kMINRFL3PDU[_nextchannel]==0xffffffff) break;
      NRF24radio.openReadingPipe(0,kMINRFL3PDU[_nextchannel]);// 95 fe 58 30 -> LYWSD03 (= no data message)
    break;
    case 5: // special CGG1 packet
      NRF24radio.openReadingPipe(0,kMINRFCGPDU[_nextchannel]); // 95 fe 50 30 -> CGG1
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
  for (uint32_t i=0;i<5;i++){
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
    if(memcmp(_serial,MIBLEsensors.at(i).serial,sizeof(_serial))==0){
      DEBUG_SENSOR_LOG(PSTR("MINRF: known sensor at slot: %u"), i);
      if(MIBLEsensors.at(i).showedUp < 3){ // if we got an intact packet, the sensor should show up several times
        MIBLEsensors.at(i).showedUp++;     // count up to the above number ... now we are pretty sure
      }
      return i;
    }
    DEBUG_SENSOR_LOG(PSTR("MINRF i: %x %x %x %x %x %x"), MIBLEsensors.at(i).serial[5], MIBLEsensors.at(i).serial[4],MIBLEsensors.at(i).serial[3],MIBLEsensors.at(i).serial[2],MIBLEsensors.at(i).serial[1],MIBLEsensors.at(i).serial[0]);
    DEBUG_SENSOR_LOG(PSTR("MINRF n: %x %x %x %x %x %x"), _serial[5], _serial[4], _serial[3],_serial[2],_serial[1],_serial[0]);
  }
  DEBUG_SENSOR_LOG(PSTR("MINRF: found new sensor"));
  mi_sensor_t _newSensor;
  memcpy(_newSensor.serial,_serial, sizeof(_serial));
  _newSensor.type = _type;
  _newSensor.showedUp = 1;
  _newSensor.temp =-1000.0f;
  switch (_type)
    {
    case 1:
      _newSensor.moisture =-1000.0f;
      _newSensor.fertility =-1000.0f;
      _newSensor.lux = 0x00ffffff;
      break;
    case 2: case 3: case 4:
      _newSensor.hum=-1.0f;
      _newSensor.bat=0xff;
      break;
    default:
      break;
    }
  MIBLEsensors.push_back(_newSensor);
  DEBUG_SENSOR_LOG(PSTR("MINRF: new sensor at slot: %u"), MIBLEsensors.size()-1);
  return MIBLEsensors.size()-1;
};

/**
 * @brief Remove "zombie" sensors after a certain amount of time.
 *        If they showed up less than 3 times, they are probably
 *        a product of data corruption.
 */
void MINRFpurgeFakeSensors(void){
  for(uint32_t i=0; i<MIBLEsensors.size(); i++){
    if(MIBLEsensors.at(i).showedUp<3){
      DEBUG_SENSOR_LOG(PSTR("MINRF: remove FAKE sensor at slot: %u"), i);
      MIBLEsensors.erase(MIBLEsensors.begin()+i);
    }
  }
}


void MINRFhandleFloraPacket(void){
  if(MINRF.buffer.floraPacket.T.valueTen!=0x10){
    DEBUG_SENSOR_LOG(PSTR("MINRF: unexpected Flora packet"));
    MINRF_LOG_BUFFER(MINRF.buffer.raw);
    return;
  }
  MINRFreverseMAC(MINRF.buffer.floraPacket.T.serial);
  uint32_t _slot = MINRFgetSensorSlot(MINRF.buffer.floraPacket.T.serial, MINRF.buffer.floraPacket.T.idWord); // T is not specific, any struct would be possible to use
  DEBUG_SENSOR_LOG(PSTR("MINRF: Sensor slot: %u"), _slot);
  if(_slot==0xff) return;

  static float _tempFloat;
  switch(MINRF.buffer.floraPacket.L.mode) { // we can use any struct with a mode, they are all same at this point
    case 4:
    _tempFloat=(float)(MINRF.buffer.floraPacket.T.data)/10.0f;
    if(_tempFloat<60){
        MIBLEsensors.at(_slot).temp=_tempFloat;
    }
    DEBUG_SENSOR_LOG(PSTR("Flora: Mode 4: U16:  %x Temp"), MINRF.buffer.floraPacket.T.data );
    break;
    case 7:
    if(true){
        MIBLEsensors.at(_slot).lux=MINRF.buffer.floraPacket.L.data;
    }
    DEBUG_SENSOR_LOG(PSTR("Flora: Mode 7: U24: %x Lux"), MINRF.buffer.floraPacket.L.data);
    break;
    case 8:
    _tempFloat =(float)MINRF.buffer.floraPacket.M.data;
    if(_tempFloat<100){
        MIBLEsensors.at(_slot).moisture=_tempFloat;
    }
    DEBUG_SENSOR_LOG(PSTR("Flora: Mode 8: U8: %x Moisture"), MINRF.buffer.floraPacket.M.data);
    break;
    case 9:
    _tempFloat=(float)(MINRF.buffer.floraPacket.F.data);
    if(_tempFloat<65535){ // ???
        MIBLEsensors.at(_slot).fertility=_tempFloat;
    }
    DEBUG_SENSOR_LOG(PSTR("Mode 9: U16: %x Fertility"), MINRF.buffer.floraPacket.F.data);
    break;
  }
}

void MINRFhandleMJ_HT_V1Packet(void){
  if(MINRF.buffer.MJ_HT_V1Packet.TH.valueTen!=0x10){
    DEBUG_SENSOR_LOG(PSTR("MINRF: unexpected MJ_HT_V1-packet"));
    MINRF_LOG_BUFFER(MINRF.buffer.raw);
    return;
  }
  MINRFreverseMAC(MINRF.buffer.MJ_HT_V1Packet.TH.serial);
  uint32_t _slot = MINRFgetSensorSlot(MINRF.buffer.MJ_HT_V1Packet.TH.serial, MINRF.buffer.MJ_HT_V1Packet.TH.idWord); // B would be possible too
  DEBUG_SENSOR_LOG(PSTR("MINRF: Sensor slot: %u"), _slot);
  if(_slot==0xff) return;

  static float _tempFloat;
  switch(MINRF.buffer.MJ_HT_V1Packet.TH.mode) { // we can use any struct with a mode, they are all same at this point
    case 0x0d:
    _tempFloat=(float)(MINRF.buffer.MJ_HT_V1Packet.TH.temp)/10.0f;
    if(_tempFloat<60){
        MIBLEsensors.at(_slot).temp = _tempFloat;
        DEBUG_SENSOR_LOG(PSTR("MJ_HT_V1: temp updated"));
    }
    _tempFloat=(float)(MINRF.buffer.MJ_HT_V1Packet.TH.hum)/10.0f;
    if(_tempFloat<100){
        MIBLEsensors.at(_slot).hum = _tempFloat;
        DEBUG_SENSOR_LOG(PSTR("MJ_HT_V1: hum updated"));
    }
    DEBUG_SENSOR_LOG(PSTR("MJ_HT_V1 mode:0x0d: U16:  %x Temp U16: %x Hum"), MINRF.buffer.MJ_HT_V1Packet.TH.temp,  MINRF.buffer.MJ_HT_V1Packet.TH.hum);
    break;
    case 0x0a:
    if(MINRF.buffer.MJ_HT_V1Packet.B.battery<101){
        MIBLEsensors.at(_slot).bat = MINRF.buffer.MJ_HT_V1Packet.B.battery;
        DEBUG_SENSOR_LOG(PSTR("MJ_HT_V1: bat updated"));
    }
    DEBUG_SENSOR_LOG(PSTR("MJ_HT_V1 mode:0x0a: U8: %x %%"), MINRF.buffer.MJ_HT_V1Packet.B.battery);
    break;
  }
}

void MINRFhandleLYWSD02Packet(void){
  if(MINRF.buffer.LYWSD02Packet.TH.valueTen!=0x10){
    DEBUG_SENSOR_LOG(PSTR("MINRF: unexpected LYWSD02-packet"));
    MINRF_LOG_BUFFER(MINRF.buffer.raw);
    return;
  }
  MINRFreverseMAC(MINRF.buffer.LYWSD02Packet.TH.serial);
  uint32_t _slot = MINRFgetSensorSlot(MINRF.buffer.LYWSD02Packet.TH.serial, MINRF.buffer.LYWSD02Packet.TH.idWord); // H would be possible too
  DEBUG_SENSOR_LOG(PSTR("MINRF: Sensor slot: %u"), _slot);
  if(_slot==0xff) return;

  static float _tempFloat;
  switch(MINRF.buffer.LYWSD02Packet.TH.mode) { // we can use any struct with a mode, they are all same at this point
    case 4:
    _tempFloat=(float)(MINRF.buffer.LYWSD02Packet.TH.data)/10.0f;
    if(_tempFloat<60){
        MIBLEsensors.at(_slot).temp=_tempFloat;
    }
    DEBUG_SENSOR_LOG(PSTR("LYWSD02: Mode 4: U16:  %x Temp"), MINRF.buffer.LYWSD02Packet.TH.data );
    break;
    case 6:
    _tempFloat=(float)(MINRF.buffer.LYWSD02Packet.TH.data)/10.0f;
    if(_tempFloat<101){
        MIBLEsensors.at(_slot).hum=_tempFloat;
    }
    DEBUG_SENSOR_LOG(PSTR("LYWSD02: Mode 6: U16:  %x Hum"), MINRF.buffer.LYWSD02Packet.TH.data );
    break;
  }
}

void MINRFhandleLYWSD03Packet(void){
  // not much to do ATM, just show the sensor without data
  MINRFreverseMAC(MINRF.buffer.LYWSD02Packet.TH.serial); //the beginning is equal to the LYWSD02-packet
  uint32_t _slot = MINRFgetSensorSlot(MINRF.buffer.LYWSD02Packet.TH.serial, MINRF.buffer.LYWSD02Packet.TH.idWord);
  DEBUG_SENSOR_LOG(PSTR("MINRF: Sensor slot: %u"), _slot);
    if(_slot==0xff) return;

  MINRF_LOG_BUFFER(MINRF.streamBuffer);
  MINRF_LOG_BUFFER(MINRF.lsfrBuffer);
  MINRF_LOG_BUFFER(MINRF.buffer.raw);
}

void MINRFhandleCGG1Packet(void){ // we assume, that the packet structure is equal to the MJ_HT_V1
  if(MINRF.buffer.MJ_HT_V1Packet.TH.valueTen!=0x10){
    DEBUG_SENSOR_LOG(PSTR("MINRF: unexpected CGG1-packet"));
    MINRF_LOG_BUFFER(MINRF.buffer.raw);
    return;
  }
  MINRFreverseMAC(MINRF.buffer.MJ_HT_V1Packet.TH.serial);
  uint32_t _slot = MINRFgetSensorSlot(MINRF.buffer.MJ_HT_V1Packet.TH.serial, MINRF.buffer.MJ_HT_V1Packet.TH.idWord); // B would be possible too
  DEBUG_SENSOR_LOG(PSTR("MINRF: Sensor slot: %u"), _slot);
  if(_slot==0xff) return;

  static float _tempFloat;
  switch(MINRF.buffer.MJ_HT_V1Packet.TH.mode) { // we can use any struct with a mode, they are all same at this point
    case 0x0d:
    _tempFloat=(float)(MINRF.buffer.MJ_HT_V1Packet.TH.temp)/10.0f;
    if(_tempFloat<60){
        MIBLEsensors.at(_slot).temp = _tempFloat;
        DEBUG_SENSOR_LOG(PSTR("CGG1: temp updated"));
    }
    _tempFloat=(float)(MINRF.buffer.MJ_HT_V1Packet.TH.hum)/10.0f;
    if(_tempFloat<100){
        MIBLEsensors.at(_slot).hum = _tempFloat;
        DEBUG_SENSOR_LOG(PSTR("CGG1: hum updated"));
    }
    DEBUG_SENSOR_LOG(PSTR("CGG1 mode:0x0d: U16:  %x Temp U16: %x Hum"), MINRF.buffer.MJ_HT_V1Packet.TH.temp,  MINRF.buffer.MJ_HT_V1Packet.TH.hum);
    break;
    case 0x0a:
    if(MINRF.buffer.MJ_HT_V1Packet.B.battery<101){
        MIBLEsensors.at(_slot).bat = MINRF.buffer.MJ_HT_V1Packet.B.battery;
        DEBUG_SENSOR_LOG(PSTR("CGG1: bat updated"));
    }
    DEBUG_SENSOR_LOG(PSTR("CGG1 mode:0x0a: U8: %x %%"), MINRF.buffer.MJ_HT_V1Packet.B.battery);
    break;
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

  else if(MINRF.buffer.bleAdv.header.uuid==0xfe95){ // XIAOMI-BLE-Packet
    MINRF_LOG_BUFFER(MINRF.streamBuffer);
    MINRF_LOG_BUFFER(MINRF.lsfrBuffer);
    MINRF_LOG_BUFFER(MINRF.buffer.raw);
    DEBUG_SENSOR_LOG(PSTR("MINRF: Type: %x"), MINRF.buffer.bleAdv.header.type);
    switch(MINRF.buffer.bleAdv.header.type){
      case 0x2050:
      DEBUG_SENSOR_LOG(PSTR("MINRF: MJ_HT_V1 Packet"));
      break;
      case 0x1613:case 0x1614:case 0x1615:
      DEBUG_SENSOR_LOG(PSTR("MINRF: Flora Packet"));
      break;
      default:
      DEBUG_SENSOR_LOG(PSTR("MINRF: unknown Packet"));
      break;
    }
  }
  else if (MINRF.packetMode == FLORA){
    MINRFhandleFloraPacket();
  }
  else if (MINRF.packetMode == MJ_HT_V1){
    MINRFhandleMJ_HT_V1Packet();
  }
  else if (MINRF.packetMode == LYWSD02){
    MINRFhandleLYWSD02Packet();
  }
  else if (MINRF.packetMode == LYWSD03){
    MINRFhandleLYWSD03Packet();
  }
  else if (MINRF.packetMode == CGG1){
    MINRFhandleCGG1Packet();
  }
  if (MINRF.packetMode == CGG1){
    MINRFinitBLE(1); // no real ble packets in release mode, otherwise for developing use 0
  }
  else {
    MINRFinitBLE(++MINRF.packetMode);
  }

  MINRFhopChannel();
  NRF24radio.startListening();
}
/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

const char HTTP_BATTERY[] PROGMEM = "{s}%s" " Battery" "{m}%u%%{e}";
const char HTTP_MINRF_MAC[] PROGMEM = "{s}%s %s{m}%02x:%02x:%02x:%02x:%02x:%02x%{e}";
const char HTTP_MINRF_FLORA_DATA[] PROGMEM =  "{s}%s" " Fertility" "{m}%sus/cm{e}";
const char HTTP_MINRF_HL[] PROGMEM = "{s}<hr>{m}<hr>{e}";

void MINRFShow(bool json)
{
  if (json) {
    for (uint32_t i = 0; i < MIBLEsensors.size(); i++) {
      if(MIBLEsensors.at(i).showedUp < 3){
        DEBUG_SENSOR_LOG(PSTR("MINRF: sensor not fully registered yet"));
        break;
        }
      char slave[33];
      sprintf_P(slave,"%s-%02x%02x%02x",kMINRFSlaveType[MIBLEsensors.at(i).type-1],MIBLEsensors.at(i).serial[3],MIBLEsensors.at(i).serial[4],MIBLEsensors.at(i).serial[5]);
      char temperature[33]; // all sensors have temperature
      dtostrfd(MIBLEsensors.at(i).temp, Settings.flag2.temperature_resolution, temperature);

      ResponseAppend_P(PSTR(",\"%s\":{"),slave);
        if(MIBLEsensors.at(i).temp!=-1000.0f){ // this is the error code -> no temperature
          ResponseAppend_P(PSTR("\"" D_JSON_TEMPERATURE "\":%s"), temperature);
        }
        if (MIBLEsensors.at(i).type==FLORA){
          char lux[33];
          char moisture[33];
          char fertility[33];
          dtostrfd((float)MIBLEsensors.at(i).lux, 0, lux);
          dtostrfd(MIBLEsensors.at(i).moisture, 0, moisture);
          dtostrfd(MIBLEsensors.at(i).fertility, 0, fertility);
          if(MIBLEsensors.at(i).lux!=0xffff){ // this is the error code -> no temperature
            ResponseAppend_P(PSTR(",\"" D_JSON_ILLUMINANCE "\":%s"), lux);
          }
          if(MIBLEsensors.at(i).moisture!=-1000.0f){ // this is the error code -> no moisture
            ResponseAppend_P(PSTR(",\"" D_JSON_MOISTURE "\":%s"), moisture);
          }
          if(MIBLEsensors.at(i).fertility!=-1000.0f){ // this is the error code -> no fertility
            ResponseAppend_P(PSTR(",\"Fertility\":%s"), fertility);
          }
        }
        if (MIBLEsensors.at(i).type>FLORA){ 
          char humidity[33];
          dtostrfd(MIBLEsensors.at(i).hum, Settings.flag2.humidity_resolution, humidity);
          if(MIBLEsensors.at(i).hum!=-1.0f){ // this is the error code -> no humidity
              ResponseAppend_P(PSTR(",\"" D_JSON_HUMIDITY "\":%s"), humidity);
          }
          if(MIBLEsensors.at(i).bat!=0xff){ // this is the error code -> no battery
            ResponseAppend_P(PSTR(",\"Battery\":%u"), MIBLEsensors.at(i).bat);
          }
        }
        ResponseAppend_P(PSTR("}"));
    }
#ifdef USE_WEBSERVER
    } else {
      WSContentSend_PD(HTTP_NRF24, NRF24type, NRF24.chipType);
      for (uint32_t i = 0; i < MIBLEsensors.size(); i++) {
        if(MIBLEsensors.at(i).showedUp < 3){
          DEBUG_SENSOR_LOG(PSTR("MINRF: sensor not fully registered yet"));
          break;
          }
        WSContentSend_PD(HTTP_MINRF_HL);
        WSContentSend_PD(HTTP_MINRF_MAC, kMINRFSlaveType[MIBLEsensors.at(i).type-1], D_MAC_ADDRESS, MIBLEsensors.at(i).serial[0], MIBLEsensors.at(i).serial[1],MIBLEsensors.at(i).serial[2],MIBLEsensors.at(i).serial[3],MIBLEsensors.at(i).serial[4],MIBLEsensors.at(i).serial[5]); 
        if(MIBLEsensors.at(i).temp!=-1000.0f){
          char temperature[33];
          dtostrfd(MIBLEsensors.at(i).temp, Settings.flag2.temperature_resolution, temperature);
          WSContentSend_PD(HTTP_SNS_TEMP, kMINRFSlaveType[MIBLEsensors.at(i).type-1], temperature, TempUnit());
        }
        if (MIBLEsensors.at(i).type==FLORA){
          if(MIBLEsensors.at(i).lux!=0x00ffffff){ // this is the error code -> no valid value
            WSContentSend_PD(HTTP_SNS_ILLUMINANCE, kMINRFSlaveType[MIBLEsensors.at(i).type-1], MIBLEsensors.at(i).lux);
          }
          if(MIBLEsensors.at(i).moisture!=-1000.0f){ // this is the error code -> no valid value
            WSContentSend_PD(HTTP_SNS_MOISTURE, kMINRFSlaveType[MIBLEsensors.at(i).type-1], MIBLEsensors.at(i).moisture);
          }
          if(MIBLEsensors.at(i).fertility!=-1000.0f){ // this is the error code -> no valid value
            char fertility[33];
            dtostrfd(MIBLEsensors.at(i).fertility, 0, fertility);
            WSContentSend_PD(HTTP_MINRF_FLORA_DATA, kMINRFSlaveType[MIBLEsensors.at(i).type-1], fertility);
          }
        }
        if (MIBLEsensors.at(i).type>FLORA){ // everything "above" Flora
          if(MIBLEsensors.at(i).hum!=-1.0f){ // this is the error code -> no humidity
            char humidity[33];
            dtostrfd(MIBLEsensors.at(i).hum, Settings.flag2.humidity_resolution, humidity);
            WSContentSend_PD(HTTP_SNS_HUM, kMINRFSlaveType[MIBLEsensors.at(i).type-1], humidity);
          }
          if(MIBLEsensors.at(i).bat!=0xff){
            WSContentSend_PD(HTTP_BATTERY, kMINRFSlaveType[MIBLEsensors.at(i).type-1], MIBLEsensors.at(i).bat);
          }
        }
      }
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


