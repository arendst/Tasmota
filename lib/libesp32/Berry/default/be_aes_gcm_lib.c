/********************************************************************
 * Berry module `webserver`
 * 
 * To use: `import webserver`
 * 
 * Allows to respond to HTTP request
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_ALEXA_AVS

extern int m_aes_gcm_init(bvm *vm);
extern int m_aes_gcm_encryt(bvm *vm);
extern int m_aes_gcm_decryt(bvm *vm);
extern int m_aes_gcm_tag(bvm *vm);

#if BE_USE_PRECOMPILED_OBJECT
#include "../generate/be_fixed_be_class_aes_gcm.h"
#endif

void be_load_aes_gcm_lib(bvm *vm) {
    // insert the class GCM in module AES
    be_newmodule(vm);
    be_setname(vm, -1, "AES");
    be_setglobal(vm, "AES");

    be_pushntvclass(vm, &be_class_aes_gcm);
    be_setmember(vm, -2, "GCM");
    be_pop(vm, 2);
}
/* @const_object_info_begin

class be_class_aes_gcm (scope: global, name: GCM) {
    .p1, var
    .p2, var

    init, func(m_aes_gcm_init)
    encrypt, func(m_aes_gcm_encryt)
    decrypt, func(m_aes_gcm_decryt)
    tag, func(m_aes_gcm_tag)
}
@const_object_info_end */

#endif // USE_ALEXA_AVS
