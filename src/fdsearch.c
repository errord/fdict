/*
 * Copyright 2012 error.d
 * by error.d@gmail.com
 *
 * FDict Project
 *
 * search
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <memory.h>
#include <string.h>
#include <fdict/wordbase.h>
#include <fdict/fdict.h>
#include <fdict/libfdict.h>
#include <fdict/index.h>

void usage()
{
  printf("Usage: ./fdsearch configfile indexfile keyword [debug]\n");
}

void search(const char *configfile, const char *name, const char *keyword, int debug)
{
  struct fdict_s *fdict;
  struct search_result_s *search_result;
  struct record_s *record;
  struct field_s *field;
  enum field_type type;
  uint32 field_number;
  uint32 r_id;
  uint32 i;
  const char *_s;
  int _int;
  double _number;

  fdict = fdict_open(name, configfile, "rb", debug);
  if (fdict->error) {
    printf("%s\n", fdict->error);
    exit(0);
  }

  search_result = fdict_search(fdict, keyword);
  if (!search_result) {
    printf("Not found: %s\n", keyword);
    return;
  }

  record = search_result_get_record(search_result);
  field_number = fdict_field_number(fdict);
  r_id = record_id(record);
  printf("[keyword:%s] -> [id:%d]", keyword, r_id);
  for (i = 0; i < field_number; i++) {
    type = fdict_field_type(fdict, i);
    field = record_get_field(fdict, record, i);
    switch (type) {
    case String:
      _s = field_get_string(field);
      printf(" [%d:%s]", i, _s);
      break;
    case Text:
      _s = field_get_text(field);
      printf(" [%d:%s]", i, _s);
      break;
    case Int:
      _int = field_get_int(field);
      printf(" [%d:%d]", i, _int);
      break;
    case Number:
      _number = field_get_number(field);
      printf(" [%d:%f]", i, _number);
      break;
    }
  }
  printf("\n");

  search_result_free(search_result);
}

int main(int argv, char** argc)
{
  char *name;
  char *keyword;
  char *configfile;
  int debug = 0;

  if (argv < 4) {
    usage();
    exit(0);
  }

  configfile = argc[1];
  name = argc[2];
  keyword = argc[3];

  if (argv == 5) {
    if (!strcmp(argc[4], "debug"))
      debug = 1;
  }
  
  if (debug)
    printf("Run Module: Debug\n");
  search(configfile, name, keyword, debug);
  return 0;
}
