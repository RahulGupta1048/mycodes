/*
 *This code generates all the prime numbers upto a given "Max" argument and simultaneously stores
 *them into a file specified by the macro FILE, using a separate thread.
 *If signals SIGINT or SIGABRT interrupts the generation, it completes storing
 *the already generated prime numbers in the aformentioned file and exits the process.
 *To execute : $ ./generatePrime MaxValue
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
#include <pthread.h>
#include <time.h>
#include "functions.h"
#include <unistd.h>

unsigned int *p;
static volatile sig_atomic_t sigCaught = 0;
int generation_completed = 0;
pthread_rwlock_t lock;


/* Function to print prime numbers to a file */
void *printToFile(void *m)
{
	int i, fd2, s2, ret;
	unsigned int* buf1;
	unsigned int *max = (unsigned int*)m;
	char *path = FILE;
	useconds_t sleep_time = 100;

	printf("Max required = %u\n", *max);
	fd2 = open(path, O_WRONLY|O_CREAT|O_TRUNC, 0666);
	if(fd2 == -1) {
	 	perror("could not create destination file");
	 	_exit(EXIT_FAILURE);
	}
	if((buf1 = (unsigned int*)malloc(sizeof(unsigned int))) == NULL) {
		perror("cannot allocate memory");
		if(close(fd2) == -1)
			perror("couldn't close destination file");
		_exit(EXIT_FAILURE);
	}
	
	i = 0;
	while(1) {
		while(p[i] == 0) {
			ret = pthread_rwlock_rdlock(&lock);
			if(ret != 0) {
				perror("Failed to take lock\n");
				continue;
			}
			if(generation_completed){
				pthread_rwlock_unlock(&lock);
				goto completed_writing;
			}
			else {
				pthread_rwlock_unlock(&lock);
				usleep(sleep_time);
			}
		}
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
completed_writing:
	free(buf1);
	if(close(fd2) == -1){
	 	perror("couldn't close destination file");
	 	_exit(EXIT_FAILURE);
	 	}
	pthread_exit((void *)0);
}


/* Signal handler*/
static void handler(int sig)
{

sigCaught = 1;

}


/* prime generator Function */
int primeGenerator(unsigned int y)
{
	int i, temp, flag, lim, k;
	p[0] = 2;
	k = 1;
	  
	for(i = 3; i <= y; i++){
		if(sigCaught) {
			pthread_rwlock_wrlock(&lock);
			generation_completed = 1;
			pthread_rwlock_unlock(&lock);
			return -1;
		}
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
	pthread_rwlock_wrlock(&lock);
	generation_completed = 1;
	pthread_rwlock_unlock(&lock);
	return 0;
}


int main(int argc, char** argv)
{
	sigset_t sigset, sigsetTemp;
	struct sigaction  sact;
	char* endptr;
	unsigned int max;
	int ret;
	clock_t begin, end;
	double time_taken;
	pthread_t tid;

	p = (unsigned int*)malloc(100000000*sizeof(unsigned int));
	if(p == NULL) {
		printf("Couldn't allocate memory for array");
		return -1;
	}
	sigfillset(&sigset);
	sigdelset(&sigset, SIGINT);
	sigdelset(&sigset, SIGABRT);

	sigfillset(&sigsetTemp);

	ret = pthread_rwlock_init(&lock, NULL);
	if(ret != 0)
		perror("RWlock initialize ");

	if(sigprocmask(SIG_SETMASK, &sigset, NULL) == -1) {
		perror("couldn't establish process mask\n");
		exit(EXIT_FAILURE);
	}

	sact.sa_flags = 0;
	sact.sa_handler = handler;
	sact.sa_mask = sigset;
	sigaction(SIGINT, &sact, NULL);
	sigaction(SIGABRT, &sact, NULL);

	if(argc < 2)
		max = DEFAULT_MAX;

	else {
		max = strtoul(argv[1], &endptr, 0);
		if(*endptr)
		max = DEFAULT_MAX;
	}
	if(pthread_create(&tid, NULL, printToFile, &max) < 0)
		return -1;
	begin = clock();
	if(primeGenerator(max) == -1) 
		perror("couldn't complete generation");
	if(sigprocmask(SIG_SETMASK, &sigsetTemp, NULL) == -1)
		perror("couldn't establish temporary process mask");
	pthread_join(tid, NULL);
	end = clock();
	if(sigprocmask(SIG_SETMASK, &sigset, NULL) == -1)
		perror("couldn't restore process mask");
	time_taken = (double)(end - begin)/CLOCKS_PER_SEC;
	readFromFile(FILE);
	printf("\n Time taken = %f", time_taken);
	return 0;
}

