#-  persistance module for Berry -#
#- -#
#- To solidify: -#
#-
  # load only persist_module and persist_module.init
  import solidify
  solidify.dump(persist_module.init)
  # copy and paste into `be_persist_lib.c`
-#
#@ solidify:persist_module
var persist_module = module("persist")

class Persist
  static var _filename = '_persist.json'
  var _p
  var _dirty

  def init()
    self._p = {}
    self._dirty = false
    self.load()
  end

  #- virtual member getter, if a key does not exists return `nil` instead of exception -#
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
  
  #- force dirty -#
  def dirty()
    self._dirty = true
  end

  def remove(k)
      self._p.remove(k)
      self._dirty = true
  end

  def contains(k)
      return self._p.contains(k)
  end

  def has(k)      # deprecated, use contains instead
    return self._p.contains(k)
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
        self._p = val     # success
      else
        print("BRY: failed to load _persist.json")
      end
      self._dirty = false
    else
      self.save()
    end

    # print("Loading")
  end

  def save(force_save)
    if self._dirty || force_save   # do not save if not dirty
      var f       # file object
      try
        f = open(self._filename, "w")
        self.json_fdump(f)
        f.close()
      except .. as e, m
        if (f != nil) f.close() end
        f = nil
        try
          f = open(self._filename, "w")
          f.write('{}')   # fallback write empty map
        except ..
        end
        if f != nil f.close() end
        raise e, m
      end
      self._dirty = false
    end
  end

  def json_fdump_any(f, v)
    import json
    if   isinstance(v, map)
      self.json_fdump_map(f, v)
    elif isinstance(v, list)
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
persist_module.Persist = Persist

persist_module.init = def (m)
  return m.Persist()    # return an instance of this class
end

return persist_module
