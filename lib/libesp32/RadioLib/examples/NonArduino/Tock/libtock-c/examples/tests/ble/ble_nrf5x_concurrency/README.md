Bluetooth Low Energy Concurrency Test
======================================

The test launches four applications that are transmitting Bluetooth Low Energy
advertisements periodically with different intervals.

A bash script named `run.sh` builds and flashes the applications via tockloader
then verify that all applications are found in your phone or Bluetooth Sniffer

Note, that this only tests the functionality roughly and don't ensure free
from deadlock, race-conditions and similar difficult problems.

Usage
-----------------
```
$ ./run.sh
```

Supported Boards
-----------------
nRF52-DK
