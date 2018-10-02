/*/////////////////////////////////////////////////////////////////////////////
//
// File name : sieve.c
// Author    : Missa Osheim
// Date      : 2010/19/10
// Desc      : Finds the primes up to M
//
// updated Wim Bohm
/////////////////////////////////////////////////////////////////////////////*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "timer.h"

#define min(x, y) (((x) < (y)) ? (x) : (y))

size_t BLK_SIZE = 9;

int first_multiple_in_block(int start, int prime) {
  int odd_start = (2*start)+1;
  if(!(odd_start % prime)) {
    return start;
  }
  int step_one = ((odd_start/prime)+1)*prime;
  return ((step_one % 2)? step_one : step_one+prime)/2;
}


int main(int argc, char **argv) {

  long N  = 100;

  char *mark;

  long   size;
  long   curr;
  long   i, j,n;
  long   count;

  /* Time */

  double time;

  if ( argc > 1 ) {
    N  = atoi(argv[1]);
    BLK_SIZE = atoi(argv[2]);
  }

  /* Start Timer */

  initialize_timer ();
  start_timer();
  
  int M = (N%2?(N/2)+1:N/2);
  mark = (char *)malloc(M);

  #pragma omp parallel for
  for (i=1; i<M; i++) {
    mark[i] = 0;
  }

  // Construct an array of primes [2, sqrt(N)]
  size_t max_size = sqrt(M);
  int primes[max_size];
  int num_primes = 1;
  primes[0] = 2;

  long root_m = sqrt(M);
  curr=1;       /*first prime*/
  while (curr*curr <= (M/2)-1) {
    int jump = (2*curr)+1;
    primes[num_primes++] = jump;
    #pragma omp parallel for
    for (i = jump*curr + curr; i < root_m; i += jump) {
      mark[i] = 1;
    }
    while(mark[++curr]); // Do nothing
  }

  // Search each block for primes in the prime list
  int prime;
  int start = sqrt((M/2))+1;
  #pragma omp parallel for private(prime, i)
  for (int block_start = start; block_start < N; block_start += BLK_SIZE) {
    for (prime = 1; prime < num_primes; prime++) {
      int jump = primes[prime];
      int first_prime = first_multiple_in_block(block_start, jump);
      for (i = first_prime;
            i <= min(block_start+BLK_SIZE, M); i += jump) {
        mark[i]=1;
      }
    }
  }


  /* stop timer */
  stop_timer();
  time=elapsed_time ();

  /*number of primes*/
  count = 1;
  for(i = 1; i < M; i++){
      if(mark[i] == 0) {
        //printf("\t prime %ld  \n", (2*i)+1);
        ++count;
      }
  }
  printf("There are %ld primes less than or equal to %ld\n", count, N);
  /* print results */
  printf("First three primes:");
  j = 1;
  printf("%d ", 2);
  for ( i=3 ; i <= N && j < 3; i+=2 ) {
    if (mark[i]==0){
          printf("%ld ", i);
          ++j;
    }
  }
  printf("\n");

  printf("Last three primes:");
  j = 0;
  for (i = M-1; i > 1 && j < 3; i--){
   if (mark[i]==0){
      printf("%ld ", (2*i)+1);
      j++;
   }
  }
  printf("\n");


  printf("elapsed time = %lf (sec)\n", time);

  free(mark);
  return 0;
}


