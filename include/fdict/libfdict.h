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

void addWord(struct datrietree_s* datrie, const char* word, unsigned int dataid, enum word_encode ecode);

/*
 * find Word
 * 0 no match
 * 1 match
 * 2 match and stop state
 */
int findWord(struct datrietree_s* datrie, const char* word, unsigned int *dataid, enum word_encode ecode, int debug);

/*
 * find Word by string
 * 0 no match
 * 1 match
 * 2 match and stop state
 */
#define findWordByString(r, datrie, word, dataid, ecode, debug) {	\
    r = dat_find_string(datrie->datrie, encode, word, dataid);		\
    if (*dataid > 0)							\
      *dataid = *dataid - 1;						\
  }



void dump_datrie(struct datrietree_s* datrie, const char *datafile);

#include "dictdatrie.h"

#endif /* _CLIB_LIBFDICT_H_ */
