#ifndef __MACROS_H__
#define __MACROS_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define MEMSET_BZERO(p,l)	memset((p), 0, (l))

#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif
