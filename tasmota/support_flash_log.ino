/*
  support_flash_log.ino - log to flash support for Sonoff-Tasmota

  Copyright (C) 2021  Theo Arends & Christian Baars

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
  Version Date      Action    Description
  --------------------------------------------------------------------------------------------


  ---
  1.0.0.0 20190923  started   - further development by Christian Baars  - https://github.com/Staars/Sonoff-Tasmota
                    forked    - from arendst/tasmota                    - https://github.com/arendst/Sonoff-Tasmota
                    base      - code base from arendst and              - written from scratch

*/

/********************************************************************************************\
| * Generic helper class to log arbitrary data to the OTA-partition
| * Working principle: Add preferrable small chunks of data to the sector buffer, which will
| *                    be written to FLASH when full automatically. The next sector will be
| *                    erased and is the anchor point for downloading and state configuration
| *                    after reboot.
\*********************************************************************************************/

#ifdef USE_FLOG
#ifdef ESP8266

class FLOG

#define MAGIC_WORD_FL 0x464c //F, L

{

struct header_t{
    uint16_t  magic_word; // FL
    uint16_t  padding; // leave something for the future
    uint32_t  physical_start_sector:10; //first used sector of the current FLOG
    uint32_t  number:10; // number of this sector, starting with 0 for the first sector
    uint32_t  buf_pointer:12; //internal pointer to the next free position in the buffer = first empty byte when reading
    }; // should be 4-byte-aligned

private:
void _readSector(uint8_t one_sector);
void _eraseSector(uint8_t one_sector);
void _writeSector(uint8_t one_sector);
void _clearBuffer(void);
void _searchSaves(void);
void _findFirstErasedSector(void);
void _showBuffer(void);
void _initBuffer(void);
void _saveBufferToSector(void);
header_t _saved_header;

public:
  uint32_t size;                // size of OTA-partition
  uint32_t start;               // start position of OTA-partition in bytes
  uint32_t end;                 // end position of OTA-partition in bytes
  uint16_t num_sectors;         // calculated number of sectors with a size of 4096 bytes

  uint16_t first_erased_sector; // this will be our new start
  uint16_t current_sector;      // always point to next sector, where data from the buffer will be written to

  uint16_t bytes_left;    // byte per buffer (of sector size 4096 bytes - 8 byte header size)
  uint16_t sectors_left;  // number of saved sectors for download

  uint8_t  mode = 0;      // 0 - write once on all sectors, then stop, 1 - write infinitely through the sectors
  bool  found_saved_data = false;   // possible saved data has been found
  bool  ready = false;              // the FLOG is initialized
  bool  running_download = false;   // a download operation is running
  bool  recording = false;          // ready for recording

  union sector_t{
  uint32_t dword_buffer[FLASH_SECTOR_SIZE/4];
  uint8_t byte_buffer[FLASH_SECTOR_SIZE];
  header_t header; // should be 4-byte-aligned
  } sector; // the global buffer of 4096 bytes, used for reading and writing

  void init(void);
  void addToBuffer(uint8_t src[], uint32_t size);
  void startRecording(bool append);
  void stopRecording(void);

  typedef void (*CallbackNoArgs) ();                      // simple typedef for a callback
  typedef void (*CallbackWithArgs) (uint8_t *_record);    // typedef for a callback with one argument

  void startDownload(size_t size, CallbackNoArgs sendHeader, CallbackWithArgs sendRecord, CallbackNoArgs sendFooter);
};

extern "C" uint32_t _SPIFFS_start; // we make shure later, that only one of the two is really used ...
extern "C" uint32_t _FS_start;     // ... depending on core-sdk-version

/**
 * @brief Will examine the start and end of the OTA-partition. Then the sector size will be computed, saved data should be found and the initial state will be configured.
 */
void FLOG::init(void)
{
DEBUG_SENSOR_LOG(PSTR("FLOG: init ..."));
size = ESP.getSketchSize();
// round one sector up
start = (size + FLASH_SECTOR_SIZE - 1) & (~(FLASH_SECTOR_SIZE - 1));
end = (uint32_t)&_FS_start - 0x40200000;
num_sectors = (end - start)/FLASH_SECTOR_SIZE;
DEBUG_SENSOR_LOG(PSTR("FLOG: size: 0x%lx, start: 0x%lx, end: 0x%lx, num_sectors(dec): %lu"), size, start, end, num_sectors );
_findFirstErasedSector();
if(first_erased_sector == 0xffff){
    _eraseSector(0);
    first_erased_sector = 0; // start with sector 0, could be first run or after crash
}
_searchSaves();
_initBuffer();
ready = true;
}

