#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <memory.h>
#include <string.h>
#include <fdict/clib.h>
#include <fdict/libdsout.h>
#include <fdict/wordimage.h>
#include <fdict/trie.h>

struct trie_s* create_trie(struct wordimage_s* wordimage)
{
  struct trie_s* trie = NULL;
  struct trie_state_s* state;
  
  trie = (struct trie_s*)TRIEMALLOC(sizeof(struct trie_s));
  if (trie == NULL)
    return NULL;
  
  trie->wordimage = wordimage;
  state = (struct trie_state_s*)TRIEMALLOC(sizeof(struct trie_state_s));
  state->state = -1;
  state->nscount = 0;
  state->index = 1;
  state->endstate = 0;
  state->dataid = 0;
  state->nextstate = NULL;
  state->brerstate = NULL;
  state->listnext = NULL;
  state->sortnext = NULL;
  trie->startstate = state;
  trie->sortlisthead = NULL;
  trie->statecount = 0;
  trie->maxstatecount = 0;
  trie->udcount = 0;
  return trie;
}

static void clear_state(struct trie_state_s* state)
{
  struct trie_state_s* curstate;

  /* no next state */
  if (state == NULL)
    return;
  
  while (state != NULL)
    {
      clear_state(state->nextstate);
      curstate = state;
      state = state->brerstate;
      TRIEFREE(curstate);
    }
}

void clear_trie(struct trie_s* trie)
{
  if (trie == NULL)
    return ;

  trie->wordimage = NULL;
  clear_state(trie->startstate);
  trie->startstate = NULL;
  trie->sortlisthead = NULL;
  trie->statecount = 0;
  trie->maxstatecount = 0;
  TRIEFREE(trie);
}

/*
 * find next state from curstate
 * NULL if no finded, nextstate if finded
 */
static struct trie_state_s* find_state(struct trie_state_s* curstate, int state)
{
  curstate = curstate->nextstate;
  while (curstate != NULL)
    {
      if (curstate->state == state)
        {
          return curstate;
        }
      curstate = curstate->brerstate;
    }
  return NULL;
}

/*
 * add sub state at curstate
 * return newstate
 */
static struct trie_state_s* add_sub_state(struct trie_state_s* curstate, int state)
{
  struct trie_state_s* newstate;
  
  newstate = (struct trie_state_s*)TRIEMALLOC(sizeof(struct trie_state_s));
  if (newstate == NULL)
    {
      TRIEMEMOUT;
      TRIEMEMOUT_EXIT(NULL);
    }

  newstate->state = state;
  newstate->nscount = 0;
  newstate->index = 1;
  newstate->endstate = 0;
  newstate->dataid = 0;
  newstate->brerstate = NULL;
  newstate->nextstate = NULL;
  newstate->listnext = NULL;
  newstate->sortnext = NULL;

  if (curstate->nextstate != NULL)
    {
      /* linke to brer state, is sub state */
      newstate->brerstate = curstate->nextstate;
      curstate->nextstate = newstate;
    }
  else
    curstate->nextstate = newstate;
  return newstate;
}

int add_states(struct trie_s* trie, struct tstate_s* tstate)
{
  struct trie_state_s* curstate;
  struct trie_state_s* nextstate;
  int* states;
  int statecount;
  int state;
  int i;

  if (trie == NULL || tstate == NULL)
    return 0;
  
  curstate = trie->startstate;
  statecount = tstate->statecount;
  states = tstate->states;
  for (i = 0; i < statecount; i++)
    {
      addword(trie->wordimage, states[i]);
      getwordcode(state, trie->wordimage, states[i]);
      nextstate = find_state(curstate, state);
      /* 
       * no finded, add new state 
       * finded, move next state
       */
      if (nextstate == NULL)
        {
          curstate = add_sub_state(curstate, state);
        }
      else
        curstate = nextstate;
    }
  /* set end state */
  curstate->endstate = 1;
  curstate->dataid = tstate->dataid;
  trie->udcount++;
  return 1;
}

