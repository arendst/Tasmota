Crash Immediately via Invalid Syscall
=====================================

This app does only one thing: it immediately calls a non-existent syscall.
It has no other instructions.

This should cause the app to fault immediately, and the kernel should handle
that according to its policy.

Might be useful for testing.
