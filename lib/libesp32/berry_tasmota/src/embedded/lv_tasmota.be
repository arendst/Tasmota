#- ------------------------------------------------------------ -#
#  Module `lv_tasmota` - piggybacks on `lv` to extend it
#- ------------------------------------------------------------ -#
lv_tasmota = module("lv_tasmota")

# rename `lv` to `lv_ntv` and replace `lv` with `lv_tasmota`
def init(lv_tasmota)
  import lv
  lv.start = lv_tasmota.start

  lv.font_montserrat = lv_tasmota.font_montserrat
  lv.montserrat_font = lv_tasmota.font_montserrat

  lv.font_seg7 = lv_tasmota.font_seg7
  lv.seg7_font = lv_tasmota.font_seg7

  lv.load_freetype_font = lv_tasmota.load_freetype_font

  lv.register_button_encoder = lv_tasmota.register_button_encoder
  lv.screenshot = lv_tasmota.screenshot

  # add widgets
  lv.clock_icon = lv_clock_icon
  lv.signal_arcs = lv_signal_arcs
  lv.signal_bars = lv_signal_bars
  lv.wifi_arcs_icon = lv_wifi_arcs_icon
  lv.wifi_arcs = lv_wifi_arcs
  lv.wifi_bars_icon = lv_wifi_bars_icon
  lv.wifi_bars = lv_wifi_bars

  return nil
end

lv_tasmota.init = init

return lv_tasmota
