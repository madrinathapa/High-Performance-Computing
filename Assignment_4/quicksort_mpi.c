#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "mpi.h"

void QuickSortFixedPivot(int nums[], int left, int right);
int FixedPartition(int input_elems[], int low, int high);
void SwapValues(int n[], int a, int b);

int main(int argc, char * argv[]){
	int myId, numsProcs, len = 1000, localLen, pivot ;
	int *nums; 


	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numsProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myId);
    	nums = (int *)malloc (len * sizeof(int));
	
	if(myId == 0){
		for(int i=0; i<len; i++){
			nums[i] = rand() % (10000 + 1 - 0) + 0;
		}
	}
	localLen = len/numsProcs;
	int numsLocal[localLen];
	
	MPI_Scatter (nums, localLen, MPI_INT, &numsLocal,
             localLen, MPI_INT, 0, MPI_COMM_WORLD);

	QuickSortFixedPivot(numsLocal, 0, localLen-1);

	MPI_Gather (numsLocal, localLen, MPI_INT, nums,
            len, MPI_INT, 0, MPI_COMM_WORLD);
	
	if(myId == 0){
		QuickSortFixedPivot(nums, 0, len -1);

		printf("\nThe sorted numbers are:\n");
		for(int i=0; i<len; i++){
			printf("%d,",nums[i]);
		}
	}


	free(nums);
	MPI_Finalize();
	return 0;

}

//Method for implementing sequential quicksort
void QuickSortFixedPivot(int nums[], int left, int right)
{
    int pivotIndex;

    if(left<right)
    {
        pivotIndex = FixedPartition(nums, left, right);
        QuickSortFixedPivot(nums, left, pivotIndex-1);
        QuickSortFixedPivot(nums, pivotIndex+1, right);
    }
}

//Fixed point pivot partition
int FixedPartition(int input[], int low, int high) 
{
    int i, j, pivot;

    pivot = input[high];
    i=low;

    for(j=low; j<high; j++)
    {
        if(input[j] <= pivot) {
        	int temp = input[i];
        	SwapValues(input, i, j);
            i++;
        }
    }

    SwapValues(input, i, high);

    return i;
}

//Swap values at two different indexes
void SwapValues(int n[], int a, int b){
  int temp;
  temp = n[a];
  n[a] = n[b];
  n[b] = temp;
}

