#- start LVGL and init environment -#
import global

lv.start()

hres = lv.get_hor_res()       # should be 320
vres = lv.get_ver_res()       # should be 240

scr = lv.scr_act()            # default screean object
f20 = lv.montserrat_font(20)  # load embedded Montserrat 20

#- Background with a gradient from black #000000 (bottom) to dark blue #0000A0 (top) -#
scr.set_style_bg_color(lv.color(0x0000A0), lv.PART_MAIN | lv.STATE_DEFAULT)
scr.set_style_bg_grad_color(lv.color(0x000000), lv.PART_MAIN | lv.STATE_DEFAULT)
scr.set_style_bg_grad_dir(lv.GRAD_DIR_VER, lv.PART_MAIN | lv.STATE_DEFAULT)

#- Upper state line -#
stat_line = lv.label(scr)
if f20 != nil stat_line.set_style_text_font(f20, lv.PART_MAIN | lv.STATE_DEFAULT) end
stat_line.set_long_mode(lv.LABEL_LONG_SCROLL)                                        # auto scrolling if text does not fit
stat_line.set_width(hres)
stat_line.set_align(lv.TEXT_ALIGN_LEFT)                                              # align text left
stat_line.set_style_bg_color(lv.color(0xD00000), lv.PART_MAIN | lv.STATE_DEFAULT)    # background #000088
stat_line.set_style_bg_opa(lv.OPA_COVER, lv.PART_MAIN | lv.STATE_DEFAULT)            # 100% background opacity
stat_line.set_style_text_color(lv.color(0xFFFFFF), lv.PART_MAIN | lv.STATE_DEFAULT)  # text color #FFFFFF
stat_line.set_text(tasmota.cmd("DeviceName").find('DeviceName',"Tasmota"))
stat_line.refr_size()                                                                # new in LVGL8
stat_line.refr_pos()                                                                 # new in LVGL8

#- display wifi strength indicator icon (for professionals ;) -#
wifi_icon = lv.wifi_arcs_icon(stat_line)    # the widget takes care of positioning and driver stuff
#wifi_icon = lv.wifi_bars_icon(stat_line)    # the widget takes care of positioning and driver stuff
clock_icon = lv.clock_icon(stat_line)

#- create a style for the buttons -#
btn_style = lv.style()
btn_style.set_radius(10)                        # radius of rounded corners
btn_style.set_bg_opa(lv.OPA_COVER)              # 100% backgrond opacity
if f20 != nil btn_style.set_text_font(f20) end  # set font to Montserrat 20
btn_style.set_bg_color(lv.color(0x1fa3ec))      # background color #1FA3EC (Tasmota Blue)
btn_style.set_border_color(lv.color(0x0000FF))  # border color #0000FF
btn_style.set_text_color(lv.color(0xFFFFFF))    # text color white #FFFFFF

#- create buttons -#
prev_btn = lv.btn(scr)                            # create button with main screen as parent
prev_btn.set_pos(20,vres-30)                      # position of button
prev_btn.set_size(80, 25)                         # size of button
prev_btn.add_style(btn_style, lv.PART_MAIN | lv.STATE_DEFAULT)   # style of button
prev_label = lv.label(prev_btn)                   # create a label as sub-object
prev_label.set_text(lv.SYMBOL_LEFT)                          # set label text
prev_label.center()

next_btn = lv.btn(scr)                            # right button
next_btn.set_pos(220,vres-30)
next_btn.set_size(80, 25)
next_btn.add_style(btn_style, lv.PART_MAIN | lv.STATE_DEFAULT)
next_label = lv.label(next_btn)
next_label.set_text(lv.SYMBOL_RIGHT)
next_label.center()

home_btn = lv.btn(scr)                            # center button
home_btn.set_pos(120,vres-30)
home_btn.set_size(80, 25)
home_btn.add_style(btn_style, lv.PART_MAIN | lv.STATE_DEFAULT)
home_label = lv.label(home_btn)
home_label.set_text(lv.SYMBOL_HOME)                 # set text as Home icon
home_label.center()

#- callback function when a button is pressed, react to EVENT_CLICKED event -#

def btn_clicked_cb(obj, event)
    var btn = "Unknown"
    if   obj == prev_btn  btn = "Prev"
    elif obj == next_btn  btn = "Next"
    elif obj == home_btn  btn = "Home"
    end
    print(btn, "button pressed")
end


prev_btn.add_event_cb(btn_clicked_cb, lv.EVENT_CLICKED, 0)
next_btn.add_event_cb(btn_clicked_cb, lv.EVENT_CLICKED, 0)
home_btn.add_event_cb(btn_clicked_cb, lv.EVENT_CLICKED, 0)

import lv_wifi_graph
ws_h = 40
ws_w = 80
ws = lv_wifi_graph(scr)
ws.set_size(ws_w,ws_h)
ws.set_pos(hres-ws_w - 3, stat_line.get_height() + 3)

# info bloc
import lv_tasmota_info
var info = lv_tasmota_info(scr)
info.set_pos(3, stat_line.get_height()+ 3)
info.set_size(hres - 80 + 1 - 10, 40)

# logs
import lv_tasmota_log_roboto
var lg = lv_tasmota_log_roboto(scr, 6)
lg.set_size(hres - 6, 90)
lg.set_pos(3, stat_line.get_height() + 40 + 6)

# 3D cube
var cube
if global.accelerator
  import lv_3D_cube_widget
  cube = lv_3D_cube_widget(scr, global.accelerator)
  cube.set_pos(3,161)
end


#-
sb120 = lv.load_freetype_font("sketchbook.ttf", 120, 0)
tt = lv.label(scr)
tt.set_style_bg_opa(lv.OPA_0, lv.PART_MAIN | lv.STATE_DEFAULT)
tt.set_style_text_color(lv.color(0xFFFFFF), lv.PART_MAIN | lv.STATE_DEFAULT)
tt.set_text("MQTT")
tt.set_pos(10,40)
tt.set_size(300,150)
if sb120 != nil tt.set_style_text_font(sb120, lv.PART_MAIN | lv.STATE_DEFAULT) end

-#
