#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[]) 
{
	// Handling the error of illegal arguments
	if (argc != 2) {
		printf("Illegal arguments. It shoule be like: sleep 30\n");
		exit(-1);
	}
	
	// The format of the command: argv = {"name of an instruction", "argv"}
	// An example: argv = {"sleep", "3"}
	int num_of_ticks = atoi(argv[1]);  // cast a string data to an integer.
	// call the system's function 'sleep(...)'.
	sleep(num_of_ticks);

	exit(0);
}
