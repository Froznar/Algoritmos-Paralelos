#ifndef F_FUNCIONES_H
#define F_FUNCIONES_H


#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <iostream>
#include <vector>
#include <time.h>
using namespace std;



//-------------------------GLOBAL------------------------
void p_vector_int(vector<int> V);

//-------------------------TRAPEZ------------------------
double s_Trap_f(double x);
double s_Trap(double a, double b, double n, double& sum_global);
void f_Trap_Rule();
//********************************************************

//---------------------PI ESTIMATION---------------------
void f_Pi_estimation();
//********************************************************

//---------------------Odd Even Sort---------------------
void f_Odd_even(int modo);
//********************************************************

//--------------------Matrix x Vetor---------------------
void s_matrix_resize(vector<vector<long>> &A, int fil, int col);
void s_print_vector(vector<long> matrix);
void s_print_matrix(vector<vector<long>> matrix);
void f_Matrix_vector();
//********************************************************


#endif // F_FUNCIONES_H
