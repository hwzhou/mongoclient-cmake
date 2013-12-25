//-----------------------------------------------------------------------------
// MurmurHash3 was written by Austin Appleby, and is placed in the public
// domain. The author hereby disclaims copyright to this source code.

#ifndef _MURMURHASH3_H_
#define _MURMURHASH3_H_

#include <stdint.h>

#include "mongo/client/export_macros.h"

#ifdef __cplusplus
extern "C"
{
#endif

    void MONGO_CLIENT_API MurmurHash3_x86_32  ( const void * key, int len, uint32_t seed, void * out );

    void MONGO_CLIENT_API MurmurHash3_x86_128 ( const void * key, int len, uint32_t seed, void * out );

    void MONGO_CLIENT_API MurmurHash3_x64_128 ( const void * key, int len, uint32_t seed, void * out );

#ifdef __cplusplus
}  /* end extern "C" */
#endif

#endif // _MURMURHASH3_H_
