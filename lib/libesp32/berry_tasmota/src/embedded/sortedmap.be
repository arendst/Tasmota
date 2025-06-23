#################################################################################
# sortedmap class
#
# Allows to use a map with members
# see https://github.com/berry-lang/berry/wiki/Chapter-8
#################################################################################
#@ solidify:sortedmap
class sortedmap
  var _data    # internal map for storing key-value pairs
  var _keys    # list for maintaining sorted keys
  
  # Constructor
  def init()
    self._data = {}
    self._keys = []
  end

  # Insert a new key-value pair or update existing value
  def insert(key, value)
    var is_new = !self._data.contains(key)
    self._data[key] = value
    
    if is_new
      # Binary search to find insert position to maintain sorted order
      var pos = self._find_insert_position(key)
      self._keys.insert(pos, key)
      return true
    end
    return false
  end
  
  # Remove a key-value pair
  def remove(key)
    if self._data.contains(key)
      self._data.remove(key)
      # Find key position in the list
      var idx = self._keys.find(key)
      if idx != nil
        self._keys.remove(idx)
      end
      return true
    end
    return false
  end
  
  # Get a value by key, with optional default if key doesn't exist
  def find(key, default)
    return self._data.find(key, default)
  end

  # Access a value by key
  def item(key)
    return self._data[key]
  end
  
  # Set a value by key
  def setitem(key, value)
    return self.insert(key, value)
  end
  
  # Return true if map contains key
  def contains(key)
    return self._data.contains(key)
  end
  
  # Return number of key-value pairs
  def size()
    return self._data.size()
  end

  # Return all sorted keys
  def get_keys()
    return self._keys
  end
  
  # Return iterator to keys in sorted order
  def keys()
    return self._keys.iter()
  end

  # String representation
  def tostring()
    import string
    var result = "{"
    var first = true
    
    for i : 0..self._keys.size()-1
      var key = self._keys[i]
      var val = self._data[key]
      
      if !first
        result += ", "
      end
      first = false
      
      if type(key) == 'string'
        result += string.format("'%s': ", key)
      else
        result += string.format("%s: ", str(key))
      end
      
      if type(val) == 'string'
        result += string.format("'%s'", val)
      else
        result += str(val)
      end
    end
    
    result += "}"
    return result
  end
  
  # Iterator method for 'for x: map' style iteration
  def iter()
    return self._data.iter()
  end

  # Get by index number
  def get_by_index(idx)
    return self._data[self._keys[idx]]
  end
  
  # Clear all key-value pairs
  def clear()
    self._data = {}
    self._keys = []
  end
  
  # Remove entries with a specific value
  def remove_by_value(value)
    var keys_to_remove = []
    
    # First pass: identify all keys with matching values
    for i : 0..self._keys.size()-1
      var key = self._keys[i]
      if self._data[key] == value
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
