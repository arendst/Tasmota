/*
  be_class_Matrix.c – Berry binding for Matrix driver

  This file declares the Berry–C interface for the Matrix class.
  It exposes the native functions implemented in xdrv_52_3_berry_matrix.ino
  to the Berry scripting environment, and maps them into the global
  `Matrix` class with its associated methods and internal storage.

  Variables:
    .p     – holds the native MatrixCore* pointer
    _buf   – reference to an external bytes buffer (prevents GC)

  Methods:
    init, deinit, get, set, clear, flip, blit, scroll

  Copyright (C) 2025
  Released under the GNU General Public License v3
*/

#ifdef USE_WS2812
#include "be_constobj.h"
#include "be_mapping.h"

extern int be_matrix_init(bvm *vm);
extern int be_matrix_deinit(bvm *vm);
extern int be_matrix_get(bvm *vm);
extern int be_matrix_set(bvm *vm);
extern int be_matrix_blit(bvm *vm);
extern int be_matrix_scroll(bvm *vm);
extern int be_matrix_clear(bvm* vm);

/* @const_object_info_begin
class be_class_Matrix (scope: global, name: Matrix, strings: weak) {
  .p, var
  _buf, var

  init,    func(be_matrix_init)
  deinit,  func(be_matrix_deinit)
  get,     func(be_matrix_get)
  set,     func(be_matrix_set)
  blit,    func(be_matrix_blit)
  scroll,  func(be_matrix_scroll)
  clear,  func(be_matrix_clear)
}
@const_object_info_end */

#include "be_fixed_be_class_Matrix.h"

#endif // USE_WS2812
