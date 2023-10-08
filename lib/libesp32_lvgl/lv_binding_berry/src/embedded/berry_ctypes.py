#-------------------------------------------------------------
#- Ctypes for Berry
#-
#- Inspired from Python's ctypes structure
#-------------------------------------------------------------#

#-------------------------------------------------------------
#- Basic types for mapping
#-
#- ints of size 1/2/4 bytes (little endian by default)
#- usigned ints of size 1/2/4 bytes (little endian by default)
#-
#- (Big Endian are negative numbers)
#-------------------------------------------------------------#

types = {
  # default is little_endian
  "i32"    : 14,
  "i16"    : 12,
  "i8"     : 11,
  "u32"    :  4,
  "u16"    :  2,
  "u8"     :  1,
  
  # explicit little endian
  "le_i32" : 14,
  "le_i16" : 12,
  "le_i8"  : 11,
  "le_u32" :  4,
  "le_u16" :  2,
  "le_u8"  :  1,
  
  # big endian
  "be_i32" : -14,
  "be_i16" : -12,
  "be_i8"  : -11,
  "be_u32" :  -4,
  "be_u16" :  -2,
  "be_u8"  :  -1,
  
  # floating point
  "float32"  : 5,
  "double64" : 10,
  
  # pointer
  "ptr32"  :  9,
  "ptr64"  : -9,
  
  # bitfields (always unsigned)
  "bf_x"   : 0, # generic bitfield
  "bf_0"   : 100, # serves as base
  "bf_1"   : 101,
  "bf_2"   : 102,
  "bf_3"   : 103,
  "bf_4"   : 104,
  "bf_5"   : 105,
  "bf_6"   : 106,
  "bf_7"   : 107,
  "bf_8"   : 108,
  "bf_9"   : 109,
  "bf_10"  : 110,
  "bf_11"  : 111,
  "bf_12"  : 112,
  "bf_13"  : 113,
  "bf_14"  : 114,
  "bf_15"  : 115,
  "bf_16"  : 116,
}

# default is little_endian
i32    = 14
i16    = 12
i8     = 11
u32    =  4
u16    =  2
u8     =  1

# explicit little endian
le_i32 = 14
le_i16 = 12
le_i8  = 11
le_u32 =  4
le_u16 =  2
le_u8  =  1

# big endian
be_i32 = -14
be_i16 = -12
be_i8  = -11
be_u32 =  -4
be_u16 =  -2
be_u8  =  -1

# floating point
float32  = 5
double64 = 10

# pointer
ptr32  =  9
ptr64  = -9

# bitfields (always unsigned)
bf_x   = 0 # generic bitfield
bf_0   = 100 # serves as base
bf_1   = 101
bf_2   = 102
bf_3   = 103
bf_4   = 104
bf_5   = 105
bf_6   = 106
bf_7   = 107
bf_8   = 108
bf_9   = 109
bf_10  = 110
bf_11  = 111
bf_12  = 112
bf_13  = 113
bf_14  = 114
bf_15  = 115
bf_16  = 116

type_mapping = {
  14: "ctypes_i32",
  12: "ctypes_i16",
  11: "ctypes_i8",
   4: "ctypes_u32",
   2: "ctypes_u16",
   1: "ctypes_u8",

  -14:"ctypes_be_i32",  # big endian
  -12:"ctypes_be_i16",
  -11:"ctypes_be_i8",
  -4: "ctypes_be_u32",
  -2: "ctypes_be_u16",
  -1: "ctypes_be_u8",

   5: "ctypes_float",
  10: "ctypes_double",
   9: "ctypes_ptr32",
  -9: "ctypes_ptr64",

   0: "ctypes_bf"       # bitfield
}

def type_to_str(type_num):
  return type_mapping.get(type_num, str(type_num))

#- print the C types -#1
def print_types():
  print("/********************************************************************")
  print(" * Generated code, don't edit")
  print(" *******************************************************************/")
  print()
  print("static const char * be_ctypes_instance_mappings[];    /* forward definition */")
  print()

