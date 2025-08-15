 ## How to use the LoRaWan Device Decoder feature ##
 1. Download to local computer, then upload to your Tasmota File System:
    - [LwDecode.be](https://github.com/arendst/Tasmota/tree/master/tasmota/berry/lorawan/decoders/LwDecode.be)
    - the _Device Decoder File(s)_ for your _End Device(s)_
 2. Add this line to `autoexec.be` in the Tasmota File System (create if necessary)   
 `load("LwDecode.be")`
 3. `Main Menu` > `Restart`
 4. Join the End Devices to the Tasmota LoRaWan Bridge.
    - METHOD 1 (Console)
	  - `LoRaWanAppKey<x> yyyyyyyy` 
	    - `<x>` is the Tasmota LoRaWan end node number
	    - `yyyyyyyy` is the 32 character `Application Key` for this end Device
	- METHOD 2 (Config page)
      - `Main Menu` > `Configuration` > `LoRaWAN`
      - Select the end node number	  
	  - Enter the 32 character `Application Key` for this end Device
 5. Inform Tasmota of the name of the _Decoder File_ for each end device
    - METHOD 1 (Console)
	  - `LoRaWanDecoder<x> <decoderfile>` 
	    - `<x>` is the Tasmota LoRaWan end node number.  
        - e.g.  `LoRaWanDecoder1 LHT52.be` associates node `1` with the `LHT52.be` decoder file
	- METHOD 2 (Config page)
	  - `Main Menu` > `Configuration` > `LoRaWAN`
	  - Select the end node number
	  - Enter the decoder file. e.g. `LHT52.be`
  
 6. Restart Berry with this Tasmota console command: `BrRestart`
