# Extend module `lv` with `font_robotocondensed_latin1(size)`

var font_robotocondensed_latin1 = module("font_robotocondensed_latin1")

font_robotocondensed_latin1.init = def (m)
  import lv
  var wd = tasmota.wd   # save current working dir

  lv.font_robotocondensed_latin1 = def (size)
    return lv.load_font("A:" + wd + "robotocondensed_latin1_" + str(size) + ".bin")
  end
end

return font_robotocondensed_latin1

#-

font_robotocondensed_latin1.init(font_robotocondensed_latin1)
print(lv.font_robotocondensed_latin1)
print(lv.font_robotocondensed_latin1(32))

-#
