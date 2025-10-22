#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.c"

INDEX indice_produto_binaria(unsigned long long id)
{
    FILE *produtos = abrir(PATH_INDEX_PROD,"rb");
    INDEX index_nulo = {0 , -1};

    if(!produtos)
        return index_nulo;

    INDEX ind;
    unsigned int tamanho = sizeof(INDEX);

    fseek(produtos, 0, SEEK_END);
    long end_fim = ftell(produtos);
    
    if (end_fim == 0) 
    {
        fclose(produtos);
        return index_nulo;
    }

    long num_registros = end_fim / tamanho;

    long indice_inicio = 0;
    long indice_fim = num_registros - 1;
    long indice_meio;
    long end_meio;
    INDEX melhor_ind; 
    fseek(produtos, -sizeof(INDEX), SEEK_END);
    fread(&melhor_ind, sizeof(INDEX), 1, produtos);

    while (indice_inicio <= indice_fim)
    {
        indice_meio = indice_inicio + (indice_fim - indice_inicio) / 2;
        end_meio = indice_meio * tamanho;

        if (fseek(produtos, end_meio, SEEK_SET) != 0 || fread(&ind, tamanho, 1, produtos) != 1) 
        {
            fclose(produtos);
            return index_nulo;
        }

        if (ind.chave >= id)
        {
            melhor_ind = ind;
            indice_fim = indice_meio - 1;
        }
        else
            indice_inicio = indice_meio + 1;
    }
    
    fclose(produtos);

    return melhor_ind;
}

INDEX indice_pedido_binaria(unsigned long long id)
{
    FILE *pedidos = abrir(PATH_INDEX_ORDER,"rb");
    INDEX index_nulo = {0 , -1};

    if(!pedidos)
        return index_nulo;

    INDEX ind;
    unsigned int tamanho = sizeof(INDEX);

    fseek(pedidos, 0, SEEK_END);
    long end_fim = ftell(pedidos);

    if (end_fim == 0) {
        fclose(pedidos);
        return index_nulo;
    }

    long num_registros = end_fim / tamanho;

    long indice_inicio = 0;
    long indice_fim = num_registros - 1;
    long indice_meio;
    long end_meio;
    INDEX melhor_ind = {0, 0};

    while (indice_inicio <= indice_fim)
    {
        indice_meio = indice_inicio + (indice_fim - indice_inicio) / 2;
        end_meio = indice_meio * tamanho;

        if (fseek(pedidos, end_meio, SEEK_SET) != 0 || fread(&ind, tamanho, 1, pedidos) != 1) 
        {
            fclose(pedidos);
            return index_nulo;
        }

        if (ind.chave >= id)
        {
            melhor_ind = ind;
            indice_fim = indice_meio - 1;
        }
        else
            indice_inicio = indice_meio + 1;
    }
    
    fclose(pedidos);

    if(melhor_ind.chave == 0)
        return index_nulo;
    
    return melhor_ind;
}

PRODUCT busca_produto_binaria(long end_inicio, unsigned long long id)
{
    FILE *produtos = abrir(PATH_DADOS_PROD,"rb");
    PRODUCT produto_vazio = {0}; 
    if(!produtos || end_inicio == -1)
    {
        fclose(produtos);
        return produto_vazio;
    }

    PRODUCT produto;
    unsigned int tamanho = sizeof(PRODUCT);

    fseek(produtos, end_inicio + tamanho * ARQ_INDEX_AMOUNT_PROD, SEEK_SET);
    long end_fim;
    if (fread(&produto, tamanho, 1, produtos) == 1) 
        end_fim = ftell(produtos); 
    else {
        fseek(produtos, 0, SEEK_END);
        end_fim = ftell(produtos);
    }

    if (end_fim <= end_inicio) {
        fclose(produtos);
        return produto_vazio;
    }

    long inicio = end_inicio;
    long fim = end_fim;

    long num_registros = (fim - inicio) / tamanho;

    long indice_inicio = 0;
    long indice_fim = num_registros - 1; 
    long indice_meio;
    long end_meio;

    while (indice_inicio <= indice_fim)
    {
        indice_meio = indice_inicio + (indice_fim - indice_inicio) / 2; 
        
        end_meio = inicio + indice_meio * tamanho;
        
        if (fseek(produtos, end_meio, SEEK_SET) != 0) {
            fclose(produtos);
            return produto_vazio; 
        }

        if (fread(&produto, tamanho, 1, produtos) != 1) {
            fclose(produtos);
            return produto_vazio; 
        }

        if (produto.product_id == id)
        {
            fclose(produtos);
            if(produto.exclusao)
                return produto_vazio;
            return produto;
        }
        else if (id < produto.product_id)
            indice_fim = indice_meio - 1;
        else
            indice_inicio = indice_meio + 1;
    }

    fclose(produtos);
    return produto_vazio; 
}

ORDER busca_pedido_binaria(long end_inicio, unsigned long long id)
{
    FILE *pedidos = abrir(PATH_DADOS_ORDER,"rb");
    ORDER pedido_vazio = {0};
    if(!pedidos || end_inicio == -1)
    {
        fclose(pedidos);
        return pedido_vazio;
    }

    ORDER pedido;
    unsigned int tamanho = sizeof(ORDER);

    fseek(pedidos, 0, SEEK_END);
    long end_fim = ftell(pedidos);

    if (end_fim <= end_inicio)
    {
        fclose(pedidos);
        return pedido_vazio;
    }

    long num_registros = (end_fim - end_inicio) / tamanho;

    long indice_inicio = 0;
    long indice_fim = num_registros - 1;
    long indice_meio;
    long end_meio;

    while (indice_inicio <= indice_fim)
    {
        indice_meio = indice_inicio + (indice_fim - indice_inicio) / 2;

        end_meio = end_inicio + indice_meio * tamanho;

        if (fseek(pedidos, end_meio, SEEK_SET) != 0)
        {
            fclose(pedidos);
            return pedido_vazio;
        }

        if (fread(&pedido, tamanho, 1, pedidos) != 1)
        {
            fclose(pedidos);
            return pedido_vazio;
        }

        if (pedido.order_id == id)
        {
            fclose(pedidos);
            if(pedido.exclusao)
                return pedido_vazio;
            return pedido;
        }
        else if (id < pedido.order_id)
        {
            indice_fim = indice_meio - 1;
        }
        else
        {
            indice_inicio = indice_meio + 1;
        }
    }

    fclose(pedidos);
    return pedido_vazio;
}
