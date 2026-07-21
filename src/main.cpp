#include "listaCompras.h"
#include "similaridade.h"
#include "recomendacao.h"
#include <stdio.h>
#include <iostream>
#include <cstdlib>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Uso: " << argv[0] << " <instancia.csv> <entrega:1|2|3> [k]\n";
        cout << "  entrega=1 -> so roda a Atividade 1 (leitura)\n";
        cout << "  entrega=2 -> roda Atividades 1 e 2 (leitura + similaridade)\n";
        cout << "  entrega=3 -> roda Atividades 1, 2 e 3 (precisa de k)\n";
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

    // ==========================================
    // Atividade 1 -- roda sempre que entrega >= 1
    // ==========================================
    cout << "\n--- ATIVIDADE 1 ---\n";
    exibirClientes(codigosParaTestar, mapaClientes, produtos, listaDeCompras);

    if (entrega == 1) {
        return 0; // para por aqui, como pedido
    }

    // ==========================================
    // Atividade 2 -- roda se entrega >= 2
    // ==========================================
    cout << "\n--- ATIVIDADE 2 ---\n";
    int n = clientes.size();
    int m = produtos.size();

    vector<vector<int>> a;
    vector<vector<int>> at;
    vector<vector<int>> I;
    vector<vector<double>> s;

    matrizCompras(listaDeCompras, n, m, a);
    matrizTransposta(a, at, n, m);
    multiplicarMatrizes(a, at, n, m, I);
    calcularMatrizes(I, listaDeCompras, n, s);

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
        return 0; // para por aqui
    }

    // ==========================================
    // Atividade 3 -- so roda se entrega == 3 (ja validamos que k existe)
    // ==========================================
    cout << "\n--- ATIVIDADE 3 (k=" << k << ") ---\n";

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

    return 0;
}