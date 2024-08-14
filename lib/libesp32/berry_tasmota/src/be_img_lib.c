/********************************************************************
 * Berry class `img`
 * 
 *******************************************************************/

#include "be_constobj.h"

#ifdef USE_BERRY_IMAGE

extern int be_img_init(bvm *vm);
extern int be_img_deinit(bvm *vm);
extern int be_img_from_jpg(bvm *vm);
extern int be_img_from_buffer(bvm *vm);
extern int be_img_get_buffer(bvm *vm);
extern int be_img_convert_to(bvm *vm);
extern int be_img_info(bvm *vm);

/* @const_object_info_begin
class be_class_img (scope: global, name: img, strings: weak) {
  .p, var

  GRAYSCALE, int(3)
  RGB565, int(0)
  RGB888, int(5)
  JPEG, int(4)

  init, func(be_img_init)
  deinit, func(be_img_deinit)
  from_jpg, func(be_img_from_jpg)
  from_buffer, func(be_img_from_buffer)
  get_buffer, func(be_img_get_buffer)
  convert_to, func(be_img_convert_to)
  info, func(be_img_info)
}
@const_object_info_end */

#include "be_fixed_be_class_img.h"

#endif // USE_BERRY_IMAGE
