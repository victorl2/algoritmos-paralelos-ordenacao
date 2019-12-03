#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define MAXVALUE 1024
#define SIZE 1024
int arr[SIZE];

//Generate random values for the given array
void generateRandomValues(int arr[], int size) {
    srand(time(NULL));
    for (int i = 0; i < size; i++)
        arr[i]= rand() % MAXVALUE;  
}