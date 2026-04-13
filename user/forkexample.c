#include "kernel/types.h"
#include "user/user.h"

/*
 * This is a example code to verify whether there are two processes after "fork()".
 */

int main(int argc, char *argv[])
{
	// 1. Create a child process in fork(). Then there are two process: the parent itself and 
	// the child process created by this parent after the "fork()" is called.
	int pid = fork();

	// 2. These two processes will simultaneously execute either of two branches 
	// of the following "if...else if...".
	if (pid > 0) {
		printf("parent: child %d\n", pid);
		// Note that "(int *) 0" is a pointer with the value of 0.
		pid = wait((int *) 0);
		printf("child is done\n", pid);
	} else if (pid == 0) {
		printf("child: exiting\n");
		exit(0);
	} else {
		exit(-1);
		printf("fork error!");
	}

	exit(0);

}
