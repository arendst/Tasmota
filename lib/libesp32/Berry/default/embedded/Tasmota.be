#- Native code used for testing and code solidification -#
#- Do not use it -#

class Timer
  var due, f, id
  def init(due, f, id)
    self.due = due
    self.f = f
    self.id = id
  end
  def tostring()
    import string
    return string.format("<instance: %s(%s, %s, %s)", str(classof(self)),
              str(self.due), str(self.f), str(self.id))
  end
end

tasmota = nil
class Tasmota
  var global      # mapping to TasmotaGlobal

  def init()
    # instanciate the mapping object to TasmotaGlobal
    self.global = ctypes_bytes_dyn(self._global_addr, self._global_def)
  end

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
      var found=self.find_key_i(sub_event,it)
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

  def set_timer(delay,f,id)
    if !self._timers self._timers=[] end
    self._timers.push(Timer(self.millis(delay),f,id))
  end

  # run every 50ms tick
  def run_deferred()
    if self._timers
      var i=0
      while i<self._timers.size()
        if self.time_reached(self._timers[i].due)
          var f=self._timers[i].f
          self._timers.remove(i)
          f()
        else
          i=i+1
        end
      end
    end
  end

  # remove timers by id
  def remove_timer(id)
    if tasmota._timers
      var i=0
      while i<tasmota._timers.size()
        if self._timers[i].id == id
          self._timers.remove(i)
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
    import path

    # if the filename has no '.' append '.be'
    if string.find(f, '.') < 0
      f += ".be"
    end

    # check that the file ends with '.be' of '.bec'
    var fl = string.split(f,'.')
    if (size(fl) <= 1 || (fl[-1] != 'be' && fl[-1] != 'bec'))
      raise "io_error", "file extension is not '.be' or '.bec'"
    end

    var is_bytecode = f[-1] == 'c'            # file is Berry source and not bytecode
    var f_time = path.last_modified(f)

    if is_bytecode
      if f_time == nil  return false end      # file does not exist
      # f is the right file, continue
    else
      var f_time_bc = path.last_modified(f + "c") # timestamp for bytecode
      if f_time == nil && f_time_bc == nil  return false end
      if f_time_bc != nil && (f_time == nil || f_time_bc >= f_time)
        # bytecode exists and is more recent than berry source, use bytecode
        f = f + "c"   # use bytecode name
        is_bytecode = true
      end
    end
    
    var c = compile(f, 'file')
    # save the compiled bytecode
    if !is_bytecode
      try
        self.save(f + 'c', c)
      except .. as e
        print(string.format('BRY: could not save compiled file %s (%s)',f+'c',e))
      end
    end
    # call the compiled code
    c()
    # call successfuls
    return true
  end

  def event(event_type, cmd, idx, payload, raw)
    import introspect
    if event_type=='every_50ms' self.run_deferred() end  #- first run deferred events -#

    if event_type=='cmd' return self.exec_cmd(cmd, idx, payload)
    elif event_type=='rule' return self.exec_rules(payload)
    elif event_type=='gc' return self.gc()
    elif self._drivers
      for d:self._drivers
        var f = introspect.get(d, event_type)   # try to match a function or method with the same name
        if type(f) == 'function'
          try
            var done = f(d, cmd, idx, payload, raw)
            if done == true return true end
          except .. as e,m
            import string
            print(string.format("BRY: Exception> '%s' - %s", e, m))
          end
        end
      end
      return false
    end
  end

  def add_driver(d)
    if self._drivers
      self._drivers.push(d)
        else
      self._drivers = [d]
    end
  end

  def remove_driver(d)
    if self._drivers
      var idx = self._drivers.find(d)
      if idx != nil
        self._drivers.pop(idx)
      end
    end
  end

  # cmd high-level function
  def cmd(command)
    import json
    var ret = self._cmd(command)
    var j = json.load(ret)
    if type(j) == 'instance'
      return j
    else
      return {'response':j}
    end
  end

  # set_light and get_light deprecetaion
  def get_light(l)
    print('tasmota.get_light() is deprecated, use light.get()')
    import light
    if l != nil
      return light.get(l)
    else
      return light.get()
    end
  end
  def set_light(v,l)
    print('tasmota.set_light() is deprecated, use light.set()')
    import light
    if l != nil
      return light.set(v,l)
    else
      return light.set(v)
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
