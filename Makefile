#This is a common Makefile for codes in mycodes github repository
ALL := copyFileContents generatePrime

CC=gcc
CFLAGS= -O2 -Wall

LINKIN := -lm

all: ${ALL}

#--- Target :: copyFileContents
copyFileContents.o: copyFileContents.c
	${CC} ${CFLAGS} -c copyFileContents.c -o copyFileContents.o

copyFileContents: copyFileContents.o 
	${CC} -o copyFileContents copyFileContents.o

#---Target :: generatePrime
readIntgsFromFile.o: readIntgsFromFile.c
	${CC} ${CFLAGS} -c readIntgsFromFile.c -o readIntgsFromFile.o

primesNsignals.o: primesNsignals.c
	${CC} ${CFLAGS} -c primesNsignals.c -o primesNsignals.o

generatePrime: primesNsignals.o readIntgsFromFile.o
	${CC} -o generatePrime primesNsignals.o readIntgsFromFile.o ${LINKIN}

clean:
	rm -vf ${ALL} *.o *~
