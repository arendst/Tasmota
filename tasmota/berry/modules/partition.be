#-------------------------------------------------------------
 - Parser for ESP32 partition
 -
 -------------------------------------------------------------#
partition = module('partition')

import flash
import string
import webserver

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

var crc32_table = crc32_create_table()

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

  # check if the parition is an OTA partition
  # if yes, return OTA number (starting at 0)
  # if no, return nil
  def is_ota()
    if self.type == 0 && (self.subtype >= 0x10 && self.subtype < 0x20)
      return self.subtype - 0x10
    end
  end

  # check if the parition is a SPIFFS partition
  # returns bool
  def is_spiffs()
    return self.type == 1 && self.subtype == 130
  end

  # get the actual image size give of the partition
  # returns -1 if the partition is not an app ota partition
  def get_image_size()
    if self.is_ota() == nil return -1 end
    try
      var addr = self.start
      var magic_byte = flash.read(addr, 1).get(0, 1)
      if magic_byte != 0xE9 return -1 end
      
      var seg_count = flash.read(addr+1, 1).get(0, 1)
      # print("Segment count", seg_count)
      
      var seg_offset = addr + 0x20 # sizeof(esp_image_header_t) + sizeof(esp_image_segment_header_t) = 24 + 8

      for seg_num:0..seg_count-1
        # print(string.format("Reading 0x%08X", seg_offset))
        var segment_header = flash.read(seg_offset - 8, 8)
        var seg_start_addr = segment_header.get(0, 4)
        var seg_size = segment_header.get(4,4)
        # print(string.format("Segment %i: flash_offset=0x%08X start_addr=0x%08X size=0x%08X", seg_num, seg_offset, seg_start_addr, seg_size))

        seg_offset += seg_size + 8    # add segment_length + sizeof(esp_image_segment_header_t)
      end
      var total_size = seg_offset - addr + 1 # add 1KB for safety

      # print(string.format("Total size = %i KB", total_size/1024))

      return total_size
    except .. as e, m
      print(string.format("BRY: Exception> '%s' - %s", e, m))
      return -1
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

  def get_ota_slot(n)
    for slot: self.slots
      if slot.is_ota() == n return slot end
    end
    return nil
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

  # get the active OTA app partition number
  def get_active()
    return self.otadata.active_otadata
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

  #- invalidate SPIFFS partition to force format at next boot -#
  #- we simply erase the first byte of the first 2 blocks in the SPIFFS partition -#
  def invalidate_spiffs()
    #- we expect the SPIFFS partition to be the last one -#
    var spiffs = self.slots[-1]
    if !spiffs.is_spiffs() raise 'value_error', 'No SPIFFS partition found' end

    var b = bytes("00")  #- flash memory: we can turn bits from '1' to '0' -#
    flash.write(spiffs.start         , b)    #- block #0 -#
    flash.write(spiffs.start + 0x1000, b)    #- block #1 -#
  end  
end


#-------------------------------------------------------------
 - Parser manager for ESP32
 -
 -------------------------------------------------------------#

