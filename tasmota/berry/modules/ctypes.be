#-------------------------------------------------------------
#- Ctypes for Berry
#-
#- Inspired from Python's ctypes structure
#-------------------------------------------------------------#
import string
 
ctypes = module('ctypes')

#-------------------------------------------------------------
#- Basic types for mapping
#-
#- ints of size 1/2/4 bytes (little endian by default)
#- usigned ints of size 1/2/4 bytes (little endian by default)
#-
#- (Big Endian are negative numbers)
#-------------------------------------------------------------#

# default is little_endian
ctypes.i32    = 14
ctypes.i16    = 12
ctypes.i8     = 11
ctypes.u32    =  4
ctypes.u16    =  2
ctypes.u8     =  1

# explicit little endian
ctypes.le_i32 = 14
ctypes.le_i16 = 12
ctypes.le_i8  = 11
ctypes.le_u32 =  4
ctypes.le_u16 =  2
ctypes.le_u8  =  1

# big endian
ctypes.be_i32 = -14
ctypes.be_i16 = -12
ctypes.be_i8  = -11
ctypes.be_u32 =  -4
ctypes.be_u16 =  -2
ctypes.be_u8  =  -1

# bitfields (always unsigned)
ctypes.bf_0   = 100 # serves as base
ctypes.bf_1   = 101
ctypes.bf_2   = 102
ctypes.bf_3   = 103
ctypes.bf_4   = 104
ctypes.bf_5   = 105
ctypes.bf_6   = 106
ctypes.bf_7   = 107
ctypes.bf_8   = 108
ctypes.bf_9   = 109
ctypes.bf_10  = 110
ctypes.bf_11  = 111
ctypes.bf_12  = 112
ctypes.bf_13  = 113
ctypes.bf_14  = 114
ctypes.bf_15  = 115


#-------------------------------------------------------------
#- 'get_bits' function
#-
#- Reads a bit-field in a `bytes()` object
#-
#- Input:
#-   b: bytes() object to read from
#-   offset_bytes (int): byte offset in the bytes() object
#-   offset_bits  (int): bit number to start reading from (0 = LSB)
#-   len_bits     (int): how many bits to read
#- Output:
#-   valuer (int)
#-------------------------------------------------------------#
ctypes.get_bits = def (b, offset_bytes, offset_bits, len_bits)
  if !isinstance(b, bytes) raise "value_error", "first argument must be of type 'bytes'" end
  if offset_bits < 0 || offset_bits > 7 raise "value_error", "offset_bits must be between 0 and 7" end
  if len_bits <= 0 || len_bits > 32 raise "value_error", "length in bits must be between 0 and 32" end
  var ret = 0

  var bit_shift = 0                   # bit number to write to

  while (len_bits > 0)
    var block_bits = 8 - offset_bits    # how many bits to read in the current block (block = byte)
    if block_bits > len_bits  block_bits = len_bits end

    var mask = ( (1<<block_bits) - 1) << offset_bits
    # print(string.format("mask = %02X", mask))
    ret = ret | ( ((b[offset_bytes] & mask) >> offset_bits) << bit_shift)

    # move the input window
    bit_shift += block_bits
    len_bits -= block_bits
    offset_bits = 0                   # start at full next byte
    offset_bytes += 1
  end

  return ret
end

ctypes.sort = def (l)
  # insertion sort
  for i:1..size(l)-1
    var k = l[i]
    var j = i
    while (j > 0) && (l[j-1] > k)
      l[j] = l[j-1]
      j -= 1
    end
    l[j] = k
  end
  return l
end

#-------------------------------------------------------------
#- 'set_bits' function
#-
#- Writes a bit-field in a `bytes()` object
#-
#- Input:
#-   b: bytes() object to write to
#-   offset_bytes (int): byte offset in the bytes() object
#-   offset_bits  (int): bit number to start writing to (0 = LSB)
#-   len_bits     (int): how many bits to write
#- Output:
#-   bytes() object modified (by reference)
#-------------------------------------------------------------#
ctypes.set_bits = def (b, offset_bytes, offset_bits, len_bits, val)
  if !isinstance(b, bytes) raise "value_error", "first argument must be of type 'bytes'" end
  if offset_bits < 0 || offset_bits > 7 raise "value_error", "offset_bits must be between 0 and 7" end
  if len_bits <= 0 || len_bits > 32 raise "value_error", "length in bits must be between 0 and 32" end

  while (len_bits > 0)
    var block_bits = 8 - offset_bits    # how many bits to write in the current block (block = byte)
    if block_bits > len_bits  block_bits = len_bits end

    var mask_val = (1<<block_bits) - 1  # mask to the n bits to get for this block
    var mask_b_inv = 0xFF - (mask_val << offset_bits)
    b[offset_bytes] = (b[offset_bytes] & mask_b_inv) | ((val & mask_val) << offset_bits)

    # move the input window
    val >>= block_bits
    len_bits -= block_bits
    offset_bits = 0                   # start at full next byte
    offset_bytes += 1
  end
  return b
