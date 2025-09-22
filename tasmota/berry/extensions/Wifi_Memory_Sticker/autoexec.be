# rm Wifi_Memory_Sticker.tapp; zip -j -0 Wifi_Memory_Sticker.tapp Wifi_Memory_Sticker/autoexec.be Wifi_Memory_Sticker/wifi_memory_sticker.be Wifi_Memory_Sticker/manifest.json
do                          # embed in `do` so we don't add anything to global namespace
  import introspect
  var wifi_memory_sticker = introspect.module('wifi_memory_sticker', true)     # load module but don't cache
  tasmota.add_extension(wifi_memory_sticker)
end
