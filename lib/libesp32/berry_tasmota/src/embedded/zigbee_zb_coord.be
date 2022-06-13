# zigbee zcl_frame class
# solidify.dump(zb_coord,true)

class zb_coord_ntv end      # fake class replaced with native one
#zb_coord_ntv = classof(super(zigbee))

class zb_coord : zb_coord_ntv
  var _handlers

  def init()
    super(self).init()
  end

  def add_handler(h)
    if type(h) != 'instance'
      raise "value_error", "instance required"
    end
    if self._handlers
      self._handlers.push(h)
        else
      self._handlers = [h]
    end
  end

  def dispatch(event_type, data_type, data_ptr, idx)
    if self._handlers == nil   return end

    import introspect
    import string

    var data = data_ptr
    if data_type == "zcl_frame"
      data = self.zcl_frame(data_ptr)
    end
    
    #print(string.format(">ZIG: cmd=%s data_type=%s data=%s idx=%i", event_type, data_type, str(data), idx))

    var i = 0
    while i < size(self._handlers)
      var h = self._handlers[i]
      var f = introspect.get(h, event_type)   # try to match a function or method with the same name
      if type(f) == 'function'
        try
          f(h, event_type, data_type, data, idx)
        except .. as e,m
          print(string.format("BRY: Exception> '%s' - %s", e, m))
          if tasmota._debug_present
            import debug
            debug.traceback()
          end
        end
      end
      i += 1
    end
      
  end

end

