#- start LVGL and init environment -#
lv.start()

hres = lv.get_hor_res()     # should be 240
vres = lv.get_ver_res()     # should be 320

scr = lv.scr_act()          # default screean object
f20 = lv.montserrat_font(20)  # load embedded Montserrat 20
f28 = lv.montserrat_font(28)  # load embedded Montserrat 28

#- Backgroun -#
scr.set_style_bg_color(lv.color(0x000066), lv.PART_MAIN | lv.STATE_DEFAULT)

#- Upper state line -#
stat_line = lv.label(scr)
if f20 != nil stat_line.set_style_text_font(f20, lv.PART_MAIN | lv.STATE_DEFAULT) end
stat_line.set_long_mode(lv.LABEL_LONG_SCROLL)                                                  # auto scrolling if text does not fit
stat_line.set_width(hres)
stat_line.set_align(lv.TEXT_ALIGN_LEFT)                                                      # align text left
stat_line.set_style_bg_color(lv.color(0xD00000), lv.PART_MAIN | lv.STATE_DEFAULT)    # background #000088
stat_line.set_style_bg_opa(lv.OPA_COVER, lv.PART_MAIN | lv.STATE_DEFAULT)            # 100% background opacity
stat_line.set_style_text_color(lv.color(0xFFFFFF), lv.PART_MAIN | lv.STATE_DEFAULT)  # text color #FFFFFF
stat_line.set_text("Tasmota")
stat_line.refr_size()                                                                # new in LVGL8
stat_line.refr_pos()                                                                 # new in LVGL8

#- display wifi strength indicator icon (for professionals ;) -#
wifi_icon = lv_wifi_arcs_icon(stat_line)    # the widget takes care of positioning and driver stuff
clock_icon = lv_clock_icon(stat_line)

#- create a style for the buttons -#
btn_style = lv.style()

btn_style.set_radius(10)                        # radius of rounded corners
btn_style.set_bg_opa(lv.OPA_COVER)              # 100% backgrond opacity
if f28 != nil btn_style.set_text_font(f28) end  # set font to Montserrat 28
btn_style.set_text_color(lv.color(0x000000))    # text color black when not checked

#- register buttons -#
var btns = []         # relay buttons are added to this list to match with Tasmota relays

#- callback function when a button is pressed -#
#- checks if the button is in the list, and react to EVENT_VALUE_CHANGED event -#
def btn_clicked_cb(obj, event)
  var btn_idx = btns.find(obj)
  if btn_idx != nil
    var checked = (obj.get_state() & lv.STATE_CHECKED) != 0
    tasmota.set_power(btn_idx, checked)                            # toggle the value
  end
end

#- create a button object, set style, register callback and add to global list -#
#- you still need to re-position the button -#
def create_btn_relay(label)
  var btn, btn_label
  btn = lv.btn(scr)
  btn.set_pos(30, 30)
  btn.set_size(hres - 60, 60)
  btn.add_style(btn_style, lv.PART_MAIN | lv.STATE_DEFAULT)   # style of button
  btn.add_flag(lv.OBJ_FLAG_CHECKABLE)                           # enable toggle mode   
  btn_label = lv.label(btn)
  btn_label.center()
  btn_label.set_text(label)
  btn.add_event_cb(btn_clicked_cb, lv.EVENT_CLICKED, 0)       # set callback to update Tasmota relays
  btns.push(btn)                                            # append button to the list
  return btn
end

#- create 3 buttons -#
var btn1 = create_btn_relay("Relay 1")
btn1.set_y(30)
var btn2 = create_btn_relay("Relay 2")
btn2.set_y(100)
var btn3 = create_btn_relay("Relay 3")
btn3.set_y(170)

#- update the buttons values according to internal relays status -#
def btns_update()
  var power_list = tasmota.get_power()                                            # get a list of booleans with status of each relay
  for b:btns
    var checked = (b.get_state() & lv.STATE_CHECKED) != 0
    var power_state = (size(power_list) > 0) ? power_list.pop(0) : false          # avoid exception if less relays than buttons
    if power_state && !checked
      b.add_state(lv.STATE_CHECKED)
    elif !power_state && checked
      b.clear_state(lv.STATE_CHECKED)
    end
  end
end

#- update every 500ms -#
def btns_update_loop()
  btns_update()
  tasmota.set_timer(500, btns_update_loop)
end
btns_update_loop()  # start

# If you change the style after creating the button, you need to update objects:
def btns_refresh_style()
  for b:btns b.refresh_style(lv.OBJ_PART_MAIN, lv.STYLE_PROP_ALL) end
end
