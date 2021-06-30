#- Native code used for testing and code solidification -#
#- Do not use it -#

class Tasmota2 : Tasmota

  # add `chars_in_string(s:string,c:string) -> int``
  # looks for any char in c, and return the position of the first char
  # or -1 if not found
  # inv is optional and inverses the behavior, i.e. look for chars not in the list
  def chars_in_string(s,c,inv)
    var inverted = inv ? true : false
    for i:0..size(s)-1
      var found = false
      for j:0..size(c)-1
        if s[i] == c[j] found = true end
      end
      if inverted != found return i end
    end
    return -1
  end

  # find a key in map, case insensitive, return actual key or nil if not found
  def find_key_i(m,keyi)
    import string
    var keyu = string.toupper(keyi)
    if classof(m) == map
      for k:m.keys()
        if string.toupper(k)==keyu || keyi=='?'
          return k
        end
      end
    end
  end


  # split the item when there is an operator, returns a list of (left,op,right)
  # ex: "Dimmer>50" -> ["Dimmer",tasmota_gt,"50"]
  def find_op(item)
    import string
    var op_chars = '=<>!'
    var pos = self.chars_in_string(item, op_chars)
    if pos >= 0
      var op_split = string.split(item,pos)
      var op_left = op_split[0]
      var op_rest = op_split[1]
      pos = self.chars_in_string(op_rest, op_chars, true)
      if pos >= 0
        var op_split2 = string.split(op_rest,pos)
        var op_middle = op_split2[0]
        var op_right = op_split2[1]
        return [op_left,op_middle,op_right]
      end
    end
    return [item, nil, nil]
  end

  # Rules
  def add_rule(pat,f)
    if !self._rules
      self._rules={}
    end
    if type(f) == 'function'
      self._rules[pat] = f
    else
      raise 'value_error', 'the second argument is not a function'
    end
  end

  def remove_rule(pat)
    if self._rules
      self._rules.remove(pat)
    end
  end

  # Rules trigger if match. return true if match, false if not
  def try_rule(event, rule, f)
    import string
    var rl_list = self.find_op(rule)
    var sub_event = event
    var rl = string.split(rl_list[0],'#')
    for it:rl
      found=self.find_key_i(sub_event,it)
      if found == nil return false end
      sub_event = sub_event[found]
    end
    var op=rl_list[1]
    var op2=rl_list[2]
    if op
      if   op=='=='
        if str(sub_event) != str(op2)   return false end
      elif op=='!=='
        if str(sub_event) == str(op2)   return false end
      elif op=='='
        if real(sub_event) != real(op2) return false end
      elif op=='!='
        if real(sub_event) == real(op2) return false end
      elif op=='>'
        if real(sub_event) <= real(op2) return false end
      elif op=='>='
        if real(sub_event) < real(op2)  return false end
      elif op=='<'
        if real(sub_event) >= real(op2) return false end
      elif op=='<='
        if real(sub_event) > real(op2)  return false end
      end
    end
    f(sub_event, rl_list[0], event)
    return true
  end

  # Run rules, i.e. check each individual rule
  # Returns true if at least one rule matched, false if none
  def exec_rules(ev_json)
    if self._rules
      import json
      var ev = json.load(ev_json)
      var ret = false
      if ev == nil
        print('BRY: ERROR, bad json: '+ev_json, 3)
      else
        for r: self._rules.keys()
          ret = self.try_rule(ev,r,self._rules[r]) || ret
        end
      end
      return ret
    end
    return false
  end

  def set_timer(delay,f)
    if !self._timers self._timers=[] end
    self._timers.push([self.millis(delay),f])
  end

  # run every 50ms tick
  def run_deferred()
    if self._timers
      var i=0
      while i<self._timers.size()
        if self.time_reached(self._timers[i][0])
          f=self._timers[i][1]
          self._timers.remove(i)
          f()
        else
          i=i+1
        end
      end
    end
  end

  # Add command to list
  def add_cmd(c,f)
    if !self._ccmd
      self._ccmd={}
    end
    if type(f) == 'function'
      self._ccmd[c]=f
    else
      raise 'value_error', 'the second argument is not a function'
    end
  end

  # Remove command from list
  def remove_cmd(c)
    if self._ccmd
      self._ccmd.remove(c)
    end
  end

  # Execute custom command
  def exec_cmd(cmd, idx, payload)
    if self._ccmd
      import json
      var payload_json = json.load(payload)
      var cmd_found = self.find_key_i(self._ccmd, cmd)
      if cmd_found != nil
        self.resolvecmnd(cmd_found)   # set the command name in XdrvMailbox.command
        self._ccmd[cmd_found](cmd_found, idx, payload, payload_json)
        return true
      end
    end
    return false
  end

  # Force gc and return allocated memory
  def gc()
    import gc
    gc.collect()
    return gc.allocated()
  end

  # tasmota.wire_scan(addr:int [, index:int]) -> wire1 or wire2 or nil
  # scan for the first occurrence of the addr, starting with bus1 then bus2
  # optional: skip if index is disabled via I2CEnable
  def wire_scan(addr,idx)
    # skip if the I2C index is disabled
    if idx != nil && !self.i2c_enabled(idx) return nil end
    if self.wire1.detect(addr) return self.wire1 end
    if self.wire2.detect(addr) return self.wire2 end
    return nil
  end

  def time_str(time)
    import string
    var tm = self.time_dump(time)
    return string.format("%04d-%02d-%02dT%02d:%02d:%02d", tm['year'], tm['month'], tm['day'], tm['hour'], tm['min'], tm['sec'])
  end

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
          if   type=='every_second' && d.every_second                           d.every_second()
          elif type=='every_100ms' && d.every_100ms                             d.every_100ms()
          elif type=='web_add_button' && d.web_add_button                       d.web_add_button()
          elif type=='web_add_main_button' && d.web_add_main_button             d.web_add_main_button()
          elif type=='web_add_management_button' && d.web_add_management_button d.web_add_management_button()
          elif type=='web_add_config_button' && d.web_add_config_button         d.web_add_config_button()
          elif type=='web_add_console_button' && d.web_add_console_button       d.web_add_console_button()
          elif type=='save_before_restart' && d.save_before_restart             d.save_before_restart()
          elif type=='web_add_handler' && d.web_add_handler                     d.web_add_handler()
          elif type=='web_sensor' && d.web_sensor                               d.web_sensor()
          elif type=='json_append' && d.json_append                             d.json_append()
          elif type=='button_pressed' && d.button_pressed                       d.button_pressed()
          elif type=='web_add_handler' && d.display                             d.display()
          elif type=='display' && d.display                                     d.display()
          end
        except .. as e,m
          import string
          print(string.format("BRY: Exception> '%s' - %s", e, m))
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