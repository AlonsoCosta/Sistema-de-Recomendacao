#ifndef RECOMENDACAO_H
#define RECOMENDACAO_H

#include <vector>
using namespace std;

typedef struct {
    int idProduto;
    double score;
} Recomendacao;

void identificarVizinhos(vector<vector<double>>& S, int c, int n, vector<int>& vizinhos);

bool compararPorScore(const Recomendacao& a, const Recomendacao& b);

void gerarRecomendacoes(int c, int k, vector<vector<double>>& S, vector<vector<int>>& A,
    int n, int m, vector<Recomendacao>& recomendacoesFinais);

#endif