 #include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

#define BUFFER_SIZE (10)
const int32_t aftersort_3[BUFFER_SIZE];

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
  int32_t aftersort_4[2];
  long syscall_ret;

  for (int32_t i = 0; i<BUFFER_SIZE; i++)
  {
    presort[i] = rand();
  }

  printf("[test 1] Involking 'shuting_sort' system call with valid arguments.\n");
  syscall_ret = syscall(333, presort, BUFFER_SIZE, aftersort_1);
  if(syscall_ret == 0)
  {
    printf("[test 1] System call 'shuting_sort' executed correctly.\n ");
    print_array(aftersort_1, BUFFER_SIZE);
  }
  else
  printf("[test 1] System call 'shuting_sort' failed, return codes: %ld.\n", syscall_ret);

  printf("[test 2] Involking 'shuting_sort' system call with invalid arguments "
         " --- NULL destination address.\n");
  syscall_ret = syscall(333, presort, BUFFER_SIZE, aftersort_2);
  if(syscall_ret == 0)
  {
    printf("[test 2] System call 'shuting_sort' executed correctly.\n ");
    print_array(aftersort_2, BUFFER_SIZE);
  }
  else
    printf("[test 2] System call 'shuting_sort' failed, return codes: %ld.\n", syscall_ret);

  printf("[test 3] Involking 'shuting_sort' system call with invalid arguments "
         " --- destination address is read-only.\n");
  syscall_ret = syscall(333, presort, BUFFER_SIZE, aftersort_3);
  if(syscall_ret == 0)
  {
    printf("[test 3] System call 'shuting_sort' executed correctly.\n ");
    print_array(aftersort_3, BUFFER_SIZE);
  }
  else
    printf("[test 3] System call 'shuting_sort' failed, return codes: %ld.\n", syscall_ret);

  printf("[test 4] Involking 'shuting_sort' system call with invalid arguments "
         " --- insufficient destination address.\n");
  syscall_ret = syscall(333, presort, BUFFER_SIZE, aftersort_4);
  if(syscall_ret == 0)
  {
    printf("[test 4] System call 'shuting_sort' executed correctly.\n ");
    print_array(aftersort_4, BUFFER_SIZE);
  }
  else
    printf("[test 4] System call 'shuting_sort' failed, return codes: %ld.\n", syscall_ret);

  printf("Use dmesg to check for kernel messages.\n");

  return 0;
}
