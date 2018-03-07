#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "message.h"

int main(int argc, char * argv[])
{
  int status;
  struct mq_attr attr;
  attr.mq_maxmsg = 10;
  attr.mq_msgsize = 100;

  pid_t pid = fork();
  if(pid==0)
  {
    msg_t msg;
    msg.msg_id = MSG_ID_LED;
    msg.payload_len = 1;
    msg.msg_payload.led_ctl = LED_ON;
    /* Open the queue */
    mqd_t mq =mq_open("/queue", O_CREAT | O_WRONLY, S_IWUSR, &attr);
    if (mq == -1)
    {
      perror("Open test queue failed.\n");
      exit(EXIT_FAILURE);
    }
    /* Send out message via the queue*/
    if(mq_send(mq, (char*)&msg, sizeof(msg), 0)==-1)
    {
      perror("mq_send failed.\n");
    }
    /* Close the queue */
    mq_close(mq);
    sleep(3);
    /* Open the queue again */
    mq =mq_open("/queue", O_CREAT | O_RDONLY, S_IRUSR, &attr);
    if (mq == -1)
    {
      perror("Open test queue failed.\n");
      exit(EXIT_FAILURE);
    }
    /* Receive message from task 2 */
    if(mq_receive(mq, (char*)&msg, 200, 0)==-1)
    {
      perror("mq_receive failed.\n");
    }
    else
      process_msg(msg);
    mq_close(mq);
  }
  /* parent process */
  else
  {
    msg_t pmsg;
    sleep(1);
    /* Open the queue */
    mqd_t pmq =mq_open("/queue", O_CREAT | O_RDWR, S_IWUSR | S_IRUSR, &attr);
    if (pmq == -1)
    {
      perror("Open test queue failed.\n");
      exit(EXIT_FAILURE);
    }
    /* Receive message */
    if(mq_receive(pmq, (char*)&pmsg, 200, 0)==-1)
    {
      perror("mq_receive failed.\n");
    }
    else
      process_msg(pmsg);

    /* Write message */
    pmsg.msg_id = MSG_ID_STRING;
    pmsg.payload_len = 3;
    sprintf(pmsg.msg_payload.string, "aha");
    if(mq_send(pmq, (char*)&pmsg, sizeof(pmsg), 0)==-1)
    {
      perror("mq_send failed.\n");
    }
    mq_close(pmq);
    waitpid(pid, &status, 0);
  }
  mq_unlink("/queue");

  /* Exit success */
  exit(EXIT_SUCCESS);
}
