#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <assert.h>
#include <fdict/clib.h>
#include <fdict/libtime.h>
#include <fdict/libdsout.h>
#include <fdict/wordimage.h>
#include <fdict/trie.h>
#include <fdict/datrie.h>
#include <fdict/datrieevent.h>

struct datrie_s* create_datrie(struct wordimage_s* wordimage, struct datrieevent_s* event,
			       int scantype, int debug)
{
  struct datrie_s* datrie;
  int encodesize;
  datrie = (struct datrie_s*)DATMALLOC(sizeof(struct datrie_s));
  if (datrie == NULL)
    {
      DATMEMOUT;
      DATMEMOUT_EXIT(NULL);
    }

  datrie->debug = debug;
  datrie->wordimage = wordimage;
  datrie->event = event;
  getencodesize(wordimage, encodesize);
  datrie->encodesize = encodesize;
  datrie->size = encodesize;
  datrie->scantype = scantype;
  datrie->lastk = 1;
  datrie->array = (struct array_s*)DATMALLOC(sizeof(struct array_s) * encodesize);
  datrie->udcount = 0;
  if (!datrie->array)
    {
      DATMEMOUT;
      DATMEMOUT_EXIT(NULL);
    }
  memset(datrie->array, 0, sizeof(struct array_s) * encodesize);
  return datrie;
}


void clear_datrie(struct datrie_s* datrie)
{  
  if (datrie != NULL)
    {
      if (datrie->array != NULL)
        {
          DATFREE(datrie->array);
          datrie->array = NULL;
        }
      
      datrie->wordimage = NULL;
      datrie->event = NULL;
      DATFREE(datrie);
    }
}

static void realloc_array(struct datrie_s *datrie)
{
  struct array_s *narray;
  int allocsize;
  int new_alloc_size;
  
  assert(datrie != NULL);
  assert(datrie->array != NULL);

  new_alloc_size = (datrie->size + datrie->encodesize * 10) * 2;

  /* new alloc array */
  allocsize = sizeof(struct array_s) * new_alloc_size;
  if (datrie->debug)
    printf("Realloc Array: %dbyte\n", allocsize);
  narray = (struct array_s*)DATMALLOC(allocsize);
  if (!narray)
    {
      DATMEMOUT;
      DATMEMOUT_EXIT(NULL);
    }
  memset(narray, 0, allocsize);
  memcpy(narray, datrie->array, sizeof(struct array_s) * datrie->size);
  DATFREE(datrie->array);
  datrie->array = narray;

  /* new size */
  datrie->size = new_alloc_size;
}

static int check_empty_slot(struct datrie_s* datrie, int k, struct trie_state_s* state)
{  
  while (state != NULL)
    {
      /* realloc memory */
      if (k + state->state >= datrie->size)
        realloc_array(datrie);
      assert(k+state->state < datrie->size);

      /*
       * base[k+s1] = base[k+s2] = base[k+sn] = 0
       * check[k+s1]=check[k+s2]=check[k+sn] = 0
       */
      if (datrie->array[k + state->state].base != 0 ||
          datrie->array[k + state->state].check != 0)
        return 0;
      state = state->brerstate;
    }
  return 1;
}

static void set_state_array(struct datrie_s* datrie, struct trie_state_s* state, int index, int k)
{
  while (state != NULL)
    {
      /* check[k+s1]=check[k+s2]=check[k+sn] = i */
      datrie->array[k + state->state].check = index;
      state->index = k + state->state;
      state = state->brerstate;
    }
}

