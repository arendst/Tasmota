#######################################################################
# Partition manager for ESP32 - ESP32C3 - ESP32S2
#
# use : `import partition_core`
#
# To solidify:
#-
   import solidify load("partition_core.be") solidify.dump(partition_core)
-#
# Provides low-level objects and a Web UI
#######################################################################

#@ solidify:partition_core
var partition_core = module('partition_core')

#######################################################################
# Class for a partition table entry
#
#   typedef struct {
#       uint16_t magic;
#       uint8_t  type;
#       uint8_t  subtype;
#       uint32_t offset;
#       uint32_t size;
#       uint8_t  label[16];
#       uint32_t flags;
#   } esp_partition_info_t_simplified;
#
#######################################################################
class Partition_info
  var type
  var subtype
  var start
  var sz
  var label
  var flags

  #- remove trailing NULL chars from a bytes buffer before converting to string -#
  #- Berry strings can contain NULL, but this messes up C-Berry interface -#
  static def remove_trailing_zeroes(b)
    var sz = size(b)
    var i = 0
    while i < sz
      if b[-1-i] != 0   break end
      i += 1
    end
    if i > 0
      b.resize(size(b)-i)
    end
    return b
  end
    
  # Init the Parition information structure, either from a bytes() buffer or an empty if no buffer is provided
  def init(raw)
    self.type = 0
    self.subtype = 0
    self.start = 0
    self.sz = 0
    self.label = ''
    self.flags = 0

    if !issubclass(bytes, raw)    # no payload, empty partition information
      return
    end

    #- we have a payload, parse it -#
    var magic = raw.get(0,2)
    if magic == 0x50AA    #- partition entry -#

      self.type = raw.get(2,1)
      self.subtype = raw.get(3,1)
      self.start = raw.get(4,4)
      self.sz = raw.get(8,4)
      self.label = self.remove_trailing_zeroes(raw[12..27]).asstring()
      self.flags = raw.get(28,4)

    # elif magic == 0xEBEB  #- MD5 -#
    else
      raise  "internal_error", format("invalid magic number %02X", magic)
    end
    
  end

  # check if the parition is an OTA partition
  # if yes, return OTA number (starting at 0)
  # if no, return nil
  def is_ota()
    var sub_type = self.subtype
    if self.type == 0 && (sub_type >= 0x10 && sub_type < 0x20)
      return sub_type - 0x10
    end
  end

  # check if factory 'safeboot' partition
  def is_factory()
    return self.type == 0 && self.subtype == 0
  end

  # check if the parition is a SPIFFS partition
  # returns bool
  def is_spiffs()
    return self.type == 1 && self.subtype == 130
  end

  # get the actual image size give of the partition
  # returns -1 if the partition is not an app ota partition
  def get_image_size()
    import flash
    if self.is_ota() == nil && !self.is_factory()   return -1 end
    try
      var addr = self.start
      var sz = self.sz
      var magic_byte = flash.read(addr, 1).get(0, 1)
      if magic_byte != 0xE9 return -1 end
      
      var seg_count = flash.read(addr+1, 1).get(0, 1)
      # print("Segment count", seg_count)
      
      var seg_offset = addr + 0x20 # sizeof(esp_image_header_t) + sizeof(esp_image_segment_header_t) = 24 + 8

      var seg_num = 0
      while seg_num < seg_count
        # print(format("Reading 0x%08X", seg_offset))
        var segment_header = flash.read(seg_offset - 8, 8)
        var seg_start_addr = segment_header.get(0, 4)
        var seg_size = segment_header.get(4,4)
        # print(format("Segment %i: flash_offset=0x%08X start_addr=0x%08X sz=0x%08X", seg_num, seg_offset, seg_start_addr, seg_size))

        seg_offset += seg_size + 8    # add segment_length + sizeof(esp_image_segment_header_t)
        if seg_offset >= (addr + sz)   return -1 end

        seg_num += 1
      end
      var total_size = seg_offset - addr + 1 # add 1KB for safety

      # print(format("Total size = %i KB", total_size/1024))

      return total_size
    except .. as e, m
      tasmota.log("BRY: Exception> '" + e + "' - " + m, 2)
      return -1
    end
  end

  def type_to_string()
    if   self.type == 0 return "app"
    elif self.type == 1  return "data"
    end
    return format("0x%02X", self.type)
  end

  def subtype_to_string()
    if   self.type == 0
      if   self.subtype == 0      return "factory"
      elif self.subtype >= 0x10 && self.subtype < 0x20 return "ota_" + str(self.subtype - 0x10)
      elif self.subtype == 0x20  return "test"
      end
    elif self.type == 1
      if   self.subtype == 0x00  return "otadata"
      elif self.subtype == 0x01  return "phy"
      elif self.subtype == 0x02  return "nvs"
      elif self.subtype == 0x03  return "coredump"
      elif self.subtype == 0x04  return "nvskeys"
      elif self.subtype == 0x05  return "efuse_em"
      elif self.subtype == 0x80  return "esphttpd"
      elif self.subtype == 0x81  return "fat"
      elif self.subtype == 0x82  return "spiffs"
      end
    end
    return format("0x%02X", self.subtype)
  end

  # Human readable version of Partition information
  # this method is not included in the solidified version to save space,
  # it is included only in the optional application `tapp` version
  def tostring()
    var type_s = self.type_to_string()
    var subtype_s = self.subtype_to_string()

    # reformat strings
    if type_s != ""    type_s = " (" + type_s + ")" end
    if subtype_s != "" subtype_s = " (" + subtype_s + ")" end
    return format("<instance: Partition_info(%d%s,%d%s,0x%08X,0x%08X,'%s',0x%X)>",
                          self.type, type_s,
                          self.subtype, subtype_s,
                          self.start, self.sz,
                          self.label, self.flags)
  end

  def tobytes()
    #- convert to raw bytes -#
    var b = bytes('AA50')     #- set magic number -#
    b.resize(32).resize(2)    #- pre-reserve 32 bytes -#
    b.add(self.type, 1)
    b.add(self.subtype, 1)
    b.add(self.start, 4)
    b.add(self.sz, 4)
    var label = bytes().fromstring(self.label)
    label.resize(16)
    b = b + label
    b.add(self.flags, 4)
    return b
  end

