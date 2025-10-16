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

long inserir_produto_direto(PRODUCT produto)
{
    FILE *produtos = abrir(PATH_DADOS_PROD, "r+b");
    PRODUCT prod;
    if(!produtos) return -1;
    fseek(produtos,-sizeof(PRODUCT),SEEK_END);
    while(fread(&prod, sizeof(PRODUCT), 1, produtos))
        if (prod.product_id<produto.product_id) break;
        else
        {
            fwrite(&prod, 1, sizeof(PRODUCT),produtos);
            fseek(produtos, 3 * (-sizeof(PRODUCT)),SEEK_END);
            if(0 == ftell(produtos)) break;
        }
    fwrite(&produto, 1, sizeof(PRODUCT), produtos);
    fseek(produtos,-sizeof(PRODUCT),SEEK_END);
    long end = ftell(produtos);
    fclose(produtos);
    return end;
}

long inserir_pedido_direto(ORDER pedido)
{
    FILE *pedidos = abrir(PATH_DADOS_ORDER, "r+b");
    ORDER ped;
    if(!pedidos) return -1;
    fseek(pedidos,-sizeof(ORDER),SEEK_END);
    while(fread(&ped, sizeof(ORDER), 1, pedidos))
        if (ped.order_id<pedido.order_id) break;
        else
        {
            fwrite(&ped, 1, sizeof(ORDER),pedidos);
            fseek(pedidos, 3 * (-sizeof(ORDER)),SEEK_END);
            if(0 == ftell(pedidos)) break;
        }
    fwrite(&pedido, 1, sizeof(ORDER), pedidos);
    fseek(pedidos,-sizeof(ORDER),SEEK_END);
    long end = ftell(pedidos);
    fclose(pedidos);
    return end;
}

void inserir_produto_pedido(long endereco_pedido, unsigned long long product_id, int sku_produto)
{
    FILE *pedidos = abrir(PATH_DADOS_ORDER, "r+b");
    if(!pedidos) return -1;
    fseek(pedidos,endereco_pedido,SEEK_SET);
    ORDER pedido;
    fread(&pedido, sizeof(ORDER), 1, pedidos);
    int indice = pedido.products_amount;
    if(indice != AMOUNT_MAX)
    {
        pedido.products_id[pedido.products_amount]=product_id;
        pedido.SKU_in_order[(pedido.products_amount)++]=sku_produto;
        fseek(pedidos,endereco_pedido,SEEK_SET);
        fwrite(&pedido, sizeof(ORDER), 1, pedidos);
    }
    fclose(pedidos);
}

void criar_arquivos_dados()
{
    FILE *dados = abrir(PATH_DADOS_ORIGEM,"r");
    FILE *produtos = abrir(PATH_DADOS_PROD,"wb");
    FILE *pedidos = abrir(PATH_DADOS_ORDER,"wb");
    
    if(!dados || !produtos || !pedidos) return;

    fclose(produtos);
    fclose(pedidos);
    char linha[122 + (30 * AMOUNT_MAX) + SIZE_CATEGORY + (3 * SIZE_MAIN_CMG) + 10 + 30];
    while(fgets(linha,1,dados))
    {
        linha[strcspn(linha, "\n")] = '\0';
        ORDER pedido;
        PRODUCT produto;
        int sku_produto;
        desconstruir_linha(linha,&sku_produto,&pedido,&produto);
        long end_prod = busca_produto(0, pedido.order_id), end_ped = busca_pedido(0, produto.product_id);
        if(end_prod == -1)
            end_prod = inserir_produto_direto(produto);
        if(end_ped == -1)
            end_ped = inserir_pedido_direto(pedido);
        inserir_produto_pedido(end_ped, produto.product_id, sku_produto);
    }
    fclose(dados);
}

void criar_arquivos_indice()
{
    FILE *ind_prod = abrir(PATH_INDEX_PROD,"wb");
    FILE *ind_ord = abrir(PATH_INDEX_ORDER,"wb");
    FILE *produtos = abrir(PATH_DADOS_PROD,"rb");
    FILE *pedidos = abrir(PATH_DADOS_ORDER,"rb");

    if(!ind_prod || !ind_ord || !produtos || !pedidos) return;

    INDEX indice;
    PRODUCT produto;
    int i=0;
    while(fread(&produto, sizeof(PRODUCT), 1, produtos))
    {
        if(i==0)
            indice.endereco = ftell(produtos);
        else if (i==ARQ_INDEX_AMOUNT_PROD - 1)
        {
            indice.chave = produto.product_id;
            i=0;
            fwrite(&indice,sizeof(INDEX),1,ind_prod);
        }
        else 
            i++;
    }
    if(i != 0)
    {
        indice.chave = produto.product_id;
        fwrite(&indice,sizeof(INDEX),1,ind_prod);
    }

    ORDER order;
    int i=0;
    while(fread(&order, sizeof(PRODUCT), 1,pedidos))
    {
        if(i==0)
            indice.endereco = ftell(pedidos);
        else if (i==ARQ_INDEX_AMOUNT_ORDER - 1)
        {
            indice.chave = order.order_id;
            i=0;
            fwrite(&indice,sizeof(INDEX),1,ind_ord);
        }
        else 
            i++;
    }
    if(i != 0)
    {
        indice.chave = order.order_id;
        i=0;
        fwrite(&indice,sizeof(INDEX),1,ind_ord);
    }

    fclose(ind_ord);
    fclose(ind_prod);
    fclose(produtos);
    fclose(pedidos);
}

void criar_arquivos_base()
{
    //add teste se ja existe os arquivos
    criar_arquivos_dados();
    criar_arquivos_indice();
}

//add e remover produto pedido produto-pedido