class Partition_manager : Driver

  def init()
  end

  # create a method for adding a button to the main menu
  # the button 'Partition Manager' redirects to '/part_mgr?'
  def web_add_button()
    webserver.content_send(
      "<form id=but_part_mgr style='display: block;' action='part_mgr' method='get'><button>Partition Manager</button></form>")
  end

  #- ---------------------------------------------------------------------- -#
  #  Show a single OTA Partition
  #- ---------------------------------------------------------------------- -#
  def page_show_partition(slot, active, ota_num)
    #- define `bdis` style for gray disabled buttons -#
    webserver.content_send("<fieldset><style>.bdis{background:#888;}.bdis:hover{background:#888;}</style>")
    webserver.content_send(string.format("<legend><b title='Start: 0x%03X 000'>&nbsp;%s%s</b></legend>",
                                         slot.start / 0x1000, slot.label, active ? " (active)" : ""))

    webserver.content_send(string.format("<p><b>Partition size: </b>%i KB</p>", slot.size / 1024))
    var used = slot.get_image_size()
    if used >= 0
      webserver.content_send(string.format("<p><b>Used: </b>%i KB</p>", used / 1024))
      webserver.content_send(string.format("<p><b>Free: </b>%i KB</p>", (slot.size - used) / 1024))
    else
      webserver.content_send("<p><b>Used</b>: unknwon</p>")
      webserver.content_send("<p><b>Free</b>: unknwon</p>")
    end
    if !active && used >= 0
      webserver.content_send("<p><form id=setactive style='display: block;' action='/part_mgr' method='post' ")
      webserver.content_send("onsubmit='return confirm(\"This will change the active partition and cause a restart.\");'>")
      webserver.content_send("<button name='setactive' class='button bgrn'>Switch To This Partition</button>")
      webserver.content_send(string.format("<input name='ota' type='hidden' value='%d'>", ota_num))
      webserver.content_send("</form></p>")
    else
      # put a fake disabled button
      webserver.content_send("<p><form style='display: block;'>")
      if used >= 0
        webserver.content_send("<button name='setactive' class='button bdis' disabled title=\"No need to click, it's already the active partition\">Current Active Partition</button>")
      else
        webserver.content_send("<button name='setactive' class='button bdis' disabled>Empty Partition</button>")
      end
      webserver.content_send("</form></p>")
    end
    
    webserver.content_send("<p></p></fieldset><p></p>")
  end


  #- ---------------------------------------------------------------------- -#
  #  Show a single OTA Partition
  #- ---------------------------------------------------------------------- -#
  def page_show_spiffs(slot, free_mem)
    webserver.content_send(string.format("<fieldset><legend><b title='Start: 0x%03X 000'>&nbsp;%s</b></legend>",
                                         slot.start / 0x1000, slot.label))

    webserver.content_send(string.format("<p><b>Partition size:</b> %i KB</p>", slot.size / 1024))
    
    if free_mem != nil
      webserver.content_send(string.format("<p><b>Max size: </b>%i KB</p>", (slot.size + free_mem) / 1024))
      webserver.content_send(string.format("<p><b>Unallocated: </b>%i KB</p>", free_mem / 1024))
    end

    #- display Resize button -#
    webserver.content_send("<hr><p><b>New size:</b> (multiple of 16 KB)</p>")
    webserver.content_send("<form action='/part_mgr' method='post' ")
    webserver.content_send("onsubmit='return confirm(\"This will DELETE the content of the file system and cause a restart.\");'>")
    webserver.content_send(string.format("<input type='number' min='0' max='%d' step='16' name='spiffs_size' value='%i'>", (slot.size + free_mem) / 1024, ((slot.size + free_mem) / 1024 / 16)*16))
    webserver.content_send("<p></p><button name='resize' class='button bred'>Resize SPIFFS</button></form></p>")
    webserver.content_send("<p></p></fieldset><p></p>")
  end

  #- ---------------------------------------------------------------------- -#
  #- Show each partition one after the other - only OTA and SPIFFS
  #- ---------------------------------------------------------------------- -#
  def page_show_partitions(p)
    # display ota partitions
    for slot: p.slots
      # is the slot app?
      var ota_num = slot.is_ota()
      if ota_num != nil
        # we have an OTA partition
        self.page_show_partition(slot, ota_num == p.otadata.active_otadata, ota_num)
      elif slot.is_spiffs()
        var flash_size = tasmota.memory()['flash'] * 1024
        var used_size = (slot.start + slot.size)
        self.page_show_spiffs(slot, slot == p.slots[-1] ? flash_size - used_size : nil)
      end
    end
  end

  #- ---------------------------------------------------------------------- -#
  #- Display the Re-partition section
  #- ---------------------------------------------------------------------- -#
  def page_show_repartition(p)
    if p.get_active() != 0
      webserver.content_send("<p style='width:320px;'>Re-partition can be done only if 'app0' is active.</p>")
    else
      # we can proceed
      var app0 = p.get_ota_slot(0)
      var app0_size_kb = ((app0.size / 1024 + 63) / 64) * 64 # rounded to upper 64kb
      var app0_used_kb = (((app0.get_image_size()) / 1024 / 64) + 1) * 64
      var flash_size_kb = tasmota.memory()['flash']
      var app_size_max = 1984   # Max OTA size (4096 - 64) / 2 rounded to lowest 64KB

      webserver.content_send("<p><b>Resize app Partitions</b></p>")
      webserver.content_send(string.format("<p><b>Min:</b> %i KB</p>", app0_used_kb))
      webserver.content_send(string.format("<p><b>Max:</b> %i KB</p>", app_size_max))
      webserver.content_send("<p><b>New:</b> (multiple of 64 KB)</p>")
      webserver.content_send("<form action='/part_mgr' method='post' ")
      webserver.content_send("onsubmit='return confirm(\"This will DELETE the content of the file system and cause a restart.\");'>")
      webserver.content_send(string.format("<input type='number' min='%d' max='%d' step='64' name='repartition' value='%i'>", app0_used_kb, app_size_max, app0_size_kb))
      webserver.content_send("<p></p><button name='resize' class='button bred'>Resize Partitions</button></form></p>")
  
    end
  end

  #- this method displays the web page -#
  def page_part_mgr()
    if !webserver.check_privileged_access() return nil end
    var p = partition.Partition()

    webserver.content_start("Partition Manager")           #- title of the web page -#
    webserver.content_send_style()                  #- send standard Tasmota styles -#

    # webserver.content_send("<p style='width:340px;'><b style='color:#f56'>Warning:</b> This can brick your device. Don't use unless you know what you are doing.</p>")
    webserver.content_send("<fieldset><legend><b>&nbsp;Partition Manager</b></legend><p></p>")
    webserver.content_send("<p style='width:320px;'><b style='color:#f56'>Warning:</b> This can brick your device.</p>")
    self.page_show_partitions(p)
    webserver.content_send("<p></p></fieldset><p></p>")

    webserver.content_send("<fieldset><legend><b>&nbsp;Re-partition</b></legend><p></p>")
    self.page_show_repartition(p)
    webserver.content_send("<p></p></fieldset><p></p>")


    webserver.content_button(webserver.BUTTON_MANAGEMENT) #- button back to management page -#
    webserver.content_stop()                        #- end of web page -#
  end

  #- ---------------------------------------------------------------------- -#
  #- this is the controller, called using POST and changing parameters
  #- ---------------------------------------------------------------------- -#
  def page_part_ctl()
    if !webserver.check_privileged_access() return nil end

    #- check that the partition is valid -#
    var p = partition.Partition()

    try
      if webserver.has_arg("ota")
        #- OTA switch partition -#
        var ota_target = int(webserver.arg("ota"))
        if ota_target < 0 || ota_target > p.ota_max() raise "value_error", string.format("Invalid partition #%d", ota_target) end
        var ota_slot = p.get_ota_slot(ota_target)

        if ota_slot == nil || ota_slot.get_image_size() < 0
          raise "value_error", string.format("Invalid OTA slot #%d", ota_target)
        end

        print(string.format("Trying to change active partition to %d", ota_target))
        #- do the change -#
        p.set_active(ota_target)
        p.otadata.save()   #- write to disk -#

        #- and force restart -#
        webserver.redirect("/?rst=")
      elif webserver.has_arg("spiffs_size")
        #- SPIFFS size change -#
        var spiffs_size_kb = int(webserver.arg("spiffs_size"))
        var spiffs_slot = p.slots[-1]   # last slot

        var spiffs_max_size = ((tasmota.memory()['flash'] - (spiffs_slot.start / 1024)) / 16) * 16
        
        if spiffs_slot == nil || !spiffs_slot.is_spiffs() raise "value_error", "Last slot is not SPIFFS type" end
        var flash_size_kb = tasmota.memory()['flash']
        if spiffs_size_kb < 0 || spiffs_size_kb > spiffs_max_size
          raise "value_error", string.format("Invalid spiffs_size %i, should be between 0 and %i", spiffs_size_kb, spiffs_max_size)
        end
        if spiffs_size_kb == spiffs_slot.size/1024 raise "value_error", "SPIFFS size unchanged, abort" end

        #- write the new SPIFFS partition size -#
        spiffs_slot.size = spiffs_size_kb * 1024
        p.save()
        p.invalidate_spiffs()   # erase SPIFFS or data is corrupt

        #- and force restart -#
        webserver.redirect("/?rst=")

      elif webserver.has_arg("repartition")
        if p.get_active() != 0 raise "value_error", "Can't repartition unless active partition is app0" end
        #- complete repartition -#
        var app0 = p.get_ota_slot(0)
        var app1 = p.get_ota_slot(1)
        var spiffs = p.slots[-1]

        if !spiffs.is_spiffs() raise 'internal_error', 'No SPIFFS partition found' end
        if app0 == nil || app1 == nil
          raise "internal_error", "Unable to find partitions app0 and app1"
        end
        if p.ota_max() != 1
          raise "internal_error", "There are more than 2 OTA partition, abort"
        end
        var app0_size_kb = ((app0.size / 1024 + 63) / 64) * 64 # rounded to upper 64kb
        var app0_used_kb = (((app0.get_image_size()) / 1024 / 64) + 1) * 64
        var flash_size_kb = tasmota.memory()['flash']
        var app_size_max = 1984   # Max OTA size (4096 - 64) / 2 rounded to lowest 64KB

        var part_size_kb = int(webserver.arg("repartition"))
        if part_size_kb < app0_used_kb || part_size_kb > app_size_max
          raise "value_error", string.printf("Invalid partition size %i KB, should be between %i and %i", part_size_kb, app0_used_kb, app_size_max)
        end
        if part_size_kb == app0_size_kb raise "value_error", "No change to partition size, abort" end

        #- all good, proceed -#
        # resize app0
        app0.size = part_size_kb * 1024
        # change app1
        app1.start = app0.start + app0.size
        app1.size = part_size_kb * 1024
        # change spiffs
        spiffs.start = app1.start + app1.size
        spiffs.size = flash_size_kb * 1024 - spiffs.start

        p.save()
        p.invalidate_spiffs()   # erase SPIFFS or data is corrupt
        #- and force restart -#
        webserver.redirect("/?rst=")
      else
        raise "value_error", "Unknown command"
      end
    except .. as e, m
      print(string.format("BRY: Exception> '%s' - %s", e, m))
      #- display error page -#
      webserver.content_start("Parameter error")           #- title of the web page -#
      webserver.content_send_style()                  #- send standard Tasmota styles -#

      webserver.content_send(string.format("<p style='width:340px;'><b>Exception:</b><br>'%s'<br>%s</p>", e, m))
      # webserver.content_send("<p></p></fieldset><p></p>")

      webserver.content_button(webserver.BUTTON_MANAGEMENT) #- button back to management page -#
      webserver.content_stop()                        #- end of web page -#
    end
  end

  #- ---------------------------------------------------------------------- -#
  # respond to web_add_handler() event to register web listeners
  #- ---------------------------------------------------------------------- -#
  #- this is called at Tasmota start-up, as soon as Wifi/Eth is up and web server running -#
  def web_add_handler()
    #- we need to register a closure, not just a function, that captures the current instance -#
    webserver.on("/part_mgr", / -> self.page_part_mgr(), webserver.HTTP_GET)
    webserver.on("/part_mgr", / -> self.page_part_ctl(), webserver.HTTP_POST)
  end

end

#- create and register driver in Tasmota -#
var partition_manager = Partition_manager()
tasmota.add_driver(partition_manager)
## can be removed if put in 'autoexec.bat'
partition_manager.web_add_handler()

partition.Partition = Partition

return partition

#- Example

import partition

# read
p = partition.Partition()
print(p)

-#
