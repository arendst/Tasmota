/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang - Stephan Hadinger
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#include "be_object.h"
#include "be_string.h"
#include "be_strlib.h"
#include "be_list.h"
#include "be_func.h"
#include "be_exec.h"
#include "be_vm.h"
#include "be_mem.h"
#include "be_constobj.h"
#include <string.h>
#include <ctype.h>
#include "be_byteslib.h"

/********************************************************************
** Base64 lib from https://github.com/Densaugeo/base64_arduino
**
********************************************************************/

/* binary_to_base64:
 *   Description:
 *     Converts a single byte from a binary value to the corresponding base64 character
 *   Parameters:
 *     v - Byte to convert
 *   Returns:
 *     ascii code of base64 character. If byte is >= 64, then there is not corresponding base64 character
 *     and 255 is returned
 */
static unsigned char binary_to_base64(unsigned char v);

/* base64_to_binary:
 *   Description:
 *     Converts a single byte from a base64 character to the corresponding binary value
 *   Parameters:
 *     c - Base64 character (as ascii code)
 *   Returns:
 *     6-bit binary value
 */
static unsigned char base64_to_binary(unsigned char c);

/* encode_base64_length:
 *   Description:
 *     Calculates length of base64 string needed for a given number of binary bytes
 *   Parameters:
 *     input_length - Amount of binary data in bytes
 *   Returns:
 *     Number of base64 characters needed to encode input_length bytes of binary data
 */
static unsigned int encode_base64_length(unsigned int input_length);

/* decode_base64_length:
 *   Description:
 *     Calculates number of bytes of binary data in a base64 string
 *   Parameters:
 *     input - Base64-encoded null-terminated string
 *   Returns:
 *     Number of bytes of binary data in input
 */
static unsigned int decode_base64_length(unsigned char input[]);

/* encode_base64:
 *   Description:
 *     Converts an array of bytes to a base64 null-terminated string
 *   Parameters:
 *     input - Pointer to input data
 *     input_length - Number of bytes to read from input pointer
 *     output - Pointer to output string. Null terminator will be added automatically
 *   Returns:
 *     Length of encoded string in bytes (not including null terminator)
 */
static unsigned int encode_base64(unsigned char input[], unsigned int input_length, unsigned char output[]);

/* decode_base64:
 *   Description:
 *     Converts a base64 null-terminated string to an array of bytes
 *   Parameters:
 *     input - Pointer to input string
 *     output - Pointer to output array
 *   Returns:
 *     Number of bytes in the decoded binary
 */
static unsigned int decode_base64(unsigned char input[], unsigned char output[]);

static unsigned char binary_to_base64(unsigned char v) {
  // Capital letters - 'A' is ascii 65 and base64 0
  if(v < 26) return v + 'A';
  
  // Lowercase letters - 'a' is ascii 97 and base64 26
  if(v < 52) return v + 71;
  
  // Digits - '0' is ascii 48 and base64 52
  if(v < 62) return v - 4;
  
  // '+' is ascii 43 and base64 62
  if(v == 62) return '+';
  
  // '/' is ascii 47 and base64 63
  if(v == 63) return '/';
  
  return 64;
}

static unsigned char base64_to_binary(unsigned char c) {
  // Capital letters - 'A' is ascii 65 and base64 0
  if('A' <= c && c <= 'Z') return c - 'A';
  
  // Lowercase letters - 'a' is ascii 97 and base64 26
  if('a' <= c && c <= 'z') return c - 71;
  
  // Digits - '0' is ascii 48 and base64 52
  if('0' <= c && c <= '9') return c + 4;
  
  // '+' is ascii 43 and base64 62
  if(c == '+') return 62;
  
  // '/' is ascii 47 and base64 63
  if(c == '/') return 63;
  
  return 255;
}

static unsigned int encode_base64_length(unsigned int input_length) {
  return (input_length + 2)/3*4;
}

static unsigned int decode_base64_length(unsigned char input[]) {
  unsigned char *start = input;
  
  while(base64_to_binary(input[0]) < 64) {
    ++input;
  }
  
  unsigned int input_length = input - start;
  
  unsigned int output_length = input_length/4*3;
  
  switch(input_length % 4) {
    default: return output_length;
    case 2: return output_length + 1;
    case 3: return output_length + 2;
  }
}

static unsigned int encode_base64(unsigned char input[], unsigned int input_length, unsigned char output[]) {
  unsigned int full_sets = input_length/3;
  
  // While there are still full sets of 24 bits...
  for(unsigned int i = 0; i < full_sets; ++i) {
    output[0] = binary_to_base64(                         input[0] >> 2);
    output[1] = binary_to_base64((input[0] & 0x03) << 4 | input[1] >> 4);
    output[2] = binary_to_base64((input[1] & 0x0F) << 2 | input[2] >> 6);
    output[3] = binary_to_base64( input[2] & 0x3F);
    
    input += 3;
    output += 4;
  }
  
  switch(input_length % 3) {
    case 0:
      output[0] = '\0';
      break;
    case 1:
      output[0] = binary_to_base64(                         input[0] >> 2);
      output[1] = binary_to_base64((input[0] & 0x03) << 4);
      output[2] = '=';
      output[3] = '=';
      output[4] = '\0';
      break;
    case 2:
      output[0] = binary_to_base64(                         input[0] >> 2);
      output[1] = binary_to_base64((input[0] & 0x03) << 4 | input[1] >> 4);
      output[2] = binary_to_base64((input[1] & 0x0F) << 2);
      output[3] = '=';
      output[4] = '\0';
      break;
  }
  
  return encode_base64_length(input_length);
}

static unsigned int decode_base64(unsigned char input[], unsigned char output[]) {
  unsigned int output_length = decode_base64_length(input);
  
  // While there are still full sets of 24 bits...
  for(unsigned int i = 2; i < output_length; i += 3) {
    output[0] = base64_to_binary(input[0]) << 2 | base64_to_binary(input[1]) >> 4;
    output[1] = base64_to_binary(input[1]) << 4 | base64_to_binary(input[2]) >> 2;
    output[2] = base64_to_binary(input[2]) << 6 | base64_to_binary(input[3]);
    
    input += 4;
    output += 3;
  }
  
  switch(output_length % 3) {
    case 1:
      output[0] = base64_to_binary(input[0]) << 2 | base64_to_binary(input[1]) >> 4;
      break;
    case 2:
      output[0] = base64_to_binary(input[0]) << 2 | base64_to_binary(input[1]) >> 4;
      output[1] = base64_to_binary(input[1]) << 4 | base64_to_binary(input[2]) >> 2;
      break;
  }
  
  return output_length;
}

/********************************************************************
** Buffer low-level implementation
**
** Extracted from Tasmota SBuffer lib
********************************************************************/
// static inline uint8_t* buf_get_buf(buf_impl* attr)
// {
//     return &attr->bufptr[0];
// }

// shrink or increase. If increase, fill with zeores. Cannot go beyond `size`
static void buf_set_len(buf_impl* attr, const size_t len)
{
    int32_t old_len = attr->len;
    attr->len = ((int32_t)len <= attr->size) ? (int32_t)len : attr->size;
    if (old_len < attr->len) {
        memset((void*) &attr->bufptr[old_len], 0, attr->len - old_len);
    }
}

static size_t buf_add1(buf_impl* attr, const uint8_t data) // append 8 bits value
{
    if (attr->len < attr->size) {       // do we have room for 1 byte
        attr->bufptr[attr->len++] = data;
    }
    return attr->len;
}

static size_t buf_add2_le(buf_impl* attr, const uint16_t data) // append 16 bits value
{
    if (attr->len < attr->size - 1) {    // do we have room for 2 bytes
        attr->bufptr[attr->len++] = data;
        attr->bufptr[attr->len++] = data >> 8;
    }
    return attr->len;
}

static size_t buf_add2_be(buf_impl* attr, const uint16_t data) // append 16 bits value
{
    if (attr->len < attr->size - 1) {    // do we have room for 2 bytes
        attr->bufptr[attr->len++] = data >> 8;
        attr->bufptr[attr->len++] = data;
    }
    return attr->len;
}

static size_t buf_add4_le(buf_impl* attr, const uint32_t data) // append 32 bits value
{
    if (attr->len < attr->size - 3) {     // do we have room for 4 bytes
        attr->bufptr[attr->len++] = data;
        attr->bufptr[attr->len++] = data >> 8;
        attr->bufptr[attr->len++] = data >> 16;
        attr->bufptr[attr->len++] = data >> 24;
    }
    return attr->len;
}

