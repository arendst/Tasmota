MPU Walk Region
===============

This test verfies that an application can read all of the memory it has been
allocated in ram and flash.

If user button zero is held down, this test will overrun the next region that
it walks, which should cause an MPU fault.
