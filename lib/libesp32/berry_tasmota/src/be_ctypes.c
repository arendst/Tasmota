/********************************************************************
 * Tasmota ctypes mapping
 *******************************************************************/
#include "be_constobj.h"
#include <string.h>

extern __attribute__((noreturn)) void be_raisef(bvm *vm, const char *except, const char *msg, ...);

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
    ctypes_i32    =  14,
    ctypes_i16    =  12,
    ctypes_i8     =  11,
    ctypes_u32    =   4,
    ctypes_u16    =   2,
    ctypes_u8     =   1,

    // big endian
    ctypes_be_i32 = -14,
    ctypes_be_i16 = -12,
    ctypes_be_i8  = -11,
    ctypes_be_u32 =  -4,
    ctypes_be_u16 =  -2,
    ctypes_be_u8  =  -1,

    // floating point
    ctypes_float  =   5,
    ctypes_double =  10,

    // pointer
    ctypes_ptr32  =   9,
    ctypes_ptr64  =  -9,

    ctypes_bf     =   0,    //bif-field
};

typedef struct be_ctypes_structure_item_t {
    const char * name;
    uint16_t  offset_bytes;
    uint8_t   offset_bits : 3;
    uint8_t   len_bits : 5;
    int8_t    type : 5;
    uint8_t   mapping : 3;
} be_ctypes_structure_item_t;

typedef struct be_ctypes_structure_t {
    uint16_t  size_bytes;       /* size in bytes */
    uint16_t  size_elt;         /* number of elements */
    const char **instance_mapping;  /* array of instance class names for automatic instanciation of class */
    const be_ctypes_structure_item_t * items;
} be_ctypes_structure_t;

typedef struct be_ctypes_class_t {
    const char * name;
    const be_ctypes_structure_t * definitions;
} be_ctypes_class_t;

typedef struct be_ctypes_classes_t {
    uint16_t  size;
    const char **instance_mapping;  /* array of instance class names for automatic instanciation of class */
    const be_ctypes_class_t * classes;
} be_ctypes_classes_t;

// const be_ctypes_class_t * g_ctypes_classes = NULL;

//
// Constructor for ctypes structure
//
// If no arg: allocate a bytes() structure of the right size, filled with zeroes
// Arg1 is instance self
// If arg 2 is int or comptr (and not null): create a mapped bytes buffer to read/write at a specific location (can be copied if need a snapshot)
// If arg 2 is a bytes object, consider it's comptr and map the buffer (it's basically casting). WARNING no size check is done so you can easily corrupt memory
int be_ctypes_init(bvm *vm) {
    int argc = be_top(vm);
    void * src_data = NULL;
    if (argc > 1 && (be_isint(vm, 2) || be_iscomptr(vm, 2) || be_isbytes(vm, 2))) {
        if (be_iscomptr(vm, 2)) {
            src_data = be_tocomptr(vm, 2);
        } else if (be_isbytes(vm, 2)) {
            be_getmember(vm, 2, ".p");
            src_data = be_tocomptr(vm, -1);
            be_pop(vm, 1);
        } else {
            src_data = (void*) be_toint(vm, 2);
        }
    }

    // look for class definition
    be_getmember(vm, 1, "_def");        // static class comptr
    const be_ctypes_structure_t *definitions;
    definitions = (const be_ctypes_structure_t *) be_tocomptr(vm, -1);
    be_pop(vm, 1);

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
    if (src_data) { be_pushcomptr(vm, src_data); }              // if mapped, push address
    be_pushint(vm, definitions ? -definitions->size_bytes : 0); // negative size signals a fixed size
    be_call(vm, src_data ? 3 : 2);      // call with 2 or 3 arguments depending on provided address
    be_pop(vm, src_data ? 4 : 3);
    // super(self, bytes) still on top of stack

    be_pop(vm, 1);

    be_return(vm);
}

//
// copy ctypes_bytes, with same class and same content
//
int be_ctypes_copy(bvm *vm) {
    be_classof(vm, 1);
    // stack: 1/self + class_object
    be_call(vm, 0);     // call empty constructor to build empty resizable copy
    // stack: 1/ self + new_empty_instance

    // source object (self)
    be_getmember(vm, 1, ".p");
    const void* src_buf = be_tocomptr(vm, -1);
    be_pop(vm, 1);

    be_getmember(vm, 1, ".len");
    int32_t src_len = be_toint(vm, -1);
    be_pop(vm, 1);

    // dest object
    be_getmember(vm, -1, ".p");
    const void* dst_buf = be_tocomptr(vm, -1);
    be_pop(vm, 1);

    be_getmember(vm, -1, ".len");
    int32_t dst_len = be_toint(vm, -1);
    be_pop(vm, 1);

    if (src_len != dst_len)  {
        be_raisef(vm, "internal_error", "new object has wrong size %i (should be %i)", dst_len, src_len);
    }

    // copy bytes
    memmove((void*)dst_buf, src_buf, src_len);

    be_return(vm);
}