int find_states(struct trie_s* trie, struct tstate_s* tstate)
{
  struct trie_state_s* curstate;
  int state;
  int i;
  int* states;
  int statecount;
  
  if (trie == NULL || tstate == NULL)
    return 0;
  
  states = tstate->states;
  statecount = tstate->statecount;
  curstate = trie->startstate;
  for (i = 0; i < statecount; i++)
    {
      addword(trie->wordimage, states[i]);
      getwordcode(state, trie->wordimage, states[i]);
      curstate = find_state(curstate, state);
      if (curstate == NULL)
        return 0;
    }
  tstate->dataid = curstate->dataid;
  return 1;
}

int get_userdata_count(struct trie_s* trie)
{
  return trie->udcount;
}

static void link_to_sortlist(struct trie_s* trie, struct trie_state_s* state)
{
  
  if (state->nscount > trie->maxstatecount)
    trie->maxstatecount = state->nscount;
  
  if (trie->sortlisthead == NULL)
    {
      state->listnext = NULL;
      trie->sortlisthead = state;
    }
  else
    {
      /* [new] -> [old] */
      state->listnext = trie->sortlisthead;
      /* [head] -> [new] */
      trie->sortlisthead = state;
    }
}


/*
 * traverse trie
 * 1. compute count
 * 2. link to sort list
 * return state count
 */
static int count_state(struct trie_s* trie, struct trie_state_s* state)
{
  int count = 0;
  
  /* no next state */
  if (state == NULL)
    return 0;
  
  while (state != NULL)
    {
      state->nscount = count_state(trie, state->nextstate);
      /* link node to sort list */
      link_to_sortlist(trie, state);
      count += state->nscount + 1;
      state = state->brerstate;
    }
  return count;
}

static int sort_state_list(struct trie_s* trie)
{
  struct sort_slot_node
  {
    struct trie_state_s* head;
    struct trie_state_s* end;
  };
  struct sort_slot_node* sorttable;
  struct trie_state_s* listhead = NULL;
  struct trie_state_s* head;
  struct trie_state_s* end;
  struct trie_state_s* node;
  struct trie_state_s* sort_node;
  int index;
  int msize;
  
  msize = sizeof(struct sort_slot_node) * (trie->maxstatecount + 1);
  sorttable = (struct sort_slot_node*)malloc(msize);
  if (sorttable == NULL)
    return 0;
  memset(sorttable, 0, msize);
  
  node = trie->sortlisthead;
  /* iterative all node from sort list */
  while (node != NULL)
    {
      /* empty sort table slot
       * insert to sort table slot 
       */
      if (sorttable[(unsigned int)node->nscount].head == NULL)
        {
          node->sortnext = NULL;
          sorttable[(unsigned int)node->nscount].head = 
            sorttable[(unsigned int)node->nscount].end = node;
        }
      /* insert node to list head  */
      else
        {
          sort_node = sorttable[(unsigned int)node->nscount].head;
          node->sortnext = sort_node;
          sorttable[(unsigned int)node->nscount].head = node;
        }
      node = node->listnext;
    }
  
  
  /* make sorted list */  
  index = trie->maxstatecount;
  end = NULL;
  while (index >= 0)
    {
      head = sorttable[index].head;
      if (head == NULL)
        {
          index--;
          continue;
        }
      
      if (listhead == NULL)
        listhead = head;
      if (end != NULL)
        end->sortnext = head;
      end = sorttable[index].end;
      index--;
    }

  free(sorttable);
  trie->sortlisthead = listhead;
  return 1;
}

struct trie_state_s* get_sort_state_list(struct trie_s* trie)
{
  if (trie == NULL)
    return NULL;

  trie->statecount = count_state(trie, trie->startstate);
  if ( sort_state_list(trie) == 0)
    return NULL;
  return trie->sortlisthead;
}

int wordimage_out_text(struct trie_s* trie, const char* out_file_name)
{
  DSOUT_DEFINE;
  int i;
  struct wordimage_s* wordimg;
  
  if (trie == NULL)
    return 0;
  
  DSOUT_OPEN_OUTFILE(out_file_name);
  DSOUT_INFO("WordImage:\n");
  wordimg = trie->wordimage;
  for (i = 0; i < wordimg->size; i++)
    {
      
      if (wordimg->wordimage[i] != 0)
        {
          sprintf(DSOUT_BUFF, "\t%d => %d\n", i, wordimg->wordimage[i]);
          DSOUT_INFO(DSOUT_BUFF);
        }
    }
  DSOUT_CLOSE_OUTFILE;
  return 0;
}
