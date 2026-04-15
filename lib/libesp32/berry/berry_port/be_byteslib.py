"""
berry_port/be_byteslib.py — Port of src/be_byteslib.c

Bytes class: buf_impl low-level buffer operations, base64 encode/decode,
bytes class methods (init, deinit, tostring, tohex, asstring, fromstring,
add, get/geti/getu, set, size, resize, clear, copy, setitem, item, tobool,
iter, merge, connect, reverse, equal, nequal, tob64, fromb64, fromhex,
setbytes, getfloat, setfloat, addfloat, appendhex, appendb64, buffer,
is_mapped, is_readonly, change_buffer).

Original C code is included as comments.
"""

# /********************************************************************
# ** Copyright (c) 2018-2020 Guan Wenliang - Stephan Hadinger
# ** This file is part of the Berry default interpreter.
# ** skiars@qq.com, https://github.com/Skiars/berry
# ** See Copyright Notice in the LICENSE file or at
# ** https://github.com/Skiars/berry/blob/master/LICENSE
# ********************************************************************/

import struct

# ============================================================================
# Imports — lazy to avoid circular dependencies
# ============================================================================

def _lazy_be_api():
    import berry_port.be_api as m; return m

def _lazy_be_object():
    import berry_port.be_object as m; return m

def _lazy_be_mem():
    import berry_port.be_mem as m; return m

def _lazy_be_exec():
    import berry_port.be_exec as m; return m

def _lazy_be_strlib():
    import berry_port.be_strlib as m; return m

# ============================================================================
# Constants from be_byteslib.h
# ============================================================================

BYTES_DEFAULT_SIZE      = 28
BYTES_OVERHEAD          = 4
BYTES_HEADROOM          = 8

BYTES_SIZE_FIXED        = -1
BYTES_SIZE_MAPPED       = -2
BYTES_SIZE_SOLIDIFIED   = -3

BYTES_RESIZE_ERROR      = "attribute_error"
BYTES_RESIZE_MESSAGE    = "bytes object size is fixed and cannot be resized"
BYTES_READ_ONLY_MESSAGE = "bytes object is read only"

_hex = "0123456789ABCDEF"


# ============================================================================
# buf_impl structure
# ============================================================================

class buf_impl:
    """
    // typedef struct buf_impl {
    //   int32_t size;               // size in bytes of the buffer
    //   int32_t len;                // current size of the data in buffer. Invariant: len <= size
    //   uint8_t *bufptr;            // the actual data
    //   int32_t prev_size;          // previous value read from the instance
    //   int32_t prev_len;           // previous value read from the instance
    //   uint8_t *prev_bufptr;
    //   bbool   fixed;              // is size fixed? (actually encoded as negative size)
    //   bbool   mapped;
    //   bbool   solidified;
    // } buf_impl;
    """
    __slots__ = ('size', 'len', 'bufptr', 'prev_size', 'prev_len',
                 'prev_bufptr', 'fixed', 'mapped', 'solidified')

    def __init__(self, size=0, length=0, bufptr=None, prev_size=0,
                 prev_len=0, prev_bufptr=None, fixed=False, mapped=False,
                 solidified=False):
        self.size = size
        self.len = length
        self.bufptr = bufptr          # bytearray or None
        self.prev_size = prev_size
        self.prev_len = prev_len
        self.prev_bufptr = prev_bufptr
        self.fixed = fixed
        self.mapped = mapped
        self.solidified = solidified


# ============================================================================
# Base64 encode/decode — from https://github.com/Densaugeo/base64_arduino
# ============================================================================

# static unsigned char binary_to_base64(unsigned char v) {
#   if(v < 26) return v + 'A';
#   if(v < 52) return v + 71;
#   if(v < 62) return v - 4;
#   if(v == 62) return '+';
#   if(v == 63) return '/';
#   return 64;
# }
def binary_to_base64(v):
    if v < 26: return v + ord('A')
    if v < 52: return v + 71
    if v < 62: return v - 4
    if v == 62: return ord('+')
    if v == 63: return ord('/')
    return 64

# static unsigned char base64_to_binary(unsigned char c) {
#   if('A' <= c && c <= 'Z') return c - 'A';
#   if('a' <= c && c <= 'z') return c - 71;
#   if('0' <= c && c <= '9') return c + 4;
#   if(c == '+') return 62;
#   if(c == '/') return 63;
#   return 255;
# }
def base64_to_binary(c):
    if ord('A') <= c <= ord('Z'): return c - ord('A')
    if ord('a') <= c <= ord('z'): return c - 71
    if ord('0') <= c <= ord('9'): return c + 4
    if c == ord('+'): return 62
    if c == ord('/'): return 63
    return 255

# static unsigned int encode_base64_length(unsigned int input_length) {
#   return (input_length + 2)/3*4;
# }
def encode_base64_length(input_length):
    return (input_length + 2) // 3 * 4

# static unsigned int decode_base64_length(unsigned char input[]) { ... }
def decode_base64_length(input_bytes):
    """Calculate number of bytes of binary data in a base64 byte string."""
    i = 0
    while i < len(input_bytes) and base64_to_binary(input_bytes[i]) < 64:
        i += 1
    input_length = i
    output_length = input_length // 4 * 3
    rem = input_length % 4
    if rem == 2: return output_length + 1
    if rem == 3: return output_length + 2
    return output_length

# static unsigned int encode_base64(unsigned char input[], unsigned int input_length, unsigned char output[]) { ... }
def encode_base64(input_bytes, input_length):
    """Encode binary data to base64. Returns bytearray of base64 characters."""
    output = bytearray()
    idx = 0
    full_sets = input_length // 3
    for _ in range(full_sets):
        output.append(binary_to_base64(input_bytes[idx] >> 2))
        output.append(binary_to_base64(((input_bytes[idx] & 0x03) << 4) | (input_bytes[idx+1] >> 4)))
        output.append(binary_to_base64(((input_bytes[idx+1] & 0x0F) << 2) | (input_bytes[idx+2] >> 6)))
        output.append(binary_to_base64(input_bytes[idx+2] & 0x3F))
        idx += 3
    rem = input_length % 3
    if rem == 1:
        output.append(binary_to_base64(input_bytes[idx] >> 2))
        output.append(binary_to_base64((input_bytes[idx] & 0x03) << 4))
        output.append(ord('='))
        output.append(ord('='))
    elif rem == 2:
        output.append(binary_to_base64(input_bytes[idx] >> 2))
        output.append(binary_to_base64(((input_bytes[idx] & 0x03) << 4) | (input_bytes[idx+1] >> 4)))
        output.append(binary_to_base64((input_bytes[idx+1] & 0x0F) << 2))
        output.append(ord('='))
    return output

# static unsigned int decode_base64(unsigned char input[], unsigned char output[]) { ... }
def decode_base64(input_bytes):
    """Decode base64 byte string to binary. Returns bytearray."""
    output_length = decode_base64_length(input_bytes)
    output = bytearray(output_length)
    idx_in = 0
    idx_out = 0
    # full sets of 3 output bytes
    while idx_out + 2 < output_length:
        output[idx_out]   = ((base64_to_binary(input_bytes[idx_in]) << 2) | (base64_to_binary(input_bytes[idx_in+1]) >> 4)) & 0xFF
        output[idx_out+1] = ((base64_to_binary(input_bytes[idx_in+1]) << 4) | (base64_to_binary(input_bytes[idx_in+2]) >> 2)) & 0xFF
        output[idx_out+2] = ((base64_to_binary(input_bytes[idx_in+2]) << 6) | base64_to_binary(input_bytes[idx_in+3])) & 0xFF
        idx_in += 4
        idx_out += 3
    rem = output_length % 3
    if rem == 1:
        output[idx_out] = ((base64_to_binary(input_bytes[idx_in]) << 2) | (base64_to_binary(input_bytes[idx_in+1]) >> 4)) & 0xFF
    elif rem == 2:
        output[idx_out]   = ((base64_to_binary(input_bytes[idx_in]) << 2) | (base64_to_binary(input_bytes[idx_in+1]) >> 4)) & 0xFF
        output[idx_out+1] = ((base64_to_binary(input_bytes[idx_in+1]) << 4) | (base64_to_binary(input_bytes[idx_in+2]) >> 2)) & 0xFF
    return output


