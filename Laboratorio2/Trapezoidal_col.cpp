#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

double fun(double x)
{
	return x*x;
}

double Trap(double a, double b, double n, double h)
{
	double apx=0,x_i=0;
	
	for(int i=1;i<n-1;i++)
	{
		x_i=a+i*h;
		apx+=fun(x_i);
	}
	return (b-a)*((fun(a) + 2*apx + fun(b)) / (2*n));
}

int main(int argc, char** argv)
{
  const int PING_PONG_LIMIT = 10;


  int world_size;
  int world_rank;
  double a=0,b=3.0,n=1024;
  double h,local_a,local_b,local_n;
  double local_int,total;
  
  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	h=(b-a)/n;
	local_n = n/world_size;
	
	local_a = a+world_rank*local_n*h;
	local_b = local_a+local_n*h;
	local_int = Trap(local_a,local_b,local_n,h);
	
	MPI_Reduce(&local_int, &total, 1, MPI_DOUBLE, MPI_SUM, 0,MPI_COMM_WORLD);
    
	if(world_rank==0)
	{
		printf("Con n = %f trapezios \n", n);
		printf("Entre %f to %f El resultdo es = %.15e \n",a,b,total);
	}
	
	MPI_Finalize();
	return 0;
	
}
