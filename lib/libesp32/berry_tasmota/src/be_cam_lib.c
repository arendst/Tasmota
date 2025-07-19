/********************************************************************
 * Tasmota lib
 * 
 * To use: `import cam`
 * 
 *******************************************************************/

#ifdef USE_BERRY_CAM

// #include "be_constobj.h"
#include "be_mapping.h"

extern int be_cam_init(bvm *vm);
extern int be_cam_setup(bvm *vm);
extern int be_cam_get_image(bvm *vm);
extern int be_cam_info(bvm *vm);

#include "be_fixed_cam.h"

/* @const_object_info_begin

module cam (scope: global) {

    init, func(be_cam_init)
    setup, func(be_cam_setup)
    get_image, func(be_cam_get_image)
    info, func(be_cam_info)
}
@const_object_info_end */

#endif // USE_BERRY_CAM
