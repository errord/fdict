#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fdict/wordbase.h>
#include <fdict/libfdict.h>
#include <fdict/clib.h>
#include <fdict/wordimage.h>
#include <fdict/trie.h>
#include <fdict/datrie.h>
#include <fdict/check.h>

struct wordimage_s* wordimage;

void check_init()
{
  /* GBK 0x0000 -- 0xFFFF */
  wordimage = create_wordimage(65535);
}

void check_close()
{
  clear_wordimage(wordimage);
}


void test_trie_tree_states()
{
  int states1[5] = {1, 2, 3, 4, 5};
  int states2[4] = {3, 6, 8, 7};
  int states3[5] = {1, 2, 5, 8, 9};
  int states4[5] = {3, 7, 9, 2, 5};

  int fs1[5] = {1, 2, 3, 4, 5};
  int fs2[6] = {1, 2, 3, 4, 5, 7};
  int fs3[3] = {1, 2, 5};
  int fs4[4] = {1, 2, 3, 4};
  int fs5[5] = {1, 2, 5, 8, 7};
  int fs6[5] = {3, 7, 9, 2, 5};

  int r;
  struct tstate_s tstate;
  struct trie_s* trie;
  
  trie = create_trie(wordimage);
  
  /* add states */
  tstate.states = states1;
  tstate.statecount = 5;
  tstate.userdata.freq = 4;
  tstate.userdata.POS = 10;
  r = add_states(trie, &tstate);
  CHECK("check add states1", r > 0);
  tstate.states = states2;
  tstate.statecount = 4;
  tstate.userdata.freq = 3;
  r = add_states(trie, &tstate);
  CHECK("check add states2", r > 0);
  tstate.states = states3;
  tstate.statecount = 5;
  tstate.userdata.freq = 2;
  r = add_states(trie, &tstate);
  CHECK("check add states3", r > 0);
  tstate.states = states4;
  tstate.statecount = 5;
  tstate.userdata.freq = 1;
  r = add_states(trie, &tstate);
  CHECK("check add states4", r > 0);

  
  /* find states */
  tstate.states = fs1;
  tstate.statecount = 5;
  tstate.userdata.freq = 0;
  r = find_states(trie, &tstate);
  CHECK("check find fs1", r == 1 && tstate.userdata.freq == 4 && tstate.userdata.POS == 10);
  tstate.states = fs2;
  tstate.statecount = 6;
  tstate.userdata.freq = 0;
  r = find_states(trie, &tstate);
  CHECK("check find fs2", r == 0 && tstate.userdata.freq == 0);
  tstate.states = fs3;
  tstate.statecount = 3;
  tstate.userdata.freq = 0;
  r = find_states(trie, &tstate);
  CHECK("check find fs3", r == 1 && tstate.userdata.freq == 0);
  tstate.states = fs4;
  tstate.statecount = 4;
  tstate.userdata.freq = 0;
  r = find_states(trie, &tstate);
  CHECK("check find fs4", r == 1 && tstate.userdata.freq == 0);
  tstate.states = fs5;
  tstate.statecount = 5;
  tstate.userdata.freq = 0;
  r = find_states(trie, &tstate);
  CHECK("check find fs5", r == 0 && tstate.userdata.freq == 0);
  tstate.states = fs6;
  tstate.statecount = 5;
  tstate.userdata.freq = 0;
  r = find_states(trie, &tstate);
  CHECK("check find fs5", r == 1 && tstate.userdata.freq == 1);

  /* trie tree sort */
  struct trie_state_s* sortlist = get_sort_state_list(trie);

  r = sortlist->nscount;
  CHECK("sort result 8", r == 16);
  sortlist = sortlist->sortnext;  
  r = sortlist->nscount;
  CHECK("sort result 7", r == 7);
  sortlist = sortlist->sortnext;
  r = sortlist->nscount;
  CHECK("sort result 7", r == 7);
  sortlist = sortlist->sortnext;
  r = sortlist->nscount;
  CHECK("sort result 6", r == 6);
  sortlist = sortlist->sortnext;
  r = sortlist->nscount;
  CHECK("sort result 3", r == 3);
  sortlist = sortlist->sortnext;
  r = sortlist->nscount;
  CHECK("sort result 2", r == 2);
  sortlist = sortlist->sortnext;
  r = sortlist->nscount;
  CHECK("sort result 2", r == 2);
  sortlist = sortlist->sortnext;
  r = sortlist->nscount;
  CHECK("sort result 2", r == 2);
  sortlist = sortlist->sortnext;
  r = sortlist->nscount;
  CHECK("sort result 2", r == 2);
  sortlist = sortlist->sortnext;
  r = sortlist->nscount;
  CHECK("sort result 1", r == 1);
  sortlist = sortlist->sortnext;
  r = sortlist->nscount;
  CHECK("sort result 1", r == 1);
  sortlist = sortlist->sortnext;
  r = sortlist->nscount;
  CHECK("sort result 1", r == 1);
  sortlist = sortlist->sortnext;
  r = sortlist->nscount;
  CHECK("sort result 1", r == 1);
  sortlist = sortlist->sortnext;
  r = sortlist->nscount;
  CHECK("sort result 0", r == 0);
  sortlist = sortlist->sortnext;
  r = sortlist->nscount;
  CHECK("sort result 0", r == 0);
  sortlist = sortlist->sortnext;
  r = sortlist->nscount;
  CHECK("sort result 0", r == 0);
  sortlist = sortlist->sortnext;
  r = sortlist->nscount;
  CHECK("sort result 0", r == 0);

  clear_trie(trie);  
}

