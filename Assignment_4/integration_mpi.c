#include <stdio.h>
#include "mpi.h"
#include <math.h>
main(int argc, char** argv){
	int myId, numsProcs, n = 1024, localN, source, dest = 0, tag = 0;
	float lowerLimit = 0.0, upperLimit = 1.5708, h, localLowerLimit, localUpperLimit, integral, total; 

	MPI_Status status;

	float TotalArea(float localLowerLimit, float localUpperLimit, int localN, float h);

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &myId);

	MPI_Comm_size(MPI_COMM_WORLD, &numsProcs);

	h = (upperLimit - lowerLimit)/n;
	localN =  n/numsProcs;

	localLowerLimit = lowerLimit +myId*localN*h;
	localUpperLimit = lowerLimit+localN*h;
	integral = TotalArea(localLowerLimit, localUpperLimit, localN, h);

	if(myId != 0){
		MPI_Send(&integral, 1, MPI_FLOAT, dest, tag, MPI_COMM_WORLD);
	}else if(myId == 0){
		total = integral;
		for(source = 1; source < numsProcs; source++){
			MPI_Recv(&integral, 1, MPI_FLOAT, source, tag, MPI_COMM_WORLD, &status);
			total += integral;
		}
		printf("The integral from %f to %f = %f\n", lowerLimit, upperLimit, total);
	}
	MPI_Finalize();
}

float TotalArea(float localLowerLimit, float localUpperLimit, int localN, float h){
	float x, integral=0.0;
	int i;

	float CalculateExp(float x);

	integral = (CalculateExp(localLowerLimit) + CalculateExp(localUpperLimit))/2.0;
	x = localLowerLimit;

	for(i=1; i <= localN-1; i++){
		x = x+h;
		integral = integral + CalculateExp(x);
	}

	integral = integral*h;
	return integral;
}

float CalculateExp(float x){
	float returnVal;
	returnVal = cos(x)*sin(x/2);
	return returnVal;
}
