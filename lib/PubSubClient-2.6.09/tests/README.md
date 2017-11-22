# Arduino Client for MQTT Test Suite

This is a regression test suite for the `PubSubClient` library.

There are two parts:

 - Tests that can be compiled and run on any machine
 - Tests that build the example sketches using the Arduino IDE


It is a work-in-progress and is subject to complete refactoring as the whim takes
me.


## Local tests

These are a set of executables that can be run to test specific areas of functionality.
They do not require a real Arduino to be attached, nor the use of the Arduino IDE.

The tests include a set of mock files to stub out the parts of the Arduino environment the library
depends on.

### Dependencies

 - g++

### Running

Build the tests using the provided `Makefile`:

    $ make

This will create a set of executables in `./bin/`. Run each of these executables to test the corresponding functionality. 

*Note:* the `connect_spec` and `keepalive_spec` tests involve testing keepalive timers so naturally take a few minutes to run through.

## Arduino tests

*Note:* INO Tool doesn't currently play nicely with Arduino 1.5. This has broken this test suite. 

Without a suitable arduino plugged in, the test suite will only check the
example sketches compile cleanly against the library.

With an arduino plugged in, each sketch that has a corresponding python
test case is built, uploaded and then the tests run.

### Dependencies

 - Python 2.7+
 - [INO Tool](http://inotool.org/) - this provides command-line build/upload of Arduino sketches

### Running

The test suite _does not_ run an MQTT server - it is assumed to be running already.
 
    $ python testsuite.py

A summary of activity is printed to the console. More comprehensive logs are written
to the `logs` directory.

### What it does

For each sketch in the library's `examples` directory, e.g. `mqtt_basic.ino`, the suite looks for a matching test case
`testcases/mqtt_basic.py`.

The test case must follow these conventions:
 - sub-class `unittest.TestCase`
 - provide the class methods `setUpClass` and `tearDownClass` (TODO: make this optional)
 - all test method names begin with `test_`
 
The suite will call the `setUpClass` method _before_ uploading the sketch. This
allows any test setup to be performed before the sketch runs - such as connecting
a client and subscribing to topics.


### Settings

The file `testcases/settings.py` is used to config the test environment.

 - `server_ip` - the IP address of the broker the client should connect to (the broker port is assumed to be 1883).
 - `arduino_ip` - the IP address the arduino should use (when not testing DHCP).

Before each sketch is compiled, these values are automatically substituted in. To
do this, the suite looks for lines that _start_ with the following:

     byte server[] = {
     byte ip[] = {

and replaces them with the appropriate values.




