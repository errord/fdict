/*
 *
 * clib -- libdatrie
 * trie tree
 *
 */
#ifndef _CLIB_LIBDATRIE_TRIE_H_
#define _CLIB_LIBDATRIE_TRIE_H_

#define TRIEMEMOUT CLIBMEMOUT
#define TRIEMEMOUT_EXIT(ret) CLIBMEMOUT
/* define memory allocate */
#define TRIEMALLOC CLIBMALLOC
#define TRIEFREE CLIBFREE


/*
 * userdata save user info, for example freq 
 */
struct userdata_s
{
  int POS; /* part of speech */ 
  int freq; /* word frequency */
  int ud[5]; /* other user data */
};

#define CLEAR_UD(a) a.POS = 0;                  \
  a.freq = 0;                                   \
  a.ud[0] = 0;                                  \
  a.ud[1] = 0;                                  \
  a.ud[2] = 0;                                  \
  a.ud[3] = 0;                                  \
  a.ud[4] = 0

#define CLEAR_UD_P(a) a->POS = 0;               \
  a->freq = 0;                                  \
  a->ud[0] = 0;                                 \
  a->ud[1] = 0;                                 \
  a->ud[2] = 0;                                 \
  a->ud[3] = 0;                                 \
  a->ud[4] = 0

#define COPY_UD(a,b) a.POS = b.POS;             \
  a.freq = b.freq;                              \
  a.ud[0] = b.ud[0];                            \
  a.ud[1] = b.ud[1];                            \
  a.ud[2] = b.ud[2];                            \
  a.ud[3] = b.ud[3];                            \
  a.ud[4] = b.ud[4]

#define COPY_UD_P_(a,b) a->POS = b.POS;           \
  a->freq = b.freq;                              \
  a->ud[0] = b.ud[0];                            \
  a->ud[1] = b.ud[1];                            \
  a->ud[2] = b.ud[2];                            \
  a->ud[3] = b.ud[3];                            \
  a->ud[4] = b.ud[4]

#define COPY_UD__P(a,b) a.POS = b->POS;          \
  a.freq = b->freq;                              \
  a.ud[0] = b->ud[0];                            \
  a.ud[1] = b->ud[1];                            \
  a.ud[2] = b->ud[2];                            \
  a.ud[3] = b->ud[3];                            \
  a.ud[4] = b->ud[4]


/*
 *                [s]
 *                /   
 *             [s1| ------> |s2| ----------> |s3]        
 *               /            \                \
 *            [s4] --> [s5]   [s6] --> [s7]   [s8]
 *             |         |     |
 *            ...       ...   ...
 *
 *  state point nextstate set and point brerstate
*/
struct trie_state_s
{
  int state;
  int index; /* f(s) = i */
  int nscount; /* next state count */
  int endstate; /* is end state, true=1 false=0 */
  struct userdata_s* userdata; /* user data */
  struct trie_state_s* brerstate; /* brer state, for example current state is s1, brerstate is s2 */
  struct trie_state_s* nextstate; /* next state, for example current state is s1, nextstate is s4 */
  struct trie_state_s* listnext; /* list next node, use at sort list */
  struct trie_state_s* sortnext; /* sort list next */
};

struct trie_s
{
  int statecount; /* all state count */
  int maxstatecount; /* max state count */
  int udcount; /* user data count */
  struct wordimage_s* wordimage;
  struct trie_state_s* startstate; /* point trie tree */
  struct trie_state_s* sortlisthead; /* point sort list head */
};

struct tstate_s
{
  int statecount;
  int* states;
  struct userdata_s userdata;
};

/*
 * create trie
 */
struct trie_s* create_trie(struct wordimage_s* wordimage);

/*
 * clear trie
 */
void clear_trie(struct trie_s* trie);

/*
 * states insert trie
 * return 0 if error, return 1 at success
 */
int add_states(struct trie_s* trie, struct tstate_s* tstate);

/*
 * find states from trie
 * return 0 if no finded, return 1 is finded
 */
int find_states(struct trie_s* trie, struct tstate_s* tstate);

/*
 * get user data count
 */
int get_userdata_count(struct trie_s* trie);

/*
 * get sort state list
 */
struct trie_state_s* get_sort_state_list(struct trie_s* trie);

/*
 * out trie to file or terminal, content is text
 * print to stdout if out_file_name is null
 * 0 if error, other success
 */
int trie_out_text(struct trie_s* trie, const char* out_file_name);

/*
 * out wordimage to file or terminal, content is text
 * print to stdout if out_file_name is null
 * 0 if error, other success
 */
int wordimage_out_text(struct trie_s* trie, const char* out_file_name);

#endif /* _CLIB_LIBDATRIE_TRIE_H_ */

