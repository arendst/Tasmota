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

#define BYTES_DEFAULT_SIZE          28              // default pre-reserved size for buffer (keep 4 bytes for len/size)
#define BYTES_MAX_SIZE              (32*1024)       // max 32Kb
#define BYTES_OVERHEAD              4               // bytes overhead to be added when allocating (used to store len and size)
#define BYTES_HEADROOM              8               // keep a natural headroom of 8 bytes when resizing

typedef struct buf_impl {
  uint16_t size;                // size in bytes of the buffer
  uint16_t len;                 // current size of the data in buffer. Invariant: len <= size
  uint8_t buf[];                // the actual data
} buf_impl;

/********************************************************************
** Buffer low-level implementation
**
** Extracted from Tasmota SBuffer lib
********************************************************************/
static inline uint8_t* buf_get_buf(buf_impl* buf)
{
    return &buf->buf[0];
}

// shrink or increase. If increase, fill with zeores. Cannot go beyond `size`
static void buf_set_len(buf_impl* buf, const size_t len)
{
    uint16_t old_len = buf->len;
    buf->len = (len <= buf->size) ? len : buf->size;
    if (old_len < buf->len) {
        memset((void*) &buf->buf[old_len], 0, buf->len - old_len);
    }
}

static size_t buf_add1(buf_impl* buf, const uint8_t data) // append 8 bits value
{
    if (buf->len < buf->size) {       // do we have room for 1 byte
        buf->buf[buf->len++] = data;
    }
    return buf->len;
}

static size_t buf_add2_le(buf_impl* buf, const uint16_t data) // append 16 bits value
{
    if (buf->len < buf->size - 1) {    // do we have room for 2 bytes
        buf->buf[buf->len++] = data;
        buf->buf[buf->len++] = data >> 8;
    }
    return buf->len;
}

static size_t buf_add2_be(buf_impl* buf, const uint16_t data) // append 16 bits value
{
    if (buf->len < buf->size - 1) {    // do we have room for 2 bytes
        buf->buf[buf->len++] = data >> 8;
        buf->buf[buf->len++] = data;
    }
    return buf->len;
}

static size_t buf_add4_le(buf_impl* buf, const uint32_t data) // append 32 bits value
{
    if (buf->len < buf->size - 3) {     // do we have room for 4 bytes
        buf->buf[buf->len++] = data;
        buf->buf[buf->len++] = data >> 8;
        buf->buf[buf->len++] = data >> 16;
        buf->buf[buf->len++] = data >> 24;
    }
    return buf->len;
}

size_t buf_add4_be(buf_impl* buf, const uint32_t data) // append 32 bits value
{
    if (buf->len < buf->size - 3) {     // do we have room for 4 bytes
        buf->buf[buf->len++] = data >> 24;
        buf->buf[buf->len++] = data >> 16;
        buf->buf[buf->len++] = data >> 8;
        buf->buf[buf->len++] = data;
    }
    return buf->len;
}

static size_t buf_add_buf(buf_impl* buf, buf_impl* buf2)
{
    if (buf->len + buf2->len <= buf->size) {
        for (uint32_t i = 0; i < buf2->len; i++) {
            buf->buf[buf->len++] = buf2->buf[i];
        }
    }
    return buf->len;
}

static uint8_t buf_get1(buf_impl* buf, int offset)
{
    if ((offset >= 0) && (offset < buf->len)) {
        return buf->buf[offset];
    }
    return 0;
}

static void buf_set1(buf_impl* buf, const size_t offset, const uint8_t data)
{
    if (offset < buf->len) {
        buf->buf[offset] = data;
    }
}

static void buf_set2_le(buf_impl* buf, const size_t offset, const uint16_t data)
{
    if ((offset >= 0) && (offset < buf->len - 1)) {
        buf->buf[offset] = data & 0xFF;
        buf->buf[offset+1] = data >> 8;
    }
}

static void buf_set2_be(buf_impl* buf, const size_t offset, const uint16_t data)
{
    if ((offset >= 0) && (offset < buf->len - 1)) {
        buf->buf[offset+1] = data & 0xFF;
        buf->buf[offset] = data >> 8;
    }
}

static uint16_t buf_get2_le(buf_impl* buf, int offset)
{
    if ((offset >= 0) && (offset < buf->len - 1)) {
        return buf->buf[offset] | (buf->buf[offset+1] << 8);
    }
    return 0;
}

