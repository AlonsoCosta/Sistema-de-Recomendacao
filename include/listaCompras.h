#ifndef LISTACOMPRAS_H
#define LISTACOMPRAS_H

#include <vector>
#include <map>
#include <iostream>
using namespace std;

typedef struct {
    int data;
    char codeCliente[9];
    int codeProduto;
    char nomeProduto[50];
} Produto;

void gerarLista(char caminho[], vector<string>& clientes, map<string, int>& mapaClientes,
    vector<string>& produtos, map<int, int>& mapaProdutos, vector<vector<int>>& listaDeCompras);

void exibirClientes(vector<string>& codigosClientes, map<string, int>& mapaClientes,
    vector<string>& produtos, vector<vector<int>>& listaDeCompras);

#endif