# ============================================================================
# Buffer low-level implementation
# ============================================================================

# static void buf_set_len(buf_impl* attr, const size_t len)
# {
#     int32_t old_len = attr->len;
#     attr->len = ((int32_t)len <= attr->size) ? (int32_t)len : attr->size;
#     if (old_len < attr->len) {
#         memset((void*) &attr->bufptr[old_len], 0, attr->len - old_len);
#     }
# }
def buf_set_len(attr, length):
    old_len = attr.len
    attr.len = length if length <= attr.size else attr.size
    if old_len < attr.len:
        for i in range(old_len, attr.len):
            attr.bufptr[i] = 0

# static size_t buf_add1(buf_impl* attr, const uint8_t data)
# {
#     if (attr->len < attr->size) {
#         attr->bufptr[attr->len++] = data;
#     }
#     return attr->len;
# }
def buf_add1(attr, data):
    if attr.len < attr.size:
        attr.bufptr[attr.len] = data & 0xFF
        attr.len += 1
    return attr.len

# static size_t buf_add2_le(buf_impl* attr, const uint16_t data)
# {
#     if (attr->len < attr->size - 1) {
#         attr->bufptr[attr->len++] = data;
#         attr->bufptr[attr->len++] = data >> 8;
#     }
#     return attr->len;
# }
def buf_add2_le(attr, data):
    data = data & 0xFFFF
    if attr.len < attr.size - 1:
        attr.bufptr[attr.len] = data & 0xFF
        attr.len += 1
        attr.bufptr[attr.len] = (data >> 8) & 0xFF
        attr.len += 1
    return attr.len

# static size_t buf_add2_be(buf_impl* attr, const uint16_t data)
# {
#     if (attr->len < attr->size - 1) {
#         attr->bufptr[attr->len++] = data >> 8;
#         attr->bufptr[attr->len++] = data;
#     }
#     return attr->len;
# }
def buf_add2_be(attr, data):
    data = data & 0xFFFF
    if attr.len < attr.size - 1:
        attr.bufptr[attr.len] = (data >> 8) & 0xFF
        attr.len += 1
        attr.bufptr[attr.len] = data & 0xFF
        attr.len += 1
    return attr.len

# static size_t buf_add3_le(buf_impl* attr, const uint32_t data)
def buf_add3_le(attr, data):
    data = data & 0xFFFFFFFF
    if attr.len < attr.size - 2:
        attr.bufptr[attr.len] = data & 0xFF;           attr.len += 1
        attr.bufptr[attr.len] = (data >> 8) & 0xFF;    attr.len += 1
        attr.bufptr[attr.len] = (data >> 16) & 0xFF;   attr.len += 1
    return attr.len

# size_t buf_add3_be(buf_impl* attr, const uint32_t data)
def buf_add3_be(attr, data):
    data = data & 0xFFFFFFFF
    if attr.len < attr.size - 2:
        attr.bufptr[attr.len] = (data >> 16) & 0xFF;   attr.len += 1
        attr.bufptr[attr.len] = (data >> 8) & 0xFF;    attr.len += 1
        attr.bufptr[attr.len] = data & 0xFF;            attr.len += 1
    return attr.len

# static size_t buf_add4_le(buf_impl* attr, const uint32_t data)
def buf_add4_le(attr, data):
    data = data & 0xFFFFFFFF
    if attr.len < attr.size - 3:
        attr.bufptr[attr.len] = data & 0xFF;            attr.len += 1
        attr.bufptr[attr.len] = (data >> 8) & 0xFF;     attr.len += 1
        attr.bufptr[attr.len] = (data >> 16) & 0xFF;    attr.len += 1
        attr.bufptr[attr.len] = (data >> 24) & 0xFF;    attr.len += 1
    return attr.len

# size_t buf_add4_be(buf_impl* attr, const uint32_t data)
def buf_add4_be(attr, data):
    data = data & 0xFFFFFFFF
    if attr.len < attr.size - 3:
        attr.bufptr[attr.len] = (data >> 24) & 0xFF;    attr.len += 1
        attr.bufptr[attr.len] = (data >> 16) & 0xFF;    attr.len += 1
        attr.bufptr[attr.len] = (data >> 8) & 0xFF;     attr.len += 1
        attr.bufptr[attr.len] = data & 0xFF;             attr.len += 1
    return attr.len

# static size_t buf_add_buf(buf_impl* attr, buf_impl* attr2)
# {
#     if (attr->len + attr2->len <= attr->size) {
#         for (int32_t i = 0; i < attr2->len; i++) {
#             attr->bufptr[attr->len++] = attr2->bufptr[i];
#         }
#     }
#     return attr->len;
# }
def buf_add_buf(attr, attr2):
    if attr.len + attr2.len <= attr.size:
        for i in range(attr2.len):
            attr.bufptr[attr.len] = attr2.bufptr[i]
            attr.len += 1
    return attr.len

# static size_t buf_add_raw(buf_impl* attr, const void* buf_raw, int32_t len)
# {
#     uint8_t *buf = (uint8_t*) buf_raw;
#     if ((len > 0) && (attr->len + len <= attr->size)) {
#         for (int32_t i = 0; i < len; i++) {
#             attr->bufptr[attr->len++] = buf[i];
#         }
#     }
#     return attr->len;
# }
def buf_add_raw(attr, buf_raw, length):
    if length > 0 and attr.len + length <= attr.size:
        for i in range(length):
            attr.bufptr[attr.len] = buf_raw[i] if isinstance(buf_raw[i], int) else ord(buf_raw[i])
            attr.len += 1
    return attr.len

# static uint8_t buf_get1(buf_impl* attr, int offset)
# {
#     if ((offset >= 0) && (offset < attr->len)) {
#         return attr->bufptr[offset];
#     }
#     return 0;
# }
def buf_get1(attr, offset):
    if offset >= 0 and offset < attr.len:
        return attr.bufptr[offset]
    return 0

# static void buf_set1(buf_impl* attr, size_t offset, uint8_t data)
# {
#     if ((int32_t)offset < attr->len) {
#         attr->bufptr[offset] = data;
#     }
# }
def buf_set1(attr, offset, data):
    if offset >= 0 and offset < attr.len:
        attr.bufptr[offset] = data & 0xFF

# static void buf_set2_le(buf_impl* attr, size_t offset, uint16_t data)
def buf_set2_le(attr, offset, data):
    data = data & 0xFFFF
    if offset + 1 < attr.len:
        attr.bufptr[offset] = data & 0xFF
        attr.bufptr[offset + 1] = (data >> 8) & 0xFF

# static void buf_set2_be(buf_impl* attr, size_t offset, uint16_t data)
def buf_set2_be(attr, offset, data):
    data = data & 0xFFFF
    if offset + 1 < attr.len:
        attr.bufptr[offset + 1] = data & 0xFF
        attr.bufptr[offset] = (data >> 8) & 0xFF

# static uint16_t buf_get2_le(buf_impl* attr, size_t offset)
def buf_get2_le(attr, offset):
    if offset + 1 < attr.len:
        return attr.bufptr[offset] | (attr.bufptr[offset + 1] << 8)
    return 0

# static uint16_t buf_get2_be(buf_impl* attr, size_t offset)
def buf_get2_be(attr, offset):
    if offset + 1 < attr.len:
        return attr.bufptr[offset + 1] | (attr.bufptr[offset] << 8)
    return 0

# static uint32_t buf_get3_le(buf_impl* attr, size_t offset)
def buf_get3_le(attr, offset):
    if offset + 2 < attr.len:
        return attr.bufptr[offset] | (attr.bufptr[offset+1] << 8) | (attr.bufptr[offset+2] << 16)
    return 0

# static uint32_t buf_get3_be(buf_impl* attr, size_t offset)
def buf_get3_be(attr, offset):
    if offset + 2 < attr.len:
        return attr.bufptr[offset+2] | (attr.bufptr[offset+1] << 8) | (attr.bufptr[offset] << 16)
    return 0

# static void buf_set3_le(buf_impl* attr, size_t offset, uint32_t data)
def buf_set3_le(attr, offset, data):
    data = data & 0xFFFFFFFF
    if offset + 2 < attr.len:
        attr.bufptr[offset]   = data & 0xFF
        attr.bufptr[offset+1] = (data >> 8) & 0xFF
        attr.bufptr[offset+2] = (data >> 16) & 0xFF

