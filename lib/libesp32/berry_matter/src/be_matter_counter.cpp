/*
  be_matter_counter.cpp - implements Matter counters for message replay prevention

  Copyright (C) 2023  Stephan Hadinger & Theo Arends

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/********************************************************************
 * Matter_Counter - Support for 32 bits counter with history window
 * 
 * This is used in Matter to detect collision and replay of packets
 * See section "4.5. Message Counters"
 *******************************************************************/

#include <bitset>
#include "be_constobj.h"
#include "be_mapping.h"
#include "be_mem.h"

#include "esp_random.h"

#define MESSAGE_COUNTER_WINDOW_SIZE   32

typedef struct {
  uint32_t counter;     // value of the counter
  std::bitset<MESSAGE_COUNTER_WINDOW_SIZE> window;
} matter_counter_t;

/*
  * Initialize a local message counter with random value between [1, 2^28]. This increases the difficulty of traffic analysis
  * attacks by making it harder to determine how long a particular session has been open. The initial counter is always 1 or
  * higher to guarantee first message is always greater than initial peer counter set to 0.
*/

static void mc_randomize(matter_counter_t *c) {
  // pick a random one in range 0..2^28
  esp_fill_random(&c->counter, sizeof(c->counter));
  c->counter = c->counter & 0x0FFFFFFF;         // keep only 28 bits
  if (c->counter == 0)  { c->counter = 1; }     // unfortunate event that the random generates `0`
  c->window.reset();
}

// `Matter_Counter([counter:int]) -> instance(Matter_Counter)`
//
// Creates a new monotonic counter for sending or receiving
// If value is `0` or absent, a random counter is generated in the range 1..2^28
static void* mc_init(bvm *vm, int32_t val) {
  matter_counter_t *c = (matter_counter_t*)be_malloc(vm, sizeof(matter_counter_t));
  c->counter = val;
  c->window.reset();    // reset the window
  return c;
}
BE_FUNC_CTYPE_DECLARE(mc_init, "+_p", "@[i]")

static void mc_deinit(bvm *vm, matter_counter_t *c) {
  be_free(vm, c, sizeof(matter_counter_t));
}
BE_FUNC_CTYPE_DECLARE(mc_deinit, "", "@.")

// do a unisgned int32 comparison
bbool mc_is_greater(uint32_t a, uint32_t b) {
  return a > b;
}
BE_FUNC_CTYPE_DECLARE(mc_is_greater, "b", "ii")

static void mc_reset(matter_counter_t *c, int32_t val) {
  c->counter = val;
  c->window.reset();
}
BE_FUNC_CTYPE_DECLARE(mc_reset, "", ".[i]")


int32_t mc_val(matter_counter_t *c) {
  return c->counter;
}
BE_FUNC_CTYPE_DECLARE(mc_val, "i", ".")

int32_t mc_next(matter_counter_t *c) {
  if (c->counter == 0) { mc_randomize(c); }
  c->counter++;
  c->window <<= 1;
  if (c->counter == 0) {
    c->counter++;
    c->window <<= 1;
  }
  return c->counter;
}
BE_FUNC_CTYPE_DECLARE(mc_next, "i", ".")

// validate if the value is acceptable
//
// strict_mode is used for encrypted messages: roll-over is not permitted
//  otherwise roll-over is permitted and counter in the past
//  (outside of window) is allowed
//
// return `true` if ok, `false` if rejected
int32_t mc_validate(matter_counter_t *c, uint32_t new_val, bbool strict_mode) {
  if (new_val == c->counter) { return bfalse; }    // simple case of a collision of the last message

  if (c->counter == 0) {          // first message received, accept any value
    c->counter = new_val;
    c->window.reset();
    return btrue;
  }

  if (new_val > c->counter) {   // counter is in the future, same handling for both modes
    if (new_val - c->counter > MESSAGE_COUNTER_WINDOW_SIZE) {
      c->window.reset();    // no index is in the window anymore, clear all
    } else {
      c->window <<= new_val - c->counter;
      c->window[new_val - c->counter - 1] = true;   // mark previous val
      // adjust window
    }
    c->counter = new_val;
    return btrue;
  } else {  // here: new_val < c->counter
    // are we in the window?
    uint32_t gap = c->counter - new_val;
    if (gap <= 32) {
      if (c->window[gap-1]) {
        return false;         // reject because already seen
      } else {
        c->window[gap-1] = btrue;
        return true;
      }
    } else {
      // in the past and out of the window (we ignore roll-over to simplify)
      if (strict_mode) {
        return bfalse;
      } else {
        c->counter = new_val;
        c->window.reset();
        return btrue;
      }
    }
  }
}
BE_FUNC_CTYPE_DECLARE(mc_validate, "b", ".ib")


