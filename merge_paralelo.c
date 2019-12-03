#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>


/**
 * Compilar: mpicc -o merge_paralelo merge_paralelo.c
 * Executar: mpiexec -np 4 ./merge_paraleloclear
 */

// Define Tamanho do Array a ser ordenado
#define MAXVALUE 99
#define SIZE 1024
#define MASTER 0 
int arr[SIZE];

//Generate random values for the given array
void generateRandomValues(int arr[], int size) {
    srand(time(NULL));
    for (int i = 0; i < size; i++)
        arr[i]= rand() % MAXVALUE;  
}

/* Copy the origin array to the destiny array starting from shift 
 until shift+size in origin array */
void copyArray(int origin[],int destiny[],int size, int shift){
    for(int i = 0; i < size; i++) 
        destiny[i] = origin[shift + i];   
}

void merge(int arr_local[], int start, int end) {
    int middle = (start + end) / 2;
    int leftSize = middle - start + 1;
    int rightSize = end - middle;
    int leftArr[rightSize], rightArr[rightSize];

    copyArray(arr_local,leftArr,leftSize,start);
    copyArray(arr_local,rightArr,rightSize,middle + 1);

    int i = 0, j = 0, current = start;
    while(i < leftSize && j < rightSize){
        if(leftArr[i] <= rightArr[j]) arr_local[current++] = leftArr[i++];
        else arr_local[current++] = rightArr[j++];
    } 
    
    while(i < leftSize) 
        arr_local[current++] = leftArr[i++];
    while(j < rightSize) 
        arr_local[current++] = rightArr[j++];
}

void mergeSort(int arr[], int start, int end){
    if(start >= end) return;
    int middle = (start+end)/2;
    mergeSort(arr,start,middle);
    mergeSort(arr,middle + 1,end);
    merge(arr, start, end); 
}


int main(int argc, char** argv) { 
    int rank;
    int np; 
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    generateRandomValues(arr, SIZE);

    clock_t start = clock();

    int arr_size = sizeof(arr)/sizeof(arr[0]);  
    int local_size = SIZE/np;
    
    int * childArray = malloc(local_size * sizeof(int));
    MPI_Scatter(arr, local_size, MPI_INT, childArray, local_size, MPI_INT, 0, MPI_COMM_WORLD);
    mergeSort(childArray, 0, local_size - 1); 

    int * sortedArray = NULL;
    if(rank == MASTER) 
        sortedArray = malloc(SIZE * sizeof(int)); 
    MPI_Gather(childArray, local_size, MPI_INT, sortedArray, local_size, MPI_INT, 0, MPI_COMM_WORLD);

    if(rank == MASTER) {
        mergeSort(sortedArray, 0, SIZE); 
        clock_t end = clock();
        float seconds = (float)(end - start)/CLOCKS_PER_SEC;
        

        printf("\nArray resultante:\n");
        for(int x = 0; x < SIZE; x++){
            printf("%d, ",sortedArray[x]);
        }

        printf("\nO tempo decorrido foi de %f segundos para realizar a ordenacao\n", seconds);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0; 
} 
