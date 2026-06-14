#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"


int main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(2, "Input at least one argument.\n");
		exit(2);
	
	} else if (argc > MAXARG) {
		fprintf(2, "Too many arguments. Input no more than 15.\n");
	}


	char buf[32];
	char *argv2[MAXARG];


		// Combine standard input and arguments.
		int i, j;  // Don't copy the argv[0], which is "xargs" itself.
		for (i = 0, j = 1; j < argc; j++, i++) {
			argv2[i] = argv[j];
		}

	
	// Read from standard input, such as from  a pipeline in `echo hello | xargs echo bye`. 
	// From the hints, we know that this progrm should read a character each time until
	// it encounters '\n'.
	char c;
	int k = 0;
	while (read(0, &c, sizeof c) > 0) {	// "read()" returns the length it reads from standard input, including the last '\0'.
		if (c == '\n') {
			buf[k++] = c;

			argv2[i] = buf;
			// "Use fork and exec to invoke a comand on each line of input." from hints
			int pid = fork();
			if (pid == 0) {
				exec(argv2[0], argv2);
				printf("exec failed");
				exit(0);
			} else if (pid > 0) {
				wait(0);
			} else {
				printf("fork error!");
			}

		} else {
			buf[k++] = c;		
		}
	}

	exit(0);

}
