/*
  xsns_61_Ml_BLE.ino - MI-BLE-sensors via nrf24l01 support for Tasmota

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

  ---
  0.9.0.0 20191127  started - further development by Christian Baars
                    base    - code base from cbm80amiga, floe, Dmitry.GR
                    forked  - from arendst/tasmota            - https://github.com/arendst/Tasmota

*/

#ifdef USE_SPI
#ifdef USE_NRF24
#ifdef USE_MIBLE

#ifdef DEBUG_TASMOTA_SENSOR
  #define MIBLE_LOG_BUFFER(x) MIBLEshowBuffer(x);
#else
  #define MIBLE_LOG_BUFFER(x)
#endif


/*********************************************************************************************\
* MIBLE
* BLE-Sniffer/Bridge for MIJIA/XIAOMI Temperatur/Humidity-Sensor, Mi Flora
*
* Usage: Configure NRF24
\*********************************************************************************************/

#define XSNS_61             61

#include <vector>

const char MIBLESlaveFlora[] PROGMEM = "Flora";
const char MIBLESlaveMJ_HT_V1[] PROGMEM = "MJ_HT_V1";


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
      uint16_t data;
      uint8_t data2; // unknown meaning, maybe it is a real uint24_t (data with data2)
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

} MIBLE;

struct mi_sensor_t{
  uint8_t type; //flora = 1; MI-HT_V1=2
  uint8_t serial[6];
  uint8_t showedUp;
  union {
    struct {
      float temp;
      float moisture;
      float fertility;
      uint16_t lux;
    } Flora;
    struct {
      float temp;
      float hum;
      uint8_t bat;
    } MJ_HT_V1;
  };
};

std::vector<mi_sensor_t> MIBLEsensors;

/********************************************************************************************/


bool MIBLEinitBLE(uint8_t _mode)
{
  NRF24radio.begin(pin[GPIO_SPI_CS],pin[GPIO_SPI_DC]);
  NRF24radio.setAutoAck(false);
  NRF24radio.setDataRate(RF24_1MBPS);
  NRF24radio.disableCRC();
  NRF24radio.setChannel( MIBLE.frequency[MIBLE.currentChan] );
  NRF24radio.setRetries(0,0);
  NRF24radio.setPALevel(RF24_PA_MIN); // we only receive
  NRF24radio.setAddressWidth(4);
  // NRF24radio.openReadingPipe(0,0x6B7D9171); // advertisement address: 0x8E89BED6 (bit-reversed -> 0x6B7D9171)
  // NRF24radio.openWritingPipe(  0x6B7D9171); // not used ATM
  NRF24radio.powerUp();

  if(NRF24radio.isChipConnected()){
    DEBUG_SENSOR_LOG(PSTR("MIBLE chip connected"));
    MIBLEchangePacketModeTo(_mode);
    return true;
  }
  DEBUG_SENSOR_LOG(PSTR("MIBLE chip NOT !!!! connected"));
  return false;
}

void MIBLEhopChannel()
{
  MIBLE.currentChan++;
  if(MIBLE.currentChan >= sizeof(MIBLE.channel)) {
    MIBLE.currentChan = 0;
  }
  NRF24radio.setChannel( MIBLE.frequency[MIBLE.currentChan] );
}

/**
 * @brief Read out FIFO-buffer, swap buffer and whiten
 *
 * @return true - If something is in the buffer
 * @return false - Nothing is in the buffer
 */
bool MIBLEreceivePacket(void)
{
  if(!NRF24radio.available()) {
    return false;
  }
  while(NRF24radio.available()) {
    // static uint8_t _lsfr = 0; //-> for testing out suitable lsfr-start-values for yet unknown packets
    // _lsfr++;
    NRF24radio.read( &MIBLE.buffer, sizeof(MIBLE.buffer) );
#ifdef DEBUG_TASMOTA_SENSOR
    memcpy(&MIBLE.streamBuffer, &MIBLE.buffer, sizeof(MIBLE.buffer));
#endif // DEBUG_TASMOTA_SENSOR
    MIBLEswapbuf( sizeof(MIBLE.buffer) );
    // MIBLE_LOG_BUFFER();
    switch (MIBLE.packetMode) {
      case 0:
      MIBLEwhiten((uint8_t *)&MIBLE.buffer, sizeof(MIBLE.buffer),  MIBLE.channel[MIBLE.currentChan] | 0x40);
      break;
      case 1:
      MIBLEwhiten((uint8_t *)&MIBLE.buffer, sizeof(MIBLE.buffer),  0x17); // "flora" mode 0x17
      break;
      case 2:
      MIBLEwhiten((uint8_t *)&MIBLE.buffer, sizeof(MIBLE.buffer),  0x72); // "MJ_HT_V1" mode 0x72
      break;
    }
    // DEBUG_SENSOR_LOG(PSTR("MIBLE: LSFR:%x"),_lsfr);
    // if (_lsfr>254) _lsfr=0;
  }
  // DEBUG_SENSOR_LOG(PSTR("MIBLE: did read FIFO"));
  return true;
}

