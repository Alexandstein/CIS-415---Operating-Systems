#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#define __NR_hw 350

long int hw_syscall(void) {
    return syscall(__NR_hw);
}

int main() {
	printf("Output: %d\n", hw_syscall());
	return 0;
}