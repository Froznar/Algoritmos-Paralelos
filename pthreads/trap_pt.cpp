#include <iostream>
#include <pthread.h>
#include <math.h>
#include <time.h>
#include <vector>


using namespace std;



clock_t startm, stopm;
#define START if ( (startm = clock()) == -1) {printf("Error calling clock");exit(1);}
#define STOP if ( (stopm = clock()) == -1) {printf("Error calling clock");exit(1);}
#define PRINTTIME printf( "%6.3f seconds used by the processor.", ((double)stopm-startm)/CLOCKS_PER_SEC);

/*suma global*/
double sum=0;
const int SIZE=100000000;
const int num_threads=64;


int world_size;
int world_rank;
double a=0,b=3.0,n=1024;
double h,local_a,local_b,local_n;



pthread_mutex_t mutex;
long flag=0;

void print_vect(double m[SIZE])
{
    cout<<"**Vector**"<<endl;
    for(int i=0;i<SIZE;i++)
    {
        cout<<m[i]<<" | ";
    }
    cout<<endl;
}

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

void *Div_op(double a, double b, double n, double h)
{
    local_n = n/world_size;
    local_a = a+world_rank*local_n*h;
    local_b = local_a+local_n*h;
    local_int = Trap(local_a,local_b,local_n,h);
    total+=local_int;
}

void* Thread_sum_busy(void* rank) {
    long my_rank = (long) rank;
    double local_int = 0.0;
    

    local_n = n/num_threads;
    local_a = a+my_rank*local_n*h;
    local_b = local_a+local_n*h;
    local_int = Trap(local_a,local_b,local_n,h);
    
    

    while (flag != my_rank);
    
    sum += local_int;
    flag = (flag+1) % num_threads;
    
}


void* Thread_sum(void* rank)
{
    long my_rank = (long) rank;
    double local_int = 0.0;
    

    local_n = n/num_threads;
    local_a = a+my_rank*local_n*h;
    local_b = local_a+local_n*h;
    local_int = Trap(local_a,local_b,local_n,h);
        
    pthread_mutex_lock(&mutex);
    sum += my_sum;
    pthread_mutex_unlock(&mutex);
}



int main()
{
    srand(time(NULL));



    pthread_t threads[num_threads];

    START;
    for (long thread = 0; thread < num_threads; thread++)
       pthread_create(&threads[thread], NULL, Thread_sum, (void*) thread);

    for (long thread = 0; thread < num_threads; thread++)
       pthread_join(threads[thread], NULL);
    STOP;
    PRINTTIME;

    cout<<"EL resultado es"<<sum<<endl;



    return 0;
} 
