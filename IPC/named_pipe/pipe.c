#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <semaphore.h>
#include "message.h"

int main(int argc, char * argv[])
{
  /* Create the pipe file */
  if (mknod("/tmp/pipefile", S_IFIFO | S_IRUSR | S_IWUSR, 0)==-1)
  {
    perror("mknod failed.\n");
    exit(EXIT_FAILURE);
  }

  pid_t pid =fork();
  /* Children process */
  if(pid==0)
  {
    sleep(3);
    msg_t cmsg;
    int cpipe;
    /* Open the pipe for read */
    if((cpipe = open("/tmp/pipefile", O_RDONLY))==-1)
    {
      perror("c: open failed\n");
      exit(EXIT_FAILURE);
    }
    /* Read out the message */
    if(read(cpipe, &cmsg, sizeof(cmsg))==-1)
    {
      perror("c: read failed\n");
      exit(EXIT_FAILURE);
    }
    /* Close the pipe */
    close(cpipe);
    process_msg(cmsg);
    /* Open the pipe again for write */
    if((cpipe = open("/tmp/pipefile", O_WRONLY))==-1)
    {
      perror("c: open failed\n");
      exit(EXIT_FAILURE);
    }
    cmsg.msg_id = MSG_ID_LED;
    cmsg.payload_len = 1;
    cmsg.msg_payload.led_ctl = LED_ON;
    /* Send out message via the pipe */
    write(cpipe, &cmsg, sizeof(cmsg));
    /* Close the pipe */
    close(cpipe);
  }
  else
  {
    msg_t pmsg;
    int ppipe;
    /* Open the pipe for write */
    if (( ppipe = open("/tmp/pipefile", O_WRONLY))==-1)
    {
      perror("p: open failed.\n");
      exit(EXIT_FAILURE);
    }
    pmsg.msg_id = MSG_ID_STRING;
    pmsg.payload_len = 3;
    sprintf(pmsg.msg_payload.string, "aha");
    /* Write the message onto the queue */
    if(write(ppipe, &pmsg, sizeof(pmsg))==-1)
    {
      perror("p: write failed\n");
      exit(EXIT_FAILURE);
    }
    /* Close the pipe */
    close(ppipe);
    sleep(6);
    /* Open the pipe again for read */
    if (( ppipe = open("/tmp/pipefile", O_RDONLY))==-1)
    {
      perror("p: open failed.\n");
      exit(EXIT_FAILURE);
    }
    /* Read the message */
    if(read(ppipe, &pmsg, sizeof(pmsg))==-1)
    {
      perror("p: read failed.\n");
      exit(EXIT_FAILURE);
    }
    process_msg(pmsg);
  }
  /* Unlink the pipe file */
  unlink("/tmp/pipefile");
  /* Exit success */
  exit(EXIT_SUCCESS);
}
