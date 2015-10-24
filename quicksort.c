#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>

#define ARRAY_SIZE 1024*1024*50

int * array;


void quicksort(int lo,int hi){

  int i=lo,j=hi,h;
  int x=array[(lo+hi)/2];

  #pragma	omp parallel
  //partition
  do{
    while(array[i]<x) i++;
    while(array[j]>x) j--;
    if(i<=j){
      h=array[i];
      array[i]=array[j];
      array[j]=h;
      i++;
      j--;
    }
  }while(i<=j);

  //recursion
  #pragma omp for schedule(guided)
  if(lo<j) quicksort(lo,j);
  if(i<hi) quicksort(i,hi);

}


int main (){
  int i=0;
  srand(time(NULL));

  array=malloc( sizeof(int) * ARRAY_SIZE); //5MB

  while(i<ARRAY_SIZE){
    array[i] = rand() % 100 + 1;
    i++;
  }

  clock_t start = clock(); //inicio contagem do tempo

  quicksort(0,ARRAY_SIZE);

  clock_t end = clock();  //fim da contagem do tempo
  float seconds = (float)(end - start) / CLOCKS_PER_SEC;

  printf("\n\nExecutado em %f segundos...\n\n",seconds);

  /*i=0;
  while(i<1024*1024*5){
    printf("%d ; ",array[i]);
    i++;
  }*/
}
