/*
 * Copyright 2012 error.d
 * by error.d@gmail.com
 *
 * FDict Project
 *
 * build
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <memory.h>
#include <string.h>
#include <fdict/wordbase.h>
#include <fdict/fdict.h>

void usage()
{
  printf("Usage: ./fdbuild configfile datafile [debug]\n");
}

int build(const char *configfile, const char *datafile, int debug)
{
  struct fdict_s *fdict;

  fdict = fdict_open(datafile, configfile, "wb", debug);
  if (fdict->error) {
    printf("%s\n", fdict->error);
    exit(0);
  }

  if (debug)
    dump_data_config(fdict->config);

  if (fdict_build(fdict))
    printf("Build Success!!\n");

  return 0;
}

int main(int argv, char** argc)
{
  const char *datafile;
  const char *configfile;
  int debug = 0;

  if (argv < 3) {
    usage();
    exit(0);
  }
  
  if (argv == 4) {
    if (!strcmp(argc[3], "debug"))
      debug = 1;
  }
  
  configfile = argc[1];
  datafile = argc[2];
  
  if (debug)
    printf("Run Module: Debug\n");
  build(configfile, datafile, debug);
  return 0;
}
