# Decoder files are modeled on the *.js files found here:
#  https://github.com/TheThingsNetwork/lorawan-devices/tree/master/vendor

var LwRegions = ["EU868", "US915", "IN865","AU915","KZ865","RU864","AS923", "AS923-1","AS923-2","AS923-3"]
var LwDeco

import mqtt 

class lwdecode_cls
  var thisDevice
  var LwDecoders

  def init()
    self.thisDevice = tasmota.cmd('Status',true)['Status']['Topic']
    self.LwDecoders = {}

    if global.lwdecode_driver
      global.lwdecode_driver.stop() # Let previous instance bail out cleanly
    end
    tasmota.add_driver(global.lwdecode_driver := self)
    tasmota.add_rule("LwReceived", /value, trigger, payload -> self.LwDecode(payload))
  end

  def LwDecode(data)
    import json

    var deviceData = data['LwReceived']
    var deviceName = deviceData.keys()()
    var Node = deviceData[deviceName]['Node']
    var RSSI = deviceData[deviceName]['RSSI']
    var Payload = deviceData[deviceName]['Payload']
    var FPort = deviceData[deviceName]['FPort']
    var decoder = deviceData[deviceName].find('Decoder')
    if !decoder 
      return true
    end
    
    if !self.LwDecoders.find(decoder)
      LwDeco = nil
      load(decoder)  # Sets LwDeco if found
      if LwDeco
        self.LwDecoders.insert(decoder, LwDeco)
      end
    end 

    if Payload.size() && self.LwDecoders.find(decoder)
      var topic = "tele/" + self.thisDevice + "/SENSOR"
      var decoded = self.LwDecoders[decoder].decodeUplink(Node, RSSI, FPort, Payload)	
      var mqttData = {"LwDecoded":{deviceName:decoded}}
      mqtt.publish(topic, json.dump(mqttData))
      tasmota.global.restart_flag = 0 # Signal LwDecoded successful (default state)
    end 

    return true  #processed
  end

  def dhm(last_time)
    var since = tasmota.rtc('local') - last_time
    var unit = "d"
    if since > (24 * 3600)
      since /= (24 * 3600)
      if since > 99 since = 99 end
    elif  since > 3600
      since /= 3600
      unit = "h"
    else
      since /= 60
      unit = "m"
    end
    return format("%02d%s", since, unit)
  end

  def header(name, name_tooltip, battery, battery_last_seen, rssi, last_seen)
    var color_text = f'{tasmota.webcolor(0 #-COL_TEXT-#)}'    # '#eaeaea'
    var msg = "<tr class='ltd htr'>"                          # ==== Start first table row
    msg += format("<td><b title='%s'>%s</b></td>", name_tooltip, name)
    if battery < 1000
      # Battery low <= 2.5V (0%), high >= 3.1V (100%)
      var batt_percent = (battery * 1000) - 2500
      batt_percent /= 6                                       # 3.1V - 2.5V = 0.6V = 100%
      if batt_percent < 0 batt_percent = 0 end
      if batt_percent > 98 batt_percent = 98 end              # 98% / 14px = 7
      batt_percent /= 7                                       # 1..14px showing battery load
      msg += format("<td><i class=\"bt\" title=\"%.3fV (%s)\" style=\"--bl:%dpx;color:%s\"></i></td>",
                   battery, self.dhm(battery_last_seen), batt_percent, color_text)
    else
      msg += "<td>&nbsp;</td>"
    end
    if rssi < 1000
      if rssi < -132 rssi = -132 end
      var num_bars = 4 - ((rssi * -1) / 33)
      msg += format("<td><div title='RSSI %i' class='si'>", rssi)
      for j:0..3
        msg += format("<i class='b%d%s'></i>", j, (num_bars < j) ? " o30" : "")         # Bars
      end
      msg += "</div></td>"                                    # Close RSSI
    else
      msg += "<td>&nbsp;</td>"
    end
    msg += format("<td style='color:%s'>&#x1F557;%s</td>",    # Clock
                 color_text, self.dhm(last_seen))
    msg += "</tr>"                                            # ==== End first table row
    return msg
  end #sensor()

  #------------------------------------------------------------
  Display sensor value in the web UI and react to button
  Called every WebRefresh time
  ------------------------------------------------------------#
  def web_sensor()
    var msg = ""
    for decoder: self.LwDecoders
      msg += decoder.add_web_sensor()	
    end
    if msg
      var color_text = f'{tasmota.webcolor(0 #-COL_TEXT-#)}'  # '#eaeaea'
      var full_msg = format("</table>"                        # Terminate current two column table and open new table
        "<style>"
        # Table CSS
        ".ltd td:not(:first-child){width:20px;font-size:70%%}"
        ".ltd td:last-child{width:45px}"
        ".ltd .bt{margin-right:10px;}"                        # Margin right should be half of the not-first width
        ".htr{line-height:20px}"
        # Signal Strength Indicator
        ".si{display:inline-flex;align-items:flex-end;height:15px;padding:0}"
        ".si i{width:3px;margin-right:1px;border-radius:3px;background-color:%s}" # WebColor(COL_TEXT)
        ".si .b0{height:25%%}.si .b1{height:50%%}.si .b2{height:75%%}.si .b3{height:100%%}.o30{opacity:.3}"
        "</style>"
        "{t}",                                                # Open new table
        color_text)
      full_msg += msg
      full_msg += "</table>{t}"                               # Close table and open new table

      tasmota.web_send_decimal(full_msg)
    end
  end
