# demo for debug panel
#
# rm debug_panel.tapp; zip -j -0 debug_panel.tapp webserver_async.be debug_panel.be autoexec.be
import webserver_async
import debug_panel

if tasmota.version() >= 0xD030002
  if !tasmota.wifi()['up']
    tasmota.add_rule("Wifi#Connected", def ()
      global._debug_panel = debug_panel(5550)
      tasmota.remove_rule("Wifi#Connected", "debug_panel_start")
    end, "debug_panel_start")
  else
    global._debug_panel = debug_panel(5550)
  end
else
  log("BRY: 'debug_panel' requires Tasmota v13.3.0.2")
end
