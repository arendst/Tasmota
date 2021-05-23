#- Native code used for testing and code solidification -#
#- Do not use it -#

class Tasmota
  def event(type, cmd, idx, payload)
    if type=='cmd' return self.exec_cmd(cmd, idx, payload)
    elif type=='rule' return self.exec_rules(payload)
    elif type=='mqtt_data' return nil
    elif type=='gc' return self.gc()
    elif type=='every_50ms' return self.run_deferred()
    elif self._drivers
      for d:self._drivers
        try
          if   type=='every_second' && d.every_second return d.every_second()
          elif type=='every_100ms' && d.every_100ms return d.every_100ms()
          elif type=='web_add_button' && d.web_add_button return d.web_add_button()
          elif type=='web_add_main_button' && d.web_add_main_button return d.web_add_main_button()
          elif type=='save_before_restart' && d.save_before_restart return d.save_before_restart()
          elif type=='web_sensor' && d.web_sensor return d.web_sensor()
          elif type=='json_append' && d.json_append return d.json_append()
          elif type=='button_pressed' && d.button_pressed return d.button_pressed()
          elif type=='display' && d.display return d.display()
          end
        except .. as e,m
          import string
          self.log(string.format('BRY: exception %s - %m',3))
        end
      end
    end
  end
end
