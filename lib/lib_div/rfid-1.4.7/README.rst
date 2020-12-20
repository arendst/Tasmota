MFRC522
=======

.. image:: https://img.shields.io/maintenance/no/2019.svg
    :target: `development`_
.. image:: https://travis-ci.org/miguelbalboa/rfid.svg?branch=master
    :target: https://travis-ci.org/miguelbalboa/rfid
.. image:: https://img.shields.io/badge/C%2B%2B-11-brightgreen.svg
    :target: `compatible ide`_
.. image:: https://img.shields.io/github/release/miguelbalboa/rfid.svg?colorB=green
    :target: https://github.com/miguelbalboa/rfid/releases
.. image:: https://img.shields.io/badge/ArduinoIDE-%3E%3D1.6.10-lightgrey.svg
    :target: `compatible ide`_

Arduino library for MFRC522 and other RFID RC522 based modules.

Read and write different types of Radio-Frequency IDentification (RFID) cards
on your Arduino using a RC522 based reader connected via the Serial Peripheral
Interface (SPI) interface.


.. _development:
Development
----------

**The development by owner miguelbalboa has ended**.

**Feature status: complete freeze**; no function or API change.

**Code status: partial freeze**; just fixes/typos or documentation updates; *no* extensions for other boards; *no* new examples.

**Maintenance status: sporadically**.

**Why no further development?**
This library has a long history and is used in many projects. These projects often do not document what version they use. Committing changes might break those old projects and lead to bad experiences (for beginners) and support requests. For these reasons the library is in freeze mode. You can still commit typo, documentation or bug fixes.


.. _before buy:
Before buy
----------
Please notice that there are many sellers (ebay, aliexpress, ..) who sell mfrc522 boards. **The quality of these boards are extremely different.** Some are soldered with wrong/low quality capacitors or fake/defect mfrc522.

**Please consider buying several devices from different suppliers.** So the chance of getting a working device is higher.

If you got a bad board and you can tell us how to detect those boards (silk, chip description, ..), please share your knowledge.


.. _what works and not:
What works and not?
----------

* **Works**
  
  #. Communication (Crypto1) with MIFARE Classic (1k, 4k, Mini).
  #. Communication (Crypto1) with MIFARE Classic compatible PICCs.
  #. Firmware self check of MFRC522.
  #. Set the UID, write to sector 0, and unbrick Chinese UID changeable MIFARE cards.
  #. Manage the SPI chip select pin (aka SS, SDA)

* **Works partially**

  #. Communication with MIFARE Ultralight.
  #. Other PICCs (Ntag216).
  #. More than 2 modules, require a multiplexer `#191 <https://github.com/miguelbalboa/rfid/issues/191#issuecomment-242631153>`_.

* **Doesn't work**
  
  #. MIFARE DESFire, MIFARE DESFire EV1/EV2, not supported by software.
  #. Communication with 3DES or AES, not supported by software.
  #. Peer-to-peer (ISO/IEC 18092), not `supported by hardware`_.
  #. Communication with smart phone, not `supported by hardware`_.
  #. Card emulation, not `supported by hardware`_.
  #. Use of IRQ pin. But there is a proof-of-concept example.
  #. With Intel Galileo (Gen2) see `#310 <https://github.com/miguelbalboa/rfid/issues/310>`__, not supported by software.
  #. Power reduction modes `#269 <https://github.com/miguelbalboa/rfid/issues/269>`_, not supported by software.
  #. I2C instead of SPI `#240 <https://github.com/miguelbalboa/rfid/issues/240>`_, not supported by software.
  #. UART instead of SPI `#281 <https://github.com/miguelbalboa/rfid/issues/281>`_, not supported by software.
  
* **Need more?**

  #. If software: code it and make a pull request.
  #. If hardware: buy a more expensive like PN532 (supports NFC and many more, but costs about $15 and not usable with this library).


.. _compatible ide:
Compatible IDE
----------
This library works with Arduino IDE 1.6, older versions are **not supported** and will cause compiler errors. The built-in library manager is supported.

