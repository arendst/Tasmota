#- start LVGL and init environment -#
lv.start()

import string

scr = lv.scr_act()            # default screean object

def ctorcb(class_p, obj)
    import string
    print(string.format("> ctorcb class_p=0x%08X obj=0x%08X", class_p, obj))
end
def dtorcb(class_p, obj)
    import string
    print(string.format("> dtorcb class_p=0x%08X obj=0x%08X", class_p, obj))
end
def eventcb(class_p, e)
    import string
    import introspect
    var event = lv_event(e)
    var obj = introspect.fromptr(event.user_data)
    var event_code = event.code
    var target = event.target
    print(string.format("> event class_p=0x%08X event=%s obj=%s code=%i",
            class_p, str(event), str(obj), event_code))
end
var cb_ctor = tasmota.gen_cb(ctorcb)
var cb_dtor = tasmota.gen_cb(dtorcb)
var cb_event = tasmota.gen_cb(eventcb)

# static void lv_img_event(const lv_obj_class_t * class_p, lv_event_t * e)

#-
class_def = lv_obj_class(lv_obj._class).copy()
print(class_def)
print(string.format("base_class = 0x%08X", class_def.base_class))
print(string.format("constructor_cb = 0x%08X", class_def.constructor_cb))
print(string.format("destructor_cb = 0x%08X", class_def.destructor_cb))
print(string.format("user_data = 0x%08X", class_def.user_data))
print(string.format("event_cb = 0x%08X", class_def.event_cb))
print(string.format("width_def = %i", class_def.width_def))
print(string.format("height_def = %i", class_def.height_def))
print(string.format("editable = %i", class_def.editable))
print(string.format("group_def = %i", class_def.group_def))
print(string.format("instance_size = %i", class_def.instance_size))
-#

import introspect

class_def = lv_obj_class()
class_def.base_class = lv_obj._class
class_def.constructor_cb = cb_ctor
class_def.destructor_cb = cb_dtor
class_def.user_data = introspect.toptr(class_def)       # self
class_def.event_cb = cb_event
class_def.width_def = lv.SIZE_CONTENT
class_def.height_def = lv.SIZE_CONTENT
class_def.editable = lv.OBJ_CLASS_EDITABLE_INHERIT
class_def.group_def = lv.OBJ_CLASS_GROUP_DEF_INHERIT
class_def.instance_size = lv_obj_class(class_def.base_class).instance_size

var obj = lv.obj_class_create_obj(class_def, scr)
print("--marker--")
obj.set_user_data(introspect.toptr(obj))
obj.class_init_obj()


# obj.class_init_obj()
# ctorcb 1065361552 1065364576
# dtorcb 1065361552 1073421092
# dtorcb 1065361552 1073421044
# dtorcb 1065361552 1073421732
# dtorcb 1065361552 1073421732
# obj.class_init_obj()
# ctorcb 1065361552 1065364576
# dtorcb 1065361552 1073421092
# dtorcb 1065361552 1073421044
# dtorcb 1065361552 1073421732
# dtorcb 1065361552 1073421732


# lv_obj_t * lv_btn_create(lv_obj_t * parent)
# {
#     LV_LOG_INFO("begin")
#     lv_obj_t * obj = lv_obj_class_create_obj(MY_CLASS, parent);
#     lv_obj_class_init_obj(obj);
#     return obj;
# }

# LV_SIZE_CONTENT


# LV_OBJ_CLASS_EDITABLE_INHERIT
# LV_OBJ_CLASS_EDITABLE_TRUE
# LV_OBJ_CLASS_EDITABLE_FALSE

# LV_OBJ_CLASS_GROUP_DEF_INHERIT
# LV_OBJ_CLASS_GROUP_DEF_TRUE
# LV_OBJ_CLASS_GROUP_DEF_FALSE

# class_def = lv_obj_class(lv_btn._class).copy()

# lv_event = [            # valid LVGL8
#     [lv_obj_ptr, "target"],
#     [lv_obj_ptr, "current_target"],
#     [lv_event_code, "code"],
#     [ptr, "user_data"],
#     [ptr, "param"],
#     [lv_event_ptr, "prev"],
#     [uint8_t_1, "deleted"],
# ]
# lv_event = ctypes.structure(lv_event, "lv_event")

# lv_obj_class = [            # valid LVGL8
#     [lv_obj_class_ptr, "base_class"],
#     [constructor_cb, "constructor_cb"],
#     [destructor_cb, "destructor_cb"],
#     [ptr, "user_data"],
#     [event_cb, "event_cb"],
#     [lv_coord, "width_def"],
#     [lv_coord, "height_def"],
#     [uint8_t_2, "editable"],
#     [uint8_t_2, "group_def"],
#     [uint16_t_16, "instance_size"],
# ]


# const be_ctypes_structure_t be_lv_obj_class = {
#   29,  /* size in bytes */
#   10,  /* number of elements */
#   (const be_ctypes_structure_item_t[10]) {
#     { "base_class", 0, 0, 0, 4, 0 },
#     { "constructor_cb", 4, 0, 0, 4, 0 },
#     { "destructor_cb", 8, 0, 0, 4, 0 },
#     { "editable", 24, 0, 2, 0, 0 },
#     { "event_cb", 16, 0, 0, 4, 0 },
#     { "group_def", 24, 2, 2, 0, 0 },
#     { "height_def", 22, 0, 0, 12, 0 },
#     { "instance_size", 24, 4, 16, 0, 0 },
#     { "user_data", 12, 0, 0, 4, 0 },
#     { "width_def", 20, 0, 0, 12, 0 },
# }};

# typedef struct be_ctypes_structure_item_t {
#     const char * name;
#     uint16_t  offset_bytes;
#     uint8_t   offset_bits : 3;
#     uint8_t   len_bits : 5;
#     int8_t    type : 5;
#     uint8_t   mapping : 3;
# } be_ctypes_structure_item_t;