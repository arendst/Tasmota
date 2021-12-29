#- ------------------------------------------------------------ -#
#  Module `lv` - allows to extend the soidified `lv` module
#- ------------------------------------------------------------ -#
lv = module("lv")

# rename `lv` to `lv_ntv` and replace `lv` with `lv_tasmota`
def lv_module_init(lv_solidified)
  var lv_new = module("lv")   # create a dynamic module
  lv_new.member = lv_solidified.member
  # lv_new.lv_solidified = lv_solidified
  return lv_new
end

lv.init = lv_module_init

def lv0_member_ntv() end

lv.member = lv0_member_ntv

return lv
