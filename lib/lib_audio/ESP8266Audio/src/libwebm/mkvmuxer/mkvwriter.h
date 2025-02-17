// Copyright (c) 2012 The WebM project authors. All Rights Reserved.
//
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file in the root of the source
// tree. An additional intellectual property rights grant can be found
// in the file PATENTS.  All contributing project authors may
// be found in the AUTHORS file in the root of the source tree.

#ifndef MKVMUXER_MKVWRITER_H_
#define MKVMUXER_MKVWRITER_H_

#ifdef ESP32
#include <stdio.h>

#include <FS.h>
#include "mkvmuxer.h"
#include "mkvmuxertypes.h"

namespace mkvmuxer {

// Default implementation of the IMkvWriter interface on Windows.
class MkvWriter : public IMkvWriter {
 public:
  MkvWriter();
  MkvWriter(File* fp);
  MkvWriter(void* userdata, int(*cb)(void* userdata, const void* buffer, uint32 length));
  virtual ~MkvWriter();

  // IMkvWriter interface
  virtual int64 Position() const;
  virtual int32 Position(int64 position);
  virtual bool Seekable() const;
  virtual int32 Write(const void* buffer, uint32 length);
  virtual void ElementStartNotify(uint64 element_id, int64 position);

  // Creates and opens a file for writing. |filename| is the name of the file
  // to open. This function will overwrite the contents of |filename|. Returns
  // true on success.
  bool Open(const char* filename);

  // Closes an opened file.
  void Close();

 private:
  // File handle to output file.
  File* file_;
  // Callback function to output stream.
  int(*write_cb_)(void* userdata, const void* buffer, uint32 length);
  void* cb_userdata_;
  bool writer_owns_file_;
  bool use_write_cb_;
  uint32_t write_cb_written_ = 0;

  LIBWEBM_DISALLOW_COPY_AND_ASSIGN(MkvWriter);
};

}  // namespace mkvmuxer

#endif // ESP32
#endif  // MKVMUXER_MKVWRITER_H_
