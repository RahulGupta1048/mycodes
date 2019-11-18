#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>


void readFromFile(char *path)
{ 	
	int fd1, s1;
	int* buf1;

	fd1 = open(path, O_RDONLY);
	if(fd1 == -1){
	 	perror("could not open source file");
	 	exit(EXIT_FAILURE);
	 	}
	if((buf1 = (int*)malloc(sizeof(int))) == NULL) {
		perror("cannot allocate memory");
		if(close(fd1) == -1)
	 		perror("couldn't close source file");
	 	_exit(EXIT_FAILURE);
		}
	
	while((s1 = read(fd1, buf1, sizeof(int))) != 0) {
			if(s1 == -1) {
				perror("cannot read from source file");
				free(buf1);
				if(close(fd1) == -1)
	 				perror("couldn't close source file"); 	
				exit(EXIT_FAILURE);
				}
			printf("%d ",*buf1);
		}

	free(buf1);
	if(close(fd1) == -1)
		perror("couldn't close source file"); 	
				
}