end
partition_core.Partition_info = Partition_info

#-------------------------------------------------------------
 - OTA Data
 -
 - Selection of the active OTA partition
 -
    typedef struct {
        uint32_t ota_seq;
        uint8_t  seq_label[20];
        uint32_t ota_state;
        uint32_t crc; /* CRC32 of ota_seq field only */
    } esp_ota_select_entry_t;

  - Excerp from esp_ota_ops.c
      esp32_idf use two sector for store information about which partition is running
      it defined the two sector as ota data partition,two structure esp_ota_select_entry_t is saved in the two sector
      named data in first sector as otadata[0], second sector data as otadata[1]
      e.g.
      if otadata[0].ota_seq == otadata[1].ota_seq == 0xFFFFFFFF,means ota info partition is in init status
      so it will boot factory application(if there is),if there's no factory application,it will boot ota[0] application
      if otadata[0].ota_seq != 0 and otadata[1].ota_seq != 0,it will choose a max seq ,and get value of max_seq%max_ota_app_number
      and boot a subtype (mask 0x0F) value is (max_seq - 1)%max_ota_app_number,so if want switch to run ota[x],can use next formulas.
      for example, if otadata[0].ota_seq = 4, otadata[1].ota_seq = 5, and there are 8 ota application,
      current running is (5-1)%8 = 4,running ota[4],so if we want to switch to run ota[7],
      we should add otadata[0].ota_seq (is 4) to 4 ,(8-1)%8=7,then it will boot ota[7]
      if      A=(B - C)%D
      then    B=(A + C)%D + D*n ,n= (0,1,2...)
      so current ota app sub type id is x , dest bin subtype is y,total ota app count is n
      seq will add (x + n*1 + 1 - seq)%n
 -------------------------------------------------------------#
