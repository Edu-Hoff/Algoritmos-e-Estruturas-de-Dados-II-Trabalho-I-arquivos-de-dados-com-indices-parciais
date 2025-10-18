#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.c"

PRODUCT indice_produto_binaria(unsigned long long id)
{
    FILE *produtos = abrir(PATH_INDEX_PROD,"rb");
    if(!produtos)
    {
        PRODUCT produto = {0};
        return produto;
    }
    INDEX ind;
    unsigned int tamanho = sizeof(INDEX);

    while(fread(&ind, tamanho, 1, produtos) == 1)
        if(ind.chave <= id)
            break;

    fclose(produtos);
    return busca_produto_binaria(ind.endereco, id, ind.chave);
}

ORDER indice_pedido_binaria(unsigned long long id)
{
    FILE *pedidos = abrir(PATH_INDEX_ORDER,"rb");
    if(!pedidos) 
    {
        ORDER pedido = {0};
        return pedido;
    }
    INDEX ind;
    unsigned int tamanho = sizeof(INDEX);

    while(fread(&ind, tamanho, 1, pedidos) == 1)
        if(ind.chave <= id)
            break;

    fclose(pedidos);
    return busca_pedido_binaria(ind.endereco, id, ind.chave);
}



PRODUCT busca_produto_binaria(long end_inicio, unsigned long long id, unsigned long long id_final)
{
    FILE *produtos = abrir(PATH_DADOS_PROD,"rb");
    if(!produtos) return ;
    PRODUCT produto;
    unsigned int tamanho = sizeof(PRODUCT);

    //busca cuidar se for antes do primeiro registro

    fclose(produtos);
    return ;
}

ORDER busca_pedido_binaria(long end_inicio, unsigned long long id, unsigned long long id_final)
{
    FILE *pedidos = abrir(PATH_DADOS_ORDER,"rb");
    if(!pedidos) return ;
    ORDER pedido;
    unsigned int tamanho = sizeof(ORDER);

    //busca cuidar se for antes do primeiro registro

    fclose(pedidos);
    return ;
}

