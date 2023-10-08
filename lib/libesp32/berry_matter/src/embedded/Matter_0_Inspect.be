#
# Matter_inspect.be - implements a generic function to inspect an instance (for debugging only)
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

#@ solidify:matter.sort,weak
def sort(l)
  # insertion sort
  for i:1..size(l)-1
    var k = l[i]
    var j = i
    while (j > 0) && (l[j-1] > k)
      l[j] = l[j-1]
      j -= 1
    end
    l[j] = k
  end
  return l
end
matter.sort = sort

#@ solidify:matter.jitter,weak
#############################################################
# jitter
#
# compute a random jitter time for an update_time value
def jitter(update_time)
  # initialization to a random value within range
  import crypto
  var rand31 = crypto.random(4).get(0,4) & 0x7FFFFFFF     # random int over 31 bits
  return tasmota.millis(rand31 % update_time)
end
matter.jitter = jitter

#@ solidify:matter.inspect,weak
# debug function
def inspect(p)
  try
    import introspect

    var keys = []
    for k : introspect.members(p)
      var v = introspect.get(p, k)
      if type(v) != 'function'    keys.push(k) end
    end
    keys = matter.sort(keys)

    var r = []
    for k : keys
      var v = introspect.get(p, k)
      # if type(v) == 'string'    v = string.escape(v, true) end
      r.push(format("'%s': %s", str(k), str(v)))
    end

    return "{" + r.concat(", ") + "}"
  except .. as e, m
    return "Exception:" + str(e) + "|" + str(m)
  end
end
matter.inspect = inspect


#############################################################
# consolidate_clusters
#
# Build a consolidated map of all the `CLUSTERS` static vars
# from the inheritance hierarchy
#@ solidify:matter.consolidate_clusters,weak
def consolidate_clusters(cl, m)
  var cl_parent = super(cl).CLUSTERS
  var ret = {}
  # clone cl_parent
  for k: cl_parent.keys()
    # print(f"{k=} {cl_parent[k]=}")
    ret[k] = cl_parent[k].copy()
  end
  # add all keys from m
  # print("--- step 2")
  for k: m.keys()
    # print(f"{k=} {ret.find(k)=} {m[k]=}")
    var l = ret.find(k)
    if l == nil     l = []  end
    ret[k] = l + m[k]
  end
  # print(ret)
  return ret
end
matter.consolidate_clusters = consolidate_clusters

#############################################################
# consolidate_update_commands_list
#
# Build a consolidated list and remove duplicates
#@ solidify:matter.UC_LIST,weak
def UC_LIST(cl, *l)
  var uc_parent = super(cl).UPDATE_COMMANDS
  return uc_parent + l
end
matter.UC_LIST = UC_LIST
