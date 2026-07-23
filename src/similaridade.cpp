#include "similaridade.h"
#include <cstdlib>

int** matrizCompras(vector<vector<int>>& listaDeCompras, int n, int m) {
    int** a = (int**) malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        a[i] = (int*) malloc(m * sizeof(int));
        for (int j = 0; j < m; j++) {
            a[i][j] = 0;
        }
    }

    for (int i = 0; i < n; i++) {
        int totalComprasDoCliente = listaDeCompras[i].size();

        for (int k = 0; k < totalComprasDoCliente; k++) {
            int indiceProduto = listaDeCompras[i][k];
            a[i][indiceProduto] = 1;
        }
    }

    return a;
}

int** matrizTransposta(int** a, int n, int m) {
    int** at = (int**) malloc(m * sizeof(int*));
    for (int i = 0; i < m; i++) {
        at[i] = (int*) malloc(n * sizeof(int));
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            at[j][i] = a[i][j];
        }
    }

    return at;
}

int** multiplicarMatrizes(int** a, int** at, int n, int m) {
    int** I = (int**) malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        I[i] = (int*) malloc(n * sizeof(int));
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int soma = 0;

            for (int k = 0; k < m; k++) {
                soma = soma + a[i][k] * at[k][j];
            }

            I[i][j] = soma;
        }
    }

    return I;
}

double** calcularMatrizes(int** I, vector<vector<int>>& listaDeCompras, int n) {
    double** s = (double**) malloc(n * sizeof(double*));
    for (int i = 0; i < n; i++) {
        s[i] = (double*) malloc(n * sizeof(double));
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

    return s;
}

int clienteSimilares(double** s, int cliente, int n) {
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

void liberarMatrizInt(int** matriz, int linhas) {
    for (int i = 0; i < linhas; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

void liberarMatrizDouble(double** matriz, int linhas) {
    for (int i = 0; i < linhas; i++) {
        free(matriz[i]);
    }
    free(matriz);
}