# static void buf_set3_be(buf_impl* attr, size_t offset, uint32_t data)
def buf_set3_be(attr, offset, data):
    data = data & 0xFFFFFFFF
    if offset + 2 < attr.len:
        attr.bufptr[offset+2] = data & 0xFF
        attr.bufptr[offset+1] = (data >> 8) & 0xFF
        attr.bufptr[offset]   = (data >> 16) & 0xFF

# static void buf_set4_le(buf_impl* attr, size_t offset, uint32_t data)
def buf_set4_le(attr, offset, data):
    data = data & 0xFFFFFFFF
    if offset + 3 < attr.len:
        attr.bufptr[offset]   = data & 0xFF
        attr.bufptr[offset+1] = (data >> 8) & 0xFF
        attr.bufptr[offset+2] = (data >> 16) & 0xFF
        attr.bufptr[offset+3] = (data >> 24) & 0xFF

# static void buf_set4_be(buf_impl* attr, size_t offset, uint32_t data)
def buf_set4_be(attr, offset, data):
    data = data & 0xFFFFFFFF
    if offset + 3 < attr.len:
        attr.bufptr[offset+3] = data & 0xFF
        attr.bufptr[offset+2] = (data >> 8) & 0xFF
        attr.bufptr[offset+1] = (data >> 16) & 0xFF
        attr.bufptr[offset]   = (data >> 24) & 0xFF

# static uint32_t buf_get4_le(buf_impl* attr, size_t offset)
def buf_get4_le(attr, offset):
    if offset + 3 < attr.len:
        return (attr.bufptr[offset] | (attr.bufptr[offset+1] << 8) |
                (attr.bufptr[offset+2] << 16) | (attr.bufptr[offset+3] << 24))
    return 0

# static uint32_t buf_get4_be(buf_impl* attr, size_t offset)
def buf_get4_be(attr, offset):
    if offset + 3 < attr.len:
        return (attr.bufptr[offset+3] | (attr.bufptr[offset+2] << 8) |
                (attr.bufptr[offset+1] << 16) | (attr.bufptr[offset] << 24))
    return 0

# static bbool buf_equals(buf_impl* buf1, buf_impl* buf2)
def buf_equals(buf1, buf2):
    if buf1 is buf2: return True
    if buf1 is None or buf2 is None: return False
    if buf1.len != buf2.len: return False
    length = buf1.len
    if buf1.bufptr is None and buf2.bufptr is None: return True
    if buf1.bufptr is None or buf2.bufptr is None: return False
    for i in range(length):
        if buf_get1(buf1, i) != buf_get1(buf2, i): return False
    return True

# static uint8_t asc2byte(char chr)
def asc2byte(ch):
    c = ord(ch) if isinstance(ch, str) else ch
    if ord('0') <= c <= ord('9'): return c - ord('0')
    if ord('A') <= c <= ord('F'): return c + 10 - ord('A')
    if ord('a') <= c <= ord('f'): return c + 10 - ord('a')
    return 0

# static void buf_add_hex(buf_impl* attr, const char *hex, size_t len)
# {
#     uint8_t val;
#     for (; len > 1; len -= 2) {
#         val = asc2byte(*hex++) << 4;
#         val |= asc2byte(*hex++);
#         buf_add1(attr, val);
#     }
# }
def buf_add_hex(attr, hex_str, length):
    idx = 0
    remaining = length
    while remaining > 1:
        val = asc2byte(hex_str[idx]) << 4
        idx += 1
        val |= asc2byte(hex_str[idx])
        idx += 1
        buf_add1(attr, val)
        remaining -= 2


# ============================================================================
# Wrapping into lib — helper functions
# ============================================================================

# static void check_ptr(bvm *vm, const buf_impl* attr) {
#     if (!attr->bufptr) {
#         be_raise(vm, "value_error", "operation not allowed on <null> pointer");
#     }
# }
def check_ptr(vm, attr):
    be_api = _lazy_be_api()
    if attr.bufptr is None:
        be_api.be_raise(vm, "value_error", "operation not allowed on <null> pointer")

# static void check_ptr_modifiable(bvm *vm, const buf_impl* attr) {
#     if (attr->solidified) {
#      be_raise(vm, "value_error", BYTES_READ_ONLY_MESSAGE);
#     }
#     if (!attr->bufptr) {
#         be_raise(vm, "value_error", "operation not allowed on <null> pointer");
#     }
# }
def check_ptr_modifiable(vm, attr):
    be_api = _lazy_be_api()
    if attr.solidified:
        be_api.be_raise(vm, "value_error", BYTES_READ_ONLY_MESSAGE)
    if attr.bufptr is None:
        be_api.be_raise(vm, "value_error", "operation not allowed on <null> pointer")

# buf_impl m_read_attributes(bvm *vm, int idx)
# {
#     buf_impl attr;
#     be_getmember(vm, idx, ".p");
#     attr.bufptr = attr.prev_bufptr = be_tocomptr(vm, -1);
#     be_pop(vm, 1);
#     be_getmember(vm, idx, ".len");
#     attr.len = attr.prev_len = be_toint(vm, -1);
#     be_pop(vm, 1);
#     be_getmember(vm, idx, ".size");
#     int32_t signed_size = be_toint(vm, -1);
#     attr.fixed = bfalse;
#     attr.mapped = bfalse;
#     attr.solidified = bfalse;
#     if (signed_size < 0) {
#         if (signed_size == BYTES_SIZE_MAPPED) { attr.mapped = btrue; }
#         if (signed_size == BYTES_SIZE_SOLIDIFIED) { attr.solidified = btrue; }
#         signed_size = attr.len;
#         attr.fixed = btrue;
#     }
#     attr.size = attr.prev_size = signed_size;
#     be_pop(vm, 1);
#     return attr;
# }
def m_read_attributes(vm, idx):
    be_api = _lazy_be_api()
    attr = buf_impl()

    be_api.be_getmember(vm, idx, ".p")
    attr.bufptr = be_api.be_tocomptr(vm, -1)
    attr.prev_bufptr = attr.bufptr
    be_api.be_pop(vm, 1)

    be_api.be_getmember(vm, idx, ".len")
    attr.len = be_api.be_toint(vm, -1)
    attr.prev_len = attr.len
    be_api.be_pop(vm, 1)

    be_api.be_getmember(vm, idx, ".size")
    signed_size = be_api.be_toint(vm, -1)
    attr.fixed = False
    attr.mapped = False
    attr.solidified = False
    if signed_size < 0:
        if signed_size == BYTES_SIZE_MAPPED:
            attr.mapped = True
        if signed_size == BYTES_SIZE_SOLIDIFIED:
            attr.solidified = True
        signed_size = attr.len
        attr.fixed = True
    attr.size = signed_size
    attr.prev_size = signed_size
    be_api.be_pop(vm, 1)
    return attr

# static void m_assert_not_readlonly(bvm *vm, const buf_impl* attr)
# {
#     if (attr->solidified) {
#      be_raise(vm, "value_error", BYTES_READ_ONLY_MESSAGE);
#     }
# }
def m_assert_not_readonly(vm, attr):
    be_api = _lazy_be_api()
    if attr.solidified:
        be_api.be_raise(vm, "value_error", BYTES_READ_ONLY_MESSAGE)

# void m_write_attributes(bvm *vm, int rel_idx, const buf_impl * attr)
# {
#     m_assert_not_readlonly(vm, attr);
#     int idx = be_absindex(vm, rel_idx);
#     if (attr->bufptr != attr->prev_bufptr) { ... }
#     if (attr->len != attr->prev_len) { ... }
#     int32_t new_size = attr->size;
#     if (attr->mapped) { new_size = BYTES_SIZE_MAPPED; }
#     else if (attr->fixed) { new_size = BYTES_SIZE_FIXED; }
#     if (new_size != attr->prev_size) { ... }
# }
def m_write_attributes(vm, rel_idx, attr):
    be_api = _lazy_be_api()
    m_assert_not_readonly(vm, attr)
    idx = be_api.be_absindex(vm, rel_idx)

    if attr.bufptr is not attr.prev_bufptr:
        be_api.be_pushcomptr(vm, attr.bufptr)
        be_api.be_setmember(vm, idx, ".p")
        be_api.be_pop(vm, 1)

    if attr.len != attr.prev_len:
        be_api.be_pushint(vm, attr.len)
        be_api.be_setmember(vm, idx, ".len")
        be_api.be_pop(vm, 1)

    new_size = attr.size
    if attr.mapped:
        new_size = BYTES_SIZE_MAPPED
    elif attr.fixed:
        new_size = BYTES_SIZE_FIXED
    if new_size != attr.prev_size:
        be_api.be_pushint(vm, new_size)
        be_api.be_setmember(vm, idx, ".size")
        be_api.be_pop(vm, 1)

