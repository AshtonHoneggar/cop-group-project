#include <time.h>
#include <stdlib.h>
#include <stdio.h>

size = 10;
int array[size];

void merge(int ll, int lr, int rl, int rr){
  int l_size = lr - ll + 1;
  int r_size = rr - rl + 1;
  //define temp arrays for both halves
  int left[size];
  int right[size];
  //copy left half into temp array
  for (int i = 0; i < l_size; i++){
    left[i] =  array[ll + i];
  }
  //copy right half into temp array
  for (int i = 0; i < r_size; i++){
    right[i] =  array[rl + i];
  }
  //ints used to keep track of lowest value in each half array
  int l = 0;
  int r = 0;
  int f = ll;
  //merge array into original
  while (l < l_size && r < r_size) {
    if (left[l] <= right[r]){
      array[f] = left[l];
      l++;
    }
    else{
      array[f] = right[r];
      r++;
    }
    f++;
  }
}
//split each recursively then merge
void mergeSort(int start, int end){
  if(start < end){
    int mid = (start + end) / 2;
    mergeSort(start, mid);
    mergeSort(mid+1, end);
    //then merge each half into one
    merge(start, mid, mid+1, end);
  }

}

int main(){
  srand(time(NULL));
  //test array
  for (int i = 0; i < 10; i++){
    array[i] = rand() % 256;
  }
  //first split will be a special case due to fork()
  int start, mid, end;
  start = 0;
  end = size - 1;
  mid = (start + end) / 2;
  //process1
  mergeSort(start, mid);
  //process2

  //merge both after both children finish
  mergeSort(mid+1, end);

  printf("test\n");
}
