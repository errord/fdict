/*
 * clib
 *
*/
#ifndef _CLIB_H_
#define _CLIB_H_


#define CLIBMEMOUT printf("memory out\n")
#define CLIBMEMOUT_EXIT(ret) exit(0)
/* define memory allocate */
#ifdef BENCHMARK
#include "memstate.h"
#define CLIBMALLOC bm_malloc
#define CLIBFREE bm_free
#define CLIBREALLOC bm_realloc
#define CLIBCALLOC bm_calloc
#else
#define CLIBMALLOC malloc
#define CLIBFREE free
#define CILBREALLOC realloc
#define CLIBCALLOC calloc
#endif /* BENCHMARK */
#endif /* _CLIB_H_ */

