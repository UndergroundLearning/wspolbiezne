#include <iostream>
#include "mpi.h"
#include <ctime>
#include <stdlib.h>

using namespace std;

double max(const double &a, const double &b) {
    return (a>b)?a:b;
}



int main(int argc, char **argv)
{

    int size, rank, maxRank, maxAll;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rozG;
    int rozL;
    int *tabG = NULL;
    int *tabL = NULL;

    MPI_Status status;


    if(rank ==0) {

        srand (time(NULL));
        rozG=100;
        rozL = rozG/size;

        tabG = new int [rozG];
        for(int i = 0; i<rozG; ++i)
            tabG[i]=i;

    }
    MPI_Bcast(&rozL, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if(rank ==0) {

        for(int i=1; i<size; ++i){
            MPI_Send (&tabG[i*rozL],rozL,MPI_INT,i,100,MPI_COMM_WORLD);
        }

        maxRank = tabG[0];
        for(int i=1; i<rozL; ++i){
            maxRank=max(maxRank, tabG[i]);
        }


    } else {
        tabL=new int[rozL];
        MPI_Recv (&tabL[0],rozL,MPI_INT,0,100,MPI_COMM_WORLD,&status);

        maxRank = tabL[0];
        for(int i=1; i<rozL; ++i){
            maxRank=max(maxRank, tabL[i]);
        }
    }

    cout<<rank<<" "<<maxRank<<endl;
    //MPI_Reduce(&maxRank, &maxAll, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD );
    MPI_Allreduce(&maxRank, &maxAll, 1, MPI_INT, MPI_MAX,  MPI_COMM_WORLD );
    cout<<rank<<" global "<<maxAll<<endl;

    if(tabG!=NULL) delete [] tabG;
    if(tabL!=NULL) delete [] tabL;

    MPI_Finalize();
    return 0;
}
