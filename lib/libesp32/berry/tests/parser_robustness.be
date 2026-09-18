# Tests that the parser refuses pathologically deep nesting cleanly,
# rather than overflowing the C stack (B-08).

def expect_syntax_error(src, hint)
    var caught = false
    try
        compile(src)
    except 'syntax_error' as e, m
        caught = true
    end
    assert(caught, 'expected syntax_error for ' + hint)
end

# Deeply nested parens: the depth limit should kick in cleanly.
# 100 is well past BE_MAX_PARSER_DEPTH (25) but cheap to construct.
var deep_parens = ''
for i : 0..99 deep_parens = deep_parens + '(' end
deep_parens = deep_parens + '1'
for i : 0..99 deep_parens = deep_parens + ')' end
expect_syntax_error(deep_parens, 'deep parens')

# Deeply nested if/end: same pressure on `block`.
var deep_if = ''
for i : 0..99 deep_if = deep_if + 'if 1 ' end
deep_if = deep_if + '1 '
for i : 0..99 deep_if = deep_if + 'end ' end
expect_syntax_error(deep_if, 'deep if/end')

# A normal-depth nested expression should still compile fine.
compile('var a = (((((((((1 + 2)))))))))')
