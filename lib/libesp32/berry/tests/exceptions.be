
try
    for k: 0..1 assert({'a':1}.contains('b'), 'failure') end
except .. as e,m
    assert(e == "assert_failed")
    assert(m == "failure")
end
