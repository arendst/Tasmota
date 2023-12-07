/*
  xdrv_16_tuyamcu_upgrade.ino - Tuya MCU upgrade support for Tasmota

  Copyright (C) 2023

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

#ifdef USE_LIGHT
#ifdef USE_TUYA_MCU
#ifdef USE_TUYA_MCU_UPGRADE

#include <include/xdrv_16_tuyamcu_upgrade.h>

#include <t_bearssl_hash.h>
#include <memory>

#ifdef ESP8266
extern "C" uint32_t _FS_start;     // ... depending on core-sdk-version
#elif defined ESP32
#ifdef USE_UFILESYS
extern FS *ufsp;
extern char *fileOnly (char *);
#else
#error USE_TUYA_MCU_UPGRADE will only compile with active UFS (define USE_UFILESYS)
#endif
#endif

TuyaUpgBuffer::TuyaUpgBuffer (void) {

}

TuyaUpgBuffer::~TuyaUpgBuffer (void) {
#ifdef ESP32
  if (_fname) {
    delete[] _fname;
    _fname = nullptr;
  }
#endif
  if (_buffer) {
    delete[] _buffer;
    _buffer = nullptr;
  }
  if (_md5_context) {
    delete _md5_context;
    _md5_context = nullptr;
  }
  if (_current_packet) {
    delete[] _current_packet;
    _current_packet = nullptr;
  }
}

bool TuyaUpgBuffer::init (uint32_t len, char* checksum, char* filename) {
  reset();
#ifdef ESP8266
  // prepare to use OTA-partition
  _start = (ESP.getSketchSize() + _flash_sector_size - 1) & (~(_flash_sector_size - 1));
  _end = (uint32_t)&_FS_start - 0x40200000;
  _num_sectors = (_end - _start)/_flash_sector_size;
  _current_address = _start;
  AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: MCU-Upgrade: used size: 0x%lx, start: 0x%lx, end: 0x%lx, num_sectors(dec): %lu"), 
          ESP.getSketchSize(), _start, _end, _num_sectors);
#elif defined ESP32
  //using the ufs
  uint32_t maxMem = (UfsSize() * 1024);
  uint32_t freeMem = (UfsFree() * 1024);
  AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: MCU-Upgrade: ufs size: %d, ufs free: %d, OTA-binary-size: %d"), 
          maxMem, freeMem, len);
  
  char * fname = fileOnly(filename);
  size_t fname_size = strlen(fname);
  _fname = new char[fname_size + 2];
  _fname[0] = '/';
  strncpy(&_fname[1], fname, fname_size);
  _fname[1 + fname_size] = '\0';
  
  if (ufsp->exists(_fname)) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: MCU-Upgrade: OTA file with equal filename exists: %s. It will be deleted now."), _fname);
    if (!ufsp->remove(_fname)) {
      AddLog(LOG_LEVEL_ERROR, PSTR("TYA: MCU-Upgrade: file deletion failed"));
      return false;
    }
  }

  File ota_file = ufsp->open(_fname, "w");
  if (!ota_file) {
    AddLog(LOG_LEVEL_ERROR, PSTR("TYA: MCU-Upgrade: OTA file %s couldn't created"), _fname);
    return false;
  }

  ota_file.close();
#endif
  if (ESP.getFreeHeap() > 2 * _flash_sector_size) {
    _buffer_size = _flash_sector_size;
  } else {
    _buffer_size = 256;
  }
  _buffer = new uint8_t[_buffer_size];
  _buffer_pos = 0;
  _len = len;
  _checksum = checksum;
#ifdef ESP8266
  return _start > 0 && _num_sectors > 0 && _buffer;
#elif defined ESP32
  return freeMem > _len && (ufsp->exists(_fname)) && _buffer;
#endif
}

void TuyaUpgBuffer::reset (void) {
  _start = 0;
  _end = 0;
  _num_sectors = 0;
  _current_address = 0;
  _len = 0;
  _buffer_size = 0;
#ifdef ESP32
  if (_fname) {
    delete[] _fname;
    _fname = nullptr;
  }
#endif
  if (_buffer) {
    delete[] _buffer;
    _buffer = nullptr;
  }
  if (_md5_context) {
    delete _md5_context;
    _md5_context = nullptr;
  }
  _checksum = nullptr;
  _packet_size = 0;
  if (_current_packet) {
    delete[] _current_packet;
    _current_packet = nullptr;
  }
  _current_packet_size = 0;
}

uint32_t TuyaUpgBuffer::writeToFlashOrFile (Stream &data) {
  uint32_t bytesWritten = 0;
  _md5_context = new br_md5_context;
  if (0 < _len && _md5_context) {
    br_md5_init(_md5_context);
    uint32_t toRead = 0;
    while (0 < remaining()) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: MCU-Upgrade: transfering OTA file chunk to internal memory %d/%d"), (_len - remaining()), _len);
      uint8_t retryCount = 1;
      uint32_t bytesToRead = _buffer_size - _buffer_pos;
      if (bytesToRead > remaining()) {
        bytesToRead = remaining();
      }
      toRead = data.readBytes(_buffer + _buffer_pos, bytesToRead);
      while (toRead == 0 && retryCount < MAX_RETRIES) {
        delay(100);
        toRead = data.readBytes(_buffer + _buffer_pos, bytesToRead);
        ++retryCount;
      }
      if (toRead == 0) { // three Timeouts
        ResponseAppend_P(PSTR("readBytes timed out"));
        abort();
        return bytesWritten;
      }
      _buffer_pos += toRead;
      if((_buffer_pos == remaining() || _buffer_pos == _buffer_size) && !writeBuffer()) {
        AddLog(LOG_LEVEL_ERROR, PSTR("TYA: MCU-Upgrade: error transfering OTA file chunk to internal memory, %d bytes written!"), bytesWritten);
        abort();
        return bytesWritten;
      }
      bytesWritten += toRead;
      yield();          // to satisfying the WDT
    }
  }
  AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: MCU-Upgrade: OTA file transfer done, %d bytes written."), bytesWritten);
  return bytesWritten;
}

bool TuyaUpgBuffer::isChecksumOk (void) {
  AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: MCU-Upgrade: verify checksum %s"), _checksum);
  bool checksumOk = true;
  if(_checksum) {
    if (_md5_context && 32 == strlen(_checksum)) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: MCU-Upgrade: length of given checksum is OK"));
      char *toConvert = _checksum;
      uint8_t md5_in[16];
      uint8_t md5_out[16];
      for(uint8_t i = 0; i < 16; ++i, toConvert += 2) {
        char currentValue[3];
        memcpy_P(&currentValue[0], toConvert, 2);
        currentValue[2] = '\0';
        md5_in[i] = strtoul(currentValue, nullptr, 16);
      }

      br_md5_out(_md5_context, md5_out);
      for (uint8_t i = 0; i < 16; ++i) {
        checksumOk = checksumOk && (md5_in[i] == md5_out[i]);
      }
    } else {
      AddLog(LOG_LEVEL_ERROR, PSTR("TYA: MCU-Upgrade: error calculating the checksums!"));
      checksumOk = false;
    }
  }
  AddLog(LOG_LEVEL_DEBUG, PSTR("TYA: MCU-Upgrade: checksum is %s"), checksumOk ? "OK" : "not OK");
  return checksumOk;
}

void TuyaUpgBuffer::resetPosition (void) {
  _current_address = _start;
  readToBuffer();
}

bool TuyaUpgBuffer::hasMoreData (void) {
  return remaining() > 0 || _current_packet_size > 0;
}

uint32_t TuyaUpgBuffer::bytesRead (void) {
  return _current_address - _start;
}

uint32_t TuyaUpgBuffer::getPackageOffset (void) {
  return bytesRead() - _current_packet_size;
}

void TuyaUpgBuffer::setPacketSize (uint16_t value) {
  _packet_size = value;
};

bool TuyaUpgBuffer::readNextPacket (void) {
  uint32_t bytesRead = 0;
  if (_current_packet) {
    delete[] _current_packet;
    _current_packet = nullptr;
  }
  _current_packet_size = _packet_size;
  if (_current_packet_size > remaining()) {
    _current_packet_size = remaining();
  }
  if (0 < _current_packet_size) {
    _current_packet = new uint8_t[_current_packet_size];
    while (bytesRead < _current_packet_size) {
      if (_buffer_pos < _buffer_size) {
        uint32_t bytesToCopy = _current_packet_size - bytesRead;
        if ( bytesToCopy > (_buffer_size - _buffer_pos)) {
          bytesToCopy = _buffer_size - _buffer_pos;
        }
        memcpy_P(_current_packet + bytesRead, _buffer + _buffer_pos, bytesToCopy);
        _current_address += bytesToCopy;
        _buffer_pos += bytesToCopy;
        bytesRead += bytesToCopy;
      } else {
        if (!readToBuffer()) {
          return false;
        }
      }
    }
  }

  return (bytesRead == _current_packet_size);
}

void TuyaUpgBuffer::getCurrentPacket (uint8_t *toBuffer) {
  if (toBuffer && _current_packet) {
    memcpy_P(toBuffer, _current_packet, _current_packet_size);
  }
}

uint16_t TuyaUpgBuffer::getCurrentPacketSize (void) {
  return _current_packet_size;
};

/* private functions */
uint32_t TuyaUpgBuffer::remaining (void) {
  return _len - (_current_address - _start);
}