end


#- print the C types -#1
ctypes.print_types = def ()
  print("enum {")
  print("  ctypes_ptr = 0,")
  print("  ctypes_uint = 1,")
  print("  ctypes_int = 2,")
  print("  ctypes_str = 3,")
  print("};")
  print()
  print("typedef struct be_ctypes_structure_t {")
  print("  const char * name;")
  print("  uint16_t  offset_bits;")
  print("  uint16_t  len_bits : 13;")
  print("  uint16_t  type : 3;")
  print("} be_ctypes_structure_t;")
end


#-------------------------------------------------------------
#- 'ctypes.structure' class
#-
#- Parses a ctypes structure descriptor and creates
#- a set of getters and setters
#-
#-------------------------------------------------------------#
class structure
  var cur_offset      # offset in bytes from buffer start
  var bit_offset      # are we intra-byte?
  var get_closures    #
  var set_closures    #
  var size_bytes      # size in bytes
  var mapping         # map to generate C binding

  # init world
  def init(mapping, name)
    self.cur_offset = 0
    self.bit_offset = 0
    self.size_bytes = 0           # overall size in bytes
    self.get_closures = {}
    self.set_closures = {}
    self.mapping = {}

    # parse mapping
    self.parse_mapping(mapping, name)
  end
  #- iteratively parse mapping
  #-
  #- if name is not nil, it also outputs a C structure for the mapping
  #-
  #-------------------------------------------------------------#
  def parse_mapping(mapping, name)
    for map_line: mapping
      self.parse_line(map_line)
    end

    if name != nil
      print(string.format("const be_ctypes_structure_t be_%s[%d] = {", name, size(self.mapping)))
      # list keys for future binary search
      var names = []
      for n:self.mapping.keys() names.push(n) end   # convert to list
      ctypes.sort(names)
      for n:names
        var args = self.mapping[n]
        print(string.format("  { \"%s\", %i, %i, %s },", n, args[0], args[1], args[2]))
      end
      print("};")
      print()
    end

    # clear any outstanding bitfield
    self.align(1)
    self.size_bytes = self.cur_offset
  end

  def size()
    return self.size_bytes
  end

  # parse a single line
  def parse_line(map_line)
    var type_obj = map_line[0]
    var name = map_line[1]
    var bits = 0
    if size(map_line) >= 3 bits = map_line[2] end

    if isinstance(type_obj, ctypes.structure)
      # nested structure
      self.nested(name, type_obj)
    elif type(type_obj) == 'int'
      # TODO check actual type
      if type_obj > ctypes.bf_0
        # bit field
        self.get_bitfield_closure(name, type_obj - ctypes.bf_0)
      else
        # multi-bytes
        self.get_int_closure(name, type_obj)
      end
    end
  end

  #- ensure alignment to 1/2/4 bytes -#
  def align(n)
    if n != 1 && n != 2 && n != 4 raise "value_error", "acceptable values are 1/2/4" end

    #- align to byte boundary if we're in a bitfield -#
    if self.bit_offset != 0
      #- we are not byte aligned, let's re-aling -#
      self.cur_offset += 1
      self.bit_offset = 0

      #- check 2/4 bytes alignment -#
      if self.cur_offset % n != 0
        # we are not aligned with current size
        self.cur_offset += n - self.cur_offset % n
      end
    end
  end

  def nested(name, type_obj)
    var sub_size = type_obj.size()
    if sub_size <= 0 raise "value_error", "empty sub-structure not supported" end

    # align to appropriate sub-size
    var align_size = sub_size
    if align_size == 3 || align_size > 4  align_size = 4 end
    self.align(align_size)

    var offset = self.cur_offset    # var for closure

    # inlcude nested
    for subname:type_obj.mapping.keys()
      var val = type_obj.mapping[subname]
      self.mapping[name+"_"+subname] = [val[0] + (offset << 3), val[1], val[2]]
    end
    # self.mapping[name] = [offset << 3, sub_size << 3]

    self.get_closures[name] = def (b, p) return ctypes.nested_buffer(type_obj, offset + p, b) end
    # self.set_closures[name] = def (b, p, v) return ctypes.nested_buffer(type_obj, offset + p, b) end

    self.cur_offset += sub_size
  end

  def get_int_closure(name, size_in_bytes_le_be)  # can be 1/2/4
    #- abs size -#
    var size_in_bytes = size_in_bytes_le_be < 0 ? - size_in_bytes_le_be : size_in_bytes_le_be
    var signed = size_in_bytes > 10
    size_in_bytes_le_be = size_in_bytes_le_be % 10  # remove sign marker
    size_in_bytes = size_in_bytes % 10  # remove sign marker

    self.align(size_in_bytes)       # force alignment
    var offset = self.cur_offset    # prepare variable for capture in closure
    
    self.mapping[name] = [offset << 3, size_in_bytes_le_be << 3, signed ? "ctypes_int" : "ctypes_uint"]

    #- add closures -#
    if signed
      self.get_closures[name] = def (b, p) return b.geti(offset + p, size_in_bytes_le_be) end
    else
      self.get_closures[name] = def (b, p) return b.get(offset + p, size_in_bytes_le_be) end
    end
    self.set_closures[name] = def (b, p, v) return b.set(offset+ p, v, size_in_bytes_le_be) end
    
    self.cur_offset += size_in_bytes    # next offset
  end


  def get_bitfield_closure(name, size_in_bits)  # can be 1..32
    var cur_offset = self.cur_offset    # prepare variable for capture in closure
    var bit_offset = self.bit_offset
    self.mapping[name] = [bit_offset + (cur_offset << 3), size_in_bits, "ctypes_uint"]
    self.get_closures[name] = def (b, p) return ctypes.get_bits(b, cur_offset + p, bit_offset, size_in_bits) end
    self.set_closures[name] = def (b, p, v) return ctypes.set_bits(b, cur_offset+ p, bit_offset, size_in_bits, v) end

    self.cur_offset += size_in_bits / 8
    self.cur_offset += (self.bit_offset + size_in_bits) / 8
    self.bit_offset = (self.bit_offset + size_in_bits) % 8
  end
