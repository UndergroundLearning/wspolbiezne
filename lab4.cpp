#include <iostream>
#include <mpi.h>
#include <cstdlib>

using namespace std;

int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);
	int rank, size, n, *ranks, sumaP, sumaN ,rankL, sizeL;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Group wszystkie ,parzyste, nieparzyste;
	MPI_Comm parzysty, nieparzysty;
	MPI_Comm_group(MPI_COMM_WORLD, &wszystkie);
	
	//int parzyste = rank % 2;
	//MPI_Comm_split(MPI_COMM_WORLD, parzyste, rank, &nowy);
	if(size % 2 == 0)
		n = (int) (size/2);
	else
		n = (int) (size/2)+1;
	ranks = new int[n];
	for(int i=0, index=0; i<size && index<n; i++)
	{
		if(i % 2 == 0)
		{
			ranks[index] = i;
			index++;
		}
	}
	MPI_Group_incl(wszystkie, n, ranks, &parzyste);
	MPI_Group_difference(wszystkie, parzyste, &nieparzyste);
	MPI_Comm_create(MPI_COMM_WORLD, parzyste, &parzysty);
	MPI_Comm_create(MPI_COMM_WORLD, nieparzyste, &nieparzysty);
	if(rank % 2 ==0)
	{
		MPI_Comm_rank(parzysty, &rankL);
		MPI_Comm_size(parzysty, &sizeL);
		MPI_Allreduce(&rank, &sumaP, 1, MPI_INT, MPI_SUM, parzysty);
		if(rankL == sizeL-1)
			cout << sumaP << endl;
	}
	else
	{
		MPI_Comm_rank(nieparzysty, &rankL);
		MPI_Comm_size(nieparzysty, &sizeL);
		MPI_Allreduce(&rank, &sumaN, 1, MPI_INT, MPI_SUM, nieparzysty);
		if(rankL == sizeL-1)
			cout << sumaN << endl;
	}
	delete[] ranks;
	// MPI_Comm_group(MPI_COMM_WORLD, &wszystkie);
	// if (rank % 2 == 0) 
	// {
	// 	MPI_Group_incl(wszystkie, n, ranks, &parzyste);
 //  	}
 //  	MPI_Group_difference(wszystkie, parzyste, &nieparzyste);
 //  	MPI_Group_rank(parzyste, &sumaP);
 //  	cout << sumaP << endl;
/* Create new new communicator and then perform collective communications */
// MPI_Comm_create(MPI_COMM_WORLD, new_group, &new_comm);
// MPI_Allreduce(&sendbuf, &recvbuf, 1, MPI_INT, MPI_SUM, new_comm);

// MPI_Group_rank (new_group, &new_rank);
// printf("rank= %d newrank= %d recvbuf= %d\n",rank,new_rank,recvbuf);

	
	

	// MPI_Comm_group(MPI_COMM_WORLD, &wszystkie);
	// MPI_Group_incl(wszystkie, n, ranks, &parzyste);
	// MPI_Group_difference(wszystkie, parzyste, &nieparzyste);
	
	// int rankL;
	// MPI_Comm_rank(parzysty, &rankL);
	// cout << rankL << endl;

	// int rankP, rankN, sizeP, sizeN;
	// MPI_Comm_rank(parzysty, &rankP);
	// MPI_Comm_size(parzysty, &sizeP);
	// MPI_Comm_rank(nieparzysty, &rankN);
	// MPI_Comm_size(nieparzysty, &sizeN);
	// MPI_Allreduce(&rankP, &sumaP, 1, MPI_INT, MPI_SUM, parzysty);
	// MPI_Allreduce(&rankN, &sumaN, 1, MPI_INT, MPI_SUM, nieparzysty);
	// if(rankP == sizeP-1)
	// 	cout << "Suma nieparzystych: " << sumaN << endl;
	// if(rankN == sizeN -1)
	// 	cout << "Suma parzystych: " << sumaP << endl;
	MPI_Finalize();
}