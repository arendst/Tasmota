# IRremoteESP8266 Library API Documentation {#mainpage}

## Getting Started

### The basics
For sending messages, look at the IRsend class.

For receiving messages, look at the IRrecv & decode_results classes.

### Air Conditioners
For _generic_ Air Conditioner control, look at the IRac class & the
stdAc::state_t structure.

For _detailed_ Air Conditioner control, you need to determine what protocol the
library detects your remote/Air Conditioner to be, look into the appropriate
`src/ir_Protocol.[h|cpp]` files and use the appropriate class object.
e.g. if `IRrecvDumpV2` (or better) detects the protocol as `KELVINATOR`,
open the `src/ir_Kelvinator.*` files, and examine the IRKelvinatorAC class the
methods available to create/decode/send `KELVINATOR` messages with all the
abilities the library offers. You can also select it from the
[Classes](annotated.html) menu above.

Various native constants & options for a given Protocol's class object can be
found in the associated header file for that protocol.

## Examples
Most of the common uses of this library's APIs have demonstration code
available under the [examples](https://github.com/crankyoldgit/IRremoteESP8266/tree/master/examples)
directory. It ranges from trivial examples to complex real-world project code.

## Tuning
The most commonly used & needed knobs for controlling aspects of this library
are available via run-time class methods or at class-object instantiation.
Again, you are referred to the IRsend & IRrecv classes.

### Advanced
Certain addition constants and options are available as compile-time tweaks.
You should inspect [IRremoteESP8266.h](https://github.com/crankyoldgit/IRremoteESP8266/blob/master/src/IRremoteESP8266.h),
[IRsend.h](https://github.com/crankyoldgit/IRremoteESP8266/blob/master/src/IRsend.h),
& [IRrecv.h](https://github.com/crankyoldgit/IRremoteESP8266/blob/master/src/IRrecv.h)
for General, Sending, & Receiving tweaks respectively.

#### Protocol timings
Generally you should never need to adjust the timing parameters for a given
protocol or device. However, occasionally some individual devices just want to
be special.
If you are having problems decoding/receiving a message, look into the
`tolerance`, `kTolerance`, or IRrecv::setTolerance constants/methods etc first.
However, if your problems is sending, or adjusting the tolerance doesn't work
you may need to tweak per-protocol timing values. These are stored as
constants in the `ir_ProtocolName.cpp` file for the given protocol. This is
typically a step of last resort.

#### Reducing code size & flash usage.
You can disable most protocols by either modifying the appropriate `#&zwj;define`s
in [IRremoteESP8266.h](https://github.com/crankyoldgit/IRremoteESP8266/blob/master/src/IRremoteESP8266.h)
or passing the appropriate compile-time flags, as documented in the same file.

Avoid using the A/C classes, especially the IRac class as they will force the
compiler to include large amounts of code you may not need.
