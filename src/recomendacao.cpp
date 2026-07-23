#include "recomendacao.h"
#include <algorithm>

void identificarVizinhos(double** S, int c, int n, vector<int>& vizinhos) {
    for (int s = 0; s < n; s++) {
        if (s != c && S[c][s] < 1.0) {
            vizinhos.push_back(s);
        }
    }
}

bool compararPorScore(const Recomendacao& a, const Recomendacao& b) {
    return a.score < b.score;
}

void gerarRecomendacoes(int c, int k, double** S, int** A,
    int n, int m, vector<Recomendacao>& recomendacoesFinais) {

    vector<int> vizinhos;
    identificarVizinhos(S, c, n, vizinhos);

    vector<Recomendacao> ranqueamento;
    for (int p = 0; p < m; p++) {
        Recomendacao r;
        r.idProduto = p;
        r.score = 1.0;
        ranqueamento.push_back(r);
    }

    int totalVizinhos = vizinhos.size();
    for (int i = 0; i < totalVizinhos; i++) {
        int s = vizinhos[i];

        for (int p = 0; p < m; p++) {
            if (A[s][p] == 1 && A[c][p] == 0) {
                ranqueamento[p].score = ranqueamento[p].score * S[c][s];
            }
        }
    }

    vector<Recomendacao> filtrados;
    int totalProdutos = ranqueamento.size();
    for (int p = 0; p < totalProdutos; p++) {
        if (ranqueamento[p].score < 1.0) {
            filtrados.push_back(ranqueamento[p]);
        }
    }

    sort(filtrados.begin(), filtrados.end(), compararPorScore);

    int totalFiltrados = filtrados.size();
    int limite = (k < totalFiltrados) ? k : totalFiltrados;

    for (int i = 0; i < limite; i++) {
        recomendacoesFinais.push_back(filtrados[i]);
    }
}