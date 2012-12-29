/*
 *
 * libfdict
 *
 */
#ifndef _CLIB_LIBFDICT_H_
#define _CLIB_LIBFDICT_H_
#include "clib.h"
#include "wordimage.h"
#include "trie.h"
#include "datrie.h"
#include "check.h"
#include "datrieevent.h"

#define LDMEMOUT CLIBMEMOUT
#define LDMEMOUT_EXIT(ret) CLIBMEMOUT
/* define memory allocate */
#define LDMALLOC CLIBMALLOC
#define LDFREE CLIBFREE

struct datrietree_s
{
  struct wordimage_s* wordimage;
  struct trie_s* trie;
  struct datrie_s* datrie;
  struct datrieevent_s* event;
};

struct datrietree_s* makeDatrieTree(int encodesize, struct datrieevent_s* event);

void clearDatrieTree(struct datrietree_s* datrie);

void buildDatrie(struct datrietree_s* datrie, int scantype, int debug);

void addWord(struct datrietree_s* datrie, const char* word, struct userdata_s* userdata, enum word_encode ecode);

/*
 * find Word
 * 0 no match
 * 1 match
 * 2 match and stop state
 */
int findWord(struct datrietree_s* datrie, const char* word, struct userdata_s* userdata, enum word_encode ecode);

void dump_datrie(struct datrietree_s* datrie, const char *datafile);

#include "dictdatrie.h"

#endif /* _CLIB_LIBFDICT_H_ */
