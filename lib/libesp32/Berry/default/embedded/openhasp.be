import string
import json

# lv.start()
# scr = lv.scr_act()            # default screean object
# scr.set_style_bg_color(lv.color(0x0000A0), lv.PART_MAIN | lv.STATE_DEFAULT)

lv.start()

hres = lv.get_hor_res()       # should be 320
vres = lv.get_ver_res()       # should be 240

scr = lv.scr_act()            # default screean object
#f20 = lv.montserrat_font(20)  # load embedded Montserrat 20
r20 = lv.font_robotocondensed_latin1(20)

th2 = lv.theme_openhasp_init(0, lv.color(0xFF0000), lv.color(0xFFFF00), true, r20)
scr.get_disp().set_theme(th2)
# apply theme to layer_top, but keep it transparent
lv.theme_apply(lv.layer_top())
lv.layer_top().set_style_bg_opa(0,0)


# parse hex string
def parse_hex(s)
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

#- ------------------------------------------------------------
  Class `lvh_obj` encapsulating `lv_obj``

  Provide a mapping for virtual members
  Stores the associated page and object id

  Adds specific virtual members used by OpenHASP
- ------------------------------------------------------------ -#
class lvh_obj
  # _lv_class refers to the lvgl class encapsulated, and is overriden by subclasses
  static _lv_class = lv.obj

  #- mapping from OpenHASP attribute to LVGL attribute -#
  #- if mapping is null, we use set_X and get_X from our own class -#
  static _attr_map = {
    "x": "x",
    "y": "y",
    "w": "width",
    "h": "height",
    "radius": "radius",
    "border_side": "border_side",
    "text": nil,    # apply to self
    "hidden": nil,    # apply to self
    "enabled": nil,    # apply to self
    "toggle": nil,
    "bg_color": nil,
    "align": nil,
  }

  var _lv_obj     # native lvgl object
  var _lv_label   # sub-label if exists

  # init
  # - create the LVGL encapsulated object
  def init(parent)
    var obj_class = self._lv_class    # need to assign to a var to distinguish from method call
    self._lv_obj = obj_class(parent)  # instanciate LVGL object
    self.post_init()
  end

  # post-init, to be overriden
  def post_init()
  end

  # get LVGL encapsulated object
  def get_obj()
    return self._lv_obj
  end

  #- ------------------------------------------------------------
    Mapping of synthetic attributes
    - text
    - hidden
    - enabled
  - ------------------------------------------------------------ -#
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

  #- `enabled` attributes mapped to OBJ_FLAG_CLICKABLE -#
  def set_enabled(h)
    if h
      self._lv_obj.add_flag(lv.OBJ_FLAG_CLICKABLE)
    else
      self._lv_obj.clear_flag(lv.OBJ_FLAG_CLICKABLE)
    end
  end

  def get_enabled()
    return self._lv_obj.has_flag(lv.OBJ_FLAG_CLICKABLE)
  end

  #- `toggle` attributes mapped to STATE_CHECKED -#
  def set_toggle(t)
    if t == "TRUE"  t = true end
    if t == "FALSE" t = false end
    if t
      self._lv_obj.add_state(lv.STATE_CHECKED)
    else
      self._lv_obj.clear_state(lv.STATE_CHECKED)
    end
  end

  def get_toggle()
    return self._lv_obj.has_state(lv.STATE_CHECKED)
  end

  #- set_text: create a `lv_label` sub object to the current object -#
  #- (default case, may be overriden by object that directly take text) -#
  def check_label()
    if self._lv_label == nil
      self._lv_label = lv.label(self.get_obj())
    end
  end
  def set_text(t)
    self.check_label()
    self._lv_label.set_text(t)
  end

  def get_text()
    if self._lv_label == nil return nil end
    return self._lv_label.get_text()
  end

  def set_align(t)
    var align
    self.check_label()

    if t == 0 || t == "left"
      align = lv.TEXT_ALIGN_LEFT
    elif t == 1 || t == "center"
      align = lv.TEXT_ALIGN_CENTER
    elif t == 2 || t == "right"
      align = lv.TEXT_ALIGN_RIGHT
    else
      align = lv.TEXT_ALIGN_auto
    end
    self._lv_label.set_style_text_align(align, lv.PART_MAIN | lv.STATE_DEFAULT)
  end

  def get_align()
    if self._lv_label == nil return nil end
    var align self._lv_label.get_style_text_align(lv.PART_MAIN | lv.STATE_DEFAULT)
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

  def set_bg_color(t)
    var color = lv.color(parse_hex(t))
    self._lv_obj.set_style_bg_color(color, lv.PART_MAIN | lv.STATE_DEFAULT)
  end

  def get_bg_color()
    return self._lv_obj.get_style_bg_color(lv.PART_MAIN | lv.STATE_DEFAULT)
  end

  #- ------------------------------------------------------------
    Mapping of virtual attributes
  - ------------------------------------------------------------ -#
  def member(k)
    # tostring is a special case, we shouldn't raise an exception for it
    if k == 'tostring' return nil end
    #
    if self._attr_map.has(k)
      import introspect
      if self._attr_map[k]
        var f = introspect.get(self._lv_obj, "get_" + self._attr_map[k])
        if type(f) == 'function'
          return f(self._lv_obj)
        end
      else
        # call self method
        var f = introspect.get(self, "get_" + k)
        if type(f) == 'function'
          return f(self, k)
        end
      end
    end
    raise "value_error", "unknown attribute " + str(k)
  end

  def setmember(k, v)
    # print(">> setmember", k, v)
    # print(">>", classname(self), self._attr_map)
    if self._attr_map.has(k)
      import introspect
      if self._attr_map[k]
        var f = introspect.get(self._lv_obj, "set_" + self._attr_map[k])
        # print("f=", f)
        if type(f) == 'function'
          f(self._lv_obj, v)
          return
        end
      else
        # call self method
        var f = introspect.get(self, "set_" + k)
        # print("f==",f)
        if type(f) == 'function'
          f(self, v)
          return
        end
      end
      
    end
    # silently ignore if the attribute name is not supported
  end
end

class lvh_label : lvh_obj
  static _lv_class = lv.label
  # label do not need a sub-label
  def post_init()
    self._lv_label = self._lv_obj
    # label don't have opaque background by default
    self._lv_obj.set_style_bg_opa(lv.OPA_COVER, lv.PART_MAIN | lv.STATE_DEFAULT)
  end
end

#- creat sub-classes of lvh_obj and map the LVGL class in static '_lv_class' attribute -#
class lvh_arc : lvh_obj       static _lv_class = lv.arc end
class lvh_bar : lvh_obj       static _lv_class = lv.bar end
class lvh_btn : lvh_obj       static _lv_class = lv.btn end
class lvh_btnmatrix : lvh_obj static _lv_class = lv.btnmatrix end
class lvh_checkbox : lvh_obj  static _lv_class = lv.checkbox end
class lvh_dropdown : lvh_obj  static _lv_class = lv.dropdown end
class lvh_img : lvh_obj       static _lv_class = lv.img end
class lvh_line : lvh_obj      static _lv_class = lv.line end
class lvh_roller : lvh_obj    static _lv_class = lv.roller end
class lvh_slider : lvh_obj    static _lv_class = lv.slider end
class lvh_slider : lvh_obj    static _lv_class = lv.slider end
class lvh_switch : lvh_obj    static _lv_class = lv.switch end
class lvh_textarea : lvh_obj  static _lv_class = lv.textarea end

#- ----------------------------------------------------------------------------
  Class `lvh_page` encapsulating `lv_obj` as screen (created with lv.obj(0))
- ----------------------------------------------------------------------------- -#
# ex of transition: lv.scr_load_anim(scr, lv.SCR_LOAD_ANIM_MOVE_RIGHT, 500, 0, false)
class lvh_page
  var _obj_id       # (map) of objects by id numbers
  var _page_id      # (int) id number of the page
  var _lv_scr       # (lv_obj) lvgl screen object

  #- init(page_number) -#
  def init(page_number)
    import global

    # if no parameter, default to page #1
    if page_number == nil  page_number = 1 end

    self._page_id = page_number     # remember our page_number
    self._obj_id = {}               # init list of objects
    if page_number == 1
      self._lv_scr = lv.scr_act()   # default screen
    elif page_number == 0
      self._lv_scr = lv.layer_top() # top layer, visible over all screens
    else
      self._lv_scr = lv.obj(0)      # allocate a new screen
      self._lv_scr.set_style_bg_color(lv_color(0x000000), lv.PART_MAIN | lv.STATE_DEFAULT) # set black background
    end

    # create a global for this page of form p<page_number>, ex p1
    var glob_name = string.format("p%i", self._page_id)
    global.(glob_name) = self
  end

  #- retrieve lvgl screen object for this page -#
  def get_scr()
    return self._lv_scr
  end

  #- add an object to this page -#
  def set_obj(id, o)
    self._obj_id[id] = o
  end

  #- return id of this page -#
  def id()
    return self._page_id
  end

  #- show this page, with animation -#
  def show(anim, duration)
    # ignore if there is no screen, like for id 0
    if self._lv_scr == nil return nil end
    # ignore if the screen is already active
    if self._lv_scr._p == lv.scr_act()._p return end    # do nothing

    # default animation is lv.SCR_LOAD_ANIM_MOVE_RIGHT
    if anim == nil  anim = lv.SCR_LOAD_ANIM_MOVE_RIGHT end
    # default duration of 500ms
    if duration == nil  duration = 500 end

    # load new screen with anumation, no delay, 500ms transition time, no auto-delete
    lv.scr_load_anim(self._lv_scr, lv.SCR_LOAD_ANIM_MOVE_RIGHT, duration, 0, false)
  end
end

#- pages -#
var lvh_page_cur = lvh_page(1)
var lvh_pages = { 1: lvh_page_cur }   # always create page #1

f = open("pages.jsonl","r")
var jsonl = string.split(f.read(), "\n")
f.close()

#- ------------------------------------------------------------
  Parse page information

  Create a new page object if required
  Change the active page
- ------------------------------------------------------------ -#
def parse_page(jline)
  if jline.has("page") && type(jline["page"]) == 'int'
    var page = int(jline["page"])
    # does the page already exist?
    if lvh_pages.has(page)
      # yes, just change the current page
      lvh_page_cur = lvh_pages[page]
    else
      # no, create a new page
      lvh_page_cur = lvh_page(page)
      lvh_pages[page] = lvh_page_cur
    end
  end
end

#- ------------------------------------------------------------
  Parse single object

- ------------------------------------------------------------ -#
def parse_obj(jline, page)
  import global
  import introspect

  # line must contain 'obj' and 'id', otherwise it is ignored
  if jline.has("obj") && jline.has("id") && type(jline["id"]) == 'int'
    # 'obj_id' must be between 1 and 254
    var obj_id = int(jline["id"])
    if obj_id < 1 || obj_id > 254
      raise "value error", "invalid id " + str(obj_id)
    end

    # extract openhasp class, prefix with `lvh_`. Ex: `btn` becomes `lvh_btn`
    var obj_type = jline["obj"]

    # check if a class with the requested name exists
    var obj_class = introspect.get(global, "lvh_" + obj_type)
    if obj_class == nil
      raise "value error", "cannot find object of type " + str(obj_type)
    end

    # instanciate the object, passing the lvgl screen as paren object
    var obj = obj_class(page.get_scr())

    # add object to page object
    lvh_page_cur.set_obj(obj_id, obj)
    # set attributes
    # try every attribute, if not supported it is silently ignored
    for k:jline.keys()
      introspect.set(obj, k, jline[k])
      # obj.(k) = jline[k]
    end

    # create a global variable for this object of form p<page>b<id>, ex p1b2
    var glob_name = string.format("p%ib%i", lvh_page_cur.id(), obj_id)
    global.(glob_name) = obj
  end
end

# ex:
# {'page': 1, 'h': 50, 'obj': 'label', 'hidden': false, 'text': 'Hello', 'x': 5, 'id': 1, 'enabled': true, 'y': 5, 'w': 50}
# {"page":1,"id":2,"obj":"btn","x":5,"y":90,"h":90,"w":50,"text":"World","enabled":false,"hidden":false}

#- ------------------------------------------------------------
  Parse jsonl file line by line

- ------------------------------------------------------------ -#
for j:jsonl
  var jline = json.load(j)

  # parse page first
  if type(jline) == 'instance'
    parse_page(jline)
    parse_obj(jline, lvh_page_cur)
  end
end
