/*****************************************************************************
 * Copyright (C) 2018 by Shuting Guo
 *
 * Redistribution, modification or use of this software in source or binary
 * forms is permitted as long as the files maintain this copyright. Users are
 * permitted to modify this and use it to learn about the field of embedded
 * software. Shuting Guo is not liable for any misuse of this material.
 *
*****************************************************************************/
/**
 * @file exsyscall.c
 * @brief The source file for the Implementation of my own system call
 *
 * @author Shuting Guo
 * @date 02/3/2018
 * @version 1.0
 *
 */
#include <linux/kernel.h>
#include <linux/stddef.h>
#include <linux/syscalls.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/printk.h>
#include <linux/kern_levels.h>
#include <uapi/asm-generic/errno-base.h>
#include "exsyscall.h"

SYSCALL_DEFINE3(shuting_sort, int __user *, src_ptr,
                size_t, buff_size, int __user *, dest_ptr)
{
  long error = 0;
  int * ksrc_ptr = NULL;
  int i,j;

  printk(KERN_INFO "Entered shuting_sort system call.\n");

  /* Validate input parameters */
  if((buff_size > SIZE_MAX) || (buff_size == 0))
  {
    error = -EINVAL;
    printk(KERN_ERR "Buffer size is not valid, return code: %ld.\n", error);
    return error;
  }
  if((src_ptr == NULL) || (dest_ptr == NULL))
  {
    error = -EINVAL;
    printk(KERN_ERR "Invaild address argument(s), return code: %ld.\n", error);
    return error;
  }

  if((!access_ok(VERIFY_READ, src_ptr, sizeof(int)*buff_size)) ||
    (!access_ok(VERIFY_WRITE, dest_ptr, sizeof(int)*buff_size)))
  {
    error = -EFAULT;
    printk(KERN_ERR "Address write/read permission checking failed, "
           "return code: %ld.\n", error);
    return error;
  }

  printk(KERN_INFO "The buffer size is %zu.\n", buff_size);

  /* Allocate memeory in kernel space for the sorting */
  ksrc_ptr = kmalloc(sizeof(int)*buff_size, GFP_ATOMIC);
  if(ksrc_ptr == NULL)
  {
    error = -ENOMEM;
    printk(KERN_ERR "Kernel memory insufficient, return code: %ld.\n", error);
    return error;
  }
  /* Copy data from user space to kernel space */
  if (copy_from_user(ksrc_ptr, src_ptr, sizeof(int)*buff_size))
  {
    error = -EFAULT;
    printk(KERN_ERR "Invalid user-space source address, "
           "return code: %ld.\n", error);
    return error;
  }

  /* Implement the sorting with bubble sort algorithm*/
  printk(KERN_INFO "Sorting starts...\n");

  for(i=0; i<buff_size-1; i++)
  {
    for(j=0; j<buff_size-i-1; j++)
    {
      if(*(ksrc_ptr+j) < *(ksrc_ptr+j+1))
      {
        *(ksrc_ptr+j) = *(ksrc_ptr+j) ^ *(ksrc_ptr+j+1);
        *(ksrc_ptr+j+1) = *(ksrc_ptr+j) ^ *(ksrc_ptr+j+1);
        *(ksrc_ptr+j) = *(ksrc_ptr+j) ^ *(ksrc_ptr+j+1);
      }
    }
  }
  printk(KERN_INFO "Sorting done.\n");

  /* Copy the data to the destination address in user space */
  if (copy_to_user(dest_ptr, ksrc_ptr, sizeof(int)*buff_size))
  {
    error = -EFAULT;
    printk(KERN_ERR "Invalid user-space destination address, "
           "return code: %ld.\n", error);
    return error;
  }

  /* Free the memory allocated */
  kfree(ksrc_ptr);

  printk(KERN_INFO "Exit shuting_sort system call.\n");
  return error;
}
