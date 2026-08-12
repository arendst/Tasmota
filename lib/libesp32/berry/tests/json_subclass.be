#- json.dump() must recognize subclasses of `map` and `list`,
   and must NOT be fooled by unrelated classes named `map`/`list` -#

import json

#- direct subclass of map -#
class MyMap : map end
var m = MyMap()
m["a"] = 1
assert(json.dump(m) == '{"a":1}')

#- direct subclass of list -#
class MyList : list end
var l = MyList()
l.push(1)
l.push("x")
assert(json.dump(l) == '[1,"x"]')

#- deeper hierarchy -#
class MyMap2 : MyMap end
var m2 = MyMap2()
m2["b"] = true
assert(json.dump(m2) == '{"b":true}')

#- subclass with its own instance variables and init -#
class MyMap3 : map
  var extra
  def init()
    super(self).init()
    self.extra = 42
  end
end
var m3 = MyMap3()
m3["c"] = nil
assert(json.dump(m3) == '{"c":null}')

#- nested inside plain containers and inside each other -#
assert(json.dump({"k": m, "j": l}) == '{"k":{"a":1},"j":[1,"x"]}')
var nested = MyList()
nested.push(m)
assert(json.dump(nested) == '[{"a":1}]')

#- formatted output still works for subclasses -#
assert(json.dump(m, "format") == '{\n  "a": 1\n}')

#- A user class that merely shares the name `map`/`list` must be dumped as a
   string, not treated as a json container. The previous root-class-name
   comparison matched on the name only and aborted in `be_strconcat()`.
   The classes are declared inside a function so that the builtins are not
   shadowed. -#
def make_fake_map()
  class map
    def tostring() return "not-a-map" end
  end
  return map()
end
assert(json.dump(make_fake_map()) == '"not-a-map"')

def make_fake_list()
  class list
    def tostring() return "not-a-list" end
  end
  return list()
end
assert(json.dump(make_fake_list()) == '"not-a-list"')

#- sanity: builtins still work -#
assert(json.dump({"a": [1, 2]}) == '{"a":[1,2]}')
