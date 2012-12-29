#include <stdlib.h>
#include <stdio.h>
#include <fdict/check.h>

static int check_cout = 0;
static int failed_count = 0;
static int success_count = 0;

void check_start(const char* info)
{
  check_cout = 0;
  failed_count = 0;
  success_count = 0;
  printf("check [%s]\n", info);
}


void check(const char* info, int b, const char* file, int line)
{
  check_cout++;
  if (b == 0)
    {
      printf("%s failed %s:%d\n", info, file, line);
      failed_count++;
    }
  else
    {    
      printf("%s success\n", info);
      success_count++;
    }
  
}

void check_end()
{
  printf("check count:%d\tsucess:%d\tfailed:%d\n", check_cout, success_count, failed_count);
}

