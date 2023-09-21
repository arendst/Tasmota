#
# Matter_Expirable.be - Support for Matter Expirable and Persistable objects and lists
#
# Copyright (C) 2023  Stephan Hadinger & Theo Arends
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

import matter

#@ solidify:Matter_Expirable,weak
#@ solidify:Matter_Expirable_list,weak

#################################################################################
# Matter_Expirable class
#
# Object that can expire after a certain timestamp or
# that does not expire and can be persisted
#
# There are only 3 valid states:
#  - not expirable and not persistable
#  - expirable and not persistable
#  - not expirable and persistable
#  - (both expirable and persistable is normally not valid but still supported)
#
#################################################################################
class Matter_Expirable
  var _list                     # the Expirable_list it belongs to (if any)
  var _persist                  # do we persist this sessions or is it remporary (bool)
  var _expiration               # if not `nil` the entry is removed after this timestamp

  #############################################################
  def init()
    self._persist = false
  end

  #############################################################
  def set_parent_list(l)
    self._list = l
  end
  def get_parent_list()
    return self._list
  end

  #############################################################
  def set_persist(p)
    self._persist = bool(p)
  end
  def does_persist()
    return self._persist
  end

  #############################################################
  # pre and post process when persist
  def persist_pre()
  end
  def persist_post()
  end

  #############################################################
  # post process after the object was loaded
  def hydrate_post()
  end
  
  #############################################################
  # before_remove
  #
  # called right before the element is removed
  def before_remove()
  end

  #############################################################
  # set absolute time for expiration
  def set_no_expiration()
    self._expiration = nil
  end

  #############################################################
  # set absolute time for expiration
  def set_expire_time(t)
    self._expiration = int(t)
  end

  #############################################################
  # set relative time in the future for expiration (in seconds)
  def set_expire_in_seconds(s, now)
    if s == nil  return end
    if now == nil     now = tasmota.rtc_utc()    end
    self.set_expire_time(now + s)
  end

  #############################################################
  # set relative time in the future for expiration (in seconds)
  # returns `true` if expiration date has been reached
  def has_expired(now)
    if now == nil     now = tasmota.rtc_utc()    end
    if self._expiration != nil
      return now >= self._expiration
    end
    return false
  end

end
matter.Expirable = Matter_Expirable


#################################################################################
# Matter_Expirable_list class
#
# Subclass of list handling Expirable(s)
#
#################################################################################
class Matter_Expirable_list : list
  # no specific attributes
  # no specific init()

  #############################################################
  # Accessors with control of arguments classes
  def push(o)
    if !isinstance(o, matter.Expirable)   raise "type_error", "argument must be of class 'Expirable'"   end
    o.set_parent_list(self)
    return super(self).push(o)
  end
  def setitem(i, o)
    if !isinstance(o, matter.Expirable)   raise "type_error", "argument must be of class 'Expirable'"   end
    o.set_parent_list(self)
    return super(self).setitem(i, o)
  end

  #############################################################
  # remove - override
  #
  def remove(i)
    if i >= 0 && i < size(self)   self[i].before_remove()   end
    return super(self).remove(i)
  end

  #############################################################
  # remove_expired
  #
  # Check is any object has expired
  #
  # returns `true` if persistable objects were actually removed (i.e. needs to persist again), `false` instead
  def remove_expired()
    var dirty = false
    var i = 0
    while i < size(self)
      if self[i].has_expired()
        if self[i]._persist    dirty = true end      # do we need to save
        self.remove(i)
      else
        i += 1
      end
    end
    return dirty
  end

  #############################################################
  # iterator over persistable instances
  #
  def persistables()
    var iterator = self.iter()
    var f = def ()
              while true
                var o = iterator()
                if o._persist   return o end
              end
              # ends with an exception
            end

    return f
  end

  #############################################################
  # Count the number of persistable objects
  def count_persistables()
    var ret = 0
    var idx = 0
    while idx < size(self)
      if self[idx]._persist   ret += 1  end
      idx += 1
    end
    return ret
  end

  #############################################################
  # every_second
  def every_second()
    self.remove_expired()
  end

end
matter.Expirable_list = Matter_Expirable_list

#-

# tests

a = matter.Expirable()
a.set_persist(true)
b = matter.Expirable()
c = matter.Expirable()
c.set_persist(true)
l = matter.Expirable_list()


l.push(a)
l.push(b)
l.push(c)

print('---')
for e:l  print(e) end
print('---')
for e:l.persistables()  print(e) end
print('---')

l.every_second()
print(size(l))
l[1].set_expire_time(10)
l.every_second()
print(size(l))

-#
