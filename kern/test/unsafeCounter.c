/*
 * Thread test code.
 */
#include <types.h>
#include <lib.h>
#include <thread.h>
#include <synch.h>
#include <test.h>

#define NTHREADS  8

static struct semaphore *tsem = NULL;

static
void
init_sem(void)
{
	if (tsem==NULL) {
		tsem = sem_create("tsem", 0);
		if (tsem == NULL) {
			panic("threadtest: sem_create failed\n");
		}
	}
}
/*
static
void
loudthread(void *junk, unsigned long num)
{
	int ch = '0' + num;
	int i;

	(void)junk;

	for (i=0; i<120; i++) {
		putch(ch);
	}
	V(tsem);
}
*/
/*
 * The idea with this is that you should see
 *
 *   01234567 <pause> 01234567
 *
 * (possibly with the numbers in different orders)
 *
 * The delay loop is supposed to be long enough that it should be clear
 * if either timeslicing or the scheduler is not working right.
 */

/*
static
void
quietthread(void *junk, unsigned long num)
{
	int ch = '0' + num;
	volatile int i;

	(void)junk;

	putch(ch);
	for (i=0; i<200000; i++);
	putch(ch);

	V(tsem);
}
*/

static
void
threadfunSet(void *junk, unsigned long num) 
{
	int ch = '0' + num;
	
	(void)junk;

	putch(ch);

	V(tsem);
}

static
void
runthreads(int input)
{
	char name[16];
	int i, result;
	int numr = input;

	for (i=0; i< numr; i++) {
		snprintf(name, sizeof(name), "threadtest%d", i);
		result = thread_fork(name, NULL,
				     threadfunSet,
				     NULL, i);
		if (result) {
			panic("threadtest: thread_fork failed %s)\n", 
			      strerror(result));
		}
	}

	for (i=0; i< numr; i++) {
		P(tsem);
	}
}

int
unsafeThreadCounter(int nargs, char *args[])
{
	(void)nargs;
	//(void)args;
	int input = *args[1] - 48;

	init_sem();
	kprintf("Starting threadfun test ... \n");
	kprintf("\n It should print 0...10,000 \n");
	for(i = 0; i <= 10000; i++) {
		kprintf(i);
	}
	runthreads(input);
	kprintf("\nThread test done. \n");

	return 0;
}


/*
int
threadtest(int nargs, char **args)
{
	(void)nargs;
	(void)args;

	init_sem();
	kprintf("Starting thread test...\n");
	runthreads(1);
	kprintf("\nThread test done.\n");

	return 0;
}

int
threadtest2(int nargs, char **args)
{
	(void)nargs;
	(void)args;

	init_sem();
	kprintf("Starting thread test 2...\n");
	runthreads(0);
	kprintf("\nThread test 2 done.\n");

	return 0;
}

*/
