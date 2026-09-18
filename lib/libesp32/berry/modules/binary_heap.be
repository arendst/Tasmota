# https://en.wikipedia.org/wiki/Binary_heap
# This allows to choose the M first elements of an N-sized array
# with respect to a comparison predicate cmp that defines a total order.
# This avoids the overhead of sorting the entire array and then picking
# the first elements.  This is related to a priority queue.
# We also define a binary heap based sort() of an entire array.

var binary_heap = module("binary_heap")

binary_heap._heapify = def(array, cmp, i)
  var m = i, child, e, am, ac
  while true
    child = 2 * i + 1
    if child >= array.size() return end
    ac = array[child]
    am = array[m]
    if cmp(ac, am) m = child am = ac end
    child += 1
    if child < array.size()
      ac = array[child]
      if cmp(ac, am) m = child am = ac end
    end
    if m == i break end
    array[m] = array[i]
    array[i] = am
    i = m
  end
end

# similar to C++11 std::make_heap
binary_heap.make_heap = def(array, cmp)
  var i = size(array) / 2
  while i >= 0 binary_heap._heapify(array, cmp, i) i -= 1 end
end

# similar to C++11 std::pop_heap, but removes and returns the element
binary_heap.remove_heap = def(array, cmp)
  var m = array.size()
  if m < 2 return m == 1 ? array.pop() : nil end
  m = array[0]
  array[0] = array.pop()
  binary_heap._heapify(array, cmp, 0)
  return m
end

# https://en.wikipedia.org/wiki/Heapsort
binary_heap.sort = def(array, cmp)
  var i = array.size(), heap = array.copy()
  binary_heap.make_heap(heap, cmp)
  array.clear()
  while i > 0 array.push(binary_heap.remove_heap(heap, cmp)) i -= 1 end
end

return binary_heap
