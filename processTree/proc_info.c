#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/sched.h>
#include <linux/pid.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shuting Guo");
MODULE_DESCRIPTION("Print Kthreads information");
MODULE_VERSION("0.01");

/* Module input parameters */
static int process_id;
module_param(process_id, int, 0);
MODULE_PARM_DESC(process_id, "Process ID");

char process_state[3][15] = {"UNRUNNABLE", "RUNNABLE", "STOPPED"};

char * interpret_proc_state(long state)
{
  char * s;
  if(state==-1)
    s = process_state[0];
  else if(state==0)
    s = process_state[1];
  else
    s = process_state[2];
  return s;
}

void print_proc_info(struct task_struct * task)
{
  int num_children = 0;
  struct task_struct * temp = task;
  struct list_head * list;

  /* Get number of children */
  list_for_each(list, &temp->children)
  {
    num_children++;
  }
  /* Print process information */
  printk(KERN_INFO "%s[%d] \t\t| status: %s\t| number of children: %d\t| nice value: %d\n",
         task->comm, task->pid, interpret_proc_state(task->state), num_children, task_nice(task));
}

/* Module init function */
static int __init proc_info_init(void)
{
  struct task_struct *task;
  printk(KERN_INFO "Loading proc_info module.\n");
  if(process_id == 0)
    task = current;
  else
    task = pid_task(find_vpid(process_id), PIDTYPE_PID);

  print_proc_info(task);
  /* Traverse backward up the process tree and print process information */
  do
  {
      task = task->parent;
      print_proc_info(task);
  }while(task->pid != 0);

  return 0;
}

static void __exit proc_info_exit(void)
{
  printk(KERN_INFO "Removing proc_info module.\n");
}

module_init(proc_info_init);
module_exit(proc_info_exit);
