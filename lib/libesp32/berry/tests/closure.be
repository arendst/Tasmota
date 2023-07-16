#- test for issue #105 -#

l=[]
def tick()
  var start=100
  for i : 1..3
    l.push(def () return [i, start] end)
  end
end
tick()
assert(l[0]() == [1, 100])
assert(l[1]() == [2, 100])
assert(l[2]() == [3, 100])

# the following failed to compile #344
def test() var nv = 1 var f = def() nv += 2*1 print(nv) end end