end

ctypes.structure = structure

#-------------------------------------------------------------
#- Structured buffer
#-
#- Inspired from Python's ctypes structure
#-
#- This class is a wrapper around `bytes()` object (actually a subclass)
#- providing virtual members mapped to the ctypes structure.
#-
#- Takes as argument a ctypes.structure and an optional bytes() object
#-------------------------------------------------------------#
class buffer : bytes
  var _cs                   # ctypes_structure associated

  def init(cs, b)
    if !isinstance(cs, ctypes.structure) raise "value_error", "first argument must be an instance of 'ctypes.structure'" end
    self._cs = cs
    var size = self._cs.size()    # size in bytes of the structure

    super(self, bytes).init(size) # init bytes object with reserved size in memory

    if isinstance(b, bytes)
        self..b
    end
    self.resize(size)    # size once for all to the target size and complete with 00s
  end

  # accessor for virtual member
  def member(name)
    return self._cs.get_closures[name](self, 0)
  end

  # setter for virtual member
  def setmember(name, value)
    self._cs.set_closures[name](self, 0, value)
  end
end
ctypes.buffer = buffer

#-------------------------------------------------------------
#- Nested buffer
#-
#- Nested structures are just pointers to the master bytes() object
#-------------------------------------------------------------#
class nested_buffer
  var _cs             # ctypes.structure instance for this buffer
  var _offset         # offset un bytes() to the structure
  var _bytes          # reference to the overall buffer (by reference)
  
  def init(cs, offset, b)
    if !isinstance(cs, ctypes.structure) raise "value_error", "first argument must be an instance of 'ctypes.structure'" end
    if type(offset) != 'int' raise "value_error", "second argument must be of type 'int'" end
    if !isinstance(b, bytes) raise "value_error", "third argument must be an instance of 'bytes'" end
    self._cs = cs
    self._offset = offset
    self._bytes = b
  end

  # accessor for virtual member
  def member(name)
    return self._cs.get_closures[name](self._bytes, self._offset)
  end

  # setter for virtual member
  def setmember(name, value)
    self._cs.set_closures[name](self._bytes, self._offset, value)
  end

  def tostring()
    return self._bytes[self._offset..self._offset+self._cs.size()-1].tostring()
  end
end
ctypes.nested_buffer = nested_buffer


return ctypes

# ex = [
#   [ctypes.u32, "a"],
#   [ctypes.u16, "b"],
#   [ctypes.i8,  "c"],
#   [ctypes.i32, "d"],   # should infer an empty byte
# ]

# cs = ctypes.structure(ex)
# bb = ctypes.buffer(cs, bytes("aabb"))

# fa = cp.get_int_closure('a', 4)
# fb = cp.get_int_closure('b', 2)
# fc = cp.get_int_closure('c', 1)
# fd = cp.get_int_closure('d', 4)

# b = bytes("04030201AA55FFFF00010001")

# cp.get_closures['a'](b)
# cp.get_closures['b'](b)
# cp.get_closures['c'](b)
# cp.get_closures['d'](b)

