#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
FILE* processDir(struct dirent *d)
{
	FILE *fd;
	struct dirent *de;
	
	
	int pid;
			pid = atoi(d->d_name);
			if (pid < 1) return NULL;

	char buffer[1024];
	sprintf(buffer, "/proc/%s/status",d->d_name);
	
	fd= fopen(buffer, "r");
	return fd;
}


char* getProcessName(char *buffer) 
{
	char name[6];
	memcpy(name, &buffer[0], 5);
	name[5] ='\0';
	int len=strlen(buffer);
	char compare[6]= "Name:";
	strcat(compare, "\0");
	if (strcmp(compare, name) == 0) {
		len = len - 7;
		char *result = malloc(len+1);
		memcpy(result, &buffer[6], len);
		strcat(result,"\0");
		return result;
	}
	return NULL;
}

int main(int argc, char *argv[])
{
	FILE* fd;
	struct dirent *de;
	DIR *dir;
	ssize_t numRead;
	ssize_t len=0;
	dir = opendir("/proc");
	while ( (de = readdir(dir)) != NULL  ) {
		if (de != NULL ) {
			fd = processDir(de);
		       char *buffer;
		       if (fd != NULL) {
		       while ( (numRead = getline(&buffer, &len, fd)  != -1 )) {
		       		char *processName;
				processName = getProcessName( buffer);
				if ( processName != NULL)
				{
					printf("%s\n", processName);
					free(processName);
				}

		       }


		       fclose(fd);
		       }
		}
	}




}
