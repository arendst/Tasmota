#-------------------------------------------------------------
 - Parser manager for ESP32
 -
 -------------------------------------------------------------#

import flash
import string
import partition
import webserver

class Partition_manager : Driver

  def init()
  end

  # create a method for adding a button to the main menu
  # the button 'Partition Manager' redirects to '/part_mgr?'
  def web_add_button()
    webserver.content_send(
      "<form id=but_part_mgr style='display: block;' action='part_mgr' method='get'><button>Partition Manager</button></form>")
  end

  #
  # Show a single OTA Partition
  #
  def page_show_partition(slot, active)
    webserver.content_send("<fieldset><legend><b>&nbsp;" + slot.label)
    if active
      webserver.content_send(" (active)")
    end
    webserver.content_send("</b></legend>")

    webserver.content_send(string.format("<p><b>Start: </b>0x<b>%03x</b> 000</p>", slot.start / 0x1000))
    webserver.content_send(string.format("<p><b>Size: </b>%i KB</p>", slot.size / 1024))
    
    webserver.content_send("<p></p></fieldset><p></p>")
  end


  #
  # Show a single OTA Partition
  #
  def page_show_spiffs(slot, free_mem)
    webserver.content_send("<fieldset><legend><b>&nbsp;" + slot.label)
    webserver.content_send("</b></legend>")

    webserver.content_send(string.format("<p><b>Start: </b>0x<b>%03x</b> 000</p>", slot.start / 0x1000))
    webserver.content_send(string.format("<p><b>Size: </b>%i KB</p>", slot.size / 1024))
    if free_mem != nil
      webserver.content_send(string.format("<p><b>Unallocated: </b>%i KB</p>", free_mem / 1024))
    end
    
    webserver.content_send("<p></p></fieldset><p></p>")
  end

  def page_show_partitions()
    var p = partition.Partition()

    # display ota partitions
    for slot: p.slots
      # is the slot app?
      var ota_num = slot.is_ota()
      if ota_num != nil
        # we have an OTA partition
        self.page_show_partition(slot, ota_num == p.otadata.active_otadata)
      elif slot.type == 1 && slot.subtype == 130
        var flash_size = tasmota.memory()['flash'] * 1024
        var used_size = (slot.start + slot.size)
        self.page_show_spiffs(slot, slot == p.slots[-1] ? flash_size - used_size : nil)
      end
    end
  end

  #- this method displays the web page -#
  def page_part_mgr()
    if !webserver.check_privileged_access() return nil end

    webserver.content_start("Partition Manager")           #- title of the web page -#
    webserver.content_send_style()                  #- send standard Tasmota styles -#

    webserver.content_send("<p><b>Esp32 Partition Manager</b></p><p></p>")   #- send any html -#
    self.page_show_partitions()

    webserver.content_button(webserver.BUTTON_MANAGEMENT) #- button back to management page -#
    webserver.content_stop()                        #- end of web page -#
  end

  #- this is called at Tasmota start-up, as soon as Wifi/Eth is up and web server running -#
  def web_add_handler()
    #- we need to register a closure, not just a function, that captures the current instance -#
    webserver.on("/part_mgr", / -> self.page_part_mgr())
  end

end

#- create and register driver in Tasmota -#
partition_manager = Partition_manager()
tasmota.add_driver(partition_manager)
## to be removed if put in 'autoexec.bat'
partition_manager.web_add_handler()