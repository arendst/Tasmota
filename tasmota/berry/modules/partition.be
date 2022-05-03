#######################################################################
# Partition manager for ESP32 - ESP32C3 - ESP32S2
#
# use : `import partition`
#
# Provides low-level objects and a Web UI
#######################################################################

var partition = module('partition')

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
# class Partition_info
#
#  def init(raw)
#
  #  # check if the parition is an OTA partition
#  # if yes, return OTA number (starting at 0)
#  # if no, return nil
#  def is_ota()
#
#  #  # check if the parition is a SPIFFS partition
#  # returns bool
#  def is_spiffs()
#
#  # get the actual image size give of the partition
#  # returns -1 if the partition is not an app ota partition
#  def get_image_size()
#
#  def tostring()
#
#  def tobytes()
#end

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
# class Partition_otadata
#  var maxota          #- number of highest OTA partition, default 1 (double ota0/ota1) -#
#  var offset          #- offset of the otadata partition (0x2000 in length), default 0xE000 -#
#  var active_otadata  #- which otadata block is active, 0 or 1, i.e. 0xE000 or 0xF000 -#
#  var seq0            #- ota_seq of first block -#
#  var seq1            #- ota_seq of second block -#
#
#  #- crc32 for ota_seq as 32 bits unsigned, with init vector -1 -#
#  static def crc32_ota_seq(seq)
#
#  #---------------------------------------------------------------------#
#  # Rest of the class
#  #---------------------------------------------------------------------#
#  def init(maxota, offset)
#
#  #- update ota_max, needs to recompute everything -#
#  def set_ota_max(n)
#
#  # change the active OTA partition
#  def set_active(n)
#
#  #- load otadata from SPI Flash -#
#  def load()
#
#  # Save partition information to SPI Flash
#  def save()
#
#  # Produce a human-readable representation of the object with relevant information
#  def tostring()
#end

#-------------------------------------------------------------
 - Class for a partition table entry
 -------------------------------------------------------------#
#class Partition
#  var raw   #- raw bytes of the partition table in flash -#
#  var md5   #- md5 hash of partition list -#
#  var slots
#  var otadata   #- instance of Partition_otadata() -#
#
#  def init()
#
#  # Load partition information from SPI Flash
#  def load()
#
#  def get_ota_slot(n)
#
#  #- compute the highest ota<x> partition -#
#  def ota_max()
#
#  def load_otadata()
#
#  # get the active OTA app partition number
#  def get_active()
#
#  #- change the active partition -#
#  def set_active(n)
#
#  #- convert to human readble -#
#  def tostring()
#
#  #- convert the slots to raw bytes, ready to falsh to parition page -#
#  def tobytes()
#
#  #- write back to flash -#
#  def save()
#
#  #- invalidate SPIFFS partition to force format at next boot -#
#  #- we simply erase the first byte of the first 2 blocks in the SPIFFS partition -#
#  def invalidate_spiffs()
#end

