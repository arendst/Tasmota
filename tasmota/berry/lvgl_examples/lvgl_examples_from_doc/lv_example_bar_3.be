def set_temp(bar, temp)
    bar.set_value(temp, lv.ANIM_ON)
end

#
# A temperature meter example
#


style_indic = lv.style()

style_indic.init()
style_indic.set_bg_opa(lv.OPA_COVER)
style_indic.set_bg_color(lv.palette_main(lv.PALETTE_RED))
style_indic.set_bg_grad_color(lv.palette_main(lv.PALETTE_BLUE))
style_indic.set_bg_grad_dir(lv.GRAD_DIR_VER)

bar = lv.bar(lv.scr_act())
bar.add_style(style_indic, lv.PART_INDICATOR)
bar.set_size(20, 200)
bar.center()
bar.set_range(-20, 40)

a = lv.anim()
a.init()
a.set_time(3000)
a.set_playback_time(3000)
a.set_var(bar)
a.set_values(-20, 40)
a.set_repeat_count(lv.ANIM_REPEAT_INFINITE)
a.set_custom_exec_cb(cb.gen_cb(/ a, val -> set_temp(bar,val)))
a.start()
