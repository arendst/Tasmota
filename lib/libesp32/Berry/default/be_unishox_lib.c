/********************************************************************
 * Berry module `unishox`
 * 
 * To use: `import unishox`
 * 
 * Allows to respond to HTTP request
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_UNISHOX_COMPRESSION

extern int be_unishox_compress(bvm *vm);
extern int be_unishox_decompress(bvm *vm);

/********************************************************************
** Solidified module: unishox
********************************************************************/
be_local_module(unishox,
    "unishox",
    be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_nested_key("decompress", -1407935646, 10, -1), be_const_func(be_unishox_decompress) },
        { be_nested_key("compress", -1476883059, 8, -1), be_const_func(be_unishox_compress) },
    }))
);
BE_EXPORT_VARIABLE be_define_const_native_module(unishox);

#endif // USE_UNISHOX_COMPRESSION
