#include <iostream>
#include <mpi.h>
#include <cstdlib>

using namespace std;

int max(int*& tab, int n)
{
	int max = tab[0];
	for(int i=1; i<n; ++i)
		if(tab[i] > max)
			max = tab[i];
	return max;
}

int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);
	srand(time(NULL));
	int rank, nRank, size, *tab = NULL, *nTab = NULL, n = atoi(argv[1]);
	int maxG, maxL;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if(rank == 0)
	{
		
		bool niepodzielne = false;
		tab = new int[n];
		for(int i=0; i<n; i++)
		{
			tab[i] = rand() % 100;
		}
		cout << endl;
		int rozmiarBloku = (int) (n/size);
		if(n % size != 0)
		{
			rozmiarBloku += 1;
			niepodzielne = true;
		}
		for(int i=0; i<size; ++i)
		{
			int k;
			int dolna = i * rozmiarBloku;
			int gorna = ((i+1) * rozmiarBloku) - 1;
			if(i == size-1 && niepodzielne)
			{
				gorna = n-1;
			}
			nTab = new int[gorna-dolna];
			for(k=dolna, nRank=0; k<=gorna; k++, nRank++)
			{
				nTab[nRank] = tab[k];
			}
			// cout << "Czesc " << i << " od " << dolna << " do " << gorna << " Rozmiar: " << nRank << endl;
			if(i == 0)
			{
				maxL = max(nTab, nRank);
				cout << "Czesc: " << i << " Rozmiar: " << nRank << " Max lokalny: " << maxL << endl; 
			}
			else
			{
				MPI_Send(&nRank, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
				MPI_Send(nTab, nRank, MPI_INT, i, 0, MPI_COMM_WORLD);
			}
		}
	}
	else
	{
		MPI_Recv(&nRank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		nTab = new int[nRank];
		MPI_Recv(nTab, nRank, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		maxL = max(nTab, nRank);
		cout << "Czesc: " << rank << " Rozmiar: " << nRank << " Max lokalny: " << maxL << endl; 
	}
	
	MPI_Reduce(&maxL, &maxG, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
	if(rank == 0)
		cout << endl << "Maksimum globalne wynosi " << maxG << endl;
	
	if(tab)
		delete[] tab;
	if(nTab)
		delete[] nTab;

	MPI_Finalize();
}