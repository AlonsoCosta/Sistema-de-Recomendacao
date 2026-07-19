#include "listaCompras.h"
#include "similaridade.h"
#include "recomendacao.h"
#include <stdio.h>

int main() {
    vector<string> clientes;
    map<string, int> mapaClientes;
    vector<string> produtos;
    map<int, int> mapaProdutos;
    vector<vector<int>> listaDeCompras;

    int quantidade;
    printf("Quantos arquivos de compras deseja carregar? ");
    scanf("%d", &quantidade);

    char caminho[512];
    for (int i = 0; i < quantidade; i++) {
        printf("Caminho do arquivo %d de %d: ", i + 1, quantidade);
        scanf("%s", caminho);

        gerarLista(caminho, clientes, mapaClientes, produtos, mapaProdutos, listaDeCompras);

        printf("  -> Carregado. Total de clientes ate agora: %d | Total de produtos ate agora: %d\n",
               (int) clientes.size(), (int) produtos.size());
    }

    //Atividade 1
    vector<string> codigosParaTestar;
    int totalParaTestar = (clientes.size() < 3) ? (int) clientes.size() : 3;
    for (int i = 0; i < totalParaTestar; i++) {
        codigosParaTestar.push_back(clientes[i]);
    }

    exibirClientes(codigosParaTestar, mapaClientes, produtos, listaDeCompras);

   //Atividade 2
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

    //Atividade 3
    int k = 5; 

    if (n >= 1) {
        vector<Recomendacao> recomendacoes1;
        gerarRecomendacoes(clienteTeste1, k, s, a, n, m, recomendacoes1);

        cout << "\nRecomendacoes para o cliente [" << clienteTeste1
             << "] (Codigo Original: " << clientes[clienteTeste1] << "):\n";

        int totalRecomendacoes1 = recomendacoes1.size();
        if (totalRecomendacoes1 == 0) {
            cout << "  (nenhuma recomendacao disponivel para este cliente)\n";
        } else {
            for (int i = 0; i < totalRecomendacoes1; i++) {
                int idProduto = recomendacoes1[i].idProduto;
                cout << "  - " << produtos[idProduto]
                     << " (score: " << recomendacoes1[i].score << ")\n";
            }
        }
    }

    if (n >= 2) {
        vector<Recomendacao> recomendacoes2;
        gerarRecomendacoes(clienteTeste2, k, s, a, n, m, recomendacoes2);

        cout << "\nRecomendacoes para o cliente [" << clienteTeste2
             << "] (Codigo Original: " << clientes[clienteTeste2] << "):\n";

        int totalRecomendacoes2 = recomendacoes2.size();
        if (totalRecomendacoes2 == 0) {
            cout << "  (nenhuma recomendacao disponivel para este cliente)\n";
        } else {
            for (int i = 0; i < totalRecomendacoes2; i++) {
                int idProduto = recomendacoes2[i].idProduto;
                cout << "  - " << produtos[idProduto]
                     << " (score: " << recomendacoes2[i].score << ")\n";
            }
        }
    }

    return 0;
}