import binary_heap

var pi = [3,14,15,92,65,35,89,79,32,38,46,26,43,38,32,79,50,28,84,19,71,69,39]
# Display the elements of pi[] in ascending order
var h = pi.copy()
var less = / a b -> a < b, sort = binary_heap.sort
sort(h, less)
print(h)

# Display the 5 largest elements of pi[]
var cmp = / a b -> a > b
var max = []
binary_heap.make_heap(h, cmp)
var i = 5
while i > 0 max.push(binary_heap.remove_heap(h, cmp)) i -= 1 end
print(max)

# Display the 7 largest elements of pi[] in ascending order of index
h = []
i = 1
while i < pi.size() h.push(i - 1) i += 1 end
cmp = / a b -> pi[a] > pi[b]
if false # less efficient alternative: sort the entire array
  sort(h, cmp)
  max = h[0..6]
  i = 0
else # more efficient: only sort the minimum necessary subset
  max = []
  binary_heap.make_heap(h, cmp)
  i = 7
  while i > 0 max.push(binary_heap.remove_heap(h, cmp)) i -= 1 end
end
sort(max, less)
while i < 7 print(f"{max[i]}: {pi[max[i]]}") i += 1 end
