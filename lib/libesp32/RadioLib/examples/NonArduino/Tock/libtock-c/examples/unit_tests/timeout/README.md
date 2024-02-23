# Timeout unit test

This simple unit test is a designed to validate that the unit test
infrastructure itself works correctly. It provides three tests, which
pass, fail, and timeout, respectively.


If you load this app along with the `unit_test_supervisor` in `examples/services/`,
you should see the following console output:

```
2.000: pass             [✓]
2.001: fail             [FAILED]
2.002: timeout          [ERROR: Timeout]
Summary 1: [1/3] Passed, [1/3] Failed, [1/3] Incomplete
```

The tests should be reported in exactly the order given above.
