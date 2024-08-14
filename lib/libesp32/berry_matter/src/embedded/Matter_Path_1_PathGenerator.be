#
# Matter_Path_1_Generator.be - suppport for Matter concrete path generator
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
#
# OUTPUT:
#   - returns a concrete Path
#   - or `nil` if exhausted
#################################################################################
class Matter_PathGenerator
  var device                  # reference of device main object
  var path_in_endpoint        # input endpoint filter (nil or int)
  var path_in_cluster         # input cluster filter (nil or int)
  var path_in_attribute       # input attribute filter (nil or int)
  var path_in_fabric_filtered # input flag for fabric filtered reads (not implemented yet)
  # current status
  var pi                      # plugin object, `nil` waiting for value, `false` exhausted values, `true` if we responded a direct unmatched and it is the last one
  var cluster                 # current cluster number, `nil` waiting for value, `false` exhausted values
  var attribute               # current attribute number, `nil` waiting for value, `false` exhausted values
  # cache
  var clusters                # list of clusters sorted
  #
  var endpoint_found          # did we find a valid endpoint?
  var cluster_found           # did we find a valid cluster?
  var attribute_found         # did we find a valid attribute?

  # reused at each output
  var path_concrete           # placeholder for output concrete path, contains 'matter.Path()' instance - WARNING it can be modified once provided

  #################################################################################
  # simple constructor
  #
  def init(device)
    self.device = device
  end

  #################################################################################
  # start generator
  #
  # `in_endpoint`: endpoint number filter (int or nil for all)
  # `in_cluster`: cluster number filter (int or nil for all)
  # `in_attribute`: attribute number filter (int or nil for all)
  # `in_fabric_filtered`: is the filter fabric-filtered (nil or false or true) - currently stored but ignored
  def start(in_endpoint, in_cluster, in_attribute, in_fabric_filtered)
    # log(f">>>: PathGenerator start ep:{in_endpoint} cluster:{in_cluster} attribute:{in_attribute}", 3)
    self.path_concrete = matter.Path()
    self.reset()
    self.path_in_endpoint = in_endpoint
    self.path_in_cluster = in_cluster
    self.path_in_attribute = in_attribute
    self.path_in_fabric_filtered = bool(in_fabric_filtered)   # defaults to `false` if `nil`
    self.pi = nil                                             # ready to start
    #
    self.endpoint_found = false
    self.cluster_found = false
    self.attribute_found = false
  end

  #################################################################################
  # reset and free memory
  #
  def reset()
    var n = nil
    self.path_concrete.reset()
    #
    self.pi = false               # mark as inactive
    self.cluster = n
    self.attribute = n
    self.clusters = n
  end

  ################################################################################
  # is_direct
  #
  # Returns true if the original path is concrete, i.e. no expansion.
  # If not, errors while reading expanded attributes should not return an error
  def is_direct()
    return (self.path_in_endpoint != nil) && (self.path_in_cluster != nil) && (self.path_in_attribute != nil)
  end

  ################################################################################
  # _default_status_error
  #
  # Get the default error if the read or write fails.
  # This error is only reported if `direct` is true
  def _default_status_error()
    if self.is_direct()
      if (!self.endpoint_found)     return matter.UNSUPPORTED_ENDPOINT      end
      if (!self.cluster_found)      return matter.UNSUPPORTED_CLUSTER       end
      if (!self.attribute_found)    return matter.UNSUPPORTED_ATTRIBUTE     end
      return matter.UNREPORTABLE_ATTRIBUTE
    end
    return nil
  end

  ################################################################################
  # is_finished
  #
  # Returns `true` if we have exhausted the generator
  def is_finished()
    return (self.pi != false)
  end

  ################################################################################
  # get_pi
  #
  # Returns the endpoint object for the last context returned, or `nil` if not found or exhausted
  def get_pi()
    return ((self.pi == false) || (self.pi == true)) ? nil : self.pi
  end

  ################################################################################
  # next_attribute
  #
  # Generate next concrete path
  # Returns:
  # - a path object (that is valid until next call)
  # - if 'direct' (concrete path), ctx.status contains the appropriate error code if the path value is not supported
  # - `nil` if no more objects
  def next_attribute()
    if (self.pi == true) || (self.pi != nil && self.is_direct())    # if we already answered a succesful or missing context for direct request, abort on second call
      self.reset()
      return nil
    end

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
      path_concrete.fabric_filtered = self.path_in_fabric_filtered
      path_concrete.status = nil
      # log(f">>>: PathGenerator next path_concrete:{path_concrete}", 3)
      return path_concrete
    end

    # special case, if it was 'direct' and we are here, then we didn't find a match
    # return the concrete path ans prepare status
    if self.is_direct()
      var path_concrete = self.path_concrete
      path_concrete.reset()
      path_concrete.endpoint = self.path_in_endpoint
      path_concrete.cluster = self.path_in_cluster
      path_concrete.attribute = self.path_in_attribute
      path_concrete.fabric_filtered = self.path_in_fabric_filtered
      path_concrete.status = self._default_status_error()
      self.pi = true                        # next call will trigger Generator exhausted
      # log(f">>>: PathGenerator next path_concrete:{path_concrete} direct", 3)
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
    var ep_filter = self.path_in_endpoint
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
    var cl_filter = self.path_in_cluster
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

    var attributes_bytes = self.pi.get_attribute_list_bytes(self.cluster)
    var attributes_bytes_sz = (attributes_bytes != nil) ? size(attributes_bytes) / 2 : 0
    var attr_filter = self.path_in_attribute
    
    var idx = -1
    if (self.attribute != nil)
      var i = 0
      while true
        if (i < attributes_bytes_sz)
          if attributes_bytes.get(i * 2, -2) == self.attribute
            idx = i
            break
          end
          i += 1
        else
          idx = nil
          break
        end
      end
      # idx = attributes.find(self.attribute)     # finds index in current list
    end
    # safeguard
    if (idx != nil)
      while (idx + 1 < attributes_bytes_sz)
        idx += 1                      # move to next item
        self.attribute = attributes_bytes.get(idx * 2, -2)
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
  gen.start(endpoint, cluster, attribute)
  var cp
  while (cp := gen.next_attribute())
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

-#
