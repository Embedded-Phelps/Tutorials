#include <sys/types.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define BUFFER_SIZE (256)

int main(int argc, char const *argv[])
{
  int pid = getpid();
  int uid = getuid();
  time_t t = time(NULL);
  struct tm time = *localtime(&t);
  int32_t presort[BUFFER_SIZE];
  int32_t aftersort_1[BUFFER_SIZE];
  long syscall_ret;

  for (int32_t i = 0; i<BUFFER_SIZE; i++)
  {
    presort[i] = rand();
  }

  syscall_ret = syscall(333, presort, BUFFER_SIZE, aftersort_1);

  printf("Current Process ID: %d\n", pid);
  printf("Current User ID: %d\n", uid);
  printf("Current Date: %02d/%02d/%04d\n",
         time.tm_mday, time.tm_mon+1, time.tm_year+1900);
  printf("Current Time: %02d:%02d:%02d\n",
         time.tm_hour, time.tm_min, time.tm_sec);

  if(syscall_ret == 0)
  {
    printf("System Call: 'shuting_sort' executed correctly.\n ");
  }
  else
    printf("System Call: 'shuting_sort' failed, return codes: %ld.\n", syscall_ret);
  return 0;
}
