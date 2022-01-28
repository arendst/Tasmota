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
