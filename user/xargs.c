#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

#define MAXARG 16 


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

	int pid = fork();

	if (pid > 0) {
		// The parent process:
		wait((int *)0);
	} else if (pid == 0) {

		read(0, buf, sizeof buf);


		int i = 0;
		while ((argv2[i] = argv[i]) != 0)
			++i;	

		argv2[i++] = buf;
		argv2[i] = '\0'; 

		printf("argv2 %s\n", argv2[1]);

		exec(argv2[1], argv2 + 1);
		// One child process exits. 
		exit(0);
	}

	exit(0);
}
