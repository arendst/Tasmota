#/usr/bin/expect

set timeout 100
spawn picocom -b 57600 /dev/ttyUSB0
expect "+READY"
send [lindex $argv 0] 
expect "+OK"
spawn picocom -b 57600 /dev/ttyUSB1
expect "+READY"
send [lindex $argv 0]
expect "+OK"