#################################################################################
# Partition_manager_UI
#
# WebUI for the partition manager
#################################################################################
class Partition_manager_UI
  static app_size_min = 896    # Min OTA size - let's set it at a safe 896KB for minimal Tasmota32 with TLS
  static app_size_max = 3072   # Max OTA size - (4096 - 896 - 128)

  # create a method for adding a button to the main menu
  # the button 'Partition Manager' redirects to '/part_mgr?'
  def web_add_button()
    import webserver
    webserver.content_send(
      "<form id=but_part_mgr style='display: block;' action='part_mgr' method='get'><button>Partition Manager</button></form>")
  end

  #- ---------------------------------------------------------------------- -#
  #  Show a single OTA Partition
  #- ---------------------------------------------------------------------- -#
  def page_show_partition(slot, active, ota_num, maxota)
    import webserver
    import string
    #- define `bdis` style for gray disabled buttons -#
    webserver.content_send("<fieldset><style>.bdis{background:#888;}.bdis:hover{background:#888;}</style>")
    webserver.content_send(string.format("<legend><b title='Start: 0x%03X 000'>&nbsp;%s%s</b></legend>",
                                         slot.start / 0x1000, slot.label, active ? " (active)" : ""))

    webserver.content_send(string.format("<p><b>Partition size: </b>%i KB</p>", slot.size / 1024))
    var used = slot.get_image_size()
    if used > slot.size   slot.used = -1 end         # we may have a leftover of a previous firmware but the slot shrank - in this case the slot is inknown
    if used >= 0
      webserver.content_send(string.format("<p><b>Used: </b>%i KB</p>", used / 1024))
      webserver.content_send(string.format("<p><b>Free: </b>%i KB</p>", (slot.size - used) / 1024))
    else
      webserver.content_send("<p><b>Used</b>: unknown</p>")
      webserver.content_send("<p><b>Free</b>: unknown</p>")
    end
    if maxota != nil && maxota > 0
      if !active && used > 0
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
    end
    
    webserver.content_send("<p></p></fieldset><p></p>")
  end


  #- ---------------------------------------------------------------------- -#
  #  Show a single OTA Partition
  #- ---------------------------------------------------------------------- -#
  def page_show_spiffs(slot, free_mem)
    import webserver
    import string
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
        self.page_show_partition(slot, ota_num == p.otadata.active_otadata, ota_num, p.otadata.maxota)
      elif slot.is_factory()
        self.page_show_partition(slot, false, nil, nil)
      elif slot.is_spiffs()
        var flash_size = tasmota.memory()['flash'] * 1024
        var used_size = (slot.start + slot.size)
        self.page_show_spiffs(slot, slot == p.slots[-1] ? flash_size - used_size : nil)
      end
    end
  end

  #- ---------------------------------------------------------------------- -#
  #- Display the Re-partition section - both OTA different sizes
  #- ---------------------------------------------------------------------- -#
  def page_show_repartition_asym(p)
    import webserver
    import string
    if p.get_active() != 0
      webserver.content_send("<p style='width:320px;'>Re-partition can be done only if 'app0' is active.</p>")
    else
      # we can proceed
      var app0 = p.get_ota_slot(0)
      var app0_size_kb = ((app0.size / 1024 + 63) / 64) * 64 # rounded to upper 64kb
      var app0_used_kb = (((app0.get_image_size()) / 1024 / 64) + 1) * 64

      var app1 = p.get_ota_slot(1)
      var app1_size_kb = ((app1.size / 1024 + 63) / 64) * 64 # rounded to upper 64kb
      # var app1_used_kb = (((app1.get_image_size()) / 1024 / 64) + 1) * 64   # we don't actually need it

      var flash_size_kb = tasmota.memory()['flash']

      webserver.content_send("<p><b>Resize app Partitions.</b><br>It is highly recommended to set<br>both partition with the same size.<br>SPIFFS is adjusted accordinlgy.</p>")

      webserver.content_send("<form action='/part_mgr' method='post' ")
      webserver.content_send("onsubmit='return confirm(\"This will DELETE the content of the file system and cause a restart.\");'>")

      webserver.content_send("<hr><p><b>app0:</b></p>")
      webserver.content_send(string.format("<p><b>Min:</b> %i KB</p>", app0_used_kb))
      webserver.content_send(string.format("<p><b>Max:</b> %i KB</p>", self.app_size_max))
      webserver.content_send("<p><b>New:</b> (multiple of 64 KB)</p>")
      webserver.content_send(string.format("<input type='number' min='%d' max='%d' step='64' name='app0' value='%i'>", app0_used_kb, self.app_size_max, app0_size_kb))

      webserver.content_send("<hr><p><b>app1:</b></p>")
      webserver.content_send(string.format("<p><b>Min:</b> %i KB</p>", self.app_size_min))
      webserver.content_send(string.format("<p><b>Max:</b> %i KB</p>", self.app_size_max))
      webserver.content_send("<p><b>New:</b> (multiple of 64 KB)</p>")
      webserver.content_send(string.format("<input type='number' min='%d' max='%d' step='64' name='app1' value='%i'>", self.app_size_min, self.app_size_max, app1_size_kb))

      webserver.content_send("<p></p><button name='resize' class='button bred'>Resize Partitions</button></form></p>")
  
    end
  end
  #######################################################################
  # Display the complete page
  #######################################################################
  def page_part_mgr()
    import webserver
    import string
    import partition_core
    if !webserver.check_privileged_access() return nil end
    var p = partition_core.Partition()

    webserver.content_start("Partition Manager")           #- title of the web page -#
    webserver.content_send_style()                  #- send standard Tasmota styles -#

    # webserver.content_send("<p style='width:340px;'><b style='color:#f56'>Warning:</b> This can brick your device. Don't use unless you know what you are doing.</p>")
    webserver.content_send("<fieldset><legend><b>&nbsp;Partition Manager</b></legend><p></p>")
    webserver.content_send("<p style='width:320px;'><b style='color:#f56'>Warning:</b> This can brick your device.</p>")
    self.page_show_partitions(p)
    webserver.content_send("<p></p></fieldset><p></p>")

    if p.otadata.maxota > 0
      webserver.content_send("<fieldset><legend><b>&nbsp;Re-partition</b></legend><p></p>")
      self.page_show_repartition_asym(p)
      webserver.content_send("<p></p></fieldset><p></p>")
    end

    webserver.content_button(webserver.BUTTON_MANAGEMENT) #- button back to management page -#
    webserver.content_stop()                        #- end of web page -#
  end

  #######################################################################
  # Web Controller, called by POST to `/part_mgr`
  #######################################################################
  def page_part_ctl()
    import webserver
    import string
    import partition_core
    if !webserver.check_privileged_access() return nil end

    #- check that the partition is valid -#
    var p = partition_core.Partition()

    try
      #---------------------------------------------------------------------#
      # Switch OTA partition from one to another
      #---------------------------------------------------------------------#
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

      #---------------------------------------------------------------------#
      # Resize the SPIFFS partition, generally to extend it to full free size
      #---------------------------------------------------------------------#
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

      #---------------------------------------------------------------------#
      # Repartition symmetrical OTA with a new SPIFFS size
      #---------------------------------------------------------------------#
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

        var part_size_kb = int(webserver.arg("repartition"))
        if part_size_kb < app0_used_kb || part_size_kb > self.app_size_max
          raise "value_error", string.printf("Invalid partition size %i KB, should be between %i and %i", part_size_kb, app0_used_kb, self.app_size_max)
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

      #---------------------------------------------------------------------#
      # Repartition OTA with a new SPIFFS size
      #---------------------------------------------------------------------#
      elif webserver.has_arg("app0") && webserver.has_arg("app1")
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
        var app1_size_kb = ((app1.size / 1024 + 63) / 64) * 64 # rounded to upper 64kb
        var flash_size_kb = tasmota.memory()['flash']

        var part0_size_kb = int(webserver.arg("app0"))
        if part0_size_kb < app0_used_kb || part0_size_kb > self.app_size_max
          raise "value_error", string.printf("Invalid partition size app%i %i KB, should be between %i and %i", 0, part0_size_kb, app0_used_kb, self.app_size_max)
        end
        var part1_size_kb = int(webserver.arg("app1"))
        if part1_size_kb < self.app_size_min || part1_size_kb > self.app_size_max
          raise "value_error", string.printf("Invalid partition size app%i %i KB, should be between %i and %i", 1, part1_size_kb, self.app_size_min, self.app_size_max)
        end
        if part0_size_kb == app0_size_kb && part1_size_kb == app1_size_kb raise "value_error", "No change to partition sizes, abort" end

        #- all good, proceed -#
        # resize app0
        app0.size = part0_size_kb * 1024
        # change app1
        app1.start = app0.start + app0.size
        app1.size = part1_size_kb * 1024
        # change spiffs
        spiffs.start = app1.start + app1.size
        spiffs.size = flash_size_kb * 1024 - spiffs.start

        p.save()
        p.invalidate_spiffs()   # erase SPIFFS or data is corrupt
        #- and force restart -#
        webserver.redirect("/?rst=")
      #---------------------------------------------------------------------#
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
    import webserver
    #- we need to register a closure, not just a function, that captures the current instance -#
    webserver.on("/part_mgr", / -> self.page_part_mgr(), webserver.HTTP_GET)
    webserver.on("/part_mgr", / -> self.page_part_ctl(), webserver.HTTP_POST)
  end
end
partition.Partition_manager_UI = Partition_manager_UI


#- create and register driver in Tasmota -#
if tasmota
  import partition_core
  var partition_manager_ui = partition.Partition_manager_UI()
  tasmota.add_driver(partition_manager_ui)
  ## can be removed if put in 'autoexec.bat'
  partition_manager_ui.web_add_handler()
end

return partition

#- Example

import partition

# read
p = partition.Partition()
print(p)

-#
