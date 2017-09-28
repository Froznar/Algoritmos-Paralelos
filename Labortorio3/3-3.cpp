#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <random>

using namespace std;

int main(){
 
    int world_size;
    int world_rank;
      
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    
    
    double global_sum=0;
    double sub_sum=0;
    
    std::random_device rd;
    std::mt19937 gen(rd()+world_rank);
    std::uniform_int_distribution<int> dis(0,100);
    sub_sum=dis(gen);
    global_sum=sub_sum;
    cout<<global_sum<<endl;
    
    int k;
    MPI_Barrier(MPI_COMM_WORLD);
    for(k=1;(world_size/k)<=1;k*=2);
	{
        
        int ind=world_size/k;
        for(int j=1;j<ind;j+=2)
        {
            if(world_rank==j)
            {
                MPI_Send(&global_sum, 1, MPI_DOUBLE, j-1, 0, MPI_COMM_WORLD);
                cout<<"send"<<endl;
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);
        for(int j=1;j<ind;j+=2)
        {
            if(world_rank==j-1)
            {
                MPI_Recv(&sub_sum, 1, MPI_DOUBLE, j, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                global_sum+=sub_sum;
                cout<<"resc"<<endl;
            }
            
        }
        MPI_Barrier(MPI_COMM_WORLD);
	}
    
    if(world_rank==0)
	{       
        cout<<"la suma es :"<<global_sum<<endl;
	}
    MPI_Finalize();
    return 0;
}
