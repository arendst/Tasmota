#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
   download-settings.py - Upload Tasmota settings file

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
      pip3 install paho-mqtt json

Instructions:
   Edit file and change parameters in User Configuration Section

   Then execute command download-settings.py

"""

import paho.mqtt.client as mqtt
import time
import base64
import hashlib
import json

# **** Start of User Configuration Section

broker = "domus1"                      # MQTT broker ip address or name
broker_port = 1883                     # MQTT broker port

mypassword = ""                        # Tasmota MQTT password
mytopic = "demo"                       # Tasmota MQTT topic
myfiletype = 2                         # Tasmota Settings file type

# **** End of User Configuration Section

use_base64 = True

# Derive fulltopic from broker LWT message
mypublish = "cmnd/"+mytopic+"/filedownload"
mysubscribe = "stat/"+mytopic+"/FILEDOWNLOAD"  # Case sensitive

Ack_flag = False
Err_flag = False

file_name = ""
file_id = 0
file_type = 0
file_size = 0
file_md5 = ""

# The callback for when mysubscribe message is received
def on_message(client, userdata, msg):
   global Ack_flag
   global Err_flag
   global file_name
   global file_id
   global file_type
   global file_size
   global file_md5
   global in_hash_md5

   base64_data = ""
   rcv_id = 0

#   try:
#      print("Received message =",str(msg.payload.decode("utf-8")))
#   except:
#      print("Received message = binary data")

   try:
      root = json.loads(msg.payload.decode("utf-8"))
      if root:
         if "FileDownload" in root:
            rcv_code = root["FileDownload"]
            if "Aborted" in rcv_code:
               print("Error: Aborted")
               Err_flag = True
               return
            if "Started" in rcv_code:
               return
            if "Error" in rcv_code:
               print("Error: "+rcv_code)
               Err_flag = True
               return
         if "Command" in root:
            rcv_code = root["Command"]
            if rcv_code == "Error":
               print("Error: Command error")
               Err_flag = True
               return
         if "File" in root: file_name = root["File"]
         if "Id" in root: rcv_id = root["Id"]
         if "Type" in root: file_type = root["Type"]
         if "Size" in root: file_size = root["Size"]
         if "Data" in root: base64_data = root["Data"]
         if "Md5" in root: file_md5 = root["Md5"]
   except:
      pass

   if file_id == 0 and rcv_id > 0 and file_size > 0 and file_type > 0 and file_name:
      file_id = rcv_id
      fi = open(file_name,"wb")
      fi.close()
   else:
      if use_base64 and file_id > 0 and file_id != rcv_id:
         Err_flag = True
         return

   if file_md5 == "" and file_name:
      if use_base64 and base64_data:
         base64_decoded_data = base64_data.encode('utf-8')
         chunk = base64.decodebytes(base64_decoded_data)
         in_hash_md5.update(chunk)    # Update hash
         fi = open(file_name,"ab")
         fi.write(chunk)
         fi.close()
      if use_base64 == False and 0 == rcv_id:
         chunk = msg.payload
         in_hash_md5.update(chunk)    # Update hash
         fi = open(file_name,"ab")
         fi.write(chunk)
         fi.close()

   if file_md5 != "":
      md5_hash = in_hash_md5.hexdigest()
      if md5_hash != file_md5:
         print("Error: MD5 mismatch")
         Err_flag = True

   Ack_flag = False

def wait_for_ack():
   timeout = 100
   while Ack_flag and Err_flag == False and timeout > 0:
      time.sleep(0.01)
      timeout = timeout -1

   if 0 == timeout:
      print("Error: Timeout")

   return Ack_flag

client = mqtt.Client()
client.on_message = on_message
client.connect(broker, broker_port)
client.loop_start()                    # Start loop to process received messages
client.subscribe(mysubscribe)

time_start = time.time()
print("Downloading file from "+mytopic+" ...")

in_hash_md5 = hashlib.md5()

if use_base64:
   client.publish(mypublish, "{\"Password\":\""+mypassword+"\",\"Type\":"+str(myfiletype)+"}")
else:
   client.publish(mypublish, "{\"Password\":\""+mypassword+"\",\"Type\":"+str(myfiletype)+",\"Binary\":1}")
Ack_flag = True

Run_flag = True
while Run_flag:
   if wait_for_ack():                  # We use Ack here
      client.publish(mypublish, "0")   # Abort any failed download
      Run_flag = False

   else:
      if file_md5 == "":               # Request chunk
         client.publish(mypublish, "?")
         Ack_flag = True
      else:
         Run_flag = False

if Err_flag == False:
   file_type_name = "Data"
   if file_type == 2:
      file_type_name = "Settings"
   print("Downloaded "+file_type_name+" saved as "+file_name)

time_taken = time.time() - time_start
print("Done in "+str("%.2f"%time_taken)+" seconds")

client.disconnect()                    # Disconnect
client.loop_stop()                     # Stop loop
