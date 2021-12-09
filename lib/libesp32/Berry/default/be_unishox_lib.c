/********************************************************************
 * Berry module `unishox`
 * 
 * To use: `import unishox`
 * 
 * Allows to respond to HTTP request
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_UNISHOX_COMPRESSION

extern int be_ntv_unishox_decompress(bvm *vm);
extern int be_ntv_unishox_compress(bvm *vm);

/********************************************************************
** Solidified module: unishox
********************************************************************/
be_local_module(unishox,
    "unishox",
    be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(decompress, -1), be_const_func(be_ntv_unishox_decompress) },
        { be_const_key(compress, -1), be_const_func(be_ntv_unishox_compress) },
    }))
);
BE_EXPORT_VARIABLE be_define_const_native_module(unishox);

#endif // USE_UNISHOX_COMPRESSION
