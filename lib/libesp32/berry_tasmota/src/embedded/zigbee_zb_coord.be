# zigbee zcl_frame class
# solidify.dump(zb_coord,true)

class zb_coord_ntv end      # fake class replaced with native one
#zb_coord_ntv = classof(super(zigbee))

#@ solidify:zb_coord,weak
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
      if self._handlers.find(h) == nil      # make it idempotent
        self._handlers.push(h)
      end
    else
      self._handlers = [h]
    end
  end

  def remove_handler(h)
    if self._handlers
      var idx = self._handlers.find(h)
      if idx != nil
        self._handlers.remove(idx)
      end
    end
  end

  # dispatch incomind events
  #
  # event_type:
  #   'frame_received'
  #   'attributes_published'
  #
  # data_str:
  #    string
  #    '<zcl_frame>'
  #    '<zcl_attribute_list>'
  #
  def dispatch(event_type, zcl_frame_ptr, zcl_attribute_list_ptr, idx)
    if self._handlers == nil   return end

    import introspect

    var frame
    var attr_list
    var nullptr = introspect.toptr(0)
    if zcl_frame_ptr != nullptr
      frame = self.zcl_frame(zcl_frame_ptr)
    end
    if zcl_attribute_list_ptr != nullptr
      attr_list = self.zcl_attribute_list(zcl_attribute_list_ptr)
    end

    var i = 0
    while i < size(self._handlers)
      var h = self._handlers[i]
      var f = introspect.get(h, event_type)   # try to match a function or method with the same name
      if type(f) == 'function'
        try
          f(h, event_type, frame, attr_list, idx)
        except .. as e,m
          print(format("BRY: Exception> '%s' - %s", e, m))
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

