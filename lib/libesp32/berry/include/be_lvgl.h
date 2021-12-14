/********************************************************************
 * Tasmota LVGL Headers
 *******************************************************************/
#ifndef __BE_LVGL_H__
#define __BE_LVGL_H__

#include "be_constobj.h"

#ifdef USE_LVGL

#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif
    // table of functions per class
    typedef struct lvbe_call_c_t {
        const char * name;
        void * func;
        const char * return_type;
        const char * arg_type;
    } lvbe_call_c_t;

    // list of classes and function tables
    typedef struct lvbe_call_c_classes_t {
        const char * name;
        const bclass * cl;
        const lvbe_call_c_t * func_table;
        size_t size;
    } lvbe_call_c_classes_t;
    extern const lvbe_call_c_classes_t lv_classes[];
    extern const size_t lv_classes_size;

#ifdef __cplusplus
}
#endif

#endif // USE_LVGL

#endif // __BE_LVGL_H__