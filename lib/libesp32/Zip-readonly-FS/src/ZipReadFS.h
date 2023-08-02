
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

  ZipReadFSImpl(FS **fs) : _fs(fs), _alt_fs(nullptr), _alt_fs_prefix(nullptr) {};
  ZipReadFSImpl(FS **fs, const char *alt_fs_prefix, FS **alt_fs) : _fs(fs), _alt_fs(alt_fs), _alt_fs_prefix(alt_fs_prefix) {};
  virtual ~ZipReadFSImpl();

  /* get the FS corresponding to the prefix, typically /sd/ for sdcard */
  FS * getFS(const char *path) const;

  FileImplPtr open(const char* path, const char* mode, const bool create);

  bool exists(const char* path);

  bool rename(const char* pathFrom, const char* pathTo) {
    FS * fs = getFS(pathFrom);
    return fs ? fs->rename(pathFrom, pathTo) : false;
  }
  bool remove(const char* path) {
    FS * fs = getFS(path);
    return fs ? fs->remove(path) : false;
  }
  bool mkdir(const char *path) {
    FS * fs = getFS(path);
    return fs ? fs->mkdir(path) : false;
  }
  bool rmdir(const char *path) {
    FS * fs = getFS(path);
    return fs ? fs->rmdir(path) : false;
  }
  void mountpoint(const char *) {
  };
  const char * mountpoint() {
    return nullptr;
  }

private:
    FS **_fs;
    FS **_alt_fs;
    const char *_alt_fs_prefix;
};

#endif // ESP32

#endif // __ZIP_READ_FS__