static int mc_tostring(bvm *vm) {
  be_getmember(vm, 1, "_p");
  matter_counter_t *c = (matter_counter_t*) be_tocomptr(vm, -1);
  if (c->counter != 0) {
    be_pushstring(vm, "[");

    for (uint32_t i = MESSAGE_COUNTER_WINDOW_SIZE; i > 0; i--) {
      be_pushint(vm, c->window[i-1]);
      be_toescape(vm, -1, 'x'); /* escape string */
      be_strconcat(vm, -2);
      be_pop(vm, 1);
    }
      
    be_pushstring(vm, "]");
    be_strconcat(vm, -2);
    be_pop(vm, 1);

    be_pushint(vm, c->counter);
    be_toescape(vm, -1, 'x'); /* escape string */
    be_strconcat(vm, -2);
    be_pop(vm, 1);
  } else {
    be_pushstring(vm, "[]-");
  }

  be_return(vm);
}

#include "be_fixed_be_class_Matter_Counter.h"

/* @const_object_info_begin
class be_class_Matter_Counter (scope: global, name: Matter_Counter, strings: weak) {
  _p, var
  init, ctype_func(mc_init)
  deinit, ctype_func(mc_deinit)
  reset, ctype_func(mc_reset)
  tostring, func(mc_tostring)

  val, ctype_func(mc_val)
  next, ctype_func(mc_next)
  validate, ctype_func(mc_validate)

  is_greater, static_ctype_func(mc_is_greater)    // compare two numbers as unsigned 32 bits
}
@const_object_info_end */


/*

# Unit tests
import matter
var c = matter.Counter(100)
assert(str(c) == '[00000000000000000000000000000000]100')
assert(c.val() == 100)
assert(c.next() == 101)
assert(c.val() == 101)
assert(str(c) == '[00000000000000000000000000000000]101')

c.reset(101)
assert(!c.validate(101, true))
assert(str(c) == '[00000000000000000000000000000000]101')

c.reset(101)
assert(c.validate(100, true))
assert(str(c) == '[00000000000000000000000000000001]101')
assert(!c.validate(100, true))
assert(str(c) == '[00000000000000000000000000000001]101')

c.reset(101)
assert(c.validate(100, false))
assert(str(c) == '[00000000000000000000000000000001]101')
assert(!c.validate(100, false))
assert(str(c) == '[00000000000000000000000000000001]101')


c.reset(128)
assert(c.validate(96, true))
assert(str(c) == '[10000000000000000000000000000000]128')
assert(!c.validate(95, true))
assert(str(c) == '[10000000000000000000000000000000]128')

c.reset(128)
assert(c.validate(96, false))
assert(str(c) == '[10000000000000000000000000000000]128')
assert(c.validate(95, false))
assert(str(c) == '[00000000000000000000000000000000]95')


c.reset(100)
# set the context as in the documentation
assert(c.validate(99, true))
assert(c.validate(96, true))
assert(c.validate(70, true))
assert(str(c) == '[00100000000000000000000000001001]100')

assert(c.validate(97, true))
assert(str(c) == '[00100000000000000000000000001101]100')

assert(!c.validate(100, true))
assert(!c.validate(99, true))
assert(!c.validate(97, true))
assert(!c.validate(96, true))
assert(!c.validate(70, true))
assert(str(c) == '[00100000000000000000000000001101]100')   # unchanged

assert(c.validate(101, true))
assert(str(c) == '[01000000000000000000000000011011]101')

assert(c.validate(103, true))
assert(str(c) == '[00000000000000000000000001101110]103')

assert(!c.validate(70, true))

assert(c.validate(200, true))
assert(str(c) == '[00000000000000000000000000000000]200')

### counters for outgoing messages

c.reset()
assert(c.val() == 0)

*/