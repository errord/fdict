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
#include <fdict/libtime.h>

void usage()
{
  printf("Usage: ./fdsearch configfile indexfile [keyword | -f file] [debug]\n");
  printf("\t-f\tkeywords file\n");
}

void search_by_keyword(struct fdict_s *fdict, const char *keyword, int index)
{
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
  time_info tinfo;
  int search_time, result_time;

  printf("T-- [%d] -------------------------------T\n", index);

  timestart(&tinfo);
  search_result = fdict_search(fdict, keyword);
  search_time = timeend_usec(&tinfo);

  timestart(&tinfo);
  if (!search_result) {
    printf("Not found: [%s]", keyword);
  } else {
    record = search_result_get_record(search_result);
    field_number = fdict_field_number(fdict);
    r_id = record_id(record);
    printf("[keyword:%s] -> \n\t[id:%d]", keyword, r_id);
    for (i = 0; i < field_number; i++) {
      type = fdict_field_type(fdict, i);
      field = record_get_field(fdict, record, i);
      switch (type) {
      case String:
	_s = field_get_string(field);
	printf("\n\t[%d:%s]", i, _s);
	break;
      case Text:
	_s = field_get_text(field);
	printf("\n\t[%d:%s]", i, _s);
	break;
      case Int:
	_int = field_get_int(field);
	printf("\n\t[%d:%d]", i, _int);
	break;
      case Number:
	_number = field_get_number(field);
	printf("\n\t[%d:%f]", i, _number);
	break;
      }
    }
    search_result_free(search_result);
  }
  result_time = timeend_usec(&tinfo);
  printf("\nSearch Time: %d usec  Result Time: %d usec\n", search_time, result_time);
}

int search_by_file(struct fdict_s *fdict, const char *kfile)
{
  char buf[MAX_DATA_SIZE];
  FILE *fd;
  size_t e;
  int index = 0;

  fd = fopen(kfile, "r");
  if (!fd) {
    printf("Open %s Failed!!!\n", kfile);
    return index;
  }

  while (fgets(buf, MAX_DATA_SIZE, fd)) {
    index++;
    if (buf[0] == '#' || 
	buf[0] == '\n' ||
	buf[0] == '\0')
      continue;
    e = strlen(buf) - 1;
    if (buf[e] == '\n' || buf[e] == '\r')
      buf[e] = '\0';
    search_by_keyword(fdict, buf, index);
  }
  fclose(fd);
  return index;
}

void search(const char *configfile, const char *name, const char *keyword, const char *kfile, int debug)
{
  struct fdict_s *fdict;
  time_info tinfo;
  int time;
  int index = 1;

  fdict = fdict_open(name, configfile, "rb", debug);
  if (fdict->error) {
    printf("%s\n", fdict->error);
    exit(0);
  }

  timestart(&tinfo);

  if (keyword)
    search_by_keyword(fdict, keyword, index);
  else
    index = search_by_file(fdict, kfile);

  time = timeend_usec(&tinfo);
  printf("%d Keyword Search Total Time: %d usec\n", index, time);

  fdict_close(fdict);
}

int main(int argv, char** argc)
{
  char *name;
  char *keyword;
  char *kfile = NULL;
  char *configfile;
  int debug = 0;
  int arg_total = 5;

  if (argv < 4) {
    usage();
    exit(0);
  }

  configfile = argc[1];
  name = argc[2];
  keyword = argc[3];

  if (!strcmp(keyword, "-f")) {
    kfile = argc[4];
    keyword = NULL;
    arg_total = 6;
  }

  if (argv == arg_total) {
    if (!strcmp(argc[arg_total - 1], "debug"))
      debug = 1;
  }
  
  if (debug)
    printf("Run Module: Debug\n");
  search(configfile, name, keyword, kfile, debug);
  return 0;
}
