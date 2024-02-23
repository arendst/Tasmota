MPU Heap End
===============

This test checks that an application can read to the end of
its heap, then walks forward in memory until it faults. This
finds the first non-readable address in process's RAM.
