#include <vector>
using namespace std;

#ifndef SIMILARIDADE_H
#define SIMILARIDADE_H

void matrizCompras(vector<vector<int>>& listaDeCompras, int n, int m, vector<vector<int>>& a);

void matrizTransposta(vector<vector<int>>& a, vector<vector<int>>& at, int n, int m);

void multiplicarMatrizes(vector<vector<int>>& a, vector<vector<int>>& at, int n, int m, vector<vector<int>>& I);

void calcularMatrizes(vector<vector<int>>& I, vector<vector<int>>& listaDeCompras, int n, vector<vector<double>>& s);

int clienteSimilares(vector<vector<double>>& s, int cliente, int n);

#endif