#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int find(char *path, char *name)
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
		// Note that the return value is NOT 0 when two strings are NOT equal.
		if(!strcmp(name, p))
			printf("%s\n", p);
		
	
	}

	close(fd);

	exit(0);


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
	find(".", argv[1]);


	return 0;
}
