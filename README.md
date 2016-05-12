# LCDD
Linux character device driver for OS class

To compile the module using the Makefile the paths must be changed in the make command to match your setup

Running make produces the kernel object file (.ko) which can then be installed by running
  sudo insmod lcdd.ko 
  
The module can be removed by running
  sudo rmmod lcdd.ko
  
Running dmesg will show printk messages to see what's going on in the kernel

To run the test program after compiling, call ./lcddtest with the desired message as the argument
