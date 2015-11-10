#include <types.h>
#include <lib.h>
#include <test.h> 

int usertest(int nargs, char *args[]) {
	int i = 0;
	for (i = 0; i < nargs; i++)
		kprintf("\n%s", args[i]);
	return 0;
}