void test_datrie_states()
{
  int states1[2] = {1, 1};
  int states2[3] = {1, 1, 2};
  int states3[3] = {1, 1, 4};
  int states4[2] = {2, 3};
  int states5[2] = {2, 5};
  int states6[3] = {2, 5, 4};
  int states7[2] = {3, 4};

  int fs1[2] = {1, 1};
  int fs2[3] = {1, 1, 2};
  int fs3[3] = {1, 1, 4};
  int fs4[2] = {2, 3};
  int fs5[2] = {2, 5};
  int fs6[3] = {2, 5, 4};
  int fs7[2] = {3, 4};

  int fs8[1] = {1}; 
  int fs9[3] = {1, 2, 3};
  int fs10[3] = {1, 1, 3};

  int r;
  struct tstate_s tstate;
  struct trie_s* trie;
  struct datrie_s* datrie;
  struct trie_state_s* state_list;
  struct stateslot_s stateslot;
  
  trie = create_trie(wordimage);
  /* add states */
  tstate.states = states1;
  tstate.statecount = 2;
  tstate.userdata.freq = 7;
  r = add_states(trie, &tstate);
  CHECK("check add states1", r > 0);
  tstate.states = states2;
  tstate.statecount = 3;
  tstate.userdata.freq = 6;
  r = add_states(trie, &tstate);
  CHECK("check add states2", r > 0);
  tstate.states = states3;
  tstate.statecount = 3;
  tstate.userdata.freq = 5;
  r = add_states(trie, &tstate);
  CHECK("check add states3", r > 0);
  tstate.states = states4;
  tstate.statecount = 2;
  tstate.userdata.freq = 4;
  r = add_states(trie, &tstate);
  CHECK("check add states4", r > 0);
  tstate.states = states5;
  tstate.statecount = 2;
  tstate.userdata.freq = 3;
  r = add_states(trie, &tstate);
  CHECK("check add states5", r > 0);
  tstate.states = states6;
  tstate.statecount = 3;
  tstate.userdata.freq = 2;
  r = add_states(trie, &tstate);
  CHECK("check add states6", r > 0);
  tstate.states = states7;
  tstate.statecount = 2;
  tstate.userdata.freq = 1;
  r = add_states(trie, &tstate);
  CHECK("check add states7", r > 0);

  trie_out_text(trie, NULL);
  datrie = create_datrie(wordimage, NULL, get_userdata_count(trie), 1, 0);
  state_list = get_sort_state_list(trie);
  build_state_list(datrie, state_list);
  
  /* find state */
  stateslot.s = 1;
  stateslot.userdata.freq = 0;
  r = dat_find_state(datrie, &stateslot, 1);
  CHECK("check find state=1", r == 1 && stateslot.userdata.freq == 0);
  r = dat_find_state(datrie, &stateslot, 1);
  CHECK("check find state=1", r == 2 && stateslot.userdata.freq == 7);
  r = dat_find_state(datrie, &stateslot, 4);
  CHECK("check find state=4", r == 2 && stateslot.userdata.freq == 5);

  /* find states */
  tstate.states = fs1;
  tstate.statecount = 2;
  tstate.userdata.freq = 0;
  r = dat_find_states(datrie, &tstate);
  CHECK("check find states1", r == 2 && tstate.userdata.freq == 7);
  tstate.states = fs2;
  tstate.statecount = 3;
  tstate.userdata.freq = 0;
  r = dat_find_states(datrie, &tstate);
  CHECK("check find states2", r == 2 && tstate.userdata.freq == 6);
  tstate.states = fs3;
  tstate.statecount = 3;
  tstate.userdata.freq = 0;
  r = dat_find_states(datrie, &tstate);
  CHECK("check find states3", r == 2 && tstate.userdata.freq == 5);
  tstate.states = fs4;
  tstate.statecount = 2;
  tstate.userdata.freq = 0;
  r = dat_find_states(datrie, &tstate);
  CHECK("check find states4", r == 2 && tstate.userdata.freq == 4);
  tstate.states = fs5;
  tstate.statecount = 2;
  tstate.userdata.freq = 0;
  r = dat_find_states(datrie, &tstate);
  CHECK("check find states5", r == 2 && tstate.userdata.freq == 3);
  tstate.states = fs6;
  tstate.statecount = 3;
  tstate.userdata.freq = 0;
  r = dat_find_states(datrie, &tstate);
  CHECK("check find states6", r == 2 && tstate.userdata.freq == 2);
  tstate.states = fs7;
  tstate.statecount = 2;
  tstate.userdata.freq = 0;
  r = dat_find_states(datrie, &tstate);
  CHECK("check find states7", r == 2 && tstate.userdata.freq == 1);
  tstate.states = fs8;
  tstate.statecount = 1;
  tstate.userdata.freq = 0;
  r = dat_find_states(datrie, &tstate);
  CHECK("check find states8", r == 1 && tstate.userdata.freq == 0);
  tstate.states = fs9;
  tstate.statecount = 3;
  tstate.userdata.freq = 0;
  r = dat_find_states(datrie, &tstate);
  CHECK("check find states9", r == 0 && tstate.userdata.freq == 0);
  tstate.states = fs10;
  tstate.statecount = 3;
  tstate.userdata.freq = 0;
  r = dat_find_states(datrie, &tstate);
  CHECK("check find states10", r == 0 && tstate.userdata.freq == 0);
  

  clear_datrie(datrie);
  clear_trie(trie);
}

