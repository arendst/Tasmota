/********************************************************************
 * Berry module `webserver`
 * 
 * To use: `import webserver`
 * 
 * Allows to respond to HTTP request
 *******************************************************************/
#include "be_constobj.h"

extern int m_md5_init(bvm *vm);
extern int m_md5_update(bvm *vm);
extern int m_md5_finish(bvm *vm);

#if BE_USE_PRECOMPILED_OBJECT
#include "../generate/be_fixed_be_class_md5.h"
#endif

void be_load_md5_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
    static const bnfuncinfo members[] = {
        { ".p", NULL },

        { "init", m_md5_init },
        { "update", m_md5_update },
        { "finish", m_md5_finish },

        { NULL, NULL }
    };
    be_regclass(vm, "MD5", members);
#else
    be_pushntvclass(vm, &be_class_md5);
    be_setglobal(vm, "MD5");
    be_pop(vm, 1);
#endif
}
/* @const_object_info_begin

class be_class_md5 (scope: global, name: MD5) {
    .p, var

    init, func(m_md5_init)
    update, func(m_md5_update)
    finish, func(m_md5_finish)
}
@const_object_info_end */