/********************************************************************************************\
| *
| * private helper functions
| *
\*********************************************************************************************/

/**
 * @brief Read a sector into the global buffer
 *
 * @param one_sector as an uint8_t
 */
void FLOG::_readSector(uint8_t one_sector){
  DEBUG_SENSOR_LOG(PSTR("FLOG: read sector number: %u" ), one_sector);
  ESP.flashRead(start+(one_sector * FLASH_SECTOR_SIZE),(uint32_t *)&sector.dword_buffer, FLASH_SECTOR_SIZE);
}
/**
 * @brief Erase the given sector og the OTA-partition
 *
 * @param one_sector as an uint8_t
 */
void FLOG::_eraseSector(uint8_t one_sector){ // Erase sector of FLOG/OTA
  DEBUG_SENSOR_LOG(PSTR("FLOG: erasing sector number: %u" ), one_sector);
  ESP.flashEraseSector((start/FLASH_SECTOR_SIZE)+one_sector);
}
/**
 * @brief Write the global buffer to the given sector
 *
 * @param one_sector as an uint8_t
 */
void FLOG::_writeSector(uint8_t one_sector){ // Write sector of FLOG/OTA
  DEBUG_SENSOR_LOG(PSTR("FLOG: write buffer to sector number: %u" ), one_sector);
  ESP.flashWrite(start+(one_sector * FLASH_SECTOR_SIZE),(uint32_t *)&sector.dword_buffer, FLASH_SECTOR_SIZE);
}
/**
 * @brief Clear the global buffer, but leave the header intact
 *
 */
void FLOG::_clearBuffer(){ //not the header
  for (uint32_t i = sizeof(sector.header)/4; i<(sizeof(sector.dword_buffer)/4); i++){
      sector.dword_buffer[i] = 0;
  }
  sector.header.buf_pointer = sizeof(sector.header);
  // _showBuffer();
}
/**
 * @brief Write global buffer to FLASH and set the current sector to the next valid position, maybe to 0
 *
 */
void FLOG::_saveBufferToSector(){ // save buffer to already erased(!) sector, erase next sector, clear buffer, increment number
  DEBUG_SENSOR_LOG(PSTR("FLOG: write buffer to current sector: %u" ),current_sector);
  _writeSector(current_sector);
  if(current_sector == num_sectors){ // 1 MB means ~110 sectors in OTA-partition, if we reach this, start a again
    current_sector = 0;
  }
  else{
    current_sector++;
  }
  _eraseSector(current_sector); // we always erase the next sector, to find out were we are after restart
  _clearBuffer();
  sector.header.number++;
  DEBUG_SENSOR_LOG(PSTR("FLOG: new sector header number: %u" ),sector.header.number);
}

/**
 * @brief Typically after restart find the first erased sector as a starting point for further operations
 *
 */
void FLOG::_findFirstErasedSector(){
  for (uint32_t i = 0; i<num_sectors; i++){
    bool success = true;
    DEBUG_SENSOR_LOG(PSTR("FLOG: read sector: %u"), i);
    _readSector(i);
    for (uint32_t j = 0; j<(sizeof(sector.dword_buffer)/4); j++){
      if(sector.dword_buffer[j]!=0xffffffff){
        // DEBUG_SENSOR_LOG(PSTR("FLOG: buffer_dword: %u"), sector.dword_buffer[j]);
        success = false;
      }
    }
    if(success){
      first_erased_sector = i; // save this for the whole next write operation
      sector.header.physical_start_sector = i; // save to header for every sector
      current_sector = i;  // this is our actual sector to write to
      DEBUG_SENSOR_LOG(PSTR("FLOG: first erased sector: %u, now init ..."), first_erased_sector);
      return;
    }
  }
  DEBUG_SENSOR_LOG(PSTR("FLOG: no erased sector found"));
  first_erased_sector = 0xffff; // this will not happen unless we have 256 MByte FLASH
}
/**
 * @brief Look at the sector before the first erased sector to check, if there could be saved data
 *
 */
