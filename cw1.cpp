#include <iostream>
#include <mpi.h>
#include <cstdlib>

using namespace std;

int Max(int *&tab, int n) {
	int max = tab[0];
	for(int i=0; i<n; i++) {
		if(tab[i] > max) {
			max = tab[i];
		}
	}
	return max;
}
int main(int argc, char **argv) {
	MPI_Init(&argc, &argv);
	int rank, size;
	int n = atoi(argv[1]);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int *tab=new int[n*size], *tab1=NULL, *tab2=NULL, *tabL1= new int [n], *tabL2= new int [n], *tabL = new int[n], maxG = 0, maxL = 0;
	int roz = n*size; 

	if(rank == 0) {
		srand(time(NULL));

		tab1 = new int[n*size];
		tab2 = new int[n*size];

		for(int i=0; i<n*size; i++) {
			tab1[i] = rand()%100;
			tab2[i] = rand()%100;
		}
	}
	MPI_Scatter(tab1, n, MPI_INT, tabL1, n, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(tab2, n, MPI_INT, tabL2, n, MPI_INT, 0, MPI_COMM_WORLD);

	for(int i =0; i<n; i++) {
		tabL[i] = tabL1[i] + tabL2[i];
	}
	maxL = Max(tabL, n);

	MPI_Gather(tabL, n, MPI_INT, tab, n, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Reduce(&maxL, &maxG, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
	if(rank == 0 ) {
		cout<<tab[3]<<endl;
		cout<<"Max "<<maxG<<endl;
	}

	MPI_Finalize(); 


	return 0;
}
