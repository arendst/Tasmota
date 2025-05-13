 ## How to use the LoRaWan Device Decoder feature ##
 1. Download to local computer, then upload to your Tasmota File System:
    - [LwDecoders.be](https://github.com/arendst/Tasmota/tree/master/tasmota/berry/lorawan/decoders/LwDecoders.be)
    - the _Device Decoder File(s)_ for your _End Device(s)_
 2. Add this line to `autoexec.be` in the Tasmota File System (create if necessary)   
 `load("LwDecode.be")`
 3. Join the End Devices to the Tasmota LoRaWan Bridge.
    - e.g. `LoRaWanAppKey<x> yyyyyyyy` where `<x>` is the Tasmota LoRaWan node number.
 4. Inform Tasmota of the name of the _Decoder File_ for each end device with this Tasmota console command: `LoRaWanDecoder<x> <decoderfile>` where `<x>` is the Tasmota LoRaWan node number.  
  e.g.  `LoRaWanDecoder1 LHT52` associates node 1 with the `LHT52.be` decoder file
  
 5. Restart Berry with this Tasmota console command: `BrRestart`