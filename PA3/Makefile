
CC = gcc
OBJS = timer.o
FLAGS = -fopenmp -O3
SEQFLAGS = -O3

EXEC = knapSeq knap1

all: $(EXEC)

knapSeq: knap.c timer.o
	$(CC)  $(SEQFLAGS) -o knapSeq knap.c $(OBJS) 

knap1: knap1.c timer.o
	$(CC)  $(SEQFLAGS) -g -o $@ knap1.c $(OBJS) 

timer.o: timer.c
	$(CC) -o $@ -c timer.c

clean:
	rm -f $(EXEC) $(OBJS)

tar: Makefile knap1.c
	tar cvf PA3.tar Makefile knap1.c
