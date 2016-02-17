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
static struct lock *counter_lock = NULL;

int counterB = 0;

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
	//int ch = '0' + num;
	(void)num;
	//int counterB = 0;
	//Added lock here

	kprintf("\n going into for loop, It should be 10,000 when it exits\n");

	//kprintf("The value of the counter before the loop is: %d \n",counterB);
	for(int i = 0; i < 10000; i++) {
		//kprintf("this is going!");
		lock_acquire(counter_lock);
		counterB++;
		lock_release(counter_lock);
	}

	//kprintf("I should be out of the four loop\n");
        //kprintf("value of counter is: %d \n ", counterB);
          //Added lock here

	(void)junk;

	//putch(ch);

	//lock_destroy(counter_lock);

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

	kprintf("Value of counter after increment is: %d", counterB);
}

int
lockThreadCounter(int nargs, char *args[])
{
	(void)nargs;
	//(void)args;
	int input = *args[1] - 48;
	//int input = **args;

	init_sem();
	kprintf("Starting lock counter test ... \n");

	counter_lock = lock_create("counter_lock");

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
