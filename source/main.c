// thrded_matmul_heap.c
// CS4540 Fall 2012
// Templated off Kapenga's matmul_heap.c
// Ian Kane
//

// The following itteretion can be used to solve linear systems
//   t_{i+1} = A t_i + b
// If the itteration converges to t, then t == t_{i+1} == t_i
// So t = A t + b
//   or  (I-a) t = b
//   where, I is the n*n idenity matrix
// There are several important applied problems where convergence
// will take place. One such case is when for
// each row of A ( rows 0 <= i < n)
//             sum(j=0 ... n-1) abs(a[i][j])  < 1.0
// Then the itteration will converge, assuming no roundoff or overflow.
// Example
// % ./matmul_heap 4 10 5

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "dbg.h"
#include "linearsystem.h"

#define NT 2   //Number of architecture supported threads.

// These two function are not ansi C so they do not appear from the
// libstd.h  header if the gcc option -std=c99 option is used.
// I should see if there is a safe way to include them from stdlib.h
// and not place them explicitly here, which is bad style.
void srand48(long int seedval);
double drand48(void);

int
main(int argc, char *argv[]){
  int   n=4;    // problenm size
  int   seed=10;// seed for srand48() / drand48()
  int i,j;      // iterators
  double *a;    // A diagonally dominant matrix A. Space to be allocated.
  double *b;    // A translation vector B.  Space to be allocated.
  int   itt_max=5;// number of itterations to preform
  int   itt;    // current itteration
  char  ch;     // for error checking on command line args.
  pthread_t threads[NT];
  linearSystem_t local_system;

  if( argc == 4 ) {
    if( (sscanf(argv[1],"%d %[^ /t]", &n, &ch) != 1) ||
        (sscanf(argv[2],"%d %[^ /t]", &seed, &ch) != 1) ||
        (sscanf(argv[3],"%d %[^ /t]", &itt_max, &ch) != 1) ) {
      fprintf(stderr," ERROR : useage: %s [ <n> <seed> <itt_max>]\n", argv[0]);
      return(1);
    }
  } else if(argc != 1 ) {
    fprintf(stderr," ERROR : useage: %s [ <n> <seed> <itt_max>]\n", argv[0]);
    return(1);
  }
  if( n<1 ) {
    fprintf(stderr," ERROR : n must be positive\n");
    return(1);
  }
  if( (a=(double *)malloc(sizeof(double)*n*n)) == NULL) {
    fprintf(stderr," ERROR : malloc for a failed\n");
    return(1);
  }
  if( (b=(double *)malloc(sizeof(double)*n)) == NULL) {
    fprintf(stderr," ERROR : malloc for b failed\n");
    return(1);
  }
  if( (local_system.t=(double *)malloc(sizeof(double)*n)) == NULL) {
    fprintf(stderr," ERROR : malloc for t failed\n");
    return(1);
  }
  if( (local_system.t1=(double *)malloc(sizeof(double)*n)) == NULL) {
    fprintf(stderr," ERROR : malloc for t1 failed\n");
    return(1);
  }
  //t = Ax + b;
  local_system.a = a;         //transformation matrix A
  local_system.b = b;         //translation vextor b
  local_system.dimension = n; //dimension of linear space.

  // Generate matrix a with | eigenvalues | < 1
  srand48((long int)seed);
  printf ("\n  a=\n");
  for(i=0; i< n; i++) {
    for(j=0; j< n; j++) {
      *(a+n*i+j) = 1.999 * (drand48() - 0.5) / n;
      printf("%10.6f ", *(a+n*i+j) );
    }
    printf("\n");
  }
  printf("\n  b=\n");
  // Generate vector b
  for(i=0; i< n; i++) {
    b[i] = 10.0 * drand48();
    printf("%10.6f ", b[i]);
  }
  printf("\n");
  // Initialize t
  for(i=0; i< n; i++) {
    local_system.t[i] = b[i];
  }

  
  //main loop
  // printf("\n  itt  error\n");
  for(itt=0; itt<=itt_max; itt++) {
    step_linear_system((pthread_t *)&threads, NT, &local_system);
  }
  return(0);
}

