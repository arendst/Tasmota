import re

in_file = "lv_widgets.txt"
out_file = "out.c"

m = re.compile("(.*?)\s(\w+)\((.*?)\);")

return_types = {
  "void": "",
  "bool": "b",
  "uint8_t": "i",
  "uint16_t": "i",
  "int16_t": "i",
  "uint32_t": "i",
  "void *": "i",
  "const void *": "i",
  "char *": "s",
  "const char *": "s",
  "lv_obj_user_data_t": "i",

  "lv_coord_t": "i",
  "lv_res_t": "i",
  "lv_style_int_t": "i",
  "lv_opa_t": "i",
  "lv_drag_dir_t": "i",
  "lv_bidi_dir_t": "i",
  "lv_state_t": "i",
  "lv_label_long_mode_t": "i",
  "lv_label_align_t": "i",
  "lv_arc_type_t":"i",
  "lv_chart_axis_t":"i",
  "lv_bar_type_t":"i",
  "lv_btn_state_t":"i",
  "lv_point_t":"i",

  "lv_obj_t *": "lv_obj",
  "lv_color_t": "lv_color",
  "lv_style_t *": "lv_style",
  #"lv_disp_t*": "lv_disp",
  #"lv_style_list_t*": "",
}

lv = {}
lvs = []   # special case for case for lv_style
lv0 = []        # function in lvlg module

lv_prefix = ['obj', 'label', 'group', 'arc', 'bar', 'btn', 'btnmatrix', 'calendar', 'canvas', 'chart', 'checkbox']

# convert a C return type to the string representation for the Berry parser
# using return_types
def c_convert_ret_type(c_ret):
  return return_types.get(c_ret, "")

with open(in_file) as f, open(out_file,"w") as out:
  for l_raw in f:
    l_raw = re.sub('static ', '', l_raw)
    l_raw = re.sub('inline ', '', l_raw)
    l_raw = l_raw.strip(" \t\n\r")    # remove leading or trailing spaces
    l_raw = re.sub('//.*$', '', l_raw) # replace any multi-space with a single space
    if (len(l_raw) == 0): continue

    g = m.search(l_raw)
    if g:
      # if match, we parse the line
      ret_type = g.group(1)
      ret_type = re.sub('\s+', ' ', ret_type) # replace any multi-space with a single space

      if ret_type not in return_types:
        print(f"  // Skipping unsupported return type: {ret_type}")
        continue

      # convert return type
      c_ret = c_convert_ret_type(ret_type)

      # convert arguments
      # TODO
      c_args = None

      # analyze function name and determine if it needs to be assigned to a specific class
      func_name = g.group(2)
      if func_name.startswith("_"): continue            # skip low-level
      if func_name.startswith("lv_debug_"): continue    # skip debug

      c_func_name = ""

      found = False
      for subtype in lv_prefix:
        if func_name.startswith("lv_" + subtype + "_"):
          be_name = re.sub("^lv_" + subtype + "_", '', func_name)
          c_func_name = "lvbe_" + subtype + "_" + be_name
          if subtype not in lv: lv[subtype] = []    # add entry
          # special case if the function is create, we change the return type to macth the class
          if be_name == 'create':
            c_ret = "lv_" + subtype
          lv[subtype].append( [ c_func_name, c_ret, c_args, func_name, be_name ] )
          found = True
          break

      if found: continue
      # if func_name.startswith("lv_obj_"):
      #   c_func_name = "lvo_" + re.sub('^lv_obj_', '', func_name)
      #   lvo.append( [ c_func_name, c_ret, c_args, func_name ] )
      # elif func_name.startswith("lv_label_"):
      #   c_func_name = "lvo_label_" + re.sub('^lv_label_', '', func_name)
      #   lvo.append( [ c_func_name, c_ret, c_args, func_name ] )
      # elif func_name.startswith("lv_group_"):
      #   c_func_name = "lvg_" + re.sub('^lv_group_', '', func_name)
      #   lvg.append( [ c_func_name, c_ret, c_args, func_name ] )


      # elif func_name.startswith("lv_arc_"):
      #   c_func_name = "lvg_" + re.sub('^lv_arc_', '', func_name)
      #   lvg.append( [ c_func_name, c_ret, c_args, func_name ] )


      if func_name.startswith("lv_style_"):
        be_name = re.sub('^lv_style_', '', func_name)
        c_func_name = "lvs_" + be_name
        lvs.append( [ c_func_name, c_ret, c_args, func_name, be_name ] )

      else:
        be_name = re.sub('^lv_', '', func_name)
        c_func_name = "lvbe0_" + be_name
        lv0.append( [ c_func_name, c_ret, c_args, func_name, be_name ] )


