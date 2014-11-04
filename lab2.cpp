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
		return(EXIT_FAILURE);
	} // spr argumentu wejsciowego
	int n = atoi(argv[1]); // pobranie n
	if(rank == 0) // zadania dla procesu nr 0
	{
		bool niepodzielne = false;
		tab = new int[n];
		for(int i=0; i<n; i++)
		{
			tab[i] = rand() % 100;
		}
		cout << endl;
		int rozmiarBloku = (int) (n/size); //wstepny rozmiar bloku
		if(n % size != 0) //jak niepodzielne dodaje j do rozmiaru bloku
		{
			rozmiarBloku += 1;
			niepodzielne = true;
		}
		for(int i=0; i<size; ++i) //petla po wszystkich procesach
		{
			int k;
			int dolna = i * rozmiarBloku; //dolna granica i * rozmiarBloku (ex. rB = 10) to 
						      //dla i=0 bedzie dolna = 0 i gorna = 9 itd
			int gorna = ((i+1) * rozmiarBloku) - 1; //jw
			if(i == size-1 && niepodzielne) // jak ostatnia porcja i niepodzielne to gorna = ostatni element
			{
				gorna = n-1;
			}
			nTab = new int[gorna-dolna];
			for(k=dolna, nRank=0; k<=gorna; k++, nRank++) // tworzenie czesci wg granic wyzej
			{
				nTab[nRank] = tab[k];
			}
			// cout << "Czesc " << i << " od " << dolna << " do " << gorna << " Rozmiar: " << nRank << endl;
			if(i == 0) // proces 0 sobie od razu liczy bez wysylania nigdzie
			{
				maxL = max(nTab, nRank);
				cout << "Czesc: " << i << " Rozmiar: " << nRank << " Max lokalny: " << maxL << endl; 
			}
			else //do innych procesow trzeba wyslac rozmiar czesci sama czesc
			{
				MPI_Send(&nRank, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
				MPI_Send(nTab, nRank, MPI_INT, i, 0, MPI_COMM_WORLD);
			}
		}
	}
	else //robota dla innych procesow
	{
		MPI_Recv(&nRank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); //odebrac rozmiar czesc
		nTab = new int[nRank]; //zaalokowac pamiec
		MPI_Recv(nTab, nRank, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); //odebrac czesc
		maxL = max(nTab, nRank); //policzyc maxa
		cout << "Czesc: " << rank << " Rozmiar: " << nRank << " Max lokalny: " << maxL << endl; 
	}
	
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
