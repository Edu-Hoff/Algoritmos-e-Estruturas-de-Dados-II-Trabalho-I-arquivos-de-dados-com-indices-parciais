#include <stdio.h>
#include <stdlib.h>
#include "busca.c"

int cmp_product(const void *a, const void *b)
{
    PRODUCT *pa = (PRODUCT*)a;
    PRODUCT *pb = (PRODUCT*)b;
    if (pa->product_id < pb->product_id) return -1;
    if (pa->product_id > pb->product_id) return 1;
    return 0;
}

int cmp_order(const void *a, const void *b)
{
    ORDER *pa = (ORDER*)a;
    ORDER *pb = (ORDER*)b;
    if (pa->order_id < pb->order_id) return -1;
    if (pa->order_id > pb->order_id) return 1;
    return 0;
}

void criar_arquivos_dados()
{
    PRODUCT *produtos_mem =(PRODUCT *) malloc(MAX_RECORDS * sizeof(PRODUCT));
    ORDER   *pedidos_mem  =(ORDER *) malloc(MAX_RECORDS * sizeof(ORDER));
    int n_prod = 0, n_ped = 0;
    FILE *dados = abrir(PATH_DADOS_ORIGEM,"r");
    char linha[122 + (30 * AMOUNT_MAX) + SIZE_CATEGORY + (3 * SIZE_MAIN_CMG) + 10 + 30];  
    while(fgets(linha, sizeof(linha), dados))
    {
        if(n_ped % 5000 == 0)
        {
            limpar_tela();
            printf("Pedidos: %d - Produtos: %d",n_ped,n_prod);
        }
        linha[strcspn(linha, "\n")] = '\0';
        int sku;
        PRODUCT prod;
        ORDER ped;
        desconstruir_linha(linha, &sku, &ped, &prod);
        if(!existe_produto(produtos_mem,prod.product_id,n_prod))
            produtos_mem[n_prod++] = prod;
        int indice = existe(pedidos_mem, ped.order_id, n_ped);
        if(indice == -1)
        {
            ped.products_id[ped.products_amount]=prod.product_id;
            ped.SKU_in_order[ped.products_amount++]=sku;
            pedidos_mem[n_ped++]   = ped;
        }
        else
        {
            pedidos_mem[indice].products_id[pedidos_mem[indice].products_amount]=prod.product_id;
            pedidos_mem[indice].SKU_in_order[pedidos_mem[indice].products_amount++]=sku;
        }
    }
    qsort(produtos_mem, n_prod, sizeof(PRODUCT), cmp_product);
    qsort(pedidos_mem, n_ped, sizeof(ORDER), cmp_order);

    FILE *fprod = fopen(PATH_DADOS_PROD, "wb");
    fwrite(produtos_mem, sizeof(PRODUCT), n_prod, fprod);
    fclose(fprod);

    FILE *fped = fopen(PATH_DADOS_ORDER, "wb");
    fwrite(pedidos_mem, sizeof(ORDER), n_ped, fped);
    fclose(fped);

    free(produtos_mem); 
    free(pedidos_mem);

}

void criar_arquivos_indice()
{
    FILE *ind_prod = abrir(PATH_INDEX_PROD,"wb");
    FILE *ind_ord = abrir(PATH_INDEX_ORDER,"wb");
    FILE *produtos = abrir(PATH_DADOS_PROD,"rb");
    FILE *pedidos = abrir(PATH_DADOS_ORDER,"rb");

    if(!ind_prod || !ind_ord || !produtos || !pedidos) {
        if(ind_prod) fclose(ind_prod);
        if(ind_ord) fclose(ind_ord);
        if(produtos) fclose(produtos);
        if(pedidos) fclose(pedidos);
        return;
    }

    INDEX indice;
    PRODUCT produto;
    int i=0; 
    
    long end_prod_inicio = 0; 

    while(fread(&produto, sizeof(PRODUCT), 1, produtos))
    {
        if(i==0)
            indice.endereco = end_prod_inicio; 

        if (i == ARQ_INDEX_AMOUNT_PROD - 1)
        {
            indice.chave = produto.product_id;
            fwrite(&indice, sizeof(INDEX), 1, ind_prod);
            i=0;
        }
        else 
            i++;
        
        end_prod_inicio += sizeof(PRODUCT);
    }
    
    if(i != 0)
    {
        indice.chave = produto.product_id;
        fwrite(&indice, sizeof(INDEX), 1, ind_prod);
    }

    ORDER order;
    i=0; 
    long end_order_inicio = 0; 

    while(fread(&order, sizeof(ORDER), 1, pedidos))
    {
        if(i==0)
            indice.endereco = end_order_inicio; 

        if (i == ARQ_INDEX_AMOUNT_ORDER - 1)
        {
            indice.chave = order.order_id;
            fwrite(&indice, sizeof(INDEX), 1, ind_ord);
            i=0; 
        }
        else 
            i++;
        end_order_inicio = ftell(produtos);
    }
    if(i != 0)
    {
        indice.chave = order.order_id;
        fwrite(&indice, sizeof(INDEX), 1, ind_ord);
    }

    fclose(ind_ord);
    fclose(ind_prod);
    fclose(produtos);
    fclose(pedidos);
}

void reorganizar()
{
    PRODUCT *produtos_mem =(PRODUCT *) malloc(MAX_RECORDS * sizeof(PRODUCT));
    ORDER   *pedidos_mem  =(ORDER *) malloc(MAX_RECORDS * sizeof(ORDER));
    int n_prod = 0, n_ped = 0;

    FILE *produtos = abrir(PATH_DADOS_PROD,"rb");
    while(fread(&produtos_mem[n_prod], sizeof(PRODUCT), 1, produtos))
        if (n_prod >= MAX_RECORDS) break;
        else n_prod++;
    fclose(produtos);

    FILE *pedidos = abrir(PATH_DADOS_ORDER,"rb");
    while(fread(&pedidos_mem[n_ped], sizeof(ORDER), 1, pedidos))
        if (n_ped >= MAX_RECORDS) break;
        else n_ped++;
    fclose(pedidos);

    qsort(produtos_mem, n_prod, sizeof(PRODUCT), cmp_product);
    qsort(pedidos_mem, n_ped, sizeof(ORDER), cmp_order);

    FILE *produtos = fopen(PATH_DADOS_PROD, "wb");
    fwrite(produtos_mem, sizeof(PRODUCT), n_prod, produtos);
    fclose(produtos);

    FILE *pedidos = fopen(PATH_DADOS_ORDER, "wb");
    fwrite(pedidos_mem, sizeof(ORDER), n_ped, pedidos);
    fclose(pedidos);
    free(produtos_mem);
    free(pedidos_mem);

    criar_arquivos_indice();
}

void criar_arquivos_base()
{
    criar_arquivos_dados();
    criar_arquivos_indice();
}

//add e remover produto pedido produto-pedido