# void bytes_realloc(bvm *vm, buf_impl * attr, size_t size)
# {
#     m_assert_not_readlonly(vm, attr);
#     if (!attr->fixed && size < 4) { size = 4; }
#     if (size > vm->bytesmaxsize) { size = vm->bytesmaxsize; }
#     size_t oldsize = attr->bufptr ? attr->size : 0;
#     attr->bufptr = (uint8_t*) be_realloc(vm, attr->bufptr, oldsize, size);
#     attr->size = size;
#     if (!attr->bufptr) { attr->len = 0; }
# }
def bytes_realloc(vm, attr, size):
    be_mem = _lazy_be_mem()
    m_assert_not_readonly(vm, attr)
    if not attr.fixed and size < 4:
        size = 4
    if size > vm.bytesmaxsize:
        size = vm.bytesmaxsize
    oldsize = attr.size if attr.bufptr is not None else 0
    # In Python, we just create a new bytearray of the right size
    if attr.bufptr is None:
        attr.bufptr = bytearray(size)
    else:
        old_buf = attr.bufptr
        attr.bufptr = bytearray(size)
        copy_len = min(len(old_buf), size)
        attr.bufptr[:copy_len] = old_buf[:copy_len]
    # Track memory via be_realloc accounting
    be_mem.be_realloc(vm, None, oldsize, size)
    attr.size = size
    # bufptr is always non-None after bytearray() creation

# static void bytes_new_object(bvm *vm, size_t size)
# {
#     be_getbuiltin(vm, "bytes");
#     be_pushint(vm, size);
#     be_call(vm, 1);
#     be_pop(vm, 1);
# }
def bytes_new_object(vm, size):
    be_api = _lazy_be_api()
    be_api.be_getbuiltin(vm, "bytes")
    be_api.be_pushint(vm, size)
    be_api.be_call(vm, 1)
    be_api.be_pop(vm, 1)

# void _bytes_resize(bvm *vm, buf_impl * attr, size_t new_size) {
#     bytes_realloc(vm, attr, new_size);
#     if (!attr->bufptr) { be_throw(vm, BE_MALLOC_FAIL); }
# }
def _bytes_resize(vm, attr, new_size):
    be_exec = _lazy_be_exec()
    from berry_port.be_object import BE_MALLOC_FAIL
    bytes_realloc(vm, attr, new_size)
    if attr.bufptr is None:
        be_exec.be_throw(vm, BE_MALLOC_FAIL)

# void bytes_resize(bvm *vm, buf_impl * attr, size_t new_size) {
#     if (attr->mapped) { return; }
#     if (attr->size >= (int32_t)new_size) {
#         if (attr->size <= 64) { return; }
#         if (attr->size < (int32_t)new_size * 2) { return; }
#     }
#     _bytes_resize(vm, attr, new_size);
# }
def bytes_resize(vm, attr, new_size):
    if attr.mapped:
        return
    if attr.size >= new_size:
        if attr.size <= 64:
            return
        if attr.size < new_size * 2:
            return
    _bytes_resize(vm, attr, new_size)

# buf_impl bytes_check_data(bvm *vm, size_t add_size) {
#     buf_impl attr = m_read_attributes(vm, 1);
#     if (attr.len + (int32_t)add_size > attr.size) {
#         if (attr.fixed) { be_raise(vm, BYTES_RESIZE_ERROR, BYTES_RESIZE_MESSAGE); }
#         bytes_resize(vm, &attr, attr.len + add_size);
#     }
#     return attr;
# }
def bytes_check_data(vm, add_size):
    be_api = _lazy_be_api()
    attr = m_read_attributes(vm, 1)
    if attr.len + add_size > attr.size:
        if attr.fixed:
            be_api.be_raise(vm, BYTES_RESIZE_ERROR, BYTES_RESIZE_MESSAGE)
        bytes_resize(vm, attr, attr.len + add_size)
    return attr

# size_t be_bytes_tohex(char * out, size_t outsz, const uint8_t * in, size_t insz) {
#   ...
# }
def be_bytes_tohex(in_bytes, insz):
    """Convert bytes to hex string. Returns the hex string."""
    parts = []
    for i in range(insz):
        b = in_bytes[i]
        parts.append(_hex[(b >> 4) & 0xF])
        parts.append(_hex[b & 0xF])
    return ''.join(parts)


# ============================================================================
# Bytes class methods
# ============================================================================

# static int m_init(bvm *vm) { ... }
def m_init(vm):
    be_api = _lazy_be_api()
    be_exec = _lazy_be_exec()
    from berry_port.be_object import BE_MALLOC_FAIL
    argc = be_api.be_top(vm)
    attr = buf_impl(size=0, length=0, bufptr=None, prev_size=-1,
                    prev_len=-1, prev_bufptr=None)
    hex_in = None

    size_arg = 0
    if argc > 1 and be_api.be_isint(vm, 2):
        size_arg = be_api.be_toint(vm, 2)
    elif argc > 2 and be_api.be_isint(vm, 3):
        size_arg = be_api.be_toint(vm, 3)

    if argc > 1 and be_api.be_iscomptr(vm, 2):
        if size_arg:
            attr.len = -size_arg if size_arg < 0 else size_arg
            attr.bufptr = be_api.be_tocomptr(vm, 2)
            attr.fixed = True
            attr.mapped = True
            m_write_attributes(vm, 1, attr)
            return be_api.be_returnnilvalue(vm)
        else:
            be_api.be_raise(vm, "value_error", "size is required")

    if size_arg == 0:
        size_arg = BYTES_DEFAULT_SIZE

    if size_arg >= 0:
        size_arg += BYTES_HEADROOM
        if size_arg > attr.size:
            attr.size = size_arg
    else:
        attr.size = -size_arg
        attr.fixed = True
    size_arg = attr.size

    if argc > 1 and be_api.be_isstring(vm, 2):
        hex_in = be_api.be_tostring(vm, 2)
        if hex_in:
            size_arg = len(hex_in) // 2
        if size_arg > attr.size:
            if attr.fixed:
                be_api.be_raise(vm, BYTES_RESIZE_ERROR, BYTES_RESIZE_MESSAGE)
            else:
                attr.size = size_arg

    bytes_realloc(vm, attr, attr.size)
    if attr.bufptr is None:
        be_exec.be_throw(vm, BE_MALLOC_FAIL)

    if hex_in:
        buf_add_hex(attr, hex_in, len(hex_in))

    if attr.fixed:
        buf_set_len(attr, attr.size)

    m_write_attributes(vm, 1, attr)
    return be_api.be_returnnilvalue(vm)

# static int m_deinit(bvm *vm) { ... }
def m_deinit(vm):
    be_api = _lazy_be_api()
    be_mem = _lazy_be_mem()
    attr = m_read_attributes(vm, 1)
    if attr.bufptr is not None and not attr.mapped:
        be_mem.be_realloc(vm, None, attr.size, 0)
    attr.size = 0
    attr.len = 0
    attr.bufptr = None
    m_write_attributes(vm, 1, attr)
    return be_api.be_returnnilvalue(vm)

# static int m_tostring(bvm *vm) { ... }
def m_tostring(vm):
    be_api = _lazy_be_api()
    argc = be_api.be_top(vm)
    max_len = 32
    if argc > 1 and be_api.be_isint(vm, 2):
        max_len = be_api.be_toint(vm, 2)
    attr = m_read_attributes(vm, 1)
    if attr.bufptr is not None:
        length = attr.len
        truncated = False
        if max_len > 0 and length > max_len:
            length = max_len
            truncated = True
        hex_out = be_bytes_tohex(attr.bufptr, length)
        result = "bytes('" + hex_out
        if truncated:
            result += "..."
        result += "')"
        be_api.be_pushstring(vm, result)
    else:
        be_api.be_pushstring(vm, "bytes(<null>)")
    return be_api.be_returnvalue(vm)

