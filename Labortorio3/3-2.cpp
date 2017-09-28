#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <random>

using namespace std;


int main()
{

  int world_size;
  int world_rank;
  double tosses,x,y;
  double pi_est,total;
  double number_in_circle=0;
  
  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  if(world_rank==0)
  {
      cout<<"Ingresar tosses";
      cin>>tosses;
      
  }
    MPI_Bcast(&tosses, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	//srand(time(NULL) + world_rank);
    std::random_device rd;
    std::mt19937 gen(rd()+world_rank);
    std::uniform_real_distribution<> dis(-1,1);
	
    for(int i=0;i<tosses;i++)
    {
        x=dis(gen);
        y=dis(gen);
        double sqr=x*x+y*y;
        if(sqr<=1){number_in_circle++;}
    }
    
	
	MPI_Reduce(&number_in_circle, &total, 1, MPI_DOUBLE, MPI_SUM, 0,MPI_COMM_WORLD);
    
	if(world_rank==0)
	{
        pi_est = 4*number_in_circle/tosses;
        cout<<"la aproximacion es :"<<pi_est<<endl;
	}
	
	MPI_Finalize();
	return 0;
	
}
