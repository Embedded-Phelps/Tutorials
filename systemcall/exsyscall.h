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
 * @file dlinkedlist.h
 * @brief The header file for the implementation of my system call
 *
 * @author Shuting Guo
 * @date 02/3/2018
 * @version 1
 *
 */

#ifndef __EXSYSCALL_H__
#define __EXSYSCALL_H__

asmlinkage long sys_shuting_sort(int __user * src_ptr,
                                 size_t buff_size, int __user * dest_ptr);
#endif /* __EXSYSCALL_H__ */
