#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define BUFFER_SIZE (256)

void print_array(int * arr, size_t n)
{
    for(int i=0; i<n; i++)
    {
      printf("%d ", *(arr+i));
    }
    printf("\n");
}

int main(int argc, char const *argv[])
{
  int32_t presort[BUFFER_SIZE];
  int32_t aftersort_1[BUFFER_SIZE];
  int32_t * aftersort_2 = NULL;
  struct timespec tstart={0,0}, tend={0,0};

  long syscall_ret;

  /* Populate the buffer */
  for (int32_t i = 0; i<BUFFER_SIZE; i++)
  {
    presort[i] = rand();
  }

  printf("[test 1] Invoking 'shuting_sort' system call with valid arguments.\n");
  /* Get system call execution time */
  clock_gettime(CLOCK_MONOTONIC, &tstart);
  syscall_ret = syscall(333, presort, BUFFER_SIZE, aftersort_1);
  clock_gettime(CLOCK_MONOTONIC, &tend);
  /* Check execution result */
  if(syscall_ret == 0)
  {
    printf("[test 1] System call 'shuting_sort' executed correctly.\n");
    printf("[test 1] System call 'shuting_sort' execution time: %.3f msec.\n",
           (1000*(double)tend.tv_sec+0.001*tend.tv_nsec) -
           (1000*(double)tstart.tv_sec+0.001*tstart.tv_nsec));
    //print_array(aftersort_1, BUFFER_SIZE);
  }
  else
    printf("[test 1] System call 'shuting_sort' failed, return codes: %ld.\n", syscall_ret);

  printf("[test 2] Invoking 'shuting_sort' system call with invalid arguments "
         " --- NULL destination address.\n");
  syscall_ret = syscall(333, presort, BUFFER_SIZE, aftersort_2);
  if(syscall_ret == 0)
  {
    printf("[test 2] System call 'shuting_sort' executed correctly.\n");
    //print_array(aftersort_2, BUFFER_SIZE);
  }
  else
    printf("[test 2] System call 'shuting_sort' failed, return codes: %ld.\n", syscall_ret);

  printf("Use dmesg to check for kernel messages.\n");
  return 0;
}
