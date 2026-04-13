#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
	int pid;
	int fds[2];
	int fds_b[2];
	char buff[2];

	// Note that a pair of pipes should be called outside the following
	// "if...else" because they are shared by a child process and its parent. 
	pipe(fds);
	pipe(fds_b);

	pid = fork();
	// After the above "fork()", there are two process executing the following code.

	// The parent process.
	if (pid > 0) {

		write(fds[1], "A", 1);

		// (1) Wait for a child process to exit.
		// If a parent process doesn't wait, the following code will be run simultaneously, 
		// therefore, it might not receive the message "B" from its child process. 
		// (2) Whereas, reading from a pipe holds the parent process if the child process doesn't
		// exit.
		wait(0);
		
		read(fds_b[0], buff, 1);  // (2) read from a pipe.
		if (buff[0] == 'B') {
				// Get the parent process ID.
				pid = getpid();
				printf("%d: received pong\n", pid);
		}
		exit(0);

	} else if (pid == 0) {
		// A child process reads a byte from a pipe and stores the data to "buff".
		read(fds[0], buff, 1);

		if (buff[0] == 'A') {

			// If a child received "A" from its parent, it writes the "A" 
			// into its file descriptor. Since the default output of a process 
			// is a console, so the "A" will be printed on the CLI.
			//write(1, buff, 1); // To test if the 'A' is output.

			// Get the child process ID.
			pid = getpid();
			printf("%d: received ping\n", pid);

			// The child should write something such as "B" back the a pipe so that its parent can
			// read from the pipe.
			write(fds_b[1], "B", 1);
			exit(0);
		}

		exit(1);

	} else {
		exit(-1);
		printf("fork error!");
	}
	
	exit(0);

}
