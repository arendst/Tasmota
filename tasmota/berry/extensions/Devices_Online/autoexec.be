# rm Devices_Online.tapp; zip -j -0 Devices_Online.tapp Devices_Online/*
do                          # embed in `do` so we don't add anything to global namespace
  import introspect
  var devices_online = introspect.module('devices_online', true)     # load module but don't cache
  tasmota.add_extension(devices_online)
end

# to remove:
#       tasmota.unload_extension('Devices Online')
