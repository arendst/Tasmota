#
# Matter_TLV.be - implements the encoding and decoding of Matter TLV structures (Tag/Lenght/Value) Appendix A.
#
# Copyright (C) 2023  Stephan Hadinger & Theo Arends
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

# Support for Matter Protocol: TLV encoding and decoding

import matter

#@ solidify:Matter_TLV.Matter_TLV_item,weak
#@ solidify:Matter_TLV.Matter_TLV_list,weak
#@ solidify:Matter_TLV.Matter_TLV_struct,weak
#@ solidify:Matter_TLV.Matter_TLV_array,weak
#@ solidify:Matter_TLV,weak

class Matter_TLV
  static var _type = [
    'i1', 'i2', 'i4', 'i8', 'u1', 'u2', 'u4', 'u8',   # signed and unsigned 0x00-0x07
    'bool', 'bool', 'float', 'double',                # 0x08-0x0B
    'UTF1', 'UTF2', 'UTF4', 'UTF8',                   # 0x0C-0x0F
    'b1', 'b2', 'b4', 'b8',                           # 0x10-0x13
    'null', 'struct', 'array', 'list', 'end'          # 0x14-0x18
    # all other are reserved
]

  static var _len  = [
    1, 2, 4, 8, 1, 2, 4, 8,
    0, 0, 4, 8,
    -1, -2, -4, -8,
    -1, -2, -4, -8,
    0, -99, -99, -99, 0
  ]

  # type values (enum like)
  #
  # Type|Description
  # :----|:---
  # I1 I2 I4|Signed integer of at most (1/2/4) bytes (as 32 bits signed Berry type)
  # U1 U2 U4|Unsiged integer of at motst (1/2/4) bytes (as 32 bits signed Berry type, be careful when comparing. Use `matter.Counter.is_greater(a,b)`)
  # I8 U8|Signed/insigned 8 bytes. You can pass `bytes(8)`, `int64()` or `int`. Type is collapsed to a lower type if possible when encoding.
  # BOOL|boolean, takes `true` and `false`. Abstracts the internal `BTRUE` and `BFALSE` that you don't need to use
  # FLOAT|32 bites float
  # UTF1 UTF2|String as UTF, size is encoded as 1 or 2 bytes automatically
  # B1 B2|raw `bytes()`, size is encoded as 1 or 2 bytes automatically
  # NULL|takes only `nil` value
  # STRUCT<BR>ARRAY<BR>LIST<BR>EOC|(internal) Use through abstractions
  # DOUBLE<BR>UTF4 UTF8<BR>B4 B8|Unsuppored in Tasmota

  static var I1     = 0x00
  static var I2     = 0x01
  static var I4     = 0x02
  static var I8     = 0x03
  static var U1     = 0x04
  static var U2     = 0x05
  static var U4     = 0x06
  static var U8     = 0x07
  static var BOOL   = 0x08    # when encoding, use indifferentiate
  static var BFALSE = 0x08
  static var BTRUE  = 0x09
  static var FLOAT  = 0x0A
  static var DOUBLE = 0x0B
  static var UTF1   = 0x0C
  static var UTF2   = 0x0D
  static var UTF4   = 0x0E
  static var UTF8   = 0x0F
  static var B1     = 0x10
  static var B2     = 0x11
  static var B4     = 0x12
  static var B8     = 0x13
  static var NULL   = 0x14
  static var STRUCT = 0x15
  static var ARRAY  = 0x16
  static var LIST   = 0x17
  static var EOC    = 0x18
  static var RAW    = 0xFF  # encodes an anonymous raw value (already encoded in TLV to save memory)

  #################################################################################
  # Matter_TLV_item class
  #################################################################################
  static class Matter_TLV_item
    # we keep a shortcut reference to the Matter_TLV class
    static var TLV = Matter_TLV
    static var is_list = false
    static var is_array = false
    static var is_struct = false
    # parent tag to inherit vendor/profile/tag
    var parent
    var next_idx              # next idx in buffer (when parsing)
    # tags
    var tag_vendor            # 16 bit VendorID [opt]
    var tag_profile           # 16 bit profile number [opt]
    var tag_number            # 32 bit tag number
    var tag_sub               # context specific tag 8 bit [opt]
    # type
    var typ                   # TLV type number, set at decoding, mandatory for encoding
    # value
    var val                   # multi-type: int, float, bool, bytes, map, list

    #############################################################
    # constructor
    def init(parent)
      self.parent = parent
    end

    #############################################################
    # reset - allows reuse of the object
    def reset(parent)
      var n = nil
      self.parent = parent
      self.next_idx = n
      self.tag_vendor = n
      self.tag_profile = n
      self.tag_number = n
      self.tag_sub = n
      self.typ = n
      self.val = n
    end
    
    #############################################################
    # set value, equivalent to create_TLV() without allocation
    #
    def set(t, value)
      self.reset()
      if value != nil || t == 0x14 #-t == matter.TLV.NULL-#   # put the actual number for performance
        self.typ = t
        self.val = value
        return self
      end
    end
    
    #############################################################
    # neutral converter
    def to_TLV()
      return self
    end

    #############################################################
    # create simple TLV
    static def create_TLV(t, value)
      if value != nil || t == 0x14 #-t == matter.TLV.NULL-#   # put the actual number for performance
        var v = _class()      # parent is nil
        v.typ = t
        v.val = value
        return v
      end
    end

    #############################################################
    # tostring
    #
    # We are trying to follow the official Matter way of printing TLV
    # Ex: '42U' or '1 = 42U' or '0xFFF1::0xDEED:0xAA55FEED = 42U'
    def tostring(no_tag)
      # var s = "<instance: Matter_TLV_item("
      var s = ""
      try       # any exception raised in `tostring()` causes a crash, so better catch it here

        if no_tag != true
          if self.tag_profile == -1
            s += "Matter::"
            if self.tag_number != nil   s += format("0x%08X ", self.tag_number) end
          else
            if self.tag_vendor != nil   s += format("0x%04X::", self.tag_vendor) end
            if self.tag_profile != nil   s += format("0x%04X:", self.tag_profile) end
            if self.tag_number != nil   s += format("0x%08X ", self.tag_number) end
            if self.tag_sub != nil   s += format("%i ", self.tag_sub) end
          end
          if size(s) > 0    s += "= " end
        end

        # print value
        if type(self.val) == 'int'        s += format("%i", self.val)
          if self.typ >= self.TLV.U1 && self.typ <= self.TLV.U8   s += "U" end
        elif type(self.val) == 'bool'     s += self.val ? "true" : "false"
        elif self.val == nil              s += "null"
        elif type(self.val) == 'real'     s += format("%g", self.val)
        elif type(self.val) == 'string'   s += format('"%s"', self.val)
        elif isinstance(self.val, int64)  s += self.val.tostring()
          if self.typ >= self.TLV.U1 && self.typ <= self.TLV.U8   s += "U" end
        elif type(self.val) == 'instance'
          s += format("%s", self.val.tohex())
        end
        
      except .. as e, m
        return e + " " + m
      end
      return s
    end

    # simplified version of tostring() for simple values
    def to_str_val()
      # print value
      if type(self.val) == 'int'
        if self.typ >= self.TLV.U1 && self.typ <= self.TLV.U8
          return str(self.val) + "U"
        else
          return str(self.val)
        end
      elif type(self.val) == 'bool'     return self.val ? "true" : "false"
      elif self.val == nil              return "null"
      elif type(self.val) == 'real'     return str(self.val)
      elif type(self.val) == 'string'   return self.val
      elif isinstance(self.val, int64)
        if self.typ >= self.TLV.U1 && self.typ <= self.TLV.U8
          return self.val.tostring() + "U"
        else
          return self.val.tostring()
        end
      elif type(self.val) == 'instance'
        return self.tostring(true)
      end
    end

    #############################################################
    # parse a bytes() array from `idx`
    # args:
    #    b: bytes() buffer
    #    idx: starting index in the bytes() buffer
    #    parent: (optional) the parent object to inherit tag values
    #
    # returns the next `idx` for the item following or `-1` if error
    # The next `idx` is also stored in `self.next_idx`
    def parse(b, idx)
      var item_type = self.typ
      # parse LV
      var TLV = self.TLV        # cache value in register
      var item_len = TLV._len[item_type]
    
      if item_len == 8                              # i64 / u64 / double
        self.val = int64()
        self.val.frombytes(b, idx)
        idx += 8
      elif item_type == TLV.BFALSE || item_type == TLV.BTRUE   # bool
        self.val =  (item_type == TLV.BTRUE)
      elif item_type < TLV.U8                         # i1/i2/i4 u1/u2/u4
        self.val = item_type <= TLV.I8 ? b.geti(idx, item_len) : b.get(idx, item_len)
        idx += item_len
      elif item_type == TLV.FLOAT                        # float
        self.val = b.getfloat(idx)
        idx += 4
      elif item_len >= -8 && item_len <= -1         # len prefix 1/2/4/8
        # TODO skip len == 8 for now
        var b_len = b.get(idx, -item_len)
        idx += -item_len
        self.val = b[idx .. idx + b_len - 1]
        idx += b_len
        if (item_type <= TLV.UTF8)  self.val = self.val.asstring() end
      elif item_type == TLV.NULL                       # null
        # do nothing
      elif item_type == TLV.EOC
        tasmota.log("MTR: unexpected eoc", 3)
      else
        tasmota.log("MTR: unexpected type: " + str(item_type), 3)
      end
      self.next_idx = idx
      return idx
    end

    #############################################################
    # encode TLV
    #
    # appends to the bytes() object
    def tlv2raw(b)
      var TLV = self.TLV
      if b == nil   b = bytes() end     # start new buffer if none passed

      if self.typ == TLV.RAW  b..self.val return b   end

      # special case for bool
      # we need to change the type according to the value
      if self.typ == TLV.BFALSE || self.typ == TLV.BTRUE
        self.typ = bool(self.val) ? TLV.BTRUE : TLV.BFALSE
      # try to compress ints
      elif self.typ >= TLV.I2 && self.typ <= TLV.I4
        var i = int(self.val)
        if   i <= 127 && i >= -128      self.typ = TLV.I1
        elif i <= 32767 && i >= -32768  self.typ = TLV.I2
        end
      elif self.typ >= TLV.U2 && self.typ <= TLV.U4
        var i = int(self.val)
        if   i <= 255 && i >= 0         self.typ = TLV.U1
        elif i <= 65535 && i >= 0       self.typ = TLV.U2
        end
      elif self.typ >= TLV.B1 && self.typ <= TLV.B8       # encode length as minimum possible
        if size(self.val) <= 255
          self.typ = TLV.B1
        elif size(self.val) <= 65535
          self.typ = TLV.B2
        else
          self.typ = TLV.B4     # B4 is unlikely, B8 is impossible
        end
      elif self.typ >= TLV.UTF1 && self.typ <= TLV.UTF8
        if size(self.val) <= 255
          self.typ = TLV.UTF1
        elif size(self.val) <= 65535
          self.typ = TLV.UTF2
        else
          self.typ = TLV.UTF4     # UTF4 is unlikely, UTF8 is impossible
        end
      end

      # encode tag and type
      self._encode_tag(b)
      # encode value

      if self.typ == TLV.I1 || self.typ == TLV.U1
        b.add(int(self.val), 1)
      elif self.typ == TLV.I2 || self.typ == TLV.U2
        b.add(int(self.val), 2)
      elif self.typ == TLV.I4 || self.typ == TLV.U4
        b.add(int(self.val), 4)
      elif self.typ == TLV.I8 || self.typ == TLV.U8
        # I8/U8 can be encoded from bytes(8)/int64/int
        var i64 = self.val
        if isinstance(i64, bytes)
          i64 = i64.copy().resize(8)     # bytes(8)
        elif isinstance(i64, int64)
          i64 = i64.tobytes()             # bytes(8)
        else
          i64 = int64(int(i64)).tobytes()  # bytes(8)
        end
        b .. i64
      elif self.typ == TLV.BFALSE || self.typ == TLV.BTRUE
        # push nothing
      elif self.typ == TLV.FLOAT
        var idx = size(b)
        b.add(0, 4)
        b.setfloat(idx, real(self.val))
      elif self.typ == TLV.DOUBLE
        raise "value_error", "Unsupported type TLV.DOUBLE"
      elif self.typ == TLV.UTF1
        if size(self.val) > 255 raise "value_error", "string too big" end
        b.add(size(self.val), 1)
        b..bytes().fromstring(str(self.val))
      elif self.typ == TLV.UTF2
        if size(self.val) > 65535 raise "value_error", "string too big" end
        b.add(size(self.val), 2)
        b..bytes().frostring(str(self.val))
      elif self.typ == TLV.B1
        if size(self.val) > 255 raise "value_error", "bytes too big" end
        b.add(size(self.val), 1)
        b..self.val
      elif self.typ == TLV.B2
        if size(self.val) > 65535 raise "value_error", "bytes too big" end
        b.add(size(self.val), 2)
        b..self.val
      elif self.typ == TLV.NULL
        # push nothing
      else
        raise "value_error", "unsupported type " + str(self.typ)
      end

      return b
    end

    #############################################################
    # compute the length in bytes of encoded TLV without actually
    # allocating buffers (faster and no memory fragmentation)
    #
    # returns a number of bytes
    def encode_len()
      var TLV = self.TLV
      var len = 0

      if self.typ == TLV.RAW  return size(self.val)   end

      # special case for bool
      # we need to change the type according to the value
      if self.typ == TLV.BFALSE || self.typ == TLV.BTRUE
        self.typ = bool(self.val) ? TLV.BTRUE : TLV.BFALSE
      # try to compress ints
      elif self.typ >= TLV.I2 && self.typ <= TLV.I4
        var i = int(self.val)
        if   i <= 127 && i >= -128      self.typ = TLV.I1
        elif i <= 32767 && i >= -32768  self.typ = TLV.I2
        end
      elif self.typ >= TLV.U2 && self.typ <= TLV.U4
        var i = int(self.val)
        if   i <= 255 && i >= 0         self.typ = TLV.U1
        elif i <= 65535 && i >= 0       self.typ = TLV.U2
        end
      elif self.typ >= TLV.B1 && self.typ <= TLV.B8       # encode length as minimum possible
        if size(self.val) <= 255
          self.typ = TLV.B1
        elif size(self.val) <= 65535
          self.typ = TLV.B2
        else
          self.typ = TLV.B4     # B4 is unlikely, B8 is impossible
        end
      elif self.typ >= TLV.UTF1 && self.typ <= TLV.UTF8
        if size(self.val) <= 255
          self.typ = TLV.UTF1
        elif size(self.val) <= 65535
          self.typ = TLV.UTF2
        else
          self.typ = TLV.UTF4     # UTF4 is unlikely, UTF8 is impossible
        end
      end

      # encode tag and type
      len += self._encode_tag_len()
      # encode value

      if self.typ == TLV.I1 || self.typ == TLV.U1
        len += 1
      elif self.typ == TLV.I2 || self.typ == TLV.U2
        len += 2
      elif self.typ == TLV.I4 || self.typ == TLV.U4
        len += 4
      elif self.typ == TLV.I8 || self.typ == TLV.U8
        len += 8
      elif self.typ == TLV.BFALSE || self.typ == TLV.BTRUE
        # push nothing
      elif self.typ == TLV.FLOAT
        len += 4
      elif self.typ == TLV.DOUBLE
        raise "value_error", "Unsupported type TLV.DOUBLE"
      elif self.typ == TLV.UTF1
        len += 1 + size(self.val)
      elif self.typ == TLV.UTF2
        len += 2 + size(self.val)
      elif self.typ == TLV.B1
        len += 1 + size(self.val)
      elif self.typ == TLV.B2
        len += 2 + size(self.val)
      elif self.typ == TLV.NULL
        # push nothing
      else
        raise "value_error", "unsupported type " + str(self.typ)
      end

      return len
    end

    #############################################################
    # internal_function
    # encode Tag+Type as the first bytes
    def _encode_tag(b)
      var tag_number = self.tag_number != nil ? self.tag_number : 0
      var tag_huge = (tag_number >= 65536) || (tag_number < 0)
      var tag_control = 0x00
      if self.tag_vendor != nil
        # full encoding
        if tag_huge
          b.add(0xE0 + self.typ, 1)
          b.add(self.tag_vendor, 2)
          b.add(self.tag_profile, 2)
          b.add(self.tag_number, 4)
        else
          b.add(0xC0 + self.typ, 1)
          b.add(self.tag_vendor, 2)
          b.add(self.tag_profile, 2)
          b.add(self.tag_number, 2)
        end
      elif self.tag_profile == -1       # Matter Common profile
        if tag_huge
          b.add(0x60 + self.typ, 1)
          b.add(self.tag_number, 4)
        else
          b.add(0x40 + self.typ, 1)
          b.add(self.tag_number, 2)
        end
      elif self.tag_profile != nil
        if tag_huge
          b.add(0xA0 + self.typ, 1)
          b.add(self.tag_number, 4)
        else
          b.add(0x80 + self.typ, 1)
          b.add(self.tag_number, 2)
        end
      elif self.tag_sub != nil
        b.add(0x20 + self.typ, 1)
        b.add(self.tag_sub, 1)
      else    # anonymous tag
        b.add(0x00 + self.typ, 1)
      end
    end

    #############################################################
    # internal_function
    # compute len of Tag+Type as the first bytes
    def _encode_tag_len()
      var tag_number = self.tag_number != nil ? self.tag_number : 0
      var tag_huge = (tag_number >= 65536) || (tag_number < 0)
      var tag_control = 0x00
      if self.tag_vendor != nil
        # full encoding
        if tag_huge
          return 9
        else
          return 7
        end
      elif self.tag_profile == -1       # Matter Common profile
        if tag_huge
          return 5
        else
          return 3
        end
      elif self.tag_profile != nil
        if tag_huge
          return 5
        else
          return 3
        end
      elif self.tag_sub != nil
        return 2
      else    # anonymous tag
        return 1
      end
    end

    #############################################################
    # Compare the value index with an element
    # returns:
    #   1 if self is strictly greater than k
    #   0 if equal or lower
    def _cmp_gt(k)
      # compare tag_vendor
      if self.tag_vendor != nil
        if k.tag_vendor == nil                return 1 end
        if self.tag_vendor > k.tag_vendor     return 1 end
        if self.tag_vendor == k.tag_vendor
          if self.tag_profile > k.tag_profile return 1 end
        end
        # continue to tag comparison
      end

      # Matter common profile
      if self.tag_profile == -1
        if k.tag_profile == nil               return 1 end
      elif self.tag_profile == nil
        if k.tag_profile == -1                return 0 end
      end

      if self.tag_number != nil
        if k.tag_number == nil                return 1 end
        if self.tag_number > k.tag_number     return 1 end
        return 0
      end

      if self.tag_sub != nil
        if k.tag_sub == nil                   return 1 end
        if self.tag_sub > k.tag_sub           return 1 end
      end

      return 0
    end

    #################################################################################
    # Simple insertion sort - sorts the list in place, and returns the list
    #################################################################################
    static def sort(l)
      # insertion sort
      for i:1..size(l)-1
        var k = l[i]
        var j = i
        while (j > 0) && (l[j-1]._cmp_gt(k) > 0)
          l[j] = l[j-1]
          j -= 1
        end
        l[j] = k
      end
      return l
    end

    #############################################################
    # set parent
    def set_parent(parent)
      self.parent = parent
    end  

    #############################################################
    # Setters for tags
    def set_fulltag(vendor, profile, tag)
      self.tag_vendor = int(vendor)
      self.tag_profile = int(profile)
      self.tag_number = int(tag)
      self.tag_sub = nil
    end
    # set context specific
    def set_anonymoustag()
      self.set_fulltag()
    end
    # set common profile
    def set_commonprofile()
      self.set_fulltag(nil, -1, nil)
    end
    # set context specific
    def set_contextspecific(n)
      self.set_fulltag()
      self.tag_sub = int(n)
    end
  end