// get an attribute from a ctypes structure
// arg1: ctypes instance
// arg2: name of the argument
// The class has a `_def` static class attribute with the C low-level mapping definition
int be_ctypes_member(bvm *vm) {
    be_getmember(vm, 1, "_def");
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
            // int result at top of stack
        } else if (ctypes_float == member->type) {
            // Note: double not supported (no need identified)
            // get raw int32_t
            be_getmember(vm, 1, "geti");   // self.get or self.geti
            be_pushvalue(vm, 1);        // push self
            be_pushint(vm, member->offset_bytes);
            be_pushint(vm, 4);          // size is 4 bytes
            be_call(vm, 3);
            be_pop(vm, 3);
            // get int and convert to float
            int32_t val = be_toint(vm, -1);
            be_pop(vm, 1);
            float *fval = (float*) &val;    // type wizardry
            be_pushreal(vm, *fval);
        } else if (ctypes_ptr32 == member->type) {
            be_getmember(vm, 1, "geti");   // self.get or self.geti
            be_pushvalue(vm, 1);        // push self
            be_pushint(vm, member->offset_bytes);
            be_pushint(vm, 4);          // size is 4 bytes TODO 32 bits only supported here
            be_call(vm, 3);
            be_pop(vm, 3);
            // convert to ptr
            int32_t val = be_toint(vm, -1);
            be_pop(vm, 1);
            be_pushcomptr(vm, (void*) val);
        } else {
            // general int support
            int size = member->type;       // eventually 1/2/4, positive if little endian, negative if big endian
            int sign = bfalse;            // signed int
            if (size >= ctypes_i8) {
                size -= ctypes_i8 - 1;
                sign = btrue;
            }
            if (size <= ctypes_be_i8) {
                size += ctypes_be_i8 - 1;
                sign = btrue;
            }
            // get
            be_getmember(vm, 1, sign ? "geti" : "get");   // self.get or self.geti
            be_pushvalue(vm, 1);        // push self
            be_pushint(vm, member->offset_bytes);
            be_pushint(vm, size);
            be_call(vm, 3);
            be_pop(vm, 3);
            // int result at top of stack
        }
        // the int result is at top of the stack
        // check if we need an instance mapping
        if (member->mapping > 0 && definitions->instance_mapping) {
            const char * mapping_name = definitions->instance_mapping[member->mapping - 1];
            if (mapping_name) {
                be_getglobal(vm, mapping_name);     // stack: class
                be_pushvalue(vm, -2);               // stack: class, value
                be_pushint(vm, -1);                 // stack; class, value, -1
                be_call(vm, 2);                     // call constructor with 2 parameters
                be_pop(vm, 2);                      // leave new instance on top of stack
            }
        }
        be_return(vm);
    }
    be_module_load(vm, be_newstr(vm, "undefined"));
    be_return(vm);
}

// setmember takes 3 arguments:
// 1: self (subclass of bytes())
// 2: name of member
// 3: value
int be_ctypes_setmember(bvm *vm) {
    // If the value is an instance, we call 'toint()' and replace the value
    if (be_isinstance(vm, 3)) {

        be_getmember(vm, 3, "toint");
        if (!be_isnil(vm, -1)) {
            be_pushvalue(vm, 3);
            be_call(vm, 1);
            be_pop(vm, 1);
            be_moveto(vm, -1, 3);
        } else {
            be_raise(vm, "value_error", "Value is an instance without 'toint()' method");
        }
        be_pop(vm, 1);
    }

    // If the value is a pointer, replace with an int of same value (works only on 32 bits CPU)
    if (be_iscomptr(vm, 3) || be_isstring(vm, 3)) {
        void* v;
        if (be_iscomptr(vm, 3)) {
            v = be_tocomptr(vm, 3);
        } else {
            v = be_tostring(vm, 3);
        }         
        be_pushint(vm, (int32_t) v);
        be_moveto(vm, -1, 3);
        be_pop(vm, 1);
    }

    be_getmember(vm, 1, "_def");
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
        } else if (ctypes_float == member->type) {
            // Note: double not supported (no need identified)
            float val = be_toreal(vm, 3);
            int32_t *ival = (int32_t*) &val;
            // set
            be_getmember(vm, 1, "seti");
            be_pushvalue(vm, 1);        // push self
            be_pushint(vm, member->offset_bytes);
            be_pushint(vm, *ival);
            be_pushint(vm, 4);      // size is 4 bytes
            be_call(vm, 4);
            be_pop(vm, 5);
            be_return_nil(vm);
        } else if (ctypes_ptr32 == member->type) {
            // Note: 64 bits pointer not supported
            int32_t ptr;
            if (be_iscomptr(vm, 3)) {
                ptr = (int32_t) be_tocomptr(vm, 3);
            } else {
                ptr = be_toint(vm, 3);
            }
            // set
            be_getmember(vm, 1, "seti");
            be_pushvalue(vm, 1);        // push self
            be_pushint(vm, member->offset_bytes);
            be_pushint(vm, ptr);
            be_pushint(vm, 4);      // size is 4 bytes - 64 bits not suppported
            be_call(vm, 4);
            be_pop(vm, 5);
            be_return_nil(vm);
        } else {
            // general int support
            int size = member->type;       // eventually 1/2/4, positive if little endian, negative if big endian
            int sign = bfalse;            // signed int
            if (size >= ctypes_i8) {
                size -= ctypes_i8 - 1;
                sign = btrue;
            }
            if (size <= ctypes_be_i8) {
                size += ctypes_be_i8 - 1;
                sign = btrue;
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
    } else {
        be_raisef(vm, "attribute_error", "class '%s' cannot assign to attribute '%s'",
                be_classname(vm, 1), be_tostring(vm, 2));
    }
}

