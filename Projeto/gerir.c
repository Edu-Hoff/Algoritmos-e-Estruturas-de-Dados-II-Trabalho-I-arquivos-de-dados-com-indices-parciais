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
            limpar_tela("");
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

void criar_arquivos_indice(int opcao)
{
    INDEX indice;
    int i=0; 

    if(opcao == 0 || opcao == 2)
    {
        FILE *ind_prod = abrir(PATH_INDEX_PROD,"wb");
        FILE *produtos = abrir(PATH_DADOS_PROD,"rb");
        if(!ind_prod || !produtos) {
            if(ind_prod) fclose(ind_prod);
            if(produtos) fclose(produtos);
            return;
        }

        PRODUCT produto;
    
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

        fclose(ind_prod);
        fclose(produtos);
    }
    
    if(opcao == 1 || opcao == 2)
    {
        FILE *ind_ord = abrir(PATH_INDEX_ORDER,"wb");
        FILE *pedidos = abrir(PATH_DADOS_ORDER,"rb");

        if(!ind_ord || !pedidos) 
        {
            if(ind_ord) fclose(ind_ord);
            if(pedidos) fclose(pedidos);
            return;
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
            end_order_inicio = ftell(pedidos);
        }
        if(i != 0)
        {
            indice.chave = order.order_id;
            fwrite(&indice, sizeof(INDEX), 1, ind_ord);
        }

        fclose(ind_ord);
        fclose(pedidos);
    }
}

void reordenar(int opcao)
{
    PRODUCT produto;
    ORDER   pedido;

    if(opcao == 0 || opcao == 2)
    {
        FILE *produtos = abrir(PATH_DADOS_PROD,"rb");
        FILE *tmp_prod = abrir(DIR_DADOS "tmp_prod.bin","wb");

        while(fread(&produto, sizeof(PRODUCT), 1, produtos))
        {
            if (produto.ant != -1)
            {
                fseek(produtos, produto.ant, SEEK_SET);
                continue;
            }

            if(!produto.exclusao)
                fwrite(&produto, sizeof(PRODUCT), 1, tmp_prod);

            if (produto.prox != -1)
                fseek(produtos, produto.prox, SEEK_SET);
        }

        fclose(produtos);
        fclose(tmp_prod);
        remove(PATH_DADOS_PROD);
        rename(DIR_DADOS "tmp_prod.bin", PATH_DADOS_PROD);

        alterar_config(6, 1);
        criar_arquivos_indice(0);
    }

    if(opcao == 1 || opcao == 2)
    {
        FILE *pedidos = abrir(PATH_DADOS_ORDER,"rb");
        FILE *tmp_ped = abrir(DIR_DADOS "tmp_ped.bin","wb");

        while(fread(&pedido, sizeof(ORDER), 1, pedidos))
        {
            if (pedido.ant != -1)
            {
                fseek(pedidos, pedido.ant, SEEK_SET);
                continue;
            }

            if(!pedido.exclusao)
                fwrite(&pedido, sizeof(ORDER), 1, tmp_ped);

            if (pedido.prox != -1)
                fseek(pedidos, pedido.prox, SEEK_SET);
        }

        fclose(pedidos);
        fclose(tmp_ped);
        remove(PATH_DADOS_ORDER);
        rename(DIR_DADOS "tmp_ped.bin", PATH_DADOS_ORDER);

        alterar_config(7, 1);
        criar_arquivos_indice(1);
    }
}

void configurar(int item, int novo)
{
    if((item != 1 && item != 2) || novo < 0) return;
    FILE *conf = abrir(ARQ_CONFIG,"r+b");
    if(!conf) return;
    int aux;
    if(item == 1)
        fwrite(&novo,sizeof(int),1,conf);
    else if (item == 2)
    {
        fread(&aux,sizeof(int),1,conf);
        fwrite(&novo,sizeof(int),1,conf);
    }
    fclose(conf);
}

void cria_config_base()
{
    FILE *conf = abrir(PATH_CONFIG,"wb");
    int v[8]={0};
    v[0] = v[1] = 10;
    v[6] = v[7] = 1;
    fwrite(v,sizeof(int),8,conf);
    fclose(conf);    
}

void criar_arquivos_base()
{
    criar_arquivos_dados();
    criar_arquivos_indice(2);
    cria_config_base();
}


void atualizar_inicio(const char * file, long offset, long novo_inicio)
{
    FILE *f = abrir(file,"rb+");
    if(!f) return;
    fseek(f, offset, SEEK_SET);
    INDEX novo_ind;
    fread(&novo_ind, sizeof(INDEX), 1, f);
    novo_ind.endereco = novo_inicio;
    fseek(f, -sizeof(INDEX), SEEK_CUR);
    fwrite(&novo_ind, sizeof(INDEX), 1, f);
    fclose(f);
}

