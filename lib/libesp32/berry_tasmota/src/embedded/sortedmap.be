#################################################################################
# sortedmap class
#
# Allows to use a map with members
# see https://github.com/berry-lang/berry/wiki/Chapter-8
#
# The key-value pairs are stored in the underlying `map` instance (inherited
# from the super class) so that any code walking a `map` -- like `json.dump()`
# or `map.tostring()` -- sees the actual content. This class only adds a list
# of keys kept in sorted order, used to iterate in a predictable order.
#################################################################################
#@ solidify:sortedmap
class sortedmap : map
  var _keys    # list of keys, maintained in sorted order
  
  # Constructor
  def init(base)
    super(self).init()    # initialize the underlying map, must not be skipped
    self._keys = []
    if isinstance(base, map)    # `sortedmap` is a `map` too
      self._load(base, self)
    end
  end

  # Shallow copy from existing map, sub-maps are converted to sortedmap
  def _load(org, copy)
    for key : org.keys()
      var value = org.item(key)
      if isinstance(value, map)
        value = self._load(value, sortedmap())
      end
      copy.insert(key, value)
    end
    return copy
  end
  
  # Insert a new key-value pair or update existing value
  def insert(key, value)
    var is_new = !super(self).contains(key)
    super(self).setitem(key, value)
    
    if is_new
      # Binary search to find insert position to maintain sorted order
      self._keys.insert(self._find_insert_position(key), key)
      return true
    end
    return false
  end
  
  # Remove a key-value pair
  def remove(key)
    if super(self).contains(key)
      super(self).remove(key)
      # Find key position in the list
      var idx = self._keys.find(key)
      if idx != nil
        self._keys.remove(idx)
      end
      return true
    end
    return false
  end
  
  # Set a value by key
  def setitem(key, value)
    return self.insert(key, value)
  end
  
  # Return all sorted keys as a list
  def get_keys()
    return self._keys
  end
  
  # Return iterator to keys in sorted order
  def keys()
    return self._keys.iter()
  end
  # String representation, in sorted key order
  def tostring()
    var r = '{'
    var sep = ''
    for k : self._keys
      r += f'{sep}{k:q}: {self.item(k):q}'
      sep = ', '
    end
    r += '}'
    return r
  end

  # Compact JSON representation, in sorted key order
  def tojson()
    import json
    var r = '{'
    var sep = ''
    for k : self._keys
      r += f'{sep}{json.dump(str(k))}:{json.dump(self.item(k))}'
      sep = ','
    end
    return r + '}'
  end

  # Return iterator to values in sorted key order
  def iter()
    var values = []
    for key : self._keys
      values.push(super(self).item(key))
    end
    return values.iter()
  end

  # Get by index number
  def get_by_index(idx)
    return super(self).item(self._keys[idx])
  end
  
  # Clear all key-value pairs
  def clear()
    super(self).init()    # replace the underlying map with an empty one
    self._keys = []
  end
  
  # Remove entries with a specific value
  def remove_by_value(value)
    var keys_to_remove = []
    
    # First pass: identify all keys with matching values
    for key : self._keys
      if super(self).item(key) == value
        keys_to_remove.push(key)
      end
    end
    
    # Second pass: remove all identified keys
    var removed = 0
    for key : keys_to_remove
      self.remove(key)
      removed += 1
    end
    
    return removed  # Return the number of entries removed
  end
  
  # Binary search to find insertion position for a new key
  def _find_insert_position(key)
    var low = 0
    var high = self._keys.size() - 1
    
    while low <= high
      var mid = int((low + high) / 2)
      var mid_key = self._keys[mid]
      
      # Compare keys - this is the tricky part since Berry doesn't have 
      # a generic comparison operator for different types
      var should_insert_after
      
      if type(key) == type(mid_key)
        # Same types, we can compare directly
        should_insert_after = (key > mid_key)
      else
        # Different types, use string representation for ordering
        should_insert_after = (str(key) > str(mid_key))
      end
      
      if should_insert_after
        low = mid + 1
      else
        high = mid - 1
      end
    end
    
    return low
  end
end

return sortedmap

#-

# Example usage:
m = sortedmap()

# Insert elements
m.insert('c', 3)
m.insert('a', 1)
m.insert('b', 2)
m.insert(1, 'number one')
m.insert(10, 'ten')
m.insert(2, 'two')