global_classes = []   # track the list of all classes and
global_mappings = []  # mapping to Berry classes, ex: lv_color

def print_classes(module_name):
  # print mappings
  global global_classes
  if len(global_mappings) > 7:
    raise Exception("too many mappings, 7 max")

  print("static const char * be_ctypes_instance_mappings[] = {")
  for n in global_mappings:
    print(f"  \"{n}\",")

  print("  NULL")
  print("};")
  print()

  global_classes = sorted(global_classes)

  for elt in global_classes:
    print(f"static be_define_ctypes_class({elt}, &be_{elt}, &be_class_ctypes_bytes, \"{elt}\");")

  print()
  print(f"void be_load_ctypes_{module_name}_definitions_lib(bvm *vm) {{")
  for elt in global_classes:
    print(f"  ctypes_register_class(vm, &be_class_{elt});")

  print("}")
  print()
  print("be_ctypes_class_by_name_t be_ctypes_lvgl_classes[] = {")
  for elt in global_classes:
    print(f"  {{ \"{elt}\", &be_class_{elt} }},")

  print("};")
  print("const size_t be_ctypes_lvgl_classes_size = sizeof(be_ctypes_lvgl_classes)/sizeof(be_ctypes_lvgl_classes[0]);")
  print()

  print("/********************************************************************/")