size_t buf_add4_be(buf_impl* attr, const uint32_t data) // append 32 bits value
{
    if (attr->len < attr->size - 3) {     // do we have room for 4 bytes
        attr->bufptr[attr->len++] = data >> 24;
        attr->bufptr[attr->len++] = data >> 16;
        attr->bufptr[attr->len++] = data >> 8;
        attr->bufptr[attr->len++] = data;
    }
    return attr->len;
}

static size_t buf_add_buf(buf_impl* attr, buf_impl* attr2)
{
    if (attr->len + attr2->len <= attr->size) {
        for (int32_t i = 0; i < attr2->len; i++) {
            attr->bufptr[attr->len++] = attr2->bufptr[i];
        }
    }
    return attr->len;
}

static size_t buf_add_raw(buf_impl* attr, const void* buf_raw, int32_t len)
{
    uint8_t *buf = (uint8_t*) buf_raw;
    if ((len > 0) && (attr->len + len <= attr->size)) {
        for (int32_t i = 0; i < len; i++) {
            attr->bufptr[attr->len++] = buf[i];
        }
    }
    return attr->len;
}

static uint8_t buf_get1(buf_impl* attr, int offset)
{
    if ((offset >= 0) && (offset < attr->len)) {
        return attr->bufptr[offset];
    }
    return 0;
}

static void buf_set1(buf_impl* attr, size_t offset, uint8_t data)
{
    if ((int32_t)offset < attr->len) {
        attr->bufptr[offset] = data;
    }
}

static void buf_set2_le(buf_impl* attr, size_t offset, uint16_t data)
{
    if ((int32_t)offset + 1 < attr->len) {
        attr->bufptr[offset] = data & 0xFF;
        attr->bufptr[offset+1] = data >> 8;
    }
}

static void buf_set2_be(buf_impl* attr, size_t offset, uint16_t data)
{
    if ((int32_t)offset + 1 < attr->len) {
        attr->bufptr[offset+1] = data & 0xFF;
        attr->bufptr[offset] = data >> 8;
    }
}

static uint16_t buf_get2_le(buf_impl* attr, size_t offset)
{
    if ((int32_t)offset + 1 < attr->len) {
        return attr->bufptr[offset] | (attr->bufptr[offset+1] << 8);
    }
    return 0;
}

static uint16_t buf_get2_be(buf_impl* attr, size_t offset)
{
    if ((int32_t)offset + 1 < attr->len) {
        return attr->bufptr[offset+1] | (attr->bufptr[offset] << 8);
    }
    return 0;
}

static uint32_t buf_get3_le(buf_impl* attr, size_t offset)
{
    if ((int32_t)offset + 2 < attr->len) {
        return attr->bufptr[offset] | (attr->bufptr[offset+1] << 8) | (attr->bufptr[offset+2] << 16);
    }
    return 0;
}

static uint32_t buf_get3_be(buf_impl* attr, size_t offset)
{
    if ((int32_t)offset + 2 < attr->len) {
        return attr->bufptr[offset+2] | (attr->bufptr[offset+1] << 8) | (attr->bufptr[offset] << 16);
    }
    return 0;
}

static void buf_set3_le(buf_impl* attr, size_t offset, uint32_t data)
{
    if ((int32_t)offset + 2 < attr->len) {
        attr->bufptr[offset] = data & 0xFF;
        attr->bufptr[offset+1] = (data >> 8) & 0xFF;
        attr->bufptr[offset+2] = (data >> 16) & 0xFF;
    }
}

static void buf_set3_be(buf_impl* attr, size_t offset, uint32_t data)
{
    if ((int32_t)offset + 2 < attr->len) {
        attr->bufptr[offset+2] = data & 0xFF;
        attr->bufptr[offset+1] = (data >> 8) & 0xFF;
        attr->bufptr[offset] = (data >> 16) & 0xFF;
    }
}

static void buf_set4_le(buf_impl* attr, size_t offset, uint32_t data)
{
    if ((int32_t)offset + 3 < attr->len) {
        attr->bufptr[offset] = data & 0xFF;
        attr->bufptr[offset+1] = (data >> 8) & 0xFF;
        attr->bufptr[offset+2] = (data >> 16) & 0xFF;
        attr->bufptr[offset+3] = data >> 24;
    }
}

static void buf_set4_be(buf_impl* attr, size_t offset, uint32_t data)
{
    if ((int32_t)offset + 3 < attr->len) {
        attr->bufptr[offset+3] = data & 0xFF;
        attr->bufptr[offset+2] = (data >> 8) & 0xFF;
        attr->bufptr[offset+1] = (data >> 16) & 0xFF;
        attr->bufptr[offset] = data >> 24;
    }
}

static uint32_t buf_get4_le(buf_impl* attr, size_t offset)
{
    if ((int32_t)offset + 3 < attr->len) {
        return attr->bufptr[offset] | (attr->bufptr[offset+1] << 8) |
            (attr->bufptr[offset+2] << 16) | (attr->bufptr[offset+3] << 24);
    }
    return 0;
}

static uint32_t buf_get4_be(buf_impl* attr, size_t offset)
{
    if ((int32_t)offset + 3 < attr->len) {
        return attr->bufptr[offset+3] | (attr->bufptr[offset+2] << 8) |
            (attr->bufptr[offset+1] << 16) | (attr->bufptr[offset] << 24);
    }
    return 0;
}

// nullptr accepted
static bbool buf_equals(buf_impl* buf1, buf_impl* buf2)
{
    if (buf1 == buf2) { return btrue; }
    if (!buf1 || !buf2) { return bfalse; }   // at least one attr is not empty
    // we know that both buf1 and buf2 are non-null
    if (buf1->len != buf2->len) { return bfalse; }
    size_t len = buf1->len;
    if (!buf1->bufptr && !buf2->bufptr) { return btrue; }   /* if both are null then considered equal */
    if (!buf1->bufptr || !buf2->bufptr) { return bfalse; }  /* if only one is null, then not equal */
    /* here none of the pointer are null */
    for (uint32_t i=0; i<len; i++) {
        if (buf_get1(buf1, i) != buf_get1(buf2, i)) { return bfalse; }
    }
    return btrue;
}

static uint8_t asc2byte(char chr)
{
    uint8_t rVal = 0;
    if (isdigit(chr)) { rVal = chr - '0'; }
    else if (chr >= 'A' && chr <= 'F') { rVal = chr + 10 - 'A'; }
    else if (chr >= 'a' && chr <= 'f') { rVal = chr + 10 - 'a'; }
    return rVal;
}

// does not check if there is enough room before hand, truncated if buffer too small
static void buf_add_hex(buf_impl* attr, const char *hex, size_t len)
{
    uint8_t val;
    for (; len > 1; len -= 2) {
        val = asc2byte(*hex++) << 4;
        val |= asc2byte(*hex++);
        buf_add1(attr, val);
    }
}

/********************************************************************
** Wrapping into lib
********************************************************************/

/* if the bufptr is null, don't try to dereference and raise an exception instead */
static void check_ptr(bvm *vm, const buf_impl* attr) {
    if (!attr->bufptr) {
        be_raise(vm, "value_error", "operation not allowed on <null> pointer");
    }
}

static void check_ptr_modifiable(bvm *vm, const buf_impl* attr) {
    if (attr->solidified) {
     be_raise(vm, "value_error", BYTES_READ_ONLY_MESSAGE);
    }
    if (!attr->bufptr) {
        be_raise(vm, "value_error", "operation not allowed on <null> pointer");
    }
}

/* load instance attribute into a single structure, and store 'previous' values in order to later update only the changed ones */
/* stack item 1 must contain the instance */
buf_impl m_read_attributes(bvm *vm, int idx)
{
    buf_impl attr;
    be_getmember(vm, idx, ".p");
    attr.bufptr = attr.prev_bufptr = be_tocomptr(vm, -1);
    be_pop(vm, 1);

    be_getmember(vm, idx, ".len");
    attr.len = attr.prev_len = be_toint(vm, -1);
    be_pop(vm, 1);

    be_getmember(vm, idx, ".size");
    int32_t signed_size = be_toint(vm, -1);
    attr.fixed = bfalse;
    attr.mapped = bfalse;
    attr.solidified = bfalse;
    if (signed_size < 0) {
        if (signed_size == BYTES_SIZE_MAPPED) {
            attr.mapped = btrue;
        }
        if (signed_size == BYTES_SIZE_SOLIDIFIED) {
            attr.solidified = btrue;
        }
        signed_size = attr.len;
        attr.fixed = btrue;
    }
    attr.size = attr.prev_size = signed_size;
    be_pop(vm, 1);
    return attr;
}