# class Matter_TLV_array var _ end
# class Matter_TLV_struct var _ end

  static class Matter_TLV_list : Matter_TLV_item
    # inherited
    static var is_list = true
    # static var is_array = false
    # static var is_struct = false

    #################################################################################
    def init(parent)
      super(self).init(parent)
      self.typ = self.TLV.LIST
      self.val = []
    end

    #################################################################################
    def tostring(no_tag)
      return self.tostring_inner(false, "[[", "]]", no_tag)
    end

    def tostring_inner(sorted, pre, post, no_tag)
      var s = ""
      try

        if no_tag != true
          if self.tag_profile == -1
            s += "Matter::"
            if self.tag_number != nil   s += format("0x%08X ", self.tag_number) end
          else
            if self.tag_vendor != nil   s += format("0x%04X::", self.tag_vendor) end
            if self.tag_profile != nil   s += format("0x%04X:", self.tag_profile) end
            if self.tag_number != nil   s += format("0x%08X ", self.tag_number) end
            if self.tag_sub != nil   s += format("%i ", self.tag_sub) end
          end
          if size(s) > 0    s += "= " end
        end

        s += pre

        # sort values
        var val_list = self.val.copy()
        if sorted
          self.sort(val_list)
        end

        s += val_list.concat(", ")

        s += post

      except .. as e, m
        return e + " " + m
      end
      return s
    end

    # simplified version of tostring() for simple values
    def to_str_val()
      return self.tostring(true)
    end

    #################################################################################
    def parse(b, idx)
      # iterate until end of struct
      while b[idx] != self.TLV.EOC
        # read next
        var item = self.TLV.parse(b, idx, self)
        idx = item.next_idx

        self.val.push(item)
      end
      idx += 1

      self.next_idx = idx
      return idx
    end

    #############################################################
    # encode to bytes
    def tlv2raw(b)
      if b == nil   b = bytes() end
      # encode tag and type
      self._encode_tag(b)
      # sort values
      var val_list = self.val
      if self.is_struct
        val_list = val_list.copy()
        self.sort(val_list)
      end

      # output each one after the other
      for v : val_list
        if isinstance(v, bytes)
          b .. v
        else
          v.tlv2raw(b)
        end
      end

      # add 'end of container'
      b.add(self.TLV.EOC, 1)

      return b
    end

    #############################################################
    # compute the length in bytes of encoded TLV without actually
    # allocating buffers (faster and no memory fragmentation)
    #
    # returns a number of bytes
    def encode_len()
      # tag and type
      var len = self._encode_tag_len()
      # output each one after the other, order doesn't infulence size
      var idx = 0
      while idx < size(self.val)
        len += self.val[idx].encode_len()
        idx += 1
      end
      # add 'end of container'
      len += 1
      return len
    end

    #############################################################
    # Getters
    #
    # get by index
    def item(n)
      return self.val[n]
    end
    def setitem(n, v)
      self.val[n] = v
    end
    def push(v)
      self.val.push(v)
    end
    def size()
      return size(self.val)
    end

    #############################################################
    # get by sub-tag, return nil if not found
    def findsub(n, v)
      for k : self.val
        if k.tag_sub == n     return k end
      end
      return v
    end
    def findsubval(n, v)
      var r = self.findsub(n)
      if r != nil   return r.val end
      return v
    end
    def findsubtyp(n)
      var r = self.findsub(n)
      if r != nil   return r.typ end
      return nil
    end
    def getsub(n)
      var v = self.findsub(n)
      if v == nil raise "value_error", "sub not found" end
      return v
    end
    def getsubval(n)
      return self.getsub(n).val
    end

    #############################################################
    # adders
    def add_TLV(tag, t, value)
      if value != nil || t == matter.TLV.NULL
        var v = self.TLV.Matter_TLV_item(self)
        v.tag_sub = tag
        v.typ = t
        v.val = value
        self.val.push(v)
      end
      return self
    end

    # add on object that implements `to_TLV()` and adds to the current container
    #
    # obj can be `nil`, in such case nothing happens
    # returns `self` to allow calls to be chained
    def add_obj(tag, obj)
      if obj != nil
        if isinstance(obj, bytes)
          self.val.push(obj)
        else
          var value = obj.to_TLV()
          value.tag_sub = tag
          self.val.push(value)
        end
      end
      return self
    end

    def add_list(tag)
      var s = self.TLV.Matter_TLV_list(self)
      s.tag_sub = tag
      self.val.push(s)
      return s
    end

    def add_array(tag)
      var s = self.TLV.Matter_TLV_array(self)
      s.tag_sub = tag
      self.val.push(s)
      return s
    end

    def add_struct(tag)
      var s = self.TLV.Matter_TLV_struct(self)
      s.tag_sub = tag
      self.val.push(s)
      return s
    end

  end

  #################################################################################
  # Matter_TLV_struct class
  #################################################################################
  static class Matter_TLV_struct : Matter_TLV_list
    static var is_list = false
  # static var is_array = false
    static var is_struct = true

    def init(parent)
      super(self).init(parent)
      self.typ = self.TLV.STRUCT
      self.val = []
    end

    #############################################################
    def tostring(no_tag)
      return self.tostring_inner(true, "{", "}", no_tag)
    end
  end

  #################################################################################
  # Matter_TLV_array class
  #################################################################################
  static class Matter_TLV_array : Matter_TLV_list
    static var is_list = false
    static var is_array = true
  # static var is_struct = false

    def init(parent)
      super(self).init(parent)
      self.typ = self.TLV.ARRAY
      self.val = []
    end

    #############################################################
    def tostring(no_tag)
      return self.tostring_inner(false, "[", "]", no_tag)
    end

    #############################################################
    def parse(b, idx)
      # iterate until end of struct
      while b[idx] != self.TLV.EOC
        # read next
        var item = self.TLV.parse(b, idx, self)
        idx = item.next_idx

        # for arrays, all tags must be anonymous
        item.tag_vendor = nil
        item.tag_profile = nil
        item.tag_number = nil
        item.tag_sub = nil
        self.val.push(item)
      end
      idx += 1

      self.next_idx = idx
      return idx
    end
  end

  #################################################################################
  # bookkeeping
  #################################################################################
  # Matter_TLV.Matter_TLV_item = Matter_TLV_item
  # Matter_TLV.Matter_TLV_struct = Matter_TLV_struct
  # Matter_TLV.Matter_TLV_array = Matter_TLV_array
  # Matter_TLV.Matter_TLV_list = Matter_TLV_list

  # parse a bytes() array from `idx`
  # args:
  #    b: bytes() buffer
  #    idx: starting index in the bytes() buffer
  #    parent: (optional) the parent object to inherit tag values
  # returns the next `idx` for the item following or `-1` if error
  static def parse(b, idx, parent)
    var TLV = _class
    if idx == nil   idx = 0 end
    # read type and tag_control
    var item_type = b[idx] & 0x1F         # values 0x00 - 0x1F
    var item_tag_control = b[idx] & 0xE0  # values 0x20 - 0xE0
    idx += 1                              # skip tag/type byte

    if (item_type > TLV.EOC)  raise "TLV_error", "invalid TLV type "+str(item_type) end

    # ############################################################
    # instanciate TLV item or struct
    var item
    if item_type == TLV.STRUCT
      item = _class.Matter_TLV_struct(parent)
    elif item_type == TLV.ARRAY
      item = _class.Matter_TLV_array(parent)
    elif item_type == TLV.LIST
      item = _class.Matter_TLV_list(parent)
    else
      item = _class.Matter_TLV_item(parent)
    end
    item.typ = item_type                 # set type for item

    # ############################################################
    # parse Tag and length
    # do we have a vendor?
    if item_tag_control == 0xC0 || item_tag_control == 0xE0
      item.tag_vendor = b.get(idx, 2)
      item.tag_profile = b.get(idx + 2, 2)
      idx += 4
    end
    # Common profile tags
    if item_tag_control == 0x40 || item_tag_control == 0x60
      item.tag_vendor = nil
      item.tag_profile = -1
    end
    # Tags
    if item_tag_control == 0x00
      # pass
    elif item_tag_control == 0x20
      # context specific tag
      item.tag_sub = b[idx]
      idx += 1
    elif item_tag_control == 0xC0 || item_tag_control == 0x80 || item_tag_control == 0x40
      item.tag_number = b.get(idx, 2)
      idx += 2
    else
      item.tag_number = b.get(idx, 4)
      idx += 4
    end

    # ############################################################
    # parse Value
    idx = item.parse(b, idx)
    
    return item
  end
  #############################################################
  # create simple TLV
  static def create_TLV(t, value)
    return _class.Matter_TLV_item.create_TLV(t, value)
  end
