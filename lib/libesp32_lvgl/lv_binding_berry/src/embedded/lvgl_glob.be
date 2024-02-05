#- embedded class for LVGL globals -#

#@ solidify:LVGL_glob,weak
#- This class stores all globals used by LVGL and cannot be stored in the solidified module -#
#- this limits the globals to a single value '_lvgl' -#
class LVGL_glob
  # all variables are lazily initialized to reduce the memory pressure. Until they are used, they consume zero memory
  var cb_obj                # map between a native C pointer (as int) and the corresponding lv.lv_* berry object, also helps marking the objects as non-gc-able
  var cb_event_closure      # mapping for event closures per LVGL native pointer (int). For each int key, contains either a closure or an array with multiple closures
  var event_cb              # array of native callback for lv.lv_event (when multiple are needed)
  var timer_cb              # native callback for lv.lv_timer
  var event                 # keep aroud the current lv_event to avoid repeated allocation
  var general_event_cb      # new simplified way to register event_cb to any widget

  #- below are native callbacks mapped to a closure to a method of this instance -#
  var null_cb               # cb called if type is not supported
  var widget_ctor_cb
  var widget_dtor_cb
  var widget_event_cb       # callback object that calls `widget_event_impl(object, event)`
  
  var widget_struct_default
  var widget_struct_by_class

  #- this is the fallback callback, if the event is unknown or unsupported -#
  static cb_do_nothing = def() print("LVG: call to unsupported callback") end

  # register our callback handler to `module cb`
  def init()
    import cb
    cb.add_handler(/ f, obj, name -> self.make_cb(f, obj, name))
    # general cb for events
    # self.general_event_cb = cb.gen_cb(self._dispatch_event_cb)
    # load extra modules
    import lv_extra
  end

  #- register an lv.lv_* object in the mapping -#
  #
  # returns `true` if it was already present, false instead
  def register_obj(obj)
    if self.cb_obj == nil    self.cb_obj = {} end
    if self.cb_obj.contains(obj._p)
      return true
    else
      self.cb_obj[obj._p] = obj
      return false
    end
  end

  # get event callback by rank number, expand the list if needed
  def get_event_cb(n)
    if self.event_cb == nil   self.event_cb = [] end          # lazy initial initialization

    var cb_arr = self.event_cb
    for i: size(cb_arr) .. n
      var next_cb = cb.gen_cb(/ event_ptr -> self.lvgl_event_dispatch(i, event_ptr))
      cb_arr.push(next_cb)
    end

    return cb_arr[n]
  end

  def get_object_from_ptr(ptr)
    if self.cb_obj != nil
      return self.cb_obj.find(ptr)   # raise an exception if something is wrong
    end
  end

  def lvgl_event_dispatch(rank, event_ptr_i)
    import introspect
    var event_ptr = introspect.toptr(event_ptr_i)

    # use always the same instance of lv.lv_event by changing pointer, create a new the first time
    if self.event   self.event._p = event_ptr
    else            self.event = lv.lv_event(event_ptr)
    end

    var target = self.event.get_target()          # LVGL native object as target of the event (comptr)
    var obj = self.get_object_from_ptr(target)    # get the corresponding Berry LVGL object previously recorded as its container
    var f = self.cb_event_closure[target]         # get the closure or closure list known for this object
    if type(f) == 'function'                      # if only one callback, just use it
      f(obj, self.event)
    elif rank < size(f)                           # if more than one, then it's a list - use the closure for `rank`
      f[rank](obj, self.event)
    end
  end

  def lvgl_timer_dispatch(timer_int)
    import introspect

    var timer_ptr = introspect.toptr(timer_int)
    var f = self.cb_event_closure[timer_ptr]
    #print('>> lvgl_timer_dispatch', f, obj, event)
    if type(f) == 'function'
      f(timer_ptr)
    else
      # array
      var i = 0
      while i < size(f)
        f[i](timer_ptr)
        i += 1
      end
    end
  end

  # add a closure `f` to the object `o`
  #
  # returns: rank of the cb for this object, starting at `0`
  def add_cb_event_closure(o, f)
    if self.cb_event_closure == nil   self.cb_event_closure = {} end    # lazy instanciation
    if self.cb_event_closure.contains(o)
      # contains already a value, create an array if needed
      var cur = self.cb_event_closure[o]
      if type(cur) == 'function'
        self.cb_event_closure[o] = [cur, f]
        return 1
      else
        # should be already an array
        self.cb_event_closure[o].push(f)
        return size(self.cb_event_closure[o]) - 1
      end
    else
      self.cb_event_closure[o] = f
      return 0
    end
  end

  # remove all references to an object when `lv.EVENT_DELETE` is triggered
  def remove_cb(obj)
    import introspect
    var obj_ptr = obj._p
    # print(">>> DELETE",obj,obj_ptr)
    self.deregister_obj(obj_ptr)
    # print(self.cb_event_closure)
  end

  #
  # Warning: `make_cb` is called by `cb.gen_cb()` as a cb handler.
  # Calling back `cb.gen_cb()` should be done with caution to avoid infinite loops.
  #
  def make_cb(f, obj, name)
    # fast exit if not for us
    if name == nil return nil end

    import cb
    # print('>> make_cb', f, name, obj)
    # record the object, whatever the callback
    
    if name  == "lv_event_cb"
      var first_cb = !self.register_obj(obj)                # keep a record of the object to prevent from being gc'ed
      if first_cb                                           # first time we register a CB for this object, register also a DELETE event
        obj.add_event_cb(/ o -> self.remove_cb(o), lv.EVENT_DELETE, 0)
      end
      var rank = self.add_cb_event_closure(obj._p, f)
      # if self.cb_event_closure.contains(obj._p)
      #   tasmota.log("LVG: object:" + str(obj) + "has already an event callback", 2)
      # end
      # self.cb_event_closure[obj._p] = f     # keep a mapping of the closure to call, indexed by internal lvgl native pointer
      return self.get_event_cb(rank)
    elif name == "lv_timer_cb"
      if self.timer_cb == nil			      self.timer_cb = cb.gen_cb(/ timer_ptr -> self.lvgl_timer_dispatch(timer_ptr)) end  # encapsulate 'self' in closure

      self.add_cb_event_closure(obj._p, f)
      return self.timer_cb

    # elif name == "<other_cb>"
    elif name[0..2] == "lv_"
      if self.null_cb == nil                  self.null_cb = cb.gen_cb(self.cb_do_nothing) end
      return self.null_cb
    else
      return nil    # the call is not for us, pass to next handler
    end
  end

  def widget_ctor_impl(cl_ptr, obj_ptr)
    import introspect
    var cl = lv.lv_obj_class(cl_ptr)
    var obj = self.get_object_from_ptr(obj_ptr)
    if self.cb_obj.find(obj)  obj = self.cb_obj[obj]  end
    # print("widget_ctor_impl", cl, obj)
    if type(obj) == 'instance' && introspect.get(obj, 'widget_constructor')
      obj.widget_constructor(cl)
    end
  end
  def widget_dtor_impl(cl_ptr, obj_ptr)
    import introspect
    var cl = lv.lv_obj_class(cl_ptr)
    var obj = self.get_object_from_ptr(obj_ptr)
    # print("widget_dtor_impl", cl, obj)
    if type(obj) == 'instance' && introspect.get(obj, 'widget_destructor')
      obj.widget_destructor(cl)
    end
  end
  # called by event handler
  # input:
  #    cl_ptr: (int) pointer to class structure
  #    e_ptr: (int) pointer to event object
  def widget_event_impl(cl_ptr, e_ptr)
    import introspect
    # tasmota.log(f'>>>: widget_event_impl cl_ptr {cl_ptr=} e_ptr {e_ptr=}')
    var cl = lv.lv_obj_class(cl_ptr)
    var event = lv.lv_event(introspect.toptr(e_ptr))
    # tasmota.log(f'>>>: widget_event_impl cl {cl=} event {event=}')
    # var obj_ptr = event.target
    var obj_ptr = event.get_target_obj()
    var obj = self.get_object_from_ptr(obj_ptr._p)
    # tasmota.log(f">>>: widget_event_impl {obj_ptr=} {obj=}")
    if type(obj) == 'instance'
      if event.get_code() == lv.EVENT_DELETE && introspect.get(obj, 'before_del')
        obj.before_del(cl, event)
      elif introspect.get(obj, 'widget_event')
        obj.widget_event(cl, event)
      end
    end
  end


  def widget_cb()
    import cb
    if self.widget_ctor_cb == nil           self.widget_ctor_cb = cb.gen_cb(/ cl, obj -> self.widget_ctor_impl(cl, obj)) end
    if self.widget_dtor_cb == nil           self.widget_dtor_cb = cb.gen_cb(/ cl, obj -> self.widget_dtor_impl(cl, obj)) end
    if self.widget_event_cb == nil          self.widget_event_cb = cb.gen_cb(/ cl, e -> self.widget_event_impl(cl, e)) end

    if self.widget_struct_default == nil
      self.widget_struct_default = lv.lv_obj_class(lv.lv_obj._class).copy()
      self.widget_struct_default.base_class = lv.lv_obj._class               # by default, inherit from base class `lv_obj`, this can be overriden
      self.widget_struct_default.constructor_cb = self.widget_ctor_cb     # set the berry cb dispatchers
      self.widget_struct_default.destructor_cb = self.widget_dtor_cb
      self.widget_struct_default.event_cb = self.widget_event_cb
    end
  end

  #- deregister_obj all information linked to a specific LVGL native object (int) -#
  def deregister_obj(obj)
    if self.cb_obj != nil               self.cb_obj.remove(obj) end
    if self.cb_event_closure != nil     self.cb_event_closure.remove(obj) end
  end

  #- initialize a custom widget -#
  #- arg must be a subclass of lv.lv_obj -#
  def create_custom_widget(obj, parent)
    import introspect

    if !isinstance(obj, lv.lv_obj)   raise "value_error", "arg must be a subclass of lv_obj" end
    if self.widget_struct_by_class == nil     self.widget_struct_by_class = {} end

    var obj_classname = classname(obj)
    var obj_class_struct = self.widget_struct_by_class.find(obj_classname)
    # print("classname=",obj_classname,"_class",super(obj)._class)
    #- not already built, create a new one for this class -#
    if obj_class_struct == nil
      self.widget_cb()    # set up all structures
      obj_class_struct = self.widget_struct_default.copy()    # get a copy of the structure with pre-defined callbacks
      obj_class_struct.base_class = super(obj)._class
      if introspect.contains(obj, 'widget_width_def')     obj_class_struct.width_def = obj.widget_width_def          end
      if introspect.contains(obj, 'widget_height_def')    obj_class_struct.height_def = obj.widget_height_def        end
      if introspect.contains(obj, 'widget_editable')      obj_class_struct.editable = obj.widget_editable            end
      if introspect.contains(obj, 'widget_group_def')     obj_class_struct.group_def = obj.widget_group_def          end
      if introspect.contains(obj, 'widget_instance_size') obj_class_struct.instance_size = obj.widget_instance_size  end
      
      # tasmota.log(f'>>>: {obj_class_struct=} map {obj_class_struct.tomap()}')
      #- keep a copy of the structure to avoid GC and reuse if needed -#
      self.widget_struct_by_class[obj_classname] = obj_class_struct
    end

    var lv_obj_ptr = lv.obj_class_create_obj(obj_class_struct, parent)
    obj._p = lv_obj_ptr._p
    self.register_obj(obj)
    obj.class_init_obj()
  end

  # # new simplified way to implement a custom widget by just adding an event cb
  # #
  # # input:
  # #   lvovj: (instance of lv_obj of sub-class) the instance that will receive events
  # #   event_filter: lvgl event_filter
  # #
  # # The method `event_cb(lv_event)` will be called 
  # #
  # # TODO: make sure that the instance to the berry object is still valid and was not GCed
  # def add_event_cb(lvobj, event_filter)
  #   import introspect
  #   var cb = self.general_event_cb
  #   #lvovj.add_event_cb()
  # end

  # # the function called by LVGL and dispatching to the right object
  # #
  # # TODO
  # static def _dispatch_event_cb(event_int)
  # end

end


# ########## ########## ########## ########## ########## ########## ########## ##########

# lv.start()

# hres = lv.get_hor_res()       # should be 320
# vres = lv.get_ver_res()       # should be 240

# scr = lv.scr_act()            # default screean object
# f20 = lv.montserrat_font(20)  # load embedded Montserrat 20

# scr.set_style_bg_color(lv.lv_color(0x0000A0), lv.PART_MAIN | lv.STATE_DEFAULT)

# w = lv_custom_widget(scr)