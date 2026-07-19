#include "similaridade.h"

void matrizCompras(vector<vector<int>>& listaDeCompras, int n, int m, vector<vector<int>>& a) {

    for (int i = 0; i < n; i++) {
        vector<int> linhaVazia;
        for (int j = 0; j < m; j++) {
            linhaVazia.push_back(0);
        }
        a.push_back(linhaVazia);
    }

    for (int i = 0; i < n; i++) {
        int totalComprasDoCliente = listaDeCompras[i].size();

        for (int k = 0; k < totalComprasDoCliente; k++) {
            int indiceProduto = listaDeCompras[i][k];
            a[i][indiceProduto] = 1;
        }
    }
}

void matrizTransposta(vector<vector<int>>& a, vector<vector<int>>& at, int n, int m) {

    for (int i = 0; i < m; i++) {
        vector<int> linhaVazia;
        for (int j = 0; j < n; j++) {
            linhaVazia.push_back(0);
        }
        at.push_back(linhaVazia);
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            at[j][i] = a[i][j];
        }
    }
}

void multiplicarMatrizes(vector<vector<int>>& a, vector<vector<int>>& at, int n, int m, vector<vector<int>>& I) {

    for (int i = 0; i < n; i++) {
        vector<int> linhaVazia;
        for (int j = 0; j < n; j++) {
            linhaVazia.push_back(0);
        }
        I.push_back(linhaVazia);
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int soma = 0;

            for (int k = 0; k < m; k++) {
                soma = soma + a[i][k]  * at[k][j];
            }

            I[i][j] = soma;
        }
    }
}

void calcularMatrizes(vector<vector<int>>& I, vector<vector<int>>& listaDeCompras, int n, vector<vector<double>>& s) {

    for (int i = 0; i < n; i++) {
        vector<double> linhaVazia;
        for(int j = 0; j < n; j++) {
            linhaVazia.push_back(0.0);
        }
        s.push_back(linhaVazia);
    }

    for (int i = 0; i < n; i++) {
        int totalProdutosClienteI = listaDeCompras[i].size();

        for (int j = 0; j < n; j++) {
            if (totalProdutosClienteI > 0) {
                s[i][j] = 1.0 - ((double) I[i][j] / (double) totalProdutosClienteI);
            } else {
                s[i][j] = 1.0;
            }
        }
    } 
}

int clienteSimilares(vector<vector<double>>& s, int cliente, int n) {

    int indiceSimilar = -1;
    double menorValor = 0.0;
    int semCandidato = 1;

    for (int j = 0; j < n; j++) {
        if (j != cliente) {
            if (semCandidato == 1) {
                menorValor = s[cliente][j];
                indiceSimilar = j;
                semCandidato = 0;
            } else if (s[cliente][j] < menorValor) {
                menorValor = s[cliente][j];
                indiceSimilar = j;
            }
        }
    }
    return indiceSimilar;
}