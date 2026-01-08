/*
 * copy input to output
 * Since the console are the default input and output of a process,
 * this program read the arguments input by a user in a console and
 * print it to the same console. 
 */ 

#include "kernel/types.h"
#include "user/user.h"


int main(int argc, char *argv[])
{

	char buf[64];

	while (1) {
		// 0 is the file descriptor for reading.
		// the console is attached to 0 by default. 
		int n = read(0, buf, sizeof(buf));	

		if (n <= 0)
			break;
		// 1 is the file descritpor for standard output.
		write(1, buf, n);
	}

	exit(0);

}
