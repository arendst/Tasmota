# #- LVGL Examples
#  -
#  - drawing example #1 from https://docs.lvgl.io/latest/en/html/overview/drawing.html
# --#
# lv.start()

# lv.scr_act().set_style_bg_color(lv_color(0xFF3333), lv.PART_MAIN | lv.STATE_DEFAULT)

# om = lv_objmask(lv.scr_act())
# om.set_size(200, 200)
# om.align(0, lv.ALIGN_CENTER, 0, 0)
# label = lv_label(om)
# label.set_long_mode(lv.LABEL_LONG_BREAK)
# label.set_align(lv.LABEL_ALIGN_CENTER)
# label.set_width(180)
# label.set_text("This label will be masked out. See how it works.")
# label.align(0, lv.ALIGN_IN_TOP_MID, 0, 20)

# cont = lv_cont(om)
# cont.set_size(180, 100)
# cont.set_drag(true)
# cont.align(0, lv.ALIGN_IN_BOTTOM_MID, 0, -10)

# btn = lv_btn(cont)
# btn.align(0, lv.ALIGN_CENTER, 0, 0)
# btn.set_style_local_value_str(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, "Button")

# lv.refr_now()
# tasmota.delay(1000)

# a=lv_area()
# r1 = lv_draw_mask_radius_param()

# a.x1 = 10
# a.y1 = 10
# a.x2 = 190
# a.y2 = 190
# lv.draw_mask_radius_init(r1, a, lv.RADIUS_CIRCLE, false)
# om.add_mask(r1)

# lv.refr_now()
# tasmota.delay(1000)

# a.x1 = 100
# a.y1 = 100
# a.x2 = 150
# a.y2 = 150
# lv.draw_mask_radius_init(r1, a, lv.RADIUS_CIRCLE, true)
# om.add_mask(r1)

# lv.refr_now()
# tasmota.delay(1000)

# l1 = lv_draw_mask_line_param()
# lv.draw_mask_line_points_init(l1, 0, 0, 100, 200, lv.DRAW_MASK_LINE_SIDE_TOP)
# om.add_mask(l1)

# lv.refr_now()
# tasmota.delay(1000)

# f1= lv_draw_mask_fade_param()
# a.x1 = 100
# a.y1 = 0
# a.x2 = 200
# a.y2 = 200
# lv.draw_mask_fade_init(f1, a, lv.OPA_TRANSP, 0, lv.OPA_COVER, 150)
# om.add_mask(f1)