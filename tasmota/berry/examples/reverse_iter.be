#- Example of reverse range function

   Allows to do range for loops in reverse ordre:

   > for i:reverse(5..8) print(i) end
   3
   2
   1
   >

 -#

class reverse
  var r

  def init(r)
    if issubclass(range, r)
      self.r = r
    else
      raise 'type_error', 'Unsupported type of class'
    end
  end

  def iter()
    var iter_func = self.r.iter()
    var offset = self.r.upper() + self.r.lower()
    return def () return offset - iter_func() end
  end
end