end

# add to matter
import matter
matter.TLV = Matter_TLV

#-

# Test
import matter

def test_TLV(b, s)
  var m = matter.TLV.parse(b)
  assert(m.tostring() == s)
  assert(m.tlv2raw() == b)
  assert(m.encode_len() == size(b))
end

test_TLV(bytes("2502054C"), "2 = 19461U")
test_TLV(bytes("0001"), "1")
test_TLV(bytes("08"), "false")
test_TLV(bytes("09"), "true")

var TLV = matter.TLV
assert(TLV.create_TLV(TLV.BOOL, 1).tlv2raw() == bytes("09"))
assert(TLV.create_TLV(TLV.BOOL, true).tlv2raw() == bytes("09"))
assert(TLV.create_TLV(TLV.BOOL, 0).tlv2raw() == bytes("08"))
assert(TLV.create_TLV(TLV.BOOL, false).tlv2raw() == bytes("08"))

test_TLV(bytes("00FF"), "-1")
test_TLV(bytes("05FFFF"), "65535U")

test_TLV(bytes("0A0000C03F"), "1.5")
test_TLV(bytes("0C06466f6f626172"), '"Foobar"')
test_TLV(bytes("1006466f6f626172"), "466F6F626172")
test_TLV(bytes("e4f1ffeddeedfe55aa2a"), "0xFFF1::0xDEED:0xAA55FEED = 42U")
test_TLV(bytes("300120D2DAEE8760C9B1D1B25E0E2E4DD6ECA8AEF6193C0203761356FCB06BBEDD7D66"), "1 = D2DAEE8760C9B1D1B25E0E2E4DD6ECA8AEF6193C0203761356FCB06BBEDD7D66")

