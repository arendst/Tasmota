

#ifndef __BE_MAPPING__
#define __BE_MAPPING__

#ifdef __cplusplus
extern "C" {
#endif

#include "berry.h"

// include this header to force compilation fo this module
#define BE_MAX_CB       20      // max number of callbacks, each callback requires a distinct address

/*********************************************************************************************\
 * Support for Berry int constants
 * as virtual members
 \*********************************************************************************************/

typedef intptr_t (*fn_any_callable)(intptr_t p0, intptr_t p1, intptr_t p2, intptr_t p3,
                                    intptr_t p4, intptr_t p5, intptr_t p6, intptr_t p7);

typedef struct be_const_member_t {
    const char * name;
    int        value;
} be_const_member_t;

// table of functions per class
typedef struct be_ntv_func_def_t {
    const char * name;
    void * func;
    const char * return_type;
    const char * arg_type;
} be_ntv_func_def_t;

struct bclass;
// list of classes and function tables
typedef struct be_ntv_class_def_t {
    const char * name;
    const struct bclass * cl;
    const be_ntv_func_def_t * func_table;
    size_t size;
} be_ntv_class_def_t;

void be_raisef(bvm *vm, const char *except, const char *msg, ...);

extern void be_map_insert_int(bvm *vm, const char *key, bint value);
extern void be_map_insert_bool(bvm *vm, const char *key, bbool value);
extern void be_map_insert_real(bvm *vm, const char *key, breal value);
extern void be_map_insert_str(bvm *vm, const char *key, const char *value);
extern void be_map_insert_list_uint8(bvm *vm, const char *key, const uint8_t *value, size_t size);

extern int be_map_bin_search(const char * needle, const void * table, size_t elt_size, size_t total_elements);

extern void be_create_class_wrapper(bvm *vm, const char * class_name, void * ptr);
extern int be_find_global_or_module_member(bvm *vm, const char * cl_name);

extern bbool be_const_member(bvm *vm, const be_const_member_t * definitions, size_t def_len);
extern intptr_t be_convert_single_elt(bvm *vm, int idx, const char * arg_type, int *len);
extern int be_check_arg_type(bvm *vm, int arg_start, int argc, const char * arg_type, intptr_t p[8]);
extern int be_call_c_func(bvm *vm, const void * func, const char * return_type, const char * arg_type);
extern int be_call_ctype_func(bvm *vm, const void *definition);     /* handler for Berry vm */

#ifdef __cplusplus
}
#endif
#endif // __BE_MAPPING__
