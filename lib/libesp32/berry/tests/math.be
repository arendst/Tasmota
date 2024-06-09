import math

#- nan -#
n = math.nan
assert(str(n) == 'nan')
assert(math.isnan(n))
assert(math.isnan(n+n))
assert(math.isnan(n+1))
assert(math.isnan(n*0))

assert(!math.isnan(0))
assert(!math.isnan(1.5))
assert(!math.isnan(math.inf))

assert(n != n)      #- nan is never identical to itself -#

#- inf -#
i = math.inf
assert(str(i) == 'inf')
assert(str(-i) == '-inf')       # inf has a sign
assert(math.isinf(i))
assert(math.isinf(i+i))
assert(math.isinf(i+1))
# the following result in nan
assert(math.isnan(i*0))
assert(math.isnan(i-i))
assert(math.isnan(i/i))

assert(!math.isinf(0))
assert(!math.isinf(1.5))
assert(!math.isinf(math.nan))

assert(i == i)      #- equality works for inf -#
assert(i == i+1)

#- nan and inf convert to null in json -#
import json

m_nan = {"v": math.nan}
assert(json.dump(m_nan) == '{"v":null}')
m_inf1 = {"v": math.inf}
assert(json.dump(m_inf1) == '{"v":null}')
m_inf2 = {"v": -math.inf}
assert(json.dump(m_inf2) == '{"v":null}')
m_v = {"v": 3.5}
assert(json.dump(m_v) == '{"v":3.5}')

# math.round
assert(math.round(3) == 3)
assert(math.round(3.2) == 3)
assert(math.round(3.5) == 4)
assert(math.round(3.6) == 4)

assert(math.round(-3) == -3)
assert(math.round(-3.2) == -3)
assert(math.round(-3.5) == -4)
assert(math.round(-3.6) == -4)

assert(math.round() == 0)

