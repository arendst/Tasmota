# Led panel web app
do                          # embed in `do` so we don't add anything to global namespace
  import introspect
  var leds_panel = introspect.module('leds_panel', true)     # load module but don't cache
  tasmota.add_driver(leds_panel, "leds_panel")
end
