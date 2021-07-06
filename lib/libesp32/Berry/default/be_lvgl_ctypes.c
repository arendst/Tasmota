/********************************************************************
 * Tasmota LVGL ctypes mapping
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_LVGL

#include "lvgl.h"


// binary search within an array of sorted strings
// the first 4 bytes are a pointer to a string
// returns 0..total_elements-1 or -1 if not found

int32_t bin_search_ctypes(const char * needle, const void * table, size_t elt_size, size_t total_elements) {
    int32_t low = 0;
    int32_t high = total_elements - 1;
    int32_t mid = (low + high) / 2;
    // start a dissect
    while (low <= high) {
        const char * elt = *(const char **) ( ((uint8_t*)table) + mid * elt_size );
        int32_t comp = strcmp(needle, elt);
        if (comp < 0) {
        high = mid - 1;
        } else if (comp > 0) {
        low = mid + 1;
        } else {
        break;
        }
        mid = (low + high) / 2;
    }
    if (low <= high) {
        return mid;
    } else {
        return -1;
    }
}


enum {

    ctypes_i32    = 14,
    ctypes_i16    = 12,
    ctypes_i8     = 11,
    ctypes_u32    =  4,
    ctypes_u16    =  2,
    ctypes_u8     =  1,

    // big endian
    ctypes_be_i32 = -14,
    ctypes_be_i16 = -12,
    ctypes_be_i8  = -11,
    ctypes_be_u32 =  -4,
    ctypes_be_u16 =  -2,
    ctypes_be_u8  =  -1,

    ctypes_bf     = 0,    //bif-field
};

typedef struct be_ctypes_structure_item_t {
    const char * name;
    uint16_t  offset_bytes;
    uint8_t   offset_bits : 3;
    uint8_t   len_bits : 5;
    int8_t    type;
} be_ctypes_structure_item_t;

typedef struct be_ctypes_structure_t {
    uint16_t  size_bytes;       /* size in bytes */
    uint16_t  size_elt;         /* number of elements */
    const be_ctypes_structure_item_t * items;
} be_ctypes_structure_t;

typedef struct be_ctypes_class_t {
    const char * name;
    const be_ctypes_structure_t * definitions;
} be_ctypes_class_t;

typedef struct be_ctypes_classes_t {
    uint16_t  size;
    const be_ctypes_class_t * classes;
} be_ctypes_classes_t;

// const be_ctypes_class_t * g_ctypes_classes = NULL;

// extern void berry_log_C(const char * berry_buf, ...);
int be_ctypes_init(bvm *vm) {
    // get global array of classes from global variable '.ctypes_classes'
    be_getglobal(vm, ".ctypes_classes");
    const be_ctypes_classes_t * be_ctypes_classes = (const be_ctypes_classes_t *) be_tocomptr(vm, -1);
    be_pop(vm, 1);
    // berry_log_C("be_ctypes_init> be_ctypes_class = %p", be_ctypes_classes);
    const char * class_name = be_classname(vm, 1);
    // berry_log_C("be_ctypes_init> class_name = %s", class_name);

    // call super(self, bytes)
    be_getglobal(vm, "super");      // push super function
    be_pushvalue(vm, 1);            // push self instance
    be_getglobal(vm, "bytes");      // push bytes class
    be_call(vm, 2);
    be_pop(vm, 2);
    // berry_log_C("be_ctypes_init> super found %p", be_toint(vm, -1));

    // call bytes.init(self)
    be_getmember(vm, -1, "init");
    be_pushvalue(vm, -2);
    be_call(vm, 1);
    be_pop(vm, 3);
    // berry_log_C("be_ctypes_init> init called");

    // look for class definition
    int32_t class_idx = bin_search_ctypes(class_name, &be_ctypes_classes->classes[0], sizeof(be_ctypes_class_t), be_ctypes_classes->size);
    if (class_idx >= 0) {
        // found
        const be_ctypes_structure_t * definitions = be_ctypes_classes->classes[class_idx].definitions;
        // store definition in '.def'
        // berry_log_C("Found definitions = %p", definitions);
        be_pushcomptr(vm, (void*) definitions);
        be_setmember(vm, 1, ".def");
        be_pop(vm, 1);

        // call self.resize(definitions->size_bytes)
        be_getmember(vm, 1, "resize");
        be_pushvalue(vm, 1);
        be_pushint(vm, definitions->size_bytes);
        be_call(vm, 2);
        be_pop(vm, 3);
    }

    be_return(vm);
}

