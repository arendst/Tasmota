#- embedded class for LVGL globals -#

#- This class stores all globals used by LVGL and cannot be stored in the solidified module -#
#- this limits the globals to a single value '_lvgl' -#
class LVGL_glob
  # all variables are lazily initialized to reduce the memory pressure. Until they are used, they consume zero memory
  var cb_obj                # map between a native C pointer (as int) and the corresponding lv.lv_* berry object, also helps marking the objects as non-gc-able
  var cb_event_closure      # mapping for event closures per LVGL native pointer (int)
  var event_cb              # native callback for lv.lv_event

  #- below are native callbacks mapped to a closure to a method of this instance -#
  var null_cb               # cb called if type is not supported
  var widget_ctor_cb
  var widget_dtor_cb
  var widget_event_cb
  
  var widget_struct_default
  var widget_struct_by_class

  #- this is the fallback callback, if the event is unknown or unsupported -#
  static cb_do_nothing = def() print("LVG: call to unsupported callback") end

  # register our callback handler to `module cb`
  def init()
    import cb
    cb.add_handler(/ f, obj, name -> self.make_cb(f, obj, name))
    # load extra modules
    import lv_extra
  end

  #- register an lv.lv_* object in the mapping -#
  def register_obj(obj)
    if self.cb_obj == nil    self.cb_obj = {} end
    self.cb_obj[obj._p] = obj
  end

  def get_object_from_ptr(ptr)
    if self.cb_obj != nil
      return self.cb_obj.find(ptr)   # raise an exception if something is wrong
    end
  end

  def lvgl_event_dispatch(event_ptr)
    import introspect

    var event = lv.lv_event(introspect.toptr(event_ptr))

    var target = event.target
    var f = self.cb_event_closure[target]
    var obj = self.get_object_from_ptr(target)
    #print('>> lvgl_event_dispatch', f, obj, event)
    f(obj, event)
  end

  def make_cb(f, obj, name)
    import cb
    # print('>> make_cb', f, name, obj)
    # record the object, whatever the callback
    
    if name  == "lv_event_cb"
      if self.cb_event_closure == nil   self.cb_event_closure = {} end    # lazy instanciation
      if self.event_cb == nil			      self.event_cb = cb.gen_cb(/ event_ptr -> self.lvgl_event_dispatch(event_ptr)) end  # encapsulate 'self' in closure
    
      self.register_obj(obj)                # keep a record of the object to prevent from being gc'ed
      if self.cb_event_closure.contains(obj._p)
        tasmota.log("LVG: object:" + str(obj) + "has already an event callback", 2)
      end
      self.cb_event_closure[obj._p] = f     # keep a mapping of the closure to call, indexed by internal lvgl native pointer
      return self.event_cb
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
  def widget_event_impl(cl_ptr, e_ptr)
    import introspect
    var cl = lv.lv_obj_class(cl_ptr)
    var event = lv.lv_event(e_ptr)
    var obj_ptr = event.target
    var obj = self.get_object_from_ptr(obj_ptr)
    if type(obj) == 'instance'
      if event.code == lv.EVENT_DELETE && introspect.get(obj, 'before_del')
        obj.before_del(cl, event)
      elif introspect.get(obj, 'widget_event')
        obj.widget_event(cl, event)
      end
    end
    # print("widget_event_impl", cl, obj_ptr, obj, event)
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
      if introspect.get(obj, 'widget_width_def')     obj_class_struct.width_def = obj.widget_width_def          end
      if introspect.get(obj, 'widget_height_def')    obj_class_struct.height_def = obj.widget_height_def        end
      if introspect.get(obj, 'widget_editable')      obj_class_struct.editable = obj.widget_editable            end
      if introspect.get(obj, 'widget_group_def')     obj_class_struct.group_def = obj.widget_group_def          end
      if introspect.get(obj, 'widget_instance_size') obj_class_struct.instance_size = obj.widget_instance_size  end

      #- keep a copy of the structure to avoid GC and reuse if needed -#
      self.widget_struct_by_class[obj_classname] = obj_class_struct
    end

    var lv_obj_ptr = lv.obj_class_create_obj(obj_class_struct, parent)
    obj._p = lv_obj_ptr._p
    self.register_obj(obj)
    obj.class_init_obj()
  end
