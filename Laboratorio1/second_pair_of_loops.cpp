#include <stdio.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <vector>

using namespace std;
typedef vector<vector<long>> matrix;
typedef vector<long> line_matrix;

#define LIMITE 100;
const int MATRIX_SIZE = 100;

#define max(a,b) \
  ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b; })
#define min(a,b) \
  ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b; })



clock_t fstart, fstop;
#define START if ( (fstart = clock()) == -1) {printf("Error en llamada a clock");exit(1);}
#define STOP if ( (fstop = clock()) == -1) {printf("Error en llamada a clock");exit(1);}
#define PRINTTIME printf( "%6.3f seconds usados por el procesador.", ((double)fstop-fstart)/CLOCKS_PER_SEC);





void print_matx(matrix m)
{

    int i,j =0;
    for( i=0;i<MATRIX_SIZE;i++)
    {
        for( j=0;j<MATRIX_SIZE;j++)
        {
            printf("%d\t",m[i][j]);
        }
        printf("\n");
    }

}

void firstpairofloops(matrix A, line_matrix x,line_matrix y)
{

    int i,j;
    for(i =0;i<MATRIX_SIZE;i++)
    {
        for(j=0;j<MATRIX_SIZE;j++)
        {
            y[i]+=A[i][j]*x[j];
        }
    }

}

void secondpairofloops(matrix A, line_matrix x,line_matrix y)
{

    int i,j;
    for(j =0;j<MATRIX_SIZE;j++)
    {
        for(i=0;i<MATRIX_SIZE;i++)
        {
            y[i] +=A[i][j]*x[j];
        }
    }
}

void matrixmultiplication(matrix a, matrix b,matrix result)
{
    int c,k,d,sum;
    for (c = 0; c < MATRIX_SIZE; c++)
    {
        for (d = 0; d < MATRIX_SIZE; d++)
        {
            for (k = 0; k < MATRIX_SIZE; k++)
            {
              sum = sum + a[c][k]*b[k][d];
            }
        result[c][d] = sum;
        sum = 0;
        }
    }
}
void sixloop(matrix a, matrix b,matrix c,long block_size)
{

    int i,ii,j,jj,k,kk=0;
    for (k = 0; k < MATRIX_SIZE; k += block_size)
        for (j = 0; j < MATRIX_SIZE; j += block_size)
            for (i = 0; i < MATRIX_SIZE; ++i)
                for (jj = j; jj < min(j + block_size, MATRIX_SIZE); ++jj)
                    for (kk = k; kk < min(k + block_size, MATRIX_SIZE); ++kk)
                        c[i][jj] += a[i][kk] * b[kk][jj];

}
void matrix_resize(matrix &A, int fil, int col)
{
    A.resize(fil);
    for(int i=0;i<fil;i++)
    {
        A[i].resize(col);
    }
}

int main(void)
{
    srand(time(NULL));

    line_matrix m_x;
    line_matrix m_y;
    matrix matx_uno;
    matrix matx_dos;
    matrix resultado;


    m_x.resize(MATRIX_SIZE);
    m_y.resize(MATRIX_SIZE);
    matrix_resize(matx_uno,MATRIX_SIZE,MATRIX_SIZE);
    matrix_resize(matx_dos,MATRIX_SIZE,MATRIX_SIZE);
    matrix_resize(resultado,MATRIX_SIZE,MATRIX_SIZE);


    for(int i=0;i<MATRIX_SIZE;i++)
    {
        for(int j=0;j<MATRIX_SIZE;j++)
        {
            long k=rand()%LIMITE;
            long h=rand()%LIMITE;
            matx_uno[i][j]=k;
            matx_dos[i][j]=h;
        }

    }
    for(int i=0;i<MATRIX_SIZE;i++)
    {
        long k=rand()%LIMITE;
        long h=rand()%LIMITE;
        m_x[i]=k;
        m_y[i]=h;
    }
/*
    printf("Fisrt pair of loops");
    START;
    firstpairofloops(matx_uno,m_x,m_y);
    STOP;
    PRINTTIME;

    printf("\n");
*/

    printf("Second pair of loops");
    START;
    secondpairofloops(matx_uno,m_x,m_y);
    STOP;
    PRINTTIME;
    printf("\n");
/*

    printf("3 loops matrix mult");
    START;
    matrixmultiplication(matx_uno,matx_dos,resultado);
    STOP;
    PRINTTIME;
    printf("\n");

    printf("blocked matrix-multiply algorithm");
    START;
    sixloop(matx_dos,matx_dos,resultado,2);
    STOP;
    PRINTTIME;
    printf("\n");
*/
    return 0;
}

