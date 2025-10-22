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

void atualizar_inicio_indice(const char * file, long endereco, long novo_inicio)
{
    FILE *f = abrir(file,"rb+");
    if(!f) return;
    INDEX novo_ind;
    while(fread(&novo_ind, sizeof(INDEX), 1, f) == 1) 
        if (novo_ind.endereco == endereco) break;
    novo_ind.endereco = novo_inicio;
    fseek(f, -sizeof(INDEX), SEEK_CUR);
    fwrite(&novo_ind, sizeof(INDEX), 1, f);
    fclose(f);
}

void atualizar_final_indice(const char * file, long endereco, unsigned long long nova_chave)
{
    FILE *f = abrir(file,"rb+");
    if(!f) return;
    INDEX novo_ind;
    while(fread(&novo_ind, sizeof(INDEX), 1, f) == 1) 
        if (novo_ind.endereco == endereco) break;
    novo_ind.chave = nova_chave;
    fseek(f, -sizeof(INDEX), SEEK_CUR);
    fwrite(&novo_ind, sizeof(INDEX), 1, f);
    fclose(f);
}

void criar_arquivos_dados()
{
    PRODUCT *produtos_mem =(PRODUCT *) malloc(MAX_RECORDS * sizeof(PRODUCT));
    ORDER   *pedidos_mem  =(ORDER *) malloc(MAX_RECORDS * sizeof(ORDER));
    int n_prod = 0, n_ped = 0;
    FILE *dados = abrir(PATH_DADOS_ORIGEM,"r");
    char linha[122 + (30 * AMOUNT_MAX) + SIZE_CATEGORY + (3 * SIZE_MAIN_CMG) + 10 + 30];
    limpar_tela("");  
    while(fgets(linha, sizeof(linha), dados))
    {
        if(n_ped % 5000 == 0)
            printf("\rPedidos: %d - Produtos: %d",n_ped,n_prod);

        linha[strcspn(linha, "\n")] = '\0';
        int sku;
        PRODUCT prod;
        ORDER ped;
        desconstruir_linha(linha, &sku, &ped, &prod);
        if(!existe_produto(produtos_mem,prod.product_id,n_prod))
            produtos_mem[n_prod++] = prod;
        int indice = existe_pedido(pedidos_mem, ped.order_id, n_ped);
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

    fclose(dados);

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
    unsigned long long ant = 0;

    if(opcao == 0 || opcao == 2)
    {
        limpar_tela("");
        int n_prod = 0;
        FILE *produtos = abrir(PATH_DADOS_PROD,"rb");
        FILE *tmp_prod = abrir(DIR_DADOS "tmp_prod.bin","wb");

        while(fread(&produto, sizeof(PRODUCT), 1, produtos))
            if (produto.ant != -1)
            {
                fseek(produtos, produto.ant, SEEK_SET);
            }
            else
            {
                fseek(produtos, -sizeof(PRODUCT), SEEK_CUR);
                break;
            }

        while(fread(&produto, sizeof(PRODUCT), 1, produtos) && ant < produto.product_id)
        {
            ant = produto.product_id;
            n_prod++;
            long prox = produto.prox;
            produto.ant = produto.prox = -1;
            if(n_prod % 200 == 0)
                printf("\rProdutos: %d",n_prod);
            if(!produto.exclusao)
                fwrite(&produto, sizeof(PRODUCT), 1, tmp_prod);
            if (prox != -1)
                fseek(produtos, prox, SEEK_SET);
        }

        fclose(produtos);
        fclose(tmp_prod);
        remove(PATH_DADOS_PROD);
        rename(DIR_DADOS "tmp_prod.bin", PATH_DADOS_PROD);

        alterar_config(6, 1);
        alterar_config(2, 0);
        alterar_config(4, 0);
        criar_arquivos_indice(0);
        printf("\n");
    }
    if(opcao == 1 || opcao == 2)
    {
        ant = 0;
        int n_ped = 0;
        if(opcao == 1) limpar_tela("");
        FILE *pedidos = abrir(PATH_DADOS_ORDER,"rb");
        FILE *tmp_ped = abrir(DIR_DADOS "tmp_ped.bin","wb");

        while(fread(&pedido, sizeof(ORDER), 1, pedidos))
            if (pedido.ant != -1)
            {
                fseek(pedidos, pedido.ant, SEEK_SET);
                continue;
            }
            else
            {
                fseek(pedidos, -sizeof(ORDER), SEEK_CUR);
                break;
            }

        while(fread(&pedido, sizeof(ORDER), 1, pedidos) && ant < pedido.order_id)
        {
            ant = pedido.order_id;
            n_ped++;
            long prox = pedido.prox;
            pedido.ant = pedido.prox = -1;
            if(n_ped % 200 == 0)
                printf("\rPedidos: %d",n_ped);
            if(!pedido.exclusao)
                fwrite(&pedido, sizeof(ORDER), 1, tmp_ped);

            if (prox != -1)
                fseek(pedidos, prox, SEEK_SET);
        }

        fclose(pedidos);
        fclose(tmp_ped);
        remove(PATH_DADOS_ORDER);
        rename(DIR_DADOS "tmp_ped.bin", PATH_DADOS_ORDER);

        alterar_config(7, 1);
        alterar_config(3, 0);
        alterar_config(5, 0);
        criar_arquivos_indice(1);
        printf("\n");
    }
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

int esta_pedido(unsigned long long produto_id)
{
    FILE *pedidos = abrir(PATH_DADOS_ORDER,"rb");
    if(!pedidos) return 0;
    ORDER pedido;
    while(fread(&pedido, sizeof(ORDER), 1, pedidos))
    {
        for(int i=0; i<pedido.products_amount; i++)
            if(pedido.products_id[i] == produto_id)
            {
                fclose(pedidos);
                return 1;
            }
    }
    fclose(pedidos);
    return 0;
}

void criar_arquivos_base()
{
    criar_arquivos_dados();
    criar_arquivos_indice(2);
    cria_config_base();
}

void inserir_produto(INDEX indice, PRODUCT produto)
{
    // é necessario encontrar em que posicao o novo produto ficara no arquivo de dados de produtos. ele sera inserido no final do arquivo, mas os ponteiros prox e ant devem ser atualizados para manter a ordem
    FILE *produtos_leitura = abrir(PATH_DADOS_PROD,"r+b");
    if(!produtos_leitura  || indice.endereco == -1) return;
    produto.ant = produto.prox = -1;
    produto.exclusao = 0;

    PRODUCT produto_atual, produto_anterior;
    long posicao_atual,posicao_anterior = -1;

    // Lê os produtos existentes para encontrar a posição correta
    fseek(produtos_leitura, indice.endereco, SEEK_SET);
    posicao_atual = ftell(produtos_leitura);
    while(fread(&produto_atual, sizeof(PRODUCT), 1, produtos_leitura))
    {
        if(produto.product_id < produto_atual.product_id || produto_atual.product_id == indice.chave) {
            break; // Encontrou a posição onde o novo produto deve ser inserido
        }
        if(produto.product_id == produto_atual.product_id) {
            fclose(produtos_leitura);
            return;
        }
        produto_anterior = produto_atual;
        posicao_anterior = posicao_atual; // Atualiza a posição anterior
        if(produto_atual.prox != -1)
            fseek(produtos_leitura, produto_atual.prox, SEEK_SET);
        posicao_atual = ftell(produtos_leitura); // Avança para o próximo índice de registro 
    }

    fseek(produtos_leitura, 0, SEEK_END);
    long posicao_novo_produto = ftell(produtos_leitura);

    if(produto.product_id < indice.chave)
    {
        produto.prox = posicao_atual;
    }
    else{
        if(produto.product_id == indice.chave) {
            fclose(produtos_leitura);
            return;
        }
        produto_anterior = produto_atual;
        posicao_anterior = posicao_atual; 
        produto_anterior.prox = posicao_novo_produto;
        atualizar_final_indice(PATH_INDEX_PROD, indice.endereco, produto.product_id);
    }

    if(posicao_anterior == -1)
    {
        produto_atual.ant = posicao_novo_produto;
        atualizar_inicio_indice(PATH_INDEX_PROD, indice.endereco, posicao_novo_produto);
    }
    else{
        produto_anterior.prox = posicao_novo_produto;
    }

    // O novo produto será adicionado ao final do arquivo
    if(posicao_anterior == -1)
    {
        fseek(produtos_leitura, posicao_atual, SEEK_SET);
        fwrite(&produto_atual, sizeof(PRODUCT), 1, produtos_leitura);
    }else{
        fseek(produtos_leitura, posicao_anterior, SEEK_SET);
        fwrite(&produto_anterior, sizeof(PRODUCT), 1, produtos_leitura);
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
    if (max_insercoes > 0 && num_insercoes >= max_insercoes)
    {
        reordenar(0);          // reordena arquivo de produtos
        alterar_config(6, 1);
        num_insercoes = 0;
    }
    else
        alterar_config(6, 0);
    alterar_config(2, num_insercoes);
}

void inserir_pedido(INDEX indice, ORDER pedido)
{
    FILE *pedidos_leitura = abrir(PATH_DADOS_ORDER,"r+b");
    if(!pedidos_leitura  || indice.endereco == -1) return;
    pedido.ant = pedido.prox = -1;
    pedido.exclusao = 0;

    ORDER pedido_atual, pedido_anterior;
    long posicao_atual,posicao_anterior = -1;

    fseek(pedidos_leitura, indice.endereco, SEEK_SET);
    posicao_atual = ftell(pedidos_leitura);
    while(fread(&pedido_atual, sizeof(ORDER), 1, pedidos_leitura))
    {
        if(pedido.order_id < pedido_atual.order_id || pedido_atual.order_id == indice.chave) {
            break; 
        }
        if(pedido.order_id == pedido_atual.order_id) {
            fclose(pedidos_leitura);
            return;
        }
        pedido_anterior = pedido_atual;
        posicao_anterior = posicao_atual; // Atualiza a posição anterior
        if(pedido_atual.prox != -1)
            fseek(pedidos_leitura, pedido_atual.prox, SEEK_SET);
        posicao_atual = ftell(pedidos_leitura); // Avança para o próximo índice de registro 
    }

    fseek(pedidos_leitura, 0, SEEK_END);
    long posicao_novo_pedido = ftell(pedidos_leitura);

    if(pedido.order_id < indice.chave)
    {
        pedido.prox = posicao_atual;
    }
    else{
        if(pedido.order_id == indice.chave) {
            fclose(pedidos_leitura);
            return;
        }
        pedido_anterior = pedido_atual;
        posicao_anterior = posicao_atual; 
        pedido_anterior.prox = posicao_novo_pedido;
        atualizar_final_indice(PATH_INDEX_ORDER, indice.endereco, pedido.order_id);
    }

    if(posicao_anterior == -1)
    {
        pedido_atual.ant = posicao_novo_pedido;
        atualizar_inicio_indice(PATH_INDEX_ORDER, indice.endereco, posicao_novo_pedido);
    }
    else{
        pedido_anterior.prox = posicao_novo_pedido;
    }

    if(posicao_anterior == -1)
    {
        fseek(pedidos_leitura, posicao_atual, SEEK_SET);
        fwrite(&pedido_atual, sizeof(ORDER), 1, pedidos_leitura);
    }else{
        fseek(pedidos_leitura, posicao_anterior, SEEK_SET);
        fwrite(&pedido_anterior, sizeof(ORDER), 1, pedidos_leitura);
    }
        
    fseek(pedidos_leitura, 0, SEEK_END);
    fwrite(&pedido, sizeof(ORDER), 1, pedidos_leitura);
    fclose(pedidos_leitura);

    int num_insercoes = checar_config(3);
    int max_insercoes = checar_config(0);
    if (num_insercoes < 0) 
        num_insercoes = 0; // segurança caso função retorne valor inválido
    num_insercoes++;
    if (max_insercoes > 0 && num_insercoes >= max_insercoes)
    {
        reordenar(1);          // reordena arquivo de pedidos
        alterar_config(7, 1);
        num_insercoes = 0;
    }
    else
        alterar_config(7, 0);
    alterar_config(3, num_insercoes);
}

void atualizar_pedido(INDEX indice, ORDER pedido)
{
    FILE *pedidos_leitura = abrir(PATH_DADOS_ORDER,"r+b");
    if(!pedidos_leitura  || indice.endereco == -1) return;

    ORDER pedido_atual;

    fseek(pedidos_leitura, indice.endereco, SEEK_SET);
    while(fread(&pedido_atual, sizeof(ORDER), 1, pedidos_leitura))
    {
        if(pedido.order_id == pedido_atual.order_id) {
            fseek(pedidos_leitura, -sizeof(ORDER), SEEK_CUR);
            fwrite(&pedido, sizeof(ORDER), 1, pedidos_leitura);
            fclose(pedidos_leitura);
            return;
        }
        if(pedido_atual.prox != -1)
            fseek(pedidos_leitura, pedido_atual.prox, SEEK_SET);
    }
    fclose(pedidos_leitura);
    return;
}

void remover_produto(INDEX indice, unsigned long long product_id)
{
    // Localiza o produto pelo product_id, marca exclusao = 1 e atualiza no arquivo

    if(esta_pedido(product_id))
    {
        printf("O produto %llu ja esta em algum pedido, nao pode ser removido\n",product_id);
        return;
    }

    FILE *produtos = abrir(PATH_DADOS_PROD, "r+b");
    if (!produtos || indice.endereco == -1) return;

    PRODUCT produto;
    long inicio = indice.endereco;
    int encontrado = 0;
    fseek(produtos, inicio, SEEK_SET);
    while (fread(&produto, sizeof(PRODUCT), 1, produtos)) {
        if (produto.product_id == product_id) {
            produto.exclusao = 1;
            encontrado = 1;
            fseek(produtos, -sizeof(PRODUCT), SEEK_CUR);
            fwrite(&produto, sizeof(PRODUCT), 1, produtos);
            break;
        }
        if(produto.prox != -1)
        {
            fseek(produtos, produto.prox, SEEK_SET);
            continue;
        }
        if(produto.product_id == indice.chave)
            break;
    }
    fclose(produtos);

    if (!encontrado) 
        return;
    

    int num_remocoes = checar_config(4);
    num_remocoes++;
    int max_remocoes = checar_config(1);
    if (max_remocoes > 0 && num_remocoes >= max_remocoes)
    {
        reordenar(0);
        alterar_config(6, 1);
        num_remocoes = 0;
    }
    else
        alterar_config(6, 0);
    alterar_config(4, num_remocoes);
}

void remover_pedido(INDEX indice, unsigned long long order_id)
{
    FILE *pedidos = abrir(PATH_DADOS_ORDER, "r+b");
    if (!pedidos || indice.endereco == -1) return;

    ORDER pedido;
    long inicio = indice.endereco;
    int encontrado = 0;
    fseek(pedidos, inicio, SEEK_SET);
    while (fread(&pedido, sizeof(ORDER), 1, pedidos)) {
        if (pedido.order_id == order_id) {
            pedido.exclusao = 1;
            encontrado = 1;
            fseek(pedidos, -sizeof(ORDER), SEEK_CUR);
            fwrite(&pedido, sizeof(ORDER), 1, pedidos);
            break;
        }
        if(pedido.prox != -1)
        {
            fseek(pedidos, pedido.prox, SEEK_SET);
            continue;
        }
        if(pedido.order_id == indice.chave)
            break;
    }
    fclose(pedidos);

    if (!encontrado) 
        return;
    
    int num_remocoes = checar_config(5);
    num_remocoes++;
    int max_remocoes = checar_config(1);
    if (max_remocoes > 0 && num_remocoes >= max_remocoes)
    {
        reordenar(1);
        alterar_config(7, 1);
        num_remocoes = 0;
    }
    else
        alterar_config(7, 0);
    alterar_config(5, num_remocoes);
}

void criar_novo_csv()
{

}
