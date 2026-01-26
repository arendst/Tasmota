# rm LoRaWan_Decoders.tapp; zip -j -0 LoRaWan_Decoders.tapp LoRaWan_Decoders/*
do                          # embed in `do` so we don't add anything to global namespace
  import introspect
  var lorawan_decoders = introspect.module('lorawan_decoders', true)     # load module but don't cache
  tasmota.add_extension(lorawan_decoders)
end

# to remove:
#       tasmota.unload_extension('LoRaWan Decoders')