end

lwdecode = lwdecode_cls()

import webserver
class webPageLoRaWAN : Driver
  def sendNodeButton(node)
     webserver.content_send("<td><button onclick='selNode("+str(node)+")' id='n"+str(node)+"' class='button inactive' style='width:30px'>"+str(node)+"</button>")
  end

  def web_add_config_button()
    webserver.content_send("<p><form id=ac action='lrw' style='display: block;' method='get'><button>LoRaWAN</button></form></p>")
  end

  #- this method displays the web page -#
  def pageLoRaWAN()
    if !webserver.check_privileged_access() return nil end

    var inode=1
    var cmdArg  
    if webserver.has_arg('save')
     inode = webserver.arg('node')
     tasmota.cmd('LoRaWanAppKey'+inode+' '+ webserver.arg('ak'),true)
     cmdArg = webserver.arg('dc')
     if !cmdArg cmdArg='"' end
     tasmota.cmd('LoRaWanDecoder'+inode+' '+cmdArg,true)
     cmdArg = webserver.arg('an')
     if !cmdArg cmdArg='"' end
     tasmota.cmd('LoRaWanName'+inode+' '+cmdArg,true)
    end

    webserver.content_start("LoRaWAN")           #- title of the web page -#
    webserver.content_send_style()               #- send standard Tasmota styles -#
    webserver.content_send(
     "<style>.inactive{background:#10537c;}.active{background:#1fa3ec;}</style>"
     "<h2>LoRaWAN End Devices</h2>"
     "<script>"
     "function selNode(n){"
     " var i;var d=4;"
     " var e=document.getElementById('n'+n);"
     " var o=document.getElementsByClassName('button active');"
     " if(o.length){"
     "  for(i=0;i<o.length;i++){"
     "  o[i].classList.add('inactive');"
     "  o[i].classList.remove('active');"
     "  }"
     " }"
     " e.classList.add('active');"
     " for(i=1;i<=16;i++){"
     "  document.getElementById('nd'+i).style.display=(i==n)?'block':'none';"
     " }"
     "}"
     "window.onload = function(){selNode("+str(inode)+");};"
     "</script>")

    var arg, appKey, decoder, name
    var hintAK='32 character Application Key'
    var hintDecoder='Decoder file, ending in .be'
    var hintAN='Device name for MQTT messages'
    webserver.content_send("<table><tr>")
    for node:1..8
        self.sendNodeButton(node)
    end
    webserver.content_send("<tr>")
    for node:9..16
        self.sendNodeButton(node)
    end
    webserver.content_send("</table>")
    for node:1..16
     arg='LoRaWanAppKey' + str(node)
     appKey=tasmota.cmd(arg,true).find(arg)
     arg='LoRaWanName' + str(node)
     name=tasmota.cmd(arg,true).find(arg)
     arg='LoRaWanDecoder' + str(node)
     decoder=tasmota.cmd(arg,true).find(arg)
     webserver.content_send(
      "<div id='nd"+str(node)+"' style='display:none'>"
      "<fieldset>"
      "<legend><b>&nbsp;End Device "+str(node)+"&nbsp;</b></legend>"
      "<form action='' method='post'>"
      "<p>Application Key"
      "<input title='"+hintAK+"' pattern='[A-Fa-f0-9]{32}' id='ak' minlength='32' maxlength='32' required='' placeholder='"+hintAK+"' value='"+appKey+ "' name='ak' style='font-size:smaller'>"
      "</p>"
      "<p></p>"
      "<p>Device Name"
      "<input id='an' placeholder='"+hintAN+"' value='"+name+ "' name='an'>"
      "</p>"
      "<p></p>"
      "<p>Decoder File"
      "<input title='"+hintDecoder+"' id='dc'  placeholder='"+hintDecoder+"' value='"+decoder+"' name='dc'>"
      "</p>"
      "<p></p>"
      "<button name='save' class='button bgrn'>Save</button>"
      "<input type='hidden' name='node' value='"+str(node)+"'>"
      "</form>"
      "</fieldset>"
      "</div>")
    end

    webserver.content_button(webserver.BUTTON_CONFIGURATION) #- button back to conf page -#
    webserver.content_stop()                        #- end of web page -#
  end

  #- this is called at Tasmota start-up, as soon as Wifi/Eth is up and web server running -#
  def web_add_handler()
    #- we need to register a closure, not just a function, that captures the current instance -#
    webserver.on("/lrw", / -> self.pageLoRaWAN())
  end
end

#- create and register driver in Tasmota -#
webPageLoRaWAN_instance = webPageLoRaWAN()
tasmota.add_driver(webPageLoRaWAN_instance)

tasmota.cmd('LoraOption3 off')    # Disable embedded decoding
tasmota.cmd('SetOption100 off')   # Keep LwReceived in JSON message
tasmota.cmd('SetOption118 off')   # Keep SENSOR as subtopic name
tasmota.cmd('SetOption119 off')   # Keep device address in JSON message
#tasmota.cmd('SetOption147 on')    # Hide LwReceived MQTT message but keep rule processing
tasmota.cmd('LoRaWanBridge on')
