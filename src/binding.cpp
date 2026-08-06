#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <ctime>

#include "similaridade.h"
#include "recomendacao.h"

using namespace std;
namespace py = pybind11;

double** converterParaMatrizBruta(vector<vector<double>>& matrizSimilaridade, int totalClientes) {
    double** matrizBruta = (double**) malloc(totalClientes * sizeof(double*));
    for (int i = 0; i < totalClientes; i++) {
        matrizBruta[i] = (double*) malloc(totalClientes * sizeof(double));
        for (int j = 0; j < totalClientes; j++) {
            matrizBruta[i][j] = matrizSimilaridade[i][j];
        }
    }
    return matrizBruta;
}

py::tuple benchmark_bind(vector<vector<int>>& listaDeCompras, int n, int m) {
    int** A = matrizCompras(listaDeCompras, n, m);

    clock_t t0 = clock();
    int** naive = intersecaoNaive(A, n, m);
    double tNaive = double(clock() - t0) / CLOCKS_PER_SEC;
    liberarMatrizInt(naive, n);

    t0 = clock();
    int** eficiente = intersecaoEficiente(A, n, m);
    double tEficiente = double(clock() - t0) / CLOCKS_PER_SEC;
    liberarMatrizInt(eficiente, n);

    liberarMatrizInt(A, n);
    return py::make_tuple(tNaive, tEficiente);
}

vector<vector<double>> similaridade_bind(vector<vector<int>>& listaDeCompras, int n, int m) {
    int** A = matrizCompras(listaDeCompras, n, m);
    int** I = intersecaoEficiente(A, n, m);
    double** S = calcularMatrizes(I, listaDeCompras, n);

    vector<vector<double>> resultado(n, vector<double>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            resultado[i][j] = S[i][j];

    liberarMatrizInt(A, n);
    liberarMatrizInt(I, n);
    liberarMatrizDouble(S, n);
    return resultado;
}

int clienteSimilar_bind(vector<vector<double>>& S, int cliente, int n) {
    double** matrizBruta = converterParaMatrizBruta(S, n);
    int resultado = clienteSimilares(matrizBruta, cliente, n);
    liberarMatrizDouble(matrizBruta, n);
    return resultado;
}

vector<Recomendacao> recomendar_bind(
    int c, int k, vector<vector<double>>& S,
    vector<vector<int>>& listaDeCompras, int n, int m) {

    int** A = matrizCompras(listaDeCompras, n, m);
    double** matrizSimilaridadeBruta = converterParaMatrizBruta(S, n);

    vector<Recomendacao> resultado;
    gerarRecomendacoes(c, k, matrizSimilaridadeBruta, A, n, m, resultado);

    liberarMatrizInt(A, n);
    liberarMatrizDouble(matrizSimilaridadeBruta, n);
    return resultado;
}

PYBIND11_MODULE(recomendacao, m) {
    m.doc() = "Sistema de Recomendações";

    py::class_<Recomendacao>(m, "Recomendacao")
        .def(py::init<>())
        .def_readwrite("idProduto", &Recomendacao::idProduto)
        .def_readwrite("score", &Recomendacao::score);

    m.def("benchmark", &benchmark_bind, "naive vs eficiente, em segundos");
    m.def("similaridade", &similaridade_bind, "monta a matriz S densa");
    m.def("clientes_similares", &clienteSimilar_bind, "cliente mais parecido com c");
    m.def("recomendacoes", &recomendar_bind, "top k produtos para dado cliente");
}