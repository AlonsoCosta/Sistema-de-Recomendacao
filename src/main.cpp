#include "listaCompras.h"
#include "similaridade.h"
#include "recomendacao.h"
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main(int argc, char* argv[]) {

    if (argc < 3) {
        cout << "Uso: " << argv[0] << " <instancia.csv> <entrega:1|2|3> [k]\n";
        cout << "  entrega=1 -> Lista de Compras\n";
        cout << "  entrega=2 -> Lista de Compras e Similaridade\n";
        cout << "  entrega=3 -> Lista de Compras, Similaridade e Recomendacao\n";
        return 1;
    }

    char* instancia = argv[1];
    int entrega = atoi(argv[2]);

    if (entrega == 3 && argc != 4) {
        cout << "Erro: para entrega=3 (Recomendacao), informe tambem o k.\n";
        cout << "Uso: " << argv[0] << " <instancia.csv> 3 <k>\n";
        return 1;
    }

    int k = 5;
    if (argc == 4) {
        k = atoi(argv[3]);
    }

    vector<string> clientes;
    map<string, int> mapaClientes;
    vector<string> produtos;
    map<int, int> mapaProdutos;
    vector<vector<int>> listaDeCompras;

    cout << "Carregando o arquivo: " << instancia << "...\n";
    gerarLista(instancia, clientes, mapaClientes, produtos, mapaProdutos, listaDeCompras);

    printf("  -> Carregado. Total de clientes: %d | Total de produtos: %d\n",
           (int) clientes.size(), (int) produtos.size());

    vector<string> codigosParaTestar;
    int totalParaTestar = ((int) clientes.size() < 3) ? (int) clientes.size() : 3;
    for (int i = 0; i < totalParaTestar; i++) {
        codigosParaTestar.push_back(clientes[i]);
    }

    cout << "\n--- LISTA DE COMPRAS ---\n";
    exibirClientes(codigosParaTestar, mapaClientes, produtos, listaDeCompras);

    if (entrega == 1) {
        return 0;
    }

    cout << "\n--- SIMILARIDADE ---\n";
    int n = clientes.size();
    int m = produtos.size();
    int** a = matrizCompras(listaDeCompras, n, m);

    cout << "\n- BENCHMARK (n=" << n << ", m=" << m << ")\n";

    clock_t inicioNaive = clock();
    int** resultadoNaive = intersecaoNaive(a, n, m);
    clock_t fimNaive = clock();
    double tempoNaive = double(fimNaive - inicioNaive) / CLOCKS_PER_SEC;
    cout << "Tempo (Naive):        " << tempoNaive << " segundos\n";
    liberarMatrizInt(resultadoNaive, n);

    clock_t inicioEficiente = clock();
    int** Idenso = intersecaoEficiente(a, n, m);
    clock_t fimEficiente = clock();
    double tempoEficiente = double(fimEficiente - inicioEficiente) / CLOCKS_PER_SEC;
    cout << "Tempo (Eficiente):     " << tempoEficiente << " segundos\n";
    liberarMatrizInt(Idenso, n);

    clock_t inicioConstrucaoCSR = clock();
    CSR csr = construirCSR(listaDeCompras, n, m);
    clock_t fimConstrucaoCSR = clock();

    clock_t inicioCSR = clock();
    CSR Icsr = intersecaoCSR(csr, n);
    clock_t fimCSR = clock();
    double tempoCSR = double(fimCSR - inicioCSR) / CLOCKS_PER_SEC;
    double tempoConstrucaoCSR = double(fimConstrucaoCSR - inicioConstrucaoCSR) / CLOCKS_PER_SEC;
    cout << "Tempo (CSR - build):   " << tempoConstrucaoCSR << " segundos\n";
    cout << "Tempo (CSR - produto): " << tempoCSR << " segundos\n";

    if (tempoCSR > 0.0) {
        cout << "Speedup (Tempo Eficiente vs Tempo CSR): " << (tempoEficiente / tempoCSR) << "x\n";
    }

    double memoriaDensaBytes = (double) n * n * sizeof(int);
    double memoriaCSRBytes = (double) Icsr.nnz * sizeof(double)
                            + (double) Icsr.nnz * sizeof(int)
                            + (double) (n + 1) * sizeof(int);

    cout << "\nMemoria estimada para a matriz de intersecao I:\n";
    cout << "  Densa: " << (memoriaDensaBytes / (1024.0 * 1024.0)) << " MB\n";
    cout << "  CSR:   " << (memoriaCSRBytes / (1024.0 * 1024.0)) << " MB "
         << "(" << Icsr.nnz << " elementos nao-nulos de " << ((long long) n * n) << " possiveis)\n";

    if (memoriaCSRBytes > 0.0) {
        cout << "  Economia de memoria: " << (memoriaDensaBytes / memoriaCSRBytes) << "x menos memoria\n";
    }
    cout << "\n";

    double** s = calcularMatrizesCSR(Icsr, listaDeCompras, n);

    int clienteTeste1 = 0;
    int clienteTeste2 = 1;

    if (n >= 2) {
        int similar1 = clienteSimilares(s, clienteTeste1, n);
        cout << "Para o cliente interno [" << clienteTeste1 << "] (Codigo Original: " << clientes[clienteTeste1] << "):\n";
        if (similar1 != -1) {
            cout << "  -> O cliente mais similar a ele e o [" << similar1
                 << "] (Codigo Original: " << clientes[similar1]
                 << ") com distancia de Jaccard: " << s[clienteTeste1][similar1] << "\n\n";
        }

        int similar2 = clienteSimilares(s, clienteTeste2, n);
        cout << "Para o cliente interno [" << clienteTeste2 << "] (Codigo Original: " << clientes[clienteTeste2] << "):\n";
        if (similar2 != -1) {
            cout << "  -> O cliente mais similar a ele e o [" << similar2
                 << "] (Codigo Original: " << clientes[similar2]
                 << ") com distancia de Jaccard: " << s[clienteTeste2][similar2] << "\n\n";
        }
    } else {
        cout << "Nao ha clientes suficientes para calcular similaridade.\n";
    }

    if (entrega == 2) {
        liberarMatrizInt(a, n);
        liberarMatrizDouble(s, n);
        liberarCSR(csr);  
        liberarCSR(Icsr);
        return 0;
    }

    cout << "\n--- RECOMENDACOES (k=" << k << ") ---\n";

    for (int i = 0; i < totalParaTestar; i++) {
        string codOriginal = codigosParaTestar[i];
        int idInterno = mapaClientes[codOriginal];

        vector<Recomendacao> recomendacoes;
        gerarRecomendacoes(idInterno, k, s, a, n, m, recomendacoes);

        cout << "Recomendacoes para o cliente [" << idInterno
             << "] (Codigo Original: " << codOriginal << "):\n";

        int totalRecomendacoes = recomendacoes.size();
        if (totalRecomendacoes == 0) {
            cout << "  (nenhuma recomendacao disponivel para este cliente)\n";
        } else {
            for (int j = 0; j < totalRecomendacoes; j++) {
                int idProduto = recomendacoes[j].idProduto;
                cout << "  - " << produtos[idProduto]
                     << " (score: " << recomendacoes[j].score << ")\n";
            }
        }
        cout << "\n";
    }

    liberarMatrizInt(a, n);
    liberarMatrizDouble(s, n);
    liberarCSR(csr);  
    liberarCSR(Icsr);

    return 0;
}