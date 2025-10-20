#include <stdio.h>
#include <stdlib.h>
#include "gerir.c"

void recriar_arquivos_base()
{
    limpar_tela("");
    char x;
    printf("Tem certeza que deseja recriar os arquivos apartir do %s? (Y/N)\n",ARQ_DADOS_ORIGEM);
    scanf("%c", &x);
    if(x=='Y' || x=='y')
        criar_arquivos_base();
    else
        limpar_tela("Criacao de Arquivos Cancelada\n");
}

void reorganizar_tudo()
{
    limpar_tela("");
    char x;
    printf("Tem certeza que deseja reordenar todos os arquivos? (Y/N)\n");
    scanf("%c", &x);
    if(x=='Y' || x=='y')
        reordenar();
    else
        limpar_tela("Reordenacao Cancelada\n");
}

void busca_pedido()
{
    unsigned long long id;
    printf("Digite o ID do pedido: ");
    scanf("%llu",&id);
    reordenar(); // se nao tiver organizado
    ORDER pedido = indice_pedido_binaria(id);
    if(pedido.order_id == 0)
        printf("Pedido nao encontrado\n");
    else
    {
        printf("Pedido encontrado\n");
        printf("ID: %d\n",pedido.order_id);
        printf("Data: %d/%d/%d %d:%d:%d\n",pedido.date_time.day,pedido.date_time.month,pedido.date_time.year,pedido.date_time.hour,pedido.date_time.min,pedido.date_time.sec);
        printf("User ID: %d\n",pedido.user_id);
        printf("Quantidade de produtos: %d\n",pedido.products_amount);
        for(int i=0; i<pedido.products_amount; i++)
        {
            //TODAS INFOS DO PRODUTO???
            printf("\tProduto[%d]: %d\n",i+1,pedido.products_id[i]);
            printf("\tSKU[%d]: %d\n",i+1,pedido.SKU_in_order[i]);
        }
    }
    printf("------------------------------------\n");
}

void busca_produto()
{
    unsigned long long id;
    printf("Digite o ID do produto: ");
    scanf("%llu",&id);
    reordenar(); // se nao tiver organizado
    PRODUCT produto = indice_produto_binaria(id);
    if(produto.product_id == 0)
        printf("Produto nao encontrado\n");
    else
    {
        printf("Produto encontrado\n");
        printf("ID: %d\n",produto.product_id);
        printf("Categoria: %d\n",produto.category_id);
        printf("Categoria Alias: %s\n",produto.category_alias);
        printf("Brand ID: %d\n",produto.brand_id);
        printf("Preço: %.2f\n",produto.price);
        printf("Gênero: %c\n",produto.product_gender);
        printf("Cor principal: %s\n",produto.main_color);
        printf("Metal principal: %s\n",produto.main_metal);
        printf("Gema principal: %s\n",produto.main_gem);
    }
    printf("------------------------------------\n");
}

void todos_produtos()
{
    FILE *produtos = abrir(PATH_DADOS_PROD,"rb");
    PRODUCT produto;
    int i=0;
    while(fread(&produto, sizeof(PRODUCT), 1, produtos))
    {
        printf("------------------------------------\n");
        printf("Produto %d\n",i++);
        printf("ID: %d\n",produto.product_id);
        printf("Categoria: %d\n",produto.category_id);
        printf("Categoria Alias: %s\n",produto.category_alias);
        printf("Brand ID: %d\n",produto.brand_id);
        printf("Preço: %.2f\n",produto.price);
        printf("Gênero: %c\n",produto.product_gender);
        printf("Cor principal: %s\n",produto.main_color);
        printf("Metal principal: %s\n",produto.main_metal);
        printf("Gema principal: %s\n",produto.main_gem);
    }
    printf("------------------------------------\n");
}

void todos_pedidos()
{
    FILE *pedidos = abrir(PATH_DADOS_ORDER,"rb");
    ORDER pedido;
    int i=0;
    while(fread(&pedido, sizeof(ORDER), 1, pedidos))
    {
        printf("------------------------------------\n");
        printf("Pedido %d\n",i++);
        printf("ID: %d\n",pedido.order_id);
        printf("Data: %d/%d/%d %d:%d:%d\n",pedido.date_time.day,pedido.date_time.month,pedido.date_time.year,pedido.date_time.hour,pedido.date_time.min,pedido.date_time.sec);
        printf("User ID: %d\n",pedido.user_id);
        printf("Quantidade de produtos: %d\n",pedido.products_amount);
        printf("Produtos:\n\t");
        for(int i=0; i<pedido.products_amount; i++)
            printf("[%d[%d]] ",pedido.products_id[i],pedido.SKU_in_order[i]);
        printf("\n");
    }
    printf("------------------------------------\n");
}

void maximo_insercoes()
{
    int maximo;
    FILE *conf = abrir(ARQ_CONFIG,"rb");
    fread(&maximo,sizeof(int),1,conf);
    fread(&maximo,sizeof(int),1,conf);
    fclose(conf);
    printf("O maximo atual de insercoes e: %d\n",maximo);
    printf("------------------------------------\n");
    printf("Informe o novo limite: ");    
    scanf("%d",&maximo);
    printf("------------------------------------\n");
    configurar(1,maximo);
}

void maximo_remocoes()
{
    int maximo;
    FILE *conf = abrir(ARQ_CONFIG,"rb");
    fread(&maximo,sizeof(int),1,conf);
    fread(&maximo,sizeof(int),1,conf);
    fread(&maximo,sizeof(int),1,conf);
    fread(&maximo,sizeof(int),1,conf);
    fclose(conf);
    printf("O maximo atual de remocoes e: %d\n",maximo);
    printf("------------------------------------\n");
    printf("Informe o novo limite: ");    
    scanf("%d",&maximo);
    printf("------------------------------------\n");
    configurar(2,maximo);
}
//consulta com exibicoes 
//insercoes e remocoes 

//pesquisa por data??? (possivel ideia se der tempo)
