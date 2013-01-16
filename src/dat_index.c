#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fdict/memory.h>
#include <fdict/wordbase.h>
#include <fdict/fdict.h>
#include <fdict/datafile_parse.h>
#include <fdict/libfdict.h>
#include <fdict/index.h>
#include <fdict/libtime.h>

bool dat_index_data_init(struct index_s *index)
{
  struct fdict_s *fdict = index->fdict;
  struct datrietree_s* datrie;
  time_info tinfo;
  int time;

  if (fdict->debug)
    timestart(&tinfo);

  datrie = loaddatrie_bindict(index->datrie_index_file_name);

  if (fdict->debug) {
    time = timeend(&tinfo);
    printf("Load Datrie Index Bindict Time: %d msec\n", time);
  }

  if (!datrie)
    return false;
  index->index_data = (void*)datrie;
  return true;
}

bool dat_index_data_clear(struct index_s *index)
{
  fdfree(index->index_data);
  return true;
}
