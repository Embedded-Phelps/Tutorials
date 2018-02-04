# Create a Kernel Module

lkm_example.c     - Hello world example of create a kernel module
lkm_timer.c       - Source code of my kernel module
Makefile          - Makefile to build the kernel module

### Build the kernel module
  make

### Install the kernel module
  sudo insmod lkm_timer.ko count=0 name="Shuting"

### Check installation result and kernel module info
  lsmod | grep lkm_timer
  modinfo lkm_timer.ko

### Remove the kernel module
  rmmod lkm_timer.ko

### Check kernel log
  sudo dmesg
