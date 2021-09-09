#- start LVGL and init environment -#
lv.start()

hres = lv.get_hor_res()       # should be 320
vres = lv.get_ver_res()       # should be 240

scr = lv.scr_act()            # default screean object
f20 = lv.montserrat_font(20)  # load embedded Montserrat 20

#- Background with a gradient from black #000000 (bottom) to dark blue #0000A0 (top) -#
scr.set_style_local_bg_color(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, lv_color(0x0000A0))
scr.set_style_local_bg_grad_color(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, lv_color(0x000000))
scr.set_style_local_bg_grad_dir(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, lv.GRAD_DIR_VER)

#- Upper state line -#
stat_line = lv_label(scr)
if f20 != nil stat_line.set_style_local_text_font(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, f20) end
stat_line.set_long_mode(lv.LABEL_LONG_SROLL)                                                  # auto scrolling if text does not fit
stat_line.set_width(hres)
stat_line.set_align(lv.LABEL_ALIGN_LEFT)                                                      # align text left
stat_line.set_style_local_bg_color(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, lv_color(0xD00000))    # background #000088
stat_line.set_style_local_bg_opa(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, lv.OPA_COVER)            # 100% background opacity
stat_line.set_style_local_text_color(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, lv_color(0xFFFFFF))  # text color #FFFFFF
stat_line.set_text("Tasmota")
stat_line_height = stat_line.get_height()

#- display wifi strength indicator icon (for professionals ;) -#
wifi_icon = lv_wifi_arcs_icon(stat_line)    # the widget takes care of positioning and driver stuff
clock_icon = lv_clock_icon(stat_line)

#- create a style for the buttons -#
btn_style = lv_style()
btn_style.set_radius(lv.STATE_DEFAULT, 10)                        # radius of rounded corners
btn_style.set_bg_opa(lv.STATE_DEFAULT, lv.OPA_COVER)              # 100% backgrond opacity
if f20 != nil btn_style.set_text_font(lv.STATE_DEFAULT, f20) end  # set font to Montserrat 20
btn_style.set_bg_color(lv.STATE_DEFAULT, lv_color(0x1fa3ec))      # background color #1FA3EC (Tasmota Blue)
btn_style.set_border_color(lv.STATE_DEFAULT, lv_color(0x0000FF))  # border color #0000FF
btn_style.set_text_color(lv.STATE_DEFAULT, lv_color(0xFFFFFF))    # text color white #FFFFFF

#- create buttons -#
prev_btn = lv_btn(scr)                            # create button with main screen as parent
prev_btn.set_pos(20,vres-40)                      # position of button
prev_btn.set_size(80, 30)                         # size of button
prev_btn.add_style(lv.OBJ_PART_MAIN, btn_style)   # style of button
prev_label = lv_label(prev_btn)                   # create a label as sub-object
prev_label.set_text("<")                          # set label text

next_btn = lv_btn(scr)                            # right button
next_btn.set_pos(220,vres-40)
next_btn.set_size(80, 30)
next_btn.add_style(lv.OBJ_PART_MAIN, btn_style)
next_label = lv_label(next_btn)
next_label.set_text(">")

home_btn = lv_btn(scr)                            # center button
home_btn.set_pos(120,vres-40)
home_btn.set_size(80, 30)
home_btn.add_style(lv.OBJ_PART_MAIN, btn_style)
home_label = lv_label(home_btn)
home_label.set_text(lv.SYMBOL_OK)                 # set text as Home icon

#- callback function when a button is pressed, react to EVENT_CLICKED event -#
def btn_event_cb(o, event)
  if lv.EVENT_CLICKED
    var btn = "Unknown"
    if   o == prev_btn  btn = "Prev"
    elif o == next_btn  btn = "Next"
    elif o == home_btn  btn = "Home"
    end
    print(btn, "button pressed")
  end
end

# associate callbacks to buttons
prev_btn.set_event_cb(btn_event_cb)
next_btn.set_event_cb(btn_event_cb)
home_btn.set_event_cb(btn_event_cb)