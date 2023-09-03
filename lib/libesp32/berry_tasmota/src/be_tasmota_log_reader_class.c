/********************************************************************
 * Tasmota Log Reader class
 * 
 *******************************************************************/
#include "be_constobj.h"
#include "be_mapping.h"

extern uint32_t* tlr_init(void);                            BE_FUNC_CTYPE_DECLARE(tlr_init, "+_p", "")
extern char* tlr_get_log(uint32_t* idx, int32_t log_level); BE_FUNC_CTYPE_DECLARE(tlr_get_log, "$", ".i")

#include "be_fixed_be_class_tasmota_log_reader.h"

/* @const_object_info_begin

class be_class_tasmota_log_reader (scope: global, name: tasmota_log_reader) {
    _p, var
    init, ctype_func(tlr_init)
    get_log, ctype_func(tlr_get_log)
}
@const_object_info_end */
