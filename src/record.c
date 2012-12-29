#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fdict/base_type.h>
#include <fdict/wordbase.h>
#include <fdict/memory.h>
#include <fdict/fdict.h>
#include <fdict/util.h>

TYPE_TABLE_HEAD(field_type_table)
"string",
"text",
"int",
"number",
TYPE_TABLE_END

const char* field_type_to_string(enum field_type field_type)
{
  return type_to_string(field_type_table, field_type);
}

enum field_type string_to_field_type(const char *field_type)
{
  return (enum field_type)string_to_type(field_type_table, field_type);
}

const char* field_get_string(struct field_s *field)
{
  return (const char*)field->data;
}

const char* field_get_text(struct field_s *field)
{
  const char **p;
  p = (const char**)(field->data + sizeof(uint32)); /* skip text length */
  return *p;
}

uint32 field_get_text_length(struct field_s *field)
{
  return *(uint32*)field->data;
}

int field_get_int(struct field_s *field)
{
  return (int)(*(int*)field->data);
}

uint32 field_get_text_offset(struct field_s *field)
{
  const char **p;
  uint32 offset;
  p = (const char**)(field->data + sizeof(uint32)); /* skip text length */
  offset = *(uint32*)p;
  return offset;
}

double field_get_number(struct field_s *field)
{
  return (double)(*(double*)field->data);
}

void field_set_string(struct field_s *field, const char *string)
{
  strcpy((char*)field->data, string);
}

void field_set_text(struct field_s *field, const char *text)
{
  const char **p;
  *(uint32*)field->data = strlen(text)+1; /* add end of string '0' */
  p = (const char**)(field->data + sizeof(uint32));
  *p = text;
}

void field_set_only_text(struct field_s *field, const char *text)
{
  const char **p;
  p = (const char**)(field->data + sizeof(uint32)); /* skip text length */
  *p = text;
}

void field_set_text_offset(struct field_s *field, size_t offset)
{
  const char **p;
  p = (const char**)(field->data + sizeof(uint32)); /* skip text length */
  *p = (const char*)offset;
}

void field_set_int(struct field_s *field, int i)
{
  *(int*)field->data = i;
}

void field_set_number(struct field_s *field, double number)
{
  *(double*)field->data = number;
}

bool field_set_value(struct fdict_s *fdict, struct field_s *field, int idx, const char* value)
{
  enum field_type type = fdict->record_info->field_type[idx];
  struct record_info_s *record_info = fdict->record_info;
  uint32 length;
  int _int;
  double _double;

  switch (type) {
  case String:
    length = record_info->field_length[idx];
    if (fdstrlen(record_info->encode, value) > length) {
      fdict->error = "String Field To Long\n";
      return false;
    }
    field_set_string(field, value);
    break;
  case Text:
    field_set_text(field, value);
    break;
  case Int:
    _int = atoi(value);
    field_set_int(field, _int);
    break;
  case Number:
    _double = atof(value);
    field_set_number(field, _double);
    break;
  }
  return true;
}

static size_t type_size(enum field_type type, enum word_encode encode)
{
  switch (type) {
  case String:
    if (encode == utf8)
      return 3;
    else if (encode == utf8_short)
      return 2;
    else if (encode == gbk)
      return 2;

  case Text:
  case Int:
  case Number:
    return 1;
  }
  return 0;
}

struct record_s* record_init(struct fdict_s *fdict, struct record_s *record)
{
  memset(record, 0, sizeof(byte) * fdict->record_info->record_size);
  record->record_id = fdict->record_info->record_number++;
  record->flags = 0;
  return record;
}

struct record_s* record_malloc(struct fdict_s *fdict)
{
  struct record_s *record;
  fdmalloc(record, struct record_s*, sizeof(byte) * fdict->record_info->record_size);
  return record_init(fdict, record);
}

char* record_key(struct record_s *record)
{
  return (char*)record->fields[0].data;
}

uint32 record_id(struct record_s *record)
{
  return record->record_id;
}

struct field_s* record_get_field(struct fdict_s *fdict, struct record_s *record, int idx)
{
  struct field_s *field;
  struct record_info_s *record_info = fdict->record_info;
  field = (struct field_s*)((byte*)record->fields + record_info->field_offset[idx]);
  return field;
}

static void record_text_write(struct fdict_s *fdict, struct field_s *field)
{
  struct record_text_file_s *record_text_file = &fdict->record_text_file;
  const char *text;
  uint32 length;

  length = field_get_text_length(field);
  text = field_get_text(field);
  fwrite(text, sizeof(char), length, record_text_file->fd);
  field_set_text_offset(field, record_text_file->offset);
  record_text_file->offset += length;
}

