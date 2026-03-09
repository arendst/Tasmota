# rm Leds_Panel.tapp; zip -j -0 Leds_Panel.tapp Leds_Panel/autoexec.be Leds_Panel/leds_panel.be Leds_Panel/manifest.json
do                          # embed in `do` so we don't add anything to global namespace
  import introspect
  var leds_panel = introspect.module('leds_panel', true)     # load module but don't cache
  tasmota.add_extension(leds_panel)
end

# to remove:
#       tasmota.unload_extension('Leds Panel')
