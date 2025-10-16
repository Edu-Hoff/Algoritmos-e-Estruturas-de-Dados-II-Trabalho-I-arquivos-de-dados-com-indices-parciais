#include <stdio.h>
#include <stdlib.h>
#include "utils.c"

long busca_produto(long endereco_inicio, unsigned long long id)
{
    FILE *produtos = abrir(PATH_DADOS_PROD,"rb");
    if(!produtos) return -1;
}

long busca_pedido(long endereco_inicio, unsigned long long id)
{
    //retorna endereco
}
