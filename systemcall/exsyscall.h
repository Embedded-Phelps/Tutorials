#ifndef __EXSYSCALL_H__
#define __EXSYSCALL_H__

asmlinkage long sys_shuting_sort(int __user * src_ptr,
                                 size_t buff_size, int __user * dest_ptr);
#endif /* __EXSYSCALL_H__ */
