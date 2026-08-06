import sys
from leitura_e_execucao import gerarListaCompras
import recomendacao


def exibirListaDeComprasCliente(codigo, mapaClientes, produtos, listaDeCompras):
    idInterno = mapaClientes[codigo]
    print(f"\nCliente {codigo}:")
    compras = listaDeCompras[idInterno]
    if not compras:
        print("   (Nenhuma Compra Registrada)")
    else:
        for idProduto in compras:
            print(f"  - {produtos[idProduto]}")


def main():
    if len(sys.argv) != 3:
        print("python main.py <caminho_csv> <k>")   
        return

    caminhoCsv = sys.argv[1]
    k = int(sys.argv[2])

    print(f"Carregando o arquivo: {caminhoCsv}...")
    clientes, mapaClientes, produtos, mapaProdutos, listaDeCompras = gerarListaCompras(caminhoCsv)
    n, m = len(clientes), len(produtos)
    print(f"  -> Total de clientes: {n} | Total de produtos: {m}\n")
    totalParaTestar = min(3, n)
    codigosParaTestar = clientes[:totalParaTestar]

    print("--- LISTA DE COMPRAS ---")
    for codigo in codigosParaTestar:
        exibirListaDeComprasCliente(codigo, mapaClientes, produtos, listaDeCompras)

    print("\n--- SIMILARIDADE ---")
    print(f"\n-> BENCHMARK (n={n}, m={m})")
    tempoNaive, tempoEficiente = recomendacao.benchmark(listaDeCompras, n, m)
    print(f"Tempo (Naive):     {tempoNaive:.6f} segundos")
    print(f"Tempo (Eficiente): {tempoEficiente:.6f} segundos")
    if tempoEficiente > 0:
        print(f"Speedup (Naive vs Eficiente): {tempoNaive / tempoEficiente:.2f}x\n")
    S = recomendacao.similaridade(listaDeCompras, n, m)

    if n >= 2:
        for idx in (0, 1):
            similar = recomendacao.clientes_similares(S, idx, n)
            if similar != -1:
                print(f"Para o cliente interno [{idx}] (Código: {clientes[idx]}):")
                print(f"  -> O cliente mais similar é o [{similar}] "
                      f"(Código: {clientes[similar]}) com distância de Jaccard: {S[idx][similar]:.4f}\n")

    print(f"--- RECOMENDACOES (k={k}) ---")
    for codigo in codigosParaTestar:
        idInterno = mapaClientes[codigo]
        recomendacoes = recomendacao.recomendacoes(idInterno, k, S, listaDeCompras, n, m)

        print(f"Recomendações para o cliente [{idInterno}] (Código: {codigo}):")
        if not recomendacoes:
            print("  (nenhuma recomendação disponível para este cliente)")
        else:
            for rec in recomendacoes:
                print(f"  - {produtos[rec.idProduto]} (score: {rec.score:.4f})")
        print()


if __name__ == "__main__":
    main()