print(lv)

      #print(f"orig {func_name} berry {c_func_name}")

      #print(g.group(1))
      #print(g.group(2))
      #print(g.group(3))

print("/********************************************************************")
print(" * Generated code, don't edit")
print(" *******************************************************************/")

for subtype, flv in lv.items():
  print(f"  /* `lv_{subtype}` methods */")
  for f in flv:
    c_func_name = f[0]
    c_ret_type = f[1]
    c_argc = f[2]
    if c_argc is not None: c_argc = "\"" + c_argc + "\""
    orig_func_name = f[3]
    #print(f"  int {c_func_name}(bvm *vm)       \{ return (vm, be_call_c_func(void*) &")
    print(f"  int {c_func_name}(bvm *vm)       {{ return be_call_c_func(vm, (void*) &{orig_func_name}, \"{c_ret_type}\", { c_argc if c_argc else 'nullptr'}); }}")
  print()

print("/********************************************************************/")
print()


print("/********************************************************************")
print(" * Generated code, don't edit")
print(" *******************************************************************/")

for subtype, flv in lv.items():
  print(f"/* `lv_{subtype}` external functions definitions */")
  for f in flv:
    c_func_name = f[0]
    print(f"extern int {c_func_name}(bvm *vm);")
  
  print()

  # class definitions
  print(f"void be_load_lv_{subtype}_lib(bvm *vm) {{")
  print(f"  static const bnfuncinfo members[] = {{")
  print(f"    {{ \".p\", NULL }},")
  print(f"    {{ \"init\", lvx_init_2 }},")
  print(f"    {{ \"tostring\", lvx_tostring }},")
  print()
  for f in flv:

    c_func_name = f[0]
    be_name = f[4]
    print(f"    {{ \"{be_name}\", {c_func_name} }},")

  print()
  print(f"    // {{ NULL, (bntvfunc) BE_CLOSURE }}, /* mark section for berry closures */")
  print()
  print(f"    {{ NULL, NULL }}")
  print(f"  }};")
  print(f"  be_regclass(vm, \"lv_{subtype}\", members);")
  print(f"}};")
  print()
  print(f"/* @const_object_info_begin")
  print(f"class be_class_lv_{subtype} (scope: global, name: lv_{subtype}, super: lv_obj) {{")
  print(f"    .p, var")
  print(f"    init, func(lvx_init_2)")
  print(f"    tostring, func(lvx_tostring)")

  for f in flv:
    c_func_name = f[0]
    be_name = f[4]
    print(f"    {be_name}, func({c_func_name})")

  print(f"}}")
  print(f"@const_object_info_end */")
  print(f"//#include \"../generate/be_fixed_be_class_lv_{subtype}.h\"")
  print()

print()
print(f"void be_load_lv_all_lib(bvm *vm) {{")

for subtype in lv:
  print(f"  be_load_lv_{subtype}_lib(vm);")

print(f"}};")



# for f in lvo:
#   c_func_name = f[0]
#   print(f"extern int {c_func_name}(bvm *vm);")

# print("/********************************************************************/")
# print()

# print("/********************************************************************")
# print(" * Generated code, don't edit")
# print(" *******************************************************************/")

# print("/* `lv_object` methods */")
# print()

# for f in lvo:
#   c_func_name = f[0]
#   berry_name = c_func_name
#   berry_name = re.sub('^lvo_label_', '', berry_name)
#   berry_name = re.sub('^lvo_', '', berry_name)
#   print(f"        {{ \"{berry_name}\", {c_func_name} }},")

