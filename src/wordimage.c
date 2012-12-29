#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <assert.h>
#include <fdict/clib.h>
#include <fdict/wordimage.h>

struct wordimage_s* create_wordimage(int size)
{
  struct wordimage_s* wdimg = NULL;
  
  wdimg = (struct wordimage_s*)WIMALLOC(sizeof(struct wordimage_s));
  if (wdimg == NULL)
    return NULL;
  
  wdimg->size = size;
  wdimg->id = 0;
  wdimg->wordimage = (int*)WIMALLOC(sizeof(int) * size);
  memset(wdimg->wordimage, 0, sizeof(int)*size);
  return wdimg;
}

void clear_wordimage(struct wordimage_s* wdimg)
{
  if (wdimg != NULL)
    {
      free(wdimg->wordimage);
      wdimg->size = 0;
      wdimg->id = 0;
      WIFREE(wdimg);
    }
}

