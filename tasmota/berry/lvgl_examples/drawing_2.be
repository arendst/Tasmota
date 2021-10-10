# #- LVGL Examples
#  -
#  - drawing example #2 from https://docs.lvgl.io/latest/en/html/overview/drawing.html
# --#
# lv.start()

# lv.scr_act().set_style_local_bg_color(lv.OBJ_PART_MAIN, lv.STATE_DEFAULT, lv_color(0xAAAAAA))

# MASK_WIDTH=100
# MASK_HEIGHT=50

# # Create the mask of a text by drawing it to a canvas
# mask_map = bytes()
# mask_map.resize(MASK_WIDTH * MASK_HEIGHT * 4)

# # Create a "8 bit alpha" canvas and clear it
# canvas = lv_canvas(lv.scr_act())
# canvas.set_buffer(mask_map, MASK_WIDTH, MASK_HEIGHT, lv.IMG_CF_ALPHA_8BIT)
# canvas.fill_bg(lv_color(lv.COLOR_BLACK), lv.OPA_TRANSP)
# canvas.fill_bg(lv_color(lv.COLOR_BLACK), lv.OPA_50)

# # Draw a label to the canvas. The result "image" will be used as mask
# label_dsc = lv_draw_label_dsc()
# lv.draw_label_dsc_init(label_dsc)
# label_dsc.color = lv_color(lv.COLOR_WHITE)
# canvas.draw_text(5, 5, MASK_WIDTH, label_dsc, "Text with gradient", lv.LABEL_ALIGN_CENTER)

# # The mask is read, the canvas is not required anymore
# canvas.del()

# # Create an object mask which will use the created mask
# om = lv_objmask(lv.scr_act())
# om.set_size(MASK_WIDTH, MASK_HEIGHT)
# om.align(0, lv.ALIGN_CENTER, 0, 0)

# # Add the created mask map to the object mask
# m = lv_draw_mask_map_param()
# a = lv_area()
# a.x1 = 0
# a.y1 = 0
# a.x2 = MASK_WIDTH - 1
# a.y2 = MASK_HEIGHT - 1
# lv.draw_mask_map_init(m, a, mask_map)
# om.add_mask(m)

# # Create a style with gradient
# style_bg = lv_style()
# style_bg.set_bg_opa(lv.STATE_DEFAULT, lv.OPA_COVER)
# style_bg.set_bg_color(lv.STATE_DEFAULT, lv_color(lv.COLOR_RED))
# style_bg.set_bg_grad_color(lv.STATE_DEFAULT, lv_color(lv.COLOR_BLUE))
# style_bg.set_bg_grad_dir(lv.STATE_DEFAULT, lv.GRAD_DIR_HOR)

# # Create and object with the gradient style on the object mask.
# # The text will be masked from the gradient
# bg = lv_obj(om)
# bg.reset_style_list(lv.OBJ_PART_MAIN)
# bg.add_style(lv.OBJ_PART_MAIN, style_bg)
# bg.set_size(MASK_WIDTH, MASK_HEIGHT)
