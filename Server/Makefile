# A basic Makefile for the shellex code in the Systems Programming Book
CC = gcc
CFLAGS = -Wall -g
INCLUDES = -I.
LIBS = -lpthread -ldl
SRCS = echoserveri.c csapp.c echo.c
OBJS = echoserveri.o csapp.o echo.o
EXE = rrshClient

all:	$(EXE)

$(EXE):	$(OBJS) 
	$(CC) -o $(EXE) $(CFLAGS) $(INCLUDES) $(OBJS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) *.o *~ $(EXE)