void FLOG::_searchSaves(void){
  //check if old Data is present
  found_saved_data = false;
  uint32_t s;
  if(first_erased_sector==0){
    DEBUG_SENSOR_LOG(PSTR("FLOG: sector 0 was erased before, examine sector: %u"), num_sectors);
    s = num_sectors; //count back to the highest possible sector
  }
  else{
    s = first_erased_sector-1;
    DEBUG_SENSOR_LOG(PSTR("FLOG: examine sector: %u"), s);
  }
  _readSector(s); //read the sector before the first erased sector
  if(sector.header.magic_word!=MAGIC_WORD_FL){
    DEBUG_SENSOR_LOG(PSTR("FLOG: wrong magic number, no saved data found"));
    return;
  }
  sectors_left = sector.header.number + 1; // this might be wrong, but this less important
  _saved_header = sector.header; // back this up for appending mode
  s = sector.header.physical_start_sector;
  DEBUG_SENSOR_LOG(PSTR("FLOG: will check pysical start sector: %u"), s);
  _readSector(s); //read the physical_start_sector
  _showBuffer();
  if(sector.header.magic_word!=MAGIC_WORD_FL){ //F, L
    DEBUG_SENSOR_LOG(PSTR("FLOG: wrong magic number, no saved data found"));
    sectors_left = 0;
    return;
  }
  if(sector.header.number==0){ //physical_start_sector should have number 0
    DEBUG_SENSOR_LOG(PSTR("FLOG: possible saved data found"));
    found_saved_data = true; // TODO: this is only a very rough check and should be completed later
  }
  else{
    DEBUG_SENSOR_LOG(PSTR("FLOG: number: %u should be 0"), sector.header.number);
    sectors_left = 0;
  }
}
/**
 * @brief Start with a new buffer to be able to start a write session
 *
 */
void FLOG::_initBuffer(void){
  if(!found_saved_data){ // we must re-init this, because the buffer is in an undefined state
    sector.header.physical_start_sector = (uint16_t)first_erased_sector;
  }
  DEBUG_SENSOR_LOG(PSTR("FLOG: init header"));
  sector.header.magic_word = MAGIC_WORD_FL; //F, L
  sector.header.number = 0;
  sector.header.buf_pointer = (uint16_t)sizeof(sector.header);
  current_sector = first_erased_sector;
  ready = true;
  _clearBuffer();
}
/**
 * @brief - a pure debug function
 *
 */
void FLOG::_showBuffer(void){
  DEBUG_SENSOR_LOG(PSTR("FLOG: Header: %c %c"), sector.byte_buffer[0],sector.byte_buffer[1]);
  DEBUG_SENSOR_LOG(PSTR("FLOG: V_Start_sector: %u, sector number: %u, pointer: %u "), sector.header.physical_start_sector, sector.header.number, sector.header.buf_pointer);
  uint32_t j = 0;
  for (uint32_t i = sector.header.buf_pointer-16; i<(sizeof(sector.byte_buffer)); i+=8){
      // DEBUG_SENSOR_LOG(PSTR("FLOG: buffer: %c %c %c %c %c %c %c %c  "), sector.byte_buffer[i], sector.byte_buffer[i+1], sector.byte_buffer[i+2], sector.byte_buffer[i+3], sector.byte_buffer[i+4], sector.byte_buffer[i+5], sector.byte_buffer[i+6], sector.byte_buffer[i+7]);
      DEBUG_SENSOR_LOG(PSTR("FLOG: buffer: %u %u %u %u %u %u %u %u  "), sector.byte_buffer[i], sector.byte_buffer[i+1], sector.byte_buffer[i+2], sector.byte_buffer[i+3], sector.byte_buffer[i+4], sector.byte_buffer[i+5], sector.byte_buffer[i+6], sector.byte_buffer[i+7]);
      j++;
      if(j>3){
        break;
      }
  }
}

/**
 * @brief pass a data entry/record as uint8_t array with its size
 *
 * @param src uint8_t array
 * @param size uint32_t size of the array
 */
void FLOG::addToBuffer(uint8_t src[], uint32_t size){
    if(mode == 0){
      if(sector.header.number == num_sectors && !ready){
        return; // we ignore additional calls and are done, TODO: maybe use meaningful return values
      }
    }
  if((FLASH_SECTOR_SIZE-sector.header.buf_pointer-sizeof(sector.header))>size){
    // DEBUG_SENSOR_LOG(PSTR("FLOG: enough space left in buffer: %u"), FLASH_SECTOR_SIZE - sector.header.buf_pointer - sizeof(sector.header));
    // DEBUG_SENSOR_LOG(PSTR("FLOG: current buf_pointer: %u, size of added: %u"), sector.header.buf_pointer, size);

    memcpy(sector.byte_buffer + sector.header.buf_pointer, src, size);
    sector.header.buf_pointer+=size; // this is the next free spot
    // DEBUG_SENSOR_LOG(PSTR("FLOG: current buf_pointer: %u"), sector.header.buf_pointer);
  }
  else{
    DEBUG_SENSOR_LOG(PSTR("FLOG: save buffer to flash sector: %u"), current_sector);
    DEBUG_SENSOR_LOG(PSTR("FLOG: current buf_pointer: %u"), sector.header.buf_pointer);
    _saveBufferToSector();
    sectors_left++;
    // but now save the data to the fresh buffer
    if((FLASH_SECTOR_SIZE-sector.header.buf_pointer-sizeof(sector.header))>size){
      memcpy(sector.byte_buffer + sector.header.buf_pointer, src, size);
      sector.header.buf_pointer+=size; // this is the next free spot
    }
  }
}

