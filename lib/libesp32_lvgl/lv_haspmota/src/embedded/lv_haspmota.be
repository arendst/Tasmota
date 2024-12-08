# HASPMota - OpenHASP compatibility module
#
# use `import haspmota` and set the JSONL definitions in `pages.jsonl`
#
# As an optimization `0 #- lv.PART_MAIN | lv.STATE_DEFAULT -#` is replaced with `0`
#
# rm haspmota.tapp; zip -j -0 haspmota.tapp haspmota_core/*
#################################################################################

var haspmota = module("haspmota")

#################################################################################
# Bytes list
#
# This function takes a list of events (uin8) and returns a bytes object
#
# It is used only at compile time, and is not included in the final flash
# The bytes object is far more compact than a list of ints and
# does automatic deduplication if the same list occurs twice
#################################################################################
def list_to_bytes(l)
  var b = bytes()
  for v: l
    b.add(v, 1)
  end
  return b
end

#################################################################################
# Pre-defined events lists
#################################################################################
var EVENTS_NONE = list_to_bytes([])
var EVENTS_TOUCH = list_to_bytes([lv.EVENT_PRESSED, lv.EVENT_CLICKED, lv.EVENT_PRESS_LOST, lv.EVENT_RELEASED,
                                lv.EVENT_LONG_PRESSED, #-lv.EVENT_LONG_PRESSED_REPEAT-# ])
var EVENTS_ALL = list_to_bytes([lv.EVENT_PRESSED, lv.EVENT_CLICKED, lv.EVENT_PRESS_LOST, lv.EVENT_RELEASED,
                                lv.EVENT_LONG_PRESSED, #-lv.EVENT_LONG_PRESSED_REPEAT,-#
                                lv.EVENT_VALUE_CHANGED ])   # adding VALUE_CHANGED


#################################################################################
# Class `lvh_root`
#
# Allows to map either a `lv_obj` for LVGL or arbitrary object
#
#################################################################################
#@ solidify:lvh_root,weak
class lvh_root
  static var _lv_class = nil                # _lv_class refers to the lvgl class encapsulated, and is overriden by subclasses
  static var _EVENTS = EVENTS_NONE

  # attributes to ignore when set at object level (they are managed by page)
  static var _attr_ignore = [
    "tostring",    # avoid issues with Berry `tostring` method
    # "id",
    "obj",
    "page",
    "comment",
    "parentid",
    # "auto_size",    # TODO not sure it's still needed in LVGL8
    # attributes for page
    "prev", "next", "back",
    "berry_run",    # run Berry code after the object is created
  ]

  # The following defines the mapping between the JSONL attribute name
  # and the Berry or LVGL attribute to set
  #
  # We try to map directly an attribute to the LVGL
  # Ex: HASPmota attribute `w` is mapped to LVGL `width`
  #
  # If mapping is null, we use set_X and get_X from our own class
  static var _attr_map = {
    "w": "width",
    "h": "height",
    "start_angle": "bg_start_angle",
    "start_angle1": "start_angle",
    "end_angle": "bg_end_angle",
    "end_angle1": "end_angle",
  }

  #====================================================================
  # Instance variables
  var id                                    # (int) object hasp id
  var _lv_obj                               # native lvgl object
  var _page                                 # parent page object
  var _parent_lvh                           # parent HASPmota object if 'parentid' was set, or 'nil'
  var _meta                                 # free form metadata

  #====================================================================
  # Rule engine to map value and text to rules
  # hence enabling auto-updates ob objects
  var _val                                  # last known value, useful for graceful initialization
  var _val_rule                             # rule pattern to map the `val` attribute
  var _val_rule_formula                     # Berry fragment to transform the value grabbed from rule
  var _val_rule_function                    # compiled function
  var _text_rule                            # rule pattern to map the `text` attribute
  var _text_rule_formula                    # Berry fragment to transform the value grabbed from rule before string format
  var _text_rule_function                   # compiled function
  var _text_rule_format                     # string format to transform the value grabbed from rule

  #################################################################################
  # General utilities
  #
  #################################################################################
  # Checks if the attribute is a color
  # I.e. ends with `color` (to not conflict with attributes containing `color_<x>`)
  #################################################################################
  def is_color_attribute(t)
    import string
    return string.endswith(str(t), "color")
  end

  #- remove trailing NULL chars from a bytes buffer before converting to string -#
  #- Berry strings can contain NULL, but this messes up C-Berry interface -#
  static def remove_trailing_zeroes(b)
    var sz = size(b)
    var i = 0
    while i < sz
      if b[-1-i] != 0   break end
      i += 1
    end
    if i > 0
      b.resize(size(b)-i)
    end
    return b
  end

  #====================================================================
  #  `delete` special attribute used to delete the object
  #====================================================================
  def set_delete(v)
    raise "type_error", "you cannot assign to 'delete'"
  end
  def get_delete()
    self._delete()
    return def () end
  end
  def _delete()
    # remove from page
    self._page.remove_obj(self.id)
  end
 
  #################################################################################
  # Parses a color attribute
  # 
  # `parse_color(hex:string) -> color:int` (as 24 bits RGB int)
  #
  # Parses colors in multiple forms:
  # - `0xRRGGBB`
  # - `#RRGGBB`
  # - `<color_name>` that are matched to `lv.COLOR_<color_name>` (ex: `red`) - case insensitive
  # - defaults to black `0x000000` if parsing fails
  #################################################################################
  static def parse_color(s)
    # inner function
    def parse_hex(s)
      # parse hex string
      # parse_hex(string) -> int
      # skip any `#` prefix, or `0x` and `0X` prefix
      import string
      var val = 0
      for i:0..size(s)-1
        var c = s[i]
        # var c_int = string.byte(c)
        if c == "#"             continue end  # skip '#' prefix if any
        if c == "x" || c == "X" continue end  # skip 'x' or 'X'
    
        if c >= "A" && c <= "F"
          val = (val << 4) | string.byte(c) - 55
        elif c >= "a" && c <= "f"
          val = (val << 4) | string.byte(c) - 87
        elif c >= "0" && c <= "9"
          val = (val << 4) | string.byte(c) - 48
        end
      end
      return val
    end

    s = str(s)
    if s[0] == '#'
      return lv.color(parse_hex(s))
    else
      import string
      import introspect
      var col_name = "COLOR_" + string.toupper(s)
      var col_try = introspect.get(lv, col_name)
      if col_try != nil
        return lv.color(col_try)
      end
    end
    # fail safe with black color
    return lv.color(0x000000)
  end

  #====================================================================
  #  parse_font
  #
  # For HASPmota compatiblity, default to "robotocondensed-latin1"
  # However we propose an extension to allow for other font names
  #
  # Arg1: (int) font size for `robotocondensed-latin1`
  #  or
  # Arg1: (string) "font_name-font_size", ex: "montserrat-20"
  #====================================================================
  def parse_font(t)
    var font
    if type(t) == 'int'
      try
        font = lv.font_embedded("robotocondensed", t)
      except ..
        try
          font = lv.font_embedded("montserrat", t)
        except ..
          print("HSP: Unsupported font:", t)
          return nil
        end
      end
    elif type(t) == 'string'
      import string
      # look for 'A:name.font' style font file name
      var drive_split = string.split(t, ':', 1)
      var fn_split = string.split(t, '-')

      var name = t
      var sz = 0
      var is_ttf = false
      var is_binary = (size(drive_split) > 1 && size(drive_split[0]))

      if size(fn_split) >= 2
        sz = int(fn_split[-1])
        name = fn_split[0..-2].concat('-')    # rebuild the font name
      end
      if string.endswith(name, ".ttf", true)
        # ttf font
        name = string.split(name, ':')[-1]      # remove A: if any
        is_ttf = true
      end

      if is_ttf
        font = lv.load_freetype_font(name, sz, 0)
      elif is_binary
        # font is from disk
        font = lv.load_font(t)
      elif sz > 0 && size(name) > 0              # looks good, let's have a try
        try
          font = lv.font_embedded(name, sz)
        except ..
          print("HSP: Unsupported font:", t)
        end
      end
    end
    if font != nil
      return font
    else
      print("HSP: Unsupported font:", t)
    end
  end

  #====================================================================
  # init HASPmota object from its jsonl definition
  #
  # parent: LVGL parent object (used to create a sub-object)
  # page: HASPmota page object
  # jline: JSONL definition of the object from HASPmota template (used in sub-classes)
  # obj: (opt) LVGL object if it already exists and was created prior to init()
  # parent_lvh: HASPmota parent object defined by `parentid`
  #====================================================================
  def init(parent, page, jline, obj, parent_lvh)
    self._page = page
    self._parent_lvh = parent_lvh
    if obj == nil && self._lv_class
      var obj_class = self._lv_class    # assign to a var to distinguish from method call
      self._lv_obj = obj_class(parent)  # instanciate LVGL object
    else
      self._lv_obj = obj
    end
    self.post_init()
  end

  #====================================================================
  # called once all attributes have been parsed
  # and gives an opportunity to clean up or refresh
  #====================================================================
  def post_config()
    # set again value, because the first time the range might not have been valid
    if (self._val != nil)
      self.set_val(self._val)
    end
  end

  #####################################################################
  # General Setters and Getters
  #####################################################################
  
  #====================================================================
  # get LVGL encapsulated object
  #====================================================================
  def get_obj()
    return self._lv_obj
  end

  #====================================================================
  # set_text: create a `lv_label` sub object to the current object
  # (default case, may be overriden by object that directly take text)
  #====================================================================
  def set_text(t)
  end
  def set_value_str(t) self.set_text(t) end
  def get_text()
    return nil
  end
  def get_value_str() return self.get_text() end

  #- ------------------------------------------------------------#
  # `digits_to_style` 
  # 
  # Convert a 2 digits style descriptor to LVGL style modifier
  # See https://www.openhasp.com/0.6.3/design/styling/
  #
  #
  # 00 = main part of the object (i.e. the background)
  # 10 = the indicator or needle, highlighting the the current value
  # 20 = the knob which can be used the change the value
  # 30 = the background of the items/buttons
  # 40 = the items/buttons
  # 50 = the selected item
  # 60 = major ticks of the gauge object
  # 70 = the text cursor
  # 80 = the scrollbar
  # 90 = other special part, not listed above
  #
  # LV_PART_MAIN         = 0x000000,   /**< A background like rectangle*/
  # LV_PART_SCROLLBAR    = 0x010000,   /**< The scrollbar(s)*/
  # LV_PART_INDICATOR    = 0x020000,   /**< Indicator, e.g. for slider, bar, switch, or the tick box of the checkbox*/
  # LV_PART_KNOB         = 0x030000,   /**< Like handle to grab to adjust the value*/
  # LV_PART_SELECTED     = 0x040000,   /**< Indicate the currently selected option or section*/
  # LV_PART_ITEMS        = 0x050000,   /**< Used if the widget has multiple similar elements (e.g. table cells)*/
  # LV_PART_CURSOR       = 0x060000,   /**< Mark a specific place e.g. for text area's cursor or on a chart*/
  # LV_PART_CUSTOM_FIRST = 0x080000,    /**< Extension point for custom widgets*/
  # LV_PART_ANY          = 0x0F0000,    /**< Special value can be used in some functions to target all parts*/
  #
  # 00 = default styling
  # 01 = styling for toggled state
  # 02 = styling for pressed, not toggled state
  # 03 = styling for pressed and toggled state
  # 04 = styling for disabled not toggled state
  # 05 = styling for disabled and toggled state
  #
  # LV_STATE_DEFAULT     =  0x0000,
  # LV_STATE_CHECKED     =  0x0001,
  # LV_STATE_FOCUSED     =  0x0002,
  # LV_STATE_FOCUS_KEY   =  0x0004,
  # LV_STATE_EDITED      =  0x0008,
  # LV_STATE_HOVERED     =  0x0010,
  # LV_STATE_PRESSED     =  0x0020,
  # LV_STATE_SCROLLED    =  0x0040,
  # LV_STATE_DISABLED    =  0x0080,

  # LV_STATE_USER_1      =  0x1000,
  # LV_STATE_USER_2      =  0x2000,
  # LV_STATE_USER_3      =  0x4000,
  # LV_STATE_USER_4      =  0x8000,
  #
  #- ------------------------------------------------------------#
  static var _digit2part = [
    lv.PART_MAIN,         # 00
    lv.PART_INDICATOR,    # 10
    lv.PART_KNOB,         # 20
    lv.PART_ITEMS,        # 30    TODO
    lv.PART_ITEMS,        # 40
    lv.PART_SELECTED,     # 50
    lv.PART_ITEMS,        # 60
    lv.PART_CURSOR,       # 70
    lv.PART_SCROLLBAR,    # 80
    lv.PART_CUSTOM_FIRST, # 90
  ]
  static var _digit2state = [
    lv.STATE_DEFAULT,                     # 00
    lv.STATE_CHECKED,                     # 01
    lv.STATE_PRESSED,                     # 02
    lv.STATE_CHECKED | lv.STATE_PRESSED,  # 03
    lv.STATE_DISABLED,                    # 04
    lv.STATE_DISABLED | lv.STATE_PRESSED, # 05
  ]
  def digits_to_style(digits)
    if digits == nil    return 0    end     # lv.PART_MAIN | lv.STATE_DEFAULT
    var first_digit = (digits / 10) % 10
    var second_digit = digits % 10
    var val = 0     # lv.PART_MAIN | lv.STATE_DEFAULT
    if first_digit >= 0 && first_digit < size(self._digit2part)
      val = val | self._digit2part[first_digit]
    else
      val = nil
    end
    if second_digit >= 0 && second_digit < size(self._digit2state)
      val = val | self._digit2state[second_digit]
    else
      val = nil
    end
    if val == nil
      raise "value_error", f"invalid style suffix {digits:02i}"
    end
    return val
  end

  #====================================================================
  #  Metadata
  #
  #====================================================================
  def set_meta(t)
    self._meta = t
  end
  def get_meta()
    return self._meta
  end

  #====================================================================
  #  Rule based updates of `val` and `text`
  #
  # `val_rule`: rule pattern to grab a value, ex: `ESP32#Temperature`
  # `val_rule_formula`: formula in Berry to transform the value
  #                     Ex: `val * 10`
  # `text_rule`: rule pattern to grab a value for text, ex: `ESP32#Temparature`
  # `text_rule_format`: format used by `format()`
  #                     Ex: `%.1f °C`
  #====================================================================
  def remove_val_rule()
    if self._val_rule != nil
      tasmota.remove_rule(self._val_rule, self)
    end
  end
  def set_val_rule(t)
    # remove previous rule if any
    self.remove_val_rule()

    self._val_rule = str(t)
    tasmota.add_rule(self._val_rule, / val -> self.val_rule_matched(val), self)
  end
  def get_val_rule()
    return self._val_rule
  end
  # text_rule
  def remove_text_rule()
    if self._text_rule != nil
      tasmota.remove_rule(self._text_rule, self)
    end
  end
  def set_text_rule(t)
    # remove previous rule if any
    self.remove_text_rule()

    self._text_rule = str(t)
    tasmota.add_rule(self._text_rule, / val -> self.text_rule_matched(val), self)
  end
  def get_text_rule()
    return self._text_rule
  end
  def set_text_rule_format(t)
    self._text_rule_format = str(t)
  end
  def get_text_rule_format()
    return self._text_rule_format
  end
  # formula that gets compiled as Berry code
  def set_val_rule_formula(t)
    self._val_rule_formula = str(t)
    var code = "return / val -> (" + self._val_rule_formula + ")"
    try
      var func = compile(code)
      self._val_rule_function = func()
    except .. as e, m
      print(format("HSP: failed to compile '%s' - %s (%s)", code, e, m))
    end
  end
  def get_val_rule_formula()
    return self._val_rule_formula
  end
  # formula that gets compiled as Berry code
  def set_text_rule_formula(t)
    self._text_rule_formula = str(t)
    var code = "return / val -> (" + self._text_rule_formula + ")"
    try
      var func = compile(code)
      self._text_rule_function = func()
    except .. as e, m
      print(format("HSP: failed to compile '%s' - %s (%s)", code, e, m))
    end
  end
  def get_text_rule_formula()
    return self._text_rule_formula
  end
  # rule matched for val
  def val_rule_matched(val)

    # print(">> rule matched", "val=", val)
    var val_n = real(val)         # force float type
    if val_n == nil  return false end   # if the matched value is not a number, ignore
    var func = self._val_rule_function
    if func != nil
      try
        val_n = func(val_n)
      except .. as e, m
        print(format("HSP: failed to run self._val_rule_function - %s (%s)", e, m))
      end
    end

    self.val = int(val_n)         # set value, truncate to int
    return false                  # propagate the event further
  end
  # rule matched for text
  def text_rule_matched(val)

    # print(">> rule matched text", "val=", val)
    if type(val) == 'int'
      val = real(val)           # force float type
    end

    var func = self._text_rule_function
    if func != nil
      try
        val = func(val)
      except .. as e, m
        print(format("HSP: failed to run self._text_rule_function - %s (%s)", e, m))
      end
    end

    var fmt = self._text_rule_format
    if type(fmt) == 'string'
      fmt = format(fmt, val)
    else
      fmt = ""
    end

    self.text = fmt
    return false                  # propagate the event further
  end
end

#################################################################################
#################################################################################
# Class `lvh_obj` encapsulating `lv_obj``
#
# Provide a mapping for virtual members
# Stores the associated page and object id
#
# Adds specific virtual members used by HASPmota
#################################################################################
#################################################################################
#@ solidify:lvh_obj,weak
class lvh_obj : lvh_root
  static var _lv_class = lv.obj     # _lv_class refers to the lvgl class encapsulated, and is overriden by subclasses
  static var _lv_part2_selector     # selector for secondary part (like knob of arc)
  static var _EVENTS = EVENTS_ALL

  #====================================================================
  # Instance variables
  var _lv_label                             # sub-label if exists
  var _action                               # value of the HASPmota `action` attribute, shouldn't be called `self.action` since we want to trigger the set/member functions

  #====================================================================
  # init HASPmota object from its jsonl definition
  #
  # parent: LVGL parent object (used to create a sub-object)
  # page: HASPmota page object
  # jline: JSONL definition of the object from HASPmota template (used in sub-classes)
  # obj: (opt) LVGL object if it already exists and was created prior to init()
  # parent_lvh: HASPmota parent object defined by `parentid`
  #====================================================================
  def init(parent, page, jline, obj, parent_lvh)
    super(self).init(parent, page, jline, obj, parent_lvh)
  end

  #====================================================================
  # post-init, to be overriden and used by certain classes
  #====================================================================
  def post_init()
    self.register_event_cb()
  end

  #####################################################################
  # General Setters and Getters
  #####################################################################

  #====================================================================
  # Value of the `action` attribute
  #====================================================================
  def set_action(t)
    self._action = str(t)
    # add callback when clicked
    # TODO
    # self._lv_obj.add_event_cb(/ obj, event -> self.action_cb(obj, event), lv.EVENT_CLICKED, 0)
  end
  def get_action()
    var action = self._action
    return action ? action : ""     # cannot be `nil` as it would mean no member
  end

  #====================================================================
  # Add cb for any action on the object
  #
  # Below is the mapping between HASP and LVGL (may need to adjust)
  # down = LV_EVENT_PRESSED
  # up = LV_EVENT_CLICKED
  # lost = LV_EVENT_PRESS_LOST
  # release = LV_EVENT_RELEASED
  # long = LV_EVENT_LONG_PRESSED
  # hold = LV_EVENT_LONG_PRESSED_REPEAT
  # changed = LV_EVENT_VALUE_CHANGED
  #====================================================================
  static var _event_map = {
    lv.EVENT_PRESSED:             "down",
    lv.EVENT_CLICKED:             "up",
    lv.EVENT_PRESS_LOST:          "lost",
    lv.EVENT_RELEASED:            "release",
    lv.EVENT_LONG_PRESSED:        "long",
    lv.EVENT_LONG_PRESSED_REPEAT: "hold",
    lv.EVENT_VALUE_CHANGED:       "changed",
  }
  def register_event_cb()
    var hm = self._page._hm
    var b = self._EVENTS
    var i = 0
    while (i < size(b))
      hm.register_event(self, b[i])
      i += 1
    end
  end

  def event_cb(event)
    # the callback avoids doing anything sophisticated in the cb
    # defer the actual action to the Tasmota event loop
    # print("-> CB fired","self",self,"obj",obj,"event",event.tomap(),"code",event.code)
    var hm = self._page._hm         # haspmota global object
    var code = event.get_code()     # materialize to a local variable, otherwise the value can change (and don't capture event object)
    if self.action != "" && code == lv.EVENT_CLICKED
      # if clicked and action is declared, do the page change event
      tasmota.set_timer(0, /-> hm.do_action(self, code))
    end

    var event_hasp = self._event_map.find(code)
    if event_hasp != nil
      import json

      var tas_event_more = ""   # complementary data
      if code == lv.EVENT_VALUE_CHANGED
        import introspect
        var val = introspect.get(self, true)     # does not raise an exception if not found
        if (val != nil && type(val) != 'module')
          tas_event_more = f',"val":{json.dump(val)}'
        end
        var text = introspect.get(self, true)     # does not raise an exception if not found
        if (text != nil && type(text) != 'module')
          tas_event_more += f',"text":{json.dump(text)}'
        end
      end
      var tas_event = format('{"hasp":{"p%ib%i":{"event":"%s"%s}}}', self._page._page_id, self.id, event_hasp, tas_event_more)
      # print("val=",val)
      tasmota.set_timer(0, /-> tasmota.publish_rule(tas_event))
    end
  end

  #====================================================================
  #  `_delete` special attribute used to delete the object
  #====================================================================
  # the actual _delete method, overriden
  def _delete()
    # remove any rule
    self.remove_val_rule()
    self.remove_text_rule()
    if (self._lv_label)   self._lv_label.del()    self._lv_label = nil    end
    if (self._lv_obj)     self._lv_obj.del()      self._lv_obj = nil      end
    super(self)._delete()
  end

  #====================================================================
  #  Mapping of synthetic attributes
  #  - text
  #  - hidden
  #  - enabled
  #====================================================================
  #- `hidden` attributes mapped to OBJ_FLAG_HIDDEN -#
  def set_hidden(h)
    if h
      self._lv_obj.add_flag(lv.OBJ_FLAG_HIDDEN)
    else
      self._lv_obj.clear_flag(lv.OBJ_FLAG_HIDDEN)
    end
  end
  def get_hidden()
    return self._lv_obj.has_flag(lv.OBJ_FLAG_HIDDEN)
  end

  #====================================================================
  #  `enabled` attributes mapped to STATE_DISABLED
  #====================================================================
  def set_enabled(h)
    if h
      self._lv_obj.clear_state(lv.STATE_DISABLED)
    else
      self._lv_obj.add_state(lv.STATE_DISABLED)
    end
  end
  def get_enabled()
    return !self._lv_obj.has_state(lv.STATE_DISABLED)
  end

  #====================================================================
  # click is synonym to enabled
  #====================================================================
  def set_click(t) self.set_enabled(t) end
  def get_click() return self.get_enabled() end

  #====================================================================
  #  line_width
  #====================================================================
  def set_line_width(t, style_modifier)
    self._lv_obj.set_style_line_width(int(t), style_modifier)
  end
  def get_line_width(style_modifier)
    return self._lv_obj.get_style_line_width(style_modifier)
  end

  #====================================================================
  #  `toggle` attributes mapped to STATE_CHECKED
  #====================================================================
  def set_toggle(t)
    import string
    if type(t) == 'string'
      t = string.toupper(str(t))
      if   t == "TRUE"  t = true
      elif t == "FALSE" t = false
      end
    end
    if t
      self._lv_obj.add_state(lv.STATE_CHECKED)
    else
      self._lv_obj.clear_state(lv.STATE_CHECKED)
    end
  end
  def get_toggle()
    return self._lv_obj.has_state(lv.STATE_CHECKED)
  end

  #====================================================================
  #  `adjustable` flag
  #====================================================================
  def set_adjustable(t)
    if t
      self._lv_obj.add_flag(lv.OBJ_FLAG_CLICKABLE)
    else
      self._lv_obj.clear_flag(lv.OBJ_FLAG_CLICKABLE)
    end
  end
  def get_adjustable()
    return self._lv_obj.has_flag(lv.OBJ_FLAG_CLICKABLE)
  end

  #====================================================================
  # set_text: create a `lv_label` sub object to the current object
  # (default case, may be overriden by object that directly take text)
  #====================================================================
  def check_label()
    if self._lv_label == nil
      import introspect
      if introspect.contains(self._lv_obj, "set_text")
        # if the `set_text` method exist, then use the native label
        self._lv_label = self._lv_obj
      else
        # otherwise create a sub-label object
        self._lv_label = lv.label(self.get_obj())
        self._lv_label.set_align(lv.ALIGN_CENTER);
      end
    end
  end
  def set_text(t)
    self.check_label()
    self._lv_label.set_text(str(t))
  end
  def get_text()
    if self._lv_label == nil return nil end
    return self._lv_label.get_text()
  end

  # mode
  def set_label_mode(t)
    var mode
    if    t == "expand" self._lv_obj.set_width(lv.SIZE_CONTENT)
    elif  t == "break"  mode = lv.LABEL_LONG_WRAP
    elif  t == "dots"   mode = lv.LABEL_LONG_DOT
    elif  t == "scroll" mode = lv.LABEL_LONG_SCROLL
    elif  t == "loop"   mode = lv.LABEL_LONG_SCROLL_CIRCULAR
    elif  t == "crop"   mode = lv.LABEL_LONG_CLIP
    end
    if mode != nil
      self.check_label()
      self._lv_label.set_long_mode(mode)
    end
  end
  def get_label_mode()
    if self._lv_label != nil
      return self._lv_label.get_long_mode()
    end
  end

  #====================================================================
  #  `align`: `left`, `center`, `right`
  #====================================================================
  def set_align(t, m)
    var align
    if (m == 0)   m = 0 #- lv.PART_MAIN | lv.STATE_DEFAULT -#   end
    self.check_label()
    if t == 0 || t == "left"
      align = lv.TEXT_ALIGN_LEFT
    elif t == 1 || t == "center"
      align = lv.TEXT_ALIGN_CENTER
    elif t == 2 || t == "right"
      align = lv.TEXT_ALIGN_RIGHT
    end
    self._lv_label.set_style_text_align(align, m)
  end

  def get_align(m)
    if (m == 0)   m = 0 #- lv.PART_MAIN | lv.STATE_DEFAULT -#   end
    if self._lv_label == nil return nil end
    var align = self._lv_label.get_style_text_align(m)
    if align == lv.TEXT_ALIGN_LEFT
      return "left"
    elif align == lv.TEXT_ALIGN_CENTER
      return "center"
    elif align == lv.TEXT_ALIGN_RIGHT
      return "right"
    else
      return align
    end
  end

  #====================================================================
  #  flex new_track
  #
  # Force flex child on a new line
  #====================================================================
  def set_flex_in_new_track(t)
    if t
      self._lv_obj.add_flag(lv.OBJ_FLAG_FLEX_IN_NEW_TRACK)
    else
      self._lv_obj.clear_flag(lv.OBJ_FLAG_FLEX_IN_NEW_TRACK)
    end
  end
  def get_flex_in_new_track()
    return self._lv_obj.has_flag(lv.OBJ_FLAG_FLEX_IN_NEW_TRACK)
  end

  #====================================================================
  #  `text_font`
  #
  # For HASPmota compatiblity, default to "robotocondensed-latin1"
  # However we propose an extension to allow for other font names
  #
  # Arg1: (int) font size for `robotocondensed-latin1`
  #  or
  # Arg1: (string) "font_name-font_size", ex: "montserrat-20"
  #====================================================================
  def set_text_font(t)
    var font = self.parse_font(t)
    if font != nil
      self._lv_obj.set_style_text_font(font, 0 #- lv.PART_MAIN | lv.STATE_DEFAULT -#)
    end
  end
  def get_text_font()
  end
  def set_value_font(t) self.set_text_font(t) end
  def get_value_font() return self.get_text_font() end

  #====================================================================
  #  `text_color`
  #====================================================================
  def set_text_color(t, style_modifier)
    self._lv_obj.set_style_text_color(self.parse_color(t), style_modifier)
  end
  def get_text_color(style_modifier)
    return self._lv_obj.get_style_text_color(style_modifier)
  end
  def set_value_color(t, style_modifier) self.set_text_color(t, style_modifier) end
  def get_value_color() return self.get_value_color() end

  #====================================================================
  #  `ofs_x`, `ofs_y`
  #====================================================================
  def set_value_ofs_x(t)
    self.check_label()
    self._lv_label.set_x(int(t))
  end
  def get_value_ofs_x()
    return self._lv_label.get_x()
  end
  def set_value_ofs_y(t)
    self.check_label()
    self._lv_label.set_y(int(t))
  end
  def get_value_ofs_y()
    return self._lv_label.get_y()
  end

  #====================================================================
  #  `pad_top2`, `pad_bottom2`, `pad_left2`, `pad_right2`, `pad_alL2`
  #   secondary element
  #====================================================================
  def set_pad_top2(t)
    if self._lv_part2_selector != nil
      self._lv_obj.set_style_pad_top(int(t), self._lv_part2_selector | lv.STATE_DEFAULT)
    end
  end
  def set_pad_bottom2(t)
    if self._lv_part2_selector != nil
      self._lv_obj.set_style_pad_bottom(int(t), self._lv_part2_selector | lv.STATE_DEFAULT)
    end
  end
  def set_pad_left2(t)
    if self._lv_part2_selector != nil
      self._lv_obj.set_style_pad_left(int(t), self._lv_part2_selector | lv.STATE_DEFAULT)
    end
  end
  def set_pad_right2(t)
    if self._lv_part2_selector != nil
      self._lv_obj.set_style_pad_right(int(t), self._lv_part2_selector | lv.STATE_DEFAULT)
    end
  end
  def set_pad_all2(t)
    if self._lv_part2_selector != nil
      self._lv_obj.set_style_pad_all(int(t), self._lv_part2_selector | lv.STATE_DEFAULT)
    end
  end

  #====================================================================
  #  `pad_top`, `pad_bottom`, `pad_left`, `pad_right`, `pad_all`
  #====================================================================
  def get_pad_top()
    if self._lv_part2_selector != nil
      return self._lv_obj.get_style_pad_top(self._lv_part2_selector | lv.STATE_DEFAULT)
    end
  end
  def get_pad_bottom()
    if self._lv_part2_selector != nil
      return self._lv_obj.get_style_pad_bottom(self._lv_part2_selector | lv.STATE_DEFAULT)
    end
  end
  def get_pad_left()
    if self._lv_part2_selector != nil
      return self._lv_obj.get_style_pad_left(self._lv_part2_selector | lv.STATE_DEFAULT)
    end
  end
  def get_pad_right()
    if self._lv_part2_selector != nil
      return self._lv_obj.get_style_pad_right(self._lv_part2_selector | lv.STATE_DEFAULT)
    end
  end
  def get_pad_all()
  end

  def set_val(t)
    import introspect
    self._val = t
    if introspect.contains(self._lv_obj, "set_value")
      self._lv_obj.set_value(t)
    end
  end
  def get_val()
    return self._lv_obj.get_value()
  end
  #====================================================================
  #  `radius2`
  #====================================================================
  def set_radius2(t)
    if self._lv_part2_selector != nil
      self._lv_obj.set_style_radius(int(t), self._lv_part2_selector | lv.STATE_DEFAULT)
    end
  end
  def get_radius2()
    if self._lv_part2_selector != nil
      return self._lv_obj.get_style_radius(self._lv_part2_selector | lv.STATE_DEFAULT)
    end
  end

  #- ------------------------------------------------------------#
  #  Internal utility functions
  #
  #  Mapping of virtual attributes
  #
  #- ------------------------------------------------------------#
  # `member` virtual getter
  #- ------------------------------------------------------------#
  def member(k)
    import string
    import introspect

    if string.startswith(k, "set_") || string.startswith(k, "get_")   return end

    # check if the attribute ends with 2 digits, if so remove the two suffix digits
    var style_modifier = nil
    if size(k) >= 3
      var char_last_1 = string.byte(k[-1])
      var char_last_2 = string.byte(k[-2])
      if (char_last_1 >= 0x30 && char_last_1 <= 0x39 && char_last_2 >= 0x30 && char_last_2 <= 0x39)
        # we extract the last 2 digits
        var suffix_digits = int(k[-2..])
        k = k[0..-3]      # remove 2 last digits
        style_modifier = self.digits_to_style(suffix_digits)
      end
    end
    # print(f">>>: getmember {k=} {style_modifier=}")

    # if attribute name is in ignore list, abort
    if self._attr_ignore.find(k) != nil return end

    # first check if there is a method named `get_X()`
    var f = introspect.get(self, "get_" + k)  # call self method
    if type(f) == 'function'
      # print(f">>>: getmember local method get_{k}")
      return f(self, style_modifier != nil ? style_modifier : 0)
    end

    # apply any synonym from _attr_map
    k = self._attr_map.find(k, k)

    # try first `get_X` from lvgl object, only if there is no style modifier
    if (style_modifier == nil)
      f = introspect.get(self._lv_obj, "get_" + k)
      if type(f) == 'function'                  # found and function, call it
        # print(f">>>: getmember standard method get_{k}")
        return f(self._lv_obj)
      end
    end

    # if not found, try `get_style_X`
    f = introspect.get(self._lv_obj, "get_style_" + k)
    if type(f) == 'function'                  # found and function, call it
      # print(f">>>: getmember style_ method get_{k}")
      # style function need a selector as second parameter
      return f(self._lv_obj, style_modifier != nil ? style_modifier : 0)
    end

    # fallback to exception if attribute unknown or not a function
    return module("undefined")
  end

  #- ------------------------------------------------------------#
  # `setmember` virtual setter
  #- ------------------------------------------------------------#
  def setmember(k, v)
    import string
    import introspect

    if string.startswith(k, "set_") || string.startswith(k, "get_")   return end

    # if value is 'real', round to nearest int
    if type(v) == 'real'
      import math
      v = int(math.round(v))
    end

    # parse value in percentage
    if string.endswith(k, "%")
      k = k[0..-2]
      v = lv.pct(int(v))
    end

    # check if the attribute ends with 2 digits, if so remove the two suffix digits
    var style_modifier = nil
    if size(k) >= 3
      var char_last_1 = string.byte(k[-1])
      var char_last_2 = string.byte(k[-2])
      if (char_last_1 >= 0x30 && char_last_1 <= 0x39 && char_last_2 >= 0x30 && char_last_2 <= 0x39)
        # we extract the last 2 digits
        var suffix_digits = int(k[-2..])
        k = k[0..-3]      # remove 2 last digits
        style_modifier = self.digits_to_style(suffix_digits)
      end
    end
    # print(f">>>: setmember {k=} {style_modifier=}")

    # if attribute name is in ignore list, abort
    if self._attr_ignore.find(k) != nil return end

    # first check if there is a method named `set_X()`
    var f = introspect.get(self, "set_" + k)
    if type(f) == 'function'
      # print(f">>>: setmember local method set_{k}")
      f(self, v, style_modifier != nil ? style_modifier : 0)
      return
    end

    # apply any synonym from _attr_map
    k = self._attr_map.find(k, k)

    # if the attribute contains 'color', convert to lv_color
    if self.is_color_attribute(k)
      v = self.parse_color(v)
    end
    
    # try first `set_X` from lvgl object
    if (style_modifier == nil)
      f = introspect.get(self._lv_obj, "set_" + k)
      if type(f) == 'function'                  # found and function, call it
        # print(f">>>: setmember standard method set_{k}")
        return f(self._lv_obj, v)
      end
    end

    # if not found, try `set_style_X`
    f = introspect.get(self._lv_obj, "set_style_" + k)
    if type(f) == 'function'                  # found and function, call it
      # print(f">>>: setmember style_ method set_{k}")
      # style function need a selector as second parameter
      return f(self._lv_obj, v, style_modifier != nil ? style_modifier : 0)
    end

    print("HSP: unknown attribute:", k)
  end
end

#################################################################################
#
#  Other widgets
#
#################################################################################

#====================================================================
#  fixed - container for a box with fixed sub-objects (no placement)
#====================================================================
#@ solidify:lvh_fixed,weak
class lvh_fixed : lvh_obj
  # static var _lv_class = lv.obj # from parent class
  # static var _EVENTS = EVENTS_ALL

  # label do not need a sub-label
  def post_init()
    super(self).post_init()         # call super
    var obj = self._lv_obj
    obj.set_style_pad_all(0, 0)
    obj.set_style_radius(0, 0)
    obj.set_style_border_width(0, 0)
    obj.set_style_margin_all(0, 0)
    obj.set_style_bg_opa(0, 0)
    obj.set_size(lv.pct(100), lv.pct(100))
  end
end

#====================================================================
#  flex - container for sub-objects placed along with flex directives
#====================================================================
#@ solidify:lvh_flex,weak
class lvh_flex : lvh_fixed
  # static var _lv_class = lv.obj # from parent class
  static var _EVENTS = EVENTS_NONE # inhetited
  # label do not need a sub-label
  def post_init()
    super(self).post_init()         # call super
    var obj = self._lv_obj
    obj.set_flex_flow(lv.FLEX_FLOW_ROW)
  end
end

#====================================================================
#  label
#====================================================================
#@ solidify:lvh_label,weak
class lvh_label : lvh_obj
  static var _lv_class = lv.label
  # label do not need a sub-label
  def post_init()
    self._lv_label = self._lv_obj   # the label is also the object itself
    super(self).post_init()         # call super
  end
end

#====================================================================
#  arc
#====================================================================
#@ solidify:lvh_arc,weak
class lvh_arc : lvh_obj
  static var _lv_class = lv.arc
  static var _lv_part2_selector = lv.PART_KNOB
  # static var _EVENTS = EVENTS_ALL
  var _label_angle                  # nil if none

  # line_width converts to arc_width
  def set_line_width(t, style_modifier)
    self._lv_obj.set_style_arc_width(int(t), style_modifier)
  end
  def get_line_width(style_modifier)
    return self._lv_obj.get_arc_line_width(style_modifier)
  end
  def set_line_width1(t)
    self._lv_obj.set_style_arc_width(int(t), lv.PART_INDICATOR | lv.STATE_DEFAULT)
  end
  def get_line_width1()
    return self._lv_obj.get_arc_line_width(lv.PART_INDICATOR | lv.STATE_DEFAULT)
  end

  def set_min(t)
    self._lv_obj.set_range(int(t), self.get_max())
    self.refresh_label_to_angle()
  end
  def set_max(t)
    self._lv_obj.set_range(self.get_min(), int(t))
    self.refresh_label_to_angle()
  end
  def get_min()
    return self._lv_obj.get_min_value()
  end
  def get_max()
    return self._lv_obj.get_max_value()
  end
  def set_type(t)
    var mode
    if   t == 0  mode = lv.ARC_MODE_NORMAL
    elif t == 1  mode = lv.ARC_MODE_REVERSE
    elif t == 2  mode = lv.ARC_MODE_SYMMETRICAL
    end
    if mode != nil
      self._lv_obj.set_mode(mode)
    end
  end
  def get_type()
    return self._lv_obj.get_mode()
  end
  # force refresh after set_val
  def set_val(t)
    super(self).set_val(t)
    self.refresh_label_to_angle()
  end
  # force refresh after set_text
  def set_text(t)
    super(self).set_text(t)
    self.refresh_label_to_angle()
  end
  #====================================================================
  # `label_to_angle`
  # ability to turn and offset the label.
  # value is the offset in pixels to move the label
  def set_label_to_angle(t)
    self._label_angle = int(t)
    self.refresh_label_to_angle()
  end
  def refresh_label_to_angle()
    if (self._label_angle != nil && self._lv_label != nil)
      self._lv_obj.rotate_obj_to_angle(self._lv_label, self._label_angle)
    end
  end
  # update after parsing
  def post_config()
    super(self).post_config()   # not needed yet
    self.refresh_label_to_angle()
  end
end

#====================================================================
#  switch
#====================================================================
#@ solidify:lvh_switch,weak
class lvh_switch : lvh_obj
  static var _lv_class = lv.switch
  static var _lv_part2_selector = lv.PART_KNOB
  # static var _EVENTS = EVENTS_ALL
  # map val to toggle
  def set_val(t)
    self._val = t
    return self.set_toggle(t)
  end
  def get_val()
    return self.get_toggle()
  end
end

#====================================================================
#  msgbox
#====================================================================
#@ solidify:lvh_msgbox,weak
class lvh_msgbox : lvh_obj
  static var _lv_class = lv.msgbox
  var _modal
  # sub_objects
  var _header, _footer, _content, _title
  var _buttons          # array containing the buttons, to apply styles later

  #====================================================================
  # init
  #
  # parent: LVGL parent object (used to create a sub-object)
  # page: HASPmota page object
  # jline: JSONL definition of the object from HASPmota template (used in sub-classes)
  # obj: (opt) LVGL object if it already exists and was created prior to init()
  # parent_lvh: HASPmota parent object defined by `parentid`
  #====================================================================
  def init(parent, page, jline, obj, parent_lvh)
    self._buttons = []
    self._modal = bool(jline.find("modal", false))
    if (self._modal)
      # the object created as modal is on top of everything
      self._lv_obj = lv.msgbox(0)
    end
    super(self).init(parent, page, jline, self._lv_obj, parent_lvh)
    # apply some default styles
    self.text_align = 2     # can be overriden
    self.bg_opa = 255       # can be overriden
  end

  #====================================================================
  # register_event_cb
  #
  # Override the normal event handler, we are only interested
  # in events on buttons
  #====================================================================
  def register_event_cb()
    # nothing to register for now, event_cb is allocated when buttons are allocted in `setoptions`
  end

  # update after parsing
  # 
  def post_config()
    var lvh_class = self._page._hm.lvh_obj      # get `lvh_obj` class from root instance
    var _lv_obj = self._lv_obj                  # get msgbox lvgl object
    # read the method, and return nil if exception 'value_error' occured
    def get_obj_safe(method)
      try
        var lv_obj = method(_lv_obj)            # equivalent of `self._lv_obj.get_XXX()` where XXX is header/footer/title/content
        return lvh_class(nil, self._page, {}, lv_obj, self) # instanciate a local lvh object
      except 'value_error'
        return nil
      end
    end

    super(self).post_config()
    # get sub-objects
    self._header = get_obj_safe(_lv_obj.get_header)
    self._footer = get_obj_safe(_lv_obj.get_footer)
    self._content = get_obj_safe(_lv_obj.get_content)
    self._title = get_obj_safe(_lv_obj.get_title)
  end

  #- ------------------------------------------------------------#
  # `setmember` virtual setter
  #
  # If the key starts with `footer_`, `header_`, `title_` or `content_`
  # send to the corresponding object
  #- ------------------------------------------------------------#
  def setmember(k, v)
    import string
    if string.startswith(k, 'footer_') && self._footer
      self._footer.setmember(k[7..], v)
    elif string.startswith(k, 'header_') && self._header
      self._header.setmember(k[7..], v)
    elif string.startswith(k, 'title_') && self._title
      self._title.setmember(k[6..], v)
    elif string.startswith(k, 'content_') && self._content
      self._content.setmember(k[8..], v)
    elif string.startswith(k, 'buttons_') && self._buttons
      for btn: self._buttons
        btn.setmember(k[8..], v)
      end
    else
      super(self).setmember(k, v)
    end
  end
  def member(k)
    import string
    if string.startswith(k, 'footer_') && self._footer
      return self._footer.member(k[7..])
    elif string.startswith(k, 'header_') && self._header
      return self._header.member(k[7..])
    elif string.startswith(k, 'title_') && self._title
      return self._title.member(k[6..])
    elif string.startswith(k, 'content_') && self._content
      return self._content.member(k[8..])
    else
      return super(self).member(k)
    end
  end

  # private function to add a button, create the lvh class and register callbacks
  def _add_button(msg)
    var lvh_class = self._page._hm.lvh_obj      # get `lvh_obj` class from root instance
    var btn_lv = self._lv_obj.add_footer_button(msg)
    var btn_lvh = lvh_class(nil, self._page, {}, btn_lv, self) # instanciate a local lvh object
    self._buttons.push(btn_lvh)
  end

  def set_options(l)
    if (isinstance(l, list) && size(l) > 0)
      for msg: l
        self._add_button(msg)
      end
    else
      print("HTP: 'msgbox' needs 'options' to be a list of strings")
    end
  end
  def get_options()
  end

  def set_title(t)
    self._lv_obj.add_title(str(t))
  end
  def get_title()
    # self._lv_obj.get_title()
  end
  def set_text(t)
    self._lv_obj.add_text(str(t))
  end
  def get_text()
    # self._lv_obj.get_text()
  end
end

#====================================================================
#  spinner
#====================================================================
#@ solidify:lvh_spinner,weak
class lvh_spinner : lvh_arc
  static var _lv_class = lv.spinner
  # static var _EVENTS = EVENTS_ALL # inherited
  var _speed, _angle

  #====================================================================
  # init
  #
  # parent: LVGL parent object (used to create a sub-object)
  # page: HASPmota page object
  # jline: JSONL definition of the object from HASPmota template (used in sub-classes)
  # obj: (opt) LVGL object if it already exists and was created prior to init()
  # parent_lvh: HASPmota parent object defined by `parentid`
  #====================================================================
  def init(parent, page, jline)
    var angle = jline.find("angle", 60)
    var speed = jline.find("speed", 1000)
    self._lv_obj = lv.spinner(parent)
    self._lv_obj.set_anim_params(speed, angle)
    super(self).init(parent, page, jline, self._lv_obj)
  end

  def set_angle(t) end
  def get_angle()  end
  def set_speed(t) end
  def get_speed()  end
end

#====================================================================
#  img
#====================================================================
#@ solidify:lvh_img,weak
class lvh_img : lvh_obj
  static var _lv_class = lv.image
  var _raw                        # used to store raw image in RAM
  var _imd_dsc

  def set_auto_size(v)
    if v
      self._lv_obj.set_inner_align(lv.IMAGE_ALIGN_STRETCH)
    end
  end
  def get_auto_size() end
  def set_angle(v)
    v = int(v)
    self._lv_obj.set_angle(v)
  end
  def get_angle()
    return self._lv_obj.get_angle()
  end
  #- ------------------------------------------------------------#
  # `src` virtual setter
  # If source is `tasmota_logo`, use the embedded logo
  #- ------------------------------------------------------------#
  def set_src(t)
    if (t == 'tasmota_logo')
      self._lv_obj.set_tasmota_logo()
    else
      self._lv_obj.set_src(t)
    end
  end
  #- ------------------------------------------------------------#
  # `raw` virtual setter
  # Decode base64
  #- ------------------------------------------------------------#
  def set_raw(t)
    self._raw = bytes().fromb64(t)
    var img_dsc = lv.lv_image_dsc()
    
    img_dsc.header_cf = lv.COLOR_FORMAT_RAW
    #img_dsc.header_w = 0
    #img_dsc.header_h = 0
    img_dsc.data_size = size(self._raw)
    img_dsc.data = self._raw._buffer()
    self._imd_dsc = img_dsc

    self._lv_obj.set_src(img_dsc)
  end
end

#====================================================================
#  qrcode
#====================================================================
#@ solidify:lvh_qrcode,weak
class lvh_qrcode : lvh_obj
  static var _lv_class = lv.qrcode
  var qr_text                         # any change needs the text to be update again

  def post_config()
    super(self).post_config()
    self._update()
  end
  def _update()
    var t = self.qr_text
    if (t != nil)
      self._lv_obj.update(t, size(t))
    end
  end
  # ignore attributes, spinner can't be changed once created
  def set_qr_size(t)                  self._lv_obj.set_size(t)                              self._update()  end
  def set_size(t)                     self._lv_obj.set_size(t)                              self._update()  end
  def get_qr_size() end
  def get_size() end
  def set_qr_dark_color(t)            self._lv_obj.set_dark_color(self.parse_color(t))      self._update()  end
  def set_dark_color(t)               self._lv_obj.set_dark_color(self.parse_color(t))      self._update()  end
  def get_qr_dark_color() end
  def get_dark_color() end
  def set_qr_light_color(t)            self._lv_obj.set_light_color(self.parse_color(t))     self._update()  end
  def set_light_color(t)               self._lv_obj.set_light_color(self.parse_color(t))     self._update()  end
  def get_qr_light_color() end
  def get_light_color() end
  def set_qr_text(t)
    self.qr_text = str(t)
    self._update()
  end
  def get_qr_text() end
end

#====================================================================
#  slider
#====================================================================
#@ solidify:lvh_slider,weak
class lvh_slider : lvh_obj
  static var _lv_class = lv.slider
  # static var _EVENTS = EVENTS_ALL

  def set_val(t)
    self._val = t
    self._lv_obj.set_value(t, 0)    # add second parameter - no animation
  end
  def set_min(t)
    self._lv_obj.set_range(int(t), self.get_max())
  end
  def set_max(t)
    self._lv_obj.set_range(self.get_min(), int(t))
  end
  def get_min()
    return self._lv_obj.get_min_value()
  end
  def get_max()
    return self._lv_obj.get_max_value()
  end
end

#====================================================================
#  roller
#====================================================================
#@ solidify:lvh_roller,weak
class lvh_roller : lvh_obj
  static var _lv_class = lv.roller

  def set_val(t)
    self._val = t
    self._lv_obj.set_selected(t, 0)    # add second parameter - no animation
  end
  def get_val()
    return self._lv_obj.get_selected()
  end

  def set_options(t)
    self._lv_obj.set_options(t, lv.ROLLER_MODE_NORMAL)
  end
  def get_options()
    return self._lv_obj.get_options()
  end

  def set_text(t)
    raise "attribute_error", "set_text unsupported on roller"
  end
  def get_text()
    # allocate a bytes buffer
    var b = bytes().resize(256)      # force 256 bytes
    self._lv_obj.get_selected_str(b._buffer(), 256)
    b = self.remove_trailing_zeroes(b)
    return b.asstring()
  end
end

#====================================================================
#  led
#====================================================================
#@ solidify:lvh_led,weak
class lvh_led : lvh_obj
  static var _lv_class = lv.led

  # `val` is a synonym for `brightness`
  def set_val(t)
    self._val = t
    self._lv_obj.set_brightness(t)
  end
  def get_val()
    return self._lv_obj.get_brightness()
  end

  def set_color(t)
    var v = self.parse_color(t)
    self._lv_obj.set_color(v)
  end
  def get_color()
  end
end

#====================================================================
#  dropdown
#====================================================================
#@ solidify:lvh_dropdown,weak
class lvh_dropdown : lvh_obj
  static var _lv_class = lv.dropdown
  # static var _EVENTS = EVENTS_ALL
  var _symbol                         # we need to keep a reference to the string used for symbol to avoid GC
  static var _dir = [ lv.DIR_BOTTOM, lv.DIR_TOP, lv.DIR_LEFT, lv.DIR_RIGHT ] # 0 = down, 1 = up, 2 = left, 3 = right

  def set_val(t)
    self._val = t
    self._lv_obj.set_selected(t)
  end
  def get_val()
    return self._lv_obj.get_selected()
  end

  def set_text(t)
    # set_text sets a static text displayed whatever the value
    # use `nil` to set back the text of the selected value
    self._lv_obj.set_text(t)
  end
  def get_text()
    var static_text = self._lv_obj.get_text()
    if static_text == nil
      # allocate a bytes buffer
      var b = bytes().resize(256)      # force 256 bytes
      self._lv_obj.get_selected_str(b._buffer(), 256)
      b = self.remove_trailing_zeroes(b)
      return b.asstring()
    else
      return static_text
    end
  end

  # direction needs a conversion from HASPmota numbers and LVGL's
  def set_direction(t)
    t = int(t)
    # 0 = down, 1 = up, 2 = left, 3 = right
    if (t < 0) || (t > 3)     t = 0   end
    self._lv_obj.set_dir(self._dir[t])
    if   t == 1       self._symbol = lv.SYMBOL_UP
    elif t == 2       self._symbol = lv.SYMBOL_LEFT
    elif t == 3       self._symbol = lv.SYMBOL_RIGHT
    else              self._symbol = lv.SYMBOL_DOWN
    end
    self._lv_obj.set_symbol(self._symbol)
  end
  def get_direction()
    var dir = self._lv_obj.get_dir()
    var i = 0
    while i < size(self._dir)
      if dir == self._dir[i]    return i end
      i += 1
    end
    return -1
  end

  # show_selected (bool) is a HASPmota addition
  # only meaningful if set to `true`, setting to false requires a call to `set_text`
  def set_show_selected(t)
    if t
      self._lv_obj.set_text(nil)  # undo static text
    end
  end
  def get_show_selected()
    var static_text = self._lv_obj.get_text()
    return (static_text == nil)
  end
end
#====================================================================
#  dropdown_list (accessing the list object)
#====================================================================
#@ solidify:lvh_dropdown_list,weak
class lvh_dropdown_list : lvh_obj
  static var _lv_class = nil
  # static var _EVENTS = EVENTS_NONE

  def post_init()
    self._lv_obj = nil                # default to nil object, whatever it was initialized with
    # check if it is the parent is a spangroup
    if isinstance(self._parent_lvh, self._page._hm.lvh_dropdown)
      self._lv_obj = lv.list(self._parent_lvh._lv_obj.get_list()._p)
    else
      print("HSP: '_dropdown_list' should have a parent of type 'dropdown'")
    end
    super(self).post_init()
  end
end

#====================================================================
#  bar
#====================================================================
#@ solidify:lvh_bar,weak
class lvh_bar : lvh_obj
  static var _lv_class = lv.bar
  
  def post_init()
    super(self).post_init()
    if isinstance(self._parent_lvh, self._page._hm.lvh_scale)
      # if sub-object of scale, copy min and max
      var min = self._parent_lvh._lv_obj.get_range_min_value()
      var max = self._parent_lvh._lv_obj.get_range_max_value()
      self._lv_obj.set_range(min, max)
    end
  end

  def set_val(t)
    self._val = t
    self._lv_obj.set_value(t, lv.ANIM_OFF)
  end
  def set_min(t)
    self._lv_obj.set_range(int(t), self._lv_obj.get_max_value())
  end
  def set_max(t)
    self._lv_obj.set_range(self._lv_obj.get_min_value(), int(t))
  end
  def get_min()
    return self._lv_obj.get_min_value()
  end
  def get_max()
    return self._lv_obj.get_max_value()
  end
end

#====================================================================
#  line
#====================================================================
#@ solidify:lvh_line,weak
class lvh_line : lvh_obj
  static var _lv_class = lv.line
  var _lv_points          # needs to save to avoid garbage collection
  # list of points
  def set_points(t)
    if isinstance(t, list)
      var pts = []
      for p: t
        if (isinstance(p, list) && size(p) == 2)
          var pt = lv.point()
          pt.x = int(p[0])
          pt.y = int(p[1])
          pts.push(pt)
        end
      end
      var pt_arr = lv.point_arr(pts)
      self._lv_points = pt_arr
      self._lv_obj.set_points_mutable(pt_arr, size(pts))
    else
      print(f"HSP: 'line' wrong format for 'points' {t}")
    end
  end
end

#====================================================================
#  scale
#====================================================================
#@ solidify:lvh_scale,weak
class lvh_scale : lvh_obj
  static var _lv_class = lv.scale
  var _options                      # need to keep the reference alive to avoid GC
  var _options_arr                  # need to keep the reference alive to avoid GC
  
  def set_text_src(l)
    if (isinstance(l, list) && size(l) > 0)
      # check if the last element is empty, if not add an empty string
      if size(l[-1]) > 0
        l.push("")
      end
      self._options = l
      self._options_arr = lv.str_arr(l)
      self._lv_obj.set_text_src(self._options_arr)
    else
      print("HTP: 'scale' needs 'text_src' to be a list of strings")
    end
  end
  def get_text_src()
    return self._options
  end
  def set_min(t)
    self._lv_obj.set_range(int(t), self._lv_obj.get_range_max_value())
  end
  def set_max(t)
    self._lv_obj.set_range(self._lv_obj.get_range_min_value(), int(t))
  end
  def get_min()
    return self._lv_obj.get_range_min_value()
  end
  def get_max()
    return self._lv_obj.get_range_max_value()
  end
end
#====================================================================
#  scale_section
#====================================================================
#@ solidify:lvh_scale_section,weak
class lvh_scale_section : lvh_root
  static var _lv_class = nil
  var _style                          # style object
  var _style10                        # style for LV_PART_INDICATOR
  var _style30                        # style for LV_PART_ITEMS
  var _min, _max

  def post_init()
    self._lv_obj = nil                # default to nil object, whatever it was initialized with
    self._min = 0                     # default value by LVGL
    self._max = 0                     # default value by LVGL
    # check if it is the parent is a spangroup
    if isinstance(self._parent_lvh, self._page._hm.lvh_scale)
      # print(">>> GOOD")
      self._lv_obj = self._parent_lvh._lv_obj.add_section()
      self._style = lv.style()        # we create a specific lv.style object for this object
      self._lv_obj.set_style(lv.PART_MAIN, self._style)
      self._style10 = lv.style()        # we create a specific lv.style object for this object
      self._lv_obj.set_style(lv.PART_INDICATOR, self._style10)
      self._style30 = lv.style()        # we create a specific lv.style object for this object
      self._lv_obj.set_style(lv.PART_ITEMS, self._style30)
    else
      print("HSP: 'scale_section' should have a parent of type 'scale'")
    end
    # super(self).post_init()         # call super - not needed for lvh_root
  end

  def set_min(t)
    var min = int(t)
    var max = self._max
    if (max < min)  max = min   end
    self.set_range(min, max)
  end
  def set_max(t)
    var min = self._min
    var max = int(t)
    if (min > max)  min = max   end
    self.set_range(min, max)
  end
  def set_range(min, max)
    self._min = min
    self._max = max
    self._lv_obj.set_range(min, max)
  end

  #====================================================================
  #  `_delete` special attribute used to delete the object
  #====================================================================
  # the actual _delete method, overriden
  def _delete()
    self._style.del()
    self._style = nil
    self._style10.del()
    self._style10 = nil
    self._style30.del()
    self._style30 = nil
    super(self)._delete()
  end

  #- ------------------------------------------------------------#
  # `setmember` virtual setter
  # trimmed down version for style only
  #- ------------------------------------------------------------#
  def setmember(k, v)
    import string
    import introspect

    if string.startswith(k, "set_") || string.startswith(k, "get_")   return end

    # parse value in percentage
    if string.endswith(k, "%")
      k = k[0..-2]
      v = lv.pct(int(v))
    end

    # check if the attribute ends with 2 digits, if so remove the two suffix digits
    var suffix_digits = nil
    if size(k) >= 3
      var char_last_1 = string.byte(k[-1])
      var char_last_2 = string.byte(k[-2])
      if (char_last_1 >= 0x30 && char_last_1 <= 0x39 && char_last_2 >= 0x30 && char_last_2 <= 0x39)
        # we extract the last 2 digits
        suffix_digits = int(k[-2..])
        k = k[0..-3]      # remove 2 last digits

        # only style modifiers allowed are `10` and `30`
        if suffix_digits != 10 && suffix_digits != 30
          raise "value_error", "only modifiers '10' or '30' allowed"
        end
      end
    end

    # if attribute name is in ignore list, abort
    if self._attr_ignore.find(k) != nil return end

    # select the right style object
    var style = self._style
    if suffix_digits == 10
      style = self._style10
    elif suffix_digits == 30
      style = self._style30
    end

    # first check if there is a method named `set_X()`
    var f = introspect.get(self, "set_" + k)
    if type(f) == 'function'
      # print(f">>>: setmember local method set_{k}")
      f(self, v)
      return
    end

    # simply check if a method `set_{k}` exists
    f = introspect.get(style, "set_" + k)    # look at style
    # print(f">>>: span name={'set_' + k} {f=}")
    if (type(f) == 'function')
      # if the attribute contains 'color', convert to lv_color
      if self.is_color_attribute(k)
        v = self.parse_color(v)
      end
      # invoke
      try
        f(style, v)
      except .. as e, m
        raise e, m + " for " + k
      end
      return nil
    else
      print("HSP: Could not find function set_" + k)
    end

  end
end

#====================================================================
#  scale_line
#====================================================================
#@ solidify:lvh_scale_line,weak
class lvh_scale_line : lvh_line
  var _needle_length
  # var _lv_points          # in superclass

  def post_init()
    # check if it is the parent is a spangroup
    if !isinstance(self._parent_lvh, self._page._hm.lvh_scale)
      print("HSP: 'scale_line' should have a parent of type 'scale'")
    end
    self._needle_length = 0
    self._lv_points = lv.point_arr([lv.point(), lv.point()])    # create an array with 2 points
    super(self).post_init()
  end

  def set_needle_length(t)
    self._needle_length = int(t)
    # force a refresh
    if self._val != nil
      self.set_val(self._val)
    end
  end
  def get_needle_length()
    return self._needle_length
  end

  def set_val(t)
    super(self).set_val(t)
    self._parent_lvh._lv_obj.set_line_needle_value(self._lv_obj, self._needle_length, self._val)
    # work-around for points being global static
    if (self._lv_obj.get_point_count() == 2)      # check that there are only 2 points
      # read back the computed points
      var p_arr = bytes(self._lv_obj.get_points(), size(self._lv_points))
      self._lv_points.setbytes(0, p_arr)
      self._lv_obj.set_points_mutable(self._lv_points, 2)
    end
  end
end


#====================================================================
#  spangroup
#====================================================================
#@ solidify:lvh_spangroup,weak
class lvh_spangroup : lvh_obj
  static var _lv_class = lv.spangroup
  # label do not need a sub-label
  def post_init()
    self._lv_obj.set_mode(lv.SPAN_MODE_BREAK)           # use lv.SPAN_MODE_BREAK by default
    self._lv_obj.refr_mode()
    super(self).post_init()         # call super -- not needed
  end
  # refresh mode
  def refr_mode()
    self._lv_obj.refr_mode()
  end
end

#====================================================================
#  span
#====================================================================
#@ solidify:lvh_span,weak
class lvh_span : lvh_root
  static var _lv_class = nil
  # label do not need a sub-label
  var _style                          # style object

  def post_init()
    self._lv_obj = nil                # default to nil object, whatever it was initialized with
    # check if it is the parent is a spangroup
    if isinstance(self._parent_lvh, self._page._hm.lvh_spangroup)
      # print(">>> GOOD")
      self._lv_obj = self._parent_lvh._lv_obj.new_span()
      self._style = self._lv_obj.get_style()
    end
    # super(self).post_init()         # call super - not needed for lvh_root
  end

  #====================================================================
  def set_text(t)
    self._lv_obj.set_text(str(t))
  end

  #====================================================================
  def set_text_font(t)
    var font = self.parse_font(t)
    if font != nil
      self._style.set_text_font(font)
      self._parent_lvh.refr_mode()
    end
  end
  
  #- ------------------------------------------------------------#
  #  Internal utility functions
  #
  #  Mapping of virtual attributes
  #
  #- ------------------------------------------------------------#
  # There are no attributes that can be read from `lv.style``
  # so we don't need virtual members
  #- ------------------------------------------------------------#
  # def member(k)
  #   import string
  #   import introspect

  #   if string.startswith(k, "set_") || string.startswith(k, "get_")   return end

  #   # if attribute name is in ignore list, abort
  #   if self._attr_ignore.find(k) != nil return end

  #   # first check if there is a method named `get_X()`
  #   var f = introspect.get(self, "get_" + k)
  #   if type(f) == 'function'
  #     # print(f">>>: setmember local method set_{k}")
  #     return f(self)
  #   end

  #   # finally try any `get_XXX` within the LVGL object
  #   f = introspect.get(self._style, "get_" + k)
  #   if type(f) == 'function'                  # found and function, call it
  #     return f(self._style)
  #   end

  #   # fallback to exception if attribute unknown or not a function
  #   return module("undefined")
  # end

  #- ------------------------------------------------------------#
  # `setmember` virtual setter
  # trimmed down version for style only
  #- ------------------------------------------------------------#
  def setmember(k, v)
    import string
    import introspect

    if string.startswith(k, "set_") || string.startswith(k, "get_")   return end

    # parse value in percentage
    if string.endswith(k, "%")
      k = k[0..-2]
      v = lv.pct(int(v))
    end

    # if attribute name is in ignore list, abort
    if self._attr_ignore.find(k) != nil return end

    # first check if there is a method named `set_X()`
    var f = introspect.get(self, "set_" + k)
    if type(f) == 'function'
      # print(f">>>: setmember local method set_{k}")
      f(self, v)
      return
    end

    # simply check if a method `set_{k}` exists
    f = introspect.get(self._style, "set_" + k)    # look at style
    # print(f">>>: span name={'set_' + k} {f=}")
    if (type(f) == 'function')
      # if the attribute contains 'color', convert to lv_color
      if self.is_color_attribute(k)
        v = self.parse_color(v)
      end
      # invoke
      try
        f(self._style, v)
        self._parent_lvh.refr_mode()
      except .. as e, m
        raise e, m + " for " + k
      end
      return nil
    else
      print("HSP: Could not find function set_" + k)
    end

  end

end

#################################################################################
# Special case for lv.chart
# Adapted to getting values one after the other
#################################################################################
#@ solidify:lvh_chart,weak
class lvh_chart : lvh_obj
  static var _lv_class = lv.chart
  # ser1/ser2 contains the first/second series of data
  var _ser1, _ser2
  # y_min/y_max contain the main range for y. Since LVGL does not have getters, we need to memorize on our side the lates tvalues
  var _y_min, _y_max
  # h_div/v_div contain the horizontal and vertical divisions, we need to memorize values because both are set from same API
  var _h_div, _v_div

  def post_init()
    # default values from LVGL are 0..100
    self._y_min = 0
    self._y_max = 100
    # default values
    #define LV_CHART_HDIV_DEF 3
    #define LV_CHART_VDIV_DEF 5
    self._h_div = 3
    self._v_div = 5

    self._lv_obj.set_update_mode(lv.CHART_UPDATE_MODE_SHIFT)

    self._ser1 = self._lv_obj.add_series(lv.color(0xEE4444), lv.CHART_AXIS_PRIMARY_Y)
    self._ser2 = self._lv_obj.add_series(lv.color(0x44EE44), lv.CHART_AXIS_PRIMARY_Y)
  end

  def add_point(v)
    self._lv_obj.set_next_value(self._ser1, v)
  end
  def add_point2(v)
    self._lv_obj.set_next_value(self._ser2, v)
  end

  def set_val(v)
    self._val = v
    self.add_point(v)
  end
  def set_val2(v)
    self.add_point2(v)
  end
  def get_y_min()
    return self._y_min
  end
  def get_y_max()
    return self._y_max
  end
  def set_y_min(_y_min)
    self._y_min = _y_min
    self._lv_obj.set_range(lv.CHART_AXIS_PRIMARY_Y, self._y_min, self._y_max)
  end
  def set_y_max(_y_max)
    self._y_max = _y_max
    self._lv_obj.set_range(lv.CHART_AXIS_PRIMARY_Y, self._y_min, self._y_max)
  end

  def set_series1_color(color)
    self._lv_obj.set_series_color(self._ser1, self.parse_color(color))
  end
  def set_series2_color(color)
    self._lv_obj.set_series_color(self._ser2, self.parse_color(color))
  end
  def set_h_div_line_count(_h_div)
    self._h_div = _h_div
    self._lv_obj.set_div_line_count(self._h_div, self._v_div)
  end
  def set_v_div_line_count(_v_div)
    self._v_div = _v_div
    self._lv_obj.set_div_line_count(self._h_div, self._v_div)
  end
end

#====================================================================
#  btnmatrix
#====================================================================
#@ solidify:lvh_btnmatrix,weak
class lvh_btnmatrix : lvh_obj
  static var _lv_class = lv.buttonmatrix
  var _options                      # need to keep the reference alive to avoid GC
  var _options_arr                  # need to keep the reference alive to avoid GC
  
  def set_options(l)
    if (isinstance(l, list) && size(l) > 0)
      # check if the last element is empty, if not add an empty string
      if size(l[-1]) > 0
        l.push("")
      end
      self._options = l
      self._options_arr = lv.str_arr(l)
      self._lv_obj.set_map(self._options_arr)
    else
      print("HTP: 'btnmatrix' needs 'options' to be a list of strings")
    end
  end
  def get_options()
    return self._options
  end
end

#====================================================================
#  cpicker - color picker
#
# OpenHASP maps to LVGL 7 `cpicker`
# However `cpicker` was replaced with `colorwheel` in LVGL 8
# and removed in LVGL 9.
# We have ported back `colorwheel` from LVGL 8 to LVGL 9
#====================================================================
#@ solidify:lvh_cpicker,weak
class lvh_cpicker : lvh_obj
  static var _lv_class = lv.colorwheel
  static var _CW_MODES = ['hue', 'saturation', 'value']

  # we need a non-standard initialization of lvgl object
  def init(parent, page, jline, obj, parent_lvh)
    obj = lv.colorwheel(parent, true #-knob_recolor = true-#)
    super(self).init(parent, page, jline, obj, parent_lvh)
    self.set_scale_width(25)      # align to OpenHASP default value
  end

  def set_color(t)
    var v = self.parse_color(t)
    self._lv_obj.set_rgb(v)
  end
  def get_color()
    var color = self._lv_obj.get_rgb()
    return f"#{color:06X}"
  end

  def set_mode(s)
    var mode = self._CW_MODES.find(s)
    if (mode != nil)
      self._lv_obj.set_mode(mode)
    else
      raise "value_error", f"unknown color mode '{mode}'"
    end
  end
  def get_mode()
    var mode = self._lv_obj.get_color_mode()
    if (mode >= 0) && (mode < size(self._CW_MODES))
      return self._CW_MODES[mode]
    else
      return 'unknown'
    end
  end

  def set_mode_fixed(b)
    b = bool(b)
    self._lv_obj.set_mode_fixed(b)
  end
  def get_mode_fixed()
    return self._lv_obj.get_color_mode_fixed()
  end

  def set_scale_width(v)
    self._lv_obj.set_style_arc_width(int(v), 0)
  end
  def get_scale_width()
    return self._lv_obj.get_style_arc_width(0)
  end
  # pad_inner is ignored (for now?)
  def set_pad_inner() end
  def get_pad_inner() end
end

#################################################################################
#
# All other subclasses than just map the LVGL object
# and doesn't have any specific behavior
#
#################################################################################
#@ solidify:lvh_btn,weak
class lvh_btn : lvh_obj         static var _lv_class = lv.button      end
#@ solidify:lvh_checkbox,weak
class lvh_checkbox : lvh_obj    static var _lv_class = lv.checkbox    end
# class lvh_textarea : lvh_obj    static var _lv_class = lv.textarea    end
# special case for scr (which is actually lv_obj)
#@ solidify:lvh_scr,weak
class lvh_scr : lvh_obj
  static var _lv_class = nil    # no class for screen
end


#################################################################################
#  Class `lvh_page`
#
#  Encapsulates a `lv_screen` which is `lv.obj(0)` object
#################################################################################
#
# ex of transition: lv.scr_load_anim(scr, lv.SCR_LOAD_ANIM_MOVE_RIGHT, 500, 0, false)
#@ solidify:lvh_page,weak
class lvh_page
  var _obj_id               # (map) of `lvh_obj` objects by id numbers
  var _page_id              # (int) id number of this page
  var _lv_scr               # (lv_obj) lvgl screen object
  var _hm                   # HASPmota global object
  # haspmota attributes for page are on item `#0`
  var prev, next, back      # (int) id values for `prev`, `next`, `back` buttons

  #====================================================================
  #  `init`
  #
  # arg1: `page_number` (int) HASPmota page id
  #        defaults to `1` if not specified
  #        page 0 is special, visible on all pages. Internally uses `layer_top`
  # arg2: `hm` global HASPmota monad object
  #  page_number: haspmota page number, defaults to `1` if not specified
  #====================================================================
  def init(page_number, hm)
    import global
    self._hm = hm                   # memorize HASPmota parent object

    # if no parameter, default to page #1
    page_number = int(page_number)
    if page_number == nil  page_number = 1 end

    self._page_id = page_number     # remember our page_number
    self._obj_id = {}               # init list of objects

    # initialize the LVGL object for the page
    # uses a lv_scr object except for page 0 where we use layer_top
    # page 1 is mapped directly to the default screen `scr_act`
    if page_number == 0
      self._lv_scr = lv.layer_top() # top layer, visible over all screens
    else
      self._lv_scr = lv.obj(0)      # allocate a new screen
      var bg_color = lv.scr_act().get_style_bg_color(0 #- lv.PART_MAIN | lv.STATE_DEFAULT -#) # bg_color of default screen
      self._lv_scr.set_style_bg_color(bg_color, 0 #- lv.PART_MAIN | lv.STATE_DEFAULT -#) # set white background
    end

    # page object is also stored in the object map at id `0` as instance of `lvg_scr`
    var lvh_scr_class = self._hm.lvh_scr
    var obj_scr = lvh_scr_class(nil, self, nil, self._lv_scr)   # store screen in a virtual object
    self._obj_id[0] = obj_scr

    # create a global for this page of form p<page_number>, ex `p1`
    # create a global for the page attributes as p<page_number>b0, ex `p1b0`
    global.(f"p{self._page_id}") = self
    global.(f"p{self._page_id}b0") = obj_scr
  end

  #####################################################################
  # General Setters and Getters
  #####################################################################
  
  #- ------------------------------------------------------------#
  #  Internal utility functions
  #
  #  Mapping of virtual attributes
  #
  #- ------------------------------------------------------------#
  # `member` virtual getter
  #- ------------------------------------------------------------#
  def member(k)
    import string
    import introspect

    if string.startswith(k, "set_") || string.startswith(k, "get_")   return end

    # if attribute name is in ignore list, abort
    # if self._attr_ignore.find(k) != nil return end
    # we don't need an ignore list for pages

    # first check if there is a method named `get_X()`
    var f = introspect.get(self, "get_" + k)  # call self method
    if type(f) == 'function'
      # print(f">>>: getmember local method get_{k}")
      return f(self)
    end

    # fallback to exception if attribute unknown or not a function
    return module("undefined")
  end

  #====================================================================
  # retrieve lvgl screen object for this page
  #====================================================================
  def get_scr()
    return self._lv_scr
  end

  #====================================================================
  # return id of this page
  #====================================================================
  def id()
    return self._page_id
  end

  #====================================================================
  # add an object to this page
  #====================================================================
  def get_obj(obj_id)
    return self._obj_id.find(obj_id)
  end
  def add_obj(obj_id, obj_lvh)
    # add object to page object
    self._obj_id[obj_id] = obj_lvh
    
    # create a global variable for this object of form p<page>b<id>, ex p1b2
    var glob_name = format("p%ib%i", obj_lvh._page.id(), obj_id)
    global.(glob_name) = obj_lvh
  end
  def remove_obj(obj_id)
    # remove object from page object
    var obj_lvh = self._obj_id.find(obj_id)
    self._obj_id.remove(obj_id)

    # set the global variable to `nil`
    if obj_lvh
      var glob_name = format("p%ib%i", obj_lvh._page.id(), obj_id)
      global.(glob_name) = nil
    end
  end

  #====================================================================
  #  `delete` special attribute used to delete the object
  #====================================================================
  def get_clear()
    self._clear()
    return def () end
  end
  def _clear()
    # iterate on all objects and try to delete
    # we first get a copy of all ids so we can delete and continue iterating
    # without fearing about an infinite loop
    var ids = []
    for id: self._obj_id.keys()
      ids.push(id)
    end
    # we iterate until the array is empty
    var idx = 0
    while idx < size(ids)
      var page_id = ids[idx]
      if (page_id != 0) && self._obj_id.contains(page_id)
        # first check if the id is still in the page - it could have been already removed if it's a sub-object
        self._obj_id[page_id]._delete()
      end
      idx += 1
    end
    self._obj_id = {}       # clear map
  end
  def get_delete()
    self._delete()
    return def () end
  end
  def _delete()
    # remove from page, also change page if this is the current one
    self._hm._remove_page(self._page_id)
    # clear content
    self._clear()
  end

  #====================================================================
  #  `show` transition from one page to another
  #    duration: in ms, default 500 ms
  #    anim: -1 right to left, 1 left to right (default), `nil` auto, 0 none
  #
  #  show this page, with animation
  #====================================================================
  static show_anim = {
     1: lv.SCR_LOAD_ANIM_MOVE_LEFT,
    -1: lv.SCR_LOAD_ANIM_MOVE_RIGHT,
    -2: lv.SCR_LOAD_ANIM_MOVE_TOP,
     2: lv.SCR_LOAD_ANIM_MOVE_BOTTOM,
     0: lv.SCR_LOAD_ANIM_NONE,
  }
  def show(anim, duration)
    # ignore if the page does not contain a screen, like when id==0
    if self._lv_scr == nil return nil end

    # ignore if the screen is already active
    # compare native LVGL objects with current screen
    if self._lv_scr._p == lv.scr_act()._p return end    # do nothing

    # default duration of 500ms
    if (duration == nil)  duration = 500 end

    # if anim is `nil` try to guess the direction from current screen
    if anim == nil
      anim = self._hm.page_dir_to(self.id())
    end

    # send page events
    var event_str_in = format('{"hasp":{"p%i":"out"}}', self._hm.lvh_page_cur_idx)
    tasmota.set_timer(0, /-> tasmota.publish_rule(event_str_in))
    var event_str_out = format('{"hasp":{"p%i":"in"}}', self._page_id)
    tasmota.set_timer(0, /-> tasmota.publish_rule(event_str_out))

    # change current page
    self._hm.lvh_page_cur_idx = self._page_id

    if (anim == 0)
      lv.screen_load(self._lv_scr)
    else    # animation
      var anim_lvgl = self.show_anim.find(anim, lv.SCR_LOAD_ANIM_NONE)
      # load new screen with animation, no delay, 500ms transition time, no auto-delete
      lv.screen_load_anim(self._lv_scr, anim_lvgl, duration, 0, false)
    end
  end
end

#################################################################################
#
# class `HASPmota` to initialize the HASPmota parsing
#
#################################################################################

# main class controller, meant to be a singleton and the only externally used class
class HASPmota
  var dark                              # (bool) use dark theme?
  var hres, vres                        # (int) resolution
  var scr                               # (lv_obj) default LVGL screen
  var r16                               # (lv_font) robotocondensed fonts size 16
  # haspmota objects
  var lvh_pages                         # (list of lvg_page) list of pages
  var lvh_page_cur_idx                  # (int) current page index number
  var lvh_page_cur_idx_parsing          # (int) index of the current page related to parsing JSONL, can be different from the displayed page
  # regex patterns
  var re_page_target                    # compiled regex for action `p<number>`
  # specific event_cb handling for less memory usage since we are registering a lot of callbacks
  var event                             # try to keep the event object around and reuse it
  var event_cb                          # the low-level callback for the closure to be registered

  # assign lvh_page to a static attribute
  static lvh_root = lvh_root
	static lvh_obj = lvh_obj
  static lvh_fixed = lvh_fixed
  static lvh_flex = lvh_flex
  static lvh_page = lvh_page
  static lvh_scr = lvh_scr
  # assign all classes as static attributes
  static lvh_btn = lvh_btn
  static lvh_switch = lvh_switch
  static lvh_checkbox = lvh_checkbox
  static lvh_label = lvh_label
 	# static lvh_led = lvh_led
	static lvh_spinner = lvh_spinner
	static lvh_line = lvh_line
	static lvh_img = lvh_img
	static lvh_dropdown = lvh_dropdown
  static lvh_dropdown_list = lvh_dropdown_list
	static lvh_roller = lvh_roller
	static lvh_btnmatrix = lvh_btnmatrix
 	static lvh_msgbox = lvh_msgbox
 	# static lvh_tabview = lvh_tabview
 	# static lvh_tab = lvh_tab
 	static lvh_cpicker = lvh_cpicker
	static lvh_bar = lvh_bar
	static lvh_slider = lvh_slider
	static lvh_arc = lvh_arc
 	# static lvh_linemeter = lvh_linemeter
 	# static lvh_gauge = lvh_gauge
	# static lvh_textarea = lvh_textarea    # additional?
  static lvh_led = lvh_led
  static lvh_scale = lvh_scale
  static lvh_scale_section = lvh_scale_section
  static lvh_scale_line = lvh_scale_line
  static lvh_spangroup = lvh_spangroup
  static lvh_span = lvh_span
  static lvh_qrcode = lvh_qrcode
  # special cases
  static lvh_chart = lvh_chart

  static def_templ_name = "pages.jsonl" # default template name

  def init()
    self.fix_lv_version()
    import re
    self.re_page_target = re.compile("p\\d+")
    # nothing to put here up to now
  end

  # make sure that `lv.version` returns a version number
  static def fix_lv_version()
    import introspect
    var v = introspect.get(lv, "version")
    # if `lv.version` does not exist, v is `module('undefined')`
    if type(v) != 'int'  lv.version = 8 end
  end

  #====================================================================
  # init
  #
  # arg1: (bool) use dark theme if `true`
  #
  # implicitly loads `pages.jsonl` from file-system // TODO allow to specicify file name
  #====================================================================
  def start(dark, templ_name)
    import path
    if templ_name == nil   templ_name = self.def_templ_name end
    if !path.exists(templ_name)
      raise "io_erorr", "file '" + templ_name + "' not found"
    end
    # start lv if not already started. It does no harm to call lv.start() if LVGL was already started
    lv.start()

    self.dark = bool(dark)

    self.hres = lv.get_hor_res()       # ex: 320
    self.vres = lv.get_ver_res()       # ex: 240
    self.scr = lv.scr_act()            # LVGL default screean object

    try
      self.r16 = lv.font_embedded("robotocondensed", 16)  # TODO what if does not exist
    except ..
      self.r16 = lv.font_embedded("montserrat", 14)  # TODO what if does not exist
    end

    # set the theme for HASPmota
    var th2 = lv.theme_haspmota_init(0, lv.color(0xFF00FF), lv.color(0x303030), self.dark, self.r16)
    self.scr.get_disp().set_theme(th2)
    self.scr.set_style_bg_color(self.dark ? lv.color(0x000000) : lv.color(0xFFFFFF),0)    # set background to white
    # apply theme to layer_top, but keep it transparent
    lv.theme_apply(lv.layer_top())
    lv.layer_top().set_style_bg_opa(0,0)
    
    self.lvh_pages = {}
    # load from JSONL
    self._load(templ_name)
  end

  #################################################################################
  # Simple insertion sort - sorts the list in place, and returns the list
  #################################################################################
  static def sort(l)
    # insertion sort
    for i:1..size(l)-1
      var k = l[i]
      var j = i
      while (j > 0) && (l[j-1] > k)
        l[j] = l[j-1]
        j -= 1
      end
      l[j] = k
    end
    return l
  end


  #####################################################################
  # General Setters and Getters
  #####################################################################
  
  #====================================================================
  # return the current page as `lvh_page` object
  #====================================================================
  def get_page_cur()
    return self.lvh_pages[self.lvh_page_cur_idx]
  end
  #====================================================================
  # return an array of all pages numbers
  #====================================================================
  def get_pages()
    return self.pages_list_sorted(nil)
  end
  #====================================================================
  # return the current page being parsed with JSONL as `lvh_page` object
  #====================================================================
  def get_page_cur_parsing()
    return self.lvh_pages[self.lvh_page_cur_idx_parsing]
  end

  #====================================================================
  # load JSONL template
  #====================================================================
  def _load(templ_name)
    import string
    import json

    var f = open(templ_name,"r")
    var f_content =  f.read()
    f.close()
    
    var jsonl = string.split(f_content, "\n")
    f = nil   # allow deallocation
    f_content = nil

    # parse each line
    while size(jsonl) > 0
      var jline = json.load(jsonl[0])

      if type(jline) == 'instance'
        if tasmota.loglevel(4)
          tasmota.log(f"HSP: parsing line '{jsonl[0]}'", 4)
        end
        self.parse_page(jline)    # parse page first to create any page related objects, may change self.lvh_page_cur_idx_parsing
        # objects are created in the current page
        if (self.lvh_pages == nil)
          raise "value_error", "no page 'id' defined"
        end
        self.parse_obj(jline, self.lvh_pages[self.lvh_page_cur_idx_parsing])    # then parse object within this page
      else
        # check if it's invalid json
        if size(string.tr(jsonl[0], " \t", "")) > 0
          tasmota.log(f"HSP: invalid JSON line '{jsonl[0]}'", 2)
        end
      end
      jline = nil
      jsonl.remove(0)
    end
    jsonl = nil     # make all of it freeable

    # current page is always 1 when we start
    var pages_sorted = self.pages_list_sorted(nil)            # nil for full list
    if (size(pages_sorted) == 0)
      raise "value_error", "no page object defined"
    end
    self.lvh_page_cur_idx = pages_sorted[0]
    self.lvh_pages[self.lvh_page_cur_idx].show(0, 0)          # show first page
  end

  #====================================================================
  #  `parse`
  #
  # Manually parse a single JSON line, after initial load
  #====================================================================
  def parse(j)
    import json
    var jline = json.load(j)

    if type(jline) == 'instance'
      self.parse_page(jline)    # parse page first to create any page related objects, may change self.lvh_page_cur_idx_parsing
      # objects are created in the current page
      self.parse_obj(jline, self.lvh_pages[self.lvh_page_cur_idx])    # then parse object within this page
    else
      raise "value_error", "unable to parse JSON line"
    end
  end

  #====================================================================
  #  `pages_list_sorted`
  #
  # Return the sorted list of page (ids without page 0) starting
  # from the current page.
  # Ex: if pages are [0,1,3,4,5,6]
  #   pages_list_sorted(4) -> [4,5,6,1,3]
  #
  # Arg1: number of current page, or `0` for current page, or `nil` to return just the list of pages
  # Returns: list of ints, or nil if current page is not found
  #====================================================================
  def pages_list_sorted(cur_page)
    # get list of pages as sorted array
    var pages = []
    if (cur_page == 0)    cur_page = self.lvh_page_cur_idx end
    for p: self.lvh_pages.keys()
      if p != 0   pages.push(p) end   # discard page 0
    end
    pages = self.sort(pages)
    if (cur_page == nil)    return pages end

    var count_pages = size(pages)                       # how many pages are defined
    pages = pages + pages                               # double the list to splice it
    var cur_idx = pages.find(cur_page)
    if cur_idx == nil     return nil end                # internal error, current page not found

    pages = pages[cur_idx .. cur_idx + count_pages - 1] # splice the list

    return pages
  end

  #====================================================================
  #  `page_dir_to`
  #
  #   Compute the best direction (right or left) to go from
  #   the current page to the destination page
  #
  #  Returns:
  #    1: scroll to the next page (right)
  #    0: unknown
  #   -1: scroll to the prev page (left)
  #   -2: scroll to the home page (up or left)
  #====================================================================
  def page_dir_to(to_page)
    var sorted_pages_list = self.pages_list_sorted(0)    # list of pages sorted by number, page 0 excluded
    if sorted_pages_list == nil   return 0 end

    var count_pages = size(sorted_pages_list)           # how many pages are possible
    if count_pages <= 1   return 0 end
    # if we have 2 pages, then only 1 direction is possible
    if count_pages == 2   return 1 end
    # we have at least 3 pages

    var to_page_idx = sorted_pages_list.find(to_page)   # find index of target page
    if to_page_idx == nil return 0 end                  # target page not found
    if to_page_idx <= (count_pages + 1) / 2
      return 1
    else
      return -1
    end
  end

  #====================================================================
  #  Execute a page changing action from `action` attribute
  #
  #  This is called in async mode after a button callback
  #
  #  Arg1: lvh_X object that fired the action
  #  Arg2: LVGL event fired
  #  Returns: nil
  #====================================================================
  def do_action(lvh_object, event_code)
    if event_code != lv.EVENT_CLICKED    return end
    self.page_show(lvh_object._action)
  end

  #====================================================================
  #  Execute a page changing action from string `action`
  #
  #  Arg1 `action` can be `prev`, `next`, `back` or `p<number>`
  #       of `delete` if we are deleting the current page
  #  duration: in ms, default 500 ms
  #  anim: -1 right to left, 1 left to right (default), `nil` auto, 0 none
  #  Returns: the target page object if changed, or `nil` if still on same page
  #====================================================================
  def page_show(action, anim, duration)
    # resolve between page numbers
    # p1 is either a number or nil (stored value)
    # p2 is the default value
    # l is the list of page ids
    def to_page_resolve(p1, p_def, l)
      if (p1 != nil) && (l.find(p1) != nil)
        return p1
      else
        return p_def
      end
    end
    # action can be `prev`, `next`, `back`, or `p<number>` like `p1`
    var to_page = nil
    var cur_page = self.get_page_cur()
    var sorted_pages_list =  self.pages_list_sorted(self.lvh_page_cur_idx)

    if size(sorted_pages_list) <= 1     # if only 1 page, do nothing
      return nil
    end

    # handle prev/next/back values
    # get the corresponding value from page object,
    # if absent, revert to next page, previous page and page 1
    # print("sorted_pages_list",sorted_pages_list)
    if action == 'prev'
      to_page = to_page_resolve(int(cur_page.prev), sorted_pages_list[-1], sorted_pages_list)
    elif action == 'next'
      to_page = to_page_resolve(int(cur_page.next), sorted_pages_list[1], sorted_pages_list)
    elif action == 'back'
      to_page = to_page_resolve(int(cur_page.back), self.pages_list_sorted(nil)[0], sorted_pages_list)
    elif action == 'delete'
      to_page = to_page_resolve(int(cur_page.back), self.pages_list_sorted(nil)[0], sorted_pages_list)
      if (to_page == cur_page.id())
        to_page = to_page_resolve(int(cur_page.next), sorted_pages_list[1], sorted_pages_list)
      end
    elif self.re_page_target.match(action)
      # action is supposed to be `p<number>` format
      to_page = to_page_resolve(int(action[1..-1]), nil #-default to nil-#, sorted_pages_list)
    end

    # print(f"{action=} {to_page=}")
    if (to_page != nil) && (to_page > 0)                          # we have a target
      var to_page_obj = self.lvh_pages[to_page]
      # print(f"{to_page_obj.id()=}")
      if (to_page_obj != nil)
        to_page_obj.show(anim, duration)
      end
      return to_page_obj
    end
  end

  #====================================================================
  #  Parse page information
  #
  #  Create a new page object if required
  #  Change the active page
  #====================================================================
  def parse_page(jline)
    if jline.has("page") && type(jline["page"]) == 'int'
      var page = int(jline["page"])
      # print(f">>> parsing page {page}")
      self.lvh_page_cur_idx_parsing = page    # change current page
      if (self.lvh_page_cur_idx == nil)       # also set current page if we haven't any yet
        self.lvh_page_cur_idx = page
      end

      # create the page object if it doesn't exist already
      if !self.lvh_pages.contains(page)
        var lvh_page_class = self.lvh_page
        self.lvh_pages[page] = lvh_page_class(page, self)
      end

      # check if there is "id":0
      if jline.find("id") == 0
        var lvh_page_cur = self.get_page_cur_parsing()
        lvh_page_cur.prev = int(jline.find("prev", nil))
        lvh_page_cur.next = int(jline.find("next", nil))
        lvh_page_cur.back = int(jline.find("back", nil))
      end
    end
  end

  #====================================================================
  #  Remove page by id
  #
  #  Should not be called directly. Indirectly called by `p<x>.delete`
  #
  #  Only removes reference to the page at root level
  #  Change the active page if needed
  #====================================================================
  def _remove_page(page_id)
    # check if we remove the active page
    var cur_page_id = self.get_page_cur().id()
    if (page_id == cur_page_id)
      # if we try to delete the current page, move do main page
      var to_page_obj = self.page_show("delete", 0, 0 #-no animation-#)    # get the target page as result
      if (to_page_obj == nil)                       # we didn't change page
        return
      end
    end
    # also update lvh_page_cur_idx_parsing, if we removed the current parsing page
    if (self.lvh_page_cur_idx_parsing == page_id)
      self.lvh_page_cur_idx_parsing = self.lvh_page_cur_idx
    end
    # remove object from page object
    if self.lvh_pages.contains(page_id)
      self.lvh_pages.remove(page_id)
    end
    # remove global for page
    global.(f"p{page_id}") = nil
  end

  #====================================================================
  # Event CB handling
  #====================================================================
  def register_event(lvh_obj, event_type)
    import cb
    import introspect

    # create the callback to the closure only once
    if self.event_cb == nil
      self.event_cb = cb.gen_cb(/ event_ptr_i -> self.event_dispatch(event_ptr_i))
    end
    # register the C callback
    var lv_obj = lvh_obj._lv_obj
    # we pass the cb as a comptr so it's already a C pointer
    lv_obj.add_event_cb(self.event_cb, event_type, introspect.toptr(lvh_obj))
  end

  def event_dispatch(event_ptr_i)
    import introspect
    var event_ptr = introspect.toptr(event_ptr_i)   # convert to comptr, because it was a pointer in the first place

    if self.event   self.event._p = event_ptr
    else            self.event = lv.lv_event(event_ptr)
    end

    var user_data = self.event.get_user_data()            # it is supposed to be a pointer to the object
    if int(user_data) != 0
      var target_lvh_obj = introspect.fromptr(user_data)
      if type(target_lvh_obj) == 'instance'
        # print("CB Fired", self.event.code, target_lvh_obj)
        target_lvh_obj.event_cb(self.event)
        # print("CB Fired After")
      end
    end
  end

  #====================================================================
  # Parse single object
  #
  # The object may be pre-existing or brand new
  #====================================================================
  def parse_obj(jline, page)
    import global
    import introspect

    var obj_id = int(jline.find("id"))        # id number or nil
    var obj_type = jline.find("obj")          # obj class or nil
    obj_type = (obj_type != nil) ? str(obj_type) : nil
    var lvh_page_cur = self.get_page_cur_parsing()    # current page object, cannot be nil

    # Step 1. Check the id for valid range
    # 'obj_id' must be between 1 and 254
    if (obj_id != nil) && (obj_id < 0 || obj_id > 254)
      if (obj_id != 0) || (obj_type == nil)
        # if `obj_id` is not `nil` and not `0`, it must have `obj_type` not set to `nil`
        print(f"HSP: invalid 'id': {obj_id} for 'obj': {obj_type}")
        return
      end
    end

    # Step 2. Check if the p<>b<> object already exists
    # `prev_obj` contains the pre-existing object, or `nil` if we create a new object
    var obj_lvh = lvh_page_cur.get_obj(obj_id)   # get reference of object or `nil` if new object

    # Step 3. Create object instance if required
    if (obj_type != nil) && (obj_id != nil) && (obj_lvh == nil)

      # Step 3.a. extract the LVGL parent object to create the object in the appropriate lvgl screen
      # Result in `parent_lvgl`

      # extract haspmota class, prefix with `lvh_`. Ex: `btn` becomes `lvh_btn`
      var parent_id = int(jline.find("parentid"))   # id of parent object, or `nil`
      var parent_obj                                # parent HASPmota object
      var parent_lvgl                               # lvgl object of parent object

      if parent_id != nil
        parent_obj = lvh_page_cur.get_obj(parent_id)        # get parent object
        if parent_obj != nil
          parent_lvgl = parent_obj._lv_obj
        end  # parent 
      end
      if parent_lvgl == nil
        parent_lvgl = lvh_page_cur.get_scr()   # if not parent, use the current page screen
      end

      # Step 3.b. Get the HASPmota class object for the `obj` class
      # check if a class with the requested name exists
      # first look for a class with name `lvh_<name>` exists
      var obj_class = introspect.get(self, "lvh_" + obj_type)
      var lv_instance             # allows to pre-instanciate the object

      # Step 3.c. if no native `lvh_<obj>` is found, try the class name from the global namespace
      if obj_class == nil
        # if not found, check if a LVGL class with name `lv_<name>` exists
        var lv_cl = introspect.get(global, obj_type)
        if (lv_cl != nil) && (type(lv_cl) == 'class')
          lv_instance = lv_cl(parent_lvgl)
          obj_class = self.lvh_obj           # use the basic lvh_obj component to encapsulate
        end
      end

      # Step 3.d. if not found, try to load a module with the name of the class
      if obj_class == nil
        var lv_cl = introspect.module(obj_type)
        if lv_cl != nil && type(lv_cl) == 'class'
          lv_instance = lv_cl(parent_lvgl)
          obj_class = self.lvh_obj           # use the basic lvh_obj component to encapsulate
        end
      end

      # Step 3.e. if none found, raise an error and abort
      if obj_class == nil
        print(f"HSP: Cannot find object of type {obj_type}")
        return
      end

      # Step 3.f. instanciate the object, passing the lvgl screen as parent object
      obj_lvh = obj_class(parent_lvgl, page, jline, lv_instance, parent_obj)

      # Step 3.g. Add object to page object
      lvh_page_cur.add_obj(obj_id, obj_lvh)
    end

    # Step 4. if "id" is 0, get the screen object
    if obj_id == 0
      if (obj_type != nil)
        print(f"HSP: cannot specify 'obj':'{obj_type}' for 'id':0")
        return
      end
      obj_lvh = self.get_page_cur_parsing().get_obj(0)   # get object id '0'
    end

    # Step 5. apply attributes
    # set attributes
    # try every attribute, if not supported it is silently ignored
    if (obj_lvh != nil)
      for k:jline.keys()
        obj_lvh.(k) = jline[k]
      end
    end

    # Step 6. apply post-config
    # finally call 'post_config()' when all attributes are set, which gives an opportunity to clean or refresh
    if (obj_lvh != nil)
      obj_lvh.post_config()
    end

    # Step 7. run any Berry code embedded
    # `func_compiled` contains compiled code, that will be run once the object is complete, or `nil` if no code
    # `berry_run` contains the actual source code, used only for logging
    var func_compiled
    var berry_run = str(jline.find("berry_run"))
    if berry_run != "nil"
      try
        func_compiled = compile(berry_run)
      except .. as e,m
        print(format("HSP: unable to compile berry code \"%s\" - '%s' - %s", berry_run, e, m))
      end
    end
    if func_compiled != nil
      try
        # run the compiled code once
        var f_ret = func_compiled()
        if type(f_ret) == 'function'
          f_ret(obj_lvh)
        end
      except .. as e,m
        print(format("HSP: unable to run berry code \"%s\" - '%s' - %s", berry_run, e, m))
      end
    end

  end
end
haspmota.HASPmota = HASPmota

#################################################################################
# General module initilization
#################################################################################

# automatically instanciate the HASPmota() monad
# note: value is cached in the module cache
#  and is returned whenever you call `import haspmota` again
# This means that the object is never garbage collected
#
haspmota.init = def (m)         # `init(m)` is called during first `import haspmota`
  var hm = m.HASPmota
  return hm()
end

#@ solidify:haspmota,weak
global.haspmota = haspmota
return haspmota
