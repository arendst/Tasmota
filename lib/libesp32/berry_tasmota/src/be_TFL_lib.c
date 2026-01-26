/********************************************************************
 * Tasmota lib
 * 
 * To use: import TFL`
 *******************************************************************/
#include "be_constobj.h"
#include "be_mapping.h"

#ifdef USE_BERRY_TF_LITE


extern const char* be_TFL_log(struct bvm *vm);
BE_FUNC_CTYPE_DECLARE(be_TFL_log, "s", "@");

extern const char* be_TFL_stats(struct bvm *vm);
BE_FUNC_CTYPE_DECLARE(be_TFL_stats, "s", "@");

extern bbool be_TFL_begin(struct bvm *vm, const char* type, const uint8_t *descriptor, size_t size);
BE_FUNC_CTYPE_DECLARE(be_TFL_begin, "b", "@s[(bytes)~]");

extern bbool be_TFL_load(struct bvm *vm, const uint8_t *model_buf, size_t model_size, const uint8_t *output_buf, size_t output_size,int arena);
BE_FUNC_CTYPE_DECLARE(be_TFL_load, "b", "@(bytes)~(bytes)~[i]");

extern bbool be_TFL_input(struct bvm *vm, const uint8_t *buf, size_t size, bbool quantize_to_int8);
BE_FUNC_CTYPE_DECLARE(be_TFL_input, "b", "@(bytes)~[b]");

extern bbool be_TFL_output(struct bvm *vm, const uint8_t *buf, size_t size);
BE_FUNC_CTYPE_DECLARE(be_TFL_output, "b", "@(bytes)~");

#include "be_fixed_TFL.h"

/* @const_object_info_begin
module TFL (scope: global) {
  begin,        ctype_func(be_TFL_begin)
  load,         ctype_func(be_TFL_load)
  input,        ctype_func(be_TFL_input)
  output,       ctype_func(be_TFL_output)
  log,          ctype_func(be_TFL_log)
  stats,        ctype_func(be_TFL_stats)
}
@const_object_info_end */

#endif // USE_BERRY_TF_LITE