class Partition_otadata
  var maxota          # number of highest OTA partition, default 1 (double ota0/ota1)
  var has_factory     # is there a factory partition
  var offset          # offset of the otadata partition (0x2000 in length), default 0xE000
  var active_otadata  # which otadata block is active, 0 or 1, i.e. 0xE000 or 0xF000 -- or -1 if no OTA active, i.e. boot on factory
  var seq0            # ota_seq of first block
  var seq1            # ota_seq of second block

  #- crc32 for ota_seq as 32 bits unsigned, with init vector -1 -#
  static def crc32_ota_seq(seq)
    import crc
    return crc.crc32(0xFFFFFFFF, bytes().add(seq, 4))
  end

  #---------------------------------------------------------------------#
  # Rest of the class
  #---------------------------------------------------------------------#
  def init(maxota, has_factory, offset)
    self.maxota = maxota
    self.has_factory = has_factory
    if self.maxota == nil  self.maxota = 1 end
    self.offset = offset
    if self.offset == nil  self.offset = 0xE000 end
    self.active_otadata = -1
    self.load()
  end

  #- update ota_max, needs to recompute everything -#
  def set_ota_max(n)
    self.maxota = n
  end

  # change the active OTA partition
  def set_active(n)
    var seq_max = 0     #- current highest seq number -#
    var block_act = 0   #- block number containing the highest seq number -#

    if self.seq0 != nil
      seq_max = self.seq0
      block_act = 0
    end
    if self.seq1 != nil && self.seq1 > seq_max
      seq_max = self.seq1
      block_act = 1
    end
    
    #- compute the next sequence number -#
    var actual_ota = (seq_max - 1) % (self.maxota + 1)
    if actual_ota != n    #- change only if different -#
      if n > actual_ota   seq_max += n - actual_ota
      else                seq_max += (self.maxota + 1) - actual_ota + n
      end
      
      #- update internal structure -#
      if block_act == 1       #- current block is 1, so update block 0 -#
        self.seq0 = seq_max
      else                    #- or write to block 1 -#
        self.seq1 = seq_max
      end
      self._validate()
    end
  end

  #- load otadata from SPI Flash -#
  def load()
    import flash
    var otadata0 = flash.read(self.offset, 32)
    var otadata1 = flash.read(self.offset + 0x1000, 32)
    self.seq0 = otadata0.get(0, 4)   #- ota_seq for block 1 -#
    self.seq1 = otadata1.get(0, 4)   #- ota_seq for block 2 -#
    var valid0 = otadata0.get(28, 4) == self.crc32_ota_seq(self.seq0) #- is CRC32 valid? -#
    var valid1 = otadata1.get(28, 4) == self.crc32_ota_seq(self.seq1) #- is CRC32 valid? -#
    if !valid0   self.seq0 = nil end
    if !valid1   self.seq1 = nil end

    self._validate()
  end

  #- internally used, validate data -#
  def _validate()
    self.active_otadata = self.has_factory ? -1 : 0        # if no valid otadata, then use factory (-1) if any, or ota_0
    if self.seq0 != nil
      self.active_otadata = (self.seq0 - 1) % (self.maxota + 1)
    end
    if self.seq1 != nil && (self.seq0 == nil || self.seq1 > self.seq0)
      self.active_otadata = (self.seq1 - 1) % (self.maxota + 1)
    end
  end

  # Save partition information to SPI Flash
  def save()
    import flash
    #- check the block number to save, 0 or 1. Choose the highest ota_seq -#
    var block_to_save = -1    #- invalid -#
    var seq_to_save = -1      #- invalid value -#
    
    # check seq0
    if self.seq0 != nil
      seq_to_save = self.seq0
      block_to_save = 0
    end
    if (self.seq1 != nil) && (self.seq1 > seq_to_save)
      seq_to_save = self.seq1
      block_to_save = 1
    end
    # if none was good
    if block_to_save < 0    block_to_save = 0 end
    if seq_to_save < 0      seq_to_save = 1 end

    var offset_to_save = self.offset + 0x1000 * block_to_save   #- default 0xE000 or 0xF000 -#

    var bytes_to_save = bytes()
    bytes_to_save.add(seq_to_save, 4)
    bytes_to_save += bytes("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF")
    bytes_to_save.add(self.crc32_ota_seq(seq_to_save), 4)
    
    #- erase flash area and write -#
    flash.erase(offset_to_save, 0x1000)
    flash.write(offset_to_save, bytes_to_save)
  end

  # Produce a human-readable representation of the object with relevant information
  def tostring()
    return format("<instance: Partition_otadata(ota_active:%s, ota_seq=[%d,%d], ota_max=%d)>",
                          self.active_otadata >= 0 ? "ota_" + str(self.active_otadata) : "factory",
                          self.seq0, self.seq1, self.maxota)
  end
end
partition_core.Partition_otadata = Partition_otadata

