# HASPMota - OpenHASP compatibility module
#
# use `import haspmota` and set the JSONL definitions in `pages.jsonl`
#
# As an optimization `0 #- lv.PART_MAIN | lv.STATE_DEFAULT -#` is replaced with `0`
#
# rm haspmota.tapp; zip -j -0 haspmota.tapp haspmota_core/*
#################################################################################
# How to solidify (needs an ESP32 with PSRAM)
#-

import path 
path.remove("haspmota.bec")
import solidify
var haspmota
load('haspmota.be')

var classes = [
  "page", "obj", "scr",
  "btn", "switch", "checkbox",
  "label", "spinner", "line", "img", "roller", "btnmatrix",
  "bar", "slider", "arc", "textarea", "dropdown",
  "qrcode"
]
var f = open("haspmota.c", "w")
for c:classes
  solidify.dump(haspmota.HASPmota.("lvh_"+c), true, f)
end
solidify.dump(haspmota, true, f)
f.close()
print("Ok")

-#
var haspmota = module("haspmota")

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
class lvh_obj
  static var _lv_class = lv.obj     # _lv_class refers to the lvgl class encapsulated, and is overriden by subclasses
  static var _lv_part2_selector     # selector for secondary part (like knob of arc)

  # attributes to ignore when set at object level (they are managed by page)
  static var _attr_ignore = [
    "tostring",    # avoid issues with Berry `tostring` method
    # "id",
    "obj",
    "page",
    "comment",
    "parentid",
    "auto_size",    # TODO not sure it's still needed in LVGL8
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
    "x": "x",
    "y": "y",
    "w": "width",
    "h": "height",
    # arc
    # "asjustable": nil,
    # "mode": nil,
    "start_angle": "bg_start_angle",
    "start_angle1": "start_angle",
    "end_angle": "bg_end_angle",
    "end_angle1": "end_angle",
    "radius": "style_radius",
    "border_side": "style_border_side",
    "border_width": "style_border_width",
    "border_color": "style_border_color",
    # "line_width": nil,                      # depends on class
    # "line_width1": nil,                     # depends on class
    # "action": nil,                          # store the action in self.action
    # "hidden": nil,                          # apply to self
    # "enabled": nil,                         # apply to self
    # "click": nil,                           # synonym to enabled
    # "toggle": nil,
    "bg_color": "style_bg_color",
    "bg_opa": "style_bg_opa",
    "bg_grad_color": "style_bg_grad_color",
    "bg_grad_dir": "style_bg_grad_dir",
    "line_color": "style_line_color",
    "pad_left": "style_pad_left",
    "pad_right": "style_pad_right",
    "pad_top": "style_pad_top",
    "pad_bottom": "style_pad_bottom",
    "pad_all": "style_pad_all",             # write-only
    # "type": nil,
    # below automatically create a sub-label
    # "text": nil,                            # apply to self
    # "value_str": nil,                       # synonym to 'text'
    # "align": nil,
    # "text_font": nil,
    # "value_font": nil,                      # synonym to text_font
    # "text_color": nil,
    # "value_color": nil,                     # synonym to text_color
    # "value_ofs_x": nil,
    # "value_ofs_y": nil,
    #
    # "min": nil,
    # "max": nil,
    # "val": nil,
    "rotation": "rotation",
    # img
    "src": "src",
    "image_recolor": "style_img_recolor",
    "image_recolor_opa": "style_img_recolor_opa",
    # spinner
    # "angle": nil,
    # "speed": nil,
    # padding of knob
    # "pad_top2": nil,
    # "pad_bottom2": nil,
    # "pad_left2": nil,
    # "pad_right2": nil,
    # "pad_all2": nil,
    # "radius2": nil,
    # rule based update of attributes
    # supporting both `val` and `text`
    # "val_rule": nil,
    # "val_rule_formula": nil,
    # "text_rule": nil,
    # "text_rule_formula": nil,
    # "text_rule_format": nil,
    # "meta": nil,
    # roller
    # "options": nil,
    # qrcode
    # "qr_size": nil,
    # "qr_dark_color": nil,
    # "qr_light_color": nil,
    # "qr_text": nil,
  }

  #====================================================================
  # Instance variables
  var id                                    # (int) object hasp id
  var _lv_obj                               # native lvgl object
  var _lv_label                             # sub-label if exists
  var _page                                 # parent page object
  var _action                               # value of the HASPmota `action` attribute, shouldn't be called `self.action` since we want to trigger the set/member functions
  var _meta                                 # free form metadata

  #====================================================================
  # Rule engine to map value and text to rules
  # hence enabling auto-updates ob objects
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
  static def is_color_attribute(t)
    import re
    return bool(re.search("color$", str(t)))
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
      s = string.toupper(s)   # turn to uppercase
      var val = 0
      for i:0..size(s)-1
        var c = s[i]
        # var c_int = string.byte(c)
        if c == "#"             continue end  # skip '#' prefix if any
        if c == "x" || c == "X" continue end  # skip 'x' or 'X'
    
        if c >= "A" && c <= "F"
          val = (val << 4) | string.byte(c) - 55
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
  # init HASPmota object from its jsonl definition
  #
  # arg1: LVGL parent object (used to create a sub-object)
  # arg2: `jline` JSONL definition of the object from HASPmota template (used in sub-classes)
  # arg3: (opt) LVGL object if it already exists and was created prior to init()
  #====================================================================
  def init(parent, page, jline, obj)
    self._page = page
    if obj == nil && self._lv_class
      var obj_class = self._lv_class    # assign to a var to distinguish from method call
      self._lv_obj = obj_class(parent)  # instanciate LVGL object
    else
      self._lv_obj = obj
    end
    self.post_init()
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
  # get LVGL encapsulated object
  #====================================================================
  def get_obj()
    return self._lv_obj
  end

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
  static _event_map = {
    lv.EVENT_PRESSED:             "down",
    lv.EVENT_CLICKED:             "up",
    lv.EVENT_PRESS_LOST:          "lost",
    lv.EVENT_RELEASED:            "release",
    lv.EVENT_LONG_PRESSED:        "long",
    lv.EVENT_LONG_PRESSED_REPEAT: "hold",
    lv.EVENT_VALUE_CHANGED:       "changed",
  }
  def register_event_cb()
    var oh = self._page._oh
    for ev:self._event_map.keys()
      oh.register_event(self, ev)
    end
  end

  def event_cb(event)
    # the callback avoids doing anything sophisticated in the cb
    # defer the actual action to the Tasmota event loop
    # print("-> CB fired","self",self,"obj",obj,"event",event.tomap(),"code",event.code)
    var oh = self._page._oh         # haspmota global object
    var code = event.code           # materialize to a local variable, otherwise the value can change (and don't capture event object)
    if self.action != "" && code == lv.EVENT_CLICKED
      # if clicked and action is declared, do the page change event
      tasmota.set_timer(0, /-> oh.do_action(self, code))
    end

    var event_hasp = self._event_map.find(code)
    if event_hasp != nil
      import json

      var tas_event_more = ""   # complementary data
      if event.code == lv.EVENT_VALUE_CHANGED
        try
          # try to get the new val
          var val = self.val
          if val != nil   tas_event_more = format(',"val":%s', json.dump(val)) end
          var text = self.text
          if text != nil
            tas_event_more += ',"text":'
            tas_event_more += json.dump(text)
          end
        except ..
        end
      end
      var tas_event = format('{"hasp":{"p%ib%i":{"event":"%s"%s}}}', self._page._page_id, self.id, event_hasp, tas_event_more)
      # print("val=",val)
      tasmota.set_timer(0, /-> tasmota.publish_rule(tas_event))
    end
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
      self._lv_label = lv.label(self.get_obj())
      self._lv_label.set_align(lv.ALIGN_CENTER);
    end
  end
  def set_text(t)
    self.check_label()
    self._lv_label.set_text(str(t))
  end
  def set_value_str(t) self.set_text(t) end
  def get_text()
    if self._lv_label == nil return nil end
    return self._lv_label.get_text()
  end
  def get_value_str() return self.get_text() end

  # mode
  def set_mode(t)
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
  def get_mode()
  end

  #====================================================================
  #  `align`: `left`, `center`, `right`
  #====================================================================
  def set_align(t)
    var align
    self.check_label()
    if t == 0 || t == "left"
      align = lv.TEXT_ALIGN_LEFT
    elif t == 1 || t == "center"
      align = lv.TEXT_ALIGN_CENTER
    elif t == 2 || t == "right"
      align = lv.TEXT_ALIGN_RIGHT
    end
    self._lv_label.set_style_text_align(align, 0 #- lv.PART_MAIN | lv.STATE_DEFAULT -#)
  end

  def get_align()
    if self._lv_label == nil return nil end
    var align self._lv_label.get_style_text_align(0 #- lv.PART_MAIN | lv.STATE_DEFAULT -#)
    if align == lv.TEXT_ALIGN_LEFT
      return "left"
    elif align == lv.TEXT_ALIGN_CENTER
      return "center"
    elif align == lv.TEXT_ALIGN_RIGHT
      return "right"
    else
      return nil
    end
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
    # self.check_label()
    var font
    if type(t) == 'int'
      try
        font = lv.font_embedded("robotocondensed", t)
      except ..
        try
          font = lv.font_embedded("montserrat", t)
        except ..
          return
        end
      end
    elif type(t) == 'string'
      import string
      import re
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
      if re.match(".*\\.ttf$", name)
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
        end
      end
    end
    if font != nil
      self._lv_obj.set_style_text_font(font, 0 #- lv.PART_MAIN | lv.STATE_DEFAULT -#)
    else
      print("HSP: Unsupported font:", t)
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
  def set_value_color(t) self.set_text_color(t) end
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
    self._lv_obj.set_value(t)
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
  # LV_PART_TICKS        = 0x060000,   /**< Ticks on scale e.g. for a chart or meter*/
  # LV_PART_CURSOR       = 0x070000,   /**< Mark a specific place e.g. for text area's cursor or on a chart*/
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
    lv.PART_TICKS,        # 60
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

    # print("> getmember", k)
    var prefix = k[0..3]
    if prefix == "set_" || prefix == "get_" return end    # avoid recursion

    # check if the attribute ends with 2 digits, if so remove the two suffix digits
    var style_modifier = 0
    if size(k) >= 3
      var char_last_1 = string.byte(k[-1])
      var char_last_2 = string.byte(k[-2])
      var suffix_digits = nil
      if (char_last_1 >= 0x30 && char_last_1 <= 0x39 && char_last_2 >= 0x30 && char_last_2 <= 0x39)
        # we extract the last 2 digits
        suffix_digits = int(k[-2..])
        k = k [0..-3]      # remove 2 last digits
      end
      style_modifier = self.digits_to_style(suffix_digits)
    end
    # print(f">>>: getmember {k=} {style_modifier=}")

    # if attribute name is in ignore list, abort
    if self._attr_ignore.find(k) != nil return end

    # first check if there is a method named `get_X()`
    var f = introspect.get(self, "get_" + k)  # call self method
    if type(f) == 'function'
      # print(f">>>: getmember local method get_{k}")
      return f(self, style_modifier)
    end

    # next check if there is a mapping to an LVGL attribute
    if self._attr_map.contains(k)
      var kv = self._attr_map[k]

      f = introspect.get(self._lv_obj, "get_" + kv)
      if type(f) == 'function'                  # found and function, call it
        if string.find(kv, "style_") == 0
          # print(f">>>: getmember style_ method get_{k}")
          # style function need a selector as second parameter
          return f(self._lv_obj, style_modifier)
        else
          # print(f">>>: getmember standard method get_{k}")
          return f(self._lv_obj)
        end
      end
    end

    # fallback to exception if attribute unknown or not a function
    raise "value_error", "unknown attribute " + str(k)
  end

  #- ------------------------------------------------------------#
  # `setmember` virtual setter
  #- ------------------------------------------------------------#
  def setmember(k, v)
    import string
    import introspect

    # print(">>>: setmember", k, v)
    var prefix = k[0..3]
    if prefix == "set_" || prefix == "get_" return end      # avoid infinite loop

    # check if the attribute ends with 2 digits, if so remove the two suffix digits
    var style_modifier = 0
    if size(k) >= 3
      var char_last_1 = string.byte(k[-1])
      var char_last_2 = string.byte(k[-2])
      var suffix_digits = nil
      if (char_last_1 >= 0x30 && char_last_1 <= 0x39 && char_last_2 >= 0x30 && char_last_2 <= 0x39)
        # we extract the last 2 digits
        suffix_digits = int(k[-2..])
        k = k [0..-3]      # remove 2 last digits
      end
      style_modifier = self.digits_to_style(suffix_digits)
    end
    # print(f">>>: setmember {k=} {style_modifier=}")

    # if attribute name is in ignore list, abort
    if self._attr_ignore.find(k) != nil return end

    # first check if there is a method named `set_X()`
    var f = introspect.get(self, "set_" + k)
    if type(f) == 'function'
      # print(f">>>: setmember local method set_{k}")
      f(self, v, style_modifier)
      return
    end

    # next check if there is a mapping to an LVGL attribute
    if self._attr_map.contains(k)
      
      var kv = self._attr_map[k]
      f = introspect.get(self._lv_obj, "set_" + kv)
      # if the attribute contains 'color', convert to lv_color
      if self.is_color_attribute(kv)
        v = self.parse_color(v)
      end
      # print("f=", f, v, kv, self._lv_obj, self)
      if type(f) == 'function'
        try
          if string.find(kv, "style_") == 0
            # print(f">>>: setmember style_ method set_{k}")
            # style function need a selector as second parameter
            f(self._lv_obj, v, style_modifier)
          else
            # print(f">>>: setmember standard method set_{k}")
            f(self._lv_obj, v)
          end
        except .. as e, m
          raise e, m + " for " + k
        end
        return
      else
        print("HSP: Could not find function set_"+kv)
      end
    else
      print("HSP: unknown attribute:", k)
    end
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
  def set_val_rule(t)
    # remove previous rule if any
    if self._val_rule != nil
      tasmota.remove_rule(self._val_rule, self)
    end

    self._val_rule = str(t)
    tasmota.add_rule(self._val_rule, / val -> self.val_rule_matched(val), self)
  end
  def get_val_rule()
    return self._val_rule
  end
  # text_rule
  def set_text_rule(t)
    # remove previous rule if any
    if self._text_rule != nil
      tasmota.remove_rule(self._text_rule, self)
    end

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
#
#  Other widgets
#
#################################################################################

#====================================================================
#  label
#====================================================================
class lvh_label : lvh_obj
  static _lv_class = lv.label
  # label do not need a sub-label
  def post_init()
    self._lv_label = self._lv_obj   # the label is also the object itself
    super(self).post_init()         # call super
  end
end

#====================================================================
#  arc
#====================================================================
class lvh_arc : lvh_obj
  static _lv_class = lv.arc
  static _lv_part2_selector = lv.PART_KNOB

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
end

#====================================================================
#  switch
#====================================================================
class lvh_switch : lvh_obj
  static _lv_class = lv.switch
  static _lv_part2_selector = lv.PART_KNOB
  # map val to toggle
  def set_val(t)
    return self.set_toggle(t)
  end
  def get_val()
    return self.get_toggle()
  end
end

#====================================================================
#  spinner
#====================================================================
class lvh_spinner : lvh_arc
  static _lv_class = lv.spinner
  var _anim_start, _anim_end        # the two raw (lv_anim_ntv) objects used for the animation

  # init
  # - create the LVGL encapsulated object
  # arg1: parent object
  # arg2: json line object
  def init(parent, page, jline)
    self._page = page
    var angle = jline.find("angle", 60)
    var speed = jline.find("speed", 1000)
    self._lv_obj = lv.spinner(parent, speed, angle)
    self.post_init()
    # do some black magic to get the two lv_anim objects used to animate the spinner
    var anim_start = lv.anim_get(self._lv_obj, self._lv_obj._arc_anim_start_angle)
    var anim_end = lv.anim_get(self._lv_obj, self._lv_obj._arc_anim_end_angle)
    # convert to a ctype C structure via pointer
    self._anim_start = lv.anim_ntv(anim_start._p)
    self._anim_end = lv.anim_ntv(anim_end._p)
  end

  def set_angle(t)
    t = int(t)
    self._anim_end.start_value = t
    self._anim_end.end_value = t + 360
  end
  def get_angle()
    return self._anim_end.start_value - self._anim_start.start_value
  end
  def set_speed(t)
    t = int(t)
    self._anim_start.time = t
    self._anim_end.time = t
  end
  def get_speed()
    return self._anim_start.time
  end
end

#====================================================================
#  img
#====================================================================
class lvh_img : lvh_obj
  static _lv_class = lv.img

  def set_angle(v)
    v = int(v)
    self._lv_obj.set_angle(v)
  end
  def get_angle()
    return self._lv_obj.get_angle()
  end
end

#====================================================================
#  qrcode
#====================================================================
class lvh_qrcode : lvh_obj
  static _lv_class = lv.qrcode

  # init
  # - create the LVGL encapsulated object
  # arg1: parent object
  # arg2: json line object
  def init(parent, page, jline)
    self._page = page

    var sz = jline.find("qr_size", 100)
    var dark_col = self.parse_color(jline.find("qr_dark_color", "#000000"))
    var light_col = self.parse_color(jline.find("qr_light_color", "#FFFFFF"))

    self._lv_obj = lv.qrcode(parent, sz, dark_col, light_col)
    self.post_init()
  end

  # ignore attributes, spinner can't be changed once created
  def set_qr_size(t) end
  def get_qr_size() end
  def set_qr_dark_color(t) end
  def get_qr_dark_color() end
  def set_qr_light_color(t) end
  def get_qr_light_color() end
  def set_qr_text(t)
    t = str(t)
    self._lv_obj.update(t, size(t))
  end
  def get_qr_text() end
end

#====================================================================
#  slider
#====================================================================
class lvh_slider : lvh_obj
  static _lv_class = lv.slider

  def set_val(t)
    self._lv_obj.set_value(t, 0)    # add second parameter - no animation
  end
end

#====================================================================
#  roller
#====================================================================
class lvh_roller : lvh_obj
  static _lv_class = lv.roller

  def set_val(t)
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
#  dropdown
#====================================================================
class lvh_dropdown : lvh_obj
  static _lv_class = lv.dropdown
  static _dir = [ lv.DIR_BOTTOM, lv.DIR_TOP, lv.DIR_LEFT, lv.DIR_RIGHT ] # 0 = down, 1 = up, 2 = left, 3 = right

  def set_val(t)
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
    # 0 = down, 1 = up, 2 = left, 3 = right
    self._lv_obj.set_dir(self._dir[int(t)])
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

#################################################################################
#
# All other subclasses than just map the LVGL object
# and doesn't have any specific behavior
#
#################################################################################
class lvh_bar : lvh_obj         static _lv_class = lv.bar         end
class lvh_btn : lvh_obj         static _lv_class = lv.btn         end
class lvh_btnmatrix : lvh_obj   static _lv_class = lv.btnmatrix   end
class lvh_checkbox : lvh_obj    static _lv_class = lv.checkbox    end
class lvh_line : lvh_obj        static _lv_class = lv.line        end
class lvh_textarea : lvh_obj    static _lv_class = lv.textarea    end
# special case for scr (which is actually lv_obj)
class lvh_scr : lvh_obj         static _lv_class = nil            end    # no class for screen


#################################################################################
#  Class `lvh_page`
#
#  Encapsulates a `lv_screen` which is `lv.obj(0)` object
#################################################################################
#
# ex of transition: lv.scr_load_anim(scr, lv.SCR_LOAD_ANIM_MOVE_RIGHT, 500, 0, false)
class lvh_page
  var _obj_id               # (map) of `lvh_obj` objects by id numbers
  var _page_id              # (int) id number of this page
  var _lv_scr               # (lv_obj) lvgl screen object
  var _oh                   # HASPmota global object
  # haspmota attributes for page are on item `#0`
  var prev, next, back      # (int) id values for `prev`, `next`, `back` buttons

  #====================================================================
  #  `init`
  #
  # arg1: `page_number` (int) HASPmota page id
  #        defaults to `1` if not specified
  #        page 0 is special, visible on all pages. Internally uses `layer_top`
  # arg2: `oh` global HASPmota monad object
  #  page_number: haspmota page number, defaults to `1` if not specified
  #====================================================================
  def init(page_number, oh)
    import global
    self._oh = oh                   # memorize HASPmota parent object

    # if no parameter, default to page #1
    page_number = int(page_number)
    if page_number == nil  page_number = 1 end

    self._page_id = page_number     # remember our page_number
    self._obj_id = {}               # init list of objects

    # initialize the LVGL object for the page
    # uses a lv_scr object except for page 0 where we use layer_top
    # page 1 is mapped directly to the default screen `scr_act`
    if page_number == 1
      self._lv_scr = lv.scr_act()   # default screen
    elif page_number == 0
      self._lv_scr = lv.layer_top() # top layer, visible over all screens
    else
      self._lv_scr = lv.obj(0)      # allocate a new screen
      var bg_color = lv.scr_act().get_style_bg_color(0 #- lv.PART_MAIN | lv.STATE_DEFAULT -#) # bg_color of default screen
      self._lv_scr.set_style_bg_color(bg_color, 0 #- lv.PART_MAIN | lv.STATE_DEFAULT -#) # set white background
    end

    # page object is also stored in the object map at id `0` as instance of `lvg_scr`
    var lvh_scr_class = self._oh.lvh_scr
    var obj_scr = lvh_scr_class(nil, self, nil, self._lv_scr)   # store screen in a virtual object
    self._obj_id[0] = obj_scr

    # create a global for this page of form p<page_number>, ex `p1`
    # create a global for the page attributes as p<page_number>b0, ex `p1b0`
    global.("p" + str(self._page_id)) = self
    global.("p" + str(self._page_id) + "b0") = obj_scr
  end

  #####################################################################
  # General Setters and Getters
  #####################################################################
  
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
  def set_obj(id, o)
    self._obj_id[id] = o
  end
  def get_obj(id)
    return self._obj_id.find(id)
  end

  #====================================================================
  #  `show` transition from one page to another
  #    duration: in ms, default 500 ms
  #    anim: -1 right to left, 1 left to right (default)
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
    if duration == nil  duration = 500 end

    # if anim is `nil` try to guess the direction from current screen
    if anim == nil
      anim = self._oh.page_dir_to(self.id())
    end

    # send page events
    var event_str_in = format('{"hasp":{"p%i":"out"}}', self._oh.lvh_page_cur_idx)
    tasmota.set_timer(0, /-> tasmota.publish_rule(event_str_in))
    var event_str_out = format('{"hasp":{"p%i":"in"}}', self._page_id)
    tasmota.set_timer(0, /-> tasmota.publish_rule(event_str_out))

    # change current page
    self._oh.lvh_page_cur_idx = self._page_id

    var anim_lvgl = self.show_anim.find(anim, lv.SCR_LOAD_ANIM_NONE)
    # load new screen with animation, no delay, 500ms transition time, no auto-delete
    lv.scr_load_anim(self._lv_scr, anim_lvgl, duration, 0, false)
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
  # regex patterns
  var re_page_target                    # compiled regex for action `p<number>`
  # specific event_cb handling for less memory usage since we are registering a lot of callbacks
  var event                             # try to keep the event object around and reuse it
  var event_cb                          # the low-level callback for the closure to be registered

  # assign lvh_page to a static attribute
  static lvh_page = lvh_page
  static lvh_scr = lvh_scr
  # assign all classes as static attributes
  static lvh_btn = lvh_btn
  static lvh_switch = lvh_switch
  static lvh_checkbox = lvh_checkbox
  static lvh_label = lvh_label
 	# static lvh_led = lvh_led
	static lvh_spinner = lvh_spinner
	static lvh_obj = lvh_obj
	static lvh_line = lvh_line
	static lvh_img = lvh_img
	static lvh_dropdown = lvh_dropdown
	static lvh_roller = lvh_roller
	static lvh_btnmatrix = lvh_btnmatrix
 	# static lvh_msgbox = lvh_msgbox
 	# static lvh_tabview = lvh_tabview
 	# static lvh_tab = lvh_tab
 	# static lvh_cpiker = lvh_cpiker
	static lvh_bar = lvh_bar
	static lvh_slider = lvh_slider
	static lvh_arc = lvh_arc
 	# static lvh_linemeter = lvh_linemeter
 	# static lvh_gauge = lvh_gauge
	static lvh_textarea = lvh_textarea    # additional?
  static lvh_qrcode = lvh_qrcode

  static def_templ_name = "pages.jsonl" # default template name

  def init()
    import re
    self.re_page_target = re.compile("p\\d+")
    # nothing to put here up to now
  end

  def deinit()
    # remove previous rule if any
    if self._val_rule != nil
      tasmota.remove_rule(self._val_rule, self)
    end
    if self._text_rule != nil
      tasmota.remove_rule(self._text_rule, self)
    end
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
  # load JSONL template
  #====================================================================
  def _load(templ_name)
    import string
    import json
    #- pages -#
    self.lvh_page_cur_idx = 1
    var lvh_page_class = self.lvh_page 
    self.lvh_pages[1] = lvh_page_class(1, self)   # always create page #1

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
        self.parse_page(jline)    # parse page first to create any page related objects, may change self.lvh_page_cur_idx
        # objects are created in the current page
        self.parse_obj(jline, self.lvh_pages[self.lvh_page_cur_idx])    # then parse object within this page
      end
      jline = nil
      jsonl.remove(0)
    end
    jsonl = nil     # make all of it freeable

    # current page is always 1 when we start
    self.lvh_page_cur_idx = 1
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
      self.parse_page(jline)    # parse page first to create any page related objects, may change self.lvh_page_cur_idx
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
    if cur_page == 0    cur_page = self.lvh_page_cur_idx end
    for p: self.lvh_pages.keys()
      if p != 0   pages.push(p) end   # discard page 0
    end
    pages = self.sort(pages)
    if cur_page == nil    return cur_page end

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
    var action = lvh_object._action
    var cur_page = self.lvh_pages[self.lvh_page_cur_idx]
    # print("do_action","lvh_object",lvh_object,"action",action,"cur_page",cur_page,self.lvh_page_cur_idx)

    # action can be `prev`, `next`, `back`, or `p<number>` like `p1`
    var to_page = nil
    var sorted_pages_list =  self.pages_list_sorted(self.lvh_page_cur_idx)
    if size(sorted_pages_list) <= 1  return end     # if only 1 page, do nothing
    # handle prev/next/back values
    # get the corresponding value from page object,
    # if absent, revert to next page, previous page and page 1
    # print("sorted_pages_list",sorted_pages_list)
    if action == 'prev'
      to_page = int(cur_page.prev)
      if to_page == nil   to_page = sorted_pages_list[-1] end   # if no prev, take the previous page
    elif action == 'next'
      to_page = int(cur_page.next)
      if to_page == nil   to_page = sorted_pages_list[1] end    # if no next, take the next page
    elif action == 'back'
      to_page = int(cur_page.back)
      if to_page == nil   to_page = 1 end                       # if no nack, take page number 1
    elif self.re_page_target.match(action)
      # action is supposed to be `p<number>` format
      to_page = int(action[1..-1])          # just skip first char and convert the rest to a string
    end

    # print("to_page=",to_page)
    if to_page != nil && to_page > 0                          # we have a target
      self.lvh_pages[to_page].show()        # switvh to the target page
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
      self.lvh_page_cur_idx = page    # change current page

      # create the page object if it doesn't exist already
      if !self.lvh_pages.contains(page)
        var lvh_page_class = self.lvh_page
        self.lvh_pages[page] = lvh_page_class(page, self)
      end

      # check if there is "id":0
      if jline.find("id") == 0
        var lvh_page_cur = self.get_page_cur()
        lvh_page_cur.prev = int(jline.find("prev", nil))
        lvh_page_cur.next = int(jline.find("next", nil))
        lvh_page_cur.back = int(jline.find("back", nil))
      end
    end
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

    if self.event   self.event._change_buffer(event_ptr)
    else            self.event = lv.lv_event(event_ptr)
    end

    var user_data = self.event.user_data            # it is supposed to be a pointer to the object
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
  #====================================================================
  def parse_obj(jline, page)
    import global
    import introspect

    var obj_id = int(jline.find("id"))        # id number or nil
    var obj_type = str(jline.find("obj"))     # obj class or nil
    var obj_lvh                               # lvgl object created
    var lvh_page_cur = self.get_page_cur()    # current page object

    # first run any Berry code embedded
    var berry_run = str(jline.find("berry_run"))
    var func_compiled
    if berry_run != "nil"
      try
        func_compiled = compile(berry_run)
      except .. as e,m
        print(format("HSP: unable to compile berry code \"%s\" - '%s' - %s", berry_run, e, m))
      end
    end

    # if line contains botn 'obj' and 'id', create the object
    if obj_id == nil return end               # if no object id, ignore line
    if obj_type != "nil" && obj_id != nil
      # 'obj_id' must be between 1 and 254
      if obj_id < 1 || obj_id > 254
        print("HSP: invalid 'id': " + str(obj_id) + " for 'obj':" + obj_type)
        return
      end

      # extract haspmota class, prefix with `lvh_`. Ex: `btn` becomes `lvh_btn`
      # extract parent
      var parent_lvgl
      var parent_id = int(jline.find("parentid"))

      if parent_id != nil
        var parent_obj = lvh_page_cur.get_obj(parent_id)        # get parent object
        if parent_obj != nil   parent_lvgl = parent_obj._lv_obj end  # parent 
      end
      if parent_lvgl == nil
        parent_lvgl = lvh_page_cur.get_scr()   # if not parent, use the current page screen
      end

      # check if a class with the requested name exists
      # first look for a class with name `lvh_<name>` exists
      var obj_class = introspect.get(self, "lvh_" + obj_type)
      var lv_instance = nil     # allows to pre-instanciate the object

      # there is no lvh_X class, try to load the class name from the global namespace
      if obj_class == nil
        # if not found, check if a LVGL class with name `lv_<name>` exists
        var lv_cl = introspect.get(global, obj_type)
        if lv_cl != nil && type(lv_cl) == 'class'
          lv_instance = lv_cl(parent_lvgl)
          obj_class = self.lvh_obj           # use the basic lvh_obj component to encapsulate
        end
      end

      # still not found, try to load a module with the name of the class
      if obj_class == nil
        var lv_cl = introspect.module(obj_type)
        if lv_cl != nil && type(lv_cl) == 'class'
          lv_instance = lv_cl(parent_lvgl)
          obj_class = self.lvh_obj           # use the basic lvh_obj component to encapsulate
        end
      end

      if obj_class == nil
        print("HSP: cannot find object of type " + str(obj_type))
        return
      end
      
      # instanciate the object, passing the lvgl screen as parent object
      obj_lvh = obj_class(parent_lvgl, page, jline, lv_instance)

      # add object to page object
      lvh_page_cur.set_obj(obj_id, obj_lvh)
      
      # create a global variable for this object of form p<page>b<id>, ex p1b2
      var glob_name = format("p%ib%i", lvh_page_cur.id(), obj_id)
      global.(glob_name) = obj_lvh

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

    if obj_id == 0 && obj_type != "nil"
      print("HSP: cannot specify 'obj' for 'id':0")
      return
    end
    
    # if id==0, retrieve the 'scr' object of the current page
    if obj_id == 0
      obj_lvh = self.get_page_cur().get_obj(0)   # get object id '0'
    end

    # set attributes
    # try every attribute, if not supported it is silently ignored
    for k:jline.keys()
      # introspect.set(obj, k, jline[k])
      obj_lvh.(k) = jline[k]
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
  var oh = m.HASPmota
  return oh()
end

global.haspmota = haspmota
return haspmota
