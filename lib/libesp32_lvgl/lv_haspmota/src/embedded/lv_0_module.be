# LVGL prepare for lv_haspmota solidification
# mock `lv` module

lv = module('lv')

import global
tasmota = nil

var classes = [
    "page", "obj", "scr",
    "btn", "switch", "checkbox",
    "label", "spinner", "line", "img", "roller", "btnmatrix",
    "bar", "slider", "arc", "textarea", "led", "dropdown",
    "scale",
    "qrcode", "chart", "spangroup", "span",
    # ported from LVGL 8
    "colorwheel",
    # new internal names
    "button", "image", "buttonmatrix", "msgbox", "tabview"
  ]

for cl: classes
    var s = f"class lv_{cl} end lv.{cl} = lv_{cl}"
    compile(s)()
end
