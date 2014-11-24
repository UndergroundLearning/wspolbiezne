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

int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);
	srand(time(NULL));
	int rank, nRank, size, *tab = NULL, *nTab = NULL;
	int maxG, maxL;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if(argc != 2)
	{
		if(rank == 0)
			cerr << "Podaj n.." << endl;
		MPI_Finalize();
		return(0);
	} // spr argumentu wejsciowego
	int n = atoi(argv[1]); // pobranie n
	if(rank == 0) // zadania dla procesu nr 0
	{
		tab = new int[n * size];
		for(int i = 0; i<n*size; i++)
			tab[i] = rand()%100;
		nRank = n;
	}
	MPI_Bcast(&nRank, 1, MPI_INT, 0, MPI_COMM_WORLD);
	nTab = new int[nRank];
	MPI_Scatter(tab, nRank, MPI_INT, nTab, nRank, MPI_INT, 0, MPI_COMM_WORLD); //wysyła do wszystkich procesów tab i odbiera nTab
	maxL = max(nTab, nRank);
	MPI_Reduce(&maxL, &maxG, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD); //policzenie maxa globalnego z wszystkich czesci
	if(rank == 0) //wyswietlenie go raz nie tyle razy ile procesow
		cout << endl << "Maksimum globalne wynosi " << maxG << endl;
	
	//ZWALNIANIE PAMIECI
	if(tab) 
		delete[] tab;
	if(nTab)
		delete[] nTab;

	MPI_Finalize();
}
