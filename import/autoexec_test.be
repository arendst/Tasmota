#- start LVGL and init environment -#
lv.start()

import display
display.dimmer(60)

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
stat_line.set_text("Tasmota")
stat_line.refr_size()                                                                # new in LVGL8
stat_line.refr_pos()                                                                 # new in LVGL8

#- display wifi strength indicator icon (for professionals ;) -#
wifi_icon = lv.wifi_arcs_icon(stat_line)    # the widget takes care of positioning and driver stuff
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
prev_btn.set_pos(20,vres-40)                      # position of button
prev_btn.set_size(80, 30)                         # size of button
prev_btn.add_style(btn_style, lv.PART_MAIN | lv.STATE_DEFAULT)   # style of button
prev_label = lv.label(prev_btn)                   # create a label as sub-object
prev_label.set_text("<")                          # set label text
prev_label.center()

next_btn = lv.btn(scr)                            # right button
next_btn.set_pos(220,vres-40)
next_btn.set_size(80, 30)
next_btn.add_style(btn_style, lv.PART_MAIN | lv.STATE_DEFAULT)
next_label = lv.label(next_btn)
next_label.set_text(">")
next_label.center()

home_btn = lv.btn(scr)                            # center button
home_btn.set_pos(120,vres-40)
home_btn.set_size(80, 30)
home_btn.add_style(btn_style, lv.PART_MAIN | lv.STATE_DEFAULT)
home_label = lv.label(home_btn)
home_label.set_text(lv.SYMBOL_OK)                 # set text as Home icon
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


# demo of gird layout

lv.start()

var col_dsc = lv.coord_arr([90, 90, 90, lv.GRID_TEMPLATE_LAST])
var row_dsc = lv.coord_arr([15, 15, 15, lv.GRID_TEMPLATE_LAST])
var scr = lv.scr_act()

var cont = lv.obj(scr)

cont.set_grid_dsc_array(col_dsc, row_dsc)
# replaces
# cont.set_style_grid_column_dsc_array(col_dsc, 0)
# cont.set_style_grid_row_dsc_array(row_dsc, 0)
# cont.set_layout(lv.LAYOUT_GRID)
cont.set_size(320, 240)
cont.center()

var zone_name = 0
var zone_mode = 1
var zone_power = 2
var zone_until = 3

var z1col = 0
var z2col = 1
var z3col = 2

# Zone Name

var lbl_z1_name = lv.label(cont)
var lbl_z2_name = lv.label(cont)
var lbl_z3_name = lv.label(cont)

lbl_z1_name.set_grid_cell(lv.GRID_ALIGN_CENTER, z1col, 1, lv.GRID_ALIGN_STRETCH, zone_name, 1)
lbl_z1_name.set_text("HTG 1")
lbl_z1_name.center()

lbl_z2_name.set_grid_cell(lv.GRID_ALIGN_CENTER, z2col, 1, lv.GRID_ALIGN_STRETCH, zone_name, 1)
lbl_z2_name.set_text("HTG 2")
lbl_z2_name.center()

lbl_z3_name.set_grid_cell(lv.GRID_ALIGN_CENTER, z3col, 1, lv.GRID_ALIGN_STRETCH, zone_name, 1)
lbl_z3_name.set_text("HTG 1")
lbl_z3_name.center()

# Zone Mode

var lbl_z1_mode = lv.label(cont)
var lbl_z2_mode = lv.label(cont)
var lbl_z3_mode = lv.label(cont)

lbl_z1_mode.set_grid_cell(lv.GRID_ALIGN_CENTER, z1col, 1, lv.GRID_ALIGN_STRETCH, zone_mode, 1)
lbl_z1_mode.set_text("Auto")
lbl_z1_mode.center()

lbl_z2_mode.set_grid_cell(lv.GRID_ALIGN_CENTER, z2col, 1, lv.GRID_ALIGN_STRETCH, zone_mode, 1)
lbl_z2_mode.set_text("Allday")
lbl_z2_mode.center()

lbl_z3_mode.set_grid_cell(lv.GRID_ALIGN_CENTER, z3col, 1, lv.GRID_ALIGN_STRETCH, zone_mode, 1)
lbl_z3_mode.set_text("Auto")
lbl_z3_mode.center()

# Zone Power

var lbl_z1_power = lv.label(cont)
var lbl_z2_power = lv.label(cont)
var lbl_z3_power = lv.label(cont)

lbl_z1_power.set_grid_cell(lv.GRID_ALIGN_CENTER, z1col, 1, lv.GRID_ALIGN_STRETCH, zone_power, 1)
lbl_z1_power.set_recolor(True)
lbl_z1_power.set_text("#ff0000 Off# Until")
lbl_z1_power.center()

lbl_z2_power.set_grid_cell(lv.GRID_ALIGN_CENTER, z2col, 1, lv.GRID_ALIGN_STRETCH, zone_power, 1)
lbl_z2_power.set_recolor(True)
lbl_z2_power.set_text("#0000ff On# Until")
lbl_z2_power.center()

lbl_z3_power.set_grid_cell(lv.GRID_ALIGN_CENTER, z3col, 1, lv.GRID_ALIGN_STRETCH, zone_power, 1)
lbl_z3_power.set_recolor(True)
lbl_z3_power.set_text("#ff0000 Off# Until")
lbl_z3_power.center()
