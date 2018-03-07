#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/stat.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/list.h>
#include <linux/kfifo.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shuting Guo");
MODULE_DESCRIPTION("kthread and kfifo example.");
MODULE_VERSION("1.0");

#define FIFO_SIZE (10)

static struct task_struct *ktask1 = NULL;
static struct task_struct *ktask2 = NULL;

/* info structure */
 struct proc_info
{
  int curr_id;
  u64 curr_vrt;
  int prev_id;
  u64 prev_vrt;
  int next_id;
  u64 next_vrt;
};

static struct kfifo myfifo;

/* kThread 1 task function */
static int task1(void * param)
{
  struct proc_info info;
  struct task_struct *temp;;

  while(!kthread_should_stop())
  {
    /* Populate the info structure */
    info.curr_id = current->pid;
    info.curr_vrt = current->se.vruntime;
    if((temp = list_prev_entry(current, tasks))!=NULL)
    {
      info.prev_id = temp->pid;
      info.prev_vrt = temp->se.vruntime;
    }
    else
    {
      info.prev_id = 0;
      info.prev_vrt = 0;
    }
    if((temp = list_next_entry(current, tasks))!=NULL)
    {
      info.next_id = temp->pid;
      info.next_vrt = temp->se.vruntime;
    }
    else
    {
      info.next_id = 0;
      info.next_vrt = 0;
    }

    /* Enqueue process info to the kfifo if the queue is not full */
    if(!kfifo_is_full(&myfifo))
      kfifo_in(&myfifo, &info, 1);
    /* sleep */
    ssleep(2);
  }
  printk(KERN_INFO "Task1 terminated.\n");
  return 0;
}

/* kThread 2 task function */
static int task2(void * param)
{
  struct proc_info info;
  while(!kthread_should_stop())
  {
    /* Dequeue if the kfifo is not empty */
    if(!kfifo_is_empty(&myfifo))
    {
      /* Dequeue info */
      kfifo_out(&myfifo, &info, 1);
      /* Print out information */
      printk(KERN_INFO "curr_task - pid(%d) | vruntime(%llu), \
                        prev_task - pid(%d) | vruntime(%llu), \
                        next_task - pid(%d) | vruntime(%llu)\n",
                        info.curr_id, info.curr_vrt,
                        info.prev_id, info.prev_vrt,
                        info.next_id, info.next_vrt);
    }
    ssleep(1);
  }
  printk(KERN_INFO "Task2 terminated.\n");
  return 0;
}

/* Module init function */
static int __init kthreads_init(void)
{
  printk(KERN_INFO "Loading lkm_kthread module.\n");
  /* Allocate memory for the fifo */
  if(kfifo_alloc(&myfifo, FIFO_SIZE, GFP_KERNEL))
  {
    printk(KERN_ERR "Error kfifo_alloc");
    return -1;
  }
  /* Create two kthreads */
  ktask1 = kthread_run(task1, NULL, "kthread1");
  ktask2 = kthread_run(task2, NULL, "kthread2");
  return 0;
}

/* Module exit function */
static void __exit kthreads_exit(void)
{
  printk(KERN_INFO "Removing lkm_kthread module.\n");

  /* Free the fifo */
  kfifo_free(&myfifo);
  /* Stop the two tasks */
  if(ktask1)
    kthread_stop(ktask1);
  if(ktask2)
    kthread_stop(ktask2);
}

module_init(kthreads_init);
module_exit(kthreads_exit);
