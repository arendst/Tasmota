/*********************************************************************************************\
 * Extended version of be_raise()
\*********************************************************************************************/

#include "be_mapping.h"
#include "be_exec.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

// variant of be_raise with string format
void be_raisef(bvm *vm, const char *except, const char *msg, ...) {
  // To save stack space support logging for max text length of 128 characters
  char log_data[128];

  va_list arg;
  va_start(arg, msg);
  uint32_t len = vsnprintf(log_data, sizeof(log_data)-3, msg, arg);
  va_end(arg);
  if (len+3 > sizeof(log_data)) { strcat(log_data, "..."); }  // Actual data is more
  be_raise(vm, except, log_data);
}
