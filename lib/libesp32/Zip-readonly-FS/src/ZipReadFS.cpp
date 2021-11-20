/*
  ZipReadFS.cpp - FS overlay to read uncompressed ZIP files

  Copyright (C) 2021  Stephan Hadinger

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

#ifdef ESP32

#include "ZipReadFS.h"

extern FS *zip_ufsp;

#define USE_TASMOTA_LOG

#ifdef USE_TASMOTA_LOG
extern void AddLog(uint32_t loglevel, PGM_P formatP, ...);
enum LoggingLevels {LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE};
#endif

/********************************************************************
** Convert DOS time to time_t
** from: https://opensource.apple.com/source/gcc3/gcc3-1161/fastjar/dostime.c
********************************************************************/
time_t dos2unixtime(uint32_t dostime)
     /* Return the Unix time_t value (GMT/UTC time) for the DOS format (local)
      * time dostime, where dostime is a four byte value (date in most
      * significant word, time in least significant word), see dostime() 
      * function.
      */
{
  struct tm *t;         /* argument for mktime() */
  time_t clock = time(NULL);

  t = localtime(&clock);
  t->tm_isdst = -1;     /* let mktime() determine if DST is in effect */
  /* Convert DOS time to UNIX time_t format */
  t->tm_sec  = (((int)dostime) <<  1) & 0x3e;
  t->tm_min  = (((int)dostime) >>  5) & 0x3f;
  t->tm_hour = (((int)dostime) >> 11) & 0x1f;
  t->tm_mday = (int)(dostime >> 16) & 0x1f;
  t->tm_mon  = ((int)(dostime >> 21) & 0x0f) - 1;
  t->tm_year = ((int)(dostime >> 25) & 0x7f) + 80;

  return mktime(t);
}

/********************************************************************
** Zip file parser
** 
********************************************************************/
template <typename T> class LList;

struct ZipHeader {
  uint16_t      padding;    // need to offset by 16 bites so that 32 bits below are aligned to 4 bytes boundaries
  uint16_t      signature1;
  uint16_t      signature2;
  uint16_t      version;
  uint16_t      gen_purpose_flags;
  uint16_t      compression;
  uint16_t      last_mod_time;
  uint16_t      last_mod_date;
  uint32_t      crc32;
  uint32_t      size_compressed;
  uint32_t      size_uncompressed;
  uint16_t      filename_size;
  uint16_t      extra_field_size;
};

class ZipEntry {
public:
  ZipEntry() :
    file_name(), file_start(0), file_len(0), last_mod(0)
    {};
  // ZipEntry(const char * fname, uint32_t start, uint32_t len) :
  //   file_name(fname), file_start(start), file_len(len)
  //   {};

  String file_name;       // name of the file as used by Berry, with all directories removed
  uint32_t file_start;    // offset in bytes where this file starts in the archive
  uint32_t file_len;      // length in bytes of the file
  time_t   last_mod;
};

class ZipArchive {
public:
  ZipArchive(File * _f) :
    f(_f), entries()
    {};
  ~ZipArchive(void) {
    f->close();      // TODO
  }

  bool parse(void);

  File * f;
  LList<ZipEntry> entries;
};

class ZipEntryFileImpl : public FileImpl {
public:
  ZipEntryFileImpl(File * f) : zip(f) { }

protected:
  ZipArchive zip;
};

/********************************************************************
** Neutral file overlay
** 
********************************************************************/

class ZipReadFileImpl;
typedef std::shared_ptr<FileImpl> ZipReadFileImplPtr;

class ZipReadFileImpl : public FileImpl {
public:
  ZipReadFileImpl(File f) { _f = f; }
  virtual ~ZipReadFileImpl() {}
  size_t write(const uint8_t *buf, size_t size) {
    return _f.write(buf, size);
  }
  size_t read(uint8_t* buf, size_t size) {
    return _f.read(buf, size);
  }
  void flush() {
    _f.flush();
  }
  bool seek(uint32_t pos, SeekMode mode) {
    return _f.seek(pos, mode);
  }
  size_t position() const {
    return _f.position();
  }
  size_t size() const {
    return _f.size();
  }
  void close() {
    _f.close();
  }
  time_t getLastWrite() {
    return _f.getLastWrite();
  }
  const char* path() const {
    return _f.path();
  }
  const char* name() const {
    return _f.name();
  }
  boolean isDirectory(void) {
    return _f.isDirectory();
  }
  FileImplPtr openNextFile(const char* mode) {
    return nullptr; // TODO
  }
  void rewindDirectory(void) {
    return _f.rewindDirectory();
  }
  operator bool() {
    return (bool) _f;
  }

protected:
  File _f;
};