# context specific
test_TLV(bytes("24012a"), "1 = 42U")
test_TLV(bytes("4401002a"), "Matter::0x00000001 = 42U")

# int64
test_TLV(bytes("030102000000000000"), "513")
test_TLV(bytes("070102000000000000"), "513U")
test_TLV(bytes("03FFFFFFFFFFFFFFFF"), "-1")
test_TLV(bytes("07FFFFFFFFFFFFFF7F"), "9223372036854775807U")

# structure
test_TLV(bytes("1518"), "{}")
test_TLV(bytes("15300120D2DAEE8760C9B1D1B25E0E2E4DD6ECA8AEF6193C0203761356FCB06BBEDD7D662502054C240300280418"), "{1 = D2DAEE8760C9B1D1B25E0E2E4DD6ECA8AEF6193C0203761356FCB06BBEDD7D66, 2 = 19461U, 3 = 0U, 4 = false}")
test_TLV(bytes("15300120D2DAEE8760C9B1D1B25E0E2E4DD6ECA8AEF6193C0203761356FCB06BBEDD7D662502054C240300280435052501881325022C011818"), "{1 = D2DAEE8760C9B1D1B25E0E2E4DD6ECA8AEF6193C0203761356FCB06BBEDD7D66, 2 = 19461U, 3 = 0U, 4 = false, 5 = {1 = 5000U, 2 = 300U}}")

# list
test_TLV(bytes("1718"), "[[]]")
test_TLV(bytes("17000120002a000200032000ef18"), "[[1, 0 = 42, 2, 3, 0 = -17]]")

# array
test_TLV(bytes("1618"), "[]")
test_TLV(bytes("160000000100020003000418"), "[0, 1, 2, 3, 4]")

# mix
test_TLV(bytes("16002a02f067fdff15180a33338f410c0648656c6c6f2118"), '[42, -170000, {}, 17.9, "Hello!"]')
test_TLV(bytes("153600172403312504FCFF18172402002403302404001817240200240330240401181724020024033024040218172402002403302404031817240200240328240402181724020024032824040418172403312404031818280324FF0118"), '{0 = [[[3 = 49U, 4 = 65532U]], [[2 = 0U, 3 = 48U, 4 = 0U]], [[2 = 0U, 3 = 48U, 4 = 1U]], [[2 = 0U, 3 = 48U, 4 = 2U]], [[2 = 0U, 3 = 48U, 4 = 3U]], [[2 = 0U, 3 = 40U, 4 = 2U]], [[2 = 0U, 3 = 40U, 4 = 4U]], [[3 = 49U, 4 = 3U]]], 3 = false, 255 = 1U}')

-#
