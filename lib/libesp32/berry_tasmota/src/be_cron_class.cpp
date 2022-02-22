/********************************************************************
 * Light_state class - abstract light state
 * 
 * Handles all states and events for a virtual light.
 * Can be eventually subclassed to handle a physical light.
 * 
 *******************************************************************/
#ifdef USE_LIGHT

#include "be_constobj.h"
#include "be_mapping.h"

#include "ccronexpr.h"

// create
static cron_expr* ccronexpr_init(struct bvm* vm, char* expr) {
  cron_expr* cron = new cron_expr();
  const char* error = nullptr;
  cron_parse_expr(expr, cron, &error);

  if (error) {
    be_raise(vm, "value_error", error);   // TODO any way to pass VM?
  }
  return cron;
}
BE_FUNC_CTYPE_DECLARE(ccronexpr_init, "+.p", "@s")

// deinit
static void ccronexpr_deinit(cron_expr* cron) {
  delete cron;
}
BE_FUNC_CTYPE_DECLARE(ccronexpr_deinit, "", ".")


// next
static uint32_t ccronexpr_next(cron_expr* cron, uint32_t date) {
  return cron_next(cron, date);
}
BE_FUNC_CTYPE_DECLARE(ccronexpr_next, "i", ".i")

// prev
static uint32_t ccronexpr_prev(cron_expr* cron, uint32_t date) {
  return cron_prev(cron, date);
}
BE_FUNC_CTYPE_DECLARE(ccronexpr_prev, "i", ".i")

#include "be_fixed_be_class_ccronexpr.h"

extern "C" void be_load_ccronexpr_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_ccronexpr);
    be_setglobal(vm, "ccronexpr");
    be_pop(vm, 1);
}
/* @const_object_info_begin

class be_class_ccronexpr (scope: global, name: ccronexpr) {
  .p, var       // pointer to cron_expr*

  init, ctype_func(ccronexpr_init)
  deinit, ctype_func(ccronexpr_init)

  next, ctype_func(ccronexpr_next)
}
@const_object_info_end */

#endif // USE_LIGHT