/**
 * @brief shows that it is ready to accept recording
 *
 * @param append - if true append to current log, else start a new log
 */
void FLOG::startRecording(bool append){
    if(recording){
        DEBUG_SENSOR_LOG(PSTR("FLOG: already recording"));
        return;
    }
    recording = true;
    DEBUG_SENSOR_LOG(PSTR("FLOG: start recording"));
    _initBuffer();
    if(!found_saved_data) {
      append = false; // nothing to append to, we silently start a new log
    }
    if(append){
        sector.header.number = _saved_header.number+1; // continue with the next number
        sector.header.physical_start_sector = _saved_header.physical_start_sector; // keep the old start sector
    }
    else{ //new log, old data is lost
        sector.header.physical_start_sector = (uint16_t)first_erased_sector;
        found_saved_data = false;
        sectors_left = 0;
    }
  }

/**
 * @brief stop recording including saving current buffer to FLASH
 *
 */
void FLOG::stopRecording(void){
      _saveBufferToSector();
      _findFirstErasedSector();
      _searchSaves();
      _initBuffer();
      recording = false;
      found_saved_data = true;
  }

/**
 * @brief Will start a downloads, needs the correct implementation of 3 callback functions
 *
 * @param size:         size of the data entry/record in bytes, i.e. sizeof(myStruct)
 * @param sendHeader:   should implement at least something like:
 * @example             Webserver->setContentLength(CONTENT_LENGTH_UNKNOWN); // This is very likely unknown!!
 *                      Webserver->sendHeader(F("Content-Disposition"), F("attachment; filename=myfile.txt"));
 * @param sendRecord:   will receive the memory address as "uint8_t* addr" and should consume the current entry/record
 * @example             myStruct_t *entry = (myStruct_t*)addr;
 *                      Then make useful Strings and send it, i.e.: Webserver->sendContent_P(myString);
 * @param sendFooter:   finish the download, should implement at least:
 * @example             Webserver->sendContent("");
 */
  void FLOG::startDownload(size_t size, CallbackNoArgs sendHeader, CallbackWithArgs sendRecord, CallbackNoArgs sendFooter){

  _readSector(sector.header.physical_start_sector);
  uint32_t next_sector = sector.header.physical_start_sector;
  bytes_left = sector.header.buf_pointer - sizeof(sector.header);
  DEBUG_SENSOR_LOG(PSTR("FLOG: create file for download, will process %u bytes"), bytes_left);
  running_download = true;
  // Callback 1: Create the header incl. file name, content length (probably unknown!!) and additional header stuff
  sendHeader();

  while(sectors_left){
    DEBUG_SENSOR_LOG(PSTR("FLOG: next sector: %u"), next_sector);
    //initially we have the first sector already loaded, so we do it at the bottom
    uint32_t k = sizeof(sector.header);
    while(bytes_left){
      // DEBUG_SENSOR_LOG(PSTR("FLOG: DL %u %u"), Flog->sector.byte_buffer[k],Flog->sector.byte_buffer[k+1]);
      uint8_t *_record_start = (uint8_t*)&sector.byte_buffer[k]; // this is basically the start address of the current record/entry of the Log
      // Callback 2: send the pointer for consuming the next record/entry and doing something useful to create a file
      sendRecord(_record_start);
      if(k%128 == 0){  // give control to the system every x iteration, TODO: This will fail, when record/entry-size is not 8
        // DEBUG_SENSOR_LOG(PSTR("FLOG: now loop(), %u bytes left"), Flog->bytes_left);
        OsWatchLoop();
        delay(TasmotaGlobal.sleep);
        }
      k+=size;
      if(bytes_left>7){
        bytes_left-=size;
      }
      else{
        bytes_left = 0;
        DEBUG_SENSOR_LOG(PSTR("FLOG: Flog->bytes_left not dividable by 8 ??????"));
        }
      }
    next_sector++;
    if(next_sector>num_sectors){
      next_sector = 0;
    }
    sectors_left--;
    _readSector(next_sector);
    bytes_left = sector.header.buf_pointer - sizeof(sector.header);
    OsWatchLoop();
    delay(TasmotaGlobal.sleep);
  }
  running_download = false;
  // Callback 3: create a footer or simply finish the download with an empty payload
  sendFooter();
  // refresh settings for another download
  _searchSaves();
  _initBuffer();
  }

 #endif  // ESP8266
 #endif  // USE_FLOG