end

_lvgl = LVGL_glob()

# class lv_custom_widget : lv.lv_obj
#   # static widget_width_def
#   # static widget_height_def
#   # static widget_editable
#   # static widget_group_def
#   # static widget_instance_size
#   #
#   var percentage              # value to display, range 0..100
#   var p1, p2, area, line_dsc  # instances of objects kept to avoid re-instanciating at each call

#   def init(parent)
#     _lvgl.create_custom_widget(self, parent)
#     # own values
#     self.percentage = 100
#     # pre-allocate buffers
#     self.p1 = lv.lv_point()
#     self.p2 = lv.lv_point()
#     self.area = lv.lv_area()
#     self.line_dsc = lv.lv_draw_line_dsc()
#   end

#   # def widget_constructor(cl)
#   #   print("widget_constructor", cl)
#   # end

#   # def widget_destructor(cl)
#   #   print("widget_destructor", cl)
#   # end

#   def widget_event(cl, event)
#     var res = lv.obj_event_base(cl, event)
#     if res != lv.RES_OK  return  end

#     def atleast1(x) if x >= 1 return x else return 1 end end
#     # the model is that we have 4 bars and inter-bar (1/4 of width)
#     var height = self.get_height()
#     var width = self.get_width()

#     var inter_bar = atleast1(width / 15)
#     var bar = atleast1((width - inter_bar * 3) / 4)
#     var bar_offset = bar / 2

#     var code = event.code
#     if code == lv.EVENT_DRAW_MAIN
#       var clip_area = lv.lv_area(event.param)
#       print("widget_event DRAW", clip_area.tomap())
#       # lv.event_send(self, lv.EVENT_DRAW_MAIN, clip_area)

#       # get coordinates of object
#       self.get_coords(self.area)
#       var x_ofs = self.area.x1
#       var y_ofs = self.area.y1

#       lv.draw_line_dsc_init(self.line_dsc)          # initialize lv.lv_draw_line_dsc structure
#       self.init_draw_line_dsc(lv.PART_MAIN, self.line_dsc)

#       self.line_dsc.round_start = 1
#       self.line_dsc.round_end = 1
#       self.line_dsc.width = bar
      
#       var on_color = self.get_style_line_color(lv.PART_MAIN | lv.STATE_DEFAULT)
#       var off_color = self.get_style_bg_color(lv.PART_MAIN | lv.STATE_DEFAULT)

#       lv.event_send(self, lv.EVENT_DRAW_PART_BEGIN, self.line_dsc)
#       for i:0..3    # 4 bars
#         self.line_dsc.color = self.percentage >= (i+1)*20 ? on_color : off_color
#         self.p1.y = y_ofs + height - 1 - bar_offset
#         self.p1.x = x_ofs + i * (bar + inter_bar) + bar_offset
#         self.p2.y = y_ofs + ((3 - i) * (height - bar)) / 4 + bar_offset
#         self.p2.x = self.p1.x
#         lv.draw_line(self.p1, self.p2, clip_area, self.line_dsc)
#       end
#       lv.event_send(self, lv.EVENT_DRAW_PART_END, self.line_dsc)

#     end
#   end

#   def set_percentage(v)
#     var old_bars = self.percentage / 5
#     if v > 100 v = 100 end
#     if v < 0   v = 0 end
#     self.percentage = v
#     if old_bars != v / 5
#       self.invalidate()    # be frugal and avoid updating the widget if it's not needed
#     end
#   end

#   def get_percentage()
#     return self.percentage
#   end
# end

# ########## ########## ########## ########## ########## ########## ########## ##########

# lv.start()

# hres = lv.get_hor_res()       # should be 320
# vres = lv.get_ver_res()       # should be 240

# scr = lv.scr_act()            # default screean object
# f20 = lv.montserrat_font(20)  # load embedded Montserrat 20

# scr.set_style_bg_color(lv.lv_color(0x0000A0), lv.PART_MAIN | lv.STATE_DEFAULT)

# w = lv_custom_widget(scr)