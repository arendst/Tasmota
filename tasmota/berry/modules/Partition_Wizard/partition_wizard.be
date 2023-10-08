#######################################################################
# Partition Wizard for ESP32 - ESP32C3 - ESP32S2
#
# use : `import partition_wizard`
#
# Provides low-level objects and a Web UI
# rm Partition_Wizard.tapp; zip Partition_Wizard.tapp -j -0 Partition_Wizard/*
#######################################################################

var partition_wizard = module('partition_wizard')

#################################################################################
# Partition_wizard_UI
#
# WebUI for the partition manager
#################################################################################
class Partition_wizard_UI
  static app_size_min = 832    # Min OTA size - let's set it at a safe 896KB for minimal Tasmota32 with TLS
  static app_size_max = 3072   # Max OTA size - (4096 - 896 - 128)
  static _default_safeboot_URL = "http://ota.tasmota.com/tasmota32/release/tasmota32%s-safeboot.bin"

  def init()
    import persist

    if persist.find("factory_migrate") == true
      # remove marker to avoid bootloop if something goes wrong
      tasmota.log("UPL: Resuming after step 1", 2)
      persist.remove("factory_migrate")
      persist.save()

      # continue the migration process 5 seconds after Wifi is connected
      def continue_after_5s()
        tasmota.remove_rule("parwiz_5s1")      # first remove rule to avoid firing it again at Wifi reconnect
        tasmota.remove_rule("parwiz_5s2")      # first remove rule to avoid firing it again at Wifi reconnect
        tasmota.set_timer(5000, /-> self.do_safeboot_partitioning())  # delay by 5 s
      end
      tasmota.add_rule("Wifi#Connected=1", continue_after_5s, "parwiz_5s1")
      tasmota.add_rule("Wifi#Connected==1", continue_after_5s, "parwiz_5s2")
      
    end
  end

  # ----------------------------------------------------------------------
  # Patch partition core since we can't chang the solidified code
  # ----------------------------------------------------------------------
  def patch_partition_core(p)
    var otadata = p.otadata

    # patch load
    import flash
    var otadata0 = flash.read(otadata.offset, 32)
    var otadata1 = flash.read(otadata.offset + 0x1000, 32)
    otadata.seq0 = otadata0.get(0, 4)   #- ota_seq for block 1 -#
    otadata.seq1 = otadata1.get(0, 4)   #- ota_seq for block 2 -#
    var valid0 = otadata0.get(28, 4) == otadata.crc32_ota_seq(otadata.seq0) #- is CRC32 valid? -#
    var valid1 = otadata1.get(28, 4) == otadata.crc32_ota_seq(otadata.seq1) #- is CRC32 valid? -#
    if !valid0   otadata.seq0 = nil end
    if !valid1   otadata.seq1 = nil end

    otadata._validate()
  end

  def default_safeboot_URL()
    import string
    var arch_sub = tasmota.arch()
    if arch_sub[0..4] == "esp32"
      arch_sub = arch_sub[5..]    # get the esp32 variant
    end
    return string.format(self._default_safeboot_URL, arch_sub)
  end

  # create a method for adding a button to the main menu
  # the button 'Partition Wizard' redirects to '/part_wiz?'
  def web_add_button()
    import webserver
    webserver.content_send(
      "<form id=but_part_mgr style='display: block;' action='part_wiz' method='get'><button>Partition Wizard</button></form><p></p>")
  end

  # ----------------------------------------------------------------------
  # Get last fs
  # 
  # Get the last fs partition
  # Return the actual slot
  # ----------------------------------------------------------------------
  def get_last_fs(p)
    var sz = size(p.slots)
    var idx = 1
    while idx < sz
      var slot = p.slots[-idx]
      if slot.is_spiffs()
        return slot
      end
      idx += 1
    end
    return nil
  end

  #- ---------------------------------------------------------------------- -#
  #- Get fs unallocated size
  #- ---------------------------------------------------------------------- -#
  def get_unallocated_k(p)
    var last_fs = self.get_last_fs(p)
    if last_fs != nil
      # verify that last slot is filesystem
      var flash_size_k = self.get_max_flash_size_k(p)
      var partition_end_k = (last_fs.start + last_fs.sz) / 1024   # last kb used for fs
      if  partition_end_k < flash_size_k
        return flash_size_k - partition_end_k
      end
    end
    return 0
  end

  #- ---------------------------------------------------------------------- -#
  #- Get max fs start address when expanded to maximum
  #- ---------------------------------------------------------------------- -#
  def get_max_fs_start_k(p)
    var last_fs = p.slots[-1]
    if last_fs != nil    # verify that last slot is filesystem
      # get end of previous partition slot
      var last_app = p.slots[-2]
      # round upper 64kB
      var max_fs_start_k = 64 * (((last_app.start + last_app.get_image_size() + 1023) / 1024 + 63) / 64)
      return max_fs_start_k
    end
    return 0
  end

  #- ---------------------------------------------------------------------- -#
  #- Get max flash size
  #
  #  Takes into account that the flash size written may not be accurate
  #  and the flash chip may be larger
  #- ---------------------------------------------------------------------- -#
  def get_max_flash_size_k(p)
    var flash_size_k = tasmota.memory()['flash']
    var flash_size_real_k = tasmota.memory().find("flash_real", flash_size_k)
    if (flash_size_k != flash_size_real_k) && self.get_flash_definition_sector(p) != nil
      flash_size_k = flash_size_real_k    # try to expand the flash size definition
    end
    return flash_size_k
  end

  # ----------------------------------------------------------------------
  # Remove any non wanted partion after last FS
  # ----------------------------------------------------------------------
  def remove_partition_after_last_fs(p)
    # remove any partition after last fs
    do
      var last_fs = self.get_last_fs(p)
      var changed = false
      if last_fs != nil
        while true
          var last_slot = p.slots[-1]
          if !last_slot.is_spiffs() && (last_slot.type != 0)
            p.slots.remove(size(p.slots) - 1)     # remove last slot
            changed = true
          else
            break
          end
        end
        if changed    p.save()    end
      end
    end
  end

  #- ---------------------------------------------------------------------- -#
  #- Resize flash definition if needed
  #- ---------------------------------------------------------------------- -#
  def resize_max_flash_size_k(p)
    self.remove_partition_after_last_fs(p)
    var flash_size_k = tasmota.memory()['flash']
    var flash_size_real_k = tasmota.memory().find("flash_real", flash_size_k)
    var flash_definition_sector = self.get_flash_definition_sector(p)
    if (flash_size_k != flash_size_real_k) && flash_definition_sector != nil
      import flash
      import string

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
        tasmota.log(string.format("UPL: changing flash definition from 0x02X to 0x%02X", old_def, flash_def[3]), 3)
      else
        raise "internal_error", "wrong flash size "+str(flash_size_real_m)
      end
    end
  end

  #- ---------------------------------------------------------------------- -#
  #- Get current fs size
  #- ---------------------------------------------------------------------- -#
  def get_cur_fs_size_k(p)
    var last_fs = p.slots[-1]
    if last_fs != nil
      return (last_fs.sz + 1023) / 1024
    end
    return 0
  end

  #- ---------------------------------------------------------------------- -#
  #- Get flash sector for flash chip definition
  #  It appears to be at 0x1000 for ESP32, but at 0x0000 for ESP32C3/S3
  #
  #  returns offset of sector containing flash information
  #  or `nil` if not found
  #
  #  Internally looks first at 0x0000 then at 0x1000 for magic byte
  #- ---------------------------------------------------------------------- -#
  def get_flash_definition_sector(p)
    import flash
    for i:0..1
      var offset = i * 0x1000
      if flash.read(offset, 1) == bytes('E9')   return offset end
    end
  end

  #- ---------------------------------------------------------------------- -#
  #- Propose to resize FS to max if some memory in unallocated
  #- ---------------------------------------------------------------------- -#
  def show_resize_fs(p)
    import webserver
    import string
    var unallocated = self.get_unallocated_k(p)

    # if there is unallocated space, propose only to claim it
    if unallocated > 0
      webserver.content_send("<fieldset><legend><b>&nbsp;Resize FS to max&nbsp;</b></legend><p></p>")

      webserver.content_send(string.format("<p>You can expand the file system by %i KB.<br>Its content will be lost.</p>", unallocated))

      webserver.content_send("<form action='/part_wiz' method='post' ")
      if !self.support_grow_on_mount()
        webserver.content_send("onsubmit='return confirm(\"This will DELETE the content of the file system and cause a restart.\");'")
      end
      webserver.content_send("><p></p><button name='max_fs' class='button bred'>Resize FS to max</button></form></p>")
      
      webserver.content_send("<p></p></fieldset><p></p>")
    elif self.has_factory_layout(p)
      # else propose to expand or shrink the file system
      var max_fs_start_k = self.get_max_fs_start_k(p)
      var flash_size_k = self.get_max_flash_size_k()
      var fs_max_size_k = flash_size_k - max_fs_start_k
      var current_fs_size_k = self.get_cur_fs_size_k(p)
      #print(string.format(">>> max_fs_start_k=0x%X flash_size_k=0x%X fs_max_size_k=%i current_fs_size_k=%i", max_fs_start_k, flash_size_k, fs_max_size_k, current_fs_size_k))

      if max_fs_start_k > 0 && fs_max_size_k > 64
        webserver.content_send("<fieldset><legend><b>&nbsp;Resize FS&nbsp;</b></legend><p></p>")

        webserver.content_send("<p>You can expand of shrink the file system.<br>Its content will be lost.</p>")

        webserver.content_send("<form action='/part_wiz' method='post' ")
        webserver.content_send("onsubmit='return confirm(\"This will DELETE the content of the file system and cause a restart.\");'>")
        webserver.content_send(string.format("<input type='number' min='64' max='%d' step='64' name='fs_size' value='%i'>", fs_max_size_k, current_fs_size_k))
        webserver.content_send("<p></p><button name='resize_fs' class='button bred'>Resize FS</button></form></p>")
        
        webserver.content_send("<p></p></fieldset><p></p>")
      end
    end
  end

  #- ---------------------------------------------------------------------- -#
  #- Tests for factory layout
  #- ---------------------------------------------------------------------- -#
  # Returns if the device already has a factory layout:
  #     devices has 1 factory partition
  #     device has at least 1 OTA partition
  #     last partition is FS
  #
  # returns true or false
  def has_factory_layout(p)
    return p.has_factory() && p.ota_max() != nil && p.slots[-1].is_spiffs()
  end

  #- ---------------------------------------------------------------------- -#
  #- Tests for factory migration
  #- ---------------------------------------------------------------------- -#
  # Returns if the device is eligible for a migration to factory layout:
  #     devices has 2x OTA partitions
  #     device has no factory partition
  #
  # returns true or false
  def factory_migrate_eligible(p)
    if p.ota_max() <= 0     return false end      # device does not have 2x OTA
    if p.get_factory_slot() != nil return false end
    if self.get_last_fs(p) == nil return false end
    return true                                  # device does not have factory partition
  end

  # ----------------------------------------------------------------------
  # Step 1:
  # - pre-condition:
  #     factory_migrate_eligible(p) returns true
  # - DONE state:
  #     boot on `app1`
  # - READY state:
  #     boot on `app0`
  # - Needed steps:
  #     check that `app1` is large enough for firmware in `app0`
  #     copy `app0` to `app1`
  #     restart on `app1`
  #     set continuation marker in persist to continue migration process at next boto
  #
  # Returns:
  # - false if READY
  # - true if DONE
  # - string if ERROR, indicating the error
  def test_step_1(p)
    import string
    if !self.factory_migrate_eligible(p)    return "not eligible to migration" end

    var cur_part = p.otadata.active_otadata     # -1=factory 0=ota_0 1=ota_1...
    if cur_part == 1    return true end
    if cur_part != 0    return string.format("active_otadata=%i", cur_part) end # unsupported configuration
    # current partition is `app0`
    # get size of firmware in `app0` and check if it fits on `app1`
    var app0 = p.get_ota_slot(0)
    var app1 = p.get_ota_slot(0)
    var app0_firmware_size = (app0 != nil) ? app0.get_image_size() : -1
    var app1_size          = (app1 != nil) ? app1.sz : -1
    if app0_firmware_size < 0 || app1_size < 0 return "can't find app0/1 sizes" end
    if app0_firmware_size >= app1_size  return "`app1` is too small" end
    return false
  end

  # ----------------------------------------------------------------------
  # Step 2:
  # - pre-condition:
  #     factory_migrate_eligible(p) returns true
  # - DONE state:
  #     `safeboot` flashed to `app0`
  #     `safeboot` is smaller than 832KB
  # - READY state:
  #     false `safeboot` to `app0`
  # - Needed steps:
  #     get `safeboot` URL
  #     check that `app0` is large enough for `safeboot`
  #     check that `safeboot` is smaller than 832KB
  #     flash `safeboot` on `app0`
  #
  # Returns:
  # - false if READY
  # - true if DONE
  # - string if ERROR, indicating the error
  def test_step_2(p)
    if !self.factory_migrate_eligible(p)    return "not eligible to migration" end

    var app0 = p.get_ota_slot(0)
    if app0.sz < (self.app_size_min * 1024)        return "`app0` is too small for `safeboot`" end
    var app0_image_size = app0.get_image_size()
    if (app0_image_size > 0) && (app0_image_size < (self.app_size_min * 1024))        return true end
    return false
  end

  # ----------------------------------------------------------------------
  # Step 3:
  # - pre-condition:
  #     booted on `app1` and `safeboot` flashed to `app0`
  # - DONE state:
  #     Partition map is:
  #       `factory` with `safeboot` flashed, sized to 832KB
  #       `app0` resized to take all the remaining size but empty
  # - READY state:
  #     `app0` is flashed with `safeboot`
  # - Needed steps:
  #     `app0` renamed to `safeboot`
  #     `app0` changed subtype to `factory`
  #     `app1` moved to right after `factory` and resized
  #     `app1` changed subtype to `app0` and renamed `app0`
  #     remove any partition past the last fs
  #
  # Returns:
  # - false if READY
  # - true if DONE
  # - string if ERROR, indicating the error
  def test_step_3(p)
    if !self.factory_migrate_eligible(p)    return "not eligible to migration" end

    return false
    # var app0 = p.get_ota_slot(0)
    # if app0.get_image_size() > (self.app_size_min * 1024)        return "`app0` is too small for `safeboot`" end
  end


  # ----------------------------------------------------------------------
  # Step 4:
  # - pre-condition:
  #
  # Returns:
  # - false if READY
  # - true if DONE
  # - string if ERROR, indicating the error
  def test_step_4(p)
    
    return false
    # var app0 = p.get_ota_slot(0)
    # if app0.get_image_size() > (self.app_size_min * 1024)        return "`app0` is too small for `safeboot`" end
  end

  static def copy_ota(from_addr, to_addr, sz)
    import flash
    import string
    var size_left = sz
    var offset = 0
  
    tasmota.log(string.format("UPL: Copy flash from 0x%06X to 0x%06X (size: %ikB)", from_addr, to_addr, sz / 1024), 2)
    while size_left > 0
      var b = flash.read(from_addr + offset, 4096)
      flash.erase(to_addr + offset, 4096)
      flash.write(to_addr + offset, b, true)
      size_left -= 4096
      offset += 4096
      if ((offset-4096) / 102400) < (offset / 102400)
        tasmota.log(string.format("UPL: Progress %ikB", offset/1024), 3)
      end
    end
    tasmota.log("UPL: done", 2)
  end

  def do_step_1(p)
    import persist
    var step1_state = self.test_step_1(p)
    if step1_state == true return true end
    if type(step1_state) == 'string)'   raise "internal_error", step1_state end

    # copy firmware frop `app0` to `app1`
    var app0 = p.get_ota_slot(0)
    var app1 = p.get_ota_slot(1)
    var app0_size = app0.get_image_size()
    if app0_size > app1.sz  raise "internal_error", "`app1` too small to copy firmware form `app0`" end
    self.copy_ota(app0.start, app1.start, app0_size)

    p.set_active(1)
    p.save()

    persist.factory_migrate = true
    persist.save()

    tasmota.log("UPL: restarting on `app1`", 2)
    tasmota.cmd("Restart 1")
  end

  def do_step_2(p, safeboot_url)
    import string
    if safeboot_url == nil || safeboot_url == ""
      safeboot_url = self.default_safeboot_URL()
      tasmota.log("UPL: no `safeboot` URL, defaulting to "+safeboot_url, 2)
    end

    var step2_state = self.test_step_2(p)
    if step2_state == true return true end
    if type(step2_state) == 'string)'   raise "internal_error", step2_state end
    if safeboot_url == nil || size(safeboot_url) == 0  raise "internal_error", "wrong safeboot URL "+str(safeboot_url) end

    var cl = webclient()
    cl.begin(safeboot_url)
    var r = cl.GET()
    if r != 200    raise "network_error", "GET returned "+str(r) end
    var safeboot_size = cl.get_size()
    if safeboot_size <= 500000   raise "internal_error", "wrong safeboot size "+str(safeboot_size) end
    if safeboot_size > (self.app_size_min * 1024)  raise "internal_error", "safeboot is too large "+str(safeboot_size / 1024)+"kB" end
    tasmota.log(string.format("UPL: flashing `safeboot` from %s %ikB", safeboot_url, (safeboot_size / 1024) + 1), 2)
    var app0 = p.get_ota_slot(0)
    if app0.start != 0x10000    raise "internal_error", "`app0` offset is not 0x10000" end
    cl.write_flash(app0.start)
    cl.close()
    return true
  end


  def do_step_3(p)
    import flash

    var step3_state = self.test_step_3(p)
    if step3_state == true return true end
    if type(step3_state) == 'string'   raise "internal_error", step3_state end

    # remove any partition after last fs
    self.remove_partition_after_last_fs(p)

    var app0 = p.get_ota_slot(0)
    var app1 = p.get_ota_slot(1)
    if app0 == nil || app1 == nil       raise "internal_error", "there are no `app0` or `app1` partitions" end
    var factory_size = self.app_size_min * 1024

    do    # open new scope
      var firm0_size = app0.get_image_size()    # get the size of the partition holding safeboot and check values
      if firm0_size <= 0              raise "internal_error", "invalid size in app0 partition" end
      if firm0_size >= factory_size   raise "internal_error", "app0 partition is too big for factory" end
    end

    # remove any SPIFFS partition that is not at the end of the partition table
    var idx = 1
    while idx < size(p.slots) - 1   # skip explicitly the last partition
      if p.slots[idx].is_spiffs()
        p.slots.remove(idx)
        tasmota.log("UPL: removesd unused SPIFFS partition", 2)
      else
        idx += 1
      end
    end

    # do the change
    app0.subtype = 0        # factory subtype
    app0.sz = factory_size
    app0.label = 'safeboot'

    app1.subtype = 0x10     # app1 becomes app0
    app1.label = 'app0'
    var f1_start = app1.start
    app1.start = app0.start + factory_size
    app1.sz += f1_start - app1.start

    # swicth partitions
    p.set_active(0)
    p.save()

    p.switch_factory(true)
    tasmota.cmd("Restart 1")

    return true
  end


  # display the step state DONE/READY/ERROR with color and either step description or error message
  # arg
  #   state: true=DONE, false=READY, string=ERROR with message
  #   msg: description if DONE or READY
  # returns HTML string
  def display_step_state(state, msg)
    if state == true
      return "<span style='color:#0F0'>DONE</span> "+msg
    elif state == false
      return "<span style='color:#FA0'>READY</span> "+msg
    else
      return "<span style='color:#F00'>ERROR</span> "+str(state)
    end
  end
  #- ---------------------------------------------------------------------- -#
  #- Show page to migrate to factory layout + single OTA
  #- ---------------------------------------------------------------------- -#
  def show_migrate_to_factory(p)
    # display ota partitions
    import webserver
    import string

    if !self.factory_migrate_eligible(p)  return end

    webserver.content_send("<fieldset><legend><b>&nbsp;Migrate to safeboot partition layout&nbsp;</b></legend><p></p>")

    webserver.content_send("<p>The `safeboot` layout allows for increased size<br>of firmware or file-system.</p>")
    webserver.content_send("<p>Please see <a href='https://tasmota.github.io/docs/Safeboot/' target='_blank'>Safeboot layout documentation</a></p>")
    webserver.content_send("<p>&nbsp;</p>")

    webserver.content_send(string.format("<p>Step 1: %s</p>", self.display_step_state(self.test_step_1(p), "boot on `app1`")))
    webserver.content_send(string.format("<p>Step 2: %s</p>", self.display_step_state(self.test_step_2(p), "flash `safeboot` to `app0`")))
    webserver.content_send(string.format("<p>Step 3: %s</p>", self.display_step_state(self.test_step_3(p), "change partition map")))
    webserver.content_send(string.format("<p>Step 4: %s</p>", self.display_step_state(self.test_step_4(p), "flash final firmware")))

    webserver.content_send("<form action='/part_wiz' method='post' ")
    webserver.content_send("onsubmit='return confirm(\"This will causes multiple restarts.\");'>")
    var ota_url = tasmota.cmd("OtaUrl").find("OtaUrl", "")
    webserver.content_send(string.format("<br><b>OTA Url</b><br><input id='o1' placeholder='OTA_URL' value='%s'><br>",
                                         ota_url))

    import persist
    var safeboot_url = persist.find("safeboot_url", self.default_safeboot_URL())
    webserver.content_send(string.format("<br><b>SAFEBOOT Url</b> (don't change)<input id='o2' placeholder='SAFEBOOT_URL' value='%s'><br>",
                                         safeboot_url))

    webserver.content_send("<p></p><button name='factory' class='button bred'>Start migration</button></form></p>")
    
    webserver.content_send("<p></p></fieldset><p></p>")
  end

  #- ---------------------------------------------------------------------- -#
  #- Show each partition one after the other - only OTA and SPIFFS
  #- ---------------------------------------------------------------------- -#
  def show_current_partitions(p)
    # display ota partitions
    import webserver
    import string
    var cur_part = p.otadata.active_otadata     # -1=factory 0=ota_0 1=ota_1...

    webserver.content_send("<fieldset><legend><b>&nbsp;Current partitions&nbsp;</b></legend><p></p><table>")

    # don't show <sys> portion
    #webserver.content_send("<tr><td title='offset:0x000000 size:0x010000'>&lt;sys&gt;:&nbsp;</td><td align='right'> 64 KB</td></tr>")

    for slot:p.slots
      var is_ota = slot.is_ota()
      var is_factory = slot.is_factory()
      if (is_ota != nil) || is_factory     # display only partitions with app type
        var current_boot_partition = (is_ota == cur_part) || (is_factory && cur_part == -1)

        var usage_str = "unknown"
        var used = slot.get_image_size()
        if (used >= 0) && (used <= slot.sz)
          usage_str = string.format("used %i%%", ((used / 1024) * 100) / (slot.sz / 1024))
        end
        var title = string.format("%ssubtype:%s offset:0x%06X size:0x%06X", current_boot_partition ? "booted " : "", slot.subtype_to_string(), slot.start, slot.sz)
        var col_before = ""
        var col_after = ""
        if current_boot_partition
          col_before = "<span style='color:#0F0'>["
          col_after = "]</span>"
        end
        # webserver.content_send(string.format("<p><b>%s</b> [%s]: %i KB (%s)</p>", slot.label, slot.subtype_to_string(), slot.size / 1024, usage_str))
        webserver.content_send(string.format("<tr><td title='%s'><b>%s%s%s</b>:&nbsp;</td><td align='right'> %i KB </td><td>&nbsp;(%s)</td></tr>",
                                             title, col_before, slot.label, col_after, slot.sz / 1024, usage_str))
      elif slot.is_spiffs()
        # spiffs partition
        var title = string.format("subtype:%s offset:0x%06X size:0x%06X", slot.subtype_to_string(), slot.start, slot.sz)
        webserver.content_send(string.format("<tr><td title='%s'><b>fs</b>:&nbsp;</td><td align='right'> %i KB</td></tr>", title, slot.sz / 1024))
      end
    end

    var unallocated = self.get_unallocated_k(p)
    if unallocated > 0
      var last_slot = p.slots[-1]
      # verify that last slot is file-system
      var partition_end_k = (last_slot.start + last_slot.sz) / 1024   # last kb used for fs
      webserver.content_send(string.format("<tr><td title='offset:0x%06X size:0x%06X'>&lt;free&gt;:&nbsp;</td><td align='right'> %i KB</td></tr>",
                                            partition_end_k * 1024, unallocated * 1024, unallocated))
    end
    webserver.content_send("</table>")

    # display if layout is factory
    if self.has_factory_layout(p)
      webserver.content_send("<p>This device uses the <a href='https://tasmota.github.io/docs/Safeboot/' target='_blank'>Safeboot</a> layout</p>")
    end

    webserver.content_send("</fieldset><p></p>")

  end

  #######################################################################
  # Display the complete page
  #######################################################################
  def page_part_mgr()
    import webserver
    import partition_core
    if !webserver.check_privileged_access() return nil end
    var p = partition_core.Partition()                    # load partition layout
    self.patch_partition_core(p)

    webserver.content_start("Partition Wizard")           #- title of the web page -#
    webserver.content_send_style()                  #- send standard Tasmota styles -#

    if webserver.has_arg("wait")
      webserver.content_send("<p>Migration process will start in 5 seconds<br>Magic is happening, leave device alone for 3 minutes.</p>")
      webserver.content_button(webserver.BUTTON_MAIN) #- button back to main page -#
    else
      webserver.content_send("<p style='width:320px;'><b style='color:#f56'>Warning:</b> actions below can brick your device.</p>")
      self.show_current_partitions(p)
      self.show_resize_fs(p)
      # show page for migration to factory
      self.show_migrate_to_factory(p)
      webserver.content_button(webserver.BUTTON_MANAGEMENT) #- button back to management page -#
    end

    webserver.content_stop()                        #- end of web page -#
  end

  #######################################################################
  # returns true is LittleFS supports grow on mount
  #
  # True if version is above 13.1.0.3
  #######################################################################
  def support_grow_on_mount()
    import introspect
    if introspect.get(tasmota, "version") != nil
      if tasmota.version() >= 0x0D010003
        return true
      end
    end
    return false
  end

  #######################################################################
  # Web Controller, called by POST to `/part_wiz`
  #######################################################################
  def page_part_ctl()
    import webserver
    import string
    if !webserver.check_privileged_access() return nil end

    import partition_core
    import persist
    

    #- check that the partition is valid -#
    var p = partition_core.Partition()
    self.patch_partition_core(p)

    try
      
      #---------------------------------------------------------------------#
      # Resize FS to max
      #---------------------------------------------------------------------#
      if webserver.has_arg("max_fs")
        var unallocated = self.get_unallocated_k(p)
        if unallocated <= 0     raise "value_error", "FS already at max size" end

        self.resize_max_flash_size_k(p)   # resize if needed

        # since unallocated succeeded, we know the last slot is FS
        var fs_slot = p.slots[-1]
        fs_slot.sz += unallocated * 1024
        p.save()
        if !self.support_grow_on_mount()
          p.invalidate_spiffs()   # erase SPIFFS or data is corrupt
        end

        #- and force restart -#
        webserver.redirect("/?rst=")

      #---------------------------------------------------------------------#
      # Resize FS to arbitrary size
      #---------------------------------------------------------------------#
      elif webserver.has_arg("resize_fs")
        if !self.has_factory_layout(p)  raise "internal_error", "Device does not have safeboot layout" end

        var fs = p.slots[-1]
        var last_app = p.slots[-2]
        if (last_app.get_image_size() <= 0)  raise "internal_error", "last `app` partition has no firmware" end
      
        var max_fs_start_k = self.get_max_fs_start_k(p)
        var flash_size_k = self.get_max_flash_size_k(p)
        
        var fs_max_size_k = flash_size_k - max_fs_start_k
        var current_fs_size_k = self.get_cur_fs_size_k(p)

        var fs_target = int(webserver.arg("fs_size"))
        if (fs_target < 64) || (fs_target > fs_max_size_k) raise "value_error", string.format("Invalid FS #%d", fs_target) end

        # apply the change
        # shrink last OTA App
        var delta = (fs_target * 1024) - fs.sz
        last_app.sz -= delta

        # move fs
        fs.start -= delta
        fs.sz += delta
        p.save()
        p.invalidate_spiffs()

        #- and force restart -#
        webserver.redirect("/?rst=")
      #---------------------------------------------------------------------#
      # Switch OTA partition from one to another
      #---------------------------------------------------------------------#
      elif webserver.has_arg("factory")
        var ota_url = webserver.arg("o1")
        var safeboot_url = webserver.arg("o2")
        
        if safeboot_url != nil && safeboot_url != ""
          persist.safeboot_url = safeboot_url
          persist.save()
        end

        if ota_url != nil && ota_url != ""
          tasmota.cmd("OtaUrl "+ota_url)
        end

        tasmota.set_timer(5000, /-> self.do_safeboot_partitioning())
        webserver.redirect("/part_wiz?wait=")

      else
        raise "value_error", "Unknown command"
      end
    except .. as e, m
      tasmota.log(string.format("BRY: Exception> '%s' - %s", e, m), 2)
      #- display error page -#
      webserver.content_start("Parameter error")           #- title of the web page -#
      webserver.content_send_style()                  #- send standard Tasmota styles -#

      webserver.content_send(string.format("<p style='width:340px;'><b>Exception:</b><br>'%s'<br>%s</p>", e, m))
      # webserver.content_send("<p></p></fieldset><p></p>")

      webserver.content_button(webserver.BUTTON_MANAGEMENT) #- button back to management page -#
      webserver.content_stop()                        #- end of web page -#
    end
  end



  #---------------------------------------------------------------------#
  # Apply the repartitioning process
  #---------------------------------------------------------------------#
  # returns:
  #  `true`: already completed
  #  `false`: in progress
  #  string: error with description of error
  def do_safeboot_partitioning()
    import webserver
    import partition_core
    import string

    var p = partition_core.Partition()                    # load partition layout
    self.patch_partition_core(p)
    if !self.factory_migrate_eligible(p) return true end

    # STEP 1
    var step1_state = self.test_step_1(p)
    if type(step1_state) == 'string'  return step1_state end
    if step1_state == false
      tasmota.log("UPL: Starting step 1", 2)
      try
        self.do_step_1(p)
      except .. as e, m
        tasmota.log(string.format("UPL: error (%s) %s", e, m), 2)
        return m
      end
      return false
    end
    tasmota.log("UPL: Step 1 Done", 2)

    # STEP 2
    var step2_state = self.test_step_2(p)
    if type(step2_state) == 'string'  return step2_state end
    if step2_state == false
      tasmota.log("UPL: Starting step 2", 2)
      import persist
      var safeboot_url = persist.find("safeboot_url")
      try
        self.do_step_2(p, safeboot_url)
      except .. as e, m
        tasmota.log(string.format("UPL: error (%s) %s", e, m), 2)
        return m
      end
    end
    tasmota.log("UPL: Step 2 Done", 2)

    # STEP 3
    var step3_state = self.test_step_3(p)
    if type(step3_state) == 'string'  return step3_state end
    if step3_state == false
      tasmota.log("UPL: Starting step 3", 2)
      try
        self.do_step_3(p)
      except .. as e, m
        tasmota.log(string.format("UPL: error (%s) %s", e, m), 2)
        return m
      end
    end
    tasmota.log("UPL: Step 3 Done", 2)

    # STEP 4
    # Nothing to do, it's automatic
    return false
  end

  #- ---------------------------------------------------------------------- -#
  # respond to web_add_handler() event to register web listeners
  #- ---------------------------------------------------------------------- -#
  #- this is called at Tasmota start-up, as soon as Wifi/Eth is up and web server running -#
  def web_add_handler()
    import webserver
    #- we need to register a closure, not just a function, that captures the current instance -#
    webserver.on("/part_wiz", / -> self.page_part_mgr(), webserver.HTTP_GET)
    webserver.on("/part_wiz", / -> self.page_part_ctl(), webserver.HTTP_POST)
  end
end
partition_wizard.Partition_wizard_UI = Partition_wizard_UI


#- create and register driver in Tasmota -#
if tasmota
  import partition_core
  var partition_wizard_ui = partition_wizard.Partition_wizard_UI()
  tasmota.add_driver(partition_wizard_ui)
  ## can be removed if put in 'autoexec.bat'
  partition_wizard_ui.web_add_handler()
end

return partition_wizard

#- Example

import partition

# read
p = partition.Partition()
print(p)

-#
