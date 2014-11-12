#include <iostream>
#include <mpi.h>
#include <cstdlib>

using namespace std;

int max(int*& tab, int n) //FUNKCJA MAX
{
	int max = tab[0];
	for(int i=1; i<n; ++i)
		if(tab[i] > max)
			max = tab[i];
	return max;
}

int min(int*& tab, const int& size1)
{
	if(tab == NULL || size1 == 0)
		return 0;
	int min = tab[0];
	for(int i=1; i<size1; i++)
		if(tab[i] < min)
			min = tab[i];
	return min;
}

bool czyNalezy(const int& rank, int*& grupa, const int& rozmiar)
{
	if(rank == 0)
		return false;
	for(int i=0; i < rozmiar; i++)
		if(grupa[i] == rank)
		{
			return true;
		}
	return false;
}

int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);
	srand(time(NULL));
	int size, rank, rankL, size1, size2, *tab, rozmiar, rozmiarCzesci, *tabL, maxL, maxG;
	int master, masterG1, masterG2, masters[3];
	MPI_Group wszystkie, workG1, workG2, mastersG;
	MPI_Comm COMM_workG1, COMM_workG2, COMM_mastersG;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	size1 = (int) ((size-1)/2);
	int* pierwszaG = new int[size1];
	size2 = (int) (size - (size1+1));
	int* drugaG = new int[size2];
	int index1=0, index2=0;
	for(int i=1; i<size; i++)
	{
		if(i <= size1)
		{
			pierwszaG[index1] = i;
			index1++;
		}
		else
		{
			drugaG[index2] = i;
			index2++;
		}
	}
	MPI_Comm_group(MPI_COMM_WORLD, &wszystkie);
	MPI_Group_incl(wszystkie, size1, pierwszaG, &workG1);
	MPI_Group_incl(wszystkie, size2, drugaG, &workG2);
	MPI_Comm_create(MPI_COMM_WORLD, workG1, &COMM_workG1);
	MPI_Comm_create(MPI_COMM_WORLD, workG2, &COMM_workG2);
	masters[0] = 0;
	masters[1] = 1;
	masters[2] = size1+1;
	MPI_Group_incl(wszystkie, 3, masters, &mastersG);
	MPI_Comm_create(MPI_COMM_WORLD, mastersG, &COMM_mastersG);
	if(rank == 0)
	{
		rozmiar = size*size1*size2;
		tab = new int[rozmiar];
		for(int i=0; i<rozmiar; i++)
			tab[i] = rand()%200;
	}
	if(rank == masters[0] || rank == masters[1] || rank == masters[2])
	{
		MPI_Bcast(&rozmiar, 1, MPI_INT, 0, COMM_mastersG);
		if(rank != masters[0])
			tab = new int[rozmiar];
		MPI_Bcast(tab, rozmiar, MPI_INT, 0, COMM_mastersG);
	}

	if(rank <= size1 && rank != 0)
	{
		rozmiarCzesci = rozmiar/size1;
		int* lokalna = new int[rozmiarCzesci];
		MPI_Comm_rank(COMM_workG1, &rankL);
		MPI_Bcast(&rozmiarCzesci, 1, MPI_INT, 0, COMM_workG1);
		cout << rankL << " " << rank << " rozmiar czesci: " << rozmiarCzesci << endl;
		if(rankL == 0)
		{
			MPI_Send(tab, rozmiarCzesci, MPI_INT, 1, 0, COMM_workG1);
		}
		else if(rankL == 1)
		{
			MPI_Recv(lokalna, rozmiarCzesci, MPI_INT, 1, 0, COMM_workG1, MPI_STATUS_IGNORE);
			cout << lokalna[2] << endl;

		}
		// for(int i=1; i<rozmiarCzesci; i++)
		// 	if(tab[i] > max)
		// 		max = tab[i];
		// cout << max << endl;
		// MPI_Reduce(&maxL, &maxG, 1, MPI_INT, MPI_MAX, 0, COMM_workG1);
		// if(rankL == 0)
		// 	cout << maxG << endl;
	}
	// cout << masterG1 << masterG2 << "!!!!" << endl;
	// MPI_Group_incl(wszystkie, 3, masters, &mastersG);
	MPI_Finalize();
}