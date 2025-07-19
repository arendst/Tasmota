#-
heatfan.be - HeatFan support for Tasmota

SPDX-FileCopyrightText: 2025 Theo Arends

SPDX-License-Identifier: GPL-3.0-only
-#

#--------------------------------------------------------------
Version v1.2.8

ESP32-C2 based HeatFan WiFi Controller using Tasmota and Berry local CH Fan control
https://heatfan.eu/product/wifi-controller/

Template {"NAME":"HeatFan","GPIO":[4864,229,228,4737,4738,224,0,225,32,448,226,0,0,0,0,0,0,0,227,0,0],"FLAG":0,"BASE":1}
Module 0              # Enable above template
SO15 0                # Disable light and enable PWM control
GroupTopic2 heatfans  # Allow MQTT group topic control using buttons "Send Mode" and "Next Mode"

Relay1 to Relay6 control Leds for Modes 1 to 6 (interlocked)

GPIO00 = ADC1 range (temperature input from comparator)
GPIO01 = Relay6 (blue)
GPIO02 = Relay5 (red)
GPIO03 = ADC2 temperature (ADC_CH_TEMP)
GPIO04 = ADC3 temperature (ADC_CH_TMCU)
GPIO05 = Relay1 (red)
GPIO06 = gpio.digital_write (GPIO_FANS_EN)
GPIO07 = Relay2 (red)
GPIO08 = Button1
GPIO09 = PWM_i1 (20kHz fan control)
GPIO10 = Relay3 (red)
GPIO18 = Relay4 (red)
--------------------------------------------------------------#

import persist
import string
import gpio
import webserver