void TuyaUpgBuffer::abort (void) {
  _current_address = (_start + _len);
}

#ifdef ESP8266
bool TuyaUpgBuffer::eraseSector (void) {
  bool eraseResult = true;
  if (_current_address % _flash_sector_size == 0) {
    eraseResult = ESP.flashEraseSector(_current_address/_flash_sector_size);
  }
  return eraseResult;
}

bool TuyaUpgBuffer::writeBuffer (void) {
  bool writeResult = true;

  if (eraseSector()) {
    writeResult = ESP.flashWrite(_current_address, (uint32_t*) _buffer, _buffer_pos);
  } else { // if erase was unsuccessful
    abort();
    AddLog(LOG_LEVEL_ERROR, PSTR("TYA: MCU-Upgrade: error erasing sector %d in flash!"), (_current_address/_flash_sector_size));
    return false;
  }

  if (!writeResult) {
    abort();
    AddLog(LOG_LEVEL_ERROR, PSTR("TYA: MCU-Upgrade: error writing buffer @ address 0x%lx to flash!"), _current_address);
    return false;
  }
  
  br_md5_update(_md5_context, _buffer, _buffer_pos);

  _current_address += _buffer_pos;
  _buffer_pos = 0;

  return true;
}

bool TuyaUpgBuffer::readToBuffer (void) {
  uint32_t bytesToRead = _buffer_size;
  if (bytesToRead > remaining()) {
    bytesToRead = remaining();
  }
  _buffer_pos = 0;

  return ESP.flashRead(_current_address, (uint32_t*) _buffer, bytesToRead);
}

