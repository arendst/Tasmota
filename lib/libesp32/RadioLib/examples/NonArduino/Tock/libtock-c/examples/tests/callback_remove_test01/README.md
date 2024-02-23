Test Removing Callbacks
====================

This tests that a removed callback is not called even if it is queued (but not
delivered) when the callback is unsubscribed from.


Expected Output
---------------

```
Initialization complete. Entering main loop
[SUCCESS] The callback was successfully canceled
```

Bad Output
----------

```
Initialization complete. Entering main loop
[ERROR] Timer still fired yet we disabled the callback!!
```
