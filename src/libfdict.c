#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <fdict/base_type.h>
#include <fdict/memory.h>
#include <fdict/libtime.h>
#include <fdict/wordbase.h>
#include <fdict/libfdict.h>
#include <fdict/utf.h>

struct datrietree_s* makeDatrieTree(int encodesize, struct datrieevent_s* event)
{
  struct datrietree_s* datrie;
  
  fdmalloc(datrie, struct datrietree_s*, sizeof(struct datrietree_s));
  if (datrie == NULL)
    {
      LDMEMOUT;
      LDMEMOUT_EXIT(NULL);
    }
  datrie->event = event;
  datrie->wordimage = create_wordimage(encodesize);
  if (datrie->wordimage == NULL)
    {
      LDMEMOUT;
      LDMEMOUT_EXIT(NULL);
    }
  datrie->trie = create_trie(datrie->wordimage);
  if (datrie->trie == NULL)
    {
      LDMEMOUT;
      LDMEMOUT_EXIT(NULL);
    }
  datrie->datrie = NULL;
  
  return datrie;
}

void clearDatrieTree(struct datrietree_s* datrie)
{
  if (datrie != NULL)
    {
      if (datrie->datrie != NULL)
        {
          clear_datrie(datrie->datrie);
          datrie->datrie = NULL;
        }
      if (datrie->trie != NULL)
        {
          clear_trie(datrie->trie);
          datrie->trie = NULL;
        }
      if (datrie->wordimage != NULL)
        {
          clear_wordimage(datrie->wordimage);
          datrie->wordimage = NULL;
        }
      datrie->event = NULL;
      fdfree(datrie);
      datrie = NULL;
    }  
}

void buildDatrie(struct datrietree_s* datrie, int scantype, int debug)
{
  struct trie_state_s* state_list;
  
  assert(datrie->datrie == NULL);

  datrie->datrie = create_datrie(datrie->wordimage, datrie->event,
				 scantype, debug);
  if (datrie->datrie == NULL)
    {
      LDMEMOUT;
      LDMEMOUT_EXIT(NULL);
    }
  state_list = get_sort_state_list(datrie->trie);
  build_state_list(datrie->datrie, state_list);
}

static int string_len(const char* string, enum word_encode encode)
{
  if (encode == gbk)
    return strlen(string);
  else if (encode == utf8 || encode == utf8_short)
    return utf8len(string);
  return 0;
}

static int string_to_wordcode(const char* word, int wlength, int* words, enum word_encode encode)
{
  if (encode == gbk)
    return wordcodebygbk(word, wlength, words);
  else if (encode == utf8)
    return wordcodebyutf8(word, wlength, words);
  else if (encode == utf8_short)
    return wordcodebyutf8short(word, wlength, words);
  return 0;
}

void addWord(struct datrietree_s* datrie, const char* word, unsigned int dataid, enum word_encode encode)
{
  struct tstate_s tstate;
  int* words;
  int wlen;
  int wcount;
  
  assert(datrie != NULL);
  assert(word != NULL);
  
  wlen = string_len(word, encode);
  fdmalloc(words, int*, sizeof(int) * wlen);
  if (words == NULL)
    {
      LDMEMOUT;
      LDMEMOUT_EXIT(NULL);
    }
  
  wcount = string_to_wordcode(word, wlen, words, encode);
  tstate.states = words;
  tstate.statecount = wcount;
  tstate.dataid = dataid + 1;
  add_states(datrie->trie, &tstate);
  fdfree(words);
}

int findWord(struct datrietree_s* datrie, const char* word, unsigned int *dataid, enum word_encode encode, int debug)
{
  struct tstate_s tstate;
  int wlen;
  int wcount;
  int r;
  int words[1024];
  
  assert(datrie != NULL);
  assert(word != NULL);

  wlen = string_len(word, encode);
  assert(wlen < 1024);
  wcount = string_to_wordcode(word, wlen, words, encode);

  tstate.states = words;
  tstate.statecount = wcount;
  tstate.dataid = 0;

  r = dat_find_states(datrie->datrie, &tstate);

  if (tstate.dataid > 0)
    *dataid = tstate.dataid - 1;
  else
    *dataid = tstate.dataid;
  return r;
}

int findWordByString(struct datrietree_s* datrie,
		     const char* word, 
		     unsigned int *dataid, 
		     enum word_encode encode, 
		     int debug)
{
  int r;
  
  assert(datrie != NULL);
  assert(word != NULL);

  r = dat_find_string(datrie->datrie, encode, word, dataid);

  if (*dataid > 0)
    *dataid = *dataid - 1;
  return r;
}

void dump_datrie(struct datrietree_s* datrie, const char *datafile)
{
  char trie_dump_file[MAX_PATH];
  char wordimage_dump_file[MAX_PATH];
  char datrie_dump_file[MAX_PATH];

  sprintf(trie_dump_file, "%s.trie.dump", datafile);
  sprintf(wordimage_dump_file, "%s.wordimage.dump", datafile);
  sprintf(datrie_dump_file, "%s.datrie.dump", datafile);
  
  trie_out_text(datrie->trie, trie_dump_file);
  wordimage_out_text(datrie->trie, wordimage_dump_file);
  datrie_out_text(datrie->datrie, datrie_dump_file);
}
