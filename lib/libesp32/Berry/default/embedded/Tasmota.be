#- Native code used for testing and code solidification -#
#- Do not use it -#

class Tasmota2 : Tasmota

  def load(f)
    import string

    # check that the file ends with '.be' of '.bec'
    var fl = string.split(f,'.')
    if (size(fl) <= 1 || (fl[-1] != 'be' && fl[-1] != 'bec'))
      raise "io_error", "file extension is not '.be' or '.bec'"
    end
    var native = f[size(f)-1] == 'c'
    # load - works the same for .be and .bec

    # try if file exists
    try
      var ff = open(f, 'r')
      ff.close()
    except 'io_error'
      return false    # signals that file does not exist
    end

    var c = compile(f,'file')
    # save the compiled bytecode
    if !native
      try
        self.save(f+'c', c)
      except .. as e
        self.log(string.format('BRY: could not save compiled file %s (%s)',f+'c',e))
      end
    end
    # call the compiled code
    c()
    # call successfuls
    return true

  end

  def event(type, cmd, idx, payload)
    if type=='cmd' return self.exec_cmd(cmd, idx, payload)
    elif type=='rule' return self.exec_rules(payload)
    elif type=='mqtt_data' return nil
    elif type=='gc' return self.gc()
    elif type=='every_50ms' return self.run_deferred()
    elif self._drivers
      for d:self._drivers
        try
          if   type=='every_second' && d.every_second                 d.every_second()
          elif type=='every_100ms' && d.every_100ms                   d.every_100ms()
          elif type=='web_add_button' && d.web_add_button             d.web_add_button()
          elif type=='web_add_main_button' && d.web_add_main_button   d.web_add_main_button()
          elif type=='save_before_restart' && d.save_before_restart   d.save_before_restart()
          elif type=='web_add_handler' && d.web_add_handler           d.web_add_handler()
          elif type=='web_sensor' && d.web_sensor                     d.web_sensor()
          elif type=='json_append' && d.json_append                   d.json_append()
          elif type=='button_pressed' && d.button_pressed             d.button_pressed()
          elif type=='web_add_handler' && d.display                   d.display()
          elif type=='display' && d.display                           d.display()
          end
        except .. as e,m
          import string
          self.log(string.format('BRY: exception %s - %m',3))
        end
      end
    end
  end

  #- dispatch callback number n, with parameters v0,v1,v2,v3 -#
  def cb_dispatch(n,v0,v1,v2,v3)
    if self._cb == nil return 0 end
    var f = self._cb.find(n)
    if f != nil
      return f(v0,v1,v2,v3)
    end
    return 0
  end

  #- generate a new C callback and record the associated Berry closure -#
  def gen_cb(f)
    if self._cb == nil self._cb = {} end  # create map if not already initialized
    for i:0..19
      if self._cb.find(i) == nil
        #- free slot -#
        self._cb[i] = f
        return self._get_cb(i)
      end
    end
    raise "internal_error", "No callback available"
  end

end
tasmota = Tasmota2()