# static int m_tohex(bvm *vm) { ... }
def m_tohex(vm):
    be_api = _lazy_be_api()
    attr = m_read_attributes(vm, 1)
    if attr.bufptr is not None:
        hex_out = be_bytes_tohex(attr.bufptr, attr.len)
        be_api.be_pushstring(vm, hex_out)
    else:
        be_api.be_pushstring(vm, "")
    return be_api.be_returnvalue(vm)

# static int m_asstring(bvm *vm) { ... }
def m_asstring(vm):
    be_api = _lazy_be_api()
    attr = bytes_check_data(vm, 0)
    check_ptr(vm, attr)
    # Find first null byte or use full length
    buf = attr.bufptr
    safe_len = attr.len
    for i in range(attr.len):
        if buf[i] == 0:
            safe_len = i
            break
    s = bytes(buf[:safe_len]).decode('latin-1')
    be_api.be_pushnstring(vm, s, safe_len)
    return be_api.be_returnvalue(vm)

# static int m_fromstring(bvm *vm) { ... }
def m_fromstring(vm):
    be_api = _lazy_be_api()
    argc = be_api.be_top(vm)
    if argc >= 2 and be_api.be_isstring(vm, 2):
        s = be_api.be_tostring(vm, 2)
        s_len = len(s)
        attr = bytes_check_data(vm, 0)
        check_ptr_modifiable(vm, attr)
        if attr.fixed and attr.len != s_len:
            be_api.be_raise(vm, BYTES_RESIZE_ERROR, BYTES_RESIZE_MESSAGE)
        bytes_resize(vm, attr, s_len)
        length = s_len if s_len <= attr.size else attr.size
        for i in range(length):
            attr.bufptr[i] = ord(s[i]) if isinstance(s[i], str) else s[i]
        attr.len = length
        be_api.be_pop(vm, 1)
        m_write_attributes(vm, 1, attr)
        return be_api.be_returnvalue(vm)
    be_api.be_raise(vm, "type_error", "operand must be a string")
    return be_api.be_returnnilvalue(vm)

# static int m_add(bvm *vm) { ... }
def m_add(vm):
    be_api = _lazy_be_api()
    argc = be_api.be_top(vm)
    attr = bytes_check_data(vm, 4)
    check_ptr_modifiable(vm, attr)
    if attr.fixed:
        be_api.be_raise(vm, BYTES_RESIZE_ERROR, BYTES_RESIZE_MESSAGE)
    if argc >= 2 and be_api.be_isint(vm, 2):
        v = be_api.be_toint(vm, 2)
        vsize = 1
        if argc >= 3:
            if be_api.be_isint(vm, 3):
                vsize = be_api.be_toint(vm, 3)
            else:
                be_api.be_raise(vm, "type_error", "operands must be int")
                return 0
        if   vsize == 0:  pass
        elif vsize in (-1, 1): buf_add1(attr, v)
        elif vsize == 2:  buf_add2_le(attr, v)
        elif vsize == 3:  buf_add3_le(attr, v)
        elif vsize == 4:  buf_add4_le(attr, v)
        elif vsize == -2: buf_add2_be(attr, v)
        elif vsize == -3: buf_add3_be(attr, v)
        elif vsize == -4: buf_add4_be(attr, v)
        else:
            be_api.be_raise(vm, "type_error", "size must be between -4 and 4.")
        be_api.be_pop(vm, argc - 1)
        m_write_attributes(vm, 1, attr)
        return be_api.be_returnvalue(vm)
    be_api.be_raise(vm, "type_error", "operands must be int")
    return be_api.be_returnnilvalue(vm)

# static int m_get(bvm *vm, bbool sign) { ... }
def m_get(vm, sign):
    be_api = _lazy_be_api()
    argc = be_api.be_top(vm)
    attr = bytes_check_data(vm, 0)
    check_ptr(vm, attr)
    if argc >= 2 and be_api.be_isint(vm, 2):
        idx = be_api.be_toint(vm, 2)
        vsize = 1
        if argc >= 3 and be_api.be_isint(vm, 3):
            vsize = be_api.be_toint(vm, 3)
        if idx < 0:
            idx = attr.len + idx
        if idx < 0:
            vsize = 0
        ret = 0
        if   vsize == 0:  pass
        elif vsize in (-1, 1):
            ret = buf_get1(attr, idx)
            if sign: ret = _to_signed8(ret)
        elif vsize == 2:
            ret = buf_get2_le(attr, idx)
            if sign: ret = _to_signed16(ret)
        elif vsize == 3:
            ret = buf_get3_le(attr, idx)
            if sign and (ret & 0x800000): ret = ret | 0xFF000000
            if sign: ret = _to_signed32(ret)
        elif vsize == 4:
            ret = buf_get4_le(attr, idx)
            if sign: ret = _to_signed32(ret)
        elif vsize == -2:
            ret = buf_get2_be(attr, idx)
            if sign: ret = _to_signed16(ret)
        elif vsize == -3:
            ret = buf_get3_be(attr, idx)
            if sign and (ret & 0x800000): ret = ret | 0xFF000000
            if sign: ret = _to_signed32(ret)
        elif vsize == -4:
            ret = buf_get4_be(attr, idx)
            if sign: ret = _to_signed32(ret)
        else:
            be_api.be_raise(vm, "type_error", "size must be -4, -3, -2, -1, 0, 1, 2, 3 or 4.")
        be_api.be_pop(vm, argc - 1)
        be_api.be_pushint(vm, ret)
        return be_api.be_returnvalue(vm)
    be_api.be_raise(vm, "type_error", "operands must be int")
    return be_api.be_returnnilvalue(vm)

def _to_signed8(v):
    v = v & 0xFF
    return v - 0x100 if v >= 0x80 else v

def _to_signed16(v):
    v = v & 0xFFFF
    return v - 0x10000 if v >= 0x8000 else v

def _to_signed32(v):
    v = v & 0xFFFFFFFF
    return v - 0x100000000 if v >= 0x80000000 else v

# static int m_getfloat(bvm *vm) { ... }
def m_getfloat(vm):
    be_api = _lazy_be_api()
    argc = be_api.be_top(vm)
    attr = bytes_check_data(vm, 0)
    check_ptr(vm, attr)
    if argc >= 2 and be_api.be_isint(vm, 2):
        idx = be_api.be_toint(vm, 2)
        ret_f = 0.0
        if idx < 0:
            idx = attr.len + idx
        if idx >= 0:
            big_endian = False
            if argc >= 3:
                big_endian = be_api.be_tobool(vm, 3)
            ret_i = buf_get4_be(attr, idx) if big_endian else buf_get4_le(attr, idx)
            # Reinterpret int32 bits as float
            ret_f = struct.unpack('<f', struct.pack('<I', ret_i & 0xFFFFFFFF))[0]
        be_api.be_pop(vm, argc - 1)
        be_api.be_pushreal(vm, ret_f)
        return be_api.be_returnvalue(vm)
    be_api.be_raise(vm, "type_error", "operands must be int")
    return be_api.be_returnnilvalue(vm)

# static int m_geti(bvm *vm) { return m_get(vm, 1); }
def m_geti(vm):
    return m_get(vm, True)

# static int m_getu(bvm *vm) { return m_get(vm, 0); }
def m_getu(vm):
    return m_get(vm, False)


# static int m_set(bvm *vm) { ... }
def m_set(vm):
    be_api = _lazy_be_api()
    argc = be_api.be_top(vm)
    attr = bytes_check_data(vm, 0)
    check_ptr_modifiable(vm, attr)
    if argc >= 3 and be_api.be_isint(vm, 2) and be_api.be_isint(vm, 3):
        idx = be_api.be_toint(vm, 2)
        value = be_api.be_toint(vm, 3)
        vsize = 1
        if argc >= 4:
            if be_api.be_isint(vm, 4):
                vsize = be_api.be_toint(vm, 4)
            else:
                be_api.be_raise(vm, "type_error", "operands must be int")
                return 0
        if idx < 0:
            idx = attr.len + idx
        if idx < 0:
            vsize = 0
        if   vsize == 0:  pass
        elif vsize in (-1, 1): buf_set1(attr, idx, value)
        elif vsize == 2:  buf_set2_le(attr, idx, value)
        elif vsize == 3:  buf_set3_le(attr, idx, value)
        elif vsize == 4:  buf_set4_le(attr, idx, value)
        elif vsize == -2: buf_set2_be(attr, idx, value)
        elif vsize == -3: buf_set3_be(attr, idx, value)
        elif vsize == -4: buf_set4_be(attr, idx, value)
        else:
            be_api.be_raise(vm, "type_error", "size must be -4, -3, -2, -1, 0, 1, 2, 3 or 4.")
        be_api.be_pop(vm, argc - 1)
        return be_api.be_returnnilvalue(vm)
    be_api.be_raise(vm, "type_error", "operands must be int")
    return be_api.be_returnnilvalue(vm)

