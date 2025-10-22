#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
	int pid, c_pid;
	int fds[2];
	int fds_b[2];
	char buff[2];
	
	pipe(fds);
	pipe(fds_b);

	c_pid = fork();
	if (c_pid == 0) {
		// A child process reads a byte from a pipe and stores the data to "buff".
		read(fds[0], buff, 1);

		if (buff[0] == 'A') {
			// If a child received "A" from its parent, it writes the "A" 
			// into its file descriptor. Since the default output of a process 
			// is a console, so the "A" will be printed on the CLI.
			//write(1, buff, 1); // To test.

			pid = getpid();
			printf("%d: received ping\n", pid);

			// The child should write "A" back the a pipe so that its parent can
			// read from the pipe.
			write(fds_b[1], "B", 1);
			exit(0);
		}

		exit(1);


	} else {

		write(fds[1], "A", 1);

		// Wait for a child process to exit.
		wait(0);
		
		read(fds_b[0], buff, 1);
		if (buff[0] == 'B') {
				pid = getpid();
				printf("%d: received pong\n", pid);
		}
		exit(0);

	}


	exit(0);

}
