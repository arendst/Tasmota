# Berry Language Reference

Compact reference for Generative AI. For Tasmota-specific features, see `BERRY_TASMOTA.md`.

## Comments
```berry
# Line comment
#- Block multiline comment -#
```

## Literals
```berry
40  0x80  3.14  1.1e-6  true  false  nil
'string'  "string"  "a" "b"  # adjacent strings concatenate
```
Escapes: `\n \r \t \\ \" \' \0 \xhh \uXXXX`

## Keywords
```
if elif else while for def end class break continue return
true false nil var do import as try except raise static
```

## Types
Simple: nil, int, real, bool, string, function, class, instance, module, comptr
Built-in classes: list, map, range, bytes

Note: `type()` returns the base type, not the class name. For list/map/range/bytes instances, `type()` returns `'instance'`. Use `classname(x)` to get `'list'`, `'map'`, etc.

## Variables
```berry
a = 1              # global if no var declaration in scope
var a              # nil
var a = 1
var a, b = 1, 2
```
Outermost block = global; inner blocks = local. `do ... end` creates a scope.

## Operators
```
Arithmetic:   + - * / %  (unary -)
Relational:   < <= == != >= >
Logical:      && || !  (short-circuit)
Bitwise:      ~ & | ^ << >>
Assignment:   = += -= *= /= %= &= |= ^= <<= >>=
Walrus:       :=  (assign + return value in expressions)
Other:        . [] ?: ..
```
`..` = range (int..int), string concat (auto-converts RHS), or list/bytes append-in-place.
`"ab" * 3` → `"ababab"`. Accepts int or bool. `"s" * false` → `""`.

## Control Flow
```berry
if cond ... elif cond2 ... else ... end
while cond ... end
for i: 0..5 ... end              # inclusive range
for i: range(0, 10, 2) ... end   # stepped
for item: [1,2,3] ... end        # container
for val: mymap ... end            # map values
for key: mymap.keys() ... end     # map keys
for i: 10.. ... end               # open-ended (upper = MAXINT)
do ... end                        # scoped block
break  continue
```

## Import
```berry
import math
import hardware as hw
```

## Functions
```berry
def add(a, b) return a + b end

# Anonymous
f = def (a, b) return a + b end

# Lambda (single expression, commas optional between params)
f = / a b -> a + b
f = /-> nil                       # zero-arg

# Varargs
def f(a, *args) end               # args is a list

# Dynamic call (last arg list is unpacked)
call(func, 1, [2, 3])            # func(1, 2, 3)
```

**Closures** capture variables by reference from enclosing scope.

## Classes
```berry
class Person
    var name, age
    def init(name, age) self.name = name self.age = age end
    def greet() print("Hello, I'm", self.name) end
end
```

**Static members:**
```berry
class C
    static var PI = 3.14          # preferred
    static PI2 = 6.28             # also valid
    static def square(x) return x * x end
    static class Inner ... end    # nested class
end
```
Instance methods access statics via `self`. Static methods get implicit `_class`.
Static methods can be called on the class (`C.square(5)`) or on an instance (`c.square(5)`), but they do not receive `self`.

**Inheritance:** `class B : A ... end`
```berry
super(self).init(args)            # auto-infers parent from calling context
super(self, TargetClass)          # explicit ancestor
super(Class)                      # parent class of a class
```

**Operator overloading** — define methods named after the operator:
Overloadable: `+ - * / % < <= == != > >= & | ^ << >> .. ~ ()`
```berry
def +(other) ... end    # binary
def -() ... end         # unary neg
def ~() ... end         # unary flip
def ()() ... end        # call operator
def tostring() ... end  # string conversion
def tobool() ... end    # boolean conversion
def toint() ... end     # int() conversion
```

**Virtual member dispatch:** define `member(name)` / `setmember(name, value)` on a class or module to handle undefined attributes dynamically. Return `import undefined` from `member()` to signal attribute doesn't exist.

**Indirect member access:** `obj.('name')` — dynamic member name from expression.

## Iterator Protocol
`for` calls `iter()` on the object, which returns a closure. The closure yields values and raises `'stop_iteration'` when done.

## List
```berry
l = [1, 2, 3]
l[0]  l[-1]  l[1..3]  l[1..]  l[0..-2]
l.push(x)  l.pop()  l.pop(i)  l.insert(i, x)  l.remove(i)
l.resize(n)  l.clear()  l.size()  l.reverse()  l.copy()
l.find(x)              # index or nil
l.concat()  l.concat("-")
l + [4,5]              # new list (does not mutate l)
l .. x                 # append in place, returns l
l.keys()               # range of indices
[1,2] == [1,2]         # deep comparison
```

## Map
```berry
m = {"a": 1, 0: true}
m["a"]                 # raises key_error if missing
m.find("a")            # nil if missing
m.find("a", default)
m["k"] = v
m.insert("k", v)      # true if new, false if exists
m.remove("k")  m.size()  m.contains("k")
```

## Range
```berry
0..5                   # inclusive, increment 1
10..                   # open-ended
range(1, 10)           # explicit
range(1, 10, 2)        # custom increment
range(30, 0, -3)       # negative increment
r.lower()  r.upper()  r.incr()  r.setrange(lo, hi)  r.setrange(lo, hi, step)
```

## String Operations
```berry
s[0]  s[-1]  s[1..3]  s[1..]  s[0..-2]   # slicing, tolerant of out-of-range
```
Strings have no methods. Use `import string` for string operations (e.g. `string.find(s, sub)`, not `s.find(sub)`).

