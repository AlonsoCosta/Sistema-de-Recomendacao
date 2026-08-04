#ifndef SIMILARIDADE_H
#define SIMILARIDADE_H

#include <vector>
using namespace std;

int** matrizCompras(vector<vector<int>>& listaDeCompras, int n, int m);

int** intersecaoNaive(int** a, int n, int m);

int** intersecaoEficiente(int** a, int n, int m);

double** calcularMatrizes(int** I, vector<vector<int>>& listaDeCompras, int n);

int clienteSimilares(double** s, int cliente, int n);

void liberarMatrizInt(int** matriz, int linhas);

void liberarMatrizDouble(double** matriz, int linhas);

typedef struct {
    double* values;
    int* col_index;
    int* row_ptr;
    int nnz; // Guarda o número exato de elementos não-nulos
} CSR;

CSR construirCSR(vector<vector<int>>& listaDeCompras, int n, int m);

CSR intersecaoCSR(CSR& csr, int n);

double** calcularMatrizesCSR(CSR& I, vector<vector<int>>& listaDeCompras, int n);

void liberarCSR(CSR& csr);

#endif