class heatfan_cls

  #------------------------------------------------------------
  User specific parameters
  Send temperature and speed to Home Automation
  ------------------------------------------------------------#
  static domoticz = {
    # hostname, temperature, speed
    "heatfan1":[3149, 3150],  # Domoticz Idx for temperature (3149) and speed (custom sensor) (3150)
    "heatfan2":[3151, 3152],
    "heatfan3":[3153, 3154]
  }

  def SendToHa()
    if self.domoticz.contains(self.hostname)
      tasmota.cmd(f"_DzSend1 {self.domoticz[self.hostname][0]},{self.temp_ext}")
      tasmota.cmd(f"_DzSend1 {self.domoticz[self.hostname][1]},{self.speed}")
    else
      tasmota.log(f"HFN: Domoticz on '{self.hostname}' not configured", 3)
    end
  end

  #------------------------------------------------------------
  Mode specific parameters
  ------------------------------------------------------------#
  static modes = {
    # start_temp, max_temp, start_speed, max_speed, led_pin
    1: [25, 100, 1, 5, 5],    # Speed starts at 1 from 25°C and caps at 5 from 100°C
    2: [25, 100, 1, 7, 7],    # Speed starts at 1 from 25°C and caps at 7 from 100°C
    3: [25, 70, 1, 8, 10],    # Speed starts at 1 from 25°C and caps at 8 from 70°C
    4: [25, 55, 1, 9, 18],    # Speed starts at 1 from 25°C and caps at 9 from 55°C
    5: [25, 40, 1, 10, 2],    # Speed starts at 1 from 25°C and caps at 10 from 40°C
    6: [10, 100, 10, 10, 1]   # Speed starts at 10 from 10°C and caps at 10 from 100°C
  }

  #-----------------------------------------------------------#

  static power_states = [ 0, 1, 10 ]    # Relay power Off, On, Duty 10

  # bool
  var teleperiod
  # int
  var mode, mode_persist, mode_states, speed, pwm_fan_change
  # float
  var temp_ext, last_temp_ext, dz_temp_ext
  # string
  var version, hostname

  #-----------------------------------------------------------#

  def persist_load()
    self.mode = 0                                # Default restart mode (Off)
    if persist.has("hf_mode")
      self.mode = persist.find("hf_mode")
    end
    self.mode_persist = self.mode
  end

  def persist_update()
    if self.mode_persist != self.mode
      persist.setmember("hf_mode", self.mode)
      persist.save(true)
      self.mode_persist = self.mode
    end
  end

  #-----------------------------------------------------------#

  def init()
    self.version = "1.2.8"
    self.persist_load()
    self.teleperiod = 0
    self.mode_states = 6                         # Number of modes (needs var as static doesn't work)
    self.speed = 0
    self.temp_ext = 0
    self.last_temp_ext = 0
    self.dz_temp_ext = 0
    self.pwm_fan_change = -1
    self.hostname = tasmota.cmd('Hostname')['Hostname']

    tasmota.log(f"HFN: HeatFan {self.version} started on {self.hostname}", 2)

    if global.heatfan_driver
      global.heatfan_driver.stop() # Let previous instance bail out cleanly
    end
    tasmota.add_driver(global.heatfan_driver := self)
    tasmota.add_rule("Analog#Range1", / value -> self.rule_range(value))
  end

  def stop()
    tasmota.remove_driver(self)
    tasmota.remove_rule("Analog#Range1")
  end

  #-----------------------------------------------------------#

  def set_pin(pin, state)
    if 2 == state
      gpio.set_pwm(pin, self.power_states[state])  # Enable PWM and set low duty
    else
      gpio.pin_mode(pin, gpio.OUTPUT)            # Disable PWM and set to Output
      gpio.digital_write(pin, self.power_states[state])  # Off / On
    end  
  end

  def set_power_handler(cmd, idx)
    var ps = tasmota.get_power()
    self.mode = 0
    for i:1..self.mode_states                    # Power1 to Power6
      self.set_pin(self.modes[i][4], 0)          # Turn all leds Off
      if ps[i -1] == true
        self.mode = i
      end
    end
    if self.mode 
      self.set_pin(self.modes[self.mode][4], 2)  # Turn active mode led On
    end
    self.persist_update()
  end

  def any_key(cmd, idx)
    # idx = device_save << 24 | key << 16 | state << 8 | device
    var state = (idx >> 8) & 0xff
    if state == 10                               # SINGLE
      # Rotate power from off,1,2,3,4,5,6,off...
      if self.mode == self.mode_states
        tasmota.set_power(self.mode_states -1, false)  # power offset from 0
      else
        tasmota.set_power(self.mode, true)       # power offset from 0
      end
    end
  end

  #------------------------------------------------------------
  Update fan speed based on temperature
  Called almost every second by GetNextSensor() or at Teleperiod time
  ------------------------------------------------------------#
  def rule_range(value)
    self.temp_ext = value

    if self.mode > 0
      var start_temp = self.modes[self.mode][0]
      var max_temp = self.modes[self.mode][1]
      var start_speed = self.modes[self.mode][2]
      var max_speed = self.modes[self.mode][3]

      var speed = max_speed
      var speeds = max_speed - start_speed       # speeds = 0 .. 9
      if speeds > 0
        var temp_range = max_temp - start_temp   # temp_range = 0 .. 99
        if temp_range > 0
          var temp_step = temp_range / speeds
          speed = int((self.temp_ext - start_temp + temp_step) / temp_step)  # 0 .. 10
          if speed < 0 speed = 0 end
          if speed < self.speed
            var speed_dn = int((self.temp_ext - start_temp + temp_step + 0.6) / temp_step)  # 0 .. 10
            if speed_dn > speed
              speed = speed_dn
            end
          end    
          if speed > max_speed speed = max_speed end
        end
      end
      self.speed = speed
    else
      self.speed = 0
    end

    var pwm_fan = 0
    if self.speed > 0
      pwm_fan = 24 + (self.speed * 6)             # 30% .. 84%
    end

