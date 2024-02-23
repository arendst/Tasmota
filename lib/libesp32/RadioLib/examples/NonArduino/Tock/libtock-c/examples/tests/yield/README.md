Test `yield`
====================
Author: Philip Levis <pal@cs.stanford.edu>
Date: 1/21/21

This tests that yield-wait and yield-no-wait work correctly.

A correct test should see "spinning", followed by "waiting",
followed by "spinning." The "spinning" message indicates the 
application is spinning on `yield_no_wait` until it returns true
(the timer callback fires). The "waiting" message indicates the
application is waiting on `yield` until the kernel wakes it up
with a timer callback.

