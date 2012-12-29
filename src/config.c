#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fdict/base_type.h>
#include <fdict/wordbase.h>
#include <fdict/memory.h>
#include <fdict/fdict.h>
#include <fdict/json.h>

static int json_string_check(const char *string)
{
  if (!strlen(string))
    return 0;
  return 1;
}

static int json_number_check(double number)
{
  if (number == 0.0)
    return 0;
  return 1;
}

static char* data_config_parse_(struct json *json, struct data_config_s *data_config)
{
  double length;
  const char *type;
  const char *type_path = "fields[#].type";
  const char *length_path = "fields[#].length";
  const char *datafile_type;
  char error_buf[1024];
  int field_number, i = 0;

  field_number = json_count(json, "fields");
  data_config->field_number = field_number;

  datafile_type = json_string(json, "datafile_type");
  if (!json_string_check(datafile_type)) {
    return "Not Define datafile_type!!!";
  }
  
  data_config->datafile_type = string_to_datafile_type(datafile_type);
  fdmalloc(data_config->field_type, enum field_type*, sizeof(enum field_type) * field_number);
  fdmalloc(data_config->field_length, uint32*, sizeof(uint32) * field_number);

  for (i = 0; i < field_number; i++) {
    type = json_string(json, type_path, i);

    if (!strcmp(type, "string")) {
      length = json_number(json, length_path, i);
      if (length == 0) {
	sprintf(error_buf, "Field:%d Type:String Length Is 0!!!", i);
	return strdup(error_buf);
      }
    } else if (!strcmp(type, "text")) {
      length = sizeof(uint32) + sizeof(char*);
    } else if (!strcmp(type, "int")) {
      length = sizeof(int);
    } else if (!strcmp(type, "number")) {
      length = sizeof(double);
    } else {
      sprintf(error_buf, "Field:%d Unknow Tpye:%s !!!", i, type);
      return strdup(error_buf);
    }

    data_config->field_type[i] = string_to_field_type(type);
    data_config->field_length[i] = length;
  }

  return NULL;
}

struct data_config_s* data_config_file_parse(const char *config_file, char **error)
{
  struct data_config_s *data_config;
  struct json *json;
  int json_error;
  fdmalloc(data_config, struct data_config_s*, sizeof *data_config);
  *error = NULL;
  json = json_open(0, &json_error);
  json_loadpath(json, config_file);
  *error = data_config_parse_(json, data_config);
  if (*error)
    return NULL;
  json_close(json);
  return data_config;
}

void data_config_free(struct data_config_s *data_config)
{
  fdfree(data_config->field_type);
  fdfree(data_config->field_length);
  fdfree(data_config);
}

void dump_data_config(struct data_config_s *data_config)
{
  int i;
  printf("Data Config:\n");
  printf("\tData File Type: %s\n", datafile_type_to_string(data_config->datafile_type));
  printf("\tField Number: %d\n", data_config->field_number);
  for (i = 0; i < data_config->field_number; i++) {
    printf("\t%d. type: %s", 
	   i+1,
	   field_type_to_string(data_config->field_type[i]));
    if (data_config->field_type[i] != Text)
      printf(" length: %d\n", data_config->field_length[i]);
    else
      printf(" length: VariableLength\n");
  }
}
