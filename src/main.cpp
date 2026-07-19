#include "listaCompras.h"
#include "similaridade.h"
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

    vector<string> codigosParaTestar;
    int totalParaTestar = (clientes.size() < 3) ? (int) clientes.size() : 3;
    for (int i = 0; i < totalParaTestar; i++) {
        codigosParaTestar.push_back(clientes[i]);
    }

    exibirClientes(codigosParaTestar, mapaClientes, produtos, listaDeCompras);

  
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

    return 0;
}