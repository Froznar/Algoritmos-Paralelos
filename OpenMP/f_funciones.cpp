#include "f_funciones.h"

//-------------------------------------------------------
//-------------------------GLOBAL------------------------
//-------------------------------------------------------
clock_t fstart, fstop;
#define START if ( (fstart = clock()) == -1) {printf("Error en llamada a clock");exit(1);}
#define STOP if ( (fstop = clock()) == -1) {printf("Error en llamada a clock");exit(1);}
#define PRINTTIME printf( "%6.3f seconds usados por el procesador.", ((double)fstop-fstart)/CLOCKS_PER_SEC);

void p_vector_int(vector<int> V)
{
    for(int i=0;i<V.size();i++)
    {
        cout<<V[i]<<", ";
    }
}

//********************************************************
//********************************************************

//-------------------------------------------------------
//-------------------------TRAPEZ------------------------
//-------------------------------------------------------
double s_Trap_f(double x)
{
    return x;
}

double s_Trap(double a, double b, double n, double& sum_global)
{
    double local_b, local_a, h, x, resul;
    int local_n;

    int my_rank = omp_get_thread_num();
    int thread_cont = omp_get_num_threads();

    h=(b-a)/n;
    local_n = n/thread_cont;
    local_a = a+ my_rank * local_n * h;
    local_b = local_a + local_n * h;
    resul = (s_Trap_f(local_a) + s_Trap_f(local_b))/2.0;

    for(int i=0; i<local_n-1; i++)
    {
        x = local_a + i * h;
        resul += s_Trap_f(x);
    }
    resul = resul * h;

    #pragma omp critical
        sum_global += resul;
}

void f_Trap_Rule()
{
    double sum_global, a, b;
    int n, thread_count;

    cout<<"Numero de threads :";
    cin>>thread_count;
    cout<<"Ingresar a,b y n :"<<endl;
    cin>>a>>b>>n;
    #pragma omp parallel num_threads(thread_count)
        s_Trap(a,b,n,sum_global);

    cout<<"Resultado : "<<sum_global<<endl;
}
//********************************************************
//********************************************************

//-------------------------------------------------------
//---------------------PI ESTIMATION---------------------
//-------------------------------------------------------
void f_Pi_estimation()
{
    double sum=0, factor=1;
    int k, n, thread_count;

    cout<<"Numero de threads :";
    cin>>thread_count;
    cout<<"Numero de iteraciones :";
    cin>>n;
    #pragma omp parallel for num_threads(thread_count) default(none) reduction(+:sum) private(k, factor) shared(n)
        for(k=0;k<n;k++)
        {
            if(k%2 == 0)
                factor = 1;
            else
                factor = -1;

            sum+=factor/(2*k+1);
        }

     cout<<"Aproximado :"<< 4.0*sum<<endl;
}
//********************************************************
//********************************************************

//-------------------------------------------------------
//---------------------Odd Even Sort---------------------
//-------------------------------------------------------
void f_Odd_even(int modo)
{
    srand(time(NULL));

    int n, j, thread_count;
    vector<int> v_array;
    cout<<"Numero de threads :";
    cin>>thread_count;
    cout<<"Numero de elementos :";
    cin>>n;

    for(int i=0;i<n;i++)
    {
        v_array.push_back(rand()%n);
    }

    if(modo == 1)
    {
        for(int phase=0;phase<n;phase++)
        {
            if(phase%2==0)
            {
                #pragma omp parallel for num_threads(thread_count) default(none) shared(v_array,n) private(j)
                    for(j=1;j<n;j+=2)
                    {
                        if(v_array[j-1] > v_array[j])
                            swap(v_array[j-1], v_array[j]);
                    }
            }
            else
            {
                #pragma omp parallel for num_threads(thread_count) default(none) shared(v_array,n) private(j)
                    for(j=1;j<n-1;j+=2)
                    {
                        if(v_array[j] > v_array[j+1])
                            swap(v_array[j], v_array[j+1]);
                    }
            }
        }


    }
    if(modo == 2)
    {
        int phase;
        #pragma omp parallel num_threads(thread_count) default(none) shared(v_array,n) private(j, phase)
            for(phase=0;phase<n;phase++)
            {
                if(phase%2==0)
                {
                    #pragma omp for
                        for(j=1;j<n;j+=2)
                        {
                            if(v_array[j-1] > v_array[j])
                                swap(v_array[j-1], v_array[j]);
                        }
                }
                else
                {
                    #pragma omp for
                        for(j=1;j<n-1;j+=2)
                        {
                            if(v_array[j] > v_array[j+1])
                                swap(v_array[j], v_array[j+1]);
                        }
                }
            }


    }
    p_vector_int(v_array);
}
//********************************************************
//********************************************************

//-------------------------------------------------------
//--------------------Matrix x Vetor---------------------
//-------------------------------------------------------
void s_matrix_resize(vector<vector<long> > &A, int fil, int col)
{
    A.resize(fil);
    for(int i=0;i<fil;i++)
    {
        A[i].resize(col);
    }
}
void s_print_vector(vector<long> matrix)
{
    for(int j=0;j<matrix.size();j++)
        cout<<matrix[j]<<"~";
}
void s_print_matrix(vector<vector<long>> matrix)
{
    for(int i=0;i<matrix.size();i++)
    {
        for(int j=0;j<matrix[0].size();j++)
            cout<<matrix[i][j]<<"~";
        cout<<endl;
    }
}

void f_Matrix_vector()
{
    srand(time(NULL));
    vector<vector<long>> matrix;
    vector<long> line_x;
    vector<long> line_y;
    int LIMITE=10;

    int m,n, thread_count;
    cout<<"Numero de threads :";
    cin>>thread_count;
    cout<<"M*N :";
    cin>>m>>n;

    line_x.resize(m);
    line_y.resize(m);
    s_matrix_resize(matrix,m,n);
    for(int i=0;i<m;i++)
    {
        for(int j=0;j<n;j++)
        {
            long k=rand()%LIMITE;
            matrix[i][j]=k;
        }
    }
    for(int i=0;i<m;i++)
    {
        long k=rand()%LIMITE;
        line_x[i]=k;
    }

    int i,j;
    START;
    #pragma omp parallel for num_threads(thread_count) default(none) private(i,j) shared(matrix,line_x,line_y,m,n)
    for(i=0;i<m;i++)
    {
        for(j=0;j<n;j++)
            line_y[i]+=matrix[i][j]*line_x[j];
    }
    STOP;
    PRINTTIME;
    //s_print_vector(line_y);
}

/* TABLA
 * threads  | Dimenciones
 *          | 8000000*8 | 8000*8000 | 8*8000
 *          | Tiempo    | Tiempo    | Tiempo
 *  1       |   1.005   |   0.972   |  0.004
 *  2       |   1.050   |   1.104   |  0.001
 *  4       |   1.784   |   1.719   |  0.019

*/
//********************************************************
//********************************************************
