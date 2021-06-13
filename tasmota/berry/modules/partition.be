#-------------------------------------------------------------
 - Parser for ESP32 partition
 -
 -------------------------------------------------------------#
partition = module('partition')

import flash
import string

#- remove trailing NULL chars from a buffer before converting to string -#
#- Berry strings can contain NULL, but this messes up C-Berry interface -#
def remove_trailing_zeroes(b)
  while size(b) > 0
    if b.get(size(b)-1,1) == 0
      b.resize(size(b)-1)
    else
      break
    end
  end
  return b
end


#-------------------------------------------------------------
 - Simple CRC32 imple
 -
 - adapted from Python https://rosettacode.org/wiki/CRC-32#Python
 -------------------------------------------------------------#
def crc32_create_table()
  var a = []
  for i:0..255
    var k = i
    for j:0..7
      if k & 1
        k = (k >> 1) & 0x7FFFFFFF
        k ^= 0xedb88320
      else
        k = (k >> 1) & 0x7FFFFFFF
      end
    end
    a.push(k)
  end
  return a
end

crc32_table = crc32_create_table()

def crc32_update(buf, crc)
  crc ^= 0xffffffff
  for k:0..size(buf)-1
    crc = (crc >> 8 & 0x00FFFFFF) ^ crc32_table[(crc & 0xff) ^ buf[k]]
  end
  return crc ^ 0xffffffff
end

#- crc32 for ota_seq as 32 bits unsigned, with init vector -1 -#
def crc32_ota_seq(seq)
  return crc32_update(bytes().add(seq, 4), 0xFFFFFFFF)
end

#-------------------------------------------------------------
 - Class for a partition table entry
 -
    typedef struct {
        uint16_t magic;
        uint8_t  type;
        uint8_t  subtype;
        uint32_t offset;
        uint32_t size;
        uint8_t  label[16];
        uint32_t flags;
    } esp_partition_info_t_simplified;
 -------------------------------------------------------------#
class Partition_info
  var type
  var subtype
  var start
  var size
  var label
  var flags

  def init(raw)
    import string

    if raw == nil || !issubclass(bytes, raw)
      self.type = 0
      self.subtype = 0
      self.start = 0
      self.size = 0
      self.label = ''
      self.flags = 0
      return
    end

    #- parse -#
    var magic = raw.get(0,2)
    if magic == 0x50AA    #- partition entry -#

      self.type = raw.get(2,1)
      self.subtype = raw.get(3,1)
      self.start = raw.get(4,4)
      self.size = raw.get(8,4)
      self.label = remove_trailing_zeroes(raw[12..27]).asstring()
      self.flags = raw.get(28,4)

    elif magic == 0xEBEB  #- MD5 -#
    else
      raise  "internal_error", string.format("invalid magic number %02X", magic)
    end
    
  end

  def tostring()
    import string
    var type_s = ""
    var subtype_s = ""
    if   self.type == 0  type_s = "app"
      if   self.subtype == 0  subtype_s = "factory"
      elif self.subtype >= 0x10 && self.subtype < 0x20 subtype_s = "ota" + str(self.subtype - 0x10)
      elif self.subtype == 0x20  subtype_s = "test"
      end
    elif self.type == 1  type_s = "data"
      if   self.subtype == 0x00  subtype_s = "otadata"
      elif self.subtype == 0x01  subtype_s = "phy"
      elif self.subtype == 0x02  subtype_s = "nvs"
      elif self.subtype == 0x03  subtype_s = "coredump"
      elif self.subtype == 0x04  subtype_s = "nvskeys"
      elif self.subtype == 0x05  subtype_s = "efuse_em"
      elif self.subtype == 0x80  subtype_s = "esphttpd"
      elif self.subtype == 0x81  subtype_s = "fat"
      elif self.subtype == 0x82  subtype_s = "spiffs"
      end
    end

    #- reformat strings -#
    if type_s != ""    type_s = " (" + type_s + ")" end
    if subtype_s != "" subtype_s = " (" + subtype_s + ")" end
    return string.format("<instance: Partition_info(%d%s,%d%s,0x%08X,0x%08X,'%s',0x%X)>",
                          self.type, type_s,
                          self.subtype, subtype_s,
                          self.start, self.size,
                          self.label, self.flags)
  end

  def tobytes()
    #- convert to raw bytes -#
    var b = bytes('AA50')     #- set magic number -#
    b.resize(32).resize(2)    #- pre-reserve 32 bytes -#
    b.add(self.type, 1)
    b.add(self.subtype, 1)
    b.add(self.start, 4)
    b.add(self.size, 4)
    var label = bytes().fromstring(self.label)
    label.resize(16)
    b = b + label
    b.add(self.flags, 4)
    return b
  end