# static int m_setfloat(bvm *vm) { ... }
def m_setfloat(vm):
    be_api = _lazy_be_api()
    argc = be_api.be_top(vm)
    attr = bytes_check_data(vm, 0)
    check_ptr_modifiable(vm, attr)
    if argc >= 3 and be_api.be_isint(vm, 2) and (be_api.be_isint(vm, 3) or be_api.be_isreal(vm, 3)):
        idx = be_api.be_toint(vm, 2)
        if idx < 0:
            idx = attr.len + idx
        if idx >= 0:
            val_f = float(be_api.be_toreal(vm, 3))
            val_i = struct.unpack('<I', struct.pack('<f', val_f))[0]
            big_endian = False
            if argc >= 4:
                big_endian = be_api.be_tobool(vm, 4)
            if big_endian:
                buf_set4_be(attr, idx, val_i)
            else:
                buf_set4_le(attr, idx, val_i)
            be_api.be_pop(vm, argc - 1)
        return be_api.be_returnnilvalue(vm)
    be_api.be_raise(vm, "type_error", "operands must be int or float")
    return be_api.be_returnnilvalue(vm)

# static int m_addfloat(bvm *vm) { ... }
def m_addfloat(vm):
    be_api = _lazy_be_api()
    argc = be_api.be_top(vm)
    attr = bytes_check_data(vm, 4)
    check_ptr_modifiable(vm, attr)
    if attr.fixed:
        be_api.be_raise(vm, BYTES_RESIZE_ERROR, BYTES_RESIZE_MESSAGE)
    if argc >= 2 and (be_api.be_isint(vm, 2) or be_api.be_isreal(vm, 2)):
        val_f = float(be_api.be_toreal(vm, 2))
        val_i = struct.unpack('<I', struct.pack('<f', val_f))[0]
        big_endian = False
        if argc >= 3:
            big_endian = be_api.be_tobool(vm, 3)
        if big_endian:
            buf_add4_be(attr, val_i)
        else:
            buf_add4_le(attr, val_i)
        be_api.be_pop(vm, argc - 1)
        m_write_attributes(vm, 1, attr)
        return be_api.be_returnvalue(vm)
    be_api.be_raise(vm, "type_error", "operands must be int or float")
    return be_api.be_returnnilvalue(vm)

# static int m_setbytes(bvm *vm) { ... }
def m_setbytes(vm):
    be_api = _lazy_be_api()
    argc = be_api.be_top(vm)
    attr = bytes_check_data(vm, 0)
    check_ptr_modifiable(vm, attr)
    if argc >= 3 and be_api.be_isint(vm, 2) and be_isbytes(vm, 3):
        idx = be_api.be_toint(vm, 2)
        attr2 = m_read_attributes(vm, 3)
        from_len_total = attr2.len
        buf_ptr = attr2.bufptr
        if idx < 0:
            idx = attr.len + idx
        if idx < 0:
            idx = 0
        if idx >= attr.len:
            idx = attr.len

        from_byte = 0
        if argc >= 4 and be_api.be_isint(vm, 4):
            from_byte = be_api.be_toint(vm, 4)
            if from_byte < 0: from_byte = 0
            if from_byte >= from_len_total: from_byte = from_len_total

        from_len = from_len_total - from_byte
        if argc >= 5 and be_api.be_isint(vm, 5):
            from_len = be_api.be_toint(vm, 5)
            if from_len < 0: from_len = 0
            if from_len >= from_len_total: from_len = from_len_total
        if idx + from_len >= attr.len:
            from_len = attr.len - idx

        if from_len > 0:
            attr.bufptr[idx:idx+from_len] = buf_ptr[from_byte:from_byte+from_len]
    else:
        be_api.be_raise(vm, "type_error", "operands must be int and bytes")
    return be_api.be_returnnilvalue(vm)

# static int m_reverse(bvm *vm) { ... }
def m_reverse(vm):
    be_api = _lazy_be_api()
    argc = be_api.be_top(vm)
    attr = bytes_check_data(vm, 0)
    check_ptr_modifiable(vm, attr)

    idx = 0
    length = attr.len
    grouplen = 1

    if argc >= 2 and be_api.be_isint(vm, 2):
        idx = be_api.be_toint(vm, 2)
        if idx < 0: idx = attr.len + idx
        if idx < 0: idx = 0
        if idx > attr.len: idx = attr.len
    if argc >= 3 and be_api.be_isint(vm, 3):
        length = be_api.be_toint(vm, 3)
        if length < 0: length = attr.len - idx
    if idx + length >= attr.len:
        length = attr.len - idx
    if argc >= 4 and be_api.be_isint(vm, 4):
        grouplen = be_api.be_toint(vm, 4)
        if grouplen <= 0: grouplen = 1
    length = length - (length % grouplen)

    if length > 0:
        if grouplen == 1:
            i = idx
            j = idx + length - 1
            while i < j:
                attr.bufptr[i], attr.bufptr[j] = attr.bufptr[j], attr.bufptr[i]
                i += 1; j -= 1
        else:
            i = idx
            j = idx + length - grouplen
            while i < j:
                for k in range(grouplen):
                    attr.bufptr[i+k], attr.bufptr[j+k] = attr.bufptr[j+k], attr.bufptr[i+k]
                i += grouplen; j -= grouplen

    be_api.be_pushvalue(vm, 1)
    return be_api.be_returnvalue(vm)

# static int m_setitem(bvm *vm) { ... }
def m_setitem(vm):
    be_api = _lazy_be_api()
    argc = be_api.be_top(vm)
    attr = bytes_check_data(vm, 0)
    check_ptr_modifiable(vm, attr)
    if argc >= 3 and be_api.be_isint(vm, 2) and be_api.be_isint(vm, 3):
        index = be_api.be_toint(vm, 2)
        val = be_api.be_toint(vm, 3)
        if index < 0:
            index += attr.len
        if 0 <= index < attr.len:
            buf_set1(attr, index, val)
            return be_api.be_returnnilvalue(vm)
    be_api.be_raise(vm, "index_error", "bytes index out of range or value non int")
    return be_api.be_returnnilvalue(vm)

# static int m_item(bvm *vm) { ... }
def m_item(vm):
    be_api = _lazy_be_api()
    argc = be_api.be_top(vm)
    attr = bytes_check_data(vm, 0)
    check_ptr(vm, attr)
    if argc >= 2 and be_api.be_isint(vm, 2):
        index = be_api.be_toint(vm, 2)
        if index < 0:
            index += attr.len
        if 0 <= index < attr.len:
            be_api.be_pushint(vm, buf_get1(attr, index))
            return be_api.be_returnvalue(vm)
    if argc >= 2 and be_api.be_isinstance(vm, 2):
        cname = be_api.be_classname(vm, 2)
        if cname == "range":
            size = attr.len
            be_api.be_getmember(vm, 2, "__lower__")
            lower = be_api.be_toint(vm, -1)
            be_api.be_pop(vm, 1)
            be_api.be_getmember(vm, 2, "__upper__")
            upper = be_api.be_toint(vm, -1)
            be_api.be_pop(vm, 1)
            if upper < 0: upper += attr.len
            if lower < 0: lower += attr.len
            upper = min(upper, size - 1)
            lower = max(lower, 0)
            bytes_new_object(vm, max(upper - lower, 0))
            attr2 = m_read_attributes(vm, -1)
            for i in range(lower, upper + 1):
                buf_add1(attr2, attr.bufptr[i])
            m_write_attributes(vm, -1, attr2)
            return be_api.be_returnvalue(vm)
    be_api.be_raise(vm, "index_error", "bytes index out of range")
    return be_api.be_returnnilvalue(vm)

