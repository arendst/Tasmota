
import string
import json

#- ------------------------------------------------------------
  Class `lvh_obj` encapsulating `lv_obj``

  Provide a mapping for virtual members
  Stores the associated page and object id

  Adds specific virtual members used by OpenHASP
- ------------------------------------------------------------ -#
class lvh_obj
  static _lv_class = lv_obj
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
  }

  var _lv_obj     # native lvgl object
  var _lv_label   # sub-label if exists

  def init(parent)
    var obj_class = self._lv_class    # need to copy to a var to distinguish from method call
    self._lv_obj = obj_class(parent)
  end

  def obj()
    return self._lv_obj
  end

  def tostring()
    return "<instance "+classname(self)+"()>"
  end

  #- ------------------------------------------------------------
    Mapping of synthetic attributes
    - text
    - hidden
    - enabled
  - ------------------------------------------------------------ -#
  #- `hidden` attributes mapped to LV_OBJ_FLAG_HIDDEN -#
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

  def set_toggle(t)
    if t == "TRUE"  t = true end
    if t == "FASLE" t = false end
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
  def set_text(t)
    if self._lv_label == nil
      self._lv_label = lv_label(self.obj())
    end
    self._lv_label.set_text(t)
  end

  def get_text()
    if self._lv_label == nil
      return nil
    end
    return self._lv_label.get_text()
  end

  #- ------------------------------------------------------------
    Mapping of virtual attributes
  - ------------------------------------------------------------ -#
  def member(k)
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

#- creat sub-classes of lvh_obj and map the LVGL class in static '_lv_class' attribute -#
class lvh_arc : lvh_obj       static _lv_class = lv_arc end
class lvh_bar : lvh_obj       static _lv_class = lv_bar end
class lvh_btn : lvh_obj       static _lv_class = lv_btn end
class lvh_btnmatrix : lvh_obj static _lv_class = lv_btnmatrix end
class lvh_checkbox : lvh_obj  static _lv_class = lv_checkbox end
class lvh_dropdown : lvh_obj  static _lv_class = lv_dropdown end
class lvh_img : lvh_obj       static _lv_class = lv_img end
class lvh_label : lvh_obj     static _lv_class = lv_label end
class lvh_line : lvh_obj      static _lv_class = lv_line end
class lvh_roller : lvh_obj    static _lv_class = lv_roller end
class lvh_slider : lvh_obj    static _lv_class = lv_slider end
class lvh_slider : lvh_obj    static _lv_class = lv_slider end
class lvh_switch : lvh_obj    static _lv_class = lv_switch end
class lvh_textarea : lvh_obj  static _lv_class = lv_textarea end

#- ------------------------------------------------------------
  Class `lvh_page` encapsulating `lv_obj` as screen (created with lv_obj(0))
- ------------------------------------------------------------ -#
# ex of transition: lv.scr_load_anim(scr, lv.SCR_LOAD_ANIM_MOVE_RIGHT, 500, 0, false)
class lvh_page
  var _obj_id        # list of objects by id numbers
  var _page_id
  var _lv_scr       # lvgl screen object

  #- init(page_number) -#
  def init(page_number)
    import string
    import global

    # if no parameter, default to page #1
    if page_number == nil  page_number = 1 end

    self._page_id = page_number     # remember our page_number
    self._obj_id = {}               # init list of objects
    if page_number == 1
      self._lv_scr = lv.scr_act()   # default screen
    else
      self._lv_scr = lv_obj(0)      # allocate a new screen
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
      obj.(k) = jline[k]
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
  parse_page(jline)
  parse_obj(jline, lvh_page_cur)
end

print(lvh_pages[1]._obj_id)
