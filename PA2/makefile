CC = gcc
OBJS = timer.o
FLAGS = -O3 -fopenmp
FLAGS = -O3 
OMP = -fopenmp
MATHFLAGS = -lm

EXEC = sieve sieve1 sieve2 sieve3 sieve4

all: $(EXEC)

sieve: sieve.c $(OBJS)
	$(CC) $(FLAGS)  -o $@ $< $(OBJS) $(MATHFLAGS)

sieve1: sieve1.c $(OBJS)
	$(CC) $(FLAGS) $(OMP) -o $@ $< $(OBJS) $(MATHFLAGS)

sieve2: sieve2.c $(OBJS)
	$(CC) $(FLAGS) $(OMP) -o $@ $< $(OBJS) $(MATHFLAGS)

sieve3: sieve3.c $(OBJS)
	$(CC) $(FLAGS) -g $(OMP) -o $@ $< $(OBJS) $(MATHFLAGS)

sieve4: sieve4.c $(OBJS)
	$(CC) $(FLAGS) -g $(OMP) -o $@ $< $(OBJS) $(MATHFLAGS)

timer.o: timer.c
	$(CC) -O3 -o $@ -c $<

clean:
	rm -f $(OBJS) $(EXEC) PA2.tar

package: sieve*.c
	tar cvf PA2.tar sieve*.c report.pdf makefile