static void m_assert_not_readlonly(bvm *vm, const buf_impl* attr)
{
    if (attr->solidified) {
     be_raise(vm, "value_error", BYTES_READ_ONLY_MESSAGE);
    }
}

/* Write back attributes to the bytes instance, only if values changed after loading */
/* stack item 1 must contain the instance */
void m_write_attributes(bvm *vm, int rel_idx, const buf_impl * attr)
{
    m_assert_not_readlonly(vm, attr);
    int idx = be_absindex(vm, rel_idx);
    if (attr->bufptr != attr->prev_bufptr) {
        be_pushcomptr(vm, attr->bufptr);
        be_setmember(vm, idx, ".p");
        be_pop(vm, 1);
    }

    if (attr->len != attr->prev_len) {
        be_pushint(vm, attr->len);
        be_setmember(vm, idx, ".len");
        be_pop(vm, 1);
    }

    int32_t new_size = attr->size;
    if (attr->mapped) {
        new_size = BYTES_SIZE_MAPPED;
    } else if (attr->fixed) {
        new_size = BYTES_SIZE_FIXED;
    }
    if (new_size != attr->prev_size) {
        be_pushint(vm, new_size);
        be_setmember(vm, idx, ".size");
        be_pop(vm, 1);
    }
}

// buf_impl * bytes_realloc(bvm *vm, buf_impl *oldbuf, int32_t size)
void bytes_realloc(bvm *vm, buf_impl * attr, int32_t size)
{
    m_assert_not_readlonly(vm, attr);
    if (!attr->fixed && size < 4) { size = 4; }
    if (size > vm->bytesmaxsize) { size = vm->bytesmaxsize; }
    size_t oldsize = attr->bufptr ? attr->size : 0;
    attr->bufptr = (uint8_t*) be_realloc(vm, attr->bufptr, oldsize, size);  /* malloc */
    attr->size = size;
    if (!attr->bufptr) {
        attr->len = 0; /* allocate a new buffer */
    }
}

/* allocate a new `bytes` object with pre-allocated size */
static void bytes_new_object(bvm *vm, size_t size)
{
    be_getbuiltin(vm, "bytes");
    be_pushint(vm, size);
    be_call(vm, 1);
    be_pop(vm, 1);
}

/*
 * constructor for bytes()
 * Arg0 is always self 
 *
 * Option 1: main use
 *    Arg1: string - a string representing the bytes in HEX
 *
 * Option 2:
 *    Arg1: string - a string representing the bytes in HEX
 *    Arg2: int - pre-reserved buffer size. If negative, size is fixed and cannot be later changed
 *
 * Option 3: used by subclasses like ctypes
 *    Arg1: int - pre-reserved buffer size. If negative, size is fixed and cannot be later changed
 *
 * Option 4: mapped buffer
 *    Arg1: comptr - buffer address of the mapped buffer
 *    Arg2: int - buffer size. Always fixed (negative or positive)
 *
 * */
static int m_init(bvm *vm)
{
    int argc = be_top(vm);
    buf_impl attr = { 0, 0, NULL, 0, -1, NULL, bfalse, bfalse, bfalse }; /* initialize prev_values to invalid to force a write at the end */
    /* size cannot be 0, len cannot be negative */
    const char * hex_in = NULL;

    int32_t size_arg = 0;
    if (argc > 1 && be_isint(vm, 2)) {
        size_arg = be_toint(vm, 2);  /* raw size arg, can be positive or negative */
    } else if (argc > 2 && be_isint(vm, 3)) {
        size_arg = be_toint(vm, 3);  /* raw size arg, can be positive or negative */
    }
    
    if (argc > 1 && be_iscomptr(vm, 2)) {
        if (size_arg) {
            attr.len = (size_arg < 0) ? -size_arg : size_arg;
            attr.bufptr = be_tocomptr(vm, 2);
            attr.fixed = btrue;
            attr.mapped = btrue;
            m_write_attributes(vm, 1, &attr);   /* write attributes back to instance */
            be_return_nil(vm);
        } else {
            be_raise(vm, "value_error", "size is required");
        }
    }
    if (size_arg == 0) { size_arg = BYTES_DEFAULT_SIZE; }   /* if not specified, use default size */

    /* compute actual size to be reserved */
    if (size_arg >= 0) {
        size_arg += BYTES_HEADROOM;     /* add automatic headroom to slightly overallocate */
        if (size_arg > attr.size) {
            attr.size = size_arg;
        }
    } else {
        attr.size = -size_arg;   /* set the size to a fixed negative value */
        attr.fixed = btrue;
    }
    size_arg = attr.size;

    /* if arg1 is string, we convert hex */
    if (argc > 1 && be_isstring(vm, 2)) {
        hex_in = be_tostring(vm, 2);
        if (hex_in) {
            size_arg = strlen(hex_in) / 2;        /* allocate headroom */
        }
        /* check if fixed size that we have the right size */
        if (size_arg > attr.size) {
            if (attr.fixed) {
                be_raise(vm, BYTES_RESIZE_ERROR, BYTES_RESIZE_MESSAGE);
            } else {
                attr.size = size_arg;
            }
        }
    }
    
    /* allocate */
    bytes_realloc(vm, &attr, attr.size); /* allocate new buffer */
    if (!attr.bufptr) {
        be_throw(vm, BE_MALLOC_FAIL);
    }
    
    if (hex_in) {
        buf_add_hex(&attr, hex_in, strlen(hex_in));
    }

    /* if fixed size, set len to size */
    if (attr.fixed) {
        buf_set_len(&attr, attr.size);
    }

    m_write_attributes(vm, 1, &attr);   /* write attributes back to instance */
    be_return_nil(vm);
}

/* deallocate buffer */
static int m_deinit(bvm *vm)
{
    buf_impl attr = m_read_attributes(vm, 1);
    if (attr.bufptr != NULL && !attr.mapped) {
        be_realloc(vm, attr.bufptr, attr.size, 0);
    }
    attr.size = 0;
    attr.len = 0;
    attr.bufptr = NULL;
    m_write_attributes(vm, 1, &attr);
    be_return_nil(vm);
}

/* grow or shrink to the exact value */
/* stack item 1 must contain the instance */
void _bytes_resize(bvm *vm, buf_impl * attr, size_t new_size) {
    bytes_realloc(vm, attr, new_size);
    if (!attr->bufptr) {
        be_throw(vm, BE_MALLOC_FAIL);
    }
}

/* grow if needed but don't shrink */
/* if grow, then add some headroom */
/* stack item 1 must contain the instance */
void bytes_resize(bvm *vm, buf_impl * attr, size_t new_size) {
    if (attr->mapped) { return; }   /* if mapped, don't bother with allocation */
    /* when resized to smaller, we introduce a new heurstic */
    /* If the buffer is 64 bytes or smaller, don't shrink */
    /* Shrink buffer only if target size is smaller than half the original size */
    if (attr->size >= (int32_t)new_size) {  /* enough room, consider if need to shrink */
        if (attr->size <= 64) { return; }  /* don't shrink if below 64 bytes */
        if (attr->size < (int32_t)new_size * 2) { return; }
    }
    _bytes_resize(vm, attr, new_size);
}

buf_impl bytes_check_data(bvm *vm, size_t add_size) {
    buf_impl attr = m_read_attributes(vm, 1);
    /* check if the `size` is big enough */
    if (attr.len + (int32_t)add_size > attr.size) {
        if (attr.fixed) {
            be_raise(vm, BYTES_RESIZE_ERROR, BYTES_RESIZE_MESSAGE);
        }
        /* it does not fit so we need to realocate the buffer */
        bytes_resize(vm, &attr, attr.len + add_size);
    }
    return attr;
}

size_t be_bytes_tohex(char * out, size_t outsz, const uint8_t * in, size_t insz) {
  static const char * hex = "0123456789ABCDEF";
  const uint8_t * pin = in;
  char * pout = out;
  for (; pin < in + insz; pout += 2, pin++) {
    pout[0] = hex[((*pin)>>4) & 0xF];
    pout[1] = hex[ (*pin)     & 0xF];
    if (pout + 3 > out + outsz) { break; } /* check overflow */
  }
  pout[0] = 0; /* terminating Nul char */
  return pout - out;
}

