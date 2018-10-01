
// File name   : knap.c
// Author      : DaeGon
// Modified by : Sanjay and Wim 
// Date        : Sep 20
// Description : Dynamic Programming for the 0/1 knapsack
//               problem.  The entire table is saved.
//

#include <stdio.h>
#include <stdlib.h>
#include "timer.h"

#define    MAX(x,y)   ((x)>(y) ? (x) : (y))
#define    table(i,j)    table[(i)*(C+1)+(j)]

void print_table(const long table[], long C, long N) {
  for (long j=0; j<=C; j++){
    printf ("%d\t", j);
    for (long i=0; i<N; i++)
      printf ("%d ", table(i, j));
    printf("\n");
  }
}

void get_last_row(long table[], const long weights[], const long profits[], long C,
                   long low_obj, long up_obj) {
  for (long j=0; j<=C; j++) {
    if (j<weights[low_obj])
      table(0, j)= 0;
    else
      table(0, j)= profits[low_obj];
  }

  for (long obj_num = low_obj+1; obj_num <= up_obj; obj_num++) {
    for (long j=0 ; j <= C ; j++ ) {
      long obj_index = obj_num-low_obj;
      long prev_obj_index = obj_index - 1;

      if(j<weights[obj_num])
        table(obj_index, j) = table(prev_obj_index, j);
      else
        table(obj_index,j)=MAX(
            table(prev_obj_index, j),
            profits[obj_num] + table(prev_obj_index, j - weights[obj_num])
          );
     }
  }
}

// Determine the optimal cut in the last rows.
long find_C_star(long C, const long first[], long left_interval_size,
                 const long second[], long right_interval_size) {
  long max = 0;
  long index = -1;
  for(long i = 0; i <= C; i++) {
    long sum = first[(left_interval_size-1)*(C+1)+i] +
               second[(right_interval_size-1)*(C+1)+(C-i)];
    if(sum >= max) {
      max = sum;
      index = i;
    }
  }
  return index;
}

void solve_kp(long solution[], const long weights[], const long profits[],
              long C, long lower, long upper) {
  /**
   * Calculate wether the left and right interval sizes. In the case where the interval is odd, 
   *  left = right + 1 (1). Check whether or not the base case has been reached, this happens
   *  when lower == upper. If we find a solution add it to the solution array (2).
   *  Contstruct the two sub tables (3). Find the optimal midpoint between the
   *  tables (4). Recurse on the left and right intervals while also reducing the capacity (5).
   */
  // (1)
  long interval_size = (upper-lower);
  long left_interval_size = interval_size/2;
  long right_interval_size = left_interval_size + (interval_size%2)?:1;

  // (2)
  if(upper-lower <= 1) {
    if (weights[upper-1] <= C) {
      solution[upper-1] = 1;
    }
    return;
  }

  // (3)
  long size;

  size = (C+1) * left_interval_size * sizeof(long);
  long* sub_table1 = (long*)malloc(size);
  get_last_row(sub_table1, weights, profits, C, lower, lower + left_interval_size-1);

  size = (C+1) * right_interval_size * sizeof(long);
  long* sub_table2 = (long*)malloc(size);
  get_last_row(sub_table2, weights, profits, C, lower + left_interval_size, upper-1);

  // (4)
  long C_star = find_C_star(C, sub_table1, left_interval_size, sub_table2, right_interval_size);

  free(sub_table1);
  free(sub_table2);

  // (6)
  solve_kp(solution, weights, profits, C_star, lower, lower + interval_size/2);
  solve_kp(solution, weights, profits, C - C_star, lower + interval_size/2, upper);
}

int main(int argc, char **argv) {

   FILE   *fp;
   long    N, C;                   // # of objects, capacity 
   long    *weights, *profits;     // weights and profits
   int    verbose,count;

   // Temp variables
   long    i, j, size;

   // Time
   double time;

   // Read input file (# of objects, capacity, (per line) weight and profit )
   if ( argc > 1 ) {
      fp = fopen(argv[1], "r"); 
      if ( fp == NULL) {
         printf("[ERROR] : Failed to read file named '%s'.\n", argv[1]);
         exit(1);
      }
   } else {
      printf("USAGE : %s [filename].\n", argv[0]);
      exit(1);
   }

   verbose = (argc > 2)? atoi(argv[2]) : 0;

   fscanf(fp, "%ld %ld", &N, &C);
   printf("The number of objects is %d, and the capacity is %d.\n", N, C);

   size    = N * sizeof(long);
   weights = (long *)malloc(size);
   profits = (long *)malloc(size);

   if ( weights == NULL || profits == NULL ) {
      printf("[ERROR] : Failed to allocate memory for weights/profits.\n");
      exit(1);
   }

   for ( i=0 ; i < N ; i++ ) {
      count = fscanf(fp, "%ld %ld", &(weights[i]), &(profits[i]));
      if ( count != 2 ) {
         printf("[ERROR] : Input file is not well formatted.\n");
         exit(1);
      }
   }

   fclose(fp);


  long solution[N];
  for(i = 0; i < N; i++)
    solution[i] = 0;

  initialize_timer ();
  start_timer();

  solve_kp(solution, weights, profits, C, 0, N);

  long optimal = 0;
  for(i = 0; i < N; i++) {
    if (solution[i] == 1) {
      optimal += profits[i];
    }
  }

  stop_timer();
  time = elapsed_time ();

  printf("The optimal profit is %d \nTime taken : %lf.\n", optimal, time);
  
  if (verbose==1) {
      printf("Solution vector is: ");
      for (i=0 ; i<N ; i++ ) {
       printf("%d ", solution[i]);
      }
      printf("\n");
    }

  free(weights);
  free(profits);

  return 0;
}
