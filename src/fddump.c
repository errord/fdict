/*
 * Copyright 2013 error.d
 * by error.d@gmail.com
 *
 * FDict Project
 *
 * dump
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fdict/wordbase.h>
#include <fdict/fdict.h>
#include <fdict/datafile_parse.h>
#include <fdict/libfdict.h>
#include <fdict/index.h>
#include <fdict/libtime.h>

struct stat_info {
  size_t zero_count;
  size_t term_count;
};

void usage()
{
  printf("Usage: ./fddump configfile indexfile\n");
}


void stat_count(struct datrie_s *datrie, struct stat_info *sinfo)
{
  int i;
  sinfo->zero_count = 0;
  sinfo->term_count = 0;
  for (i = 0; i < datrie->size; i++) {
    if (!datrie->array[i].base && !datrie->array[i].check)
      sinfo->zero_count++;
    if (datrie->array[i].base < 0)
      sinfo->term_count++;
  }
}

void dat_dump(struct index_s *index)
{
  time_info tinfo;
  int time;
  struct datrietree_s* datrie;
  struct stat_info sinfo;

  timestart(&tinfo);
  datrie = loaddatrie_bindict(index->datrie_index_file_name);
  time = timeend(&tinfo);
  printf("Load Datrie Bindict Time: %d\n", time);
  if (!datrie) {
    printf("Load Index Failed: %s\n", index->datrie_index_file_name);
    return;
  }

  printf("Wordimage Size: %lubyte\n", datrie->wordimage->size * sizeof(int));
  printf("Array Size: %lubyte\n", datrie->datrie->size * sizeof(int) * 3);

  stat_count(datrie->datrie, &sinfo);
  printf("Array Total Count: %lu Zero Count: %lu Ratio: %.3f%%\n",
	 datrie->datrie->size,
	 sinfo.zero_count,
	 ((double)sinfo.zero_count / (double)datrie->datrie->size)*100);
  printf("Term Total Count: %lu\n",
	 sinfo.term_count);
}

void dump(const char *configfile, const char *name)
{
  struct fdict_s *fdict;
  struct index_s *index = NULL;

  fdict = fdict_open(name, configfile, "rb", 0);
  if (fdict->error) {
    printf("%s\n", fdict->error);
    exit(0);
  }

  index = fdict->index;
  dat_dump(index);
}

int main(int argv, char** argc)
{
  char *name;
  char *configfile;

  if (argv < 3) {
    usage();
    exit(0);
  }

  configfile = argc[1];
  name = argc[2];
  dump(configfile, name);
  return 0;
}
