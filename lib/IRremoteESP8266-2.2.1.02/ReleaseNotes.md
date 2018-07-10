# Release Notes

## _v2.2.1 (20171025)_

**[Features]**
- Support for sending and decoding Nikai TV messages. (#311, #313)
- gc_decode: External utility to decode Global Cache codes. (#308, #312)
- IRMQTTServer: Example code to send IR messages via HTTP & MQTT. (#316, #323)
- Improve converting 64bit values to hexidecimal. (#318)

**[Misc]**
- IRrecvDump.ino code is now deprecated. Use IRrecvDumpV2.ino instead. (#314)


## _v2.2.0 (20170922)_

**[Bug Fixes]**
- Add printing output of RC-MM and RC-5X protocols in example code. (#284)
- LG timing improvements based on observations (#291)

**[Features]**
- Automatic capture timing calibration for some protocols. (#268)
- Support for creating & sending Trotec AC codes. (#279)
- Support for creating & sending Argo Ulisse 13 DCI codes. (#280 #300)
- Move to 2 microsecond timing resolution for capture of codes. (#287)
- Capture buffer changes:
- Size at runtime. (#276)
- Message timeout at runtime. (#294)
- Simplify creating & using a second buffer (#303)
- New example code:
  - Trotec A/C (#279)
  - LG A/C units (#289)
  - Argo Ulisse 13 DCI codes. (#300)


## _v2.1.1 (20170711)_

**[Bug Fixes]**
- GlobalCache incorrectly using hardware offset for period calc. (#267)

**[Features]**
- Support reporting of 'NEC'-like 32-bit protocols. e.g. Apple TV remote (#265)
- Add an example of sendRaw() to IRsendDemo.ino (#270)


## _v2.1.0 (20170704)_

**[Features]**
- Support for sending Pronto IR codes. (#248)
- Support for sending Fujitsu A/C codes. (#88)
- Minor improvements to examples.


## _v2.0.3 (20170618)_

**[Bug fixes]**
- Capture buffer could become corrupt after large message, breaking subsequent decodes. (#253)


## _v2.0.2 (20170615)_

**[Bug fixes]**
- Correct decode issue introduced in v2.0.1 affecting multiple protocol decoders (#243)
- Correct post-message gap for the Panasonic protocol(s) (#245)
- Incorrect display of the decoded uint64_t value in the example code. (#245)


## _v2.0.1 (20170614)_

**[Bug fixes]**
- Decoding protocols when it doesn't detect a post-command gap, and there is no more data. (#243)
- Incorrect minimum size calculation when there is no post-command gap. (#243)


## _v2.0.0 - 64 bit support and major improvements (20170612)_

**[Misc]**
- This is almost a complete re-write of the library.

**[Features]**
- All suitable protocols now handle 64-bit data messages and are repeatable via an optional argument.
- Unit tests for all protocols.
- Far better and stricter decoding for most protocols.
- Address & command decoding for protocols where that information is available.
- Much more precise timing for generation of signals sent.
- Lower duty-cycles for some protocols.
- Several new protocols added, and some new sending and decoding routines for existing ones.
- Ability to optionally chose which protocols are included, enabling faster decoding and smaller code footprints if desired.
- Support for far larger capture buffers. (e.g. RAWLEN > 256)

**[Bug fixes]**
- Numerous bug fixes.


## _v1.2.0 (20170429)_

**[Features]**
- Add ability to copy IR capture buffer, and continue capturing. Means faster and better IR command decoding.
- Reduce IRAM usage by 28 bytes.
- Improve capture of RC-MM & Panasonic protocols.
- Upgrade IRrecvDumpV2 to new IR capture buffer. Much fewer corrupted/truncated IR messages.


## _v1.1.1 (20170413)_

**[Bug fixes]**
- Fix a reported problem when sending the LG protocol. Preemptive fix for possible similar cases.
- Fix minor issues in examples.

**[Features]**
- Add documentation to some examples to aid new people.
- Add ALPHA support for RC-MM protocol. (Known to be currently not 100% working.)