/********************************************************************
** Subfile implementation
** 
** Takes a `File` object of the ZIP archive
** First byte in archive and len
********************************************************************/

class ZipItemImpl;
typedef std::shared_ptr<ZipItemImpl> ZipItemImplPtr;

class ZipItemImpl : public FileImpl {
public:

  ZipItemImpl(File f, uint32_t first_byte, uint32_t len, time_t last_mod) {
    _f = f;
    _first_byte = first_byte;
    _len = len;
    _seek = 0;
    _last_mod = last_mod;
  }

  virtual ~ZipItemImpl() {}

  size_t write(const uint8_t *buf, size_t size) {
    return 0;   // not accepted
  }

  size_t read(uint8_t* buf, size_t size) {
    // AddLog(LOG_LEVEL_DEBUG, "ZIP: read bytes=%i seek=%i len=%i", size, _seek, _len);
    if (_seek < _len) {
      if (size + _seek > _len) {
        size = _len - _seek;  // always > 0 because of guarding test
      }
      bool bret = _f.seek(_first_byte + _seek, SeekSet);
      // AddLog(LOG_LEVEL_DEBUG, "ZIP: seek_ret ret=%i zip_seek=%i", bret, _first_byte + _seek);
      if (bret) {
        size_t ret = _f.read(buf, size);
        // AddLog(LOG_LEVEL_DEBUG, "ZIP: read done ret=%i zip_seek=%i", ret, size);
        _seek += ret;
        if (_seek > _len) { _seek = _len; }
        return ret;
      }
    }
    return 0;   // abort
  }

  void flush() {
    // do nothing
  }

  bool seek(uint32_t pos, SeekMode mode) {
    AddLog(LOG_LEVEL_DEBUG, "ZIP: seek pos=%i mode=%i", pos, mode);
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
    AddLog(LOG_LEVEL_DEBUG, "ZIP: position return=%i", _seek);
    return _seek;
  }

  size_t size() const {
    AddLog(LOG_LEVEL_DEBUG, "ZIP: size return=%i", _len);
    return _len;
  }

  void close() {
    // do nothing
  }
  time_t getLastWrite() {
    return _last_mod;
  }

  const char* path() const {
    return _f.path();   // TODO
  }

