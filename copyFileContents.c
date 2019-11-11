/* This code is used to copy contents of one file to another.
 * To run this code, compile it using   $gcc copyFileContents.c
 * To run it, use $ ./a.out Source_file_path Destination_file_path buffer_size
 * the buffer size is an integer value, which decides the speed of copy
 * If, buffer size is not provided, it takes default size of buffer, which is 1000 bytes
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv) {

	char* endptr;
	int fd1, fd2, l;
	size_t s1, s2;

	if (argc < 3) {
	 	perror("Enter source and destination file names");                     
 		exit(0);
 	}

	/*Obtaining buffer size from user, setting default to 1000 B, for wrong I/P*/
	if (argc == 3)
 	l = 1000;
 else {
 	l = strtol(argv[3], &endptr, 0);
 	if(*endptr)
 	l = 1000;
 	}
 	
 void* buf=malloc(l);

/* Opening source file descriptor */
 fd1 = open(argv[1], O_RDONLY);

 if(fd1 == -1){
 	perror("could not open source file");
 	exit(EXIT_FAILURE);
 	}

/* Opening/Creating destination file descriptor */ 
 fd2 = open(argv[2], O_WRONLY|O_CREAT, 0666);

 if(fd2 == -1){
 	perror("could not create destination file");
 	exit(EXIT_FAILURE);
 	}

/* Copying source contents to destination in chuncks of buffer size in loop */
 while((s1 = read(fd1, buf, l)) != 0) {
        if(s1 == -1){
 		perror("cannot read from source file");
 		exit(EXIT_FAILURE);
 		}
        s2 = write(fd2, buf, l);
 	if(s2 == -1){
 		perror("cannot write into destination file");
 		exit(EXIT_FAILURE);
 		}
	}

/* Closing source and destination files */
 if( close(fd1) == -1){
 	perror("couldn't close source file");
 	exit(EXIT_FAILURE);
 	}
 if( close(fd2) == -1){
 	perror("couldn't close destination file");
 	exit(EXIT_FAILURE);
 	} 
 free(buf);
}
3], &endptr, 0);
 	if(*endptr)
 	l = 1000;
 	}
 	
 void* buf=malloc(l);

/* Opening source file descriptor */
 fd1 = open(argv[1], O_RDONLY);

 if(fd1 == -1){
 