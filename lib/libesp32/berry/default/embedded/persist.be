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
    var _filename
    var _p
    var _dirty

    #- persist can be initialized with pre-existing values. The map is not copied so any change will be reflected -#
    def init(m)
      # print("Persist init")
      self._filename = '_persist.json'
      if isinstance(m,map)
        self._p = m.copy()   # need to copy instead?
      else
        self._p = {}
      end
      self.load(self._p, self._filename)
      self._dirty = false
      # print("Persist init")
    end

    #- virtual member getter, if a key does not exists return `nil`-#
    def member(key)
      return self._p.find(key)
    end

    #- virtual member setter -#
    def setmember(key, value)
      self._p[key] = value
      self._dirty = true
    end

    #- clear all entries -#
    def zero()
      self._p = {}
      self._dirty = true
    end
    
    def remove(k)
        self._p.remove(k)
        self._dirty = true
    end

    def has(k)
        return self._p.has(k)
    end

    def find(k, d)
        return self._p.find(k, d)
    end

    def load()
      import json
      import path
      var f           # file object
      var val         # values loaded from json

      if path.exists(self._filename)
        try
          f = open(self._filename, "r")
          val = json.load(f.read())
          f.close()
        except .. as e, m
          if f != nil f.close() end
          raise e, m
        end
        if isinstance(val, map)
          self._p = val     # sucess
        else
          print("BRY: failed to load _persist.json")
        end
        self._dirty = false
      else
        self.save()
      end

      # print("Loading")
    end

    def save()
      var f       # file object
      try
        f = open(self._filename, "w")
        self.json_fdump(f)
        f.close()
      except .. as e, m
        if f != nil f.close() end
        f = open(self._filename, "w")
        f.write('{}')   # fallback write empty map
        f.close()
        raise e, m
      end
      self._dirty = false
      # print("Saving")
    end

    def json_fdump_any(f, v)
      import json
      if   isinstance(v, map)
        self.json_fdump_map(f, v)
      elif isinstance(v, list)v
        self.json_fdump_list(f, v)
      else
        f.write(json.dump(v))
      end
    end

    def json_fdump_map(f, v)
      import json
      f.write('{')
      var sep = nil
      for k:v.keys()
        if sep != nil  f.write(sep) end
        
        f.write(json.dump(str(k)))
        f.write(':')
        self.json_fdump_any(f, v[k])

        sep = ","
      end
      f.write('}')
    end

    def json_fdump_list(f, v)
      import json
      f.write('[')
      var i = 0
      while i < size(v)
        if i > 0   f.write(',') end
        self.json_fdump_any(f, v[i])
        i += 1
      end
      f.write(']')
    end

    def json_fdump(f)
      import json
      if isinstance(self._p, map)
        self.json_fdump_map(f, self._p)
      else
        raise "internal_error", "persist._p is not a map"
      end
    end
  end


  return Persist()    # return an instance of this class
end

return persist_module