If you use your own compiler, you have to enable ``c++11``-support.


.. _compatible boards:
Compatible boards
----------

**!!!Only for advanced users!!!**

This library is compatible with the Teensy and ESP8266 if you use the board plugin of the Arduino IDE. Not all examples are available for every board. You also have to change pins. See `pin layout`_.

Some user made some patches/suggestions/ports for other boards:

* Linux: https://github.com/miguelbalboa/rfid/pull/216
* chipKIT: https://github.com/miguelbalboa/rfid/pull/230
* ESP8266 (native): https://github.com/miguelbalboa/rfid/pull/235
* LPCOPen (in C): https://github.com/miguelbalboa/rfid/pull/258

Note that the main target/support of library is still Arduino.

.. _support issue:
Support/issue
----------
1. First checkout `what works and not`_ and `troubleshooting`_ .

2. It seems to be a hardware issue or you need support to program your project?
    Please ask in the official `Arduino forum`_, where you would get a much faster answer than on Github.

3. It seems to be a software issue?
    Open an issue on Github.


.. _code style:
Code style
----------

Please use ``fixed integers``, see `stdint.h`_. Why? This library is compatible with different boards which use different architectures (16bit and 32bit.) Unfixed ``int`` variables have different sizes in different environments and may cause unpredictable behaviour.


.. _pin layout:
Pin Layout
----------

The following table shows the typical pin layout used:

+-----------+----------+---------------------------------------------------------------+--------------------------+
|           | PCD      | Arduino                                                       | Teensy                   |
|           +----------+-------------+---------+---------+-----------------+-----------+--------+--------+--------+
|           | MFRC522  | Uno / 101   | Mega    | Nano v3 |Leonardo / Micro | Pro Micro | 2.0    | ++ 2.0 | 3.1    |
+-----------+----------+-------------+---------+---------+-----------------+-----------+--------+--------+--------+
| Signal    | Pin      | Pin         | Pin     | Pin     | Pin             | Pin       | Pin    | Pin    | Pin    |
+===========+==========+=============+=========+=========+=================+===========+========+========+========+
| RST/Reset | RST      | 9 [1]_      | 5 [1]_  | D9      | RESET / ICSP-5  | RST       | 7      | 4      | 9      |
+-----------+----------+-------------+---------+---------+-----------------+-----------+--------+--------+--------+
| SPI SS    | SDA [3]_ | 10 [2]_     | 53 [2]_ | D10     | 10              | 10        | 0      | 20     | 10     |
+-----------+----------+-------------+---------+---------+-----------------+-----------+--------+--------+--------+
| SPI MOSI  | MOSI     | 11 / ICSP-4 | 51      | D11     | ICSP-4          | 16        | 2      | 22     | 11     |
+-----------+----------+-------------+---------+---------+-----------------+-----------+--------+--------+--------+
| SPI MISO  | MISO     | 12 / ICSP-1 | 50      | D12     | ICSP-1          | 14        | 3      | 23     | 12     |
+-----------+----------+-------------+---------+---------+-----------------+-----------+--------+--------+--------+
| SPI SCK   | SCK      | 13 / ICSP-3 | 52      | D13     | ICSP-3          | 15        | 1      | 21     | 13     |
+-----------+----------+-------------+---------+---------+-----------------+-----------+--------+--------+--------+

+-----------+---------------+---------+
|           | ESP8266       | Arduino |
|           +---------------+---------+
|           | Wemos D1 mini | Yun [4]_|
+-----------+---------------+---------+
| Signal    | Pin           | Pin     |
+===========+===============+=========+
| RST/Reset | D3            | Pin9    |
+-----------+---------------+---------+
| SPI SS    | D8            | Pin10   |
+-----------+---------------+---------+
| SPI MOSI  | D7            | ICSP4   |
+-----------+---------------+---------+
| SPI MISO  | D6            | ICSP1   |
+-----------+---------------+---------+
| SPI SCK   | D5            | ICSP3   |
+-----------+---------------+---------+

