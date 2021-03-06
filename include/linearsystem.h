#ifndef __lnrsys_h__
#define __lnrsys_h__

#include <pthread.h>


/* linear system:
 * t = Ax + b
 */
typedef struct{
  double *a;   //A
  double *b;   //B 
  double *t;   //t
  double *t1;  //x
  int dimension; //dimension of the rowspace of t.
}linearSystem_t;

/*Structured meta information.
 *  A packaged reference to a linear system,
 *  and some reflective information for dispatched threads.
 */
typedef struct {
  linearSystem_t *system;
  int thread_max;
  int thread_number;
}threadLocalState_t;


int step_linear_system(pthread_t *threads, int threadcount, linearSystem_t *system );
void *thread_transformation(void * threadstate);

#endif
