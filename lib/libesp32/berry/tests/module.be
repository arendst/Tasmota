# test for monkey patching of modules

import string
import introspect

var string_orig = string

introspect.setmodule("string", 42)
import string
assert(string == 42)

# set back original value
introspect.setmodule("string", string_orig)
import string
assert(type(string) == 'module')
assert(str(string) == '<module: string>')

#
# demo, how to monkey patch string with a new function `foo()` returning `bar`
#
import string
import introspect
var string_orig = string            # keep a copy of the original string module
var string_alt = module('string')   # create a new module

# function `super()` is a closure to capture the original value of string
string_alt.super = def()
    return string_orig
end

# function `member()` is a clousre to capture the original value of string module
string_alt.member = def(k)
    import introspect
    return introspect.get(string_orig, k, true)
end

string_alt.foo = def() return 'bar' end

# replace the entry for module "string", from now on each `import string` will use `string_alt`
introspect.setmodule("string", string_alt)
import string

# test the new string module
assert(string.tolower('abCD') == 'abcd')
assert(string.foo() == 'bar')
