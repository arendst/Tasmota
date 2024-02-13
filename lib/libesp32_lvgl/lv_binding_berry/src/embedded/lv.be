#- ------------------------------------------------------------ -#
#  Module `lv` - allows to extend the solidified `lv` module
#
#  When doing `import lv`, the function `lv_module_init` below
#  is ran, with the solidified `lv` module passed as argument.
#
#  The solidified module is read-only and cannot be extended.
#  This is why we are dynamically creating a new `lv` module
#  object that can be written to, and we assign the same
#  `member` function as the solidified one to allow for
#  dynamic members resolution. The `lv` solidified module has only
#  dynamic members.
#- ------------------------------------------------------------ -#

#@ solidify:lv_module_init,weak
def lv_module_init(lv_solidified)
  var lv_new = module("lv")   # create a dynamic module
  lv_new.member = lv_solidified.member
  # lv_new.lv_solidified = lv_solidified

  # add mapping from LVGL 8 legacy names and LVGL 9
  lv_new.scr_act = lv_solidified.screen_active
  lv_new.img = lv_solidified.image
  lv_new.disp = lv_solidified.display
  lv_new.btn = lv_solidified.button
  lv_new.btnmatrix = lv_solidified.buttonmatrix
  return lv_new
end
