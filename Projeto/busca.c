#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.c"

//nao usei ainda

long busca_produto(long end_inicio, unsigned long long id, unsigned long long id_final)
{
    FILE *produtos = abrir(PATH_DADOS_PROD,"rb");
    if(!produtos) return -1;
    PRODUCT produto;
    unsigned int tamanho = sizeof(PRODUCT);
    fclose(produtos);
    return -1;
}

long busca_pedido(long end_inicio, unsigned long long id, unsigned long long id_final)
{
    FILE *pedidos = abrir(PATH_DADOS_ORDER,"rb");
    if(!pedidos) return -1;
    ORDER pedido;
    unsigned int tamanho = sizeof(ORDER);
    fclose(pedidos);
    return -1;
}

