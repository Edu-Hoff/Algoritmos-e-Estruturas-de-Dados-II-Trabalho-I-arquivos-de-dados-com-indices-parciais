# Algoritmos-e-Estruturas-de-Dados-II-Trabalho-I-arquivos-de-dados-com-indices-parciais

*Organização do arquivo de dados*
O arquivo de dados completo foi gerado a partir de um CSV corrigido, que originalmente apresentava pedidos com campos ausentes (Category ID, Brand ID e Category alias). Após a correção, os registros foram mantidos na mesma ordem do CSV original e separados em dois arquivos binários: um destinado aos pedidos e outro aos produtos.

O arquivo de pedidos armazena: Order ID, Order datetime, vetores de products_id e sku_in_order, products_amount, User ID, além de um campo de exclusão lógica e ponteiros (prox, ant) para navegação entre registros.
O arquivo de produtos contém: Product ID, Category ID, Category alias, Brand ID, Price, Product gender, Main color, Main metal, Main gem, também com exclusão lógica e ponteiros (prox, ant).

*Organização de arquivo de índice*
O arquivo binário de índices é estruturado em blocos que armazenam **dois atributos principais**: o **endereço do primeiro registro** (seja de pedido ou produto) e o **valor do ID do último registro** presente nesse bloco.
Os arquivos de índice — de produto e de pedido — podem ser recriados de forma independente ou conjunta, dependendo do tipo de reordenação aplicada. Caso apenas o arquivo de dados de produtos ou de pedidos seja reordenado, é suficiente reconstruir o índice correspondente. Já quando ambos são reorganizados simultaneamente, recomenda-se a recriação conjunta dos dois arquivos de índices, garantindo que os endereços e intervalos de IDs permaneçam coerentes com a nova disposição física dos registros.


**CSV**
    Order datetime
        Order ID
            Purchased product ID
                Quantity of SKU in the order
                    Category ID
                        Category alias
                            Brand ID
                                Price in USD
                                    User ID
                                        Product gender (m/f)
                                            Main color
                                            	Main metal
                                                    Main gem
    Data e hora do pedido  
        ID do pedido  
            ID do produto comprado  
                Quantidade do SKU no pedido  
                    ID da categoria  
                        Apelido da categoria  
                            ID da marca  
                                Preço em USD  
                                    ID do usuário  
                                        Gênero do produto (m/f)  
                                            Cor principal  
                                                Metal principal  
                                                    Gema principal
