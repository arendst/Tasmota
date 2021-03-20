/********************************************************************
 * Tasmota lib
 * 
 * To use: `import tasmota`
 *******************************************************************/
#include "be_object.h"

extern int l_getFreeHeap(bvm *vm);
extern int l_publish(bvm *vm);
extern int l_cmd(bvm *vm);
extern int l_getoption(bvm *vm);
extern int l_millis(bvm *vm);
extern int l_timereached(bvm *vm);
extern int l_yield(bvm *vm);
extern int l_delay(bvm *vm);
extern int l_scaleuint(bvm *vm);

extern int l_respCmnd(bvm *vm);
extern int l_respCmndStr(bvm *vm);
extern int l_respCmndDone(bvm *vm);
extern int l_respCmndError(bvm *vm);
extern int l_respCmndFailed(bvm *vm);
extern int l_resolveCmnd(bvm *vm);

extern int l_getlight(bvm *vm);
extern int l_getpower(bvm *vm);
extern int l_setlight(bvm *vm);
extern int l_setpower(bvm *vm);

// #if !BE_USE_PRECOMPILED_OBJECT
#if 1           // TODO we will do pre-compiled later
// Class definition
void be_load_tasmota_ntvlib(bvm *vm)
{
    static const bnfuncinfo members[] = {
        { "_op", NULL },
        { "_opf", NULL },
        { "_operators", NULL },
        { "_rules", NULL },
        { "_timers", NULL },
        { "_cmd", NULL },
        { "getfreeheap", l_getFreeHeap },
        { "publish", l_publish },
        { "cmd", l_cmd },
        { "getoption", l_getoption },
        { "millis", l_millis },
        { "timereached", l_timereached },
        { "yield", l_yield },
        { "delay", l_delay },
        { "scaleuint", l_scaleuint },

        { "respcmnd", l_respCmnd },
        { "respcmndstr", l_respCmndStr },
        { "respcmnd_done", l_respCmndDone },
        { "respcmnd_error", l_respCmndError },
        { "respcmnd_failed", l_respCmndFailed },
        { "resolvecmnd", l_resolveCmnd },

        { "getlight", l_getlight },
        { "getpower", l_getpower },
        { "setlight", l_setlight },
        { "setpower", l_setpower },
        
        { NULL, NULL }
    };
    be_regclass(vm, "Tasmota_ntv", members);
}

#else
/* @const_object_info_begin
module tasmota (scope: global, depend: 1) {
    getfreeheap, func(l_getFreeHeap)
}
@const_object_info_end */
#include "../generate/be_fixed_tasmota.h"
#endif