static void build_state(struct datrie_s* datrie, struct trie_state_s* state)
{
  struct trie_state_s* substate;
  int index;
  int k;
  
  assert(datrie != NULL);
  assert(state != NULL);

  /* scan type */
  /* full scan */
  if (datrie->scantype == 1)
    k = 1;
  /* increment scan */
  else if (datrie->scantype == 2)
    k = datrie->lastk;
  /* default scan */
  else 
    k = 1;
  
  if (datrie->event != NULL && datrie->event->build_state_event != NULL)
    datrie->event->build_state_event(datrie, state);

  substate = state->nextstate;
  index = state->index;

  while (1)
    {
      if (check_empty_slot(datrie, k, substate) == 1)
        {
          /* base[i] = k */
          datrie->array[index].base = k;
          set_state_array(datrie, substate, index, k);
          break;
        }
      k++;
    }
  /* increment scan , save k */
  if (datrie->scantype == 2)
    datrie->lastk = k;

  /* end state */
  if (state->endstate == 1)
    {
      assert(datrie->array[index].base > 0);
      datrie->array[index].base *= -1;
      datrie->array[index].dataids = state->dataid;
      ++datrie->udcount;
    }
}

static void truncate(struct datrie_s *datrie)
{
  int i = datrie->size - 1;
  for (; i >= 0; i--) {
    if (datrie->array[i].base != 0)
      break;
  }
  if (datrie->debug) {
    printf("Alloc Array Size: %lubyte\n", datrie->size * sizeof(int));
    printf("Real Array Size: %lubyte\n", (i + 1) * sizeof(int));
  }
  datrie->size = i + 1;
}

void build_state_list(struct datrie_s* datrie, struct trie_state_s* state_list)
{ 
  if (state_list == NULL)
    return ;

  while (state_list != NULL)
    {
      build_state(datrie, state_list);
      state_list = state_list->sortnext;
    }
  truncate(datrie);
}

int dat_find_states(struct datrie_s* datrie, struct tstate_s* tstate)
{
  int* states;
  int statecount;
  int c, i, s, t = 0;
  int m = 0;
  
  if (datrie == NULL || tstate == NULL)
    return 0;

  states = tstate->states;
  statecount = tstate->statecount;
  tstate->dataid = 0;
  s = 1;
  for (i = 0; i < statecount; i++)
    {
      getwordcode(c, datrie->wordimage, states[i]);
      t = abs(datrie->array[s].base) + c;
      if (datrie->array[t].check != s)
          return 0;
      s = t;
    }
  if (datrie->array[t].base < 0)
    {
      m = 1;
      tstate->dataid = datrie->array[t].dataids;
    }
  return m+1;
}

int dat_find_state(struct datrie_s* datrie, struct stateslot_s* stateslot, int state)
{
  int s;
  int t;
  int c;
  
  if (datrie == NULL || stateslot == NULL)
    return 0;
  
  s = stateslot->s;
  stateslot->dataid = 0;
  getwordcode(c, datrie->wordimage, state);

  t = abs(datrie->array[s].base) + c;
  if (datrie->array[t].check != s)
    {
      stateslot->s = 1;
      return 0;
    }
  stateslot->s = t;
  if (datrie->array[t].base < 0)
    {
      stateslot->dataid = datrie->array[t].dataids;
      return 2;
    }
  return 1;
}

int datrie_out_text(struct datrie_s* datrie, const char* out_file_name)
{
  DSOUT_DEFINE;
  int i;
  
  if (datrie == NULL)
    return 0;
  
  DSOUT_OPEN_OUTFILE(out_file_name);

  DSOUT_INFO("Datrie:\n");
  sprintf(DSOUT_BUFF, "\tArray Size: %d\n", datrie->size);
  DSOUT_INFO(DSOUT_BUFF);

  /* out base array */
  DSOUT_INFO("\tBase : [");
  for (i = 0; i < datrie->size; i++)
    {
      sprintf(DSOUT_BUFF, "%d", datrie->array[i].base);
      DSOUT_INFO(DSOUT_BUFF);
      if (i + 1 < datrie->size)
        {
          DSOUT_INFO(", ");
        }
    }
  DSOUT_INFO("]\n");
  
  /* out check array */
  DSOUT_INFO("\tCheck: [");
  for (i = 0; i < datrie->size; i++)
    {
      sprintf(DSOUT_BUFF, "%d", datrie->array[i].check);
      DSOUT_INFO(DSOUT_BUFF);
      if (i + 1 < datrie->size)
        {    
          DSOUT_INFO(", ");
        }
      
    }
  DSOUT_INFO("]\n");

  DSOUT_CLOSE_OUTFILE;
  return 1;
}
