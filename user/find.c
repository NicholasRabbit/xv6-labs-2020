#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int find(char *path, char *file_name)
{
	char buf[512], *p;
	int fd;
	struct dirent de;
	struct stat st;
	
	// Open the current directory and get its file descriptor.
	fd = open(path, 0);


	/*
	 * "fstat" is a system call populating infomation about an open
	 * file into *st.
	 */ 
	fstat(fd, &st);
	
	strcpy(buf, path);
	p = buf + strlen(buf);
	*p++ = '/'; 

	while(read(fd, &de, sizeof(de)) == sizeof(de)) {

		if ((de.inum) == 0)
			continue;
		// Concact the name of a file to the address of "p".
		memmove(p, de.name, DIRSIZ);
		p[DIRSIZ] = 0; // Add 0(NULL) to indicate the end of a string.
		stat(buf, &st);
		// Note that the return value is NOT 0 when two strings are NOT equal.
		// And we only need to find files.
		if(st.type == T_FILE && !strcmp(file_name, p)) {
			printf("%s\n", buf);
		}

		// Recursive directories, but not "." or "..".
		if(st.type == T_DIR && strcmp(".", p) && strcmp("..", p)) {
			find(buf, file_name);
		}


		
	
	}

	close(fd);

	exit(0);


	return 0;
}

int main(int argc, char *argv[])
{
	if (argc < 3) {
		fprintf(2, "find: without the name of a file.");
		exit(2);
	}
	if (argc > 3) {
		fprintf(2, "find . file_name : No more than two arguments, please");
	}

	// For now, we find current directory by default. 
	find(argv[1], argv[2]);


	return 0;
}
