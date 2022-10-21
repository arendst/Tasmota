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
      pip3 install paho-mqtt json

Instructions:
   Edit file and change parameters in User Configuration Section

   Then execute command upload-settings.py

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
myfile = "Config_demo_9.5.0.2.dmp"     # Tasmota Settings file name
myfiletype = 2                         # Tasmota Settings file type

# **** End of User Configuration Section

use_base64 = True

# Derive fulltopic from broker LWT message
mypublish = "cmnd/"+mytopic+"/fileupload"
mysubscribe = "stat/"+mytopic+"/FILEUPLOAD"  # Case sensitive

Ack_flag = False
Err_flag = False

file_id = 116                          # Even id between 2 and 254
file_chunk_size = 700                  # Default Tasmota MQTT max message size

# The callback for when mysubscribe message is received
def on_message(client, userdata, msg):
   global Ack_flag
   global Err_flag
   global file_chunk_size

   rcv_code = ""
   rcv_id = 0

#   print("Received message =",str(msg.payload.decode("utf-8")))

   root = json.loads(msg.payload.decode("utf-8"))
   if "FileUpload" in root:
      rcv_code = root["FileUpload"]
      if "Aborted" in rcv_code:
         print("Error: Aborted")
         Err_flag = True
         return
      if "MD5 mismatch" in rcv_code:
         print("Error: MD5 mismatch")
         Err_flag = True
         return
      if "Started" in rcv_code:
         return
      if "Error" in rcv_code:
         if "1" in rcv_code: print("Error: Wrong password")
         else:
            if "2" in rcv_code: print("Error: Bad chunk size")
            else:
               if "3" in rcv_code: print("Error: Invalid file type")
               else: print("Error: "+rcv_code)
         Err_flag = True
         return
   if "Command" in root:
      rcv_code = root["Command"]
      if rcv_code == "Error":
         print("Error: Command error")
         Err_flag = True
         return
   if "Id" in root:
      rcv_id = root["Id"]
      if rcv_id == file_id:
         if "MaxSize" in root: file_chunk_size = root["MaxSize"]

   Ack_flag = False

def wait_for_ack():
   global Err_flag

   timeout = 500
   while Ack_flag and Err_flag == False and timeout > 0:
      time.sleep(0.01)
      timeout = timeout -1

   if 0 == timeout:
      print("Error: Timeout")
      Err_flag = True

   return Ack_flag

client = mqtt.Client()
client.on_message = on_message
client.connect(broker, broker_port)
client.loop_start()                    # Start loop to process received messages
client.subscribe(mysubscribe)

time_start = time.time()
print("Uploading file "+myfile+" to "+mytopic+" ...")

fo = open(myfile,"rb")
fo.seek(0, 2)  # os.SEEK_END
file_size = fo.tell()
fo.seek(0, 0)  # os.SEEK_SET

client.publish(mypublish, "{\"Password\":\""+mypassword+"\",\"File\":\""+myfile+"\",\"Id\":"+str("%3d"%file_id)+",\"Type\":"+str(myfiletype)+",\"Size\":"+str(file_size)+"}")
Ack_flag = True

out_hash_md5 = hashlib.md5()

Run_flag = True
while Run_flag:
   if wait_for_ack():                  # We use Ack here
      client.publish(mypublish, "0")   # Abort any failed upload
      Run_flag = False

   else:
      chunk = fo.read(file_chunk_size)
      if chunk:
         out_hash_md5.update(chunk)       # Update hash
         if use_base64:
            base64_encoded_data = base64.b64encode(chunk)
            base64_data = base64_encoded_data.decode('utf-8')
            # Message length used by Tasmota (FileTransferHeaderSize)
            client.publish(mypublish, "{\"Id\":"+str("%3d"%file_id)+",\"Data\":\""+base64_data+"\"}")
         else:
            client.publish(mypublish+"201", chunk)
         Ack_flag = True

      else:
         md5_hash = out_hash_md5.hexdigest()
         client.publish(mypublish, "{\"Id\":"+str("%3d"%file_id)+",\"Md5\":\""+md5_hash+"\"}")
         Run_flag = False

fo.close()

time_taken = time.time() - time_start
print("Done in "+str("%.2f"%time_taken)+" seconds")

client.disconnect()                    # Disconnect
client.loop_stop()                     # Stop loop
