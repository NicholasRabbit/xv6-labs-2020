#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int display_names(char *path, char *name)
{
	//char buff[512], *p;
	int fd;
	//struct dirent de;
	struct stat st;
	
	// Open the current directory and get its file descriptor.
	fd = open(path, 0);

	// "fstat" is a system call populating infomation about an open
	// file into *st.
	fstat(fd, &st);

	printf("st.size = %d\n", st.size);


	return 0;
}

int main(int argc, char *argv[])
{
		if (argc < 2) {
			fprintf(2, "find: without the name of file.");
			exit(2);
		}
		if (argc > 2) {
			fprintf(2, "find: No more than two arguments, please");
	}

	// For now, we find current directory by default. 
	display_names(".", argv[1]);


	return 0;
}