## Bytes
```berry
b = bytes()  bytes("1122AA")  bytes(10)  bytes(-8)  # -N = fixed size
b[0]  b[1..2]  b[0] = 0xFF
b.size()  b.resize(n)  b.clear()  b.reverse()  b.copy()
# Structured I/O (size: 1,2,-2,4,-4; negative=big-endian)
b.get(off, sz)  b.geti(off, sz)  b.set(off, val, sz)  b.add(val, sz)
b.getfloat(off)  b.setfloat(off, v)  b.addfloat(v)
b.setbytes(off, other)
b.tohex()  b.asstring()  b.tob64()
b.fromhex(s)  b.fromstring(s)  b.fromb64(s)
b1 .. b2               # append in place
b1 == b2               # content equality
```

## File I/O
```berry
f = open("file.txt", "r")
f.read()  f.readline()  f.readbytes()  f.close()
f = open("file.txt", "w")
f.write(s)  f.flush()  f.close()
f.seek(pos)  f.tell()  f.size()
```

## Built-in Functions
```berry
print(...)  input("prompt")
type(x)  classname(x)  classof(x)
isinstance(inst, cls)  issubclass(cls, parent)
int(x)  real(x)  bool(x)  str(x)  number(s)  size(x)
compile("code")  compile("file.be", "file")
super(self)  super(self, Class)  super(Class)
assert(cond)  assert(cond, "msg")
format(fmt, ...)  call(func, args...)
module()  module("name")
```

## String Module
```berry
import string
string.find(s, sub [, start [, end]])     # -1 if not found
string.count(s, sub [, start [, end]])
string.split(s, sep [, max_splits])
string.byte("A")  string.char(65)
string.toupper(s)  string.tolower(s)
string.tr(s, from, to)  string.replace(s, old, new)
string.startswith(s, prefix [, case_insensitive])
string.endswith(s, suffix [, case_insensitive])
string.escape(s [, single_quote])
string.format(fmt, ...)                   # %d %i %u %o %x %X %f %e %E %g %G %s %c %q %%
f"Value: {x}"  f"{x:.2f}"  f"{x=}"  f"{{literal braces}}"
```

## Math Module
```berry
import math
math.pi  math.inf  math.nan  math.imin  math.imax
math.abs(x)  math.floor(x)  math.ceil(x)  math.round(x)
math.min(...)  math.max(...)
math.sqrt(x)  math.pow(x,y)  math.exp(x)  math.log(x)  math.log10(x)
math.sin(x)  math.cos(x)  math.tan(x)
math.asin(x)  math.acos(x)  math.atan(x)  math.atan2(y,x)
math.deg(rad)  math.rad(deg)
math.srand(seed)  math.rand()
math.isinf(x)  math.isnan(x)
```

## JSON Module
```berry
import json
json.load('{"a":1}')          # parse, nil on error
json.dump(obj)                 # compact
json.dump(obj, "format")       # pretty
```

## Regular Expressions
```berry
import re
re.search(pat, str)            # ['match', 'group1', ...] or nil
re.match(pat, str)             # anchored to start
re.match2(pat, str)            # [match_len, 'group1', ...]
re.searchall(pat, str)         # list of all matches
re.matchall(pat, str)          # consecutive matches from start
re.split(pat, str)
# Pre-compiled:
p = re.compile(pat)
p.search(s)  p.match(s)  p.match2(s)  p.searchall(s)  p.matchall(s)  p.split(s)
```

## OS Module
```berry
import os
os.getcwd()  os.chdir(p)  os.mkdir(p)  os.remove(p)
os.listdir()  os.listdir(p)  os.system(cmd)  os.exit()
os.path.exists(p)  os.path.isfile(p)  os.path.isdir(p)
os.path.split(p)  os.path.splitext(p)  os.path.join(a, b)
```

## Global Module
```berry
import global
global()                       # list all global names
global.contains("name")
global.var_name                # get (nil if absent)
global.var_name = 42           # set
global.("dynamic")             # indirect access
global.undef("name")           # remove
```

## Introspect Module
```berry
import introspect
introspect.members(obj)        # list member names
introspect.get(obj, "attr")    # nil if missing (protected)
introspect.set(obj, "attr", v)
introspect.contains(obj, "a")
introspect.name(obj)           # name of class/closure/module
introspect.ismethod(fn)
introspect.module("math")      # dynamic import, no global created
introspect.setmodule("n", m)   # override cached module
introspect.toptr(obj)          # to comptr
introspect.fromptr(ptr)        # comptr back to object
introspect.solidified(obj)     # is constant/ROM?
```

## Exception Handling
Exceptions: `assert_failed` `attribute_error` `index_error` `io_error` `key_error` `runtime_error` `stop_iteration` `syntax_error` `type_error` `unrealized_error` `value_error`

```berry
raise "error_type", "message"
try ... except "type" as e, msg ... end
try ... except "t1", "t2" as e, msg ... end
try ... except .. as e, msg ... end        # catch all
```
Many functions return nil on error instead of raising (json.load, map.find, list.find).

## Preprocessor
Directives must start at beginning of line (`#` mid-line = comment):
```berry
#define NAME 1       # truthy
#define NAME 0       # falsy
#define NAME         # truthy (defined, no value)
#undef NAME
#if NAME ... #elif NAME2 ... #else ... #endif
#if !UNDEFINED ... #endif    # negation
```
Nesting up to 8 levels. `#define` inside skipped blocks is ignored.
Runtime: `import preproc` → `preproc.define('X','1')` `preproc.undef('X')` `preproc.clear()`
