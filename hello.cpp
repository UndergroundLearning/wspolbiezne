#include <iostream>
#include <mpi.h>

using namespace std;

int main(int argc, char** argv)
{
	int rank=0, size=0, msg;
	MPI_Init(&argc, &argv );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	MPI_Comm_size( MPI_COMM_WORLD, &size );
	
  //jestem patolem
	if(rank == size-1)
	{
		MPI_Recv (&msg, 1, MPI_INT, size-2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) ;
		msg += rank;
		cout << "RANK:" << rank << " MSG: " << msg << endl; 
	} 
	else if(rank==0)
	{
		MPI_Send (&msg, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
	}
	else
	{
		MPI_Recv (&msg, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) ;
		msg += rank;
		MPI_Send (&msg, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
	}
	MPI_Finalize();
}