//
// tomap, create a map instance containing all values decoded
//
int be_ctypes_tomap(bvm *vm) {
    // don't need argc
    be_getmember(vm, 1, "_def");
    const be_ctypes_structure_t *definitions;
    definitions = (const be_ctypes_structure_t *) be_tocomptr(vm, -1);
    be_pop(vm, 1);

    // create empty map
    be_newobject(vm, "map");

    for (uint32_t i = 0; i < definitions->size_elt; i++) {
        const be_ctypes_structure_item_t * item = &definitions->items[i];

        be_pushstring(vm, item->name);     // stack: map - key

        be_getmember(vm, 1, "member");
        be_pushvalue(vm, 1);
        be_pushstring(vm, item->name);
        be_call(vm, 2);
        be_pop(vm, 2);      // stack: map - key - value

        be_data_insert(vm, -3);
        be_pop(vm, 2);     // stack: map
    }

    be_pop(vm, 1);  // remove map struct, to leave map instance
    be_return(vm);
}

//
// Constructor for ctypes_dyn structure
//
// Arg1 is instance self
// Arg2 is int or comptr (and not null): create a mapped bytes buffer to read/write at a specific location
// Arg3 is int or comptr (and not null): the binary definition of the struct (dynamic and not fixed as static member)
int be_ctypes_dyn_init(bvm *vm) {
    int argc = be_top(vm);
    void * src_data = NULL;
    const be_ctypes_structure_t * definitions = NULL;
    if (argc > 2 && (be_isint(vm, 2) || be_iscomptr(vm, 2)) && (be_isint(vm, 3) || be_iscomptr(vm, 3))) {
        if (be_iscomptr(vm, 2)) {
            src_data = be_tocomptr(vm, 2);
        } else {
            src_data = (void*) be_toint(vm, 2);
        }
        if (be_iscomptr(vm, 3)) {
            definitions = (const be_ctypes_structure_t *) be_tocomptr(vm, 3);
        } else {
            definitions = (const be_ctypes_structure_t *) be_toint(vm, 3);
        }
    }
    if (!src_data || !definitions) {
        be_raise(vm, "value_error", "'address' and 'definition' cannot be null");
    }

    // store definition in member variable
    be_pushcomptr(vm, (void*) definitions);
    be_setmember(vm, 1, "_def");        // static class comptr
    be_pop(vm, 1);

    // call bytes.init(self)
    be_getbuiltin(vm, "bytes");     // shortcut `ctypes` init and call directly bytes.init()
    be_getmember(vm, -1, "init");
    be_pushvalue(vm, 1);
    be_pushcomptr(vm, src_data);
    be_pushint(vm, -definitions->size_bytes); // negative size signals a fixed size
    be_call(vm, 3);      // call with 2 or 3 arguments depending on provided address
    be_pop(vm, 4);
    // super(self, bytes) still on top of stack

    be_pop(vm, 1);

    be_return(vm);
}

BE_EXPORT_VARIABLE extern const bclass be_class_bytes;

#include "be_fixed_be_class_ctypes_bytes.h"
#include "be_fixed_be_class_ctypes_bytes_dyn.h"

/* @const_object_info_begin
class be_class_ctypes_bytes (scope: global, name: ctypes_bytes, super: be_class_bytes) {
    _def, nil()
    copy, func(be_ctypes_copy)
    init, func(be_ctypes_init)
    member, func(be_ctypes_member)
    setmember, func(be_ctypes_setmember)

    tomap, func(be_ctypes_tomap)
}
@const_object_info_end */

/* @const_object_info_begin
class be_class_ctypes_bytes_dyn (scope: global, name: ctypes_bytes_dyn, super: be_class_ctypes_bytes) {
    _def, var
    init, func(be_ctypes_dyn_init)
}
@const_object_info_end */
