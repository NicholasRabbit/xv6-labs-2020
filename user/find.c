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
		// 4. Note that the return value of 'strcmp(...)' is 0 when two strings are equal.
		if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) 
			continue;

		// 5. Concact the name of a file to the address of "p".
		memmove(p, de.name, DIRSIZ);
		p[DIRSIZ] = 0; // Add 0(NULL) to indicate the end of a string.
		stat(buf, &st);

		// We need to find files with same type and name.
		if(st.type == T_FILE && strcmp(file_name, p) == 0) {
			printf("%s\n", buf);
		} else if (st.type == T_DIR ) {
			// 6. Recursive directories, but not "." or "..".
			find(buf, file_name);
		}

	
	}

	/*
	 * exit(...) should not be written here because it will ternimate the current
     * process so that the following directories or file will NOT be read. 
	 */ 
	// exit(0);

	close(fd);


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
		exit(2);
	}

	// For now, we find current directory by default. 
	find(argv[1], argv[2]);

	exit(0);

}
