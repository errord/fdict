/*
 *
 * clib -- libdatrie -- wordimage
 *
 */
#ifndef _CLIB_LIBDATRIE_WORDIMAGE_H_
#define _CLIB_LIBDATRIE_WORDIMAGE_H_

#define WIMEMOUT CLIBMEMOUT
#define WIMEMOUT_EXIT(ret) CLIBMEMOUT
/* define memory allocate */
#define WIMALLOC CLIBMALLOC
#define WIFREE CLIBFREE


struct wordimage_s
{
  int size; /* wordimage size */
  int id; /* increment id */
  int* wordimage; /* wordimage table */
};

/*
 * create wordimage table
 * malloc size to wordimage table
 * return struct wordimage_s, NULL if error
 */
struct wordimage_s* create_wordimage(int size);

/*
 * clear wordimage table
 */
void clear_wordimage(struct wordimage_s* wdimg);

/*
 * add new word to wordimage table
 */
#define addword(wdimg, word) assert(wdimg != NULL && word >= 0 && word < wdimg->size); \
  if (wdimg->wordimage[word] == 0) {                                    \
    wdimg->id++;                                                        \
    wdimg->wordimage[word] = wdimg->id;                                 \
  }

/*
 * get word code from wordimage table
 * return value to r
 * 0 if word not exist
 */
#define getwordcode(r, wdimg, word) assert(wdimg != NULL && word >= 0 && word < wdimg->size);   \
  r = wdimg->wordimage[word]

/*
 * get encode size
 */
#define getencodesize(wdimg, r) assert(wdimg != NULL);    \
  r = wdimg->id

#endif /* _CLIB_LIBDATRIE_WORDIMAGE_H_ */

