Test Multiple Alarms
====================

This tests the virtual alarms available to userspace. For each LED on the
board, it sets up a repeating alarm, with a 1 second spacing. At each alarm,
the corresponding LED is blinked for 300ms. This exercises both multiple
repeating alarms and the synchronous `delay_ms` concurrently.
