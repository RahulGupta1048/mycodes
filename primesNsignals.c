/*
 *This code generates all the prime numbers upto a given "Max" argument and stores
 *them into a file at "~/path/to/storing/list", which is passed in the argument.
 *If signals SIGINT or SIGABRT interrupts the generation, it stores
 *the already generated prime numbers in the aformentioned file and exits the process.
 *To execute : $ ./generatePrime ~/path/to/storing/list MaxValue
 */


#define _POSIX_C_SOURCE 199309L
#define _GNU_SOURCE
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
#include "functions.h"

int p[9999999]={0};
static volatile sig_atomic_t sigCaught = 0;


/* Function to print prime numbers to a file */
int printToFile(char* path)
{
	int i, fd2, s2;
	int* buf1; 

	fd2 = open(path, O_WRONLY|O_CREAT|O_TRUNC, 0666);
	if(fd2 == -1) {
	 	perror("could not create destination file");
	 	_exit(EXIT_FAILURE);
	 	}
	if((buf1 = (int*)malloc(sizeof(int))) == NULL) {
		perror("cannot allocate memory");
		if(close(fd2) == -1)
	 		perror("couldn't close destination file");
	 	_exit(EXIT_FAILURE);
		}
	
	i = 0;
	while(p[i] > 0) {
		*buf1 = p[i];
		if((s2 = write(fd2, buf1, sizeof(int))) == -1) {
			free(buf1);
			perror("cannot write into destination file");
			if(close(fd2) == -1)
	 			perror("couldn't close destination file");
			_exit(EXIT_FAILURE);
			}
		i++;
		}
	free(buf1);
	if(close(fd2) == -1){
	 	perror("couldn't close destination file");
	 	_exit(EXIT_FAILURE);
	 	}
	return 0; 
}


/* Signal handler*/
static void handler(int sig)
{

sigCaught = 1;

}


/* prime generator Function */
int primeGenerator(int y)
{
	int i, temp, flag, lim, k;
	p[0] = 2;
	k = 1;
	  
	for(i = 3; i <= y; i++){
		if(sigCaught)
			return -1;
		temp = 0;
		flag = 0; 
		lim = sqrt(i);
		while(p[temp] <= lim) {
			if(i % p[temp] == 0) {
				flag = 1;
				break;
				}
			temp++;
			} 
		if(flag == 0) {
			p[k] = i;
			k++;
			}
		} 
	return 0;
}


int main(int argc, char** argv)
{
	sigset_t sigset;
	sigset_t sigsetTemp;
	struct sigaction  sact;
	char* endptr;
	int max;

	sigfillset(&sigset);
	sigdelset(&sigset, SIGHUP);
	sigdelset(&sigset, SIGINT);
	sigdelset(&sigset, SIGQUIT);
	sigdelset(&sigset, SIGABRT);
	sigdelset(&sigset, SIGTERM);
	sigdelset(&sigset, SIGSTOP);
	sigdelset(&sigset, SIGTSTP);
	sigfillset(&sigsetTemp);

	if(sigprocmask(SIG_SETMASK, &sigset, NULL) == -1) {
		perror("couldn't establish process mask");
		exit(EXIT_FAILURE);
		}

	sact.sa_flags = 0;
	sact.sa_handler = handler;
	sact.sa_mask = sigset;
	sigaction(SIGINT, &sact, NULL);
	sigaction(SIGABRT, &sact, NULL);

	if(argc < 3) {
		max = 10000000;
		}
	else {
		max = strtol(argv[2], &endptr, 0);
		if(*endptr)
		max = 10000000;
		}
	if(primeGenerator(max) == -1) 
		perror("couldn't complete generation");
	if(sigprocmask(SIG_SETMASK, &sigsetTemp, NULL) == -1)
		perror("couldn't establish temporary process mask");
	printToFile(FILE);
	if(sigprocmask(SIG_SETMASK, &sigset, NULL) == -1)
		perror("couldn't restore process mask");

	readFromFile(FILE);
}

