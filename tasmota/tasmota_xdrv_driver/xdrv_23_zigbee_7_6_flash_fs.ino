/*
  xdrv_23_zigbee_7_6_flash_fs.ino - implement a Flash based read-only triviall file-system

  Copyright (C) 2022  Theo Arends and Stephan Hadinger

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

#ifdef USE_ZIGBEE

#ifdef ESP32
#include <vfs_api.h>
#else
#include "FSImpl.h"
#endif

/********************************************************************
** Subfile implementation
**
** Takes a string point in Flash and turn it to a read-only file
********************************************************************/

class FlashFileImpl;
typedef std::shared_ptr<FlashFileImpl> FlashFileImplPtr;

class FlashFileImpl : public FileImpl {
public:

  FlashFileImpl(const char* str) {
    _buf = str;
    _len = strlen_P(str);
    _seek = 0;
  }

  virtual ~FlashFileImpl() {}

  size_t write(const uint8_t *buf, size_t size) {
    return 0;   // not accepted
  }

  size_t read(uint8_t* buf, size_t size) {
    if (_seek < _len) {
      if (size + _seek > _len) {
        size = _len - _seek;  // always > 0 because of guarding test
      }
      memcpy_P(buf, _buf + _seek, size);
      _seek += size;
      return size;
    }
    return 0;   // abort
  }

  void flush() {
    // do nothing
  }

  bool setBufferSize(size_t size) {
    return true;
  }

  bool seek(uint32_t pos, SeekMode mode) {
    // AddLog(LOG_LEVEL_DEBUG, "ZIP: seek pos=%i mode=%i", pos, mode);
    if (SeekSet == mode) {
      if (pos <= _len) {
        _seek = pos;
        return true;
      }
    } else if (SeekCur == mode) {
      if (_seek + pos <= _len) {
        _seek += pos;
        return true;
      }
    } else if (SeekEnd == mode) {
      _seek = _len;
      return true;
    }
    return false;
  }

  size_t position() const {
    return _seek;
  }

  size_t size() const {
    return _len;
  }

  void close() {
    // do nothing
  }
  time_t getLastWrite() {
    return 0;
  }

  const char* path() const {
    return "";
  }

  #ifdef ESP32
  bool seekDir(long position){
    return false;
    // ignore
  }
  String getNextFileName(void)
  {
    return "";
    // ignore
  }
  String getNextFileName(bool *isDir)
  {
    return "";
    // ignore
    }
  #endif // ESP32

  const char* name() const {
    return "<internal>";
  }

  boolean isDirectory(void) {
    return false;       // no directory allowed
  }

  FileImplPtr openNextFile(const char* mode) {
    return nullptr;     // TODO
  }

  void rewindDirectory(void) {
    // ignore
  }

  operator bool() {
    return true;
  }

  // ESP8266 specific?
  bool truncate(uint32_t size) { return false; }
  const char* fullName() const { return ""; }
  bool isFile() const { return true; }
  bool isDirectory() const { return false; }

protected:
  const char *  _buf;
  size_t        _len;
  uint32_t      _seek;
};

#endif // USE_ZIGBEE
