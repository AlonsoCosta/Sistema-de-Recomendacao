import csv

def gerarListaCompras(caminho):
    clientes = []
    mapaClientes = {}
    produtos = []
    mapaProdutos = {}
    listaDeCompras = []

    with open(caminho, 'r', encoding='utf-8') as arquivo:
        linha = csv.reader(arquivo)
        next(linha)

        for i in linha:
            if len(i) != 4:
                continue

            codigoCliente = i[1]
            codigoProduto = i[2]
            nomeProduto = i[3]

            if codigoCliente not in mapaClientes:
                clientes.append(codigoCliente)
                mapaClientes[codigoCliente] = len(clientes) - 1
                listaDeCompras.append([])

            if codigoProduto not in mapaProdutos:
                produtos.append(nomeProduto)
                mapaProdutos[codigoProduto] = len(produtos) - 1

            id_cliente = mapaClientes[codigoCliente]
            id_produto = mapaProdutos[codigoProduto]

            if id_produto not in listaDeCompras[id_cliente]:
                listaDeCompras[id_cliente].append(id_produto)

    return clientes, mapaClientes, produtos, mapaProdutos, listaDeCompras