static int m_tostring(bvm *vm)
{
    int argc = be_top(vm);
    int32_t max_len = 32;  /* limit to 32 bytes by default */
    int truncated = 0;
    if (argc > 1 && be_isint(vm, 2)) {
        max_len = be_toint(vm, 2);  /* you can specify the len as second argument, or 0 for unlimited */
    }
    buf_impl attr = m_read_attributes(vm, 1);
    if (attr.bufptr) {              /* pointer looks valid */
        int32_t len = attr.len;
        if (max_len > 0 && len > max_len) {
            len = max_len;  /* limit output size */
            truncated = 1;
        }
        size_t hex_len = len * 2 + 5 + 2 + 2 + 1 + truncated * 3;  /* reserve size for `bytes("")\0` - 9 chars */

        char * hex_out = be_pushbuffer(vm, hex_len);
        size_t l = be_strlcpy(hex_out, "bytes('", hex_len);
        l += be_bytes_tohex(&hex_out[l], hex_len - l, attr.bufptr, len);
        if (truncated) {
            l += be_strlcpy(&hex_out[l], "...", hex_len - l);
        }
        l += be_strlcpy(&hex_out[l], "')", hex_len - l);

        be_pushnstring(vm, hex_out, l); /* make escape string from buffer */
        be_remove(vm, -2); /* remove buffer */
    } else {                    /* pointer is null, don't try to dereference it as it would crash */
        be_pushstring(vm, "bytes(<null>)");
    }
    be_return(vm);
}

static int m_tohex(bvm *vm)
{
    buf_impl attr = m_read_attributes(vm, 1);
    if (attr.bufptr) {              /* pointer looks valid */
        int32_t len = attr.len;
        size_t hex_len = len * 2 + 1;

        char * hex_out = be_pushbuffer(vm, hex_len);
        size_t l = be_bytes_tohex(hex_out, hex_len, attr.bufptr, len);

        be_pushnstring(vm, hex_out, l); /* make escape string from buffer */
        be_remove(vm, -2); /* remove buffer */
    } else {                    /* pointer is null, don't try to dereference it as it would crash */
        be_pushstring(vm, "");
    }
    be_return(vm);
}

/*
 * Copy the buffer into a string without any changes
 */
static int m_asstring(bvm *vm)
{
    buf_impl attr = bytes_check_data(vm, 0);
    check_ptr(vm, &attr);
    be_pushnstring(vm, (const char*) attr.bufptr, attr.len);
    be_return(vm);
}

static int m_fromstring(bvm *vm)
{
    int argc = be_top(vm);
    if (argc >= 2 && be_isstring(vm, 2)) {
        const char *s = be_tostring(vm, 2);
        int32_t len = be_strlen(vm, 2);      /* calling be_strlen to support null chars in string */
        buf_impl attr = bytes_check_data(vm, 0);
        check_ptr_modifiable(vm, &attr);
        if (attr.fixed && attr.len != len) {
            be_raise(vm, BYTES_RESIZE_ERROR, BYTES_RESIZE_MESSAGE);
        }
        bytes_resize(vm, &attr, len); /* resize if needed */
        if (len > attr.size) { len = attr.size; } /* avoid overflow */
        memmove(attr.bufptr, s, len);
        attr.len = len;
        be_pop(vm, 1); /* remove arg to leave instance */
        m_write_attributes(vm, 1, &attr);  /* update attributes */
        be_return(vm);
    }
    be_raise(vm, "type_error", "operand must be a string");
    be_return_nil(vm);
}

/*
 * Add an int made of 1, 2 or 4 bytes, in little or big endian
 * `add(value:int[, size:int = 1]) -> instance`
 * 
 * size: may be 1, 2, 4 (little endian), or -1, -2, -4 (big endian)
 *       obvisouly -1 is idntical to 1
 *       size==0 does nothing
 */
static int m_add(bvm *vm)
{
    int argc = be_top(vm);
    buf_impl attr = bytes_check_data(vm, 4); /* we reserve 4 bytes anyways */
    check_ptr_modifiable(vm, &attr);
    if (attr.fixed) { be_raise(vm, BYTES_RESIZE_ERROR, BYTES_RESIZE_MESSAGE); }
    if (argc >= 2 && be_isint(vm, 2)) {
        int32_t v = be_toint(vm, 2);
        int vsize = 1;
        if (argc >= 3 && be_isint(vm, 3)) {
            vsize = be_toint(vm, 3);
        }
        switch (vsize) {
            case 0:                               break;
            case -1:    /* fallback below */
            case 1:     buf_add1(&attr, v);       break;
            case 2:     buf_add2_le(&attr, v);    break;
            case 4:     buf_add4_le(&attr, v);    break;
            case -2:    buf_add2_be(&attr, v);    break;
            case -4:    buf_add4_be(&attr, v);    break;
            default:    be_raise(vm, "type_error", "size must be -4, -2, -1, 0, 1, 2 or 4.");
        }
        be_pop(vm, argc - 1);
        m_write_attributes(vm, 1, &attr);  /* update attributes */
        be_return(vm);
    }
    be_return_nil(vm);
}

/*
 * Get an int made of 1, 2 or 4 bytes, in little or big endian
 * `get(index:int[, size:int = 1]) -> int`
 * 
 * size: may be 1, 2, 4 (little endian), or -1, -2, -4 (big endian)
 *       obvisouly -1 is identical to 1
 *       0 returns nil
 */
static int m_get(bvm *vm, bbool sign)
{
    int argc = be_top(vm);
    buf_impl attr = bytes_check_data(vm, 0); /* we reserve 4 bytes anyways */
    check_ptr(vm, &attr);
    if (argc >=2 && be_isint(vm, 2)) {
        int32_t idx = be_toint(vm, 2);
        int vsize = 1;
        if (argc >= 3 && be_isint(vm, 3)) {
            vsize = be_toint(vm, 3);
        }
        if (idx < 0) {
            idx = attr.len + idx;       /* if index is negative, count from end */
        }
        if (idx < 0) {
            vsize = 0;                  /* if still negative, then invalid, return 0 */
        }
        int ret = 0;
        switch (vsize) {
            case 0:     break;
            case -1:    /* fallback below */
            case 1:     ret = buf_get1(&attr, idx);
                        if (sign) { ret = (int8_t)(uint8_t) ret; }
                        break;
            case 2:     ret = buf_get2_le(&attr, idx);
                        if (sign) { ret = (int16_t)(uint16_t) ret; }
                        break;
            case 3:     ret = buf_get3_le(&attr, idx);
                        if (sign & (ret & 0x800000)) { ret = ret | 0xFF000000; }
                        break;
            case 4:     ret = buf_get4_le(&attr, idx);    break;
            case -2:    ret = buf_get2_be(&attr, idx);
                        if (sign) { ret = (int16_t)(uint16_t) ret; }
                        break;
            case -3:    ret = buf_get3_be(&attr, idx);
                        if (sign & (ret & 0x800000)) { ret = ret | 0xFF000000; }
                        break;
            case -4:    ret = buf_get4_be(&attr, idx);    break;
            default:    be_raise(vm, "type_error", "size must be -4, -3, -2, -1, 0, 1, 2, 3 or 4.");
        }
        be_pop(vm, argc - 1);
        be_pushint(vm, ret);
        be_return(vm);
    }
    be_return_nil(vm);
}

/*
 * Get a float (32 bits)
 * `getfloat(index:int [, big_endian:bool]) -> real`
 */
static int m_getfloat(bvm *vm)
{
    int argc = be_top(vm);
    buf_impl attr = bytes_check_data(vm, 0); /* we reserve 4 bytes anyways */
    check_ptr(vm, &attr);
    if (argc >=2 && be_isint(vm, 2)) {
        int32_t idx = be_toint(vm, 2);
        float ret_f = 0;
        if (idx < 0) {
            idx = attr.len + idx;       /* if index is negative, count from end */
        }
        if (idx >= 0) {
            bbool be = bfalse;             /* little endian? */
            if (argc >= 3) {
                be = be_tobool(vm, 3);
            }
            int32_t ret_i = be ? buf_get4_be(&attr, idx) : buf_get4_le(&attr, idx);
            ret_f = *(float*) &ret_i;
        }
        be_pop(vm, argc - 1);
        be_pushreal(vm, ret_f);
        be_return(vm);
    }
    be_return_nil(vm);
}

/* signed int */
static int m_geti(bvm *vm)
{
    return m_get(vm, 1);
}

/* unsigned int */
static int m_getu(bvm *vm)
{
    return m_get(vm, 0);
}

/*
 * Set an int made of 1, 2 or 4 bytes, in little or big endian
 * `set(index:int, value:int[, size:int = 1]) -> nil`
 * 
 * size: may be 1, 2, 4 (little endian), or -1, -2, -4 (big endian)
 *       obvisouly -1 is identical to 1
 *       0 returns nil
 */
