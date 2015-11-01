#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>

#define ARRAY_SIZE 5*1000*1000

// 5 milhoes +/- 19MB
// 1 milhar +/- 4KB
//#define N_THREADS 48

int * array;
//int nThreads;

int test (long double sum) {
//test variables
  long long int sumF=0;
  int lastI=-1;
  int correct=1;
  int i=0;

  lastI=-1;
  while(i<ARRAY_SIZE && correct==1){
  	sumF += array[i];
    if (array[i] < lastI) correct=0;
    lastI=array[i];
  	i++;
  }

  //printf("Sumatório depois de ordenação: %lld\n",sumF);
  if (sum!=sumF) correct=0;

  return correct;
}

void printArray(){
  printf("[%d",array[0]);
  int i=1;
  while(i<ARRAY_SIZE){
    printf(", %d",array[i]);
    i++;
  }
  printf("]\n");
}

void quicksort(int lo,int hi){

  int i=lo,j=hi,h,tID;
  int x=array[(lo+hi)/2];

    do{
      //#pragma omp sections
      {
          //#pragma omp task //shared(i)
          //#pragma omp section
          while(array[i]<x) i++;
          //#pragma omp task //shared(j)
          //#pragma omp section
          while(array[j]>x) j--;
          //#pragma omp taskwait
      }

        if(i<=j){
            h=array[i];
            array[i]=array[j];
            array[j]=h;
            i++;
            j--;
        }
    }while(i<=j);

    //#pragma omp parallel //shared (i,j,hi,lo)
    {
      //#pragma omp sections //nowait
      //#pragma omp single
      {
        //#pragma omp section
        #pragma omp task
        if(lo<j) quicksort(lo,j);

        //#pragma omp section
        #pragma omp task
        if(i<hi) quicksort(i,hi);
      }
    }
}

int main (int argc, char **argv){

  printf("\n\n*******************************************************\n\n");
  int threads = atoi(argv[1]);
  int i=0;
  long long int sum =0;
  srand(time(NULL));

  array=malloc( sizeof(int) * ARRAY_SIZE); //5MB

  printf("A inicializar o array com %d elementos...\n", ARRAY_SIZE);

  while(i<ARRAY_SIZE){
    array[i] = rand() % 100 +1;
    sum += array[i];
    i++;
  }

  //printArray();

  printf("Concluido!\n");
  printf("Tamanho do array: ");
  double bytes = sizeof(int) * ARRAY_SIZE;
  if(bytes<=1024)  printf("%.3f bytes...\n", (double) bytes);
  if(bytes>1024 && bytes <= 1024*1024)  printf("%.3f Kbytes...\n", (double) bytes/1024);
  if(bytes>1024*1024)  printf("%.3f Mbytes...\n", (double) bytes/(1024*1024));

  omp_set_num_threads(threads);
  //printf("Sumatório: %lld\n",sum);
  printf("A correr o quicksort...\n");

  double start = omp_get_wtime(); //inicio contagem do tempo

  #pragma omp parallel
  {
    #pragma omp master
    {
      //#pragma omp sections
      {
      quicksort(0,ARRAY_SIZE-1);
      }
    }
  }

  double end = omp_get_wtime();  //fim da contagem do tempo

  printf("Concluido com %d threads em %f segundos.\n", threads, (end-start));
  printf("A iniciar função de teste...\n");

  int r=test(sum);

  printf("Concluido!\n");
  if (r==0) printf("Incorreto\n");
  if (r==1) printf("Correto\n");

  //printArray();

  printf("\n\n*******************************************************\n\n");
}

//
