#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fdict/base_type.h>
#include <fdict/wordbase.h>
#include <fdict/util.h>
#include <fdict/libfdict.h>

/*
 *  Datrie Binary Dictionary File Format
 *
 *           13*sizeof(char)
 *  --------------------------
 *  | Magic("DATRIEBINDICT") |
 *  --------------------------
 *        1*sizeof(char)
 *  -------------------------
 *  | Version(0.0 -- 15.15) |
 *  -------------------------
 *             int                 int       Wsize*int
 *  -----------------------------------------------------
 *  |  Wsize(wordimage size)  |     Id     |  wordimage |
 *  -----------------------------------------------------
 *
 *         int
 *  ------------------
 *  | As(array size) |
 *  ------------------
 *       As*int    As*int      As*int
 *  -----------------------------------------
 *  |    Base     |   Check     |  Dataids  |
 *  -----------------------------------------
*/

int savedatrie_bindict(struct datrietree_s* datrietree, const char* dictname)
{
  FILE *fd;
  char version;
  int wsize;
  int id;
  int as;
  int r;
  
  if (datrietree == NULL || dictname == NULL)
    return 0;
  
  fd = fopen(dictname, "wb");
  if (fd == NULL)
    return 0;
  
  /* Magic */
  r = fwrite(DATRIE_MAGIC, sizeof(char), strlen(DATRIE_MAGIC), fd);
  /* Version */
  version = DATRIE_VERSION_1_0;
  r = fwrite(&version, sizeof(char), 1, fd);
  /* Wsize */
  wsize = datrietree->wordimage->size;
  r = fwrite(&wsize, sizeof(int), 1, fd);
  /* Id */
  id = datrietree->wordimage->id;
  r = fwrite(&id, sizeof(int), 1, fd);
  /* wordimage */
  r = fwrite(datrietree->wordimage->wordimage, sizeof(int), wsize, fd);
  /* As */
  as = datrietree->datrie->size;
  r = fwrite(&as, sizeof(int), 1, fd);
  /* Array */
  r = fwrite(datrietree->datrie->array, sizeof(struct array_s), as, fd);
  fclose(fd);
  return 1;
}

struct datrietree_s* loaddatrie_bindict(const char* dictname)
{
  FILE* fd = NULL;
  int magiclen;
  char version;
  char* buff[1024];
  struct datrietree_s* datrietree = NULL;
  struct wordimage_s* wdimg = NULL;
  struct datrie_s* datrie = NULL;
  int as;
  int r;
  
  if (dictname == NULL)
    return NULL;
  
  fd = fopen(dictname, "rb");
  if (fd == NULL)
    return 0;
 
  magiclen = strlen(DATRIE_MAGIC);
  r = fread(buff, sizeof(char), magiclen, fd);
  buff[magiclen] = '\0';

  /* Magic */
  if (strncmp(DATRIE_MAGIC, (const char*)buff, magiclen) != 0)
    goto failed;
   
  /* Version */
  r = fread(&version, sizeof(char), 1, fd);
  if (version != DATRIE_VERSION_1_0)
    goto failed;
  
  datrietree = (struct datrietree_s*)LDMALLOC(sizeof(struct datrietree_s));
  if (datrietree == NULL)
    {
      LDMEMOUT;
      LDMEMOUT_EXIT(NULL);
    }
  wdimg = (struct wordimage_s*)WIMALLOC(sizeof(struct wordimage_s));
  if (wdimg == NULL)
    goto failed;
  wdimg->wordimage = NULL;
  
  /* Wsize */
  r = fread(&(wdimg->size), sizeof(int), 1, fd);
  /* Id */
  r = fread(&(wdimg->id), sizeof(int), 1, fd);
  /* wordimage */
  wdimg->wordimage = (int*)WIMALLOC(sizeof(int) * wdimg->size);
  r = fread(wdimg->wordimage, sizeof(int), wdimg->size, fd);
  datrie = (struct datrie_s*)DATMALLOC(sizeof(struct datrie_s));
  if (datrie == NULL)
    {
      DATMEMOUT;
      DATMEMOUT_EXIT(NULL);
    }
  
  datrie->wordimage = wdimg;
  datrie->encodesize = 0;
  datrie->scantype = 0;
  datrie->lastk = 0;
  
  datrie->array = NULL;

  /* As */
  r = fread(&as, sizeof(int), 1, fd);
  datrie->size = as;

  /* Array */
  datrie->array = (struct array_s*)DATMALLOC(sizeof(struct array_s) * as);
  memset(datrie->array, 0, sizeof(struct array_s) * as);
  r = fread(datrie->array, sizeof(struct array_s), as, fd);

  datrietree->wordimage = wdimg;
  datrietree->trie = NULL;
  datrietree->datrie = datrie;

  goto success;

 failed:
  if (datrietree != NULL)
    {
      LDFREE(datrietree);
      datrietree = NULL;
    }
  if (wdimg != NULL)
    {
      if (wdimg->wordimage != NULL)
        {
          WIFREE(wdimg->wordimage);
          wdimg->wordimage = NULL;
        }
      WIFREE(wdimg);
      wdimg = NULL;
    }
  if (datrie != NULL)
    {
      if (datrie->array != NULL)
        {
          DATFREE(datrie->array);
          datrie->array = NULL;
        }
      DATFREE(datrie);
      datrie = NULL;
    }
  
 success:
  fclose(fd);
  return datrietree;
}