void test_datrie()
{
  struct datrietree_s* datrie;
  struct userdata_s userdata;
  int r;
  enum word_encode encode = utf8_short;
  
  datrie = makeDatrieTree(encode, NULL);
  userdata.freq = 1;
  addWord(datrie, "中国人", &userdata, encode);
  userdata.freq = 2;
  addWord(datrie, "你好啊", &userdata, encode);
  userdata.freq = 3;
  addWord(datrie, "口碑网", &userdata, encode);
  userdata.freq = 4;
  addWord(datrie, "阿里巴巴", &userdata, encode);
  userdata.freq = 5;
  addWord(datrie, "阿里人", &userdata, encode);
  userdata.freq = 6;
  addWord(datrie, "垂直搜索", &userdata, encode);
  userdata.freq = 7;
  addWord(datrie, "垂直房产", &userdata, encode);
  userdata.freq = 8;
  addWord(datrie, "百度", &userdata, encode);
  userdata.freq = 9;
  addWord(datrie, "baby", &userdata, encode);
  userdata.freq = 10;
  addWord(datrie, "百度", &userdata, encode);
  
  buildDatrie(datrie, 2, 0);

  r = findWord(datrie, "中国人", &userdata, encode);
  CHECK("check find 中国人", r == 2 && userdata.freq == 1);
  r = findWord(datrie, "你好啊", &userdata, encode);
  CHECK("check find 你好啊", r == 2 && userdata.freq == 2);
  r = findWord(datrie, "口碑网", &userdata, encode);
  CHECK("check find 口碑网", r == 2 && userdata.freq == 3);
  r = findWord(datrie, "阿里巴巴", &userdata, encode);
  CHECK("check find 阿里巴巴", r == 2 && userdata.freq == 4);
  r = findWord(datrie, "阿里人", &userdata, encode);
  CHECK("check find 阿里人", r == 2 && userdata.freq== 5);
  r = findWord(datrie, "垂直搜索", &userdata, encode);
  CHECK("check find 垂直搜索", r == 2 && userdata.freq == 6);
  r = findWord(datrie, "垂直房产", &userdata, encode);
  CHECK("check find 垂直房产", r == 2 && userdata.freq == 7);
  r = findWord(datrie, "百度", &userdata, encode);
  CHECK("check find 百度", r == 2 && userdata.freq == 8);
  r = findWord(datrie, "baby", &userdata, encode);
  CHECK("check find baby", r == 2 && userdata.freq == 9);
  r = findWord(datrie, "百度贴吧", &userdata, encode);
  CHECK("check find 百度贴吧", r == 0 && userdata.freq == 0);
  r = findWord(datrie, "百", &userdata, encode);
  CHECK("check find 百", r == 1 && userdata.freq == 0);
  r = findWord(datrie, "电费单", &userdata, encode);
  CHECK("check find 电费单", r == 0 && userdata.freq == 0);
  r = findWord(datrie, "垂直", &userdata, encode);
  CHECK("check find 垂直", r == 1 && userdata.freq == 0);
  r = findWord(datrie, "阿里人民", &userdata, encode);
  CHECK("check find 阿里人民", r == 0 && userdata.freq == 0);
  r = findWord(datrie, "口碑网络", &userdata, encode);
  CHECK("check find 口碑网络", r == 0 && userdata.freq == 0);
  r = findWord(datrie, "房产", &userdata, encode);
  CHECK("check find 房产", r == 0 && userdata.freq == 0);
  r = findWord(datrie, "垂询房产", &userdata, encode);
  CHECK("check find 垂询房产", r == 0 && userdata.freq == 0);
  r = findWord(datrie, "bab", &userdata, encode);
  CHECK("check find bab", r == 1 && userdata.freq == 0);
  r = findWord(datrie, "babye", &userdata, encode);
  CHECK("check find babye", r == 0 && userdata.freq == 0);
  r = findWord(datrie, "cabye", &userdata, encode);
  CHECK("check find cabye", r == 0 && userdata.freq == 0);
  r = findWord(datrie, "BABYE", &userdata, encode);
  CHECK("check find BABYE", r == 0 && userdata.freq == 0);
  r = findWord(datrie, "Babye", &userdata, encode);
  CHECK("check find Babye", r == 0 && userdata.freq == 0);
  clearDatrieTree(datrie);
}


int main(int argv, char** argc)
{
  check_init();
  check_start("clib -- libdatrie -- trie");
  test_trie_tree_states();
  test_datrie_states();
  test_datrie();
  check_end();
  check_close();
  return 0;
}