end


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
  var maxota          #- number of highest OTA partition, default 1 (double ota0/ota1) -#
  var offset          #- offset of the otadata partition (0x2000 in length), default 0xE000 -#
  var active_otadata  #- which otadata block is active, 0 or 1, i.e. 0xE000 or 0xF000 -#
  var seq0            #- ota_seq of first block -#
  var seq1            #- ota_seq of second block -#

  def init(maxota, offset)
    self.maxota = maxota
    if self.maxota == nil  self.maxota = 1 end
    self.offset = offset
    if self.offset == nil  self.offset = 0xE000 end
    self.active_otadata = 0
    self.load()
  end

  #- update ota_max, needs to recompute everything -#
  def set_ota_max(n)
    self.maxota = n
  end

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

  #- load otadata -#
  def load()
    var otadata0 = flash.read(0xE000, 32)
    var otadata1 = flash.read(0xF000, 32)
    self.seq0 = otadata0.get(0, 4)   #- ota_seq for block 1 -#
    self.seq1 = otadata1.get(0, 4)   #- ota_seq for block 2 -#
    var valid0 = otadata0.get(28, 4) == crc32_ota_seq(self.seq0) #- is CRC32 valid? -#
    var valid1 = otadata1.get(28, 4) == crc32_ota_seq(self.seq1) #- is CRC32 valid? -#
    if !valid0   self.seq0 = nil end
    if !valid1   self.seq1 = nil end

    self._validate()
  end

  #- internally used, validate data -#
  def _validate()
    self.active_otadata = 0        #- if none is valid, default to OTA0 -#
    if self.seq0 != nil
      self.active_otadata = (self.seq0 - 1) % (self.maxota + 1)
    end
    if self.seq1 != nil && (self.seq0 == nil || self.seq1 > self.seq0)
      self.active_otadata = (self.seq1 - 1) % (self.maxota + 1)
    end
  end

  def save()
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
    bytes_to_save.add(crc32_ota_seq(seq_to_save), 4)
    
    #- erase flash area and write -#
    flash.erase(offset_to_save, 0x1000)
    flash.write(offset_to_save, bytes_to_save)
  end

  def tostring()
    return string.format("<instance: Partition_otadata(ota_active:%d, ota_seq=[%d,%d], ota_max=%d)>",
              self.active_otadata, self.seq0, self.seq1, self.maxota)
  end

end

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

  def load()
    #- load partition table from flash -#
    self.raw = flash.read(0x8000,0x1000)
  end

  #- parse the raw bytes to a structured list of partition items -#
  def parse()
    var i
    for i:0..94       # there are maximum 95 slots + md5 (0xC00)
      var item_raw = self.raw[i*32..(i+1)*32-1]
      var magic = item_raw.get(0,2)
      if magic == 0x50AA    #- partition entry -#
        var slot = Partition_info(item_raw)
        self.slots.push(slot)
      elif magic == 0xEBEB  #- MD5 -#
        self.md5 = self.raw[i*32+16..i*33-1]
        break
      else
        break
      end
    end
  end

  #- compute the highest ota<x> partition -#
  def ota_max()
    var ota_max = 0
    for slot:self.slots
      if slot.type == 0 && (slot.subtype >= 0x10 && slot.subtype < 0x20)
        var ota_num = slot.subtype - 0x10
        if ota_num > ota_max  ota_max = ota_num end
      end
    end
    return ota_max
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

    self.otadata = Partition_otadata(ota_max, otadata_offset)
  end

  #- change the active partition -#
  def set_active(n)
    if n < 0 || n > self.ota_max()  raise "value_error", "Invalid ota partition number" end
    self.otadata.set_ota_max(self.ota_max())    #- update ota_max if it changed -#
    self.otadata.set_active(n)
  end

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
    var b = self.tobytes()
    #- erase flash area and write -#
    flash.erase(0x8000, 0x1000)
    flash.write(0x8000, b)
    self.otadata.save()
  end
end

partition.Partition = Partition

return partition
#Example
#-

import partition
Partition = partition.Partition

# read
p = Partition()
print(p)

-#