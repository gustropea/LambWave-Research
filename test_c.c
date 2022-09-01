#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NMAX 10000
#define LOOP_COUNT 1000

void showmat(double mat[NMAX][NMAX]){
  for(int i=0;i<NMAX;++i){
    for(int j=0;j<NMAX;++j){
      printf(" %f ", mat[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

void setx(double *x0, int n){
  for(int i=0;i<n*n;i++){
      *(x0+i) = 10;
  }
}

double setrow(double *row, int n){
  clock_t start = clock();
  for(int i=0; i<n; i++){
    *(row + i) = 7;
  }
  clock_t stop = clock();
  double time = (((stop-start) * 1000.0)/CLOCKS_PER_SEC);
  return time;
}

double setcol(double *col, int n){
  clock_t start = clock();
  for(int j=0; j<n; j++){
    *(col+(j*n)) = 5;
  }
  clock_t stop = clock();
  double time = (((stop-start) * 1000.0)/CLOCKS_PER_SEC);
  return time;
}

int main(int argc, char *argv[]) {
  clock_t start_prog = clock();
  double (*x)[NMAX];
  x = malloc(sizeof(double[NMAX][NMAX]));
  setx(&x[0][0],NMAX);
  //showmat(x);
  printf("matrix size : %d x %d \n",NMAX,NMAX);
  printf("loop count: %d \n",LOOP_COUNT);
  double rowtime = 0;
  for(int i=0; i<LOOP_COUNT; i++){
    rowtime = rowtime + setrow(&(x[3][0]),NMAX);
  }
  printf("set row time : %f ms \n", rowtime);
  //showmat(x);
  double columntime = 0;
  for(int i=0; i<LOOP_COUNT; i++){
    columntime = columntime + setcol(&(x[0][2]),NMAX);
  }
  printf("set column time : %f ms \n",columntime);
  //showmat(x);
  free(x);
  clock_t stop_prog = clock();
  double prog_time = (((stop_prog-start_prog) * 1000.0)/CLOCKS_PER_SEC);
  printf("overall program run time : %f ms \n",prog_time);
  return 0;
}
