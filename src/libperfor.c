/*
 * libperfor
 *
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <sys/types.h>
#include <unistd.h>
#include <fdict/libperfor.h>

void out_sys_vm()
{  
  pid_t pid;
  char buff[1024];

  pid = getpid();
  sprintf(buff, "cat /proc/%d/status | grep Vm", pid);
  system(buff);
}
