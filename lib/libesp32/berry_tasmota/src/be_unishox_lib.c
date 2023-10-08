/********************************************************************
 * Berry module `unishox`
 * 
 * To use: `import unishox`
 * 
 * Allows to respond to HTTP request
 *******************************************************************/

#ifdef USE_UNISHOX_COMPRESSION

#include "be_constobj.h"

extern int be_ntv_unishox_compress(bvm *vm);
extern int be_ntv_unishox_decompress(bvm *vm);

/* @const_object_info_begin
module unishox (scope: global) {
    decompress, func(be_ntv_unishox_decompress)
    compress, func(be_ntv_unishox_compress)
}
@const_object_info_end */
#include "be_fixed_unishox.h"

#endif // USE_UNISHOX_COMPRESSION