void atualizar_final(const char * file, long offset, unsigned long long nova_chave)
{
    FILE *f = abrir(file,"rb+");
    if(!f) return;
    fseek(f, offset, SEEK_SET);
    INDEX novo_ind;
    fread(&novo_ind, sizeof(INDEX), 1, f);
    novo_ind.chave = nova_chave;
    fseek(f, -sizeof(INDEX), SEEK_CUR);
    fwrite(&novo_ind, sizeof(INDEX), 1, f);
    fclose(f);
}

void inserir_produto(PRODUCT produto)
{
    // é necessario encontrar em que posicao o novo produto ficara no arquivo de dados de produtos. ele sera inserido no final do arquivo, mas os ponteiros prox e ant devem ser atualizados para manter a ordem
    FILE *produtos_leitura = abrir(PATH_DADOS_PROD,"r+b");
    if(!produtos_leitura) return;

    PRODUCT produto_atual;
    long posicao_atual = 0;
    long posicao_anterior = -1;

    // Lê os produtos existentes para encontrar a posição correta
    while(fread(&produto_atual, sizeof(PRODUCT), 1, produtos_leitura))
    {
        if(produto.product_id < produto_atual.product_id) {
            break; // Encontrou a posição onde o novo produto deve ser inserido
        }
        posicao_anterior = posicao_atual; // Atualiza a posição anterior
        posicao_atual += 1; // Avança para o próximo índice de registro (não bytes)
    }

    // Atualiza os ponteiros prox e ant do novo produto
    produto.ant = posicao_anterior; // O novo produto aponta para o anterior
    // Se chegou ao final do arquivo, prox deve ser -1
    produto.prox = feof(produtos_leitura) ? -1 : posicao_atual; // O novo produto aponta para o próximo, se houver

    fseek(produtos_leitura, 0, SEEK_END);
    long posicao_novo_produto = ftell(produtos_leitura) / sizeof(PRODUCT);

    if (posicao_anterior != -1) {
        // O novo produto será adicionado ao final do arquivo

        // Atualiza o produto anterior para apontar para o novo produto
        fseek(produtos_leitura, posicao_anterior * sizeof(PRODUCT), SEEK_SET);
        PRODUCT produto_anterior;
        fread(&produto_anterior, sizeof(PRODUCT), 1, produtos_leitura);
        produto_anterior.prox = posicao_novo_produto; // Atualiza o próximo do anterior
        fseek(produtos_leitura, -sizeof(PRODUCT), SEEK_CUR);
        fwrite(&produto_anterior, sizeof(PRODUCT), 1, produtos_leitura);

        if (produto.prox != -1) {
            // Atualiza o produto posterior para apontar para o novo produto
            fseek(produtos_leitura, produto.prox * sizeof(PRODUCT), SEEK_SET);
            PRODUCT produto_posterior;
            fread(&produto_posterior, sizeof(PRODUCT), 1, produtos_leitura);
            produto_posterior.ant = posicao_novo_produto;
            fseek(produtos_leitura, -sizeof(PRODUCT), SEEK_CUR);
            fwrite(&produto_posterior, sizeof(PRODUCT), 1, produtos_leitura);
        }
    }

    // Se o novo produto for o primeiro, atualiza o ponteiro do início
    if (posicao_anterior == -1) {
        // O novo produto será o primeiro

        if (produto.prox != -1) {
            // Atualiza o produto que era o primeiro para apontar para o novo produto
            fseek(produtos_leitura, produto.prox * sizeof(PRODUCT), SEEK_SET);
            PRODUCT produto_posterior;
            fread(&produto_posterior, sizeof(PRODUCT), 1, produtos_leitura);
            produto_posterior.ant = posicao_novo_produto;
            fseek(produtos_leitura, -sizeof(PRODUCT), SEEK_CUR);
            fwrite(&produto_posterior, sizeof(PRODUCT), 1, produtos_leitura);
        }
    }

    // Adiciona o novo produto ao final do arquivo
    fseek(produtos_leitura, 0, SEEK_END);
    fwrite(&produto, sizeof(PRODUCT), 1, produtos_leitura);
    fclose(produtos_leitura);

    // atualiza contador de insercoes de produtos e, se atingir o limite configurado, reordena
    int num_insercoes = checar_config(2);
    int max_insercoes = checar_config(0);
    if (num_insercoes < 0) 
        num_insercoes = 0; // segurança caso função retorne valor inválido
    num_insercoes++;
    alterar_config(2, num_insercoes);
    if (max_insercoes > 0 && num_insercoes >= max_insercoes)
    {
        reordenar(0);          // reordena arquivo de produtos
        alterar_config(2, 0);  // reseta contador de inserções
    }
}

void inserir_pedido(ORDER pedido)
{
    
}

void inserir_produto_pedido(unsigned long long order_id, unsigned long long product_id)
{
    
}

void remover_produto_pedido(unsigned long long order_id, unsigned long long product_id)
{
    
}

void remover_produto(unsigned long long product_id)
{
    
}

void remover_pedido(unsigned long long order_id)
{
    
}

//add e remover produto pedido produto-pedido
//unsigned long long enda, end, endp; // Anterior, atual, final
//cuidar se for antes do primeiro registro
