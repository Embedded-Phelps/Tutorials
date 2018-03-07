#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "message.h"

int main(int argc, char * argv[])
{
  pthread_mutexattr_t attributes;
  pthread_mutexattr_init(&attributes);
  pthread_mutexattr_setpshared(&attributes, PTHREAD_PROCESS_SHARED);

  /* Create shared memory */
  int handle = shm_open("/shm", O_CREAT | O_RDWR, 0777);
  /* Set the size of the shared memory segment */
  ftruncate(handle, 1000*sizeof(int));
  char * memory = (char *)mmap(0, 1000*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, handle, 0);

  /* Mutex share */
  pthread_mutex_t *mutex = (pthread_mutex_t*)memory;
  /* Initialize a mutex lock for shared memory protection */
  pthread_mutex_init(mutex, &attributes);
  pthread_mutexattr_destroy(&attributes);

  /* Variable share */
  /* Establish mapping between address space to memory object */
  msg_t * msg = (msg_t*)(memory+sizeof(pthread_mutex_t));

  pid_t pid = fork();
  /* Children process */
  if(pid==0)
  {
    pthread_mutex_lock(mutex);
    msg->msg_id = MSG_ID_LED;
    msg->payload_len = 1;
    msg->msg_payload.led_ctl = LED_ON;
    pthread_mutex_unlock(mutex);
    sleep(2);
    pthread_mutex_lock(mutex);
    msg_t *tempc = msg;
    pthread_mutex_unlock(mutex);
    process_msg(*tempc);
  }
  /* Parent process */
  else
  {
    sleep(1);
    pthread_mutex_lock(mutex);
    msg_t *tempp = msg;
    pthread_mutex_unlock(mutex);
    process_msg(*tempp);
    pthread_mutex_lock(mutex);
    msg->msg_id = MSG_ID_STRING;
    msg->payload_len = 3;
    sprintf(msg->msg_payload.string, "aha");
    pthread_mutex_unlock(mutex);
    int status;
    // wait for the cild process to finish
    waitpid(pid, &status, 0);
  }

  /* Unmap the memory object from the address space */
  munmap(memory, 1000*sizeof(int));
  /* Delete the share memory */
  shm_unlink("/shm");

  /* Exit success */
  exit(EXIT_SUCCESS);

}
