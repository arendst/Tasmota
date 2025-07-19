# rm ArtNet.tapp; zip ArtNet.tapp -j -0 artnet/*
#
# check if `dyn` class is embedded, or load it
if !global.contains("dyn")
  import artnet_dyn
  global.dyn = artnet_dyn
end

import artnet
import artnet_ui

import persist
if persist.find("artnet_auto")
  tasmota.add_rule("Wifi#Connected", def () tasmota.remove_rule("Wifi#Connected", "artnet_run") artnet.run_from_conf() end, "artnet_run")
end
