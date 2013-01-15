#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fdict/base_type.h>
#include <fdict/memory.h>
#include <fdict/wordbase.h>
#include <fdict/util.h>
#include <fdict/fdict.h>
#include <fdict/index.h>

static bool record_head_init(FILE *fd, const char *magic, byte *version, const char *file_mod)
{
  char buf[128];
  if (!strcmp("wb", file_mod)) {
    fwrite(magic, sizeof(char), strlen(magic), fd);
    fwrite(version, sizeof(char), 1, fd);
  } else {
    fread(buf, sizeof(char), strlen(magic), fd);
    if (strncmp(magic, (const char*)buf, strlen(magic)) != 0)
      return false;
    fread(version, sizeof(char), 1, fd);
  }
  return true;
}

static bool record_file_open(struct fdict_s *fdict)
{
  struct record_file_s *record_file = &fdict->record_file;
  sprintf(record_file->file_name, "%s.record", fdict->name);
  record_file->fd = fopen(record_file->file_name, fdict->file_mod);
  if (!record_file->fd)
    return false;
  record_file->data_base = strlen(RECORD_MAGIC);
  record_file->data_base += sizeof(record_file->version);
  return record_head_init(record_file->fd, RECORD_MAGIC, &record_file->version, fdict->file_mod);
}

static bool record_text_file_open(struct fdict_s *fdict)
{
  struct record_text_file_s *record_text_file = &fdict->record_text_file;
  sprintf(record_text_file->file_name, "%s.record.text", fdict->name);
  record_text_file->fd = fopen(record_text_file->file_name, fdict->file_mod);
  record_text_file->data_base = strlen(RECORD_TEXT_MAGIC);
  record_text_file->data_base += sizeof(record_text_file->version);
  return record_head_init(record_text_file->fd, RECORD_TEXT_MAGIC, &record_text_file->version, fdict->file_mod);
}

static bool fdict_open_file(struct fdict_s *fdict)
{
  if (!record_file_open(fdict))
    return false;
  if (!record_text_file_open(fdict))
    return false;
  return true;
}

struct fdict_s* fdict_open(const char *name, const char* configfile, const char *file_mod, int debug)
{
  struct fdict_s *fdict;
  struct data_config_s *config;

  fdmalloc0(fdict, struct fdict_s*, sizeof(struct fdict_s));
  config = data_config_file_parse(configfile, &(fdict->error));
  if (fdict->error)
    return fdict;

  fdict->name = name;
  fdict->debug = debug;
  fdict->config = config;
  fdict->file_mod = file_mod;
  if (!fdict_open_file(fdict)) {
    fdict->error = "Open Fdict File Failed!!!";
    return fdict;
  }
  fdict->index = index_malloc(fdict);
  index_setup(fdict->index);
  fdict->index->index_data_init(fdict->index);
  record_info_malloc(fdict);
  return fdict;
}

bool fdict_build(struct fdict_s *fdict)
{
  struct index_s *index = fdict->index;
  struct build_s *build = &fdict->index->build;
  build->build_init(index);
  if (build->build_start(index))
    return true;
  return false;
}

struct search_result_s* fdict_search(struct fdict_s *fdict, const char *keyword)
{
  struct index_s *index = fdict->index;
  struct search_s *search = &fdict->index->search;
  search->search_init(index);
  return search->search_start(index, keyword);
}

uint32 fdict_field_number(struct fdict_s *fdict)
{
  return fdict->record_info->field_number;
}

enum field_type fdict_field_type(struct fdict_s *fdict, int idx)
{
  return fdict->record_info->field_type[idx];
}

void fdict_close(struct fdict_s *fdict)
{
  fdict->index->index_data_clear(fdict->index);
  record_info_free(fdict);
  fdfree(fdict);
}