.. [1] Configurable, typically defined as RST_PIN in sketch/program.
.. [2] Configurable, typically defined as SS_PIN in sketch/program.
.. [3] The SDA pin might be labeled SS on some/older MFRC522 boards. 
.. [4] Source: https://github.com/miguelbalboa/rfid/issues/111#issuecomment-420433658 .

Important: If your micro controller supports multiple SPI interfaces, the library only uses the **default (first) SPI** of the Arduino framework.

.. _hardware:
Hardware
--------

There are three hardware components involved:

1. **Micro Controller**:

* An `Arduino`_ or compatible executing the Sketch using this library.

* Prices vary from USD 7 for clones, to USD 75 for "starter kits" (which
  might be a good choice if this is your first exposure to Arduino;
  check if such kit already includes the Arduino, Reader, and some Tags).

2. **Proximity Coupling Device (PCD)**:

* The PCD is the actual RFID **Reader** based on the `NXP MFRC522`_ Contactless
  Reader Integrated Circuit.

* Readers can be found on `eBay`_ for around USD 5: search for *"rc522"*.

* You can also find them on several web stores. They are often included in
  *"starter kits"*, so check your favourite electronics provider as well.

3. **Proximity Integrated Circuit Card (PICC)**:

* The PICC is the RFID **Card** or **Tag** using the `ISO/IEC 14443A`_
  interface, for example Mifare or NTAG203.

* One or two might be included with the Reader or *"starter kit"* already.


.. _protocol:
Protocols
---------

1. The micro controller and the reader use SPI for communication.

* The protocol is described in the `NXP MFRC522`_ datasheet.

* See the `Pin Layout`_ section for details on connecting the pins.

2. The reader and the tags communicate using a 13.56 MHz electromagnetic field.

* The protocol is defined in ISO/IEC 14443-3:2011 Part 3 Type A.

  * Details are found in chapter 6 *"Type A – Initialization and anticollision"*.
  
  * See http://wg8.de/wg8n1496_17n3613_Ballot_FCD14443-3.pdf for a free version
    of the final draft (which might be outdated in some areas).
    
  * The reader does not support ISO/IEC 14443-3 Type B.


.. _security:
Security
-------
* The **UID** of a card **can not be used** as an unique identification for security related projects. Some Chinese cards allow to change the UID which means you can easily clone a card. For projects like *access control*, *door opener* or *payment systems* you **must implement** an **additional security mechanism** like a password or normal key.

* This library only supports crypto1-encrypted communication. Crypto1 has been known as `broken`_ for a few years, so it does NOT offer ANY security, it is virtually unencrypted communication. **Do not use it for any security related applications!**

* This library does not offer 3DES or AES authentication used by cards like the Mifare DESFire, it may be possible to be implemented because the datasheet says there is support. We hope for pull requests :).


.. _troubleshooting:
Troubleshooting
-------

* **I don't get input from reader** or **WARNING: Communication failure, is the MFRC522 properly connected?**

  #. Check your physical connection, see `Pin Layout`_ .
  #. Check your pin settings/variables in the code, see `Pin Layout`_ .
  #. Check your pin header soldering. Maybe you have cold solder joints.
  #. Check voltage. Most breakouts work with 3.3V.
  #. SPI only works with 3.3V, most breakouts seem 5V tolerant, but try a level shifter.
  #. SPI does not like long connections. Try shorter connections.
  #. SPI does not like prototyping boards. Try soldered connections.
  #. According to reports #101, #126 and #131, there may be a problem with the soldering on the MFRC522 breakout. You could fix this on your own.


* **Firmware Version: 0x12 = (unknown) or other random values**

  #. The exact reason of this behaviour is unknown.
  #. Some boards need more time after `PCD_Init()` to be ready. As workaround add a `delay(4)` directly after `PCD_Init()` to give the PCD more time.
  #. If this sometimes appears, a bad connection or power source is the reason.
  #. If the firmware version is reported permanent, it is very likely that the hardware is a fake or has a defect. Contact your supplier.


