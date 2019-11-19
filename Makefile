#This is a common Makefile for codes in mycodes github repository
ALL := copyFileContents generatePrime
ALL_DBG := copyFileContents_dbg generatePrime_dbg

CC=gcc
CFLAGS= -O2 -Wall
CFLAGS_DBG= -g -ggdb -O0 -Wall -Wextra -DDEBUG

LINKIN := -lm

all: ${ALL}
all_debug: ${ALL_DBG}

#--- Target :: copyFileContents
copyFileContents.o: copyFileContents.c
	${CC} ${CFLAGS} -c copyFileContents.c -o copyFileContents.o

copyFileContents: copyFileContents.o 
	${CC} -o copyFileContents copyFileContents.o

copyFileContents_dbg.o: copyFileContents.c
	${CC} ${CFLAGS_DBG} -c copyFileContents.c -o copyFileContents_dbg.o

copyFileContents_dbg: copyFileContents_dbg.o 
	${CC} -o copyFileContents_dbg copyFileContents_dbg.o

#---Target :: generatePrime
readIntgsFromFile.o: readIntgsFromFile.c
	${CC} ${CFLAGS} -c readIntgsFromFile.c -o readIntgsFromFile.o

primesNsignals.o: primesNsignals.c
	${CC} ${CFLAGS} -c primesNsignals.c -o primesNsignals.o

generatePrime: primesNsignals.o readIntgsFromFile.o
	${CC} -o generatePrime primesNsignals.o readIntgsFromFile.o ${LINKIN}

#---Target :: generatePrime_dbg
readIntgsFromFile_dbg.o: readIntgsFromFile.c
	${CC} ${CFLAGS_DBG} -c readIntgsFromFile.c -o readIntgsFromFile_dbg.o

primesNsignals_dbg.o: primesNsignals.c
	${CC} ${CFLAGS_DBG} -c primesNsignals.c -o primesNsignals_dbg.o

generatePrime_dbg: primesNsignals_dbg.o readIntgsFromFile_dbg.o
	${CC} -o generatePrime_dbg primesNsignals_dbg.o readIntgsFromFile_dbg.o ${LINKIN}


clean:
	rm -vf ${ALL} ${ALL_DBG} *.o *~;\
	rm -vf ${ALL_DBG} *.o *~
