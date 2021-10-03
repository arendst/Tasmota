#-  persistance module for Berry -#
#- -#
#- To solidify: -#
#-
  # load only persis_module and persist_module.init
  import solidify
  solidify.dump(persist_module.init)
  # copy and paste into `be_persist_lib.c`
-#
var persist_module = module("persist")

persist_module.init = def (m)

  class Persist
    var filename
    var p
    var dirty

    #- persist can be initialized with pre-existing values. The map is not copied so any change will be reflected -#
    def init(m)
      # print("Persist init")
      self.filename = '_persist.json'
      if isinstance(m,map)
        self.p = m.copy()   # need to copy instead?
      else
        self.p = {}
      end
      self.load(self.p, self.filename)
      self.dirty = false
      # print("Persist init")
    end

    #- virtual member getter, if a key does not exists return `nil`-#
    def member(key)
      return self.p.find(key)
    end

    #- virtual member setter -#
    def setmember(key, value)
      self.p[key] = value
      self.dirty = true
    end

    #- clear all entries -#
    def zero()
      self.p = {}
      self.dirty = true
    end
    
    #- custom tostring -#
    def tostring()
      import string
      return string.format("<instance: %s(%s)>", classname(self), str(self.p))
    end

    def load()
      import json
      import path
      var f           # file object
      var val         # values loaded from json

      if path.exists(self.filename)
        try
          f = open(self.filename, "r")
          val = json.load(f.read())
          f.close()
        except .. as e, m
          if f != nil f.close() end
          raise e, m
        end
        self.p = val
        self.dirty = false
      end

      # print("Loading")
    end

    def save()
      import json
  
      var f       # file object
      try
        f = open(self.filename, "w")
        f.write(json.dump(self.p))
        f.close()
      except .. as e, m
        if f != nil f.close() end
        raise e, m
      end
      self.dirty = false
      # print("Saving")
    end
  end

  return Persist()    # return an instance of this class
end

return persist_module