# bb = ctypes_buffer(cp, b)

# bb.a = 0x11223344
# bb
# bb.a


# > bb = ctypes_buffer(cp, b)
# > bb.a
# 16909060
# > bb.b
# 21930
# > bb.c
# 255
# > bb.d
# 16777472
# > bb.e
# key_error: e
# stack traceback:
# 	<native>: in native function
# 	stdin:11: in function `member`
# 	stdin:1: in function `main`


# > bb['a']
# 16909060
# > bb['b']
# 21930
# > bb['c']
# 255
# > bb['d']
# 16777472

#-
> fa(b)
16909060
> fb(b)
21930
> fc(b)
255
> fd(b)
16777472

-#

# def get_bits(b, offset_bytes, offset_bits, len_bits)
#   if !isinstance(b, bytes) raise "value_error", "first argument must be of type 'bytes'" end
#   if offset_bits < 0 || offset_bits > 7 raise "value_error", "offset_bits must be between 0 and 7" end
#   if len_bits <= 0 || len_bits > 32 raise "value_error", "length in bits must be between 0 and 32" end
#   var ret = 0

#   var bit_shift = 0                   # bit number to wrtie to

#   while (len_bits > 0)
#     var block_bits = 8 - offset_bits    # bit number to read in current block (block = byte)
#     if block_bits > len_bits  block_bits = len_bits end

#     var mask = ( (1<<block_bits) - 1) << offset_bits
#     # print(string.format("mask = %02X", mask))
#     ret = ret | ( ((b[offset_bytes] & mask) >> offset_bits) << bit_shift)

#     # move the input window
#     bit_shift += block_bits
#     len_bits -= block_bits
#     offset_bits = 0                   # start at full next byte
#     offset_bytes += 1
#   end

#   return ret
# end

# Test

# b=bytes("AAFF10")
# assert(get_bits(b, 0, 0, 1) == 0)
# assert(get_bits(b, 0, 1, 1) == 1)
# assert(get_bits(b, 0, 2, 1) == 0)
# assert(get_bits(b, 0, 3, 1) == 1)

# assert(get_bits(b, 0, 0, 2) == 2)
# assert(get_bits(b, 0, 1, 2) == 1)
# assert(get_bits(b, 0, 2, 2) == 2)

# assert(get_bits(b, 0, 0, 8) == 0xAA)
# assert(get_bits(b, 0, 0, 10) == 0x3AA)
# assert(get_bits(b, 0, 2, 8) == 0xEA)

# assert(get_bits(b, 1, 0, 8) == 0xFF)
# assert(get_bits(b, 1, 0, 10) == 0x0FF)

# assert(get_bits(b, 1, 0, 16) == 0x10FF)


# assert(get_bits(b, 1, 7, 4) == 0x01)
# assert(get_bits(b, 1, 7, 6) == 0x21)

# def set_bits(b, offset_bytes, offset_bits, len_bits, val)
#   if !isinstance(b, bytes) raise "value_error", "first argument must be of type 'bytes'" end
#   if offset_bits < 0 || offset_bits > 7 raise "value_error", "offset_bits must be between 0 and 7" end
#   if len_bits <= 0 || len_bits > 32 raise "value_error", "length in bits must be between 0 and 32" end

#   while (len_bits > 0)
#     var block_bits = 8 - offset_bits    # how many bits to write in the current block (block = byte)
#     if block_bits > len_bits  block_bits = len_bits end

#     var mask_val = (1<<block_bits) - 1  # mask to the n bits to get for this block
#     var mask_b_inv = 0xFF - (mask_val << offset_bits)
#     b[offset_bytes] = (b[offset_bytes] & mask_b_inv) | ((val & mask_val) << offset_bits)

#     # move the input window
#     val >>= block_bits
#     len_bits -= block_bits
#     offset_bits = 0                   # start at full next byte
#     offset_bytes += 1
#   end
#   return b
# end

# b=bytes("00000000")

# assert(set_bits(b,0,0,1,1) == bytes('01000000'))
# assert(set_bits(b,1,0,1,1) == bytes('01010000'))
# assert(set_bits(b,0,0,1,2) == bytes('00010000'))
# assert(set_bits(b,0,4,1,1) == bytes('10010000'))
# assert(set_bits(b,0,4,1,0) == bytes('00010000'))

# b=bytes("FF000000")
# assert(set_bits(b,0,4,1,0) == bytes('EF000000'))
# assert(set_bits(b,0,4,1,1) == bytes('FF000000'))

# b=bytes("00000000")
# assert(set_bits(b,2,6,1,-1) == bytes('00004000'))
# b=bytes("00000000")
# assert(set_bits(b,2,1,6,-1) == bytes('00007E00'))