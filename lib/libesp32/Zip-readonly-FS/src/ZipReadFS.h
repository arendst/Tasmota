
#ifndef __ZIP_READ_FS__
#define __ZIP_READ_FS__

#include <Arduino.h>

#ifdef ESP32

#include <FS.h>
#include <vfs_api.h>
#include <LList.h>

class ZipReadFSImpl;
typedef std::shared_ptr<FSImpl> ZipReadFSImplPtr;


class ZipReadFSImpl : public FSImpl {
public:

  ZipReadFSImpl(FS **fs) : _fs(fs) {};
  virtual ~ZipReadFSImpl();

  FileImplPtr open(const char* path, const char* mode, const bool create);

  bool exists(const char* path);

  bool rename(const char* pathFrom, const char* pathTo) {
    if (*_fs) {
      return (*_fs)->rename(pathFrom, pathTo);
    } else {
      return false;
    }
  }
  bool remove(const char* path) {
    if (*_fs) {
      return (*_fs)->remove(path);
    } else {
      return false;
    }
  }
  bool mkdir(const char *path) {
    if (*_fs) {
      return (*_fs)->mkdir(path);
    } else {
      return false;
    }
  }
  bool rmdir(const char *path) {
    if (*_fs) {
      return (*_fs)->rmdir(path);
    } else {
      return false;
    }
  }
  void mountpoint(const char *) {
  };
  const char * mountpoint() {
    return nullptr;
  }

private:
    FS **_fs;
};

#endif // ESP32

#endif // __ZIP_READ_FS__
