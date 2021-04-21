#-
 - Demo for M5Stack black edition, using ILI9341
 -
 -#

if gpio.pin_used(gpio.ILI9341_CS) && gpio.pin_used(gpio.ILI9341_DC) && gpio.pin_used(gpio.OLED_RESET)

  lv.start(udisplay.ILI9341_M5Stack_Core)

  hres = lv.get_hor_res()
  vres = lv.get_ver_res()

  scr = lv.scr_act()
  scr.set_style_local_bg_color(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, lv_color(lv.RED))
  scr.set_style_local_bg_opa(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, lv.OPA_COVER)

  scr.set_style_local_bg_color(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, lv_color(0xFFA000))
  scr.set_style_local_bg_grad_color(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, lv_color(0xFF4000))
  scr.set_style_local_bg_color(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, lv_color(0x000080))
  scr.set_style_local_bg_grad_color(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, lv_color(0x000030))
  scr.set_style_local_bg_grad_dir(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, lv.GRAD_DIR_VER)

  #- logo = lv.img_create(scr)
  logo.set_src("A:/Sunrise320.bin")
  logo.align(0,0,0,0) -#

  btn = lv_btn(scr)
  btn.set_pos(40,40)
  btn.set_size(180, 45)
  #btn.set_style_local_bg_color(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, lv_color(0x1fa3ec))
  

  label = lv_label(btn)
  label.set_text(lv.SYMBOL_HOME + "Tasmota")
  #label.set_style_local_text_color(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, lv_color(0xFFFFFF))
  #f28 = lv.montserrat_font(28)
  #if f28 != nil label.set_style_local_text_font(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, f28) end
  btn.align(0, lv.ALIGN_CENTER, 0, 0)


  log = lv_label(scr)
  f20 = lv.montserrat_font(20)
  if f20 != nil log.set_style_local_text_font(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, f20) end
  #- log.set_long_mode(lv.LABEL_LONG_DOT) -#
  log.set_long_mode(lv.LABEL_LONG_SROLL)
  log.set_width(hres)
  log.set_align(lv.LABEL_ALIGN_LEFT)
  log.set_style_local_bg_color(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, lv_color(0xD00000))
  log.set_style_local_bg_opa(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, lv.OPA_COVER)
  log.set_style_local_text_color(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, lv_color(0xFFFFFF))
  log.set_text("Welcome to Tasmota - long text rolling")
  log.set_text("Welcome to Tasmota")

  tastyle = lv_style()

  tastyle.set_bg_color(lv.STATE_DEFAULT, lv_color(0x1fa3ec))
  tastyle.set_text_color(lv.STATE_DEFAULT, lv_color(0xFFFFFF))
  tastyle.set_radius(lv.STATE_DEFAULT, 10)
  tastyle.set_bg_opa(lv.STATE_DEFAULT, lv.OPA_COVER)
  f20 = lv.montserrat_font(20)
  if f20 != nil tastyle.set_text_font(lv.STATE_DEFAULT, f20) end

  btn.add_style(lv.OBJ_PART_MAIN, tastyle)

  prev_btn = lv_btn(scr)
  prev_btn.set_pos(20,vres-40)
  prev_btn.set_size(80, 30)
  prev_btn.add_style(lv.OBJ_PART_MAIN, tastyle)
  prev_label = lv_label(prev_btn)
  # prev_label.set_text("Prev")
  prev_label.set_text("<")
  
  next_btn = lv_btn(scr)
  next_btn.set_pos(220,vres-40)
  next_btn.set_size(80, 30)
  next_btn.add_style(lv.OBJ_PART_MAIN, tastyle)
  next_label = lv_label(next_btn)
  #next_label.set_text("Next")
  next_label.set_text(">")

  home_btn = lv_btn(scr)
  home_btn.set_pos(120,vres-40)
  home_btn.set_size(80, 30)
  home_btn.add_style(lv.OBJ_PART_MAIN, tastyle)
  home_label = lv_label(home_btn)
  home_label.set_text(lv.SYMBOL_OK)

  btn.del()





  logo.set_src("A:/Sunrise.bin")
  logo.align(0,0,0,0)



  btn.set_height(120)
  label.del()
  logo = lv.img_create(btn)
  logo.set_tasmota_logo()

  logo.set_zoom(384)
  logo.set_angle(400)

  logo.set_style_local_image_recolor_opa(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, 255)
  logo.set_style_local_image_recolor(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, lv_color(lv.BLUE))


  #- logo on splash screen -#
  btn.del()
  logo = lv.img_create(lv.scr_act())
  logo.set_tasmota_logo()
  logo.set_style_local_image_recolor_opa(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, 255)
  logo.set_style_local_image_recolor(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, lv_color(lv.WHITE))
  logo.align(0,0,0,0)
  logo.set_zoom(400)


  #- anil -#
  logo.set_zoom(384)

  angle = 0
  do_rotate = nil
  do_rotate = def () angle += 100 logo.set_angle(angle) tasmota.timer(100, do_rotate) end

  t48 = lv.tasmota_font(48)
  label.set_text("A")
  if t48 != nil label.set_style_local_text_font(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, t48) end
  btn.set_height(120)


  f10 = lv.montserrat_font(10)
  if f10 != nil label.set_style_local_text_font(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, f10) end

  #- try zooming an image -#
  img = lv.img_create(btn)
  label.del()
  img.set_src(lv.SYMBOL_OK)



#-
  f8 = lv.montserrat_font(8)
  if f8 != nil label.set_style_local_text_font(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, f8) end
-#
#-
f14 = lv.montserrat_font(14)
f28 = lv.montserrat_font(28)
btn.set_height(80)
btn.set_style_local_bg_color(0, lv.STATE_DEFAULT, lv_color(0xFFEEFF))
label.set_style_local_text_font(0, lv.STATE_DEFAULT, f28)


scr = lv.scr_act()
scr.set_style_local_bg_color(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, lv_color(0x400000))
scr.set_style_local_value_font(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, f28)

label.set_style_local_value_font(lv.BTN_PART_MAIN, lv.STATE_DEFAULT, f28)
-#

  #- lv_obj_set_style_local_bg_color(lv_scr_act(), LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED); -#
  #- lv.obj_set_style_local_bg_color(lv.scr_act(), lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, lv_color(0x00FF00)) -#

  #- lv_obj_set_style_local_bg_opa( lv_scr_act(), LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);  -#

  #-
  lv.demo()
  -#
else
  print('ILI9341 pins are not configured')
end

#-
lv.obj_set_style_local_bg_color(lv.scr_act(), lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, lv_color(lv.BLACK))

gauge = lv.gauge_create(lv.scr_act())
gauge.set_size(150,150)
co = lv.cpicker_create(lv.scr_act())
co.set_size(150,150)
co.set_pos(170,20)
k = lv.keyboard_create(lv.scr_act())

cal = lv.calendar_create(lv.scr_act())
cal.del()
c = lv.checkbox_create(lv.scr_act())
c.del()
c = lv.chart_create(lv.scr_act())
c.del()

co.del()

k = lv.keyboard_create(lv.scr_act())
k.del()

led = lv.led_create(lv.scr_act())
led.del()

m = lv.msgbox_create(lv.scr_act())
m.del()

# menu item
rol = lv.roller_create(lv.scr_act())
rol.del()
sl = lv.slider_create(lv.scr_act())
sl.del()

sp = lv.spinner_create(lv.scr_act())
sp.del()

w = lv.win_create(lv.scr_act())
w.del()

t = lv.textarea_create(lv.scr_act())
t.set_text("Tasmota")
t.del()

-#