void record_write_disk(struct fdict_s *fdict, struct record_s *record)
{
  struct record_file_s *record_file = &fdict->record_file;
  struct record_info_s *record_info = fdict->record_info;
  struct field_s *field;
  int i;
  for (i = 0; i < record_info->field_number; i++) {
    if (record_info->field_type[i] == Text) {
      field = record_get_field(fdict, record, i);
      record_text_write(fdict, field);
    }
  }
  fwrite(record, sizeof(byte), record_info->record_size, record_file->fd);
}

static const char*  record_text_read(struct fdict_s *fdict, struct field_s *field)
{
  struct record_text_file_s *record_text_file = &fdict->record_text_file;
  char *text;
  uint32 length;
  uint32 offset;

  length = field_get_text_length(field);
  offset = field_get_text_offset(field);
  fdmalloc(text, char*, sizeof(char) * length);
  fseek(record_text_file->fd, record_text_file->data_base + offset, SEEK_SET);
  fread(text, sizeof(char), length, record_text_file->fd);
  return text;
}

struct record_s* record_read_disk(struct fdict_s *fdict, uint32 record_id)
{
  struct record_file_s *record_file = &fdict->record_file;
  struct record_info_s *record_info = fdict->record_info;
  struct record_s *record;
  struct field_s *field;
  const char *text;
  uint32 offset;
  uint32 size;
  int i;

  if (record_id == 0)
    return NULL;

  record = record_malloc(fdict);
  size = fdict->record_info->record_size;
  offset = (record_id-1) * sizeof(byte) * size;
  fseek(record_file->fd, record_file->data_base + offset, SEEK_SET);
  fread((char*)record, sizeof(char), size, record_file->fd);

  for (i = 0; i < record_info->field_number; i++) {
    if (record_info->field_type[i] == Text) {
      field = record_get_field(fdict, record, i);
      text = record_text_read(fdict, field);
      field_set_only_text(field, text);
    }
  }
  return record;
}

void record_free(struct record_s *record)
{
  fdfree(record);
}

void record_free_all(struct fdict_s *fdict, struct record_s *record)
{
  struct record_info_s *record_info = fdict->record_info;
  struct field_s *field;
  const char *text;
  int i;
  for (i = 0; i < record_info->field_number; i++) {
    if (record_info->field_type[i] == Text) {
      field = record_get_field(fdict, record, i);
      text = field_get_text(field);
      fdfree((char*)text);
    }
  }
  record_free(record);
}

static uint32 record_size(struct record_info_s *record_info)
{
  uint32 record_size = sizeof(struct record_s);
  uint32 field_offset = 0;
  uint32 field_size;
  int i;
  for (i = 0; i < record_info->field_number; i++) {
    field_size = record_info->field_length[i] * type_size(record_info->field_type[i], record_info->encode);
    record_info->field_size[i] = field_size;
    record_info->field_offset[i] = field_offset;
    record_size += field_size;
    field_offset += field_size;
  }
  record_info->record_size = record_size;
  return record_size;
}

struct record_info_s* record_info_malloc(struct fdict_s *fdict)
{
  struct record_info_s *record_info;
  struct data_config_s *config;

  fdmalloc(record_info, struct record_info_s*, sizeof(struct record_info_s));
  fdict->record_info = record_info;
  config = fdict->config;
  record_info->record_number = 1; /* record_id start on 1 */
  record_info->record_size = 0;
  record_info->encode = utf8;
  record_info->field_number = config->field_number;
  record_info->field_type = config->field_type;
  record_info->field_length = config->field_length;
  fdmalloc(record_info->field_size, uint32*, sizeof(uint32) * config->field_number);
  fdmalloc(record_info->field_offset, uint32*, sizeof(uint32) * config->field_number);
  record_size(record_info);
  return record_info;
}

void record_info_free(struct fdict_s *fdict)
{
  struct record_info_s *record_info = fdict->record_info;
  fdfree(record_info->field_size);
  fdfree(record_info->field_offset);
  fdfree(record_info);
  fdict->record_info = NULL;
}

void record_write(struct fdict_s *fdict, struct record_s *record)
{
  record_write_disk(fdict, record);
}

struct record_s* record_read(struct fdict_s *fdict, uint32 record_id)
{
  return record_read_disk(fdict, record_id);
}
