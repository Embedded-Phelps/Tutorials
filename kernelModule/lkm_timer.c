#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/moduleparam.h>
#include <linux/stat.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shuting Guo");
MODULE_DESCRIPTION("A module example utilizing timer.");
MODULE_VERSION("1.0");

/* Module Input Parameters */
static long count;
module_param(count, long, 0);
MODULE_PARM_DESC(count, "timer count variable");

static char *name = "";
module_param(name, charp, 0);
MODULE_PARM_DESC(name, "my name string");

/* Timer interrupt interval - 500ms */
static int k_time_interval = 500;
/* A timer instance */
static struct timer_list k_timer;

/* Timer interrupt handler */
void timer_handler(unsigned long data)
{
  /* Keeping count */
  count++;
  /* Restoring timer */
  mod_timer(&k_timer, jiffies + msecs_to_jiffies(k_time_interval));
  /* Log periodic information */
  printk(KERN_INFO "[%s] - Timer count = %ld\n", name, count);
}

/* Module init function */
static int __init timer_init(void)
{
  printk(KERN_INFO "lkm_timer module installed in kernel.\n");
  printk(KERN_INFO "Name: %s\n", name);
  printk(KERN_INFO "Initial timer count: %ld\n", count);
  /* Start the timer */
  setup_timer(&k_timer, timer_handler, 0);
  mod_timer(&k_timer, jiffies + msecs_to_jiffies(k_time_interval));
  return 0;
}

/* Module exit function */
static void __exit timer_exit(void)
{
  /* Delete the timer */
  del_timer(&k_timer);
  printk(KERN_INFO "lkm_timer module removed from kernel.\n");
}

/* Register module init and exit functions */
module_init(timer_init);
module_exit(timer_exit);
