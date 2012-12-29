#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <assert.h>
#include <fdict/wordbase.h>
#include <fdict/fdict.h>
#include <fdict/util.h>
#include <fdict/datafile_parse.h>

TYPE_TABLE_HEAD(datafile_type_table)
"fldata",
TYPE_TABLE_END

data_parse_fn data_parse_fn_table[] = {
  NULL,
  fldata_parse
};

const char* datafile_type_to_string(enum datafile_type datafile_type)
{
  return type_to_string(datafile_type_table, datafile_type);
}

enum datafile_type string_to_datafile_type(const char *datafile_type)
{
  return (enum datafile_type)string_to_type(datafile_type_table, datafile_type);
}

data_parse_fn get_data_parse_fn(enum datafile_type type)
{
  return data_parse_fn_table[type];
}

static char* fldata_field_end_pos(char *data)
{
  char ch = *data;
  while (ch != '\t' && ch != '\n' && ch != '\0') {
    data++;
    ch = *data;
  }
  return data;
}

bool fldata_parse(struct fdict_s *fdict, struct record_s *record, char *data)
{
  struct field_s *field;
  char *start, *end;
  int idx = 0;

  start = data;
  end = data;
  while (*end != '\0' && *end != '\n') {
    end = fldata_field_end_pos(end);
    *end = '\0';
    field = record_get_field(fdict, record, idx);
    if (!field_set_value(fdict, field, idx, (const char*)start)) {
      if (fdict->error)
	printf("%s\n", fdict->error);
      else
	printf("Unknow Error!!");
      return false;
    }
    start = ++end;
    idx++;
  }
  return true;
}
