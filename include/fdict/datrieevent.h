/*
 *
 * clib -- libdatrie
 * datrie event
 *
 */
#ifndef _CLIB_LIBDATRIE_DATRIE_EVENT_H_
#define _CLIB_LIBDATRIE_DATRIE_EVENT_H_

struct datrieevent_s
{
  int build_state_event_data;
  void (*build_state_event)(struct datrie_s* datrie, 
                            struct trie_state_s* state, 
                            struct userdata_s* userdata);
};


#endif /* _CLIB_LIBDATRIE_DATRIE_EVENT_H_ */

