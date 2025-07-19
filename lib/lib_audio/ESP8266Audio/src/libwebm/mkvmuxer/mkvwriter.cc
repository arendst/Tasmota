// Copyright (c) 2012 The WebM project authors. All Rights Reserved.
//
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file in the root of the source
// tree. An additional intellectual property rights grant can be found
// in the file PATENTS.  All contributing project authors may
// be found in the AUTHORS file in the root of the source tree.

#include "mkvwriter.h"
#ifdef ESP32

#include <sys/types.h>

#ifdef _MSC_VER
#include <share.h>  // for _SH_DENYWR
#endif

namespace mkvmuxer {

MkvWriter::MkvWriter() : file_(NULL), writer_owns_file_(true) {}

MkvWriter::MkvWriter(File* fp) : file_(fp), writer_owns_file_(false), use_write_cb_(false) {}

MkvWriter::MkvWriter(void* userdata, int(*cb)(void* userdata, const void* buffer, uint32 length)) : write_cb_(cb), cb_userdata_(userdata), writer_owns_file_(false), use_write_cb_(true) {}

MkvWriter::~MkvWriter() { Close(); }

int32 MkvWriter::Write(const void* buffer, uint32 length) {
  if (!file_ && !write_cb_)
    return -1;

  if (length == 0)
    return 0;

  if (buffer == NULL)
    return -1;

  size_t bytes_written = 0;
  if(!use_write_cb_){
    bytes_written = file_->write((const uint8_t *)buffer, (size_t)length);
  } else {
      bytes_written = write_cb_(cb_userdata_, buffer, length);
      write_cb_written_ += bytes_written;
  }

  return (bytes_written == length) ? 0 : -1;
}

bool MkvWriter::Open(const char* filename) {
  return false;
}

void MkvWriter::Close() {
  if (file_ && writer_owns_file_) {
    file_->close();
  }
  file_ = NULL;
}

int64 MkvWriter::Position() const {
  if (!file_ || use_write_cb_)
    return write_cb_written_;

  return file_->position();
}

int32 MkvWriter::Position(int64 position) {
  if (use_write_cb_ && write_cb_written_ == position)
    return 0;
  if (!file_ || use_write_cb_)
    return -1;

  return file_->seek(position);

// #ifdef _MSC_VER
//   return _fseeki64(file_, position, SEEK_SET);
// #elif defined(_WIN32)
//   return fseeko64(file_, static_cast<off_t>(position), SEEK_SET);
// #elif !(defined(__ANDROID__) && __ANDROID_API__ < 24 && !defined(__LP64__) && \
//         defined(_FILE_OFFSET_BITS) && _FILE_OFFSET_BITS == 64)
//   // POSIX.1 has fseeko and ftello. fseeko and ftello are not available before
//   // Android API level 24. See
//   // https://android.googlesource.com/platform/bionic/+/main/docs/32-bit-abi.md
//   // return fseeko(file_, static_cast<off_t>(position), SEEK_SET);
// #else
//   return fseek(file_, static_cast<long>(position), SEEK_SET);
// #endif
//   return -1;
}

bool MkvWriter::Seekable() const {
  if (!file_ || use_write_cb_)
    return false;

  return true; 
}

void MkvWriter::ElementStartNotify(uint64, int64) {}

}  // namespace mkvmuxer
#endif // ESP32