# Extend module `lv` with `font_robotocondensed_latin1(size)`

var font_robotocondensed_latin1 = module("font_robotocondensed_latin1")

font_robotocondensed_latin1.init = def (m)
  import lv
  var wd = tasmota.wd   # save current working dir

  class robotocondensed_latin1_loader
    var wd
    var font_cache
    def init(wd)
      self.wd = wd
      self.font_cache = {}
    end

    def load_robotocondensed_latin1(sz)
      sz = int(sz)
      var font
      # is the font already embedded?
      try
        font = lv.font_embedded("robotocondensed", sz)
      except ..
      end
      if (font) return font end
      # is the font in our local cache?
      font = self.font_cache.find(sz)
      if (font) return font end
      # load the font from the archive
      font = lv.load_font("A:" + wd + "robotocondensed_latin1_" + str(sz) + ".bin")
      self.font_cache[sz] = font
      return font
    end
  end

  var monad = robotocondensed_latin1_loader(wd)
  lv.font_robotocondensed_latin1 = / sz -> monad.load_robotocondensed_latin1(sz)
end

return font_robotocondensed_latin1

#-

font_robotocondensed_latin1.init(font_robotocondensed_latin1)
print(lv.font_robotocondensed_latin1)
print(lv.font_robotocondensed_latin1(32))

-#
