Console Receive Timeout Test
=======================

This app tests canceling asynchronous reads from the console. The application
reads from the console and sets a 5 second time. When the timer fires, it
aborts the read from the console. If aborts are working correctly, this
should result in a receive callback containing anything that was read so far.

Running
=======================
Load the application. Run `tockloader listen` and type a few characters
at the console.

Successful Output
======================
After 5 seconds, you should see a message saying

Userspace call to read console returned: [text]

where [text] are the characters you typed.

Details
=======================

If the device has the process console capsule loaded in the
kernel, then characters typed at the console will be echoed. You can
tell if the process console is running by either looking for a
"Starting process console" message at boot or by typing 'list' or any
text, followed by a newline: the process console will either print a
help message or print output for the command typed. For example, if
the devices is running the process console and you type 'afaf' you
will see the following output:

pal@ubuntu:~/src/libtock-c/examples/tests/console_timeout$ tockloader listen
No device name specified. Using default "tock"
Using "/dev/ttyUSB0 - imix IoT Module - TockOS"

Listening for serial output.
Starting process console
Initialization complete. Entering main loop
afafUserspace call to read console returned: afaf