static int m_set(bvm *vm)
{
    int argc = be_top(vm);
    buf_impl attr = bytes_check_data(vm, 0); /* we reserve 4 bytes anyways */
    check_ptr_modifiable(vm, &attr);
    if (argc >=3 && be_isint(vm, 2) && be_isint(vm, 3)) {
        int32_t idx = be_toint(vm, 2);
        int32_t value = be_toint(vm, 3);
        int vsize = 1;
        if (argc >= 4 && be_isint(vm, 4)) {
            vsize = be_toint(vm, 4);
        }
        if (idx < 0) {
            idx = attr.len + idx;       /* if index is negative, count from end */
        }
        if (idx < 0) {
            vsize = 0;                  /* if still negative, then invalid, do nothing */
        }
        switch (vsize) {
            case 0:     break;
            case -1:    /* fallback below */
            case 1:     buf_set1(&attr, idx, value);      break;
            case 2:     buf_set2_le(&attr, idx, value);   break;
            case 3:     buf_set3_le(&attr, idx, value);   break;
            case 4:     buf_set4_le(&attr, idx, value);   break;
            case -2:    buf_set2_be(&attr, idx, value);   break;
            case -3:    buf_set3_be(&attr, idx, value);   break;
            case -4:    buf_set4_be(&attr, idx, value);   break;
            default:    be_raise(vm, "type_error", "size must be -4, -3, -2, -1, 0, 1, 2, 3 or 4.");
        }
        be_pop(vm, argc - 1);
        // m_write_attributes(vm, 1, &attr);  /* update attributes */
        be_return_nil(vm);
    }
    be_return_nil(vm);
}

/*
 * Set a 32 bits float
 * `setfloat(index:int, value:real or int [, big_endian:bool]) -> nil`
 * 
 */
static int m_setfloat(bvm *vm)
{
    int argc = be_top(vm);
    buf_impl attr = bytes_check_data(vm, 0); /* we reserve 4 bytes anyways */
    check_ptr_modifiable(vm, &attr);
    if (argc >=3 && be_isint(vm, 2) && (be_isint(vm, 3) || be_isreal(vm, 3))) {
        int32_t idx = be_toint(vm, 2);
        if (idx < 0) {
            idx = attr.len + idx;       /* if index is negative, count from end */
        }
        if (idx >= 0) {
            float val_f = (float) be_toreal(vm, 3);
            int32_t* val_i = (int32_t*) &val_f;
            bbool be = bfalse;
            if (argc >= 4) {
                be = be_tobool(vm, 4);
            }
            if (be) { buf_set4_be(&attr, idx, *val_i); } else { buf_set4_le(&attr, idx, *val_i); }
            be_pop(vm, argc - 1);
            // m_write_attributes(vm, 1, &attr);  /* update attributes */
        }
        be_return_nil(vm);
    }
    be_return_nil(vm);
}

/*
 * Add a 32 bits float
 * `addfloat(value:real or int [, big_endian:bool]) -> instance`
 * 
 */
static int m_addfloat(bvm *vm)
{
    int argc = be_top(vm);
    buf_impl attr = bytes_check_data(vm, 4); /* we reserve 4 bytes anyways */
    check_ptr_modifiable(vm, &attr);
    if (attr.fixed) { be_raise(vm, BYTES_RESIZE_ERROR, BYTES_RESIZE_MESSAGE); }
    if (argc >=2 && (be_isint(vm, 2) || be_isreal(vm, 2))) {
        float val_f = (float) be_toreal(vm, 2);
        int32_t* val_i = (int32_t*) &val_f;
        bbool be = bfalse;
        if (argc >= 3) {
            be = be_tobool(vm, 3);
        }
        if (be) { buf_add4_be(&attr, *val_i); } else { buf_add4_le(&attr, *val_i); }
        be_pop(vm, argc - 1);
        m_write_attributes(vm, 1, &attr);  /* update attributes */
        be_return(vm);
    }
    be_return_nil(vm);
}

/*
 * Fills a buffer with another buffer.
 *
 * This is meant to be very flexible and avoid loops
 * 
 * `setbytes(index:int, fill:bytes [, from:int, len:int]) -> nil`
 * 
 */
static int m_setbytes(bvm *vm)
{
    int argc = be_top(vm);
    buf_impl attr = bytes_check_data(vm, 0); /* we reserve 4 bytes anyways */
    check_ptr_modifiable(vm, &attr);
    if (argc >=3 && be_isint(vm, 2) && (be_isbytes(vm, 3))) {
        int32_t idx = be_toint(vm, 2);
        size_t from_len_total;
        const uint8_t* buf_ptr = (const uint8_t*) be_tobytes(vm, 3, &from_len_total);
        if (idx < 0) {
            idx = attr.len + idx;       /* if index is negative, count from end */
        }
        if (idx < 0) { idx = 0; }       /* if still negative, start from offset 0 */
        if (idx >= attr.len) { idx = attr.len; }

        int32_t from_byte = 0;
        if (argc >= 4 && be_isint(vm, 4)) {
            from_byte = be_toint(vm, 4);
            if (from_byte < 0) { from_byte = 0; }
            if ((size_t)from_byte >= from_len_total) { from_byte = from_len_total; }
        }

        int32_t from_len = from_len_total - from_byte;
        if (argc >= 5 && be_isint(vm, 5)) {
            from_len = be_toint(vm, 5);
            if (from_len < 0) { from_len = 0; }
            if (from_len >= (int32_t)from_len_total) { from_len = from_len_total; }
        }
        if (idx + from_len >= attr.len) { from_len = attr.len - idx; }

        // all parameters ok
        if (from_len > 0) {
            memmove(attr.bufptr + idx, buf_ptr + from_byte, from_len);
        }
    }
    be_return_nil(vm);
}


/*
 * Reverses in-place a sub-buffer composed of groups of n-bytes packets
 *
 * This is useful for pixel manipulation when displaying RGB pixels
 * 
 * `reverse([index:int, len:int, grouplen:int]) -> self`
 * 
 */
static int m_reverse(bvm *vm)
{
    int argc = be_top(vm);
    buf_impl attr = bytes_check_data(vm, 0); /* we reserve 4 bytes anyways */
    check_ptr_modifiable(vm, &attr);

    int32_t idx = 0;            /* start from index 0 */
    int32_t len = attr.len;     /* entire len */
    int32_t grouplen = 1;       /* default to 1-byte group */

    if (argc >= 2 && be_isint(vm, 2)) {
        idx = be_toint(vm, 2);
        if (idx < 0) { idx = attr.len + idx; }  /* if negative, count from end */
        if (idx < 0) { idx = 0; }               /* railguards */
        if (idx > attr.len) { idx = attr.len; }
    }
    if (argc >= 3 && be_isint(vm, 3)) {
        len = be_toint(vm, 3);
        if (len < 0) { len = attr.len - idx; }  /* negative len means */
    }
    if (idx + len >= attr.len) { len = attr.len - idx; }

    // truncate len to multiple of grouplen
    if (argc >= 4 && be_isint(vm, 4)) {
        grouplen = be_toint(vm, 4);
        if (grouplen <= 0) { grouplen = 1; }
    }
    len = len - (len % grouplen);

    // apply reverse
    if (len > 0) {
        if (grouplen == 1) {
            /* fast version if simple byte inversion */
            for (int32_t i = idx, j = idx + len -1; i < j; i++, j--) {
                uint8_t temp = attr.bufptr[i];
                attr.bufptr[i] = attr.bufptr[j];
                attr.bufptr[j] = temp;
            }
        } else {
            for (int32_t i = idx, j = idx + len - grouplen; i < j; i += grouplen, j -= grouplen) {
                for (int32_t k = 0; k < grouplen; k++) {
                    uint8_t temp = attr.bufptr[i+k];
                    attr.bufptr[i+k] = attr.bufptr[j+k];
                    attr.bufptr[j+k] = temp;
                }
            }
        }
    }
    be_pushvalue(vm, 1);    /* push bytes object */
    be_return(vm);
}

static int m_setitem(bvm *vm)
{
    int argc = be_top(vm);
    buf_impl attr = bytes_check_data(vm, 0); /* we reserve 4 bytes anyways */
    check_ptr_modifiable(vm, &attr);
    if (argc >=3 && be_isint(vm, 2) && be_isint(vm, 3)) {
        int index = be_toint(vm, 2);
        int val = be_toint(vm, 3);
        if (index < 0) {
            index += attr.len;       /* if index is negative, count from end */
        }
        if (index >= 0 && index < attr.len) {
            buf_set1(&attr, index, val);
            // m_write_attributes(vm, 1, &attr);  /* update attributes */
            be_return_nil(vm);
        }
    }
    be_raise(vm, "index_error", "bytes index out of range or value non int");
    be_return_nil(vm);
}