  const char* name() const {
    return _f.name();   // TODO
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

protected:
  File _f;
  uint32_t  _first_byte;
  uint32_t  _len;
  uint32_t  _seek;
  time_t    _last_mod;
};

/********************************************************************
** Zip file parser
** Implementation
********************************************************************/

// parse the Zip archive to extract all entries
// returns true if ok
bool ZipArchive::parse(void) {
  ZipHeader header;
  f->seek(0);    // start of file
  int32_t offset = 0;
  const size_t zip_header_size = sizeof(header) - sizeof(header.padding);

  while (1) {
    f->seek(offset);
    int32_t bytes_read = f->read(sizeof(header.padding) + (uint8_t*) &header, zip_header_size);
    if (bytes_read != zip_header_size) {
      break;
    }
    // AddLog(LOG_LEVEL_DEBUG, "ZIG: header version=%i flags=%p compress=%i mod_time=%i mod_date=%i size=%i-%i fnamesize=%i",
    //   header.version, header.gen_purpose_flags, header.compression, header.last_mod_time, header.last_mod_date,
    //   header.size_compressed, header.size_uncompressed, header.filename_size);
    // Check signature
    if (header.signature1 != 0x4B50) {
      AddLog(LOG_LEVEL_INFO, "ZIP: invalid zip signature");
      return false;
    }
    if (header.signature2 != 0x0403) {
      AddLog(LOG_LEVEL_DEBUG, "ZIP: end of file section");
      break;
    }
    // Check no extra field
    if (header.gen_purpose_flags != 0x0000) {
      AddLog(LOG_LEVEL_INFO, "ZIP: invalid general purpose flags 0x%04X", header.gen_purpose_flags);
      return false;
    }
    // Check no compression
    if (header.compression != 0x0000) {
      AddLog(LOG_LEVEL_INFO, "ZIP: compressed files unsupported 0x%04X", header.compression);
      return false;
    }
    // Check size is the same for compressed and uncompressed
    if (header.size_compressed != header.size_uncompressed) {
      AddLog(LOG_LEVEL_INFO, "ZIP: compressed size differs from uncompressed %i - %i", header.size_compressed, header.size_uncompressed);
      return false;
    }
    // Check file name size
    if (header.filename_size > 64) {
      AddLog(LOG_LEVEL_INFO, "ZIP: entry filename size too long %i", header.filename_size);
      return false;
    }

    // read full filename
    char fname[header.filename_size + 1];
    if (f->read((uint8_t*) &fname[0], header.filename_size) != header.filename_size) {
      return false;
    }
    fname[header.filename_size] = 0;  // add NULL termination

    // Remove any directory names, and keep only what's after the last `/``
    char * fname_suffix;
    char * saveptr;
    fname_suffix = strtok_r(&fname[0], "#", &saveptr);
    char * res = fname_suffix;
    while (res) {
      res = strtok_r(nullptr, "#", &saveptr);
      if (res) { fname_suffix = res; }
    }
    offset += zip_header_size + header.filename_size + header.extra_field_size;

    ZipEntry & entry = entries.addToLast();
    entry.file_name = fname_suffix;
    entry.file_start = offset;
    entry.file_len = header.size_uncompressed;
    entry.last_mod = dos2unixtime((header.last_mod_date << 16) | header.last_mod_time);
    offset += header.size_uncompressed;

    AddLog(LOG_LEVEL_DEBUG_MORE, "ZIP: found file '%s' (%i bytes - offset %i) - next entry %i", &fname[0], header.size_uncompressed, entry.file_start, offset);
  }

  return true;
}


/********************************************************************
** Encapsulation of FS and File to piggyback on Arduino
** 
********************************************************************/

FileImplPtr ZipReadFSImpl::open(const char* path, const char* mode, const bool create) {
  if (*_fs == nullptr) { return nullptr; }

  if (strchr(path, '#')) {
    // we don't support any other mode than "r" and no-create
    if (strcmp(mode, "r") != 0 || create)  {
      AddLog(LOG_LEVEL_INFO, "ZIP: writing to zip is not supported");
      return ZipReadFileImplPtr();    // return an error
    }
    // treat as a ZIP archive
    char sub_path[strlen(path)+1];
    strcpy(sub_path, path);

    // extract the suffix
    char *tok;
    char *prefix = strtok_r(sub_path, "#", &tok);
    char *suffix = strtok_r(NULL, "", &tok);
    // if suffix starts with '/', skip the first char
    if (*suffix == '/') { suffix++; }
    AddLog(LOG_LEVEL_DEBUG, "ZIP: prefix=%s suffix=%s", prefix, suffix);
    // parse ZIP archive
    File zipfile = (*_fs)->open(prefix, "r", false);
    if ((bool)zipfile) {
      // we could read the file
      ZipArchive zip_archive = ZipArchive(&zipfile);
      zip_archive.parse();

      for (auto & entry : zip_archive.entries) {
        if (entry.file_name.equals(suffix)) {
          // found
          AddLog(LOG_LEVEL_DEBUG, "ZIP: file '%s' in archive (start=%i - len=%i - last_mod=%i)", suffix, entry.file_start, entry.file_len, entry.last_mod);
          return ZipItemImplPtr(new ZipItemImpl((*_fs)->open(prefix, "r", false), entry.file_start, entry.file_len, entry.last_mod));
        }
      }
      return ZipReadFileImplPtr();    // return an error
    } else {
      AddLog(LOG_LEVEL_INFO, "ZIP: could not open '%s'", prefix);
      return ZipReadFileImplPtr();    // return an error
    }
  } else {
    // simple file, do nothing
    return ZipReadFileImplPtr(new ZipReadFileImpl((*_fs)->open(path, mode, create)));
  }
}

bool ZipReadFSImpl::exists(const char* path) {
  if (*_fs == nullptr) { return false; }

  if (strchr(path, '#')) {
    // treat as a ZIP archive
    char sub_path[strlen(path)+1];
    strcpy(sub_path, path);

    // extract the suffix
    char *tok;
    char *prefix = strtok_r(sub_path, "#", &tok);
    char *suffix = strtok_r(NULL, "", &tok);
    // parse ZIP archive
    File zipfile = (*_fs)->open(prefix, "r", false);
    if ((bool)zipfile) {
      // we could read the file
      ZipArchive zip_archive = ZipArchive(&zipfile);
      zip_archive.parse();

      for (auto & entry : zip_archive.entries) {
        if (entry.file_name.equals(suffix)) {
          return true;
        }
      }
    }
    return false;
  } else {
    // simple file, do nothing
    return (*_fs)->exists(path);
  }
}

ZipReadFSImpl::~ZipReadFSImpl() {};

#endif // ESP32