#
# Matter_IM_Path_1.be - suppport for Matter concrete path generator
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

#@ solidify:Matter_PathGenerator,weak

#################################################################################
# Matter_PathGenerator
#
# INPUT: Takes a context:
#   - plugin
#   - path (abstract or concrete)
#   - session
#
# OUTPUT:
#   - returns a concrete Path
#   - or `nil` if exhausted
#################################################################################
class Matter_PathGenerator
  var device                  # reference of device main object
  var path_in                 # input path (abstract or concrete)
  var session                 # session object in which the request was made
  # current status
  var pi                      # plugin object, `nil` waiting for value, `false` exhausted values
  var cluster                 # current cluster number, `nil` waiting for value, `false` exhausted values
  var attribute               # current attribute number, `nil` waiting for value, `false` exhausted values
  # cache
  var clusters                # list of clusters sorted
  #
  var endpoint_found          # did we find a valid endpoint?
  var cluster_found           # did we find a valid cluster?
  var attribute_found         # did we find a valid attribute?

  # reused at each output
  var path_concrete           # placeholder for output concrete path

  def init(device)
    self.device = device
  end

  # start generator
  def start(path_in, session)
    self.path_concrete = matter.Path()
    self.reset()
    self.path_in = path_in
    self.session = session
    #
    self.endpoint_found = false
    self.cluster_found = false
    self.attribute_found = false
  end

  def reset()
    var n = nil
    self.path_in = n
    self.session = n
    self.path_concrete.reset()
    #
    self.pi = n               # pre-load first plugin
    self.cluster = n
    self.attribute = n
    self.clusters = n
    self.clusters = n
  end

  def get_pi()
    return self.pi
  end
  ################################################################################
  # next
  #
  # Generate next concrete path
  # Returns:
  # - a path object (that is valid until next call)
  # - `nil` if no more objects
  def next()
    if (self.path_in == nil)    return nil  end

    while (self.pi != false)        # loop until we exhausted endpoints
      # PRE: self.pi is not `false`
      if (self.pi == nil) || (self.cluster == false)    # no endpoint yet, or exhausted clusters
        self._next_endpoint()
        continue
      end
      # PRE: self.pi is valid, self.cluster is not false
      self.endpoint_found = true
      if (self.cluster == nil) || (self.attribute == false)   # no cluster yet, or exhausted attributes
        self._next_cluster()
        continue
      end
      # PRE: self.pi and self.cluster are valid, self.attribute is not false
      self.cluster_found = true
      self._next_attribute()        # advance to first or next attribute
      if (self.attribute == false)
        continue                       # iterate so that we explore next cluster
      end
      # we have a concrete path
      self.attribute_found = true
      var path_concrete = self.path_concrete
      path_concrete.reset()
      path_concrete.endpoint = self.pi.get_endpoint()
      path_concrete.cluster = self.cluster
      path_concrete.attribute = self.attribute
      return path_concrete
    end
    # we exhausted all endpoints - finish and clean
    self.reset()
    return nil
  end

  #------------------------------------------------------------------------------#
  # advance to next endpoint
  def _next_endpoint()
    if (self.pi == false)   return false    end       # exhausted all possible values

    var plugins = self.device.plugins     # shortcut
    var ep_filter = self.path_in.endpoint
    # cluster and attribute are now undefined
    self.cluster = nil
    self.attribute = nil
    # idx contains the index of current plugin, or `nil` if not started
    var idx = -1
    if (self.pi != nil)
      idx = plugins.find(self.pi)     # find index in current list
    end
    # safeguard
    if (idx != nil)
      while (idx + 1 < size(plugins))
        idx += 1                      # move to next item
        self.pi = plugins[idx]
        if (ep_filter == nil) || (ep_filter == self.pi.get_endpoint())
          self.clusters = self.pi.get_cluster_list_sorted()   # memoize clusters
          return self.pi
        end
      # iterate
      end
    end
    self.pi = false
    return false
  end

  #------------------------------------------------------------------------------#
  # advance to next cluster
  #
  # self.clusters already contains the sorted list of clusters of pi
  def _next_cluster()
    if (self.cluster == false)   return false    end       # exhausted all possible values

    var clusters = self.clusters
    var cl_filter = self.path_in.cluster
    # attribute is now undefined
    self.attribute = nil
    var idx = -1
    if (self.cluster != nil)
      idx = clusters.find(self.cluster)     # find index in current list
    end
    # safeguard
    if (idx != nil)
      while (idx + 1 < size(clusters))
        idx += 1                      # move to next item
        self.cluster = clusters[idx]
        if (cl_filter == nil) || (cl_filter == self.cluster)
          return self.cluster
        end
      end
    end
    self.cluster = false
    return false
  end

  #------------------------------------------------------------------------------#
  # advance to next attribute
  #
  # self.clusters already contains the sorted list of clusters of pi
  def _next_attribute()
    if (self.attribute == false)   return false    end       # exhausted all possible values

    var attributes = self.pi.get_attribute_list(self.cluster)
    var attr_filter = self.path_in.attribute
    var idx = -1
    if (self.attribute != nil)
      idx = attributes.find(self.attribute)     # find index in current list
    end
    # safeguard
    if (idx != nil)
      while (idx + 1 < size(attributes))
        idx += 1                      # move to next item
        self.attribute = attributes[idx]
        if (attr_filter == nil) || (attr_filter == self.attribute)
          return self.attribute
        end
      end
    end
    self.attribute = false
    return false
  end

end
matter.PathGenerator = Matter_PathGenerator

#-

# Tests

var gen = matter.PathGenerator(matter_device)

def gen_path_dump(endpoint, cluster, attribute)
  var path = matter.Path()
  path.endpoint = endpoint
  path.cluster = cluster
  path.attribute = attribute
  gen.start(path)
  var cp
  while (cp := gen.next())
    print(cp)
  end
end


gen_path_dump(nil, nil, nil)
gen_path_dump(1, nil, nil)
gen_path_dump(1, 3, nil)
gen_path_dump(nil, 5, nil)
gen_path_dump(nil, nil, 0xFFFB)
gen_path_dump(4, 5, 5)
gen_path_dump(4, 5, 6)



var gen = matter.PathGenerator(matter_device)
var path = matter.Path()
path.endpoint = nil
gen.start(path)

# print(gen._next_endpoint())
# print(gen._next_cluster())
# print(gen._next_attribute())



var gen = matter.PathGenerator(matter_device)
var path = matter.Path()
path.endpoint = 4
path.cluster = 5
path.attribute = 1
gen.start(path)




var cp
while (cp := gen.next())
  print(cp)
end

-#