# static int m_size(bvm *vm) { ... }
def m_size(vm):
    be_api = _lazy_be_api()
    attr = m_read_attributes(vm, 1)
    be_api.be_pushint(vm, attr.len)
    return be_api.be_returnvalue(vm)

# static int m_tobool(bvm *vm) { ... }
def m_tobool(vm):
    be_api = _lazy_be_api()
    attr = m_read_attributes(vm, 1)
    be_api.be_pushbool(vm, attr.len > 0)
    return be_api.be_returnvalue(vm)

# static int m_resize(bvm *vm) { ... }
def m_resize(vm):
    be_api = _lazy_be_api()
    argc = be_api.be_top(vm)
    attr = m_read_attributes(vm, 1)
    check_ptr_modifiable(vm, attr)
    if argc <= 1 or not be_api.be_isint(vm, 2):
        be_api.be_raise(vm, "type_error", "size must be of type 'int'")
    new_len = be_api.be_toint(vm, 2)
    if new_len < 0:
        new_len = 0
    if attr.fixed and attr.len != new_len:
        be_api.be_raise(vm, BYTES_RESIZE_ERROR, BYTES_RESIZE_MESSAGE)
    bytes_resize(vm, attr, new_len)
    buf_set_len(attr, new_len)
    be_api.be_pop(vm, 1)
    m_write_attributes(vm, 1, attr)
    return be_api.be_returnvalue(vm)

# static int m_clear(bvm *vm) { ... }
def m_clear(vm):
    be_api = _lazy_be_api()
    attr = m_read_attributes(vm, 1)
    check_ptr_modifiable(vm, attr)
    if attr.fixed:
        be_api.be_raise(vm, BYTES_RESIZE_ERROR, BYTES_RESIZE_MESSAGE)
    attr.len = 0
    m_write_attributes(vm, 1, attr)
    return be_api.be_returnnilvalue(vm)


# static int m_merge(bvm *vm) { ... }
def m_merge(vm):
    be_api = _lazy_be_api()
    argc = be_api.be_top(vm)
    attr = m_read_attributes(vm, 1)
    check_ptr(vm, attr)
    if argc >= 2 and (be_isbytes(vm, 2) or be_api.be_isstring(vm, 2)):
        if be_isbytes(vm, 2):
            attr2 = m_read_attributes(vm, 2)
            check_ptr(vm, attr2)
            buf = attr2.bufptr
            buf_len = attr2.len
        else:
            s = be_api.be_tostring(vm, 2)
            buf = bytearray(s.encode('latin-1')) if isinstance(s, str) else bytearray(s)
            buf_len = len(buf)

        bytes_new_object(vm, attr.len + buf_len)
        attr3 = m_read_attributes(vm, -1)
        check_ptr(vm, attr3)
        buf_add_buf(attr3, attr)
        buf_add_raw(attr3, buf, buf_len)
        m_write_attributes(vm, -1, attr3)
        return be_api.be_returnvalue(vm)
    be_api.be_raise(vm, "type_error", "operand must be bytes")
    return be_api.be_returnnilvalue(vm)

# static int m_copy(bvm *vm) { ... }
def m_copy(vm):
    be_api = _lazy_be_api()
    attr = m_read_attributes(vm, 1)
    check_ptr(vm, attr)
    bytes_new_object(vm, attr.len)
    attr2 = m_read_attributes(vm, -1)
    check_ptr(vm, attr2)
    buf_add_buf(attr2, attr)
    m_write_attributes(vm, -1, attr2)
    return be_api.be_returnvalue(vm)

# static int m_connect(bvm *vm) { ... }
def m_connect(vm):
    be_api = _lazy_be_api()
    argc = be_api.be_top(vm)
    attr = m_read_attributes(vm, 1)
    check_ptr_modifiable(vm, attr)
    if attr.fixed:
        be_api.be_raise(vm, BYTES_RESIZE_ERROR, BYTES_RESIZE_MESSAGE)
    if argc >= 2 and (be_isbytes(vm, 2) or be_api.be_isint(vm, 2) or be_api.be_isstring(vm, 2)):
        if be_api.be_isint(vm, 2):
            bytes_resize(vm, attr, attr.len + 1)
            buf_add1(attr, be_api.be_toint(vm, 2))
            m_write_attributes(vm, 1, attr)
        elif be_api.be_isstring(vm, 2):
            s = be_api.be_tostring(vm, 2)
            str_len = len(s)
            if str_len > 0:
                bytes_resize(vm, attr, attr.len + str_len)
                s_bytes = s.encode('latin-1') if isinstance(s, str) else s
                buf_add_raw(attr, s_bytes, str_len)
                m_write_attributes(vm, 1, attr)
        else:
            attr2 = m_read_attributes(vm, 2)
            check_ptr(vm, attr2)
            bytes_resize(vm, attr, attr.len + attr2.len)
            buf_add_buf(attr, attr2)
            m_write_attributes(vm, 1, attr)
        be_api.be_pushvalue(vm, 1)
        return be_api.be_returnvalue(vm)
    be_api.be_raise(vm, "type_error", "operand must be bytes or int or string")
    return be_api.be_returnnilvalue(vm)

# static int m_appendhex(bvm *vm) { ... }
def m_appendhex(vm):
    be_api = _lazy_be_api()
    argc = be_api.be_top(vm)
    attr = m_read_attributes(vm, 1)
    check_ptr_modifiable(vm, attr)
    if attr.fixed:
        be_api.be_raise(vm, BYTES_RESIZE_ERROR, BYTES_RESIZE_MESSAGE)
    if argc >= 2 and be_isbytes(vm, 2):
        attr2 = m_read_attributes(vm, 2)
        check_ptr(vm, attr2)
        bytes_resize(vm, attr, attr.len + attr2.len * 2)
        for i in range(attr2.len):
            b = attr2.bufptr[i]
            buf_add1(attr, ord(_hex[(b >> 4) & 0xF]))
            buf_add1(attr, ord(_hex[b & 0xF]))
        m_write_attributes(vm, 1, attr)
        be_api.be_pushvalue(vm, 1)
        return be_api.be_returnvalue(vm)
    be_api.be_raise(vm, "type_error", "operand must be bytes")
    return be_api.be_returnnilvalue(vm)

# static int m_appendb64(bvm *vm) { ... }
def m_appendb64(vm):
    be_api = _lazy_be_api()
    argc = be_api.be_top(vm)
    attr = m_read_attributes(vm, 1)
    check_ptr_modifiable(vm, attr)
    if attr.fixed:
        be_api.be_raise(vm, BYTES_RESIZE_ERROR, BYTES_RESIZE_MESSAGE)
    if argc >= 2 and be_isbytes(vm, 2):
        attr2 = m_read_attributes(vm, 2)
        check_ptr(vm, attr2)
        idx = 0
        length = attr2.len
        if argc >= 3 and be_api.be_isint(vm, 3):
            idx = be_api.be_toint(vm, 3)
            if idx < 0: idx = attr2.len + idx
            if idx < 0: idx = 0
            if idx > attr2.len: idx = attr2.len
            if argc >= 4 and be_api.be_isint(vm, 4):
                length = be_api.be_toint(vm, 4)
                if length < 0: length = 0
            if idx + length >= attr2.len:
                length = attr2.len - idx
        if length > 0:
            bytes_resize(vm, attr, attr.len + encode_base64_length(length) + 1)
            encoded = encode_base64(attr2.bufptr[idx:idx+length], length)
            for b in encoded:
                buf_add1(attr, b)
            m_write_attributes(vm, 1, attr)
        be_api.be_pushvalue(vm, 1)
        return be_api.be_returnvalue(vm)
    be_api.be_raise(vm, "type_error", "operand must be bytes")
    return be_api.be_returnnilvalue(vm)

# static int bytes_equal(bvm *vm, bbool iseq) { ... }
def bytes_equal(vm, iseq):
    be_api = _lazy_be_api()
    attr1 = m_read_attributes(vm, 1)
    if not be_isbytes(vm, 2):
        ret = not iseq
    else:
        attr2 = m_read_attributes(vm, 2)
        if buf_equals(attr1, attr2):
            ret = iseq
        else:
            ret = not iseq
    be_api.be_pushbool(vm, ret)
    return be_api.be_returnvalue(vm)

