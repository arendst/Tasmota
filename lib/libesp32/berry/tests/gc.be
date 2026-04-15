import gc

# gc.collect() returns nil
assert(gc.collect() == nil)

# gc.allocated() returns a number (int for normal heap sizes)
var mem = gc.allocated()
assert(type(mem) == 'int' || type(mem) == 'real')
assert(mem > 0)

# allocating objects increases memory usage
var before = gc.allocated()
var l = []
for i : 0..99
    l.push(str(i))
end
var after = gc.allocated()
assert(after > before)

# collecting after freeing should reduce or maintain allocation
l = nil
gc.collect()
var post = gc.allocated()
assert(post < after)

# gc.allocated() is consistent across calls (non-decreasing without allocation)
var a1 = gc.allocated()
var a2 = gc.allocated()
assert(a2 >= a1 || a2 == a1)
