#the compiler used:gcc
CC = gcc
#the compilation flags
CFLAGS = -Wall -pedantic -ansi -g

assembler: assembler.o symboltable.o dataimage.o first_transition.o second_transition.o kelet.o files.o instExecution.o constants.o instructions.o
	$(CC) $(CFLAGS) -o assembler assembler.o symboltable.o dataimage.o first_transition.o second_transition.o kelet.o files.o instExecution.o constants.o instructions.o

assembler.o: assembler.c assembler.h symboltable.h dataimage.h first_transition.h second_transition.h
	$(CC) -c $(CFLAGS) -o assembler.o assembler.c
	
first_transition.o: first_transition.c first_transition.h symboltable.h kelet.h dataimage.h
	$(CC) -c $(CFLAGS) -o first_transition.o first_transition.c
	
second_transition.o: second_transition.c second_transition.h kelet.h instExecution.h
	$(CC) -c $(CFLAGS) -o second_transition.o second_transition.c
	
instExecution.o: instExecution.c instExecution.h kelet.h
	$(CC) -c $(CFLAGS) -o instExecution.o instExecution.c
	
dataimage.o: dataimage.c dataimage.h constants.h
	$(CC) -c $(CFLAGS) -o dataimage.o dataimage.c
	
symboltable.o: symboltable.c symboltable.h constants.h
	$(CC) -c $(CFLAGS) -o symboltable.o symboltable.c

instructions.o: instructions.c instructions.h constants.h
	$(CC) -c $(CFLAGS) -o instructions.o instructions.c
	
files.o: files.c files.h
	$(CC) -c $(CFLAGS) -o files.o files.c
	
kelet.o: kelet.c kelet.h instructions.h constants.h
	$(CC) -c $(CFLAGS) -o kelet.o kelet.c
	
constants.o: constants.h constants.c
	$(CC) -c $(CFLAGS) -o constants.o constants.c
