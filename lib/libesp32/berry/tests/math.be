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

assert(n != n)      #- nan is never identical to itself -#
