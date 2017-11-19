# IRremote ESP8266 Library

[![Build Status](https://travis-ci.org/markszabo/IRremoteESP8266.svg?branch=master)](https://travis-ci.org/markszabo/IRremoteESP8266)
[![Average time to resolve an issue](http://isitmaintained.com/badge/resolution/markszabo/IRremoteESP8266.svg)](http://isitmaintained.com/project/markszabo/IRremoteESP8266 "Average time to resolve an issue")
[![Percentage of issues still open](http://isitmaintained.com/badge/open/markszabo/IRremoteESP8266.svg)](http://isitmaintained.com/project/markszabo/IRremoteESP8266 "Percentage of issues still open")
[![GitLicense](https://gitlicense.com/badge/markszabo/IRremoteESP8266)](https://gitlicense.com/license/markszabo/IRremoteESP8266)

This library enables you to **send and receive** infra-red signals on an ESP8266 using Arduino framework (https://github.com/esp8266/Arduino)

## v2.2 Now Available
Version 2.2 of the library is now available. This is a significant internal change to existing versions.

## Upgrading from versions prior to v2.0
You will need to change your pre-v2.0 code slightly to work with post-v2.0 versions of the library. You can read more about the changes on our [wiki](https://github.com/markszabo/IRremoteESP8266/wiki/Upgrading-to-v2.0) page.
Please [report an issue](https://github.com/markszabo/IRremoteESP8266/issues/new) if you find any problems with the documentation or the library itself.

## Troubleshooting
Before reporting an issue or asking for help, please try to follow our [Troubleshooting Guide](https://github.com/markszabo/IRremoteESP8266/wiki/Troubleshooting-Guide) first.

## Frequently Asked Questions
Some common answers to common questions and problems can be found on our [F.A.Q. wiki page](https://github.com/markszabo/IRremoteESP8266/wiki/Frequently-Asked-Questions).

## History
This library was originally based on Ken Shirriff's work (https://github.com/shirriff/Arduino-IRremote/)

[Mark Szabo](https://github.com/markszabo/IRremoteESP8266) has updated the IRsend class to work on ESP8266 and [Sebastien Warin](https://github.com/sebastienwarin/IRremoteESP8266) the receiving & decoding part (IRrecv class).

## Installation
1. Click "Download ZIP"
2. Extract the downloaded zip file
3. Rename the extracted folder to "IRremoteESP8266"
4. Move this folder to your libraries directory (under windows: C:\Users\YOURNAME\Documents\Arduino\libraries\)
5. Restart your Arduino ide
6. Check out the examples

###### Using Git to install library ( Linux )
```
cd ~/Arduino/libraries
git clone https://github.com/markszabo/IRremoteESP8266.git
```
###### To Update to the latest version of the library
`
cd ~/Arduino/libraries/IRremoteESP8266 && git pull
`

## Unit Tests
The [Unit Tests](https://en.wikipedia.org/wiki/Unit_testing) under the test/ directory are for a Unix machine, **not** the micro-controller (ESP8266).
This allows execution under Travis and on the developer's machine.
We can do this from v2.0 of the library onwards, as everything now uses c98-style type definitions.
e.g. uint16_t etc.
Any Arduino/ESP8266 specific code needs to be disabled using something similar to the following lines:
```
#ifndef UNIT_TEST
<Arduino specific code ...>
#endif
```

This is not a perfect situation as we can not obviously emulate hardware specific features and differences. e.g. Interrupts, GPIOs, CPU instruction timing etc, etc.

If you want to run all the tests yourself, try the following:
```
$ cd test
$ make run
```

## Contributing
If you want to [contribute](.github/CONTRIBUTING.md#how-can-i-contribute) to this project, consider:
- [Report](.github/CONTRIBUTING.md#reporting-bugs) bugs and errors
- Ask for enhancements
- [Create issues](.github/CONTRIBUTING.md#reporting-bugs) and [pull requests](.github/CONTRIBUTING.md#pull-requests)
- Tell other people about this library

## Contributors
Available [here](.github/Contributors.md)