int be_ctypes_member(bvm *vm) {
    int argc = be_top(vm);
    be_getmember(vm, 1, ".def");
    const be_ctypes_structure_t *definitions;
    definitions = (const be_ctypes_structure_t *) be_tocomptr(vm, -1);
    be_pop(vm, 1);
    const char *name = be_tostring(vm, 2);

    // look for member
    int32_t member_idx = bin_search_ctypes(name, &definitions->items[0], sizeof(be_ctypes_structure_item_t), definitions->size_elt);
    if (member_idx >= 0) {
        const be_ctypes_structure_item_t *member = &definitions->items[member_idx];
        // berry_log_C("member found bytes=%i, bits=%i, len_bits=%i, type=%i", member->offset_bytes, member->offset_bits, member->len_bits, member->type);

        // dispatch according to types
        if (ctypes_bf == member->type) {
            // bitfield
            be_getmember(vm, 1, "getbits");
            be_pushvalue(vm, 1);        // self
            be_pushint(vm, member->offset_bytes * 8 + member->offset_bits);
            be_pushint(vm, member->len_bits);
            be_call(vm, 3);
            be_pop(vm, 3);
            be_return(vm);
        } else {
            // general int support
            int size = member->type;       // eventually 1/2/4, positive if little endian, negative if big endian
            int sign = false;            // signed int
            if (size >= ctypes_i8) {
                size -= ctypes_i8 - 1;
                sign = true;
            }
            if (size <= ctypes_be_i8) {
                size += ctypes_be_i8 - 1;
                sign = true;
            }
            // get
            be_getmember(vm, 1, sign ? "geti" : "get");   // self.get or self.geti
            be_pushvalue(vm, 1);        // push self
            be_pushint(vm, member->offset_bytes);
            be_pushint(vm, size);
            be_call(vm, 3);
            be_pop(vm, 3);
            be_return(vm);
        }
    }

    be_return_nil(vm);
}

// setmember takes 3 arguments:
// 1: self (subclass of bytes())
// 2: name of member
// 3: value
int be_ctypes_setmember(bvm *vm) {
    int argc = be_top(vm);
    be_getmember(vm, 1, ".def");
    const be_ctypes_structure_t *definitions;
    definitions = (const be_ctypes_structure_t *) be_tocomptr(vm, -1);
    be_pop(vm, 1);
    const char *name = be_tostring(vm, 2);

    // look for member
    int32_t member_idx = bin_search_ctypes(name, &definitions->items[0], sizeof(be_ctypes_structure_item_t), definitions->size_elt);
    if (member_idx >= 0) {
        const be_ctypes_structure_item_t *member = &definitions->items[member_idx];
        // berry_log_C("member found bytes=%i, bits=%i, len_bits=%i, type=%i", member->offset_bytes, member->offset_bits, member->len_bits, member->type);

        // dispatch according to types
        if (ctypes_bf == member->type) {
            // bitfield
            be_getmember(vm, 1, "setbits");
            be_pushvalue(vm, 1);        // self
            be_pushint(vm, member->offset_bytes * 8 + member->offset_bits);
            be_pushint(vm, member->len_bits);
            be_pushvalue(vm, 3);        // val
            be_call(vm, 4);
            be_pop(vm, 5);
            be_return_nil(vm);
        } else {
            // general int support
            int size = member->type;       // eventually 1/2/4, positive if little endian, negative if big endian
            int sign = false;            // signed int
            if (size >= ctypes_i8) {
                size -= ctypes_i8 - 1;
                sign = true;
            }
            if (size <= ctypes_be_i8) {
                size += ctypes_be_i8 - 1;
                sign = true;
            }
            // set
            be_getmember(vm, 1, sign ? "seti" : "set");   // self.get or self.geti
            be_pushvalue(vm, 1);        // push self
            be_pushint(vm, member->offset_bytes);
            be_pushvalue(vm, 3);        // val
            be_pushint(vm, size);
            be_call(vm, 4);
            be_pop(vm, 5);
            be_return_nil(vm);
        }
    }

    be_return_nil(vm);
}

BE_EXPORT_VARIABLE extern const bclass be_class_bytes;

#if BE_USE_PRECOMPILED_OBJECT
#include "../generate/be_fixed_be_class_lv_ctypes.h"
#endif

void be_load_lvgl_ctypes_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
    static const bnfuncinfo members[] = {
        { ".def", NULL },               // pointer to definition
        { "init", be_ctypes_init },
        { "member", be_ctypes_member },
        { "setmember", be_ctypes_setmember },
        { NULL, NULL }
    };
    be_regclass(vm, "ctypes_bytes", members);

    be_getglobal(vm, "ctypes_bytes");
    be_getglobal(vm, "bytes");
    be_setsuper(vm, -2);
    be_pop(vm, 2);
#else
    be_pushntvclass(vm, &be_class_lv_ctypes);
    be_setglobal(vm, "ctypes_bytes");
    be_pop(vm, 1);
#endif
}

/* @const_object_info_begin
class be_class_lv_ctypes (scope: global, name: ctypes_bytes, super: be_class_bytes) {
    .def, var
    init, func(be_ctypes_init)
    member, func(be_ctypes_member)
    setmember, func(be_ctypes_setmember)
}
@const_object_info_end */

#endif // USE_LVGL