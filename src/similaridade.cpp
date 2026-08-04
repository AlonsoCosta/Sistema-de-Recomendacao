#include "similaridade.h"
#include <cstdlib>
#include <algorithm>

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

int** intersecaoNaive(int** a, int n, int m) {
    int** I = (int**) malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        I[i] = (int*) malloc(n * sizeof(int));
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int soma = 0;

            for (int k = 0; k < m; k++) {
                soma = soma + a[i][k] * a[j][k];
            }

            I[i][j] = soma;
        }
    }

    return I;
}

int** intersecaoEficiente(int** a, int n, int m) {
    int** I = (int**) malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        I[i] = (int*) malloc(n * sizeof(int));
    }

    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            int soma = 0;

            for (int k = 0; k < m; k++) {
                soma = soma + a[i][k] * a[j][k];
            }

            I[i][j] = soma;
            I[j][i] = soma;
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

CSR construirCSR(vector<vector<int>>& listaDeCompras, int n, int m) {
    CSR csr;
    
    int totalElementos = 0;
    for (int i = 0; i < n; i++) {
        totalElementos += listaDeCompras[i].size();
    }
    
    csr.nnz = totalElementos;
    csr.values = (double*) malloc(totalElementos * sizeof(double));
    csr.col_index = (int*) malloc(totalElementos * sizeof(int));
    csr.row_ptr = (int*) malloc((n + 1) * sizeof(int));

    csr.row_ptr[0] = 0;
    int cont = 0;

    for (int i = 0; i < n; i++) {
        vector<int> produtosOrdenados = listaDeCompras[i];
        sort(produtosOrdenados.begin(), produtosOrdenados.end());

        int totalProdutos = produtosOrdenados.size();
        for (int k = 0; k < totalProdutos; k++) {
            csr.values[cont] = 1.0; 
            csr.col_index[cont] = produtosOrdenados[k];
            cont++;
        }
        
        csr.row_ptr[i + 1] = cont;
    }

    return csr;
}

CSR intersecaoCSR(CSR& csr, int n) {
    CSR resultado;
    
    int capacidade = 1000; 
    resultado.values = (double*) malloc(capacidade * sizeof(double));
    resultado.col_index = (int*) malloc(capacidade * sizeof(int));
    resultado.row_ptr = (int*) malloc((n + 1) * sizeof(int));
    
    resultado.row_ptr[0] = 0;
    int cont = 0;

    for (int i = 0; i < n; i++) {
        int inicioI = csr.row_ptr[i];
        int fimI = csr.row_ptr[i + 1];

        for (int j = 0; j < n; j++) {
            int inicioJ = csr.row_ptr[j];
            int fimJ = csr.row_ptr[j + 1];
            int ponteiroI = inicioI;
            int ponteiroJ = inicioJ;
            double soma = 0.0;

            while (ponteiroI < fimI && ponteiroJ < fimJ) {
                if (csr.col_index[ponteiroI] < csr.col_index[ponteiroJ]) {
                    ponteiroI++;
                } else if (csr.col_index[ponteiroI] > csr.col_index[ponteiroJ]) {
                    ponteiroJ++;
                } else {
                    soma = soma + csr.values[ponteiroI] * csr.values[ponteiroJ];
                    ponteiroI++;
                    ponteiroJ++;
                }
            }
            
            if (soma > 0.0) {
                if (cont >= capacidade) {
                    capacidade *= 2; 
                    resultado.values = (double*) realloc(resultado.values, capacidade * sizeof(double));
                    resultado.col_index = (int*) realloc(resultado.col_index, capacidade * sizeof(int));
                }
                
                resultado.values[cont] = soma;
                resultado.col_index[cont] = j;
                cont++;
            }
        }
        resultado.row_ptr[i + 1] = cont;
    }

    resultado.values = (double*) realloc(resultado.values, cont * sizeof(double));
    resultado.col_index = (int*) realloc(resultado.col_index, cont * sizeof(int));
    resultado.nnz = cont;

    return resultado;
}
void liberarCSR(CSR& csr) {
    if (csr.values != NULL) free(csr.values);
    if (csr.col_index != NULL) free(csr.col_index);
    if (csr.row_ptr != NULL) free(csr.row_ptr);
}

double** calcularMatrizesCSR(CSR& I, vector<vector<int>>& listaDeCompras, int n) {
    double** s = (double**) malloc(n * sizeof(double*));
    for (int i = 0; i < n; i++) {
        s[i] = (double*) malloc(n * sizeof(double));
        for (int j = 0; j < n; j++) {
            s[i][j] = 1.0; 
        }
    }

    for (int i = 0; i < n; i++) {
        int totalProdutosClienteI = listaDeCompras[i].size();
        int inicio = I.row_ptr[i];
        int fim = I.row_ptr[i + 1];

        for (int pos = inicio; pos < fim; pos++) {
            int j = I.col_index[pos];
            double intersecao = I.values[pos];

            if (totalProdutosClienteI > 0) {
                s[i][j] = 1.0 - (intersecao / (double) totalProdutosClienteI);
            }
        }
    }

    return s;
}