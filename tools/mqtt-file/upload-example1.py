#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
   upload-example1.py - Upload Tasmota settings file

   Copyright (C) 2021  Theo Arends

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

Requirements:
   - Python 3.x and Pip:
      sudo apt-get install python3 python3-pip
      pip3 install paho-mqtt

Instructions:
   Edit file and change parameters in User Configuration Section

   Then execute command upload-example1.py

"""

import paho.mqtt.client as mqtt
import time
import base64
import hashlib

# **** Start of User Configuration Section

broker = "domus1"                      # MQTT broker ip address or name
broker_port = 1883                     # MQTT broker port

mytopic = "Demo"                       # Tasmota MQTT topic
myfile = "Config_demo_9.4.0.3.dmp"     # Tasmota Settings file name
myfiletype = 2                         # Tasmota Settings file type

# **** End of User Configuration Section

# Derive from myfile
myfilesize = 4096

# Derive from time epoch
myid = 1620484815

# Derive fulltopic from broker LWT message
mypublish = "cmnd/" + mytopic + "/fileupload"
mysubscribe = "stat/" + mytopic + "/fileupload"

# Tasmota currently supports MQTT message size of 1040 characters. Base64 adds 0.25 chars
chucksize = 700                        # Tasmota max chunk size

# Example does not use feedback Acknowledge
sleeptime = 0.2

client = mqtt.Client()
client.connect(broker, broker_port)

client.publish(mypublish, "{\"File\":\""+myfile+"\",\"Id\":"+str(myid)+",\"Type\":"+str(myfiletype)+",\"Size\":"+str(myfilesize)+"}")

out_hash_md5 = hashlib.md5()

fo = open(myfile,"rb")
Run_flag = True
while Run_flag:
   chunk=fo.read(chucksize)
   if chunk:
      out_hash_md5.update(chunk)       # Update hash
      base64_encoded_data = base64.b64encode(chunk)
      base64_data = base64_encoded_data.decode('utf-8')
      client.publish(mypublish, "{\"Id\":"+str(myid)+",\"Data\":\""+base64_data+"\"}")

   else:
      Run_flag=False
   time.sleep(sleeptime)               # We do not use Ack here

md5_hash = out_hash_md5.hexdigest()
client.publish(mypublish, "{\"Id\":"+str(myid)+",\"Md5\":\""+md5_hash+"\"}")

fo.close()
client.disconnect()                    # Disconnect
