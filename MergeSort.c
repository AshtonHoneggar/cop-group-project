//
// Created by ash on 11/11/2017.
//

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

/* Key number */
#define SHMKEY ((key_t) 1497)

/* Size of array */
#define SIZE 1024

// Shared memory structure to store integer array of size 1024
typdef struct {
    int arr[SIZE];
} shared_mem;

// Declare shared memory
shared_mem* data;

void merge(int l_start, int l_length, int r_start, int r_length) {
    int left[SIZE];
    int right[SIZE];
    int i;

    for (i = 0; i < l_length; ++i)
        left[i] = data->arr[l_start + i];

    for (i = 0; i < r_length; ++i)
        right[i] = data->arr[r_start + i];

    int l = 0, r = 0, index = l_start;

    while (l < l_length && r < r_length) {
        if (left[l] < right[r]) {
            data->arr[index] = left[l];
            ++l;
        } else {
            data->arr[index] = right[r];
            ++r;
        }
        ++index;
    }

    if (l = l_length) {
        for (;r < r_length; ++r, ++index) data->arr[index] = right[r];
    } else if (r = r_length) {
        for (;l < l_length; ++l, ++index) data->arr[index] = left[l];
    }
}

void mergeSort(int start, int length) {
    if (length > 1) {
        int l_newLength, r_newLength, r_newStart;
        l_newLength = length / 2;
        r_newStart = start + l_newLength + 1;
        r_newLength = length - l_newLength;
        mergeSort(start, l_newLength);
        mergeSort(r_newStart, r_newLength);

        merge(start, l_newLength, r_newStart, r_newLength);
    }
}

int main()
{
    int shmid, pid1 = 1, pid2 = 1;
    char* shmadd;
    shmadd = (char*) 0;

    // Create shared memory for integer array of size SIZE
    if ((shmid = shmget(SHMKEY, SIZE*sizeof(int), IPC_CREAT | 0666)) < 0) {
        printf("Error with shmget.\n");
        return 1;
    }

    data = shmat(shmid, shmadd, 0);
    // Point "data" variable to shared memory
/*    if ((data = (shared_mem *) shmat(shmid, shmadd, 0)) == (shared_mem *) -1) {
        printf("Error with shmat.\n");
        return 1;
    }
*/
    // Fill array with random variables
    srand(time(NULL));
    int i;
    for (i = 0; i < SIZE; ++i)
        data->arr[i] = rand % 256;

    int l_start, l_length, r_start, r_length;
    l_start = 0;
    l_length = SIZE / 2;
    r_start = l_start + l_length + 1;
    r_length = SIZE - l_length;

    if ((pid1 = fork()) == 0) {
        mergeSort(l_start, l_length);
    }

    if ((pid2 = fork()) == 0) {
        mergeSort(r_start, r_length);
    }

    waitpid(pid1);
    waitpid(pid2);

    merge(l_start, l_length, r_start, r_length);
    for (i = 0; i < SIZE; ++i) {
        printf("%d ", data->arr[i]);
    }
}