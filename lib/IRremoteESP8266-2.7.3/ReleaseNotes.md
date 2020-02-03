# Release Notes

## _v2.7.3 (20200130)_

**[Features]**
- Allow protocols to be enabled or disabled with compiler flags. (#1013, #1012)
- Panasonic AC: Add Ion Filter support for DKE models. (#1025, #1024)
- Add support for sending Sony at 38Khz (#1029, #1018, #1019)
- auto_analyse_raw_data.py: Handle analysing messages with no headers. (#1017)

**[Misc]**
- Fix Coolix unit test errors when using Apple c++ compiler. (#1030, #1028)
- Fix Apple clang c++ compiler error in unit tests. (#1027, #1026)
- Improve/fix scraping of supported devices (#1022)
- Panasonic PKR series A/C uses DKE protocol. (#1020, #1021)
- Update NEC supported devices. (#1018)
- Add note to avoid GPIO16 on the ESP8266 for receiving. (#1016, #1015)


## _v2.7.2 (20200106)_

**[Bug Fixes]**
- Common AC api: Better handle protocols with power toggles. (#1002)

**[Features]**
- Experimental detailed support for LG a/c. (#1008 #1009)

**[Misc]**
- Add remote codes for Aloka LED lamp. (#1005)
- Improve Supported Devices scraping. (#1006)


## _v2.7.1 (20191125)_

**[Bug Fixes]**
- Hitachi424Ac: Fix Incorrect Power Byte Values (#987)
- Coolix: Fix setPower(false) issue. (#990)

**[Features]**
- Use `char*` instead of `String` for common text. Saves ~1-3k. (#992, #989)
- Hitachi424Ac: Add Vertical Swing ability (#986)

**[Misc]**
- IRMQTTServer: Update HA example/discovery message. (#995)
- Move newly added common text to a better location. (#993)


## _v2.7.0 (20191030)_

**[Bug Fixes]**
- auto_analyse: Fix > 64 bit send code generation. (#976)
- auto_analyse: Fix missing arguments in generated code for send64+ (#972)
- IRsendProntoDemo: Fix compile issue on ESP32 platform. (#938)
- IRMQTTServer: Fix compile error when `MQTT_ENABLE` is false. (#933)

**[Features]**
- Add Hitachi 424 bit A/C support. (#975, #980, #981)
- Experimental detailed support for `DAIKIN152` (#971)
- Mitsubishi 112bit A/C support (#947, #968)
- gc_decode: Adding Support for Decoding codes in raw code format (#963)
- Refactor to use common routines/macros to handle bit manipulation. (#934)
- Use centralised common strings. Saves ~1.5k of program space. (#946)
- Add Internationalisation (i18n) / Locale support. (#946, #955, #966)
  - `de-CH`: Swiss German. (#949, #954)
  - `de-DE`: German. (#946, #950, #952)
  - `en-AU`: English/Australia (Default locale) (#946)
  - `en-IE`: English/Ireland (#946)
  - `en-UK`: English/United Kingdom (#946)
  - `en-US`: English/United States (#946)
  - `es-ES`: Spanish. (#953)
  - `fr-FR`: French. (#962)
- Port CI pipeline to PlatformIO (#936)

**[Misc]**
- Add DAIKIN128 & DAIKIN152 to `decodeToState()` (#982)
- auto_analyse: Produce better code when leader is detected. (#977)
- Coolix A/C improvements (#944)
- A/C setRaw/getRaw/stateReset() cleanup. (#967)
- Add documentation on how to use & support the i18n aspects of the library.
- Make travis checks faster. (#957)
- Translate README.md to french (#959)
- Fixed Coolix kCoolixDefaultState (#941)
- Improve generation of list of pio projects. (#940)


## _v2.6.6 (20190923)_

**[Bug Fixes]**
- Ensure `begin()` is called for every supported common a/c. (#905, #899)
- IRMQTTServer: Fix JSON state parsing. (#896)
- IRMQTTServer: Fix compilation error when `MQTT_CLIMATE_JSON` is `true`. (#893)

**[Features]**
- Mitsubishi136: Full A/C support. (#898, #890)
- Fujitsu: Add support for ARRY4 remote. (#895)
- Web-AC-control: Add new WebUI example sketch. (#880, #886)
- Improve Common A/C API (#913)
- IRMQTTServer: Support for multiple climates. (#903)
- IRMQTTServer: Add TX channel support for HTTP interface. (#929)
- IRMQTTServer: Add option to clear retained settings. (#917)
- auto_analyse_raw_data.py: Add decode code generation. (#909)
- auto_analyse_raw_data.py: General improvements (#906)

**[Misc]**
- IRMQTTServer: Use latest API for common A/C. (#928)
- IRMQTTServer: Add flag & documentation for Home Assistant mode. (#919)
- IRMQTTServer: Move from ArduinoJson v5 to v6. (#878)
- IRMQTTServer: Use retain for discovery message. (#881)
- Goodweather: Adjust timings & minor fixes. (#924)
- PanasonicAc: Add better SwingV support for common a/c framework. (#923)
- Daikin2: Corrections for common A/C interface. (#910)
- MitsubishiAC: Improve decoding. (#914)
- Fujitsu: Disable horiz swing for ARRY4. (#907)
- SamsungAc: Only send power on/off code if it's needed. (#884)
- Teco: Add timer support. (#883)
- More consistent A/C `::toString()` output. (#920)


## _v2.6.5 (20190828)_

**[Bug Fixes]**
- IRMQTTServer: Remove duplicate MQTT_CLIMATE from HA discovery (#869)
- Fujitsu: Ensure `on()` is called in common a/c framework. (#862)
- Update `strToModel()` (#861)
- IRMQTTServer: Add missing header file. (#858)
- IRMQTTServer: Fix a compile error when HTML_PASSWORD_ENABLE is enabled. (#856)

**[Features]**
- IRrecv: Allow tolerance percentage to be set at run-time. (#865)
- Basic support for Daikin152 A/C protocol. (#874)
- Teco: Add light, humid, & save support. (#871)
- Detailed support for Amcor A/C protocol. (#836, #854)
- IRMQTTServer: Add ability to report Vcc at the ESP chip. (#845)
- Gree: Add timer support. (#849)
- IRac/Mitsubishi A/C: Support wide `swingh_t` mode (#844)
- IRMQTTServer: Generate protocol and bit size html selects (#838)

**[Misc]**
- New example code to show how to use the `IRac` class to control A/Cs (#839)
- Improve/fix `swingh_t::kWide` support (#846)
- Kelvinator: Optimise code a little to save space. (#843)


## _v2.6.4 (20190726)_

**[Bug Fixes]**
- Fix some swing problems with the Mitsubishi HAVC protocol (#831)
- Fix parameter ordering for Gree in common a/c code. (#815)
- Fix parameters for Coolix in IRac::sendAc() (#829)
- IRMQTTServer: Fix sending >64 bit codes. (#811)

**[Features]**
- Daikin128: Full detailed support & common a/c support. (#832)
- Midea: Support native temp units of Celsius & SwingV. (#823)
- Gree: Support `YBOFB` models and bug fix. (#815)
- Pioneer: Fix sendPioneer with Pioneer specific timings (#830)
- Daikin128: Initial support for Daikin 17 Series/BRC52B63 (#828)
- Coolix: Better `toCommon()` support. (#825)
- Experimental detailed support for Daikin 176 bits (#816)
- Add setting of output options to A/C classes. (#808)
- Add invert flag support to Samsung AC (#807)

**[Misc]**
- Daikin176: making some change on Daikin176 to work with IRMQTTServer (#826)
- Reduce duplicate code to save (3K+) space. (#813)
- Daikin176: Experiment Daikin176bits with IRMQTTServer (#824)
- Update platformio.ini files for PlatformIO v4.0.0 (#812)
- Change repo URLs to new location. (#806)
- Move `htmlEscape()` to the IRutils namespace (#801)


## _v2.6.3 (20190704)_

**[Bug Fixes]**
- IRMQTTServer: REPLAY_DECODED_AC_MESSAGE not working. (#784, #797)
- ESP32: Ensure `IRrecv`'s GPIO is set to input mode. (#774)

**[Features]**
- IRMQTTServer: Show available sketch space for OTA uploads. (#795)
- Experimental detailed support for Electra/AUX protocol (#788)
- IRMQTTServer: Ability to resend existing climate state via MQTT & HTTP (#784)
- Daikin160: Add detailed & common a/c support. (#777)
- Experimental detailed support for Neoclima protocol. (#767)
- Gree: add WiFi and IFeel bits (#770)
- Handle A/Cs with toggles better. (#758)
- IRMQTTServer: Allow sending/receiving climate via JSON over MQTT. (#763)

**[Misc]**
- Move converting of IR A/C messages out of example code. (#798)
- Reduce example code size and complexity (#790)
- Change `ControlSamsungAC` example to not use `sendExtended()` (#792)
- IRMQTTServer: Add MQTT_CLIMATE_IR_SEND_ON_RESTART compile-time flag. (#784)
- Refactor A/C's toString()'s to reduce code size. Saves ~3.5k (#782)
- Add sanity tests for unexpected conditions in IRrecv. (#773)
- IRMQTTServer: Fixed the HA config documentation (missing '-') (#776)
- Improve `mkkeywords` tool. (#766)
- Refactor with generic decode routines in `IRrecv` class. Saves ~7k. (#765)


## _v2.6.2 (20190616)_

**[Features]**
- Initial support for the ESP32 architecture & boards. (#742)
- Add changable GPIO settings to IRMQTTServer. (#730)
- IRMQTTServer: Enforce a repeat for all Coolix calls (#752)
- Basic DAIKIN 160bit send and decode. (#754)
- Add example code for a Smart(er) IR Repeater. (#740)
- Enforce Samsung A/C Quiet & Powerful mutual exclusivity.

**[Misc]**
- IRMQTTServer: Add some memory alloc safety checks. (#749)
- Move some ToString() functions to IRac.cpp (#748)
- Increase tolerance value for TCL112AC protocol. (#745)
- Fix compiler warning in IRutils_test.cpp (#756)
- Scrape Supported Protocols and generate SupportedProtocols.md (#755)
- Make supported device info more organised. (#753)


## _v2.6.1 (20190609)_

**[Breaking Changes]**
- Major rework/breaking changes to Argo A/C support. (#705)

**[Bug Fixes]**
- Correct `set/getQuiet` for Samsung A/C (#736)
- Add missing `on/off()` to IRCoolixAC class. (#725)
- Daikin `set/getEye()` uses wrong bit. (#711)
- IRMQTTServer: Continue to use same Temperature units. (#710)
- Fixed a bug with `setMode()`/`getMode()` for HAIER_AC. (#705)

**[Features]**
- Add set/getPowerful for Samsung A/C (#736)
- Add `calibrate()` to all the A/C classes. (#735)
- IRMQTTServer: Add sequencing for sending MQTT IR commands. (#723)
- Add support for Fujitsu AR-REB1E & AR-JW2 remotes. (#718)
- Add Beta `decodeTrotec()` support. (#719)
- Add experimental `decodeArgo()` support. (#717)
- Support for Goodweather A/Cs. (#715)
- Add `DISABLE_CAPTURE_WHILE_TRANSMITTING` feature to IRMQTTServer. (#713)
- Support for Lixil Inax Toilet protocol. (#712)
- Add `set/getWeeklyTimerEnable()` to Daikin (#711)
- IRMQTTServer: Update Common A/C settings based on received IR messages. (#705)
- Add day of week to DAIKIN protocol (#699)
- Add limited support for Sharp A/C (#696)
- SAMSUNG_AC: Make sure special power mode messages are sent. (#695)
- Add `set/getPowerful()` (turbo) to DAIKIN216 (#693)

**[Misc]**
- Add kPeriodOffset for CPU Freq of 160MHz. (#729)
- Example code for a Dumb IR repeater. (#737)
- Update swing handling for Fujitsu A/Cs. (#724)
- Add function to convert `decode_results` to `sendRaw()` array. (#721)
- Attempt to reduce heap fragmentation from strings. (#707)
- Update Fujitsu A/C example code to safer settings (#716)
- Enforce better `const` usage in IRUtils. (#708)
- Attempt to reduce heap fragmentation by A/C `toString()`s. (#694)
- Minor changes to DAIKIN216 timings and features. (#693)


## _v2.6.0 (20190430)_

**[Bug Fixes]**
- Fixed problem where LG protocol used wrong duty cycle for repeat. (#687)
- Fix checksum calculation for Daikin protocols. (#678)
- Fix the byte array version of sendGree() (#684, #685)
- Fix artificial vs. real state creation on HaierAC. (#668, #671)
- Fix issues caused by having `MQTT_ENABLE` set to false. (#677)
- Fix compile problem when DEBUG is defined. (#673, #674)
- Fix Minor bug with MQTT_ENABLE False condition (#654)

**[Features]**
- Experimental support for DAIKIN216 (ARC433B69) (#690)
- Experimental support for Mitsubishi Heavy Industries A/Cs. (#660, #665, #667)
- Support more features of TCL A/C (#656)
- Add LEGO(TM) Power Functions IR protocol. (#655)
- Add Panasonic AC RKR model & Example (#649)
- DAIKIN/IRDaikinESP overhaul and add Comfort mode support. (#678)
  **WARNING**: Previous `sendDaikin()` calls may not work.
               Please recapture codes or use `kDaikinStateLengthShort` for
               `nbytes` in those calls.
- IRMQTTServer: Move MQTT server and other parameters to WifiManager. (#680)
  **WARNING**: Previous users may need to fully wipe/reset the
               SPIFFS/WifiManager settings by visiting
               `http://<your_esp8266's_ip_address>/reset` prior to or
               after update.
- Add Wifi filtering options to IRMQTTServer. (#679)
- Add advanced aircon/climate functionality to IRMQTTServer (#677)
- Initial prototype of a common interface for all A/Cs. (#664)
- Improve MQTT topic usage for feedback messages. (#663)
- Add multiple independent GPIO sending support via MQTT. (#661)

**[Misc]**
- Adjust kGreeHdrSpace to 4500 (#684, #686)
- Add Home Assistant mqtt climate instructions. (#682)
- Implement htmlEscape() to prevent XSS etc. (#681)
- Add F() Macros (#670)
- Update Daikin2's Cool mode min temp to 18C (#658)
- Change per byte bit-order in Electra protocol. (#648)
- Improve Daikin2 power on/off. (#647)


## _v2.5.6 (20190316)_

**[Bug Fixes]**
- Fix Coolix A/C Class to handle special states better. (#633, #624)

**[Features]**
- Fix case style for recent A/C protocols. (#631)
- Update `IRsend::send()` to include all simple protocols. (#629, #628)
- Experimental basic support for 112 bit TCL AC messages (#627, #619)
- Add support for TECO AC (#622)
- Experimental support for Samsung 36 bit protocol (#625, #621)

**[Misc]**
- Set Coolix to default to 1 repeat. (#637, #636, #624, #439)
- Set Daikin2 modulation to 36.7kHz. (#635)
- Refactor IRVestelAC class to use portable code. (#617)
- Adjust Daikin2 timings and tolerance. (#616, #582)


## _v2.5.5 (20190207)_

**[Bug Fixes]**
- Fix decoding of Samsung A/C Extended messages. (#610)
- Fix IRMQTTServer example to work with GPIO0 as IR_RX (#608)
- Fix incorrect #define usage. (#597, #596)

**[Features]**
- Add deep decoding/construction of Daikin2 messages (#600)
- Added Old Vestel A/C support (56 Bits) with full functions. (#607)

**[Misc]**
- Add ControlSamsungAC example code. (#599)
- Add how to send a state/air-con to IRsendDemo.ino (#594)


## _v2.5.4 (20190102)_

**[Features]**
- Experimental basic support for 39 Byte Daikin A/C (#583)
- Handle send() repeats in A/C classes. Improve Coolix support. (#580)
- Add optional RX pin pullup and dump raw messages in IRMQTTServer.ino (#589)

**[Misc]**
- Make auto_analyse_raw_data.py work with Python3 (#581)
- Update CI/travis config due to esp8266 core 2.5.0 changes (#591)


## _v2.5.3 (20181123)_

**[Features]**
- Add deep support for the Hitachi 28-Byte A/C Protocol (#563)
- Deep decoding for Whirlpool A/C (#572)
- Improve security options for IRMQTTServer example. (#575)
- Require a changed firmware password before upload. (#576)

**[Misc]**
- Add missing '}' in output of Auto analyse. (#562)
- Make A/C example code a bit more simple. (#571)


## _v2.5.2 (20181021)_

**[Bug Fixes]**
- Add missing send() method to IRPanasonicAC class. (#545)
- Add missing sendWhirlpoolAC() to IRMQTTServer.ino (#558)

**[Features]**
- Add IR receiving support to IRMQTTServer. (#543)
- Pioneer support (#547)
- Add support for a second LG protocol variant. (#552)
- Support for short Panasonic A/C messages. (#553)
- Add support for Panasonic CKP series A/Cs. (#554)
- Experimental timer/clock support for Panasonic A/Cs. (#546)
- Add Made With Magic (MWM) support (#557)

**[Misc]**
- Grammar and typo fixes (#541, #549)
- Increase Panasonic A/C message tolerances. (#542)
- Added command mode2_decode in tools/ (#557)
- General code style cleanup (#560)


## _v2.5.1 (20181002)_

**[Bug Fixes]**
- Correct the byte used for Samsung AC Swing. (#529)
- Fix not sending Samsung A/C messages in IRMQTTServer. (#529)

**[Features]**
- Experimental support for Electra A/C messages. (#528)
- Experimental support for Panasonic A/C messages. (#535)
- Samsung A/C fixes & improvements (#529)
- IRMQTTServer v0.6.0 (#530)

**[Misc]**
- Change required WifiManager lib version to v0.14
- Add alias for RAWTICK to kRawTick. (#535)
- Update sendLutron() status. (#515)
- Remove leftover debug message in IRrecvDumpV2 (#526)


## _v2.5.0 (20180919)_

**[Bug Fixes]**
- Fix HTML menu error for GICABLE in IRMQTTServer. (#516)
- Fix Mitsubishi A/C mode setting. (#514)
- Add missing ',' in auto analyse tool generated code. (#513)
- Fix Fujitsu checksum validation. (#501)
- Remove errant Repeat debug statement in IRMQTTServer. (#507)

**[Features]**
- Mitsubishi A/C decode improvements. (#514)
- Basic support for Whirlpool A/C messages. (#511)
- Basic support for Samsung A/C messages. (#512)
- Experimental support for detailed Samsung A/C messages. (#521)
- Experimental support for detailed Coolix A/C messages. (#518)
- Experimental support for Lutron protocol. (#516)
- Calculate and use average values for timings in analysing tool. (#513)

**[Misc]**
- Style change from using #define's for constants to `const kConstantName`.
- Improve the JVC example code.


## _v2.4.3 (20180727)_

**[Bug Fixes]**
- Handle Space Gaps better in auto analyse tool. (#482)
- Correct min repeat for GICABLE in IRMQTTServer. (#494)

**[Features]**
- Add static IP config option to IRMQTTServer (#480)
- Full decoding/encoding support for the Haier YRW02 A/C. (#485 #486 #487)

**[Misc]**
- Update LG (28-bit) HDR mark and space timings. (#492)
- Spelling and grammar fixes (#491)


## _v2.4.2 (20180601)_

**[Bug Fixes]**
- Timing Fix: Update the period offset compensation.

**[Features]**
- Improvements for IRMQTTServer example (#466)


## _v2.4.1 (20180520)_

**[Bug Fixes]**
- Fix crash in IRMQTTServer when compiled under Arduino IDE. (#455)
- Default bit length not set for RCMM in IRMQTTServer example. (#456)
- Bad acknowledgements for some A/C protocols in IRMQTTServer example. (#460)

**[Features]**
- Allow disabling the use of delay() calls. (#450)
- Initial support for G.I. Cable protocol. (#458)
- Support of Hitachi A/C 13 & 53 byte messages. (#461)

**[Misc]**
- Auto Analyse Raw Data script converted to Python. (#454)

## _v2.4.0 (20180407)_

**[Bug Fixes]**
- Add missing WiFi.begin() call to IRGCTCPServer example. (#433)
- Add missing sendHaierAC() to IRMQTTServer example. (#434 & #444)
- Make mqtt clientid unique in IRMQTTServer example. (#444)

**[Features]**

- Initial Mitsubishi projector protocol support. (#442)
- Experimental support of Hitachi A/C messages. (#445)
- Improve transmission pulse modulation support.
  Allow disabling of transmission frequency modulation.(#439)

**[Misc]**
- IRMQTTServer example improvements. (#444)


## _v2.3.3 (20180302)_

**[Bug Fixes]**
- Ensure the IR LED is off before we start. (#405)

**[Features]**
- Experimental decode support for Gree HVAC units (#397)
- Initial support for Haier A/Cs. (#409)
- Improve timing accuracy of unit tests. (#403)
- Rework matchData() to handle equal total data bit time protocols. (#408)

**[Misc]**
- Add startup text to IRrecvDumpV2 and IRrecvDemo (#412)
- Tweak timings on Fujitsu A/C header (#418)
- AutoAnalyseRawData.sh: Add some support for handling larger than 64 bit codes. (#419)
- Use better comments for send GPIO in examples. (#425)


## _v2.3.2 (20180126)_

**[Bug Fixes]**
- Integer underflow caused device not to respond in `sendJVC()` (#401)

**[Features]**
- Initial support for sending & receiving Carrier HVAC codes. (#387)
- Add Pronto HEX code support to _gc_decode_ tool. (#388)

**[Misc]**
- Make mDNS independent of MQTT in IRMQTTServer example code. (#390 #391)


## _v2.3.1 (20171229)_

**[Bug Fixes]**
- Setting `#define SEND_FUJITSU_AC false` caused a compilation error (#375)
- Integer underflow caused huge `space()` in `sendGeneric()` (#381)

**[Features]**
- Support sending & receiving Lasertag codes. (#374)
- Reduce the library footprint by using a new `sendGeneric()` routine. (#373)

**[Misc]**
- Lots of grammar & typo fixes. (#378)
- Update keywords.txt for Arduino IDE users (#371)
- Update pins in examples so they are compatible with Adafruit boards. (#383)


## _v2.3.0 (20171208)_

**[Bug Fixes]**
- Panasonic-based protocols had incorrect message gap. (#358)
- Formatting error for large rawData values in example code. (#355)
- Off-by-one error in payload_copy malloc. (#337)
- Off-by-one error in unit test helper routines (#363)

**[Features]**
- Support sending and receiving Midea A/C codes.
- Support for receiving Kelvinator A/C codes. (#332)
- Support more operation features for Daikin A/Cs.
- Support for decoding Daikin A/Cs.
- Support sending and receiving Toshiba A/Cs. (#333)
- Support sending and receiving AR-DB1 Fujitsu A/C codes. (#367)
- Add new AutoAnalyseRawData.sh & RawToGlobalCache.sh tools (#345) (#343)
- Support for MagiQuest wands. (#365)

**[Misc]**
- Add checksum verification to Kelvinator A/C decodes. (#348)
- Changes to the threshold reporting of UNKNOWN messages (#347)
- Major re-work of Daikin A/C support.
- Sending for all A/Cs added to MQTT example code.
- MQTT example code improvements. (#334)
- IRrecvDumpV2 significant output improvements. (#363)
- Improved unit test coverage for the library.


## _v2.2.1 (20171025)_

**[Features]**
- Support for sending and decoding Nikai TV messages. (#311, #313)
- gc_decode: External utility to decode Global Cache codes. (#308, #312)
- IRMQTTServer: Example code to send IR messages via HTTP & MQTT. (#316, #323)
- Improve converting 64bit values to hexadecimal. (#318)

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
