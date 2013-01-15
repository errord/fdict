#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fdict/wordbase.h>
#include <fdict/fdict.h>
#include <fdict/datafile_parse.h>
#include <fdict/libfdict.h>
#include <fdict/index.h>
#include <fdict/libtime.h>

struct search_result_s* dat_search_start(struct index_s *index, const char *keyword)
{
  struct search_result_s *search_result;
  struct record_node_s *record_node;
  struct record_s *record;
  struct fdict_s *fdict = index->fdict;
  struct datrietree_s* datrie;
  enum word_encode encode = fdict->config->encode;
  uint32 record_id;
  int r;
  time_info tinfo;
  int time;
  unsigned int dataid;

  datrie = (struct datrietree_s*)index->index_data;

  if (!datrie) {
    printf("Load Index Failed: %s\n", index->datrie_index_file_name);
    return NULL;
  }

  if (fdict->debug)
    timestart(&tinfo);

  r = findWordByString(datrie, keyword, &dataid, encode, fdict->debug);

  if (fdict->debug) {
    time = timeend_usec(&tinfo);
    printf("Search Time: %d usec\n", time);
  }

  if (r != 2) {
    return NULL;
  }

  if (fdict->debug)
    restart_timeinfo(&tinfo);

  search_result = search_result_malloc(fdict);
  record_id = dataid;
  record = record_read(fdict, record_id);
  record_node = record_node_malloc(record);
  search_result_add_node(search_result, record_node);

  if (fdict->debug) {
    time = timeend_usec(&tinfo);
    printf("Create Search Result Time: %d usec\n", time);
  }

  return search_result;
}

bool dat_search_init(struct index_s *index)
{
  return true;
}

bool dat_search_end(struct index_s *index)
{
  return false;
}