#-------------------------------------------------------------
 - Class for a partition table entry
 -------------------------------------------------------------#
class Partition
  var raw   #- raw bytes of the partition table in flash -#
  var md5   #- md5 hash of partition list -#
  var slots
  var otadata   #- instance of Partition_otadata() -#

  def init()
    self.slots = []
    self.load()
    self.parse()
    self.load_otadata()
  end

  # Load partition information from SPI Flash
  def load()
    import flash
    self.raw = flash.read(0x8000,0x1000)
  end

  #- parse the raw bytes to a structured list of partition items -#
  def parse()
    for i:0..94       # there are maximum 95 slots + md5 (0xC00)
      var item_raw = self.raw[i*32..(i+1)*32-1]
      var magic = item_raw.get(0,2)
      if magic == 0x50AA    #- partition entry -#
        var slot = partition_core.Partition_info(item_raw)
        self.slots.push(slot)
      elif magic == 0xEBEB  #- MD5 -#
        self.md5 = self.raw[i*32+16..i*33-1]
        break
      else
        break
      end
    end
  end

  def get_ota_slot(n)
    for slot: self.slots
      if slot.is_ota() == n return slot end
    end
    return nil
  end

  def get_factory_slot()
    for slot: self.slots
      if slot.is_factory() return slot end
    end
  end

  def has_factory()
    return self.get_factory_slot() != nil
  end

  #- compute the highest ota<x> partition -#
  def ota_max()
    var ota_max = nil
    for slot:self.slots
      if slot.type == 0 && (slot.subtype >= 0x10 && slot.subtype < 0x20)
        var ota_num = slot.subtype - 0x10
        if (ota_max == nil) || (ota_num > ota_max)   ota_max = ota_num end
      end
    end
    return ota_max
  end

  # get the active OTA app partition number
  def get_active()
    return self.otadata.active_otadata
  end

  def load_otadata()
    #- look for otadata partition offset, and max_ota -#
    var otadata_offset = 0xE000   #- default value -#
    var ota_max = self.ota_max()
    for slot:self.slots
      if slot.type == 1 && slot.subtype == 0    #- otadata -#
        otadata_offset = slot.start
      end
    end

    self.otadata = partition_core.Partition_otadata(ota_max, self.has_factory(), otadata_offset)
  end

  #- change the active partition -#
  def set_active(n)
    if n < 0 || n > self.ota_max()  raise "value_error", "Invalid ota partition number" end
    self.otadata.set_ota_max(self.ota_max())    #- update ota_max if it changed -#
    self.otadata.set_active(n)
  end

  # Human readable version of Partition information
  # this method is not included in the solidified version to save space,
  # it is included only in the optional application `tapp` version
  #- convert to human readble -#
  def tostring()
    var ret = "<instance: Partition([\n"
    for slot: self.slots
      ret += "  "
      ret += slot.tostring()
      ret += "\n"
    end
    ret += "],\n  "
    ret += self.otadata.tostring()
    ret += "\n)>"
    return ret
  end

  #- convert the slots to raw bytes, ready to falsh to parition page -#
  def tobytes()
    if size(self.slots) > 95 raise "value_error", "Too many partiition slots" end
    var b = bytes()
    for slot: self.slots
      b += slot.tobytes()
    end
    #- compute MD5 -#
    var md5 = MD5()
    md5.update(b)
    #- add the last segment -#
    b += bytes("EBEBFFFFFFFFFFFFFFFFFFFFFFFFFFFF")
    b += md5.finish()
    #- complete -#
    return b
  end

  #- write back to flash -#
  def save()
    import flash
    var b = self.tobytes()
    #- erase flash area and write -#
    flash.erase(0x8000, 0x1000)
    flash.write(0x8000, b)
    self.otadata.save()
  end

  # Internal: returns which flash sector contains the partition definition
  # Returns 0 or 1, or `nil` if something went wrong
  # Note: partition flash sector vary from ESP32 to ESP32C3/S3
  static def get_flash_definition_sector()
    import flash
    for i:0..1
      var offset = i * 0x1000
      if flash.read(offset, 1) == bytes('E9')   return offset end
    end
  end  

  # Internal: returns the maximum flash size possible
  # Returns max flash size ok kB
  def get_max_flash_size_k()
    var flash_size_k = tasmota.memory()['flash']
    var flash_size_real_k = tasmota.memory().find("flash_real", flash_size_k)
    if (flash_size_k != flash_size_real_k) && self.get_flash_definition_sector() != nil
      flash_size_k = flash_size_real_k    # try to expand the flash size definition
    end
    return flash_size_k
  end

  # Internal: returns the unallocated flash size (in kB) beyond the file-system
  # this indicates that the file-system can be extended (although erased at the same time)
  def get_unallocated_k()
    var last_slot = self.slots[-1]
    if last_slot.is_spiffs()
      # verify that last slot is filesystem
      var flash_size_k = self.get_max_flash_size_k()
      var partition_end_k = (last_slot.start + last_slot.sz) / 1024   # last kb used for fs
      if  partition_end_k < flash_size_k
        return flash_size_k - partition_end_k
      end
    end
    return 0
  end

  #- ---------------------------------------------------------------------- -#
  #- Resize flash definition if needed
  #- ---------------------------------------------------------------------- -#
  def resize_max_flash_size_k()
    var flash_size_k = tasmota.memory()['flash']
    var flash_size_real_k = tasmota.memory().find("flash_real", flash_size_k)
    var flash_definition_sector = self.get_flash_definition_sector()
    if (flash_size_k != flash_size_real_k) && flash_definition_sector != nil
      import flash

      flash_size_k = flash_size_real_k    # try to expand the flash size definition

      var flash_def = flash.read(flash_definition_sector, 4)
      var size_before = flash_def[3]

      var flash_size_code
      var flash_size_real_m = flash_size_real_k / 1024    # size in MB
      if   flash_size_real_m == 1      flash_size_code = 0x00
      elif flash_size_real_m == 2      flash_size_code = 0x10
      elif flash_size_real_m == 4      flash_size_code = 0x20
      elif flash_size_real_m == 8      flash_size_code = 0x30
      elif flash_size_real_m == 16     flash_size_code = 0x40
      elif flash_size_real_m == 32     flash_size_code = 0x50
      elif flash_size_real_m == 64     flash_size_code = 0x60
      elif flash_size_real_m == 128    flash_size_code = 0x70
      end

      if flash_size_code != nil
        # apply the update
        var old_def = flash_def[3]
        flash_def[3] = (flash_def[3] & 0x0F) | flash_size_code
        flash.write(flash_definition_sector, flash_def)
        tasmota.log(format("UPL: changing flash definition from 0x02X to 0x%02X", old_def, flash_def[3]), 3)
      else
        raise "internal_error", "wrong flash size "+str(flash_size_real_m)
      end
    end
  end

  # Called at first boot
  # Try to expand FS to max of flash size
  def resize_fs_to_max()
    try
      var unallocated = self.get_unallocated_k()
      if unallocated <= 0     return nil end

      tasmota.log(format("BRY: Trying to expand FS by %i kB", unallocated), 2)

      self.resize_max_flash_size_k()   # resize if needed
      # since unallocated succeeded, we know the last slot is FS
      var fs_slot = self.slots[-1]
      fs_slot.sz += unallocated * 1024
      self.save()

      # restart
      tasmota.global.restart_flag = 2
      tasmota.log("BRY: Successfully resized FS, restarting", 2)
    except .. as e, m
      tasmota.log(format("BRY: Exception> '%s' - %s", e, m), 2)
    end
  end

  #- invalidate SPIFFS partition to force format at next boot -#
  #- we simply erase the first byte of the first 2 blocks in the SPIFFS partition -#
  def invalidate_spiffs()
    import flash
    #- we expect the SPIFFS partition to be the last one -#
    var spiffs = self.slots[-1]
    if !spiffs.is_spiffs() raise 'value_error', 'No FS partition found' end

    var b = bytes("00")  #- flash memory: we can turn bits from '1' to '0' -#
    flash.write(spiffs.start         , b)    #- block #0 -#
    flash.write(spiffs.start + 0x1000, b)    #- block #1 -#
  end
  
  # switch to safeboot `factory` partition
  def switch_factory(force_ota)
    import flash
    flash.factory(force_ota)
  end
end
partition_core.Partition = Partition

# init method to force the global `partition_core` is defined even if the import is done within a function
def init(m)
  import global
  global.partition_core = m
  return m
end
partition_core.init = init

return partition_core

#- Example

import partition_core

# read
p = partition_core.Partition()
print(p)

-#
