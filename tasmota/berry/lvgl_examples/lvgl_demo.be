lv.start()

hres = lv.get_hor_res()
vres = lv.get_ver_res()

scr = lv.scr_act()
f20 = lv.montserrat_font(20)

scr.set_style_local_bg_color(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, lv_color(0x0000A0))
scr.set_style_local_bg_grad_color(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, lv_color(0x000000))
scr.set_style_local_bg_grad_dir(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, lv.GRAD_DIR_VER)

log = lv_label(scr)
if f20 != nil log.set_style_local_text_font(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, f20) end
log.set_long_mode(lv.LABEL_LONG_SROLL)
log.set_width(hres)
log.set_align(lv.LABEL_ALIGN_LEFT)
log.set_style_local_bg_color(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, lv_color(0xD00000))
log.set_style_local_bg_opa(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, lv.OPA_COVER)
log.set_style_local_text_color(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, lv_color(0xFFFFFF))
log.set_text("Welcome to Tasmota")

tastyle = lv_style()
tastyle.set_bg_color(lv.STATE_DEFAULT, lv_color(0x1fa3ec))
tastyle.set_border_color(lv.STATE_DEFAULT, lv_color(0x0000FF))
tastyle.set_bg_color(lv.STATE_FOCUSED, lv_color(0x0000FF))
tastyle.set_border_color(lv.STATE_FOCUSED, lv_color(0xFFFFFF))
tastyle.set_text_color(lv.STATE_DEFAULT, lv_color(0xFFFFFF))
tastyle.set_radius(lv.STATE_DEFAULT, 10)
tastyle.set_bg_opa(lv.STATE_DEFAULT, lv.OPA_COVER)
if f20 != nil tastyle.set_text_font(lv.STATE_DEFAULT, f20) end

prev_btn = lv_btn(scr)
prev_btn.set_pos(20,vres-40)
prev_btn.set_size(80, 30)
prev_btn.add_style(lv.OBJ_PART_MAIN, tastyle)
prev_label = lv_label(prev_btn)
prev_label.set_text("<")

next_btn = lv_btn(scr)
next_btn.set_pos(220,vres-40)
next_btn.set_size(80, 30)
next_btn.add_style(lv.OBJ_PART_MAIN, tastyle)
next_label = lv_label(next_btn)
next_label.set_text(">")

home_btn = lv_btn(scr)
home_btn.set_pos(120,vres-40)
home_btn.set_size(80, 30)
home_btn.add_style(lv.OBJ_PART_MAIN, tastyle)
home_label = lv_label(home_btn)
home_label.set_text(lv.SYMBOL_OK)


log_height = log.get_height()

# log.get_style_pad_right(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT)
log.set_style_local_pad_right(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, log_height+1)

bars = lv_wifi_bars(log)

bars.set_style_local_bg_color(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, lv_color(lv.BLACK))

bars.set_height(log_height)
bars.set_width(log_height)
bars.set_x(log.get_width() - log_height)