#-------------------------------------------------------------
#- 'berry_ctypes.structure' class
#-
#- Parses a ctypes structure descriptor and creates
#- a set of getters and setters
#-
#-------------------------------------------------------------#
class structure:
  # var cur_offset      # offset in bytes from buffer start
  # var bit_offset      # are we intra-byte?
  # var size_bytes      # size in bytes
  # var mapping         # map to generate C binding

  # init world
  def __init__(self, mapping, name):
    self.cur_offset = 0
    self.bit_offset = 0
    self.size_bytes = 0           # overall size in bytes
    self.mapping = {}

    # parse mapping
    self.parse_mapping(mapping, name)

  #- iteratively parse mapping
  #-
  #- if name is not nil, it also outputs a C structure for the mapping
  #-
  #-------------------------------------------------------------#
  def parse_mapping(self, mapping, name):
    for map_line in mapping:
      self.parse_line(map_line)

    # clear any outstanding bitfield
    self.align(1)
    self.size_bytes = self.cur_offset

    if name != None:
      size_aligned = self.size_bytes
      # as a final structure, we align to 2/4 bytes boundaries
      if size_aligned >= 3:
        size_aligned = int((size_aligned + 3)/4)*4

      print(f"const be_ctypes_structure_t be_{name} = {{")
      print(f"  {size_aligned},  /* size in bytes */")
      print(f"  {len(self.mapping)},  /* number of elements */")
      print(f"  be_ctypes_instance_mappings,")
      print(f"  (const be_ctypes_structure_item_t[{len(self.mapping)}]) {{")
      # list keys for future binary search
      names = []
      for n in self.mapping.keys(): names.append(n)    # convert to list
      names = sorted(names)
      for n in names:
        args = self.mapping[n]
        print(f"    {{ \"{n}\", {args[0]}, {args[1]}, {args[2]}, {type_to_str(args[3])}, {args[4]} }},")

      print("}};")
      print()

      # retain class definition
      global_classes.append(name)

  def size(self):
    return self.size_bytes

  # parse a single line
  def parse_line(self, map_line):
    # print(f"parse_line: {map_line}")
    line_type = map_line[0]
    line_name = map_line[1]

    if isinstance(line_type, structure):    # TODO
      # nested structure
      self.nested(line_name, line_type)

    # special case where the type is a list
    # which means a specific mapping to a class
    # Ex: `[ct.u16, "lv_color"]`
    mapping_idx = 0   # mapping starts at 1
    if isinstance(line_type, list):    # TODO
      # it may be a list to denote a mapping to an instance
      mapping_name = line_type[1]
      if not mapping_name in global_mappings: # TODO
        global_mappings.append(mapping_name)

      mapping_idx = global_mappings.index(mapping_name) + 1
      line_type = line_type[0]  # take the simple value of first element in the list

    # regular type as int
    if isinstance(line_type, int):   # TODO
      # simple attibute
      # TODO check actual type
      if line_type > bf_0:
        # bit field
        self.parse_bitfield(line_name, line_type - bf_0, mapping_idx)
      elif (line_type == ptr32) or (line_type == ptr64):
        # pointer
        self.parse_ptr(line_name, line_type, mapping_idx)
      elif (line_type == float32) or (line_type == double64):
        # multi-bytes
        self.parse_float(line_name, line_type, mapping_idx)
      else:
        # multi-bytes
        self.parse_int(line_name, line_type, mapping_idx)

  #- ensure alignment to 1/2/4 bytes -#
  def align(self, n):
    if n != 1 and n != 2 and n != 4:
      raise Exception("acceptable values are 1/2/4")

    #- align to byte boundary if we're in a bitfield -#
    if self.bit_offset != 0:
      #- we are not byte aligned, let's re-aling -#
      self.cur_offset += 1
      self.bit_offset = 0

    #- check 2/4 bytes alignment -#
    if self.cur_offset % n != 0:
      # we are not aligned with current size
      self.cur_offset += n - self.cur_offset % n

  def nested(self, name, type_obj):
    sub_size = type_obj.size()
    if sub_size <= 0:
      raise Exception("empty sub-structure not supported")

    # align to appropriate sub-size
    align_size = sub_size
    if align_size == 3 or align_size > 4:  align_size = 4
    self.align(align_size)

    offset = self.cur_offset    # var for closure

    # include nested
    for subname in type_obj.mapping.keys():
      val = type_obj.mapping[subname]
      self.mapping[name+"_"+subname] = [val[0] + offset, val[1], val[2], val[3], val[4]]
    # self.mapping[name] = [offset << 3, sub_size << 3]

    self.cur_offset += sub_size

  def parse_int(self, name, type, instance_mapping):  # can be 1/2/4
    # print(f"parse_int name={name} type={type} map={instance_mapping}")
    #- abs size -#
    size_in_bytes = type
    if size_in_bytes < 0: size_in_bytes = -type

    signed = size_in_bytes > 10
    size_in_bytes_le_be = type % 10  # remove sign marker
    size_in_bytes = size_in_bytes % 10  # remove sign marker

    # print(f"size_in_bytes={size_in_bytes} signed={signed} size_in_bytes_le_be={size_in_bytes_le_be}")

    self.align(size_in_bytes)       # force alignment
    offset = self.cur_offset    # prepare variable for capture in closure
    
    self.mapping[name] = [offset, 0, 0, type, instance_mapping]

    self.cur_offset += size_in_bytes    # next offset

  def parse_ptr(self, name, type, instance_mapping):  # can be 1/2/4
    #- actual size -#
    size_in_bytes = 4
    if type == ptr64: size_in_bytes = 8

    self.align(size_in_bytes)       # force alignment
    offset = self.cur_offset    # prepare variable for capture in closure
    
    self.mapping[name] = [offset, 0, 0, type, instance_mapping]
    
    self.cur_offset += size_in_bytes    # next offset

  def parse_float(self, name, type, instance_mapping):  # can be 1/2/4
    #- actual size -#
    size_in_bytes = 4
    if type == double64: size_in_bytes = 8

    self.align(size_in_bytes)       # force alignment
    offset = self.cur_offset    # prepare variable for capture in closure
    
    self.mapping[name] = [offset, 0, 0, type, instance_mapping]

    self.cur_offset += size_in_bytes    # next offset

  def parse_bitfield(self, name, size_in_bits, instance_mapping):  # can be 1..32
    cur_offset = self.cur_offset    # prepare variable for capture in closure
    bit_offset = self.bit_offset
    self.mapping[name] = [cur_offset, bit_offset, size_in_bits, 0, instance_mapping]

    self.cur_offset += int((self.bit_offset + size_in_bits) / 8)
    self.bit_offset = (self.bit_offset + size_in_bits) % 8