static int m_item(bvm *vm)
{
    int argc = be_top(vm);
    buf_impl attr = bytes_check_data(vm, 0); /* we reserve 4 bytes anyways */
    check_ptr_modifiable(vm, &attr);
    if (argc >=2 && be_isint(vm, 2)) {  /* single byte */
        int index = be_toint(vm,2);
        if (index < 0) {
            index += attr.len;
        }
        if (index >= 0 && index < attr.len) {
            be_pushint(vm, buf_get1(&attr, index));
            be_return(vm);
        }
    }
    if (argc >= 2 && be_isinstance(vm, 2)) {
        const char *cname = be_classname(vm, 2);
        if (!strcmp(cname, "range")) {
            bint lower, upper;
            bint size = attr.len;
            /* get index range */
            be_getmember(vm, 2, "__lower__");
            lower = be_toint(vm, -1);
            be_pop(vm, 1);
            be_getmember(vm, 2, "__upper__");
            upper = be_toint(vm, -1);
            be_pop(vm, 1);
            /* handle negative limits */
            if (upper < 0) { upper += attr.len; }
            if (lower < 0) { lower += attr.len; }
            /* protection scope */
            upper = upper < size ? upper : size - 1;
            lower = lower < 0 ? 0 : lower;
            /* construction result list instance */
            bytes_new_object(vm, upper > lower ? upper-lower : 0);
            buf_impl attr2 = m_read_attributes(vm, -1);

            for (; lower <= upper; ++lower) {
                buf_add1(&attr2, attr.bufptr[lower]);
            }
            m_write_attributes(vm, -1, &attr2);  /* update instance */
            be_return(vm);    
        }
    }
    be_raise(vm, "index_error", "bytes index out of range");
    be_return_nil(vm);
}

static int m_size(bvm *vm)
{
    buf_impl attr = m_read_attributes(vm, 1);
    be_pushint(vm, attr.len);
    be_return(vm);
}

static int m_tobool(bvm *vm)
{
    buf_impl attr = m_read_attributes(vm, 1);
    be_pushbool(vm, attr.len > 0 ? 1 : 0);
    be_return(vm);
}

static int m_resize(bvm *vm)
{
    int argc = be_top(vm);
    buf_impl attr = m_read_attributes(vm, 1);
    check_ptr_modifiable(vm, &attr);

    if (argc <= 1 || !be_isint(vm, 2)) {
        be_raise(vm, "type_error", "size must be of type 'int'");
    }
    int new_len = be_toint(vm, 2);
    if (new_len < 0) {
        new_len = 0;
    }
    if (attr.fixed && attr.len != new_len) {
        be_raise(vm, BYTES_RESIZE_ERROR, BYTES_RESIZE_MESSAGE);
    }

    bytes_resize(vm, &attr, new_len);
    buf_set_len(&attr, new_len);
    be_pop(vm, 1);
    m_write_attributes(vm, 1, &attr);  /* update instance */
    be_return(vm);
}

static int m_clear(bvm *vm)
{
    buf_impl attr = m_read_attributes(vm, 1);
    check_ptr_modifiable(vm, &attr);
    if (attr.fixed) { be_raise(vm, BYTES_RESIZE_ERROR, BYTES_RESIZE_MESSAGE); }
    attr.len = 0;
    m_write_attributes(vm, 1, &attr);  /* update instance */
    be_return_nil(vm);
}

static int m_merge(bvm *vm)
{
    int argc = be_top(vm);
    buf_impl attr = m_read_attributes(vm, 1); /* no resize yet */
    check_ptr(vm, &attr);
    if (argc >= 2 && (be_isbytes(vm, 2) || be_isstring(vm, 2))) {
        const uint8_t * buf;
        int32_t buf_len;
        if (be_isbytes(vm, 2)) {
            buf_impl attr2 = m_read_attributes(vm, 2);
            check_ptr(vm, &attr2);
            buf = attr2.bufptr;
            buf_len = attr2.len;
        } else {
            buf = (const uint8_t *)be_tostring(vm, 2);
            buf_len = strlen((const char *)buf);
        }

        /* allocate new object */
        bytes_new_object(vm, attr.len + buf_len);
        buf_impl attr3 = m_read_attributes(vm, -1);
        check_ptr(vm, &attr3);

        buf_add_buf(&attr3, &attr);
        buf_add_raw(&attr3, buf, buf_len);

        m_write_attributes(vm, -1, &attr3);  /* update instance */
        be_return(vm); /* return self */
    }
    be_raise(vm, "type_error", "operand must be bytes");
    be_return_nil(vm); /* return self */
}

static int m_copy(bvm *vm)
{
    buf_impl attr = m_read_attributes(vm, 1);
    check_ptr(vm, &attr);
    bytes_new_object(vm, attr.len);
    buf_impl attr2 = m_read_attributes(vm, -1);
    check_ptr(vm, &attr2);
    buf_add_buf(&attr2, &attr);
    m_write_attributes(vm, -1, &attr2);  /* update instance */
    be_return(vm); /* return self */
}

/* accept bytes or int as operand */
static int m_connect(bvm *vm)
{
    int argc = be_top(vm);
    buf_impl attr = m_read_attributes(vm, 1);
    check_ptr_modifiable(vm, &attr);
    if (attr.fixed) { be_raise(vm, BYTES_RESIZE_ERROR, BYTES_RESIZE_MESSAGE); }
    if (argc >= 2 && (be_isbytes(vm, 2) || be_isint(vm, 2) || be_isstring(vm, 2))) {
        if (be_isint(vm, 2)) {
            bytes_resize(vm, &attr, attr.len + 1); /* resize */
            buf_add1(&attr, be_toint(vm, 2));
            m_write_attributes(vm, 1, &attr);  /* update instance */
            be_pushvalue(vm, 1);
            be_return(vm); /* return self */
        } else if (be_isstring(vm, 2)) {
            const char *str = be_tostring(vm, 2);
            size_t str_len = strlen(str);
            if (str_len > 0) {
                bytes_resize(vm, &attr, attr.len + str_len); /* resize */
                buf_add_raw(&attr, str, str_len);
                m_write_attributes(vm, 1, &attr);  /* update instance */
            }
            be_pushvalue(vm, 1);
            be_return(vm); /* return self */
        } else {
            buf_impl attr2 = m_read_attributes(vm, 2);
            check_ptr(vm, &attr2);
            bytes_resize(vm, &attr, attr.len + attr2.len); /* resize buf1 for total size */
            buf_add_buf(&attr, &attr2);
            m_write_attributes(vm, 1, &attr);  /* update instance */
            be_pushvalue(vm, 1);
            be_return(vm); /* return self */
        }
    }
    be_raise(vm, "type_error", "operand must be bytes or int or string");
    be_return_nil(vm); /* return self */
}

static int bytes_equal(bvm *vm, bbool iseq)
{
    bbool ret;
    buf_impl attr1 = m_read_attributes(vm, 1);
    if (!be_isbytes(vm, 2)) {
        ret = !iseq;
    } else {
        buf_impl attr2 = m_read_attributes(vm, 2);

        if (buf_equals(&attr1, &attr2)) {
            ret = iseq;
        } else {
            ret = !iseq;
        }
    }
    be_pushbool(vm, ret);
    be_return(vm);
}

static int m_equal(bvm *vm)
{
    return bytes_equal(vm, btrue);
}

static int m_nequal(bvm *vm)
{
    return bytes_equal(vm, bfalse);
}

/*
 * Converts bytes() to a base64 string
 * 
 * Note: there are no line breaks inserted
 * 
 * `b.tob64() -> string`
 */
static int m_tob64(bvm *vm)
{
    buf_impl attr = m_read_attributes(vm, 1);
    check_ptr(vm, &attr);
    int32_t len = attr.len;
    int32_t b64_len = encode_base64_length(len) + 1;  /* size of base64 encoded string for this binary length, add NULL terminator */

    char * b64_out = be_pushbuffer(vm, b64_len);
    size_t converted = encode_base64(attr.bufptr, len, (unsigned char*)b64_out);

    be_pushnstring(vm, b64_out, converted); /* make string from buffer */
    be_remove(vm, -2); /* remove buffer */
    be_return(vm);
}

/*
 * Converts base63 to bytes()
 * 
 * `bytes().fromb64() -> bytes()`
 */
