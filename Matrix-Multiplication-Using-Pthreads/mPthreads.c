#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <time.h>


int size, num_threads;
double **matrix1, **matrix2, **matrix3;

double ** allocate_matrix( int size )
{
  
  double * vals = (double *) malloc( size * size * sizeof(double) );
  double ** ptrs = (double **) malloc( size * sizeof(double*) );

  int i;
  for (i = 0; i < size; ++i) {
    ptrs[ i ] = &vals[ i * size ];
  }

  return ptrs;
}

void init_matrix( double **matrix, int size )
{
  int i, j;

  for (i = 0; i < size; ++i) {
    for (j = 0; j < size; ++j) {
      matrix[ i ][ j ] = 1.0;
    }
  }
}

void * worker( void *arg )
{
   
  int i, j, k, tid, portion_size, row_start, row_end;
  double sum;
  
  tid = *(int *)(arg);
  portion_size = size / num_threads;
  row_start = tid * portion_size;
  row_end = (tid+1) * portion_size;

  for (i = row_start; i < row_end; ++i) { 
    for (j = 0; j < size; ++j) { 
      for (k = 0; k < size; ++k) { 
	sum += matrix1[ i ][ k ] * matrix2[ k ][ j ];
      }
      matrix3[ i ][ j ] = sum;
    }
  }

}

int main( int argc, char *argv[1] )
{
  int i;
  double sum = 0;
  srand(time(NULL));
  struct timeval tstart, tend;
  double exectime;
  pthread_t * threads;

  size = 256;
  num_threads = atoi( argv[1] );

  threads = (pthread_t *) malloc( num_threads * sizeof(pthread_t) );

  matrix1 = allocate_matrix( size );
  matrix2 = allocate_matrix( size );
  matrix3 = allocate_matrix( size );
  
  init_matrix( matrix1, size );
  init_matrix( matrix2, size );

  gettimeofday( &tstart, NULL );
  for ( i = 0; i < num_threads; ++i ) {
    int *tid;
    tid = (int *) malloc( sizeof(int) );
    *tid = i;
    pthread_create( &threads[i], NULL, worker, (void *)tid );
  }

  for ( i = 0; i < num_threads; ++i ) {
    pthread_join( threads[i], NULL );
  }
  gettimeofday( &tend, NULL );
 
  exectime = (tend.tv_sec - tstart.tv_sec) * 1000.0;
  exectime += (tend.tv_usec - tstart.tv_usec) / 1000.0;  

  printf("%0.3lf\n",exectime/1000.0);

  return 0;
}
