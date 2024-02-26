# classes for extra components

#@ solidify:lv_extra,weak
var lv_extra = module("lv_extra")

class lv_coord_arr : bytes
  # initializer can either create a new structure or map an existing
  # 1. create a new
  #    arg 1 : list of ints
  #
  # 2. map an existing
  #    arg 1 : pointer of existing array
  #    arg 2 : number of items in array
  def init(l, n)
    if type(l) == 'instance' && isinstance(l, list)
      # size of the array is 2x number of elements
      super(self).init(size(l) * 4)
      for e: l
        self.add(int(e), 4)
      end
    elif type(l) == 'ptr' && type(n) == 'int'
      super(self).init(l, n * 4)
    else
      raise "value_error", "argument must be a list or a pointer+size"
    end
  end

  def item(n)
    return self.get(n * 4, 4)
  end

  def setitem(n, v)
    self.set(n * 4, v, 4)
  end
end

class lv_int_arr : bytes
  # initializer can either create a new structure or map an existing
  # 1. create a new
  #    arg 1 : list of ints
  #
  # 2. map an existing
  #    arg 1 : pointer of existing array
  #    arg 2 : number of items in array
  def init(l, n)
    if type(l) == 'instance' && isinstance(l, list)
      # size of the array is 2x number of elements
      super(self).init(size(l) * 4)
      for e: l
        self.add(int(e), 4)
      end
    elif type(l) == 'ptr' && type(n) == 'int'
      super(self).init(l, n * 4)
    else
      raise "value_error", "argument must be a list or a pointer+size"
    end
  end

  def item(n)
    return self.get(n * 4, 4)
  end

  def setitem(n, v)
    self.set(n * 4, v, 4)
  end
end

class lv_float_arr : bytes
  # initializer can either create a new structure or map an existing
  # 1. create a new
  #    arg 1 : list of floats
  #
  # 2. map an existing
  #    arg 1 : pointer of existing array
  #    arg 2 : number of items in array
  def init(l, n)
    if type(l) == 'instance' && isinstance(l, list)
      # size of the array is 2x number of elements
      super(self).init(size(l) * 4)
      for e: l
        self.addfloat(real(e), 4)
      end
    elif type(l) == 'ptr' && type(n) == 'int'
      super(self).init(l, n * 4)
    else
      raise "value_error", "argument must be a list or a pointer+size"
    end
  end

  def item(n)
    return self.getfloat(n * 4)
  end

  def setitem(n, v)
    self.setfloat(n * 4, v)
  end
end

class lv_point_arr : bytes
  def init(l)
    if type(l) != 'instance' || !isinstance(l, list)  raise "value_error", "argument must be a list" end
    super(self).init(size(l) * 8)

    for e: l
      if type(e) != 'instance' || !isinstance(e, lv.lv_point)  raise "value_error", "elements must be a lv_point" end
      self.add(e.x, 4)
      self.add(e.y, 4)
    end
  end
end

class lv_style_prop_arr : bytes
  def init(l)
    if type(l) != 'instance' || !isinstance(l, list)  raise "value_error", "argument must be a list" end
    super(self).init(size(l) * 4)

    for e: l
      self.add(int(e), 4)
    end
  end
end

class lv_str_arr : bytes
  var l                         # keep a copy of the list because we want the pointer to strings to remain valid
  def init(l)
    self.l = l
    super(self).init(size(l) * 4)

    import introspect
    for e: l
      self.add(int(introspect.toptr(e)), 4)
    end
  end
end

lv_extra.lv_coord_arr = lv_coord_arr
lv_extra.lv_int_arr = lv_int_arr
lv_extra.lv_point_arr = lv_point_arr
lv_extra.lv_style_prop_arr = lv_style_prop_arr
lv_extra.lv_str_arr = lv_str_arr

lv_extra.init = def (m)
  import global
  var lv = global.lv

  lv.lv_coord_arr = m.lv_coord_arr
  lv.lv_int_arr = m.lv_int_arr
  lv.lv_point_arr = m.lv_point_arr
  lv.coord_arr = m.lv_coord_arr
  lv.int_arr = m.lv_int_arr
  lv.point_arr = m.lv_point_arr
  lv.style_prop_arr = m.lv_style_prop_arr
  lv.str_arr = m.lv_str_arr

  return m
end

return lv_extra

#-

print(lv_coord_arr([1,2,3]))
# bytes('010002000300')

var p1 = lv.point()
p1.x = 1
p1.y = 2
var p2 = lv.point()
p2.x = 3
p2.y = 4
print(lv_point_arr([p1, p2]))

-#