* **Sometimes I get timeouts** or **sometimes tag/card does not work.**

  #. Try the other side of the antenna.
  #. Try to decrease the distance between the MFRC522 and your tag.
  #. Increase the antenna gain per firmware: ``mfrc522.PCD_SetAntennaGain(mfrc522.RxGain_max);``
  #. Use better power supply.
  #. Hardware may be corrupted, most products are from china and sometimes the quality is really poor. Contact your seller.


* **My tag/card doesn't work.**
  
  #. Distance between antenna and token too large (>1cm).
  #. You got the wrong type PICC. Is it really 13.56 MHz? Is it really a Mifare Type A?
  #. NFC tokens are not supported. Some may work.
  #. Animal RFID tags are not supported. They use a different frequency (125 kHz).
  #. Hardware may be corrupted, most products are from china and sometimes the quality is really poor. Contact your seller.
  #. Newer versions of Mifare cards like DESFire/Ultralight maybe not work according to missing authentication, see `security`_ or different `protocol`_.
  #. Some boards bought from Chinese manufactures do not use the best components and this can affect the detection of different types of tag/card. In some of these boards, the L1 and L2 inductors do not have a high enough current so the signal generated is not enough to get Ultralight C and NTAG203 tags to work, replacing those with same inductance (2.2uH) but higher operating current inductors should make things work smoothly. Also, in some of those boards the  harmonic and matching circuit needs to be tuned, for this replace C4 and C5 with 33pf capacitors and you are all set. (Source: `Mikro Elektronika`_) 

* **My mobile phone doesn't recognize the MFRC522** or **my MFRC522 can't read data from other MFRC522**

  #. Card simulation is not supported.
  #. Communication with mobile phones is not supported.
  #. Peer to peer communication is not supported.

* **I can only read the card UID.**

  #. Maybe the `AccessBits` have been accidentally set and now an unknown password is set. This can not be reverted.
  #. Probably the card is encrypted. Especially official cards like public transport, university or library cards. There is *no* way to get access with this library.

* **I need more features.**

  #. If software: code it and make a pull request.
  #. If hardware: buy a more expensive chip like the PN532 (supports NFC and many more, but costs about $15)


.. _license:
License
-------
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to https://unlicense.org/


.. _dependency:
Dependency
----------

* **Arduino.h**

  * From: Arduino IDE / target specific
  * License: (target: Arduino) GNU Lesser General Public License 2.1
  
* **SPI.h**

  * From: Arduino IDE / target specific
  * License: (target: Arduino) GNU Lesser General Public License 2.1
  
* **stdint.h**

  * From: Arduino IDE / Compiler and target specific
  * License: different


History
-------

The MFRC522 library was first created in Jan 2012 by Miguel Balboa (from
http://circuitito.com) based on code by Dr. Leong (from http://B2CQSHOP.com)
for *"Arduino RFID module Kit 13.56 Mhz with Tags SPI W and R By COOQRobot"*.

It was translated into English and rewritten/refactored in the fall of 2013
by Søren Thing Andersen (from http://access.thing.dk).

It has been extended with functionality to alter sector 0 on Chinese UID changeable MIFARE card in Oct 2014 by Tom Clement (from http://tomclement.nl).

Maintained by miguelbalboa until 2016.
Maintained by Rotzbua from 2016 until 2020.


.. _arduino: https://arduino.cc/
.. _ebay: https://www.ebay.com/
.. _iso/iec 14443a: https://en.wikipedia.org/wiki/ISO/IEC_14443
.. _iso/iec 14443-3\:2011 part 3: 
.. _nxp mfrc522: https://www.nxp.com/documents/data_sheet/MFRC522.pdf
.. _broken: https://eprint.iacr.org/2008/166
.. _supported by hardware: https://web.archive.org/web/20151210045625/http://www.nxp.com/documents/leaflet/939775017564.pdf
.. _Arduino forum: https://forum.arduino.cc
.. _stdint.h: https://en.wikibooks.org/wiki/C_Programming/C_Reference/stdint.h
.. _Mikro Elektronika: https://forum.mikroe.com/viewtopic.php?f=147&t=64203
