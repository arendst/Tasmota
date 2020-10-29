#!/usr/bin/env python

#
# Simplest possible example of using RF24Network,
#
#  RECEIVER NODE
#  Listens for messages from the transmitter and prints them out.
#
from __future__ import print_function
import time
from struct import *
from RF24 import *
from RF24Network import *

# CE Pin, CSN Pin, SPI Speed

# Setup for GPIO 22 CE and GPIO 25 CSN with SPI Speed @ 1Mhz
#radio = radio(RPI_V2_GPIO_P1_22, RPI_V2_GPIO_P1_18, BCM2835_SPI_SPEED_1MHZ)

# Setup for GPIO 22 CE and CE0 CSN with SPI Speed @ 4Mhz
#radio = RF24(RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_4MHZ)

# Setup for GPIO 22 CE and CE1 CSN with SPI Speed @ 8Mhz
#radio = RF24(RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_8MHZ)

# Setup for GPIO 22 CE and CE0 CSN for RPi B+ with SPI Speed @ 8Mhz
#radio = RF24(RPI_BPLUS_GPIO_J8_22, RPI_BPLUS_GPIO_J8_24, BCM2835_SPI_SPEED_8MHZ)

radio = RF24(RPI_V2_GPIO_P1_15, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_8MHZ)
network = RF24Network(radio)

millis = lambda: int(round(time.time() * 1000))
octlit = lambda n:int(n, 8)

# Address of our node in Octal format (01, 021, etc)
this_node = octlit("00")

# Address of the other node
other_node = octlit("01")

radio.begin()
time.sleep(0.1)
network.begin(90, this_node)    # channel 90
radio.printDetails()
packets_sent = 0
last_sent = 0

while 1:
    network.update()
    while network.available():
        header, payload = network.read(8)
        print("payload length ", len(payload))
        ms, number = unpack('<LL', bytes(payload))
        print('Received payload ', number, ' at ', ms, ' from ', oct(header.from_node))
    time.sleep(1)

