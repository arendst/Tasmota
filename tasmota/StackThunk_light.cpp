/*
  StackThunk_light.c - Allow use second stack for BearSSL calls
  Light version with reduced Stack size due to Tasmota optimizations.

  BearSSL uses a significant amount of stack space, much larger than
  the default Arduino core stack. These routines handle swapping
  between a secondary, user-allocated stack on the heap and the real
  stack.

  Copyright (c) 2017 Earle F. Philhower, III. All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
  Modified 8 May 2015 by Hristo Gochkov (proper post and file upload handling)
*/

#include "my_user_config.h"

#include <stdint.h>
#include <stdlib.h>
#include "StackThunk_light.h"
#include <ets_sys.h>

extern "C" {

uint32_t *stack_thunk_light_ptr = NULL;
uint32_t *stack_thunk_light_top = NULL;
uint32_t *stack_thunk_light_save = NULL;  /* Saved A1 while in BearSSL */
uint32_t stack_thunk_light_refcnt = 0;

//#define _stackSize (5600/4)
#ifdef USE_MQTT_TLS_FORCE_EC_CIPHER
  #define _stackSize (5300/4)   // using a light version of bearssl we can save 300 bytes
#else
  #define _stackSize (3600/4)   // using a light version of bearssl we can save 2k
#endif
#define _stackPaint 0xdeadbeef

/* Add a reference, and allocate the stack if necessary */
void stack_thunk_light_add_ref()
{
  stack_thunk_light_refcnt++;
  if (stack_thunk_light_refcnt == 1) {
    stack_thunk_light_ptr = (uint32_t *)malloc(_stackSize * sizeof(uint32_t));
    stack_thunk_light_top = stack_thunk_light_ptr + _stackSize - 1;
    stack_thunk_light_save = NULL;
    stack_thunk_light_repaint();
  }
}

/* Drop a reference, and free stack if no more in use */
void stack_thunk_light_del_ref()
{
  if (stack_thunk_light_refcnt == 0) {
    /* Error! */
    return;
  }
  stack_thunk_light_refcnt--;
  if (!stack_thunk_light_refcnt) {
    free(stack_thunk_light_ptr);
    stack_thunk_light_ptr = NULL;
    stack_thunk_light_top = NULL;
    stack_thunk_light_save = NULL;
  }
}

void stack_thunk_light_repaint()
{
  if (stack_thunk_light_ptr) {
    for (int i=0; i < _stackSize; i++) {
      stack_thunk_light_ptr[i] = _stackPaint;
    }
  }
}

/* Simple accessor functions used by postmortem */
uint32_t stack_thunk_light_get_refcnt() {
  return stack_thunk_light_refcnt;
}

uint32_t stack_thunk_light_get_stack_top() {
  return (uint32_t)stack_thunk_light_top;
}

uint32_t stack_thunk_light_get_stack_bot() {
  return (uint32_t)stack_thunk_light_ptr;
}

uint32_t stack_thunk_light_get_cont_sp() {
  return (uint32_t)stack_thunk_light_save;
}

/* Return the number of bytes ever used since the stack was created */
uint32_t stack_thunk_light_get_max_usage()
{
  uint32_t cnt = 0;

  /* No stack == no usage by definition! */
  if (!stack_thunk_light_ptr) {
    return 0;
  }

  for (cnt=0; (cnt < _stackSize) && (stack_thunk_light_ptr[cnt] == _stackPaint); cnt++) {
    /* Noop, all work done in for() */
  }
  return 4 * (_stackSize - cnt);
}

/* Print the stack from the first used 16-byte chunk to the top, decodable by the exception decoder */
void stack_thunk_light_dump_stack()
{
  uint32_t *pos = stack_thunk_light_top;
  while (pos < stack_thunk_light_ptr) {
    if ((pos[0] != _stackPaint) || (pos[1] != _stackPaint) || (pos[2] != _stackPaint) || (pos[3] != _stackPaint))
      break;
    pos += 4;
  }
  ets_printf(">>>stack>>>\n");
  while (pos < stack_thunk_light_ptr) {
    ets_printf("%08x:  %08x %08x %08x %08x\n", (int32_t)pos, pos[0], pos[1], pos[2], pos[3]);
    pos += 4;
  }
  ets_printf("<<<stack<<<\n");
}

/* Called when the stack overflow is detected by a thunk.  Main memory is corrupted at this point.  Do not return. */
void stack_thunk_light_fatal_overflow()
{
    ets_printf("FATAL ERROR: BSSL stack overflow\n");
    abort();
}

};