# Access using different syntaxes
print(m)               # Prints in sorted key order
print(m['a'])          # Access by key
print(m.get_keys())    # Get list of sorted keys

# Iteration through key-value pairs
for k : m.get_keys()
  print(k, m[k])
end

# Remove elements
m.remove('b')
print(m)

-#

#-
# Test case

var m = sortedmap()

# Test initial state
assert(m.size() == 0)
assert(m.get_keys().size() == 0)

# Test insertion
m.insert('c', 3)
m.insert('a', 1)
m.insert('b', 2)

# Test size
assert(m.size() == 3)

# Test key order
assert(m.get_keys()[0] == 'a')
assert(m.get_keys()[1] == 'b')
assert(m.get_keys()[2] == 'c')

# Test retrieval
assert(m['a'] == 1)
assert(m['b'] == 2)
assert(m['c'] == 3)

# Test contains
assert(m.contains('a'))
assert(!m.contains('d'))

# Test find with default
assert(m.find('a', 0) == 1)
assert(m.find('d', 0) == 0)

# Test mixed types
m.insert(1, 'number one')
m.insert(10, 'ten')
m.insert(2, 'two')

# Check size again
assert(m.size() == 6)

# Test remove
m.remove('b')
assert(m.size() == 5)
assert(!m.contains('b'))

# Test key order after removing
var keys = m.get_keys()
assert(keys[0] == 1)
assert(keys[1] == 2)
assert(keys[2] == 10)
assert(keys[3] == 'a')
assert(keys[4] == 'c')

# Test string representation and escaping
m = sortedmap()
m.insert('b', 2)
m.insert('a', 1)
assert(str(m) == "{'a': 1, 'b': 2}")
var key = "k'\"\\\n\r\t\x01é"
var value = "v'\"\\\n\r\t\x1f世界"
m = sortedmap()
m.insert(key, value)
var plain = {}
plain.insert(key, value)
assert(str(m) == str(plain))

# Test JSON serialization
import json
m = sortedmap()
assert(json.dump(m) == '{}')
m.insert('z', 1)
m.insert('a', 'x')
assert(json.dump(m) == '{"a":"x","z":1}')
assert(json.dump(m, 'format') == '{"a":"x","z":1}')
m = sortedmap({'z': {'b': 2, 'a': 1}, 'a"b': 'line\n'})
assert(json.dump(m) == '{"a\\"b":"line\\n","z":{"a":1,"b":2}}')

# Test clear
m.clear()
assert(m.size() == 0)
assert(m.get_keys().size() == 0)

# Test remove_by_value
m = sortedmap()
m.insert('a', 1)
m.insert('b', 2)
m.insert('c', 2)
m.insert('d', 3)
m.insert('e', 2)
m.insert('f', 4)

# Test removing multiple entries with the same value
var removed = m.remove_by_value(2)
assert(removed == 3, "Expected to remove 3 entries, got " + str(removed))
assert(m.size() == 3, "Expected 3 entries remaining, got " + str(m.size()))
assert(!m.contains('b'), "Key 'b' should be removed")
assert(!m.contains('c'), "Key 'c' should be removed")
assert(!m.contains('e'), "Key 'e' should be removed")
assert(m.contains('a'), "Key 'a' should still exist")
assert(m.contains('d'), "Key 'd' should still exist")
assert(m.contains('f'), "Key 'f' should still exist")

# Test removing a value that doesn't exist
removed = m.remove_by_value(100)
assert(removed == 0, "Expected to remove 0 entries, got " + str(removed))
assert(m.size() == 3, "Map size should remain unchanged")

# Test removing the last remaining entries
removed = m.remove_by_value(1)
assert(removed == 1, "Expected to remove 1 entry, got " + str(removed))
assert(m.size() == 2, "Expected 2 entries remaining, got " + str(m.size()))

# Test removing all remaining entries
m.insert('g', 3)
removed = m.remove_by_value(3)
assert(removed == 2, "Expected to remove 2 entries, got " + str(removed))
removed = m.remove_by_value(4)
assert(removed == 1, "Expected to remove 1 entry, got " + str(removed))
assert(m.size() == 0, "Map should be empty")

-#
