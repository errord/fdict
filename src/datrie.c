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

struct datrie_s* create_datrie(struct wordimage_s* wordimage, struct datrieevent_s* event, int udsize, int scantype, int debug)
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
  datrie->base = (int*)DATMALLOC(sizeof(int) * encodesize);
  datrie->check = (int*)DATMALLOC(sizeof(int) * encodesize);
  datrie->udindex = (int*)DATMALLOC(sizeof(int) * encodesize);
  datrie->udcount = 0;
  datrie->udsize = udsize;
  datrie->userdata = (struct userdata_s*)DATMALLOC(sizeof(struct userdata_s) * udsize);
  if (datrie->base == NULL || datrie->check == NULL || 
      datrie->udindex == NULL || datrie->userdata == NULL)
    {
      DATMEMOUT;
      DATMEMOUT_EXIT(NULL);
    }
  memset(datrie->base, 0, sizeof(int) * encodesize);
  memset(datrie->check, 0, sizeof(int) * encodesize);
  memset(datrie->udindex, 0, sizeof(int) * encodesize);
  memset(datrie->userdata, 0, sizeof(struct userdata_s) * udsize);
  return datrie;
}


void clear_datrie(struct datrie_s* datrie)
{  
  if (datrie != NULL)
    {
      if (datrie->base != NULL)
        {
          DATFREE(datrie->base);
          datrie->base = NULL;
        }
      
      if (datrie->check != NULL)
        {
          DATFREE(datrie->check);
          datrie->check = NULL;
        }
      
      if (datrie->udindex != NULL)
        {
          DATFREE(datrie->udindex);
          datrie->udindex = NULL;
        }

      if (datrie->userdata != NULL)
        {
          DATFREE(datrie->userdata);
          datrie->userdata = NULL;
        }
      
      datrie->wordimage = NULL;
      datrie->event = NULL;
      DATFREE(datrie);
    }
}

static void realloc_array(struct datrie_s* datrie)
{
  int* newbase;
  int* newcheck;
  int* newuserindex;
  int allocsize;
  
  assert(datrie != NULL);
  assert(datrie->base != NULL);
  assert(datrie->check != NULL);
  assert(datrie->udindex != NULL);

  /* base array */
  allocsize = sizeof(int) * (datrie->size + datrie->encodesize * 10) * 2;
  if (datrie->debug)
    printf("Realloc Array: %d\n", allocsize);
  newbase = (int*)DATMALLOC(allocsize);
  if (newbase == NULL)
    {
      DATMEMOUT;
      DATMEMOUT_EXIT(NULL);
    }
  memset(newbase, 0, allocsize);
  memcpy(newbase, datrie->base, sizeof(int) * datrie->size);
  DATFREE(datrie->base);
  datrie->base = newbase;
  
  /* check array */
  newcheck = (int*)DATMALLOC(allocsize);
  if (newcheck == NULL)
    {
      DATMEMOUT;
      DATMEMOUT_EXIT(NULL);
    }
  memset(newcheck, 0, allocsize);
  memcpy(newcheck, datrie->check, sizeof(int) * datrie->size);
  DATFREE(datrie->check);
  datrie->check = newcheck;

  /* userdata index array */
  allocsize = sizeof(int) * (datrie->size + datrie->encodesize * 10) * 2;
  newuserindex = (int*)DATMALLOC(allocsize);
  if (newuserindex == NULL)
    {
      DATMEMOUT;
      DATMEMOUT_EXIT(NULL);
    }
  memset(newuserindex, 0, allocsize);
  memcpy(newuserindex, datrie->udindex, sizeof(int) * datrie->size);
  DATFREE(datrie->udindex);
  datrie->udindex = newuserindex;

  /* new size */
  datrie->size = (datrie->size + datrie->encodesize * 10) * 2;
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
      if (datrie->base[k + state->state] != 0 ||
          datrie->check[k + state->state] != 0)
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
      datrie->check[k + state->state] = index;
      state->index = k + state->state;
      state = state->brerstate;
    }
}

/* 
 * realloc user data 
 * size is new node size
*/
static void realloc_userdata(struct datrie_s* datrie, int size)
{
  int allocsize;
  struct userdata_s* newuserdata;

  datrie->udsize = datrie->udcount + size;
  allocsize  = sizeof(struct userdata_s) * (datrie->udsize);
  newuserdata = (struct userdata_s*)DATMALLOC(allocsize);
  if (newuserdata == NULL)
    {
      DATMEMOUT;
      DATMEMOUT_EXIT(NULL);
    }
  memset(newuserdata, 0, allocsize);
  memcpy(newuserdata, datrie->userdata, sizeof(struct userdata_s) * (datrie->udcount));
  DATFREE(datrie->userdata);
  datrie->userdata = newuserdata;
}

static void build_state(struct datrie_s* datrie, struct trie_state_s* state, struct userdata_s* userdata)
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
    datrie->event->build_state_event(datrie, state, userdata);

  substate = state->nextstate;
  index = state->index;

  while (1)
    {
      if (check_empty_slot(datrie, k, substate) == 1)
        {
          /* base[i] = k */
          datrie->base[index] = k;
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
      assert(datrie->base[index] > 0);
      datrie->base[index] *= -1;
      datrie->udindex[index] = ++datrie->udcount;
      assert(datrie->udcount <= datrie->udsize);
      if (datrie->udcount > datrie->udsize)
        realloc_userdata(datrie, 10);
      COPY_UD__P(datrie->userdata[datrie->udcount-1], userdata);
    }
}

void build_state_list(struct datrie_s* datrie, struct trie_state_s* state_list)
{ 
  if (state_list == NULL)
    return ;

  while (state_list != NULL)
    {
      build_state(datrie, state_list, state_list->userdata);
      state_list = state_list->sortnext;
    }
}

int dat_find_states(struct datrie_s* datrie, struct tstate_s* tstate)
{
  int* states;
  int statecount;
  int c, i, s, t = 0;
  int m = 0;
  int userindex;
  
  if (datrie == NULL || tstate == NULL)
    return 0;

  states = tstate->states;
  statecount = tstate->statecount;
  s = 1;
  for (i = 0; i < statecount; i++)
    {
      getwordcode(c, datrie->wordimage, states[i]);
      t = abs(datrie->base[s]) + c;
      if (datrie->check[t] != s)
          return 0;
      s = t;
    }
  if (datrie->base[t] < 0)
    {
      m = 1;
      userindex = datrie->udindex[t]-1;
      COPY_UD(tstate->userdata, datrie->userdata[userindex]);
    }
  else
    {
      CLEAR_UD(tstate->userdata);
    }
  return m+1;
}

int dat_find_state(struct datrie_s* datrie, struct stateslot_s* stateslot, int state)
{
  int s;
  int t;
  int c;
  int userindex;
  
  if (datrie == NULL || stateslot == NULL)
    return 0;
  
  s = stateslot->s;
  getwordcode(c, datrie->wordimage, state);

  t = abs(datrie->base[s]) + c;
  if (datrie->check[t] != s)
    {
      stateslot->s = 1;
      CLEAR_UD(stateslot->userdata);
      return 0;
    }
  stateslot->s = t;
  if (datrie->base[t] < 0)
    {
      userindex = datrie->udindex[t]-1;
      COPY_UD(stateslot->userdata, datrie->userdata[userindex]);
      return 2;
    }
  CLEAR_UD(stateslot->userdata);
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
      sprintf(DSOUT_BUFF, "%d", datrie->base[i]);
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
      sprintf(DSOUT_BUFF, "%d", datrie->check[i]);
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
