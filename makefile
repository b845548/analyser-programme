# File : makefile
# Programme : l'analyseur du code C avec le graph brin et mat
# Auteur : HOBEAN BAE
# Date : 4/DEC/2016
RM=rm
CC=gcc
CFLAGS= -Wall -m32
HEADER = analyser_C.h ANSIstandard.h
TESTSRC = testcode\*.c
PROGRAMMENAME = analyser
OBJ = $(HEADER) brin_base.o brin_analyse.o mat_base.o parserUtils.o main.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) 
$(PROGRAMMENAME): $(OBJ)
	gcc -o $@ $^ $(CFLAGS)
clean: 
	$(RM) *.o
