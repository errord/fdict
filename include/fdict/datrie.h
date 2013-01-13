/*
 *
 * clib -- libdatrie
 * double array trie
 *
 */
#ifndef _CLIB_LIBDATRIE_DATRIE_H_
#define _CLIB_LIBDATRIE_DATRIE_H_

#define DATMEMOUT CLIBMEMOUT
#define DATMEMOUT_EXIT(ret) CLIBMEMOUT
/* define memory allocate */
#define DATMALLOC CLIBMALLOC
#define DATFREE CLIBFREE

struct array_s 
{  
  int base;
  int check;
  unsigned int dataids;  /* user data index, start at 1 */
};

struct datrie_s
{
  int debug;
  int size; /* base and check and userindex size */
  int encodesize; /* state encode size */
  int scantype; /* scan memory type, 1 full scan, 2 increment scan */
  int lastk; /* k start search pos */
  int udcount; /* user data use count */
  struct array_s *array; /* base and check array */
  struct wordimage_s* wordimage;
  struct datrieevent_s* event;
};

/*
 * find state slot
 */
struct stateslot_s
{
  int s;
  int dataid;
};

/*
 * create datrie
 */
struct datrie_s* create_datrie(struct wordimage_s* wordimage, struct datrieevent_s* event,
			       int scantype, int debug);

/*
 * clear datrie
 */
void clear_datrie(struct datrie_s* datrie);

/*
 * build state list, result to base and check
 */
void build_state_list(struct datrie_s* datrie, struct trie_state_s* state_list);

/*
 * find state from datrie
 * 0 no match
 * 1 match
 * 2 match and stop state
 */
int dat_find_states(struct datrie_s* datrie, struct tstate_s* tstate);

/*
 * find state from datrie
 * 0 no match
 * 1 match
 * 2 match and stop state
 */
int dat_find_state(struct datrie_s* datrie, struct stateslot_s* stateslot, int state);

/*
 * out datrie to file or terminal, content is text
 * print to stdout if out_file_name is null
 * 0 if error, other success
 */
int datrie_out_text(struct datrie_s* datrie, const char* out_file_name);

#endif /* _CLIB_LIBDATRIE_DATRIE_H_ */
