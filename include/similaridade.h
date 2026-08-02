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

#endif