static int m_fromb64(bvm *vm)
{
    int argc = be_top(vm);
    if (argc >= 2 && be_isstring(vm, 2)) {
        const char *s = be_tostring(vm, 2);
        int32_t bin_len = decode_base64_length((unsigned char*)s);   /* do a first pass to calculate the buffer size */

        buf_impl attr = m_read_attributes(vm, 1);
        check_ptr_modifiable(vm, &attr);
        if (attr.fixed && attr.len != bin_len) {
            be_raise(vm, BYTES_RESIZE_ERROR, BYTES_RESIZE_MESSAGE);
        }
        bytes_resize(vm, &attr, bin_len); /* resize if needed */
        if (bin_len > attr.size) { /* avoid overflow */
            be_raise(vm, "memory_error", "cannot allocate buffer");
        }

        int32_t bin_len_final = decode_base64((unsigned char*)s, attr.bufptr);  /* decode */
        attr.len = bin_len_final;
        be_pop(vm, 1); /* remove arg to leave instance */
        m_write_attributes(vm, 1, &attr);  /* update instance */
        be_return(vm);
    }
    be_raise(vm, "type_error", "operand must be a string");
    be_return_nil(vm);
}

/*
 * Converts hex to bytes()
 * 
 * `bytes().fromhexx() -> bytes()`
 */
static int m_fromhex(bvm *vm)
{
    int argc = be_top(vm);
    if (argc >= 2 && be_isstring(vm, 2)) {
        int32_t from = 0;                       // skip x chars
        if (argc >= 3 && be_isint(vm, 3)) {
            from = be_toint(vm, 3);
        }
        const char *s = be_tostring(vm, 2);
        int32_t s_len = strlen(s);
        if (from < 0) { from = 0; }
        if (from > s_len) { from = s_len; }
        int32_t bin_len = (s_len - from) / 2;

        buf_impl attr = m_read_attributes(vm, 1);
        check_ptr_modifiable(vm, &attr);
        if (attr.fixed && attr.len != bin_len) {
            be_raise(vm, BYTES_RESIZE_ERROR, BYTES_RESIZE_MESSAGE);
        }
        bytes_resize(vm, &attr, bin_len); /* resize if needed */
        if (bin_len > attr.size) { /* avoid overflow */
            be_raise(vm, "memory_error", "cannot allocate buffer");
        }
        attr.len = 0;
        buf_add_hex(&attr, s + from, s_len - from);
        
        be_pop(vm, 1); /* remove arg to leave instance */
        m_write_attributes(vm, 1, &attr);  /* update instance */
        be_pop(vm, be_top(vm) - 1);     /* leave instance on stack */
        be_return(vm);
    }
    be_raise(vm, "type_error", "operand must be a string");
    be_return_nil(vm);
}

/*
 * Advanced API
 */

/*
 * Retrieve the memory address of the raw buffer
 * to be used in C functions.
 * 
 * Note: the address is guaranteed not to move unless you
 * resize the buffer
 * 
 * `_buffer() -> comptr`
 */
static int m_buffer(bvm *vm)
{
    buf_impl attr = m_read_attributes(vm, 1);
    be_pushcomptr(vm, attr.bufptr);
    be_return(vm);
}

/*
 * Returns `btrue` if the buffer is mapped to memory
 * or `bfalse` if memory was allocated by us.
 * 
 * `ismapped() -> bool`
 */
static int m_is_mapped(bvm *vm)
{
    buf_impl attr = m_read_attributes(vm, 1);
    bbool mapped = (attr.mapped || (attr.bufptr == NULL));
    be_pushbool(vm, mapped);
    be_return(vm);
}

/*
 * Returns `btrue` if the buffer is solidified and read only
 * 
 * `isreadonly() -> bool`
 */
static int m_is_readonly(bvm *vm)
{
    buf_impl attr = m_read_attributes(vm, 1);
    be_pushbool(vm, attr.solidified);
    be_return(vm);
}

/*
 * Change the pointer to a mapped buffer.
 * 
 * This call does nothing if the buffer is not mapped (i.e. memory is managed externally)
 * 
 * It is typically used to reuse existing Berry object and avoid a complete reallocation
 * 
 * `_change_buffer(comptr) -> comptr`
 */
static int m_change_buffer(bvm *vm)
{
    int argc = be_top(vm);
    if (argc >= 2 && be_iscomptr(vm, 2)) {
        buf_impl attr = m_read_attributes(vm, 1);
        if (attr.solidified) {
            be_raise(vm, "value_error", BYTES_READ_ONLY_MESSAGE);
        }
        if (!attr.mapped) {
            be_raise(vm, "type_error", "bytes() object must be mapped");
        }
        attr.bufptr = be_tocomptr(vm, 2);
        m_write_attributes(vm, 1, &attr);   /* write attributes back to instance */
        be_pushcomptr(vm, attr.bufptr);
        be_return(vm);
    }
    be_raise(vm, "type_error", "operand must be a comptr");
    be_return_nil(vm);
}

/*
 * External API
 */
BERRY_API void * be_pushbytes(bvm *vm, const void * bytes, size_t len)
{
    bytes_new_object(vm, len);
    buf_impl attr = m_read_attributes(vm, -1);
    check_ptr(vm, &attr);
    if ((int32_t)len > attr.size) { len = attr.size; } /* double check if the buffer allocated was smaller */
    if (bytes) {  /* if bytes is null, buffer is filled with zeros */
        memmove((void*)attr.bufptr, bytes, len);
    } else {
        memset((void*)attr.bufptr, 0, len);
    }
    attr.len = len;
    m_write_attributes(vm, -1, &attr);  /* update instance */
    /* bytes instance is on top of stack */
    return (void*)attr.bufptr;
}

BERRY_API const void *be_tobytes(bvm *vm, int rel_index, size_t *len)
{
    int index = be_absindex(vm, rel_index);
    if (be_isbytes(vm, index)) {
        buf_impl attr = m_read_attributes(vm, index);
        check_ptr(vm, &attr);
        if (len) { *len = attr.len; }
        return (void*) attr.bufptr;
    }
    if (len) { *len = 0; }
    return NULL;
}

BERRY_API bbool be_isbytes(bvm *vm, int rel_index)
{
    bbool ret = bfalse;
    int index = be_absindex(vm, rel_index);
    if (be_isinstance(vm, index)) {
        be_getbuiltin(vm, "bytes");
        if (be_isderived(vm, index)) {
            ret = btrue;
        }
        be_pop(vm, 1);
    }
    return ret;
}

/* Helper code to compile bytecode


class Bytes : bytes
#-------------------------------------------------------------
#- 'getbits' function
#-
#- Reads a bit-field in a `bytes()` object
#-
#- Input:
#-   offset_bits  (int): bit number to start reading from (0 = LSB)
#-   len_bits     (int): how many bits to read
#- Output:
#-   valuer (int)
#-------------------------------------------------------------#
  def getbits(offset_bits, len_bits)
    if len_bits <= 0 || len_bits > 32 raise "value_error", "length in bits must be between 0 and 32" end
    var ret = 0
  
    var offset_bytes = offset_bits >> 3
    offset_bits = offset_bits % 8

    var bit_shift = 0                   #- bit number to write to -#
  
    while (len_bits > 0)
      var block_bits = 8 - offset_bits    # how many bits to read in the current block (block = byte) -#
      if block_bits > len_bits  block_bits = len_bits end
  
      var mask = ( (1<<block_bits) - 1) << offset_bits
      ret = ret | ( ((self[offset_bytes] & mask) >> offset_bits) << bit_shift)
  
      #- move the input window -#
      bit_shift += block_bits
      len_bits -= block_bits
      offset_bits = 0                   #- start at full next byte -#
      offset_bytes += 1
    end
  
    return ret
  end
  
  #-------------------------------------------------------------
  #- 'setbits' function
  #-
  #- Writes a bit-field in a `bytes()` object
  #-
  #- Input:
  #-   offset_bits  (int): bit number to start writing to (0 = LSB)
  #-   len_bits     (int): how many bits to write
  #-   val          (int): value to set
  #-------------------------------------------------------------#
  def setbits(offset_bits, len_bits, val)
    if len_bits < 0 || len_bits > 32 raise "value_error", "length in bits must be between 0 and 32" end

    val = int(val)      #- convert bool or others to int -#
    var offset_bytes = offset_bits >> 3
    offset_bits = offset_bits % 8
  
    while (len_bits > 0)
      var block_bits = 8 - offset_bits    #- how many bits to write in the current block (block = byte) -#
      if block_bits > len_bits  block_bits = len_bits end
  
      var mask_val = (1<<block_bits) - 1  #- mask to the n bits to get for this block -#
      var mask_b_inv = 0xFF - (mask_val << offset_bits)
      self[offset_bytes] = (self[offset_bytes] & mask_b_inv) | ((val & mask_val) << offset_bits)
  
      #- move the input window -#
      val >>= block_bits
      len_bits -= block_bits
      offset_bits = 0                   #- start at full next byte -#
      offset_bytes += 1
    end
    return self
  end
end

*/

