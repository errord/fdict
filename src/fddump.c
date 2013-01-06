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

void usage()
{
  printf("Usage: ./fddump configfile indexfile\n");
}


int array_zero_count(struct datrie_s *datrie)
{
  int i;
  size_t zero_count = 0;
  for (i = 0; i < datrie->size; i++) {
    if (!datrie->base[i] && !datrie->check[i])
      zero_count++;
  }
  return zero_count;
}

void dat_dump(struct index_s *index)
{
  time_info tinfo;
  int time;
  struct datrietree_s* datrie;
  size_t zero_count = 0;

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

  zero_count = array_zero_count(datrie->datrie);
  printf("Array Total Count: %lu Zero Count: %lu Ratio: %.3f%%\n",
	 datrie->datrie->size,
	 zero_count,
	 ((double)zero_count / (double)datrie->datrie->size)*100);
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