#ifdef DEBUG_TASMOTA_SENSOR
void MIBLEshowBuffer(uint8_t (&buf)[32]){ // we use this only for the 32-byte-FIFO-buffer, so 32 is hardcoded
  // DEBUG_SENSOR_LOG(PSTR("MIBLE: Buffer: %c %c %c %c %c %c %c %c"
  //                                       " %c %c %c %c %c %c %c %c"
  //                                       " %c %c %c %c %c %c %c %c"
  //                                       " %c %c %c %c %c %c %c %c")
  DEBUG_SENSOR_LOG(PSTR("MIBLE: Buffer: %02x %02x %02x %02x %02x %02x %02x %02x "
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
void MIBLEswapbuf(uint8_t len)
{
  uint8_t* buf = (uint8_t*)&MIBLE.buffer;
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
void MIBLEwhiten(uint8_t *buf, uint8_t len, uint8_t lfsr)
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
    MIBLE.lsfrBuffer[31-len] = lfsr;
#endif //DEBUG_TASMOTA_SENSOR
  }
}


/**
 * @brief Set packet mode and fitting PDU-type of the NRF24L01
 *
 * @param _mode The internal packet mode number
 */
void MIBLEchangePacketModeTo(uint8_t _mode) {
  switch(_mode){
    case 0: // normal BLE advertisement
      NRF24radio.openReadingPipe(0,0x6B7D9171); // advertisement address: 0x8E89BED6 (bit-reversed -> 0x6B7D9171)
    break;
    case 1: // special flora packet
      NRF24radio.openReadingPipe(0,0xef3b8730); // 95 fe 71 20 -> flora, needs lfsr 0x17
    break;
    case 2: // special MJ_HT_V1 packet
      NRF24radio.openReadingPipe(0,0xdbcc0cd3); // 95 fe 50 20 -> MJ_HT_V1, needs lsfr 0x72
    break;
  }
  DEBUG_SENSOR_LOG(PSTR("MIBLE: Change Mode to %u"),_mode);
  MIBLE.timer = 0;
  MIBLE.packetMode = _mode;
}

/**
 * @brief Return the slot number of a known sensor or return create new sensor slot
 *
 * @param _serial     BLE address of the sensor
 * @param _type       Type number of the sensor
 * @return uint32_t   Known or new slot in the sensors-vector
 */
uint32_t MIBLEgetSensorSlot(uint8_t (&_serial)[6], uint8_t _type){
  DEBUG_SENSOR_LOG(PSTR("MIBLE: vector size %u"), MIBLEsensors.size());
  for(uint32_t i=0; i<MIBLEsensors.size(); i++){
    if(memcmp(_serial,MIBLEsensors.at(i).serial,sizeof(_serial))==0){
      DEBUG_SENSOR_LOG(PSTR("MIBLE: known sensor at slot: %u"), i);
      if(MIBLEsensors.at(i).showedUp < 3){ // if we got an intact packet, the sensor should show up several times
        MIBLEsensors.at(i).showedUp++;     // count up to the above number ... now we are pretty sure
      }
      return i;
    }
    DEBUG_SENSOR_LOG(PSTR("MIBLE i: %x %x %x %x %x %x"), MIBLEsensors.at(i).serial[5], MIBLEsensors.at(i).serial[4],MIBLEsensors.at(i).serial[3],MIBLEsensors.at(i).serial[2],MIBLEsensors.at(i).serial[1],MIBLEsensors.at(i).serial[0]);
    DEBUG_SENSOR_LOG(PSTR("MIBLE n: %x %x %x %x %x %x"), _serial[5], _serial[4], _serial[3],_serial[2],_serial[1],_serial[0]);
  }
  DEBUG_SENSOR_LOG(PSTR("MIBLE: found new sensor"));
  mi_sensor_t _newSensor;
  memcpy(_newSensor.serial,_serial, sizeof(_serial));
  _newSensor.type = _type;
  _newSensor.showedUp = 1;
  switch (_type)
    {
    case 1:
      _newSensor.Flora.temp =-1000.0f;
      _newSensor.Flora.moisture =-1000.0f;
      _newSensor.Flora.fertility =-1000.0f;
      _newSensor.Flora.lux = 0xffff;
      break;
    case 2:
      _newSensor.MJ_HT_V1.temp=-1000.0f;
      _newSensor.MJ_HT_V1.hum=-1.0f;
      _newSensor.MJ_HT_V1.bat=0xff;
      break;
    default:
      break;
    }
  MIBLEsensors.push_back(_newSensor);
  DEBUG_SENSOR_LOG(PSTR("MIBLE: new sensor at slot: %u"), MIBLEsensors.size()-1);
  return MIBLEsensors.size()-1;
};

/**
 * @brief Remove "zombie" sensors after a certain amount of time.
 *        If they showed up less than 3 times, they are probably
 *        a product of data corruption.
 */
void MIBLEpurgeFakeSensors(void){
  for(uint32_t i=0; i<MIBLEsensors.size(); i++){
    if(MIBLEsensors.at(i).showedUp<3){
      DEBUG_SENSOR_LOG(PSTR("MIBLE: remove FAKE sensor at slot: %u"), i);
      MIBLEsensors.erase(MIBLEsensors.begin()+i);
    }
  }
}


void MIBLEhandleFloraPacket(void){
  if(MIBLE.buffer.floraPacket.T.idWord!=0x9800 && MIBLE.buffer.floraPacket.T.valueTen!=0x10){
    DEBUG_SENSOR_LOG(PSTR("MIBLE: unexpected Flora packet"));
    MIBLE_LOG_BUFFER(MIBLE.buffer.raw);
    return;
  }
  uint32_t _slot = MIBLEgetSensorSlot(MIBLE.buffer.floraPacket.T.serial, 1); // T is not specific, any struct would be possible to use
  DEBUG_SENSOR_LOG(PSTR("MIBLE: Sensor slot: %u"), _slot);

  static float _tempFloat;
  switch(MIBLE.buffer.floraPacket.L.mode) { // we can use any struct with a mode, they are all same at this point
    case 4:
    _tempFloat=(float)(MIBLE.buffer.floraPacket.T.data)/10.0f;
    if(_tempFloat<60){
        MIBLEsensors.at(_slot).Flora.temp=_tempFloat;
    }
    DEBUG_SENSOR_LOG(PSTR("Flora: Mode 4: U16:  %x Temp"), MIBLE.buffer.floraPacket.T.data );
    break;
    case 7:
    _tempFloat=MIBLE.buffer.floraPacket.L.data;
    if(_tempFloat<65535){
        MIBLEsensors.at(_slot).Flora.lux=_tempFloat;
    }
    DEBUG_SENSOR_LOG(PSTR("Flora: Mode 7: U8: %x  U16: %x Lux"), MIBLE.buffer.floraPacket.L.data, MIBLE.buffer.floraPacket.L.data);
    break;
    case 8:
    _tempFloat =(float)MIBLE.buffer.floraPacket.M.data;
    if(_tempFloat<100){
        MIBLEsensors.at(_slot).Flora.moisture=_tempFloat;
    }
    DEBUG_SENSOR_LOG(PSTR("Flora: Mode 8: U8: %x Moisture"), MIBLE.buffer.floraPacket.M.data);
    break;
    case 9:
    _tempFloat=(float)(MIBLE.buffer.floraPacket.F.data);
    if(_tempFloat<65535){ // ???
        MIBLEsensors.at(_slot).Flora.fertility=_tempFloat;
    }
    DEBUG_SENSOR_LOG(PSTR("Mode 9: U16: %x Fertility"), MIBLE.buffer.floraPacket.F.data);
    break;
  }
}

void MIBLEhandleMJ_HT_V1Packet(void){
  if(MIBLE.buffer.MJ_HT_V1Packet.TH.idWord != 0xaa01 && MIBLE.buffer.MJ_HT_V1Packet.TH.valueTen!=0x10){
    DEBUG_SENSOR_LOG(PSTR("MIBLE: unexpected MJ_HT_V1-packet"));
    MIBLE_LOG_BUFFER(MIBLE.buffer.raw);
    return;
  }
  uint32_t _slot = MIBLEgetSensorSlot(MIBLE.buffer.MJ_HT_V1Packet.TH.serial, 2); // B would be possible too
  DEBUG_SENSOR_LOG(PSTR("MIBLE: Sensor slot: %u"), _slot);

  static float _tempFloat;
  switch(MIBLE.buffer.MJ_HT_V1Packet.TH.mode) { // we can use any struct with a mode, they are all same at this point
    case 0x0d:
    _tempFloat=(float)(MIBLE.buffer.MJ_HT_V1Packet.TH.temp)/10.0f;
    if(_tempFloat<60){
        MIBLEsensors.at(_slot).MJ_HT_V1.temp = _tempFloat;
        DEBUG_SENSOR_LOG(PSTR("MJ_HT_V1: temp updated"));
    }
    _tempFloat=(float)(MIBLE.buffer.MJ_HT_V1Packet.TH.hum)/10.0f;
    if(_tempFloat<100){
        MIBLEsensors.at(_slot).MJ_HT_V1.hum = _tempFloat;
        DEBUG_SENSOR_LOG(PSTR("MJ_HT_V1: hum updated"));
    }
    DEBUG_SENSOR_LOG(PSTR("MJ_HT_V1 mode:0x0d: U16:  %x Temp U16: %x Hum"), MIBLE.buffer.MJ_HT_V1Packet.TH.temp,  MIBLE.buffer.MJ_HT_V1Packet.TH.hum);
    break;
    case 0x0a:
    if(MIBLE.buffer.MJ_HT_V1Packet.B.battery<101){
        MIBLEsensors.at(_slot).MJ_HT_V1.bat = MIBLE.buffer.MJ_HT_V1Packet.B.battery;
        DEBUG_SENSOR_LOG(PSTR("MJ_HT_V1: bat updated"));
    }
    DEBUG_SENSOR_LOG(PSTR("MJ_HT_V1 mode:0x0a: U8: %x %%"), MIBLE.buffer.MJ_HT_V1Packet.B.battery);
    break;
  }

}

void MIBLE_EVERY_100_MSECOND() { // Every 100mseconds, with many sensors 50ms could make sense
  static uint32_t _purgeCounter = 0;
  if (MIBLE.timer>600){ // Change read mode every n/10 seconds
    if(++_purgeCounter>8){ // happens every 8 x 600 = 4800 seconds
      DEBUG_SENSOR_LOG(PSTR("MIBLE: check for FAKE sensors"));
      MIBLEpurgeFakeSensors();
    }
    DEBUG_SENSOR_LOG(PSTR("MIBLE: Change packet mode after 60 seconds, MIBLE.timer: %u"),MIBLE.timer);
    if (MIBLE.packetMode == 2){
      MIBLEinitBLE(1); // no real ble packets in release mode, otherwise for developing use 0
    }
    else {
      MIBLEinitBLE(MIBLE.packetMode + 1);
    }
  }
  MIBLE.timer++;

  if (!MIBLEreceivePacket()){
    MIBLEhopChannel();
    NRF24radio.startListening();
    return;
  }

  if(MIBLE.buffer.bleAdv.header.uuid==0xfe95){ // XIAOMI-BLE-Packet
    MIBLE_LOG_BUFFER(MIBLE.streamBuffer);
    MIBLE_LOG_BUFFER(MIBLE.lsfrBuffer);
    MIBLE_LOG_BUFFER(MIBLE.buffer.raw);
    DEBUG_SENSOR_LOG(PSTR("MIBLE: Type: %x"), MIBLE.buffer.bleAdv.header.type);
    switch(MIBLE.buffer.bleAdv.header.type){
      case 0x2050:
      DEBUG_SENSOR_LOG(PSTR("MIBLE: MJ_HT_V1 Packet"));
      break;
      case 0x1613:case 0x1614:case 0x1615:
      DEBUG_SENSOR_LOG(PSTR("MIBLE: Flora Packet"));
      break;
      default:
      DEBUG_SENSOR_LOG(PSTR("MIBLE: unknown Packet"));
      break;
    }
  }
  if (MIBLE.packetMode == 1){ // "flora" mode
    MIBLEhandleFloraPacket();
  }
  if (MIBLE.packetMode == 2){ // "MJ_HT_V1" mode
    MIBLEhandleMJ_HT_V1Packet();
    }

    MIBLEhopChannel();
    NRF24radio.startListening();
}

const char HTTP_MIBLE_SERIAL[] PROGMEM =
 "{s}%s" " Address" "{m}%02x:%02x:%02x:%02x:%02x:%02x%{e}";
const char HTTP_BATTERY[] PROGMEM =
 "{s}%s" " Battery" "{m}%u%%{e}";

const char HTTP_MIBLE_FLORA_DATA[] PROGMEM =
  "{s}%s" " Fertility" "{m}%sus/cm{e}";


void MIBLEShow(bool json)
{
  if (json) {
    if (!MIBLEsensors.size()) { return; }

    for (uint32_t i = 0; i < MIBLEsensors.size(); i++) {
      char slave[33];
      switch(MIBLEsensors.at(i).type){
        case 1:
            if(MIBLEsensors.at(i).showedUp < 3){
              DEBUG_SENSOR_LOG(PSTR("MIBLE: sensor not fully registered yet"));
              break;
            }
            sprintf_P(slave,"%s-%02x%02x%02x",MIBLESlaveFlora,MIBLEsensors.at(i).serial[2],MIBLEsensors.at(i).serial[1],MIBLEsensors.at(i).serial[0]);
            char temperature_flora[33];
            dtostrfd(MIBLEsensors.at(i).Flora.temp, Settings.flag2.temperature_resolution, temperature_flora);
            char lux_flora[33];
            dtostrfd((float)MIBLEsensors.at(i).Flora.lux, 0, lux_flora);
            char moisture_flora[33];
            dtostrfd(MIBLEsensors.at(i).Flora.moisture, 0, moisture_flora);
            char fertility_flora[33];
            dtostrfd(MIBLEsensors.at(i).Flora.fertility, 0, fertility_flora);
            ResponseAppend_P(PSTR(",\"%s\":{"),slave);
            if(MIBLEsensors.at(i).Flora.temp!=-1000.0f){ // this is the error code -> no temperature
              ResponseAppend_P(PSTR("\"" D_JSON_TEMPERATURE "\":%s"), temperature_flora);
            }
            if(MIBLEsensors.at(i).Flora.lux!=0xffff){ // this is the error code -> no temperature
              ResponseAppend_P(PSTR(",\"" D_JSON_ILLUMINANCE "\":%s"), lux_flora);
            }
            if(MIBLEsensors.at(i).Flora.moisture!=-1000.0f){ // this is the error code -> no temperature
              ResponseAppend_P(PSTR(",\"" D_JSON_MOISTURE "\":%s"), moisture_flora);
            }
            if(MIBLEsensors.at(i).Flora.fertility!=-1000.0f){ // this is the error code -> no temperature
              ResponseAppend_P(PSTR(",\"Fertility\":%s"), fertility_flora);
            }
            ResponseAppend_P(PSTR("}"));
          break;
        case 2:
          if(MIBLEsensors.at(i).showedUp < 3){
            DEBUG_SENSOR_LOG(PSTR("MIBLE: sensor not fully registered yet"));
            break;
          }
          sprintf_P(slave,"%s-%02x%02x%02x",MIBLESlaveMJ_HT_V1,MIBLEsensors.at(i).serial[2],MIBLEsensors.at(i).serial[1],MIBLEsensors.at(i).serial[0]);
          char temperature[33];
          dtostrfd(MIBLEsensors.at(i).MJ_HT_V1.temp, Settings.flag2.temperature_resolution, temperature);
          char humidity[33];
          dtostrfd(MIBLEsensors.at(i).MJ_HT_V1.hum, 1, humidity);
          ResponseAppend_P(PSTR(",\"%s\":{"),slave);
          if(MIBLEsensors.at(i).MJ_HT_V1.temp!=-1000.0f){ // this is the error code -> no temperature
            ResponseAppend_P(PSTR("\"" D_JSON_TEMPERATURE "\":%s"), temperature);
          }
          if(MIBLEsensors.at(i).MJ_HT_V1.hum!=-1000.0f){ // this is the error code -> no temperature
            ResponseAppend_P(PSTR(",\"" D_JSON_HUMIDITY "\":%s"), humidity);
          }
          if(MIBLEsensors.at(i).MJ_HT_V1.bat!=0xff){ // this is the error code -> no temperature
            ResponseAppend_P(PSTR(",\"Battery\":%u"), MIBLEsensors.at(i).MJ_HT_V1.bat);
          }
          ResponseAppend_P(PSTR("}"));
          break;
      }
    }
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_NRF24, NRF24type, NRF24.chipType);

    if (!MIBLEsensors.size()) { return; }

    for (uint32_t i = 0; i < MIBLEsensors.size(); i++) {
      switch(MIBLEsensors.at(i).type){
        case 1:
          if(MIBLEsensors.at(i).showedUp < 3){
            DEBUG_SENSOR_LOG(PSTR("MIBLE: sensor not fully registered yet"));
            break;
          }
          char temperature_flora[33];
          dtostrfd(MIBLEsensors.at(i).Flora.temp, Settings.flag2.temperature_resolution, temperature_flora);
          char lux_flora[33];
          dtostrfd((float)MIBLEsensors.at(i).Flora.lux, 0, lux_flora);
          char fertility_flora[33];
          dtostrfd(MIBLEsensors.at(i).Flora.fertility, 0, fertility_flora);

          WSContentSend_PD(HTTP_MIBLE_SERIAL, F("Flora "), MIBLEsensors.at(i).serial[5], MIBLEsensors.at(i).serial[4],MIBLEsensors.at(i).serial[3],MIBLEsensors.at(i).serial[2],MIBLEsensors.at(i).serial[1],MIBLEsensors.at(i).serial[0]);
          if(MIBLEsensors.at(i).Flora.temp!=-1000.0f){ // this is the error code -> no temperature
            WSContentSend_PD(HTTP_SNS_TEMP, MIBLESlaveFlora, temperature_flora, TempUnit());
          }
          if(MIBLEsensors.at(i).Flora.lux!=0xffff){ // this is the error code -> no temperature
            WSContentSend_PD(HTTP_SNS_ILLUMINANCE, MIBLESlaveFlora, MIBLEsensors.at(i).Flora.lux);
          }
          if(MIBLEsensors.at(i).Flora.moisture!=-1000.0f){ // this is the error code -> no temperature
            WSContentSend_PD(HTTP_SNS_MOISTURE, MIBLESlaveFlora, MIBLEsensors.at(i).Flora.moisture);
          }
          if(MIBLEsensors.at(i).Flora.fertility!=-1000.0f){ // this is the error code -> no temperature
            WSContentSend_PD(HTTP_MIBLE_FLORA_DATA, MIBLESlaveFlora, fertility_flora);
          }
          break;
        case 2:
          if(MIBLEsensors.at(i).showedUp < 3){
            DEBUG_SENSOR_LOG(PSTR("MIBLE: sensor not fully registered yet"));
            break;
          }
          char temperature[33];
          dtostrfd(MIBLEsensors.at(i).MJ_HT_V1.temp, Settings.flag2.temperature_resolution, temperature);
          char humidity[33];
          dtostrfd(MIBLEsensors.at(i).MJ_HT_V1.hum, 1, humidity);

          WSContentSend_PD(HTTP_MIBLE_SERIAL, MIBLESlaveMJ_HT_V1, MIBLEsensors.at(i).serial[5], MIBLEsensors.at(i).serial[4],MIBLEsensors.at(i).serial[3],MIBLEsensors.at(i).serial[2],MIBLEsensors.at(i).serial[1],MIBLEsensors.at(i).serial[0]);
          if(MIBLEsensors.at(i).MJ_HT_V1.temp!=-1000.0f){
            WSContentSend_PD(HTTP_SNS_TEMP, MIBLESlaveMJ_HT_V1, temperature, TempUnit());
          }
          if(MIBLEsensors.at(i).MJ_HT_V1.hum!=-1.0f){
            WSContentSend_PD(HTTP_SNS_HUM, MIBLESlaveMJ_HT_V1, humidity);
          }
          if(MIBLEsensors.at(i).MJ_HT_V1.bat!=0xff){
            WSContentSend_PD(HTTP_BATTERY, MIBLESlaveMJ_HT_V1, MIBLEsensors.at(i).MJ_HT_V1.bat);
          break;
        }
      }
    }
  }
#endif  // USE_WEBSERVER
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
        MIBLEinitBLE(1);
        AddLog_P2(LOG_LEVEL_INFO,PSTR("MIBLE: started"));
        break;
      case FUNC_EVERY_100_MSECOND:
        MIBLE_EVERY_100_MSECOND();
        break;
      case FUNC_JSON_APPEND:
        MIBLEShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        MIBLEShow(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_MIBLE
#endif  // USE_NRF24
#endif  // USE_SPI

