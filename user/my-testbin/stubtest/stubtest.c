/*
 * An example program.
 */
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <err.h>

int
main()
{
	printf("Beggining Stubtest... \n");

	static volatile int mypid,anotherpid;
	mypid = getpid();
	printf("pid: %d \n",mypid);

	int status;
	anotherpid = 5;
	printf("waitpid: %d \n",waitpid(anotherpid,&status, 0));

	pid_t pid;
	pid = fork();
	if(pid > 0) {
	  printf("P...I'm a parent \n");
	} else {
	  printf("C...I'm a child \n");
	}
	//printf("pid: %d \n",pid);

	_exit(0);
	//return 0; /* avoid compiler warnings */
}
