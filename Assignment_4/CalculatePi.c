/*Calculating value of Pi using Monte Carlo Method*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "mpi.h"

int main(int argc, char * argv[]){
	int myId, numsProcs, counter;
	long int  i, n = 100000000;
	double result, a, b, c;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numsProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myId);
	int countBuffer[numsProcs];
	long int countIter[numsProcs];
	srand(7853981);
	
	if(myId != 0){

		//Use rand() to generate random numbers
		for(i = 0; i < n; i++)
		{
			a = (double)(rand() % ((n/1000+1)))/(n/1000);
			b = (double)(rand() % ((n/1000)+1))/(n/1000);

			//Condition to check if the coordinate is inside the circle
		    if ((sqrt(pow(a,2) + pow(b,2))) <= 1){
		        counter++;
		    } 
		}

		//Send the count of coordinates inside the circle and iteration
		for(i=0; i<numsProcs; i++){
            MPI_Send(&counter, 1, MPI_INT, 0, myId, MPI_COMM_WORLD);
            MPI_Send(&n, 1, MPI_LONG, 0, myId, MPI_COMM_WORLD);
        }	
	}else if(myId == 0){
		//Receive the count of coordinates inside the circle and iteration
		for(i=0; i<numsProcs; i++)
        {
            MPI_Recv(&countBuffer[i], numsProcs, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            MPI_Recv(&countIter[i], numsProcs, MPI_LONG, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		}
		if (myId == 0){
	        long int total = 0, innerTotal = 0;
	        for(i = 0; i<numsProcs; ++i)
	        {
	            innerTotal += countBuffer[i];
	            total += countIter[i];
	        }
	 		
	 		//Calculate the average
	        result = ((double)innerTotal/(double)total)*4.0;
	        printf("Pi value is: %.10lf\n", result);
	    }
 	}
    MPI_Finalize();  
	return 0;
}