#elif defined ESP32
bool TuyaUpgBuffer::writeBuffer (void) {
  File ota_file = ufsp->open(_fname, "a");
  if (!ota_file) {
    abort();
    AddLog(LOG_LEVEL_ERROR, PSTR("TYA: MCU-Upgrade: file %s couldn't be opened."), _fname);
    return false;
  }
  
  ota_file.write(_buffer, _buffer_pos);
  ota_file.close();

  br_md5_update(_md5_context, _buffer, _buffer_pos);

  _current_address += _buffer_pos;
  _buffer_pos = 0;

  return true;
}

bool TuyaUpgBuffer::readToBuffer (void) {
  uint32_t bytesToRead = _buffer_size;
  if (bytesToRead > remaining()) {
    bytesToRead = remaining();
  }
  _buffer_pos = 0;

  File ota_file = ufsp->open(_fname, "r");
  if (!ota_file) {
    abort();
    AddLog(LOG_LEVEL_ERROR, PSTR("TYA: MCU-Upgrade: file %s couldn't be opened."), _fname);
    return false;
  }
  
  if (!ota_file.seek(_current_address)) {
    abort();
    AddLog(LOG_LEVEL_ERROR, PSTR("TYA: MCU-Upgrade: file position %d not seekable."), _current_address);
    return false;
  }
  
  uint32_t bytesObtained = ota_file.read(_buffer, bytesToRead);
  if (bytesToRead != bytesObtained) {
    abort();
    AddLog(LOG_LEVEL_ERROR, PSTR("TYA: MCU-Upgrade: file read out error (obtain: %d/expected: %d)."), bytesObtained, bytesToRead);
    return false;
  }
  
  ota_file.close();

  return true;
}
#endif

#endif  // USE_TUYA_MCU_UPGRADE
#endif  // USE_TUYA_MCU
#endif  // USE_LIGHT
