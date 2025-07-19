---
name: Problem report
about: Create a report to help us improve
title: ''
labels: ''
assignees: ''

---

### Version/revision of the library used
_Typically located in the `library.json` & `src/IRremoteESP8266.h` files in the root directory of the library.
e.g. v2.0.0, or 'master' as at 1st of June, 2017. etc._

### Describe the bug
A clear and concise description of what the bug is.

#### To Reproduce
_What steps did you do, and what did or didn't actually happen? How can we reproduce the issue?_

_e.g._
_1. Initialise the IRsend class._
_2. `IRsend.sendFoobar(0xdeadbeef);`_
_3. Foobar BBQ went into Cow(er)-saving mode and fried me a couple of eggs instead._

#### Example code used
_Include all relevant code snippets or links to the actual code files. Tip: [How to quote your code so it is still readable in the report](https://github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet#code)._

#### Expected behaviour
_What steps did you do and what should it have done? A clear and concise description of what you expected to happen._

_e.g._
_1. Initialise the IRsend class._
_2. `IRsend.sendFoobar(0xdeadbeef);`_
_3. Foobar branded BBQ turns on and cooks me some ribs._

#### Output of raw data from [IRrecvDumpV2.ino](https://github.com/crankyoldgit/IRremoteESP8266/blob/master/examples/IRrecvDumpV2/IRrecvDumpV2.ino) or [V3](https://github.com/crankyoldgit/IRremoteESP8266/blob/master/examples/IRrecvDumpV3/) (if applicable)
_Include some serial text of the raw dumps of what the device saw._

_**Note**: Output from Tasmota is not acceptable. We can't easily use their raw format._


### What brand/model IR demodulator are you using?
_Brand/Model code_ or _None_

_If VS1838b: That is likely your problem. Please buy a better one & try again. Reporting a capture or decode issue when you use one of these is effectively wasting our & your time. See the [FAQ](https://github.com/crankyoldgit/IRremoteESP8266/wiki/Frequently-Asked-Questions#Help_Im_getting_very_inconsistent_results_when_capturing_an_IR_message_using_a_VS1838b_IR_demodulator)_

### Circuit diagram and hardware used (if applicable)
_Link to an image of the circuit diagram used. Part number of the IR receiver module etc. ESP8266 or ESP32 board type._

### I have followed the steps in the [Troubleshooting Guide](https://github.com/crankyoldgit/IRremoteESP8266/wiki/Troubleshooting-Guide) & read the [FAQ](https://github.com/crankyoldgit/IRremoteESP8266/wiki/Frequently-Asked-Questions)
_Yes/No._

### Has this library/code previously worked as expected for you?
_Yes/No. If "Yes", which version last worked for you?_

### Other useful information
_More information is always welcome. Be verbose._
