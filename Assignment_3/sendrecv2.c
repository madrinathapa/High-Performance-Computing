#include <stdio.h>
#include "mpi.h"


typedef struct{
	int max_iter;
	double t0;
	double tf;
	double xmax[12];
	double xmin;
} Pars;

int main(int argc, char *argv[]){
	int myid, numsprocs, left, right;
	Pars buffer, buffer2;
	MPI_Request request;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numsprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	right = (myid+2)%numsprocs;
	left = myid - 2;
	if(left<0)
		left += numsprocs;

	//initialise the send buffer
	buffer.max_iter = myid;
	buffer.t0 = 3.14*myid;
	buffer.tf = 1.67*myid;
	buffer.xmin = 2.55*myid;

	for(int i=0;i<12;i++){
		buffer.xmax[i] = 2.7*myid;
	}

	int nitems = 5;
	MPI_Datatype types[]={MPI_INT, MPI_DOUBLE, MPI_DOUBLE,MPI_DOUBLE, MPI_DOUBLE}; 
	MPI_Datatype mpi_par;  // give my new type a name
	MPI_Aint offsets[nitems];  // an array for storing the element offsets
	int blocklengths[] = {1,1,1,12,1};	

  	
	MPI_Aint intsz,doubsz;
	MPI_Type_extent(MPI_INT,&intsz);
	MPI_Type_extent(MPI_DOUBLE,&doubsz);
	offsets[0] = 0;
	offsets[1] = intsz;
	offsets[2] = intsz+doubsz;
	offsets[3] = (2*doubsz)+intsz;
	offsets[4] = (3*doubsz)+intsz;

	MPI_Type_create_struct(nitems,blocklengths,offsets,types,&mpi_par);
	MPI_Type_commit(&mpi_par);
	//Send myid to the left
	MPI_Sendrecv(&buffer, 1, mpi_par, left, 123, 
		&buffer2, 1, mpi_par, right, 123, MPI_COMM_WORLD, &status);

	printf("Process %d received %d\n", myid, buffer2.max_iter);

	MPI_Finalize();
	return 0;
}
