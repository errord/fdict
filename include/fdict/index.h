#ifndef _FDICT_INDEX_H_
#define _FDICT_INDEX_H_

enum index_type {
  Datrie = 1
};

struct record_node_s {
  struct record_s *record;
  struct record_node_s *next;
};

struct search_result_s {
  struct fdict_s *fdict;
  uint32 record_count;
  struct record_node_s *root;
  struct record_node_s *end;
  struct record_node_s *cursor;
};

struct record_node_s* record_node_malloc(struct record_s *record);
struct search_result_s* search_result_malloc(struct fdict_s *fdict);
void search_result_free(struct search_result_s *search_result);
void search_result_add_node(struct search_result_s *search_result, struct record_node_s *node);
uint32 search_result_record_count(struct search_result_s *search_result);
struct record_s* search_result_get_record(struct search_result_s *search_result);

typedef bool (*build_init_fn)(struct index_s *index);
typedef bool (*build_start_fn)(struct index_s *index);
typedef bool (*build_end_fn)(struct index_s *index);
typedef bool (*search_init_fn)(struct index_s *index);
typedef struct search_result_s* (*search_start_fn)(struct index_s *index, const char *keyword);
typedef bool (*search_end_fn)(struct index_s *index);

struct build_s {
  const char *data_file;
  data_parse_fn data_parse;
  build_init_fn build_init;
  build_start_fn build_start;
  build_end_fn build_end;
};

struct search_s {
  search_init_fn search_init;
  search_start_fn search_start;
  search_end_fn search_end;
};

struct index_s {
  enum index_type index_type;
  struct build_s build;
  struct search_s search;
  struct fdict_s *fdict;
  char datrie_index_file_name[MAX_PATH];
};

struct index_s* index_malloc(struct fdict_s *fdict);
void index_free(struct index_s *index);
void index_setup(struct index_s *index);

// -- build and search

bool dat_build_init(struct index_s *index);
bool dat_build_start(struct index_s *index);
bool dat_build_end(struct index_s *index);

bool dat_search_init(struct index_s *index);
struct search_result_s* dat_search_start(struct index_s *index, const char *keyword);
bool dat_search_end(struct index_s *index);

#endif /* _FDICT_INDEX_H_ */