# static int m_equal(bvm *vm) { return bytes_equal(vm, btrue); }
def m_equal(vm):
    return bytes_equal(vm, True)

# static int m_nequal(bvm *vm) { return bytes_equal(vm, bfalse); }
def m_nequal(vm):
    return bytes_equal(vm, False)

# static int m_tob64(bvm *vm) { ... }
def m_tob64(vm):
    be_api = _lazy_be_api()
    attr = m_read_attributes(vm, 1)
    check_ptr(vm, attr)
    length = attr.len
    encoded = encode_base64(attr.bufptr, length)
    be_api.be_pushstring(vm, bytes(encoded).decode('ascii'))
    return be_api.be_returnvalue(vm)

# static int m_fromb64(bvm *vm) { ... }
def m_fromb64(vm):
    be_api = _lazy_be_api()
    argc = be_api.be_top(vm)
    if argc >= 2 and be_api.be_isstring(vm, 2):
        s = be_api.be_tostring(vm, 2)
        s_bytes = s.encode('ascii') if isinstance(s, str) else s
        bin_len = decode_base64_length(s_bytes)
        attr = m_read_attributes(vm, 1)
        check_ptr_modifiable(vm, attr)
        if attr.fixed and attr.len != bin_len:
            be_api.be_raise(vm, BYTES_RESIZE_ERROR, BYTES_RESIZE_MESSAGE)
        bytes_resize(vm, attr, bin_len)
        if bin_len > attr.size:
            be_api.be_raise(vm, "memory_error", "cannot allocate buffer")
        decoded = decode_base64(s_bytes)
        attr.bufptr[:len(decoded)] = decoded
        attr.len = len(decoded)
        be_api.be_pop(vm, 1)
        m_write_attributes(vm, 1, attr)
        return be_api.be_returnvalue(vm)
    be_api.be_raise(vm, "type_error", "operand must be a string")
    return be_api.be_returnnilvalue(vm)

# static int m_fromhex(bvm *vm) { ... }
def m_fromhex(vm):
    be_api = _lazy_be_api()
    argc = be_api.be_top(vm)
    if argc >= 2 and be_api.be_isstring(vm, 2):
        from_skip = 0
        if argc >= 3 and be_api.be_isint(vm, 3):
            from_skip = be_api.be_toint(vm, 3)
        s = be_api.be_tostring(vm, 2)
        s_len = len(s)
        if from_skip < 0: from_skip = 0
        if from_skip > s_len: from_skip = s_len
        bin_len = (s_len - from_skip) // 2
        attr = m_read_attributes(vm, 1)
        check_ptr_modifiable(vm, attr)
        if attr.fixed and attr.len != bin_len:
            be_api.be_raise(vm, BYTES_RESIZE_ERROR, BYTES_RESIZE_MESSAGE)
        bytes_resize(vm, attr, bin_len)
        if bin_len > attr.size:
            be_api.be_raise(vm, "memory_error", "cannot allocate buffer")
        attr.len = 0
        buf_add_hex(attr, s[from_skip:], s_len - from_skip)
        be_api.be_pop(vm, 1)
        m_write_attributes(vm, 1, attr)
        be_api.be_pop(vm, be_api.be_top(vm) - 1)
        return be_api.be_returnvalue(vm)
    be_api.be_raise(vm, "type_error", "operand must be a string")
    return be_api.be_returnnilvalue(vm)


# ============================================================================
# Advanced API
# ============================================================================

# static int m_buffer(bvm *vm) { ... }
def m_buffer(vm):
    be_api = _lazy_be_api()
    attr = m_read_attributes(vm, 1)
    be_api.be_pushcomptr(vm, attr.bufptr)
    return be_api.be_returnvalue(vm)

# static int m_is_mapped(bvm *vm) { ... }
def m_is_mapped(vm):
    be_api = _lazy_be_api()
    attr = m_read_attributes(vm, 1)
    mapped = attr.mapped or (attr.bufptr is None)
    be_api.be_pushbool(vm, mapped)
    return be_api.be_returnvalue(vm)

# static int m_is_readonly(bvm *vm) { ... }
def m_is_readonly(vm):
    be_api = _lazy_be_api()
    attr = m_read_attributes(vm, 1)
    be_api.be_pushbool(vm, attr.solidified)
    return be_api.be_returnvalue(vm)

# static int m_change_buffer(bvm *vm) { ... }
def m_change_buffer(vm):
    be_api = _lazy_be_api()
    argc = be_api.be_top(vm)
    if argc >= 2 and be_api.be_iscomptr(vm, 2):
        attr = m_read_attributes(vm, 1)
        if attr.solidified:
            be_api.be_raise(vm, "value_error", BYTES_READ_ONLY_MESSAGE)
        if not attr.mapped:
            be_api.be_raise(vm, "type_error", "bytes() object must be mapped")
        attr.bufptr = be_api.be_tocomptr(vm, 2)
        m_write_attributes(vm, 1, attr)
        be_api.be_pushcomptr(vm, attr.bufptr)
        return be_api.be_returnvalue(vm)
    be_api.be_raise(vm, "type_error", "operand must be a comptr")
    return be_api.be_returnnilvalue(vm)


# ============================================================================
# External API
# ============================================================================

# BERRY_API void * be_pushbytes(bvm *vm, const void * bytes, size_t len) { ... }
def be_pushbytes(vm, data, length):
    be_api = _lazy_be_api()
    bytes_new_object(vm, length)
    attr = m_read_attributes(vm, -1)
    check_ptr(vm, attr)
    actual_len = min(length, attr.size)
    if data is not None:
        for i in range(actual_len):
            attr.bufptr[i] = data[i] if isinstance(data[i], int) else ord(data[i])
    else:
        for i in range(actual_len):
            attr.bufptr[i] = 0
    attr.len = actual_len
    m_write_attributes(vm, -1, attr)
    return attr.bufptr

# BERRY_API const void *be_tobytes(bvm *vm, int rel_index, size_t *len) { ... }
def be_tobytes(vm, rel_index):
    """Returns (bufptr, length) or (None, 0)."""
    be_api = _lazy_be_api()
    index = be_api.be_absindex(vm, rel_index)
    if be_isbytes(vm, index):
        attr = m_read_attributes(vm, index)
        check_ptr(vm, attr)
        return (attr.bufptr, attr.len)
    return (None, 0)

# BERRY_API bbool be_isbytes(bvm *vm, int rel_index) { ... }
def be_isbytes(vm, rel_index):
    be_api = _lazy_be_api()
    index = be_api.be_absindex(vm, rel_index)
    if be_api.be_isinstance(vm, index):
        be_api.be_getbuiltin(vm, "bytes")
        ret = be_api.be_isderived(vm, index)
        be_api.be_pop(vm, 1)
        return ret
    return False


# ============================================================================
# Library loader
# ============================================================================

# void be_load_byteslib(bvm *vm) { ... }
def be_load_byteslib(vm):
    be_api = _lazy_be_api()
    members = [
        (".p", None),
        (".len", None),
        (".size", None),
        ("_buffer", m_buffer),
        ("_change_buffer", m_change_buffer),
        ("ismapped", m_is_mapped),
        ("isreadonly", m_is_readonly),
        ("init", m_init),
        ("deinit", m_deinit),
        ("tostring", m_tostring),
        ("asstring", m_asstring),
        ("tobool", m_tobool),
        ("fromstring", m_fromstring),
        ("tob64", m_tob64),
        ("fromb64", m_fromb64),
        ("fromhex", m_fromhex),
        ("tohex", m_tohex),
        ("add", m_add),
        ("get", m_getu),
        ("geti", m_geti),
        ("set", m_set),
        ("seti", m_set),
        ("setbytes", m_setbytes),
        ("getfloat", m_getfloat),
        ("setfloat", m_setfloat),
        ("addfloat", m_addfloat),
        ("item", m_item),
        ("setitem", m_setitem),
        ("size", m_size),
        ("resize", m_resize),
        ("clear", m_clear),
        ("reverse", m_reverse),
        ("copy", m_copy),
        ("append", m_connect),
        ("appendhex", m_appendhex),
        ("appendb64", m_appendb64),
        ("+", m_merge),
        ("..", m_connect),
        ("==", m_equal),
        ("!=", m_nequal),
    ]
    be_api.be_regclass(vm, "bytes", members)
