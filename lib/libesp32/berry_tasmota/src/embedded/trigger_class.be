#- Native code used for testing and code solidification -#
#- Do not use it -#

#@ solidify:Trigger
class Trigger
  var trig, f, id
  var o             # optional object
  def init(trig, f, id, o)
    self.trig = trig
    self.f = f
    self.id = id
    self.o = o
  end
  def tostring()
    import string
    return string.format("<instance: %s(%s, %s, %s)", str(classof(self)),
              str(self.trig), str(self.f), str(self.id))
  end
  # next() returns the next trigger, or 0 if rtc is invalid, or nil if no more
  def next()
    if self.o
      return self.o.next()
    end
  end
  
  def time_reached()
    if self.o && self.trig > 0
      return self.o.time_reached(self.trig)
    end
    return false
  end
end
