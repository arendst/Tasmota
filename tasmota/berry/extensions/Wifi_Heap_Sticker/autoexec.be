# rm Wifi_Heap_Sticker.tapp; zip -j -0 Wifi_Heap_Sticker.tapp Wifi_Heap_Sticker/autoexec.be Wifi_Heap_Sticker/wifi_heap_sticker.be Wifi_Heap_Sticker/manifest.json
do                          # embed in `do` so we don't add anything to global namespace
  import introspect
  var wifi_heap_sticker = introspect.module('wifi_heap_sticker', true)     # load module but don't cache
  tasmota.add_extension(wifi_heap_sticker)
end
