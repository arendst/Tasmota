#################################################################################
# dyn class
#
# Allows to use a map with members
# see https://github.com/berry-lang/berry/wiki/Chapter-8
#################################################################################
#@ solidify:dyn
class dyn
  var _attr
  def init()
      self._attr = {}
  end
  def setmember(name, value)
      self._attr[name] = value
  end
  def member(name)
    if self._attr.contains(name)
        return self._attr[name]
    else
        import undefined
        return undefined
    end
  end
  def tostring()
    return self._attr.tostring()
  end
end
