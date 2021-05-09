#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
   upload-settings.py - Upload Tasmota settings file

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

   Then execute command upload-settings.py

"""

import paho.mqtt.client as mqtt
import time
import base64
import hashlib

# **** Start of User Configuration Section

broker = "domus1"                      # MQTT broker ip address or name
broker_port = 1883                     # MQTT broker port

mytopic = "demo"                       # Tasmota MQTT topic
myfile = "Config_demo_9.4.0.3.dmp"     # Tasmota Settings file name
myfiletype = 2                         # Tasmota Settings file type

# **** End of User Configuration Section

# Derive from myfile
myfilesize = 4096

# Derive from time epoch
myid = 1620484815

# Derive fulltopic from broker LWT message
mypublish = "cmnd/"+mytopic+"/fileupload"
mysubscribe = "stat/"+mytopic+"/FILEUPLOAD"  # Case sensitive

# Tasmota currently supports MQTT message size of 1040 characters. Base64 adds 0.25 chars
chucksize = 700                        # Tasmota max chunk size

# Example does use feedback Acknowledge
Ack_flag = False

# The callback for when mysubscribe message is received
def on_message(client, userdata, msg):
   global Ack_flag
#   print("Received message =",str(msg.payload.decode("utf-8")))
   Ack_flag = False

def wait_for_ack():
   global Ack_flag
   timeout = 100
   while Ack_flag and timeout > 0:
      time.sleep(0.01)
      timeout = timeout -1

   if Ack_flag:
      print("Error: Ack timeout")

   return Ack_flag

client = mqtt.Client()
client.on_message = on_message
client.connect(broker, broker_port)
client.loop_start()                    # Start loop to process received messages
client.subscribe(mysubscribe)

time_start = time.time()
print("Uploading file "+myfile+" to "+mytopic+" ...")

client.publish(mypublish, "{\"File\":\""+myfile+"\",\"Id\":"+str(myid)+",\"Type\":"+str(myfiletype)+",\"Size\":"+str(myfilesize)+"}")
Ack_flag = True

out_hash_md5 = hashlib.md5()

fo = open(myfile,"rb")
Run_flag = True
while Run_flag:
   if wait_for_ack():                  # We use Ack here
      Run_flag = False

   else:
      chunk = fo.read(chucksize)
      if chunk:
         out_hash_md5.update(chunk)    # Update hash
         base64_encoded_data = base64.b64encode(chunk)
         base64_data = base64_encoded_data.decode('utf-8')
         client.publish(mypublish, "{\"Id\":"+str(myid)+",\"Data\":\""+base64_data+"\"}")
         Ack_flag = True

      else:
         md5_hash = out_hash_md5.hexdigest()
         client.publish(mypublish, "{\"Id\":"+str(myid)+",\"Md5\":\""+md5_hash+"\"}")
         Run_flag = False

fo.close()

time_taken = time.time() - time_start
print("Done in "+str("%.2f"%time_taken)+" seconds")

client.disconnect()                    # Disconnect
client.loop_stop()                     # Stop loop
