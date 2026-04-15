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

#- min and max -#
def assert_error(f, error_type)
    try
        f()
        assert(false, 'unexpected execution flow')
    except .. as e, m
        assert(e == error_type)
    end
end

assert(math.min() == nil)
assert(math.min(0) == 0)
assert(math.min(0,2,10,56) == 0)
assert(math.min(4, 2, -10, 3) == -10)
assert(math.min(4, 2) == 2)

# result is int unless one of the parameters is real
assert(type(math.min(4, 2, -10, 3)) == 'int')
assert(type(math.min(4, 2, -10.0, 3)) == 'real')

assert(math.min(-3.4, 5) == -3.4)

# test invalid parameters
assert_error(def () return math.min(4, nil) end, 'type_error')
assert_error(def () return math.min(4, "", 4.5) end, 'type_error')

# math.max
assert(math.max() == nil)
assert(math.max(0) == 0)
assert(math.max(0,2,10,56) == 56)
assert(math.max(4, 2, -10, 3) == 4)
assert(math.max(4, 2) == 4)

assert(type(math.max(4, 2, 10, 3)) == 'int')
assert(type(math.max(4, 2, 10.0, 3)) == 'real')

assert(math.max(-3.4, 5) == 5)

assert_error(def () return math.max(4, nil) end, 'type_error')
assert_error(def () return math.max(4, "", 4.5) end, 'type_error')

# math.abs
assert(math.abs(3) == 3)
assert(math.abs(-3) == 3)
assert(math.abs(0) == 0)
assert(math.abs(-3.5) == 3.5)
assert(math.abs(3.5) == 3.5)
assert(math.abs() == 0)   # no-arg fallback returns 0.0

# math.ceil
assert(math.ceil(3.0) == 3)
assert(math.ceil(3.1) == 4)
assert(math.ceil(3.9) == 4)
assert(math.ceil(-3.1) == -3)
assert(math.ceil(-3.9) == -3)
assert(math.ceil(0) == 0)
assert(math.ceil() == 0)

# math.floor
assert(math.floor(3.0) == 3)
assert(math.floor(3.1) == 3)
assert(math.floor(3.9) == 3)
assert(math.floor(-3.1) == -4)
assert(math.floor(-3.9) == -4)
assert(math.floor(0) == 0)
assert(math.floor() == 0)

# math.sqrt
assert(math.sqrt(4) == 2)
assert(math.sqrt(9) == 3)
assert(math.sqrt(0) == 0)
assert(math.isnan(math.sqrt(-1)))
assert(math.sqrt() == 0)

# math.exp / math.log
# use tolerance for float vs double compatibility
var eps = 1e-5
assert(math.abs(math.exp(0) - 1.0) < eps)
assert(math.abs(math.exp(1) - 2.71828) < 1e-4)
assert(math.exp() == 0)

assert(math.abs(math.log(1) - 0.0) < eps)
assert(math.abs(math.log(math.exp(1)) - 1.0) < eps)
assert(math.isnan(math.log(-1)))
assert(math.log() == 0)

# math.log10
assert(math.abs(math.log10(1) - 0.0) < eps)
assert(math.abs(math.log10(10) - 1.0) < eps)
assert(math.abs(math.log10(100) - 2.0) < eps)
assert(math.log10() == 0)

# math.pow
assert(math.pow(2, 0) == 1)
assert(math.pow(2, 10) == 1024)
assert(math.abs(math.pow(2, 0.5) - math.sqrt(2)) < eps)
assert(math.pow() == 0)

# math.sin / math.cos / math.tan
assert(math.abs(math.sin(0)) < eps)
assert(math.abs(math.sin(math.pi) ) < 1e-5)
assert(math.abs(math.cos(0) - 1.0) < eps)
assert(math.abs(math.cos(math.pi) + 1.0) < 1e-5)
assert(math.abs(math.tan(0)) < eps)
assert(math.sin() == 0)
assert(math.cos() == 0)
assert(math.tan() == 0)

# math.asin / math.acos / math.atan
assert(math.abs(math.asin(0)) < eps)
assert(math.abs(math.asin(1) - math.pi/2) < eps)
assert(math.abs(math.acos(1)) < eps)
assert(math.abs(math.acos(0) - math.pi/2) < eps)
assert(math.abs(math.atan(0)) < eps)
assert(math.abs(math.atan(1) - math.pi/4) < eps)
assert(math.asin() == 0)
assert(math.acos() == 0)
assert(math.atan() == 0)

# math.atan2
assert(math.abs(math.atan2(0, 1)) < eps)
assert(math.abs(math.atan2(1, 0) - math.pi/2) < eps)
assert(math.abs(math.atan2(1, 1) - math.pi/4) < eps)
assert(math.atan2() == 0)

# math.sinh / math.cosh / math.tanh
assert(math.abs(math.sinh(0)) < eps)
assert(math.abs(math.cosh(0) - 1.0) < eps)
assert(math.abs(math.tanh(0)) < eps)
assert(math.sinh() == 0)
assert(math.cosh() == 0)
assert(math.tanh() == 0)

# math.deg / math.rad
assert(math.abs(math.deg(math.pi) - 180.0) < 1e-4)
assert(math.abs(math.deg(math.pi/2) - 90.0) < 1e-4)
assert(math.abs(math.rad(180) - math.pi) < 1e-4)
assert(math.abs(math.rad(90) - math.pi/2) < 1e-4)
assert(math.deg() == 0)
assert(math.rad() == 0)

# math.srand / math.rand
math.srand(42)
var r1 = math.rand()
assert(type(r1) == 'int')
math.srand(42)
var r2 = math.rand()
assert(r1 == r2)   # same seed -> same first value
math.srand()       # no-arg call (no-op, returns nil)

# math.pi / math.imax / math.imin
assert(math.abs(math.pi - 3.14159) < 1e-4)
assert(math.imax > 0)
assert(math.imin < 0)
