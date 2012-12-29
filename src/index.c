#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fdict/memory.h>
#include <fdict/wordbase.h>
#include <fdict/fdict.h>
#include <fdict/datafile_parse.h>
#include <fdict/index.h>

struct index_s* index_malloc(struct fdict_s *fdict)
{
  struct index_s *index;
  fdmalloc0(index, struct index_s*, sizeof(struct fdict_s));
  index->index_type = Datrie;
  index->fdict = fdict;
  sprintf(index->datrie_index_file_name, "%s.datindex", fdict->name);
  return index;
}

void index_free(struct index_s *index)
{
  fdfree(index);
}

void index_setup(struct index_s *index)
{
  if (index->index_type == Datrie) {
    index->build.build_init = dat_build_init;
    index->build.build_start = dat_build_start;
    index->build.build_end = dat_build_end;
    
    index->search.search_init = dat_search_init;
    index->search.search_start = dat_search_start;
    index->search.search_end = dat_search_end;
  }
}

struct record_node_s* record_node_malloc(struct record_s *record)
{
  struct record_node_s *record_node;
  fdmalloc(record_node, struct record_node_s*, sizeof(struct record_node_s));
  record_node->record = record;
  record_node->next = NULL;
  return record_node;
}

struct search_result_s* search_result_malloc(struct fdict_s *fdict)
{
  struct search_result_s *search_result;
  fdmalloc(search_result, struct search_result_s*, sizeof(struct search_result_s));
  search_result->fdict = fdict;
  search_result->record_count = 0;
  search_result->root = NULL;
  search_result->end = NULL;
  search_result->cursor = NULL;
  return search_result;
}

void search_result_free(struct search_result_s *search_result)
{
  struct record_node_s *record_node = search_result->root;
  struct record_node_s *temp;
  while (record_node) {
    temp = record_node;
    record_node = record_node->next;
    record_free_all(search_result->fdict, temp->record);
    fdfree(temp);
  }
  fdfree(search_result);  
}

void search_result_add_node(struct search_result_s *search_result, struct record_node_s *node)
{
  if (!search_result->root) {
    search_result->root = node;
    search_result->cursor = node;
  }
  if (search_result->end)
    search_result->end->next = node;
  search_result->end = node;
}

uint32 search_result_record_count(struct search_result_s *search_result)
{
  return search_result->record_count;
}

struct record_s* search_result_get_record(struct search_result_s *search_result)
{
  struct record_s *record = NULL;
  if (search_result->cursor) {
    record = search_result->cursor->record;
    search_result->cursor = search_result->cursor->next;
  }
  return record;
}
