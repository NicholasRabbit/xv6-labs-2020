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
	
	// 1. Open the current directory and get its file descriptor.
	fd = open(path, 0);


	/*
	 * 2. "fstat" is a system call populating infomation about an open
	 * file into *st.
	 */ 
	fstat(fd, &st);
	
	/*
	 * 3. Let "p" deference to the end of the full path: buf.
	 * Then we can cancat the name of file to it. See 4.
	 */
	strcpy(buf, path);
	p = buf + strlen(buf);
	*p++ = '/'; 

	while(read(fd, &de, sizeof(de)) == sizeof(de)) {

		if ((de.inum) == 0)
			continue;
		// 4. Concact the name of a file to the address of "p".
		memmove(p, de.name, DIRSIZ);
		p[DIRSIZ] = 0; // Add 0(NULL) to indicate the end of a string.
		stat(buf, &st);
		// 5. Note that the return value of 'strcmp(...)' is NOT 0 when two strings are NOT equal.
		// We need to find files with same type and name.
		if(st.type == T_FILE && !strcmp(file_name, p)) {
			printf("%s\n", buf);
		}

		// 6. Recursive directories, but not "." or "..".
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