#    tasmota.log(f"HFN: Range1 {self.temp_ext}, Mode {self.mode}, Speeds {speeds}, Step {temp_step}, Speed {self.speed}, PwmFan {pwm_fan}", 3)

    if pwm_fan != self.pwm_fan_change
      if pwm_fan > 0
        var duty = (1023 * pwm_fan) / 100
        gpio.set_pwm(9, duty)                    # Set fan speed
        self.set_pin(6, 1)                       # GPIO_FANS_EN
      else
        self.set_pin(6, 0)                       # GPIO_FANS_EN
      end
      self.pwm_fan_change = pwm_fan
    end

    if self.teleperiod == 1                      # Needs to be executed here as tasmota.cmd destroys Response
      self.SendToHa()
      self.teleperiod = 0
    end

  end

  #------------------------------------------------------------
  Add sensor value to teleperiod
  Called almost every second by GetNextSensor() or at Teleperiod time
  ------------------------------------------------------------#
  def json_append()
    var msg = string.format(",\"HeatFan\":{\"Mode\":%i,\"Speed\":%i,\"Temperature\":%.2f}",
                              self.mode, self.speed, self.temp_ext)
    tasmota.response_append(msg)

    if tasmota.global.tele_period == 0
      self.teleperiod = 1;
    end

  end

  #------------------------------------------------------------
  Display sensor value in the web UI and react to button
  Called every WebRefresh time
  ------------------------------------------------------------#
  def web_sensor()
    var max_speed = 0
    if self.mode > 0
      max_speed = self.modes[self.mode][3]
    end
    var msg = string.format("{s}Radiator Temperature{m}%.2f °C{e}"..
                            "{s}Fan Speed{m}%i / %i{e}",
                            self.temp_ext,
                            self.speed, max_speed)
    tasmota.web_send_decimal(msg)

    if webserver.has_arg("m_grp_send")
      # Send current mode to group topic
      if 0 == self.mode
        tasmota.cmd("Publish cmnd/heatfans/Power0 0")
      else
        tasmota.cmd(f"Publish cmnd/heatfans/Power{self.mode} 1")
      end
    elif webserver.has_arg("m_grp_next")
      # Rotate group topic mode from off,1,2,3,4,5,6,off...
      if self.mode == self.mode_states
        tasmota.cmd(f"Publish cmnd/heatfans/Power{self.mode_states} 0")
      else
        tasmota.cmd(f"Publish cmnd/heatfans/Power{self.mode +1} 1")
      end
    end
  end

  def web_add_main_button()
    webserver.content_send("<table style=\"width:100%\"><tr>")
    webserver.content_send("<td style=\"width:50%\"><button onclick='la(\"&m_grp_send=1\");'>Send Mode</button></td>")
    webserver.content_send("<td style=\"width:50%\"><button onclick='la(\"&m_grp_next=1\");'>Next Mode</button></td>")
    webserver.content_send("</tr></table>")
    webserver.content_send(f"<div style='text-align:right;font-size:11px;color:#aaa;'>HeatFan {self.version}</div>")
  end

end

heatfan = heatfan_cls()

tasmota.cmd("Interlock 1,2,3,4,5,6") # Interlock modes 1 to 6
tasmota.cmd("Interlock 1")           # Enable interlock
var mode = heatfan.mode              # Save persitent mode
tasmota.set_power(0, true)           # Set mode 1 to reset interlock on restart
# any_key() support cycling power from off,1,2,3,4,5,6,off... 
tasmota.cmd("SO1 1")                 # (Button) Control button single press (1)
tasmota.cmd("SO13 1")                # (Button) Support only single press (1) 
tasmota.cmd("SO73 1")                # (Button) Detach buttons from relays (1)
tasmota.cmd("SO146 1")               # (ESP32) Show ESP32 internal temperature sensor
tasmota.cmd("SO161 1")               # (GUI) Disable display of state text (1)
tasmota.cmd("PwmRange 1023")         # Fan PWM duty cycle
tasmota.cmd("PwmFrequency 20000")    # Fan PWM frequency to 20kHz
tasmota.cmd("webtime 11,19")         # Enable GUI time HH:MM:SS
tasmota.cmd("websensor2 0")          # Disable display of ADC information in GUI
tasmota.cmd("webrefresh 1000")       # Update GUI every second
tasmota.cmd("Tempres 2")             # ADC2/3 temperature resolution
tasmota.cmd("Freqres 2")             # ADC1 Range resolution
tasmota.cmd("Teleperiod 60")         # Update HA every minute
tasmota.cmd("AdcGpio0 400,1710,60,20") # ADC1 range

if 0 == mode
  tasmota.set_power(0, false)        # Set mode 0
else
  tasmota.set_power(mode -1, true)   # Set stored mode
end
