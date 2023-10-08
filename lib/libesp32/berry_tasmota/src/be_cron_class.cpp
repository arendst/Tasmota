/********************************************************************
 * Light_state class - abstract light state
 * 
 * Handles all states and events for a virtual light.
 * Can be eventually subclassed to handle a physical light.
 * 
 *******************************************************************/

#include "be_constobj.h"
#include "be_mapping.h"

#include "ccronexpr.h"

extern uint32_t LocalTime(void);
static const uint32_t START_VALID_TIME = 1451602800;  // Time is synced and after 2016-01-01

// create
static cron_expr* ccronexpr_init(struct bvm* vm, char* expr) {
  cron_expr* cron = new cron_expr();
  const char* error = nullptr;
  cron_parse_expr(expr, cron, &error);

  if (error) {
    be_raise(vm, "value_error", error);
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
// Returns the next trigger time, or 0 if rtc is invalid
static uint32_t ccronexpr_next(cron_expr* cron) {
  uint32_t now_local = LocalTime();
  return now_local > START_VALID_TIME ? cron_next(cron, now_local) : 0;
}
BE_FUNC_CTYPE_DECLARE(ccronexpr_next, "i", ".")

// time_reached
// Compares as uint32_t (Berry only handles int32_t) to avoid the 2038 bug
// Also prevents from triggering an event if the clock is not set (i.e. year is 1970)
static bool ccronexpr_time_reached(uint32_t date) {
  uint32_t now_local = LocalTime();
  bool reached = (date <= now_local);
  return now_local > START_VALID_TIME ? reached : false;
}
BE_FUNC_CTYPE_DECLARE(ccronexpr_time_reached, "b", "i")

// now (local time)
static uint32_t ccronexpr_now(void) {
  return LocalTime();
}
BE_FUNC_CTYPE_DECLARE(ccronexpr_now, "i", "")

#include "be_fixed_be_class_ccronexpr.h"

/* @const_object_info_begin

class be_class_ccronexpr (scope: global, name: ccronexpr) {
  .p, var       // pointer to cron_expr*

  init, ctype_func(ccronexpr_init)
  // deinit, ctype_func(ccronexpr_deinit)

  next, ctype_func(ccronexpr_next)
  time_reached, static_ctype_func(ccronexpr_time_reached)
  now, static_ctype_func(ccronexpr_now)
}
@const_object_info_end */
