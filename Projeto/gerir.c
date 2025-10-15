#include <stdio.h>
#include <stdlib.h>
#include "busca.c"

FILE *abrir(const char *nome, const char *modo)
{
    FILE *temp = fopen(ARQ_DADOS_ORIGEM,"r");
    if(!temp) 
    {
        printf("Erro ao abrir o arquivo %s\n",ARQ_DADOS_ORIGEM);
        return NULL;
    } 
    return temp;
}

long inserir_produto(PRODUCT produto)
{
    //em ordem pelo id
    //+char pra exclucao e long pro prox (-1 de padrao) e long pro ant (-1 de padrao)
    //retorna endereco
    //sempre salvando o arquivo
}

long inserir_pedido(ORDER pedido)
{
    //em ordem pelo id
    //+char pra exclucao e long pro prox (-1 de padrao) e long pro ant (-1 de padrao)
    //retorna endereco
    //sempre salvando o arquivo
}

void inserir_produto_pedido(long endereco_pedido, unsigned long long product_id, int sku_produto)
{
    //insere o product_id no vetor de produtos e o sku tbm, e incrementa a quantidade
    //sempre salvando o arquivo
}

void criar_arquivos_dados()
{
    FILE *dados = abrir(PATH_DADOS_ORIGEM,"r");
    FILE *produtos = abrir(PATH_DADOS_PROD,"wb");
    FILE *pedidos = abrir(PATH_DADOS_ORDER,"wb");
    
    if(!dados || !produtos || !pedidos) return;

    fclose(produtos);
    fclose(pedidos);
    char linha[122 + (30 * AMOUNT_MAX) + SIZE_CATEGORY + (3 * SIZE_MAIN_CMG) + 30];
    while(fgets(linha,1,dados))
    {
        linha[strcspn(linha, "\n")] = '\0';
        ORDER pedido;
        PRODUCT produto;
        int sku_produto;
        desconstruir_linha(linha,&sku_produto,&pedido,&produto);
        long end_prod = busca_produto(pedido.order_id), end_ped = busca_pedido(produto.product_id);
        if(end_prod == -1)
            end_prod = inserir_produto(produto);
        if(end_ped == -1)
            end_ped = inserir_pedido(pedido);
        inserir_produto_pedido(end_ped, produto.product_id, sku_produto);
    }
    fclose(dados);
}

void criar_arquivos_indice()
{
    //criar arquivo de indice com base no maximo do define 
}

void criar_arquivos_base()
{
    criar_arquivos_dados();
    criar_arquivos_indice();
}

//add e remover produto pedido produto-pedido