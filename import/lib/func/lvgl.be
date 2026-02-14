import sys

var lvgl = module('lvgl')

lvgl.RES = {}                          # key -> pinned lv.img
lvgl.RES_OBJ = lv.obj(lv.layer_top())  # parent drží referencie

# vnútorné: vráti sys.path[0] alebo "/"
lvgl._root = def()
  var p = sys.path()
  if p != nil && size(p) > 0 && p[0] != nil && size(p[0]) > 0
    return p[0]
  end
  return "/"
end

# preload: načíta obrázok (dekóduje LVGL) a pripne pod 'key'
lvgl.img_preload = def(key, relpath)
  if lvgl.RES.contains(key) return true end
  var src = "A:" + lvgl._root() + relpath   # očakáva REL cestu typu "img/x.png"
  var pin = lv.img(lvgl.RES_OBJ)
  pin.set_size(1,1)
  pin.set_pos(-1000,-1000)
  pin.set_src(src)
  lvgl.RES[key] = pin
  return true
end

# vytvorí nový lv.img a nasadí bitmapu z preloaded zdroja (bez I/O)
lvgl.img = def(parent, key)
  if !lvgl.RES.contains(key)
    raise "RES not preloaded: " + str(key)
  end
  var pin = lvgl.RES[key]
  var img = lv.img(parent)
  img.set_bitmap_map_src(pin.get_bitmap_map_src())
  return img
end

return global.lvgl
