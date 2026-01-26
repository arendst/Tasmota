/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang - Stephan Hadinger
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#ifndef __BE_BYTESLIB_H
#define __BE_BYTESLIB_H

#include "be_object.h"

#define BYTES_DEFAULT_SIZE          28              /* default pre-reserved size for buffer (keep 4 bytes for len/size) */
#define BYTES_OVERHEAD              4               /* bytes overhead to be added when allocating (used to store len and size) */
#define BYTES_HEADROOM              8               /* keep a natural headroom of 8 bytes when resizing */

#define BYTES_SIZE_FIXED            -1              /* if size is -1, then the bytes object cannot be reized */
#define BYTES_SIZE_MAPPED           -2              /* if size is -2, then the bytes object is mapped to a fixed memory region, i.e. cannot be resized */
#define BYTES_SIZE_SOLIDIFIED       -3              /* is size is -3, then the bytes object is solidified and cannot be resized nor modified */

#define BYTES_RESIZE_ERROR          "attribute_error"
#define BYTES_RESIZE_MESSAGE        "bytes object size is fixed and cannot be resized"
#define BYTES_READ_ONLY_MESSAGE     "bytes object is read only"
/* be_raise(vm, BYTES_RESIZE_ERROR, BYTES_RESIZE_MESSAGE); */

typedef struct buf_impl {
  int32_t size;               // size in bytes of the buffer
  int32_t len;                // current size of the data in buffer. Invariant: len <= size
  uint8_t *bufptr;            // the actual data
  int32_t prev_size;          // previous value read from the instance
  int32_t prev_len;           // previous value read from the instance
  uint8_t *prev_bufptr;
  bbool   fixed;              // is size fixed? (actually encoded as negative size)
  bbool   mapped;
  bbool   solidified;
} buf_impl;

size_t be_bytes_tohex(char * out, size_t outsz, const uint8_t * in, size_t insz);

#if BE_USE_PRECOMPILED_OBJECT
#include "../generate/be_const_bytes.h"
#endif

#endif
