#include "kernel/types.h"
#include "user/user.h"

#define N 34
#define SIZE_INT 4	// size of int


// Wrong impletation.
int loop_proc(int p[]) 
{

	int m, k, pid;

	read(p[0], &m, sizeof(int));
	printf("pid %d --> %d\n", getpid(), m);
	while (read(p[0], &k, sizeof(int))) {
		// Drop multiples of m.
		if (k % m == 0)
			continue;
		// Feed numbers to the right neighbour.
		int p2[2];
		pipe(p2);
		pid = fork();
		// The child is the parent of the newly created grandchild process.
		if (pid > 0) {
			// Write non-multiplied numbers to the right neighbour.
			write(p2[1], &k, SIZE_INT);
		} else if (pid == 0) {
			// The grandchild process.
			read(p2[0], &m, SIZE_INT);
			printf("pid %d --> %d\n", getpid(), m);
		}

	}
	return 0;
}


void sieve(int fds[])
{
	// close the write end of a pipe in a child.
	// The reason both child and parent have file descriptors refering to the pipe, 
	// therefore, we must close the write end both in parent and child so that the 
	// read end won't block. 
	close(fds[1]);

	int prime, np[2], next, pid;
	// Read the first "p": prime.
	int count = read(fds[0], &prime, SIZE_INT);
	if (count == 0)
		exit(0);
	printf("pid %d --> %d\n", getpid(), prime);
	
	// Create another pipe.
	pipe(np);
	// 
	pid = fork();

	if (pid > 0) {
		// The current child is a new parent prcess.
		// Close the read end at the write side.
		close(np[0]);

		// Keep on writing numbers to the write end.
		while (read(fds[0], &next, SIZE_INT)) {
			// Drop multiples of the first prime.
			if (next % prime == 0)
				continue;
			write(np[1], &next, SIZE_INT);
		}
		// After reading "fds", close it.
		close(fds[0]);

		// Close the write end after the while loop; Wrinting to a pipe is finished. 
		close(np[1]);

		wait(0);

	} else if (pid == 0) {
		// Newly created child process.
		close(np[1]);
		sieve(np);
	}
	
	// Actually, all of the processes are connected by pipes with the recursive "sieve".

	exit(0);

}



int main(int argc, char *argv[])
{
	int n, pid;

	int p[2];
	// Create a pipe.
	pipe(p);

	// Create a child process.
	pid = fork();

	// The parent process.
	if (pid > 0) {
		// Close the read end of a pipe since parent only needs to write. 
		close(p[0]);

		for (n = 2; n < 35; n++) {
			write(p[1], &n,  sizeof(int));
		}
		// When writing ends in the for loop, close the write end so that the read end won't block.
		close(p[1]);

		wait(0);

	} else if (pid == 0) {
	// A child process.

		// Wrong!
		//loop_proc(p);

		sieve(p);

		exit(0);
	} else {
		fprintf(2, "fork error!");
	}

	exit(0);
}
