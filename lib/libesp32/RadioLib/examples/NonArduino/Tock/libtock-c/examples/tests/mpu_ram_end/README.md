MPU RAM End
===============

This test checks whether an application can read the last byte in its RAM
allocation and whether it can read the byte immediately after. Usually a
process cannot read the last byte in its RAM allocation because it is taken up
by grants.
