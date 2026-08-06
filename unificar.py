import pandas as pd

unificados = [
    "data/dados_venda_cluster_3.csv",
    "data/dados_venda_cluster_5.csv",
]

lista_df = []

for arquivos in unificados:
    df = pd.read_csv(arquivos, sep=",")
    lista_df.append(df)

df_unificado = pd.concat(lista_df, ignore_index=True)
df_unificado.to_csv("dados_venda_cluster_unificado3.csv", sep=",", index=False)

print("Os datasets foram unificados!") 