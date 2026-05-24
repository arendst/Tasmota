#-
  Property-Based Testing (PBT) Helper Library for Berry

  This module provides lightweight randomized testing utilities for Berry.
  Property-based testing works by:
    1. Defining a "property" — a condition that must hold for ALL valid inputs
    2. Generating many random inputs
    3. Checking the property holds for each input

  Unlike unit tests that check specific examples, PBT explores the input space
  randomly to find edge cases the developer might not think of.

  Usage:
    # In your test file, inline or import these helpers, then:
    pbt_run('my_property', def (i)
        var input = pbt_random_int(0, 100)
        var result = my_function(input)
        assert(result >= 0, 'result must be non-negative')
        return true
    end, 100)
-#

import math
import string

# Fixed seed for reproducible test runs — same seed always produces
# the same sequence of "random" values, making failures reproducible.
math.srand(12345)

# Generate a random integer in the inclusive range [lo, hi].
# Uses math.rand() with modulo to map into the desired range.
def pbt_random_int(lo, hi)
    if lo >= hi return lo end
    var span = hi - lo + 1
    var r = math.rand()
    if r < 0 r = -r end
    return lo + (r % span)
end

# Generate a random string of length between min_len and max_len
# using printable ASCII characters (codes 32-126).
# May include any printable char — quotes, backslashes, etc.
# For strings safe to embed in Berry source code, use pbt_random_safe_string().
def pbt_random_string(min_len, max_len)
    var len = pbt_random_int(min_len, max_len)
    var s = ''
    for i : 0 .. len - 1
        s += string.char(pbt_random_int(32, 126))
    end
    return s
end

# Generate a random valid Berry identifier.
# Identifiers start with a letter or underscore, followed by
# letters, digits, or underscores. Length is 1-12 characters.
def pbt_random_ident()
    var starts = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_'
    var chars = starts + '0123456789'
    var len = pbt_random_int(1, 12)
    var s = starts[pbt_random_int(0, 52)]
    for i : 1 .. len - 1
        s += chars[pbt_random_int(0, 62)]
    end
    return s
end

# Pick a random element from a list.
def pbt_random_choice(lst)
    return lst[pbt_random_int(0, lst.size() - 1)]
end

# Run a property test function for N iterations (default 100).
# The function `fn` receives the iteration index and should either:
#   - return true (or nil) on success
#   - call assert() to fail with a message
#   - raise an exception on failure
# On failure, prints the iteration number and re-raises for diagnosis.
def pbt_run(name, fn, iterations)
    if iterations == nil iterations = 100 end
    for i : 0 .. iterations - 1
        try
            var result = fn(i)
            if result != nil && result != true
                assert(false, 'PBT ' + name + ' failed at iteration ' + str(i))
            end
        except .. as e, m
            print('PBT FAIL: ' + name + ' iteration ' + str(i) + ': ' + e + ' - ' + m)
            assert(false, 'PBT ' + name + ' failed at iteration ' + str(i) + ': ' + e + ' - ' + m)
        end
    end
end

# Berry keywords and preprocessor directive names that must not be
# used as random macro names (they would conflict with the parser).
var _pbt_reserved = [
    'if', 'elif', 'else', 'while', 'for', 'def', 'end', 'class',
    'break', 'continue', 'return', 'true', 'false', 'nil', 'var',
    'do', 'import', 'as', 'try', 'except', 'raise', 'static',
    'define', 'undef', 'endif'
]

# Generate a random identifier that is NOT a Berry keyword or
# preprocessor directive name. Retries until a safe name is found.
def pbt_random_safe_ident()
    while true
        var id = pbt_random_ident()
        var ok = true
        for kw : _pbt_reserved
            if id == kw ok = false break end
        end
        if ok return id end
    end
end

# Generate a random string safe for embedding inside Berry string literals.
# Excludes characters that would break string parsing:
#   " (34) - would close double-quoted strings
#   ' (39) - would close single-quoted strings
#   $ (36) - would start a translatable string expression ($IDENT"...")
#   \ (92) - would start escape sequences
#   # (35) - would start comments
def pbt_random_safe_string(min_len, max_len)
    var len = pbt_random_int(min_len, max_len)
    var s = ''
    for i : 0 .. len - 1
        var c = pbt_random_int(32, 126)
        while c == 34 || c == 39 || c == 92 || c == 35 || c == 36
            c = pbt_random_int(32, 126)
        end
        s += string.char(c)
    end
    return s
end