static uint16_t buf_get2_be(buf_impl* buf, int offset)
{
    if (offset < buf->len - 1) {
        return buf->buf[offset+1] | (buf->buf[offset] << 8);
    }
    return 0;
}

static void buf_set4_le(buf_impl* buf, const size_t offset, const uint32_t data)
{
    if ((offset >= 0) && (offset < buf->len - 3)) {
        buf->buf[offset] = data & 0xFF;
        buf->buf[offset+1] = (data >> 8) & 0xFF;
        buf->buf[offset+2] = (data >> 16) & 0xFF;
        buf->buf[offset+3] = data >> 24;
    }
}

static void buf_set4_be(buf_impl* buf, const size_t offset, const uint32_t data)
{
    if ((offset >= 0) && (offset < buf->len - 3)) {
        buf->buf[offset+3] = data & 0xFF;
        buf->buf[offset+2] = (data >> 8) & 0xFF;
        buf->buf[offset+1] = (data >> 16) & 0xFF;
        buf->buf[offset] = data >> 24;
    }
}

static uint32_t buf_get4_le(buf_impl* buf, int offset)
{
    if ((offset >= 0) && (offset < buf->len - 3)) {
        return buf->buf[offset] | (buf->buf[offset+1] << 8) |
            (buf->buf[offset+2] << 16) | (buf->buf[offset+3] << 24);
    }
    return 0;
}

static uint32_t buf_get4_be(buf_impl* buf, int offset)
{
    if (offset < buf->len - 3) {
        return buf->buf[offset+3] | (buf->buf[offset+2] << 8) |
            (buf->buf[offset+1] << 16) | (buf->buf[offset] << 24);
    }
    return 0;
}

