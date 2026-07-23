#include "listaCompras.h"
#include <stdio.h>

void gerarLista(char caminho[], vector<string>& clientes, map<string, int>& mapaClientes,
    vector<string>& produtos, map<int, int>& mapaProdutos, vector<vector<int>>& listaDeCompras) {

    Produto p;
    FILE* arquivo;
    char linhaCabecalho[512];

    arquivo = fopen(caminho, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    fgets(linhaCabecalho, sizeof(linhaCabecalho), arquivo);

    while (fscanf(arquivo, "%d,%8[^,],%d,%49[^\n]\n",
                &p.data, p.codeCliente, &p.codeProduto, p.nomeProduto) == 4) {

        string codCliente(p.codeCliente);

        int indiceCliente;
        if (mapaClientes.find(codCliente) == mapaClientes.end()) {
            indiceCliente = clientes.size();
            clientes.push_back(codCliente);
            mapaClientes[codCliente] = indiceCliente;

            vector<int> listaVazia;
            listaDeCompras.push_back(listaVazia);
        } else {
            indiceCliente = mapaClientes[codCliente];
        }

        int indiceProduto;
        if (mapaProdutos.find(p.codeProduto) == mapaProdutos.end()) {
            indiceProduto = produtos.size();
            produtos.push_back(string(p.nomeProduto));
            mapaProdutos[p.codeProduto] = indiceProduto;
        } else {
            indiceProduto = mapaProdutos[p.codeProduto];
        }

        int jaComprou = 0;
        int totalAtual = listaDeCompras[indiceCliente].size();
        for (int idx = 0; idx < totalAtual; idx++) {
            if (listaDeCompras[indiceCliente][idx] == indiceProduto) {
                jaComprou = 1;
                break;
            }
        }

        if (jaComprou == 0) {
            listaDeCompras[indiceCliente].push_back(indiceProduto);
        }
    }

    fclose(arquivo);
}

void exibirClientes(vector<string>& codigosClientes, map<string, int>& mapaClientes,
    vector<string>& produtos, vector<vector<int>>& listaDeCompras) {

    int totalCodigos = codigosClientes.size();

    for (int i = 0; i < totalCodigos; i++) {
        string codigoBusca = codigosClientes[i];

        if (mapaClientes.find(codigoBusca) != mapaClientes.end()) {
            int indiceInterno = mapaClientes[codigoBusca];

            cout << "\nCliente " << codigoBusca << ":\n";

            vector<int> comprasCliente(listaDeCompras[indiceInterno].begin(),
                                      listaDeCompras[indiceInterno].end());

            if (comprasCliente.empty()) {
                cout << "   (Nenhuma Compra Registrada)\n";
            } else {
                for (auto it = comprasCliente.begin(); it != comprasCliente.end(); ++it) {
                    int idxProduto = *it;
                    cout << "  - " << produtos[idxProduto] << "\n";
                }
            }
        } else {
            cout << "\nCliente " << codigoBusca << " nao encontrado na base de dados.\n";
        }
    }
}