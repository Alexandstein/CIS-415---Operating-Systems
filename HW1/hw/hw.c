#include <linux/kernel.h>
asmlinkage long sys_hw(void){
	printk("Hey there! I am a system call!\n");
	return 0;
}