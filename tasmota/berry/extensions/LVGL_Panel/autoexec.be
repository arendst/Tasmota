# rm LVGL_Panel.tapp; zip -j -0 LVGL_Panel.tapp LVGL_Panel/autoexec.be LVGL_Panel/lvgl_panel.be LVGL_Panel/lvgl.html LVGL_Panel/manifest.json
do                          # embed in `do` so we don't add anything to global namespace
  import introspect
  var lvgl_panel = introspect.module('lvgl_panel', true)     # load module but don't cache
  tasmota.add_extension(lvgl_panel)
end
