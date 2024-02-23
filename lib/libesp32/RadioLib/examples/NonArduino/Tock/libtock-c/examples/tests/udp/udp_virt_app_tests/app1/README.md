UDP Testing App
=============

A testing app for Imix that tests UDP virtualization.
When combined with app2, it tests that multiple apps cannot bind to the same port,
that single app sending works, that sending without binding fails, and that sending
a too-long packet fails. It also tests that near simultaneous sending from multiple
apps works.

## Running

This application should be tested using the associated `app2` app. Flash one Imix
with both apps simultaneously and run `tockloader listen` to view the results.

Expected output:
```
[APP1] Starting App 1 UDP Test App.
[App2] Starting App2 Test App.
App1 test success!
App2 test success!
```

If the expected output is not received, try setting `DEBUG=1` in both apps to debug.

NOTE: App1 can be tested alone, and should succeed regardless of app2 being flashed.

NOTE2: App1 and App2 running together on a board is also used for testing reception
on a second board. Further details can be found in `udp_lowpan_test.rs` in
`boards/imix/src/` in the main Tock repository.
