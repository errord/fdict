#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fdict/wordbase.h>
#include <fdict/fdict.h>
#include <fdict/datafile_parse.h>
#include <fdict/libfdict.h>
#include <fdict/index.h>

bool dat_build_start(struct index_s *index)
{
  char buf[MAX_DATA_SIZE];
  struct fdict_s *fdict = index->fdict;
  struct build_s *build = &index->build;
  struct datrietree_s* datrie;
  struct userdata_s userdata;
  enum word_encode encode = utf8_short;
  struct record_s *record;
  data_parse_fn data_parse;
  FILE *fd;
  
  fd = fopen(build->data_file, "r");
  if (!fd) {
    printf("Open %s Failed!!!\n", build->data_file);
    return false;
  }

  data_parse = get_data_parse_fn(fdict->config->datafile_type);
  datrie = makeDatrieTree(encode, NULL);
  record = record_malloc(fdict);
  if (fdict->debug)
    printf("Parse Data And Write Record ...\n");
  while (fgets(buf, MAX_DATA_SIZE, fd)) {
    if (buf[0] == '#' || 
	buf[0] == '\n' ||
	buf[0] == '\0')
      continue;

    if (!data_parse(fdict, record, buf)) {
      printf("Data Failed: %s\n", buf);
      continue;
    }

    userdata.POS = record->record_id;
    addWord(datrie, record_key(record), &userdata, encode);
    record_write(fdict, record);
    record_init(fdict, record);
  }
  record_free(record);
  if (fdict->debug)
    printf("Build Datrie ...\n");
  buildDatrie(datrie, 2, fdict->debug);
  if (fdict->debug)
    dump_datrie(datrie, build->data_file);
  printf("Write Datrie Index File: %s\n", index->datrie_index_file_name);
  savedatrie_bindict(datrie, index->datrie_index_file_name);
  return true;
}

bool dat_build_init(struct index_s *index)
{
  struct build_s *build = &index->build;
  struct fdict_s *fdict = index->fdict;
  build->data_parse = get_data_parse_fn(fdict->config->datafile_type);
  build->data_file = fdict->name;
  return 0;
}

bool dat_build_end(struct index_s *index)
{
  return 0;
}
