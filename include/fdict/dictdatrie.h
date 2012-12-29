/*
 * Copyright 2012 error.d
 * by error.d@gmail.com
 *
 * FDict Project
 *
 * dict datrie
 *
 */
#ifndef _FDICT_LIBDATRIE_DICTDATRIE_H_
#define _FDICT_LIBDATRIE_DICTDATRIE_H_

#define DDMEMOUT CLIBMEMOUT
#define DDMEMOUT_EXIT(ret) CLIBMEMOUT
/* define memory allocate */
#define DDMALLOC CLIBMALLOC
#define DDFREE CLIBFREE


/* Magic */
#define DATRIE_MAGIC (const char*)"FDICT_INDEX__DATRIE"

/* Version */
#define DATRIE_VERSION_1_0 MAKE_VERSION(1,0)

/*
 * save datrie to binary dictionary
 * datrie struct
 * dictionary file name
 * return 0 save failed
 * return 1 save success
 */
int savedatrie_bindict(struct datrietree_s* datrietree, const char* dictname);

/*
 * load binary dictionary to datrie
 */
struct datrietree_s* loaddatrie_bindict(const char* dictname);

#endif /* _FDICT_LIBDATRIE_DICTDATRIE_H_ */
