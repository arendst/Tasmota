/*
  be_class_pixmat.c – Berry binding for pixmat driver

  This file declares the Berry–C interface for the pixmat class.
  It exposes the native functions implemented in xdrv_52_3_berry_pixmat.ino
  to the Berry scripting environment, and maps them into the global
  `pixmat` class with its associated methods and internal storage.

  Variables:
    .p     – holds the native PixmatCore* pointer
    _buf   – reference to an external bytes buffer (prevents GC)

  Methods:
    init, deinit, get, set, clear, flip, blit, scroll

  Copyright (C) 2025
  Released under the GNU General Public License v3
*/

#ifdef USE_WS2812
#include "be_constobj.h"
#include "be_mapping.h"

extern int be_pixmat_init(bvm *vm);
extern int be_pixmat_deinit(bvm *vm);
extern int be_pixmat_get(bvm *vm);
extern int be_pixmat_set(bvm *vm);
extern int be_pixmat_blit(bvm *vm);
extern int be_pixmat_scroll(bvm *vm);
extern int be_pixmat_clear(bvm* vm);

/* @const_object_info_begin
class be_class_pixmat (scope: global, name: pixmat, strings: weak) {
  .p, var
  _buf, var

  init,    func(be_pixmat_init)
  deinit,  func(be_pixmat_deinit)
  get,     func(be_pixmat_get)
  set,     func(be_pixmat_set)
  blit,    func(be_pixmat_blit)
  scroll,  func(be_pixmat_scroll)
  clear,  func(be_pixmat_clear)
}
@const_object_info_end */

#include "be_fixed_be_class_pixmat.h"

#endif // USE_WS2812