# print("/********************************************************************/")
# print()





# print("/********************************************************************")
# print(" * Generated code, don't edit")
# print(" *******************************************************************/")

# print("/* `lv_group methods */")
# print()

# for f in lvg:
#   c_func_name = f[0]
#   c_ret_type = f[1]
#   c_argc = f[2]
#   if c_argc is not None: c_argc = "\"" + c_argc + "\""
#   orig_func_name = f[3]
#   #print(f"  int {c_func_name}(bvm *vm)       \{ return (vm, be_call_c_func(void*) &")
#   print(f"  int {c_func_name}(bvm *vm)       {{ return be_call_c_func(vm, (void*) &{orig_func_name}, \"{c_ret_type}\", { c_argc if c_argc else 'nullptr'}); }}")

# print("/********************************************************************/")
# print()


# print("/********************************************************************")
# print(" * Generated code, don't edit")
# print(" *******************************************************************/")

# print("/* `lv_group external functions definitions */")
# print()

# for f in lvg:
#   c_func_name = f[0]
#   print(f"extern int {c_func_name}(bvm *vm);")

# print("/********************************************************************/")
# print()

# print("/********************************************************************")
# print(" * Generated code, don't edit")
# print(" *******************************************************************/")

# print("/* `lv_group` methods */")
# print()

# for f in lvg:
#   c_func_name = f[0]
#   berry_name = c_func_name
#   berry_name = re.sub('^lvg_', '', berry_name)
#   print(f"        {{ \"{berry_name}\", {c_func_name} }},")

# print("/********************************************************************/")
# print()




# print("/********************************************************************")
# print(" * Generated code, don't edit")
# print(" *******************************************************************/")

# print("/* `lv_style methods */")
# print()

# for f in lvs:
#   c_func_name = f[0]
#   c_ret_type = f[1]
#   c_argc = f[2]
#   if c_argc is not None: c_argc = "\"" + c_argc + "\""
#   orig_func_name = f[3]
#   #print(f"  int {c_func_name}(bvm *vm)       \{ return (vm, be_call_c_func(void*) &")
#   print(f"  int {c_func_name}(bvm *vm)       {{ return be_call_c_func(vm, (void*) &{orig_func_name}, \"{c_ret_type}\", { c_argc if c_argc else 'nullptr'}); }}")

# print("/********************************************************************/")
# print()


# print("/********************************************************************")
# print(" * Generated code, don't edit")
# print(" *******************************************************************/")

# print("/* `lv_style external functions definitions */")
# print()

# for f in lvs:
#   c_func_name = f[0]
#   print(f"extern int {c_func_name}(bvm *vm);")

# print("/********************************************************************/")
# print()

# print("/********************************************************************")
# print(" * Generated code, don't edit")
# print(" *******************************************************************/")

# print("/* `lv_style` methods */")
# print()

# for f in lvs:
#   c_func_name = f[0]
#   berry_name = c_func_name
#   berry_name = re.sub('^lvs_', '', berry_name)
#   print(f"        {{ \"{berry_name}\", {c_func_name} }},")

# print("/********************************************************************/")
# print()




# print("/********************************************************************")
# print(" * Generated code, don't edit")
# print(" *******************************************************************/")

# print("/* `lvgl` functions */")
# print()

# for f in lv:
#   c_func_name = f[0]
#   c_ret_type = f[1]
#   c_argc = f[2]
#   if c_argc is not None: c_argc = "\"" + c_argc + "\""
#   orig_func_name = f[3]
#   #print(f"  int {c_func_name}(bvm *vm)       \{ return (vm, be_call_c_func(void*) &")
#   print(f"  int {c_func_name}(bvm *vm)       {{ return be_call_c_func(vm, (void*) &{orig_func_name}, \"{c_ret_type}\", { c_argc if c_argc else 'nullptr'}); }}")

# print("/********************************************************************/")
# print()



  #int lv0_label_create(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_label_create, LV_OBJ_CLASS); }


#print(lvo)