// nullptr accepted
static bbool buf_equals(buf_impl* buf1, buf_impl* buf2)
{
    if (buf1 == buf2) { return btrue; }
    if (!buf1 || !buf2) { return bfalse; }   // at least one buf is not empty
    // we know that both buf1 and buf2 are non-null
    if (buf1->len != buf2->len) { return bfalse; }
    size_t len = buf1->len;
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
static void buf_add_hex(buf_impl* buf, const char *hex, size_t len)
{
    uint8_t val;
    for (; len > 1; len -= 2) {
        val = asc2byte(*hex++) << 4;
        val |= asc2byte(*hex++);
        buf_add1(buf, val);
    }
}

/********************************************************************
** Wrapping into lib
********************************************************************/
// typedef int (*bntvfunc)(bvm*); /* native function pointer */
int free_bytes_buf(bvm* vm)
{
    int argc = be_top(vm);
    if (argc > 0) {
        buf_impl * buf = (buf_impl*) be_tocomptr(vm, 1);
        if (buf != NULL) {
            be_os_free(buf);
        }
    }
    be_return_nil(vm);
}

buf_impl * bytes_alloc(int32_t size)
{
    if (size < 4) { size = 4; }
    if (size > BYTES_MAX_SIZE) { size = BYTES_MAX_SIZE; }
    buf_impl * next = (buf_impl*) be_os_malloc(size + BYTES_OVERHEAD);
    next->size = size;
    next->len = 0;
    return next;
}

/* allocate a new `bytes` object with pre-allocated size */
static void bytes_new_object(bvm *vm, size_t size)
{
    be_getglobal(vm, "bytes"); /* eventually change with be_getbuiltin */
    be_call(vm, 0); /* stack has only instance */
    be_getmember(vm, -1, "init");
    be_pushvalue(vm, -2);
    be_pushint(vm, size); /* stack: instance, init func, instance, size */
    be_call(vm, 2); /* stack: instance, ret, instance, size */
    be_pop(vm, 3); /* remove ret, instance, size */
}

static int m_init(bvm *vm)
{
    int argc = be_top(vm);
    int size = BYTES_DEFAULT_SIZE;
    const char * hex_in = NULL;
    if (argc > 1 && be_isint(vm, 2)) {
        int new_size = be_toint(vm, 2) + BYTES_HEADROOM;
        if (new_size > size) {
            size = new_size;
        }
    } else if (argc > 1 && be_isstring(vm, 2)) {
        hex_in = be_tostring(vm, 2);
        if (hex_in) {
            size = strlen(hex_in) / 2 + BYTES_HEADROOM;        // allocate headroom
        }
    }
    buf_impl * buf = bytes_alloc(size);
    if (!buf) {
        be_throw(vm, BE_MALLOC_FAIL);
    }
    
    if (hex_in) {
        buf_add_hex(buf, hex_in, strlen(hex_in));
    } 
    be_newcomobj(vm, buf, &free_bytes_buf);
    be_setmember(vm, 1, ".p");
    be_return_nil(vm);
}

/* grow or shrink to the exact value */
/* stack item 1 must contain the instance */
static buf_impl * _bytes_resize(bvm *vm, buf_impl * buf, size_t new_size) {
    buf_impl * new_buf = bytes_alloc(new_size);
    if (!new_buf) {
        be_throw(vm, BE_MALLOC_FAIL);
    }
    memmove(buf_get_buf(new_buf), buf_get_buf(buf), buf->len);
    new_buf->len = buf->len;
    /* replace the .p attribute */
    be_newcomobj(vm, new_buf, &free_bytes_buf);
    be_setmember(vm, 1, ".p");
    be_pop(vm, 1); /* remove comobj from stack */
    /* the old buffer will be garbage collected later */
    return new_buf;
}

/* grow if needed but don't shrink */
/* if grow, then add some headroom */
/* stack item 1 must contain the instance */
static buf_impl * bytes_resize(bvm *vm, buf_impl * buf, size_t new_size) {
    if (buf->size >= new_size) { return buf; }  /* no resize needed */
    return _bytes_resize(vm, buf, new_size + BYTES_HEADROOM);
}

static buf_impl * bytes_check_data(bvm *vm, size_t add_size) {
    be_getmember(vm, 1, ".p");
    buf_impl * buf = be_tocomptr(vm, -1);
    be_pop(vm, 1); /* remove member from stack */
    /* check if the `size` is big enough */
    if (buf->len + add_size > buf->size) {
        /* it does not fit so we need to realocate the buffer */
        buf = bytes_resize(vm, buf, buf->len + add_size);
    }
    return buf;
}

static size_t tohex(char * out, size_t outsz, const uint8_t * in, size_t insz) {
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
    buf_impl * buf = bytes_check_data(vm, 0);
    size_t len = buf->len;
    size_t hex_len = len * 2 + 5 + 2 + 2 + 1;  /* reserve size for `bytes("")\0` - 9 chars */

    char * hex_out = be_pushbuffer(vm, hex_len);
    size_t l = be_strlcpy(hex_out, "bytes('", hex_len);
    l += tohex(&hex_out[l], hex_len - l, buf_get_buf(buf), buf->len);
    l += be_strlcpy(&hex_out[l], "')", hex_len - l);

    be_pushnstring(vm, hex_out, l); /* make escape string from buffer */
    be_remove(vm, -2); /* remove buffer */
    be_return(vm);
}

/*
 * Copy the buffer into a string without any changes
 */
static int m_asstring(bvm *vm)
{
    buf_impl * buf = bytes_check_data(vm, 0);
    be_pushnstring(vm, (const char*) buf_get_buf(buf), buf->len);
    be_return(vm);
}

static int m_fromstring(bvm *vm)
{
    int argc = be_top(vm);
    if (argc >= 2 && be_isstring(vm, 2)) {
        const char *s = be_tostring(vm, 2);
        size_t len = strlen(s);
        buf_impl * buf = bytes_check_data(vm, 0);
        buf = bytes_resize(vm, buf, len); /* resize if needed */
        if (len > buf->size) { len = buf->size; } /* avoid overflow */
        memmove(buf_get_buf(buf), s, len);
        buf->len = len;
        be_pop(vm, 1); /* remove arg to leave instance */
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
    buf_impl * buf = bytes_check_data(vm, 4); /* we reserve 4 bytes anyways */
    if (argc >= 2 && be_isint(vm, 2)) {
        int32_t v = be_toint(vm, 2);
        int vsize = 1;
        if (argc >= 3 && be_isint(vm, 3)) {
            vsize = be_toint(vm, 3);
        }
        switch (vsize) {
            case 0:                             break;
            case -1:    /* fallback below */
            case 1:     buf_add1(buf, v);       break;
            case 2:     buf_add2_le(buf, v);    break;
            case 4:     buf_add4_le(buf, v);    break;
            case -2:    buf_add2_be(buf, v);    break;
            case -4:    buf_add4_be(buf, v);    break;
            default:    be_raise(vm, "type_error", "size must be -4, -2, -1, 0, 1, 2 or 4.");
        }
        be_pop(vm, argc - 1);
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
    buf_impl * buf = bytes_check_data(vm, 0); /* we reserve 4 bytes anyways */
    if (argc >=2 && be_isint(vm, 2)) {
        int32_t idx = be_toint(vm, 2);
        int vsize = 1;
        if (argc >= 3 && be_isint(vm, 3)) {
            vsize = be_toint(vm, 3);
        }
        int ret = 0;
        switch (vsize) {
            case 0:                                     break;
            case -1:    /* fallback below */
            case 1:     ret = buf_get1(buf, idx);
                        if (sign) { ret = (int8_t)(uint8_t) ret; }
                        break;
            case 2:     ret = buf_get2_le(buf, idx);
                        if (sign) { ret = (int16_t)(uint16_t) ret; }
                        break;
            case 4:     ret = buf_get4_le(buf, idx);    break;
            case -2:    ret = buf_get2_be(buf, idx);
                        if (sign) { ret = (int16_t)(uint16_t) ret; }
                        break;
            case -4:    ret = buf_get4_be(buf, idx);    break;
            default:    be_raise(vm, "type_error", "size must be -4, -2, -1, 0, 1, 2 or 4.");
        }
        be_pop(vm, argc - 1);
        if (vsize != 0) {
            be_pushint(vm, ret);
        } else {
            be_pushnil(vm);
        }
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
    buf_impl * buf = bytes_check_data(vm, 0); /* we reserve 4 bytes anyways */
    if (argc >=3 && be_isint(vm, 2) && be_isint(vm, 3)) {
        int32_t idx = be_toint(vm, 2);
        int32_t value = be_toint(vm, 3);
        int vsize = 1;
        if (argc >= 4 && be_isint(vm, 4)) {
            vsize = be_toint(vm, 4);
        }
        switch (vsize) {
            case 0:                                     break;
            case -1:    /* fallback below */
            case 1:     buf_set1(buf, idx, value);      break;
            case 2:     buf_set2_le(buf, idx, value);   break;
            case 4:     buf_set4_le(buf, idx, value);   break;
            case -2:    buf_set2_be(buf, idx, value);   break;
            case -4:    buf_set4_be(buf, idx, value);   break;
            default:    be_raise(vm, "type_error", "size must be -4, -2, -1, 0, 1, 2 or 4.");
        }
        be_pop(vm, argc - 1);
        be_return_nil(vm);
    }
    be_return_nil(vm);
}

static int m_setitem(bvm *vm)
{
    int argc = be_top(vm);
    buf_impl * buf = bytes_check_data(vm, 0); /* we reserve 4 bytes anyways */
    if (argc >=3 && be_isint(vm, 2) && be_isint(vm, 3)) {
        int index = be_toint(vm, 2);
        int val = be_toint(vm, 3);
        if (index >= 0 && index < buf->len) {
            buf_set1(buf, index, val);
            be_return_nil(vm);
        }
    }
    be_raise(vm, "index_error", "bytes index out of range or value non int");
    be_return_nil(vm);
}

static int m_item(bvm *vm)
{
    int argc = be_top(vm);
    buf_impl * buf = bytes_check_data(vm, 0); /* we reserve 4 bytes anyways */
    if (argc >=2 && be_isint(vm, 2)) {
        int index = be_toint(vm,2);
        if (index >= 0 && index < buf->len) {
            be_pushint(vm, buf_get1(buf, index));
            be_return(vm);
        }
    }
    if (argc >= 2 && be_isinstance(vm, 2)) {
        const char *cname = be_classname(vm, 2);
        if (!strcmp(cname, "range")) {
            bint lower, upper;
            bint size = buf->len;
            /* get index range */
            be_getmember(vm, 2, "__lower__");
            lower = be_toint(vm, -1);
            be_pop(vm, 1);
            be_getmember(vm, 2, "__upper__");
            upper = be_toint(vm, -1);
            be_pop(vm, 1);
            /* protection scope */
            upper = upper < size ? upper : size - 1;
            lower = lower < 0 ? 0 : lower;
            /* construction result list instance */
            bytes_new_object(vm, upper > lower ? upper-lower : 0);
            be_getmember(vm, -1, ".p");
            buf_impl * buf2 = be_tocomptr(vm, -1);
            be_pop(vm, 1);  /* remove .p and leave bytes instance */
            for (; lower <= upper; ++lower) {
                buf_add1(buf2, buf->buf[lower]);
            }
            be_return(vm);    
        }
    }
    be_raise(vm, "index_error", "bytes index out of range");
    be_return_nil(vm);
}

static int m_size(bvm *vm)
{
    buf_impl * buf = bytes_check_data(vm, 0);
    be_pushint(vm, buf->len);
    be_return(vm);
}

static int m_resize(bvm *vm)
{
    int argc = be_top(vm);
    be_getmember(vm, 1, ".p");
    buf_impl * buf = be_tocomptr(vm, -1);
    be_pop(vm, 1);

    if (argc <= 1 || !be_isint(vm, 2)) {
        be_raise(vm, "type_error", "size must be of type 'int'");
    }
    int new_len = be_toint(vm, 2);
    if (new_len < 0) {
        new_len = 0;
    }

    buf = bytes_resize(vm, buf, new_len);
    buf_set_len(buf, new_len);
    be_pop(vm, 1);
    be_return(vm);
}

static int m_clear(bvm *vm)
{
    buf_impl * buf = bytes_check_data(vm, 0);
    buf->len = 0;
    be_return_nil(vm);
}

static int m_merge(bvm *vm)
{
    int argc = be_top(vm);
    buf_impl * buf1 = bytes_check_data(vm, 0); /* no resize yet */
    if (argc >= 2 && be_isinstance(vm, 2)) {
        be_getglobal(vm, "bytes"); /* get the bytes class */ /* TODO eventually replace with be_getbuiltin */
        if (be_isderived(vm, 2)) {
            be_getmember(vm, 2, ".p");
            buf_impl * buf2 = be_tocomptr(vm, -1);
            be_pop(vm, 4); /* remove class, member, and 2 operands */

            /* allocate new object */
            bytes_new_object(vm, buf1->len + buf2->len);
            be_getmember(vm, -1, ".p");
            /* .p is on top of stack, then instance */
            buf_impl * buf3 = be_tocomptr(vm, -1);
            be_pop(vm, 1);
            buf_add_buf(buf3, buf1);
            buf_add_buf(buf3, buf2);

            be_return(vm); /* return self */
        }
    }
    be_raise(vm, "type_error", "operand must be bytes");
    be_return_nil(vm); /* return self */
}

static int m_copy(bvm *vm)
{
    buf_impl * buf1 = bytes_check_data(vm, 0); /* no resize */
    bytes_new_object(vm, buf1->len);
    be_getmember(vm, -1, ".p");
    buf_impl * buf2 = be_tocomptr(vm, -1);
    be_pop(vm, 1);
    buf_add_buf(buf2, buf1);
    be_return(vm); /* return self */
}

/* accept bytes or int as operand */
static int m_connect(bvm *vm)
{
    int argc = be_top(vm);
    buf_impl * buf1 = bytes_check_data(vm, 0); /* don't resize yet */
    if (argc >= 2 && (be_isinstance(vm, 2) || be_isint(vm, 2))) {
        if (be_isint(vm, 2)) {
            buf1 = bytes_resize(vm, buf1, buf1->len + 1); /* resize */
            buf_add1(buf1, be_toint(vm, 2));
            be_pop(vm, 1);  /* remove operand */
            be_return(vm); /* return self */
        } else {
            be_getglobal(vm, "bytes"); /* get the bytes class */ /* TODO eventually replace with be_getbuiltin */
            if (be_isderived(vm, 2)) {
                be_getmember(vm, 2, ".p");
                buf_impl * buf2 = be_tocomptr(vm, -1);
                buf1 = bytes_resize(vm, buf1, buf1->len + buf2->len); /* resize buf1 for total size */
                buf_add_buf(buf1, buf2);
                be_pop(vm, 3); /* remove class, member, and last operand */
                be_return(vm); /* return self */
            }
        }
    }
    be_raise(vm, "type_error", "operand must be bytes or int");
    be_return_nil(vm); /* return self */
}

static int bytes_equal(bvm *vm, bbool iseq)
{
    be_getmember(vm, 1, ".p");
    buf_impl * buf1 = be_tocomptr(vm, -1);
    be_pop(vm, 1);

    be_getmember(vm, 2, ".p");
    buf_impl * buf2 = be_tocomptr(vm, -1);
    be_pop(vm, 1);

    bbool ret;
    if (buf_equals(buf1, buf2)) {
        ret = iseq;
    } else {
        ret = !iseq;
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
    buf_impl * buf = bytes_check_data(vm, 0);
    be_pushcomptr(vm, &buf->buf);
    be_return(vm);
}

/*
 * External API
 */
BERRY_API void be_pushbytes(bvm *vm, const void * bytes, size_t len)
{
    bytes_new_object(vm, len);
    be_getmember(vm, -1, ".p");
    buf_impl * buf = be_tocomptr(vm, -1);
    be_pop(vm, 1); /* remove .p1 and leave instance */
    if (len > buf->size) { len = buf->size; } /* double check if the buffer allocated was smaller */
    memmove((void*)buf_get_buf(buf), bytes, len);
    buf->len = len;
    /* bytes instance is on top of stack */
}

BERRY_API const void *be_tobytes(bvm *vm, int rel_index, size_t *len)
{
    int index = be_absindex(vm, rel_index);
    if (be_isinstance(vm, index)) {
        be_getglobal(vm, "bytes"); /* get the bytes class */ /* TODO eventually replace with be_getbuiltin */
        if (be_isderived(vm, index)) {
            be_getmember(vm, index, ".p");
            buf_impl * buf = be_tocomptr(vm, -1);
            be_pop(vm, 2); /* class and .p */
            if (len) { *len = buf->len; }
            return (void*) buf_get_buf(buf);
        } else {
            be_pop(vm, 1);  /* remove class */
        }
    }
    if (len) { *len = 0; }
    return NULL;
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
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
      { { .i=0 }, BE_INT},
      { { .s=be_nested_const_str("value_error", 773297791, 11) }, BE_STRING},
      { { .s=be_nested_const_str("length in bits must be between 0 and 32", -1710458168, 39) }, BE_STRING},
      { { .i=3 }, BE_INT},
      { { .i=1 }, BE_INT},
    }),
    (be_nested_const_str("getbits", -1200798317, 7)),
    (be_nested_const_str("stdin", -1529146723, 5)),
    ( &(const binstruction[32]) {  /* code */
      0x180C0500,  //  0000  LE R3  R2  R256
      0x740E0002,  //  0001  JMPT R3  #0005
      0x540E001F,  //  0002  LDINT  R3  32
      0x240C0403,  //  0003  GT R3  R2  R3
      0x780E0000,  //  0004  JMPF R3  #0006
      0xB0060302,  //  0005  RAISE  1 R257  R258
      0x580C0000,  //  0006  LDCONST  R3  K0
      0x3C100303,  //  0007  SHR  R4  R1  R259
      0x54160007,  //  0008  LDINT  R5  8
      0x10040205,  //  0009  MOD  R1  R1  R5
      0x58140000,  //  000A  LDCONST  R5  K0
      0x24180500,  //  000B  GT R6  R2  R256
      0x781A0011,  //  000C  JMPF R6  #001F
      0x541A0007,  //  000D  LDINT  R6  8
      0x04180C01,  //  000E  SUB  R6  R6  R1
      0x241C0C02,  //  000F  GT R7  R6  R2
      0x781E0000,  //  0010  JMPF R7  #0012
      0x5C180400,  //  0011  MOVE R6  R2
      0x381E0806,  //  0012  SHL  R7  R260  R6
      0x041C0F04,  //  0013  SUB  R7  R7  R260
      0x381C0E01,  //  0014  SHL  R7  R7  R1
      0x94200004,  //  0015  GETIDX R8  R0  R4
      0x2C201007,  //  0016  AND  R8  R8  R7
      0x3C201001,  //  0017  SHR  R8  R8  R1
      0x38201005,  //  0018  SHL  R8  R8  R5
      0x300C0608,  //  0019  OR R3  R3  R8
      0x00140A06,  //  001A  ADD  R5  R5  R6
      0x04080406,  //  001B  SUB  R2  R2  R6
      0x58040000,  //  001C  LDCONST  R1  K0
      0x00100904,  //  001D  ADD  R4  R4  R260
      0x7001FFEB,  //  001E  JMP    #000B
      0x80040600,  //  001F  RET  1 R3
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
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
      { { .i=0 }, BE_INT},
      { { .s=be_nested_const_str("value_error", 773297791, 11) }, BE_STRING},
      { { .s=be_nested_const_str("length in bits must be between 0 and 32", -1710458168, 39) }, BE_STRING},
      { { .i=3 }, BE_INT},
      { { .i=1 }, BE_INT},
    }),
    (be_nested_const_str("setbits", -1532559129, 7)),
    (be_nested_const_str("stdin", -1529146723, 5)),
    ( &(const binstruction[33]) {  /* code */
      0x14100500,  //  0000  LT R4  R2  R256
      0x74120002,  //  0001  JMPT R4  #0005
      0x5412001F,  //  0002  LDINT  R4  32
      0x24100404,  //  0003  GT R4  R2  R4
      0x78120000,  //  0004  JMPF R4  #0006
      0xB0060302,  //  0005  RAISE  1 R257  R258
      0x3C100303,  //  0006  SHR  R4  R1  R259
      0x54160007,  //  0007  LDINT  R5  8
      0x10040205,  //  0008  MOD  R1  R1  R5
      0x24140500,  //  0009  GT R5  R2  R256
      0x78160014,  //  000A  JMPF R5  #0020
      0x54160007,  //  000B  LDINT  R5  8
      0x04140A01,  //  000C  SUB  R5  R5  R1
      0x24180A02,  //  000D  GT R6  R5  R2
      0x781A0000,  //  000E  JMPF R6  #0010
      0x5C140400,  //  000F  MOVE R5  R2
      0x381A0805,  //  0010  SHL  R6  R260  R5
      0x04180D04,  //  0011  SUB  R6  R6  R260
      0x541E00FE,  //  0012  LDINT  R7  255
      0x38200C01,  //  0013  SHL  R8  R6  R1
      0x041C0E08,  //  0014  SUB  R7  R7  R8
      0x94200004,  //  0015  GETIDX R8  R0  R4
      0x2C201007,  //  0016  AND  R8  R8  R7
      0x2C240606,  //  0017  AND  R9  R3  R6
      0x38241201,  //  0018  SHL  R9  R9  R1
      0x30201009,  //  0019  OR R8  R8  R9
      0x98000808,  //  001A  SETIDX R0  R4  R8
      0x3C0C0605,  //  001B  SHR  R3  R3  R5
      0x04080405,  //  001C  SUB  R2  R2  R5
      0x58040000,  //  001D  LDCONST  R1  K0
      0x00100904,  //  001E  ADD  R4  R4  R260
      0x7001FFE8,  //  001F  JMP    #0009
      0x80040000,  //  0020  RET  1 R0
    })
  )
);
/*******************************************************************/

#if !BE_USE_PRECOMPILED_OBJECT
void be_load_byteslib(bvm *vm)
{
    static const bnfuncinfo members[] = {
        { ".p", NULL },
        { "_buffer", m_buffer },
        { "init", m_init },
        { "tostring", m_tostring },
        { "asstring", m_asstring },
        { "fromstring", m_fromstring },
        { "add", m_add },
        { "get", m_getu },
        { "geti", m_geti },
        { "set", m_set },
        { "seti", m_set },      // setters for signed and unsigned are identical
        { "item", m_item },
        { "setitem", m_setitem },
        { "size", m_size },
        { "resize", m_resize },
        { "clear", m_clear },
        { "copy", m_copy },
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
/* @const_object_info_begin
class be_class_bytes (scope: global, name: bytes) {
    .p, var
    _buffer, func(m_buffer)
    init, func(m_init)
    tostring, func(m_tostring)
    asstring, func(m_asstring)
    fromstring, func(m_fromstring)
    add, func(m_add)
    get, func(m_getu)
    geti, func(m_geti)
    set, func(m_set)
    seti, func(m_set)
    item, func(m_item)
    setitem, func(m_setitem)
    size, func(m_size)
    resize, func(m_resize)
    clear, func(m_clear)
    copy, func(m_copy)
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
