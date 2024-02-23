Test `yield_for_with_timeout`
====================

This tests the `yield_for_with_timeout` function. It sets a one shot alarm to change
a variable then performs a `yield_for_with_timeout` on that variable with a timeout 
shorter than the alarm, then performs another `yield_for_with_timeout` that 
will timeout after the alarm. An LED is set and cleared based on the result
of the `yield_for_with_timeout` function. The user should see the LED stay off
for 500ms, then turn on for 1s.
