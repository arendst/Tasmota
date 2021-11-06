import re
import sys

lv_module_file = "lv_gpio_enum.h"

out_prefix = "../../tasmota/berry/include/"

be_gpio_defines = "be_gpio_defines.h"

lv_module = []

def try_int(s):
  try:
    v = int(s)
    return v
  except ValueError:
    return None

# parse module file containing numerical constants
with open(lv_module_file) as f:
  for l_raw in f:
    l_raw = l_raw.strip(" \t\n\r")    # remove leading or trailing spaces
    if l_raw.startswith("//"):
      lv_module.append( [ None, l_raw ] )   # if key in None then add comment line
    l_raw = re.sub('//.*$', '', l_raw) # remove trailing comments
    l_raw = re.sub('\s+', '', l_raw) # remove all spaces
    l_raw = re.sub(',.*$', '', l_raw) # remove comma and anything after it
    if (len(l_raw) == 0): continue

    k_v = l_raw.split("=")
    if len(k_v) > 2:
      print(f"Error: cannot match {l_raw}")
      continue
    # extract the key name
    k = k_v[0]
    if k.startswith("_"):
      continue      # skip any label starting with '_'
    k = re.sub('^LV_', '', k) # remove remove any LV_ prefix
    v = None
    if len(k_v) == 2:   # value is included
      v = k_v[1]
      if k is None or v is None: continue   # we skip if key is void or value is void

    if not k.isidentifier():
      print(f"Error: {k} is not an identifier")
      continue

    lv_module.append( [k, v] )    # keep as string or None




sys.stdout = open(out_prefix + be_gpio_defines, 'w')
print("/********************************************************************")
print(" * Generated code, don't edit")
print(" *******************************************************************/")

print("""
const be_constint_t lv_gpio_constants[] = {
""")

lv_module2 = {}
for k_v in lv_module:
  (k,v) = k_v
  if k is not None:
    lv_module2[k] = v

# print the enums, symbols and functions
# Ex: { "ALIGN_BOTTOM_LEFT", LV_ALIGN_BOTTOM_LEFT },
#
# Encoding:
# 1. `COLOR_WHITE=0xFFFFFF` enum with explicit value
# 2. `LV_EVENT_ALL` enum with implicit value with same name resolved by C compiler
# 3. `$SYMBOL_OK="\xef\x80\x8c"` if starts with `$` then it's a string
# 4. `&seg7_font=lv0_load_seg7_font` if starts with `&` then it's a native function
#
# We need to sort ignoring the first char if it's not a letter
for k in sorted(lv_module2):
  v = lv_module2[k]
  # check the type of the value
  # if first char is '"' then it's a string and prefix with '$'
  # if first char is '&' then it's a function and prefix with '&'
  # if no value, then it's an enum, prefix with `LV_`
  # otherwise it's an int, leave if unchanged
  if v is not None:
    v_prefix = ""
    if v[0] == '"': v_prefix = "$"
    if v[0] == '&': v_prefix = "&"
    print(f"    {{ \"{v_prefix}{k}\", (int32_t) {v} }},")
  else:
    print(f"    {{ \"{k}\", {k} }},")

print("""
};

const size_t lv_gpio_constants_size = sizeof(lv_gpio_constants)/sizeof(lv_gpio_constants[0]);
""")

print("/********************************************************************/")
sys.stdout.close()

