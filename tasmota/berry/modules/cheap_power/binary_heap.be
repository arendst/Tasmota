# https://en.wikipedia.org/wiki/Binary_heap
# similar to C++11 std::make_heap

def heapify(array, cmp, i)
  while true
    var m = i, child = 2 * i
    child += 1
    if child >= array.size() return end
    if cmp(array[child], array[m]) m = child end
    child += 1
    if child < array.size() && cmp(array[child], array[m]) m = child end
    if m == i break end
    var e = array[i]
    array[i] = array[m]
    i = m
    array[i] = e
  end
end

var binary_heap = module("binary_heap")

binary_heap.make_heap = def(array, cmp)
  for i: range(size(array) / 2, 0, -1) heapify(array, cmp, i) end
end

binary_heap.remove_heap = def(array, cmp)
  if array.size() == 0
    return array
  elif array.size() == 1
    return array.pop()
  end
  var m = array[0]
  array[0] = array.pop()
  heapify(array, cmp, 0)
  return m
end

binary_heap.sort = def(array, cmp)
  var heap = array.copy()
  binary_heap.make_heap(heap, cmp)
  array.clear()
  for i: 1..heap.size() array.push(binary_heap.remove_heap(heap, cmp)) end
end

return binary_heap
