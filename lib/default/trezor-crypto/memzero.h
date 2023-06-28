#ifndef __MEMZERO_H__
#define __MEMZERO_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

void memzero(void* const pnt, const size_t len);
    
#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif

