<!--
#
# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
#  KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.
#
-->

<img src="http://mynewt.apache.org/img/logo.svg" width="250" alt="Apache Mynewt">

## Overview

Apache NimBLE is an open-source Bluetooth 5.0 stack (both Host & Controller)
that completely replaces the proprietary SoftDevice on Nordic chipsets. It is
part of [Apache Mynewt project](https://github.com/apache/mynewt-core).

Features highlight:
  - Support for 251 byte packet size
  - Support for all 4 roles concurrently - Broadcaster, Observer, Peripheral and Central
  - Support for up to 32 simultaneous connections.
  - Legacy and SC (secure connections) SMP support (pairing and bonding).
  - Advertising Extensions.
  - Periodic Advertising.
  - Coded (aka Long Range) and 2M PHYs.
  - Bluetooth Mesh.

## Supported hardware

Controller supports Nordic nRF51 and nRF52 chipsets. Host runs on any board
and architecture [supported](https://github.com/apache/mynewt-core#overview)
by Apache Mynewt OS.


## Browsing

If you are browsing around the source tree, and want to see some of the
major functional chunks, here are a few pointers:

- nimble/controller: Contains code for controller including Link Layer and HCI implementation
([controller](https://github.com/apache/mynewt-nimble/tree/master/nimble/controller))

- nimble/drivers: Contains drivers for supported radio transceivers (Nordic nRF51 and nRF52)
([drivers](https://github.com/apache/mynewt-nimble/tree/master/nimble/drivers))

- nimble/host: Contains code for host subsystem. This includes protocols like
L2CAP and ATT, support for HCI commands and events, Generic Access Profile (GAP),
Generic Attribute Profile (GATT) and Security Manager (SM).
([host](https://github.com/apache/mynewt-nimble/tree/master/nimble/host))

- nimble/host/mesh: Contains code for Bluetooth Mesh subsystem.
([mesh](https://github.com/apache/mynewt-nimble/tree/master/nimble/host/mesh))

- nimble/transport: Contains code for supported transport protocols between host
and controller. This includes UART, emSPI and RAM (used in combined build when
host and controller run on same CPU)
([transport](https://github.com/apache/mynewt-nimble/tree/master/nimble/transport))

- porting: Contains implementation of NimBLE Porting Layer (NPL) for supported
operating systems
([porting](https://github.com/apache/mynewt-nimble/tree/master/porting))

- ext: Contains external libraries used by NimBLE. Those are used if not
provided by OS
([ext](https://github.com/apache/mynewt-nimble/tree/master/ext))

- kernel: Contains the core of the RTOS ([kernel/os](https://github.com/apache/mynewt-core/tree/master/kernel/os))

## Sample Applications

There are also some sample applications that show how to Apache Mynewt NimBLE
stack. These sample applications are located in the `apps/` directory of
Apache Mynewt [repo](https://github.com/apache/mynewt-core). Some examples:

* [blecent](https://github.com/apache/mynewt-core/tree/master/apps/blecent):
A basic central device with no user interface.  This application scans for
a peripheral that supports the alert notification service (ANS). Upon
discovering such a peripheral, blecent connects and performs a characteristic
read, characteristic write, and notification subscription.
* [blehci](https://github.com/apache/mynewt-core/tree/master/apps/blehci):
Implements a BLE controller-only application.  A separate host-only
implementation, such as Linux's BlueZ, can interface with this application via
HCI over UART.
* [bleprph](https://github.com/apache/mynewt-core/tree/master/apps/bleprph): An
  implementation of a minimal BLE peripheral.
* [btshell](https://github.com/apache/mynewt-core/tree/master/apps/btshell): A
  shell-like application allowing to configure and use most of NimBLE
  functionality from command line.
* [bleuart](https://github.com/apache/mynewt-core/tree/master/apps/bleuart):
Implements a simple BLE peripheral that supports the Nordic
UART / Serial Port Emulation service
(https://developer.nordicsemi.com/nRF5_SDK/nRF51_SDK_v8.x.x/doc/8.0.0/s110/html/a00072.html).
* [test](https://github.com/apache/mynewt-core/tree/master/apps/test): Test
  project which can be compiled either with the simulator, or on a per-architecture basis.
  Test will run all the package's unit tests.

# Getting Help

If you are having trouble using or contributing to Apache Mynewt NimBLE, or just
want to talk to a human about what you're working on, you can contact us via the
[developers mailing list](mailto:dev@mynewt.apache.org).

Although not a formal channel, you can also find a number of core developers
on the #mynewt channel on Freenode IRC or #general channel on [Mynewt Slack](https://join.slack.com/mynewt/shared_invite/MTkwMTg1ODM1NTg5LTE0OTYxNzQ4NzQtZTU1YmNhYjhkMg)

Also, be sure to checkout the [Frequently Asked Questions](https://mynewt.apache.org/faq/answers)
for some help troubleshooting first.

# Contributing

Anybody who works with Apache Mynewt can be a contributing member of the
community that develops and deploys it.  The process of releasing an operating
system for microcontrollers is never done: and we welcome your contributions
to that effort.

More information can be found at the Community section of the Apache Mynewt
website, located [here](https://mynewt.apache.org/community).

## Pull Requests

Apache Mynewt welcomes pull request via Github.  Discussions are done on Github,
but depending on the topic, can also be relayed to the official Apache Mynewt
developer mailing list dev@mynewt.apache.org.

If you are suggesting a new feature, please email the developer list directly,
with a description of the feature you are planning to work on.

## Filing Bugs

Bugs can be filed on the
[Apache Mynewt NimBLE Issues](https://github.com/apache/mynewt-nimble/issues).
Please label the issue as a "Bug".

Where possible, please include a self-contained reproduction case!

## Feature Requests

Feature requests should also be filed on the
[Apache Mynewt NimBLE Bug Tracker](https://github.com/apache/mynewt-nimble/issues).
Please label the issue as a "Feature" or "Enhancement" depending on the scope.

## Writing Tests

We love getting newt tests!  Apache Mynewt is a huge undertaking, and improving
code coverage is a win for every Apache Mynewt user.

<!--
TODO
## Writing Documentation

Contributing to documentation (in addition to writing tests), is a great way
to get involved with the Apache Mynewt project.

 The Mynewt NimBLE documentation is found in [/docs](/docs).
-->

# License

The code in this repository is all under either the Apache 2 license, or a
license compatible with the Apache 2 license.  See the LICENSE file for more
information.
