import os
import subprocess
import json
import shutil

r = open("/home/runner/work/Tasmota/Tasmota/platformio_tasmota32.ini","r+")
framework = ""
for line in r:
    items = line.split("=")
    if "platform" == items[0].strip():
        framework = items[1].strip()
        print (framework)
cmd = ("pio","pkg","install","-p", framework)
returncode = subprocess.call(cmd, shell=False)
if returncode == 0:
    print("Framework installed ...")
else:
    print("Could not install Framework!!")

with open("/home/runner/.platformio/platforms/espressif32/platform.json") as json_file:
        data = json.load(json_file)
        for package in data['packages']:
            if "framework-arduino" in package:
                cmd = ("pio","pkg","install","-p",data['packages'][package]['version'])
                print(cmd)
                returncode = subprocess.call(cmd, shell=False)


safeboot_dir = "./firmware/firmware"

variants_dir = "/home/runner/.platformio/packages/framework-arduinoespressif32/variants/tasmota"
shutil.copytree(safeboot_dir, variants_dir)

variants_dir = "/home/runner/.platformio/packages/framework-arduino-ITEAD/variants/tasmota"
shutil.copytree(safeboot_dir, variants_dir)

variants_dir = "/home/runner/.platformio/packages/framework-arduino-solo1/variants/tasmota"
shutil.copytree(safeboot_dir, variants_dir)
