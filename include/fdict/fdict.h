#ifndef _FDICT_CONFIG_H_
#define _FDICT_CONFIG_H_

#include <fdict/base_type.h>

#define DEBUG

#define MAX_DATA_SIZE 1024000 /* 1M */

/* Magic */
#define RECORD_MAGIC (const char*)"FDICT_RECORD"
/* Version */
#define RECORD_VERSION_1_0 MAKE_VERSION(1,0)
#define RECORD_VERSION RECORD_VERSION_1_0

/* Magic */
#define RECORD_TEXT_MAGIC (const char*)"FDICT_RECORD_TEXT"
/* Version */
#define RECORD_TEXT_VERSION_1_0 MAKE_VERSION(1,0)
#define RECORD_TEXT_VERSION RECORD_TEXT_VERSION_1_0

enum field_type {
  String = 1,
  Text,
  Int,
  Number
};

struct field_s {
  byte data[0];
};

struct record_s {
  uint32 record_id;
  uint8 flags;
  struct field_s fields[0];
};

struct record_info_s {
  uint32 record_number;
  uint32 record_size;
  enum word_encode encode;
  uint8 field_number;
  enum field_type *field_type;
  uint32 *field_length;
  uint32 *field_size;
  uint32 *field_offset;
};

enum datafile_type {
  Fldata = 1
};

struct data_config_s {
  enum datafile_type datafile_type;
  int field_number;
  enum field_type *field_type;
  uint32 *field_length;
};

struct record_file_s {
  FILE *fd;
  uint32 data_base;
  byte version;
  char file_name[MAX_PATH];
};

struct record_text_file_s {
  uint32 offset;
  FILE *fd;
  uint32 data_base;
  byte version;
  char file_name[MAX_PATH];
};

struct fdict_s {
  const char *name;
  const char *file_mod;
  struct index_s *index;
  struct record_file_s record_file;
  struct record_text_file_s record_text_file;
  struct record_info_s *record_info;
  struct data_config_s *config;
  char *error;
  int debug;
};

typedef bool (*data_parse_fn)(struct fdict_s *fdict, struct record_s *record, char *data);

const char* field_type_to_string(enum field_type field_type);
enum field_type string_to_field_type(const char *field_type);

const char* field_get_string(struct field_s *field);
const char* field_get_text(struct field_s *field);
uint32 field_get_text_length(struct field_s *field);
uint32 field_get_text_offset(struct field_s *field);
int field_get_int(struct field_s *field);
double field_get_number(struct field_s *field);
void field_set_string(struct field_s *field, const char *string);
void field_set_text(struct field_s *field, const char *text);
void field_set_only_text(struct field_s *field, const char *text);
void field_set_text_offset(struct field_s *field, size_t offset);
void field_set_int(struct field_s *field, int i);
void field_set_number(struct field_s *field, double number);
bool field_set_value(struct fdict_s *fdict, struct field_s *field, int idx, const char* value);

void record_write(struct fdict_s *fdict, struct record_s *record);
struct record_s* record_read(struct fdict_s *fdict, uint32 record_id);

struct record_s* record_init(struct fdict_s *fdict, struct record_s *record);
struct record_s* record_malloc(struct fdict_s *fdict);
char* record_key(struct record_s *record);
uint32 record_id(struct record_s *record);
struct field_s* record_get_field(struct fdict_s *fdict, struct record_s *record, int idx);
void record_write_disk(struct fdict_s *fdict, struct record_s *record);
struct record_s* record_read_disk(struct fdict_s *fdict, uint32 record_id);
void record_free(struct record_s *record);
void record_free_all(struct fdict_s *fdict, struct record_s *record);

struct record_info_s* record_info_malloc(struct fdict_s *fdict);
void record_info_free(struct fdict_s *fdict);

enum datafile_type string_to_datafile_type(const char *datafile_type);
const char* datafile_type_to_string(enum datafile_type datafile_type);

data_parse_fn get_data_parse_fn(enum datafile_type);

struct data_config_s* data_config_file_parse(const char *config_file, char **error);
void data_config_free(struct data_config_s *data_config);
void dump_data_config(struct data_config_s *data_config);

struct fdict_s* fdict_open(const char *name, const char* configfile, const char *file_mod, int debug);
bool fdict_build(struct fdict_s *fdict);
struct search_result_s* fdict_search(struct fdict_s *fdict, const char *keyword);
enum field_type fdict_field_type(struct fdict_s *fdict, int idx);
uint32 fdict_field_number(struct fdict_s *fdict);
void fdict_close(struct fdict_s *fdict);

#endif /* _FDICT_CONFIG_H_ */
