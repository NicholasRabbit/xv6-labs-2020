#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
	int n, pid, c_pid;
	int fds[2];
	char buff[10];
	
	pipe(fds);
	c_pid = fork();
	if (c_pid == 0) {

		pid = getpid();
		write(fds[1], "ping\n", 5);
		printf("child's pid: %d\n", pid);

	} else {

		n = read(fds[0], buff, sizeof(buff));
		write(1, buff, n);

		pid = getpid();

		printf("In parent process, child's pid: %d\n", c_pid);
		printf("parent's pid: %d\n", pid);
	}


	exit(0);

}