/********************************************************************
** Solidified function: getbits
********************************************************************/
be_local_closure(getbits,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str(value_error),
    /* K2   */  be_nested_str(length_X20in_X20bits_X20must_X20be_X20between_X200_X20and_X2032),
    /* K3   */  be_const_int(3),
    /* K4   */  be_const_int(1),
    }),
    &be_const_str_getbits,
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
      0x180C0500,  //  0000  LE R3 R2 K0
      0x740E0002,  //  0001  JMPT R3 #0005
      0x540E001F,  //  0002  LDINT R3 32
      0x240C0403,  //  0003  GT R3 R2 R3
      0x780E0000,  //  0004  JMPF R3 #0006
      0xB0060302,  //  0005  RAISE 1 K1 K2
      0x580C0000,  //  0006  LDCONST R3 K0
      0x3C100303,  //  0007  SHR R4 R1 K3
      0x54160007,  //  0008  LDINT R5 8
      0x10040205,  //  0009  MOD R1 R1 R5
      0x58140000,  //  000A  LDCONST R5 K0
      0x24180500,  //  000B  GT R6 R2 K0
      0x781A0011,  //  000C  JMPF R6 #001F
      0x541A0007,  //  000D  LDINT R6 8
      0x04180C01,  //  000E  SUB R6 R6 R1
      0x241C0C02,  //  000F  GT R7 R6 R2
      0x781E0000,  //  0010  JMPF R7 #0012
      0x5C180400,  //  0011  MOVE R6 R2
      0x381E0806,  //  0012  SHL R7 K4 R6
      0x041C0F04,  //  0013  SUB R7 R7 K4
      0x381C0E01,  //  0014  SHL R7 R7 R1
      0x94200004,  //  0015  GETIDX R8 R0 R4
      0x2C201007,  //  0016  AND R8 R8 R7
      0x3C201001,  //  0017  SHR R8 R8 R1
      0x38201005,  //  0018  SHL R8 R8 R5
      0x300C0608,  //  0019  OR R3 R3 R8
      0x00140A06,  //  001A  ADD R5 R5 R6
      0x04080406,  //  001B  SUB R2 R2 R6
      0x58040000,  //  001C  LDCONST R1 K0
      0x00100904,  //  001D  ADD R4 R4 K4
      0x7001FFEB,  //  001E  JMP  #000B
      0x80040600,  //  001F  RET 1 R3
    })
  )
);
/*******************************************************************/

/********************************************************************
** Solidified function: setbits
********************************************************************/
be_local_closure(setbits,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    4,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str(value_error),
    /* K2   */  be_nested_str(length_X20in_X20bits_X20must_X20be_X20between_X200_X20and_X2032),
    /* K3   */  be_const_int(3),
    /* K4   */  be_const_int(1),
    }),
    &be_const_str_setbits,
    &be_const_str_solidified,
    ( &(const binstruction[37]) {  /* code */
      0x14100500,  //  0000  LT R4 R2 K0
      0x74120002,  //  0001  JMPT R4 #0005
      0x5412001F,  //  0002  LDINT R4 32
      0x24100404,  //  0003  GT R4 R2 R4
      0x78120000,  //  0004  JMPF R4 #0006
      0xB0060302,  //  0005  RAISE 1 K1 K2
      0x60100009,  //  0006  GETGBL R4 G9
      0x5C140600,  //  0007  MOVE R5 R3
      0x7C100200,  //  0008  CALL R4 1
      0x5C0C0800,  //  0009  MOVE R3 R4
      0x3C100303,  //  000A  SHR R4 R1 K3
      0x54160007,  //  000B  LDINT R5 8
      0x10040205,  //  000C  MOD R1 R1 R5
      0x24140500,  //  000D  GT R5 R2 K0
      0x78160014,  //  000E  JMPF R5 #0024
      0x54160007,  //  000F  LDINT R5 8
      0x04140A01,  //  0010  SUB R5 R5 R1
      0x24180A02,  //  0011  GT R6 R5 R2
      0x781A0000,  //  0012  JMPF R6 #0014
      0x5C140400,  //  0013  MOVE R5 R2
      0x381A0805,  //  0014  SHL R6 K4 R5
      0x04180D04,  //  0015  SUB R6 R6 K4
      0x541E00FE,  //  0016  LDINT R7 255
      0x38200C01,  //  0017  SHL R8 R6 R1
      0x041C0E08,  //  0018  SUB R7 R7 R8
      0x94200004,  //  0019  GETIDX R8 R0 R4
      0x2C201007,  //  001A  AND R8 R8 R7
      0x2C240606,  //  001B  AND R9 R3 R6
      0x38241201,  //  001C  SHL R9 R9 R1
      0x30201009,  //  001D  OR R8 R8 R9
      0x98000808,  //  001E  SETIDX R0 R4 R8
      0x3C0C0605,  //  001F  SHR R3 R3 R5
      0x04080405,  //  0020  SUB R2 R2 R5
      0x58040000,  //  0021  LDCONST R1 K0
      0x00100904,  //  0022  ADD R4 R4 K4
      0x7001FFE8,  //  0023  JMP  #000D
      0x80040000,  //  0024  RET 1 R0
    })
  )
);
/*******************************************************************/

#if !BE_USE_PRECOMPILED_OBJECT
void be_load_byteslib(bvm *vm)
{
    static const bnfuncinfo members[] = {
        { ".p", NULL },
        { ".len", NULL },
        { ".size", NULL },
        { "_buffer", m_buffer },
        { "_change_buffer", m_change_buffer },
        { "ismapped", m_is_mapped },
        { "isreadonly", m_is_readonly },
        { "init", m_init },
        { "deinit", m_deinit },
        { "tostring", m_tostring },
        { "asstring", m_asstring },
        { "tobool", m_tobool },
        { "fromstring", m_fromstring },
        { "tob64", m_tob64 },
        { "fromb64", m_fromb64 },
        { "fromhex", m_fromhex },
        { "tohex", m_tohex },
        { "add", m_add },
        { "get", m_getu },
        { "geti", m_geti },
        { "set", m_set },
        { "seti", m_set },      // setters for signed and unsigned are identical
        { "setbytes", m_setbytes },
        { "getfloat", m_getfloat },
        { "setfloat", m_setfloat },
        { "addfloat", m_addfloat },
        { "item", m_item },
        { "setitem", m_setitem },
        { "size", m_size },
        { "resize", m_resize },
        { "clear", m_clear },
        { "reverse", m_reverse },
        { "copy", m_copy },
        { "append", m_connect },
        { "+", m_merge },
        { "..", m_connect },
        { "==", m_equal },
        { "!=", m_nequal },

        { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */
        { "getbits", (bntvfunc) &getbits_closure },
        { "setbits", (bntvfunc) &setbits_closure },

        { NULL, NULL }
    };
    be_regclass(vm, "bytes", members);
}
#else

#include "../generate/be_const_bytes_def.h"

/* @const_object_info_begin
class be_class_bytes (scope: global, name: bytes) {
    .p, var
    .len, var
    .size, var
    _buffer, func(m_buffer)
    _change_buffer, func(m_change_buffer)
    ismapped, func(m_is_mapped)
    isreadonly, func(m_is_readonly)
    init, func(m_init)
    deinit, func(m_deinit)
    tostring, func(m_tostring)
    asstring, func(m_asstring)
    tobool, func(m_tobool)
    fromstring, func(m_fromstring)
    tob64, func(m_tob64)
    fromb64, func(m_fromb64)
    fromhex, func(m_fromhex)
    tohex, func(m_tohex)
    add, func(m_add)
    get, func(m_getu)
    geti, func(m_geti)
    getfloat, func(m_getfloat)
    setfloat, func(m_setfloat)
    addfloat, func(m_addfloat)
    set, func(m_set)
    seti, func(m_set)
    setbytes, func(m_setbytes)
    item, func(m_item)
    setitem, func(m_setitem)
    size, func(m_size)
    resize, func(m_resize)
    clear, func(m_clear)
    reverse, func(m_reverse)
    copy, func(m_copy)
    append, func(m_connect)
    +, func(m_merge)
    .., func(m_connect)
    ==, func(m_equal)
    !=, func(m_nequal)

    getbits, closure(getbits_closure)
    setbits, closure(setbits_closure)
}
@const_object_info_end */
#include "../generate/be_fixed_be_class_bytes.h"
#endif
