#include <stdio.h>
#include <stdlib.h>
#include "gerir.c"

void busca_pedido()
{
    unsigned long long id;
    if(!checar_config(7))
    {
        printf("O arquivo de pedidos nao esta ordenado, e necessario \nfazer uma reordenacao para realizar a busca binaria\n");
        return;
    }
    printf("Digite o ID do pedido: ");
    scanf("%llu",&id);
    ORDER pedido = busca_pedido_binaria(indice_pedido_binaria(id).endereco, id);
    if(pedido.order_id == 0)
        printf("Pedido nao encontrado\n");
    else
    {
        printf("Pedido encontrado\n");
        printf("ID: %llu\n",pedido.order_id);
        printf("Data: %02d/%02d/%02d %02d:%02d:%02d\n",pedido.date_time.day,pedido.date_time.month,pedido.date_time.year,pedido.date_time.hour,pedido.date_time.min,pedido.date_time.sec);
        printf("User ID: %llu\n",pedido.user_id);
        printf("Quantidade de produtos: %d\n",pedido.products_amount);
        for(int i=0; i<pedido.products_amount; i++)
        {
            //TODAS INFOS DO PRODUTO???
            printf("\tProduto[%d]: %llu\n",i+1,pedido.products_id[i]);
            printf("\tSKU[%d]: %d\n",i+1,pedido.SKU_in_order[i]);
        }
    }
    printf("------------------------------------\n");
}

void busca_produto()
{
    unsigned long long id;
    if(!checar_config(6))
    {
        printf("O arquivo de produtos nao esta ordenado, e necessario \nfazer uma reordenacao para realizar a busca binaria\n");
        return;
    }
    printf("Digite o ID do produto: ");
    scanf("%llu",&id);
    PRODUCT produto = busca_produto_binaria(indice_produto_binaria(id).endereco, id);
    if(produto.product_id == 0)
        printf("Produto nao encontrado\n");
    else
    {
        printf("Produto encontrado\n");
        printf("ID: %llu\n",produto.product_id);
        printf("Categoria: %llu\n",produto.category_id);
        printf("Categoria Alias: %s\n",produto.category_alias);
        printf("Brand ID: %d\n",produto.brand_id);
        printf("Preco: %.2f\n",produto.price);
        printf("Genero: %c\n",produto.product_gender);
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
    unsigned long long ant = 0;
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
        if(!produto.exclusao){
            printf("------------------------------------\n");
            printf("Produto %d\n",i++);
            printf("ID: %llu\n",produto.product_id);
            printf("Categoria: %llu\n",produto.category_id);
            printf("Categoria Alias: %s\n",produto.category_alias);
            printf("Brand ID: %d\n",produto.brand_id);
            printf("Preco: %.2f\n",produto.price);
            printf("Genero: %c\n",produto.product_gender);
            printf("Cor principal: %s\n",produto.main_color);
            printf("Metal principal: %s\n",produto.main_metal);
            printf("Gema principal: %s\n",produto.main_gem);
        }
        if (produto.prox != -1)
                fseek(produtos, produto.prox, SEEK_SET);
    }
    fclose(produtos);
    printf("------------------------------------\n");
}

void todos_pedidos()
{
    FILE *pedidos = abrir(PATH_DADOS_ORDER,"rb");
    ORDER pedido;
    int i=0;
    unsigned long long ant = 0;
    while(fread(&pedido, sizeof(ORDER), 1, pedidos))
        if (pedido.ant != -1)
        {
            fseek(pedidos, pedido.ant, SEEK_SET);
        }
        else
        {
            fseek(pedidos, -sizeof(ORDER), SEEK_CUR);
            break;
        }
    while(fread(&pedido, sizeof(ORDER), 1, pedidos) && ant < pedido.order_id)
    {
        ant = pedido.order_id;
        if(!pedido.exclusao){
            printf("------------------------------------\n");
            printf("Pedido %d\n",i++);
            printf("ID: %llu\n",pedido.order_id);
            printf("Data: %02d/%02d/%d %02d:%02d:%02d\n",pedido.date_time.day,pedido.date_time.month,pedido.date_time.year,pedido.date_time.hour,pedido.date_time.min,pedido.date_time.sec);
            printf("User ID: %llu\n",pedido.user_id);
            printf("Quantidade de produtos: %d\n",pedido.products_amount);
            printf("Produtos:\n\t");
            for(int i=0; i<pedido.products_amount; i++)
                printf("[%llu[%d]] ",pedido.products_id[i],pedido.SKU_in_order[i]);
            printf("\n");
        }
        if (pedido.prox != -1)
            fseek(pedidos, pedido.prox, SEEK_SET);
    }
    fclose(pedidos);
    printf("------------------------------------\n");
}

void maximo_insercoes()
{
    int maximo;
    printf("O maximo atual de insercoes e: %d\n",checar_config(0));
    printf("------------------------------------\n");
    printf("Informe o novo limite: ");    
    scanf("%d",&maximo);
    printf("------------------------------------\n");
    alterar_config(0,maximo);
}

void maximo_remocoes()
{
    int maximo;
    printf("O maximo atual de remocoes e: %d\n",checar_config(1));
    printf("------------------------------------\n");
    printf("Informe o novo limite: ");    
    scanf("%d",&maximo);
    printf("------------------------------------\n");
    alterar_config(1,maximo);
}

void adicionar_produto()
{
    PRODUCT produto;
    printf("Digite o ID do produto: ");
    scanf("%llu",&produto.product_id);
    printf("Digite o id da categoria do produto: ");
    scanf("%llu",&produto.category_id);
    printf("Digite o apelido da categoria do produto: ");
    scanf("%s",produto.category_alias);
    printf("Digite o id da marca do produto: ");
    scanf("%d",&produto.brand_id);
    printf("Digite o preco do produto: ");
    scanf("%f",&produto.price);
    getchar();
    printf("Digite o genero do produto: ");
    scanf("%c",&produto.product_gender);
    printf("Digite a cor principal do produto: ");
    scanf("%s",produto.main_color);
    printf("Digite o metal principal do produto: ");
    scanf("%s",produto.main_metal);
    printf("Digite a gema principal do produto: ");
    scanf("%s",produto.main_gem);
    inserir_produto(indice_produto_binaria(produto.product_id), produto);
}

void adicionar_pedido()
{
    ORDER pedido={0};
    printf("Digite a data do pedido: (dd/mm/aaaa hh:mm:ss)\n");
    scanf("%d/%d/%d %d:%d:%d",&pedido.date_time.year,&pedido.date_time.month,&pedido.date_time.day,&pedido.date_time.hour,&pedido.date_time.min,&pedido.date_time.sec);
    printf("Digite o ID do pedido: ");
    scanf("%llu",&pedido.order_id);
    printf("Digite o ID do usuário: ");
    scanf("%llu",&pedido.user_id);
    printf("Digite o ID do produto: ");
    scanf("%llu",&pedido.products_id[0]);
    printf("Digite a quantidade do produto: ");
    scanf("%d",&pedido.SKU_in_order[0]);
    pedido.products_amount = 1;
    inserir_pedido(indice_pedido_binaria(pedido.order_id), pedido);
}

void adicionar_produto_pedido()
{
    unsigned long long order_id;
    printf("Digite o ID do pedido: ");
    scanf("%llu",&order_id);
    ORDER pedido = busca_pedido_binaria(indice_pedido_binaria(order_id).endereco, order_id);
    if(pedido.order_id == 0)
        printf("Pedido nao encontrado\n");
    else
    {
        printf("Pedido encontrado\n");
        printf("Digite o ID do produto: ");
        scanf("%llu",&pedido.products_id[pedido.products_amount]);
        printf("Digite a quantidade do produto: ");
        scanf("%d",&pedido.SKU_in_order[pedido.products_amount]);
        pedido.products_amount++;
        PRODUCT produto = busca_produto_binaria(indice_produto_binaria(pedido.products_id[pedido.products_amount-1]).endereco, pedido.products_id[pedido.products_amount-1]);
        if(produto.product_id == 0)
            printf("Produto nao encontrado\n");
        else
            atualizar_pedido(indice_pedido_binaria(order_id), pedido);
    }
}

void remove_produto_pedido()
{
    unsigned long long order_id;
    printf("Digite o ID do pedido: ");
    scanf("%llu",&order_id);
    ORDER pedido = busca_pedido_binaria(indice_pedido_binaria(order_id).endereco, order_id);
    if(pedido.order_id == 0)
        printf("Pedido nao encontrado\n");
    else
    {
        unsigned long long produto_id;
        printf("Pedido encontrado\n");
        printf("Digite o ID do produto: ");
        scanf("%llu",&produto_id);
        PRODUCT produto = busca_produto_binaria(indice_produto_binaria(produto_id).endereco, produto_id);
        if(produto.product_id == 0)
        {
            printf("Produto nao encontrado\n");
            return;
        }
        for(int i=0; i<pedido.products_amount; i++)
            if(pedido.products_id[i] == produto_id)
            {
                pedido.products_amount--;
                for(int j=i; j<pedido.products_amount; j++)
                {
                    pedido.products_id[j] = pedido.products_id[j+1];
                    pedido.SKU_in_order[i] = pedido.SKU_in_order[i+1];
                }
                pedido.products_id[pedido.products_amount] = 0;
                pedido.SKU_in_order[pedido.products_amount] = 0;
                break;
            }
        atualizar_pedido(indice_pedido_binaria(order_id), pedido);
    }
}

void remove_pedido()
{
    unsigned long long id;
    printf("Digite o ID do pedido: ");
    scanf("%llu",&id);
    remover_pedido(indice_pedido_binaria(id),id);
}

void remove_produto()
{
    unsigned long long id;
    printf("Digite o ID do produto: ");
    scanf("%llu",&id);
    remover_produto(indice_produto_binaria(id),id);
}

void produtos_por_categoria()
{
    char categoria[SIZE_CATEGORY];
    printf("Digite o apelido da categoria: ");
    scanf("%s",categoria);
    copiar_e_preencher(categoria,categoria,SIZE_CATEGORY);
    FILE *produtos = abrir(PATH_DADOS_PROD,"rb");
    if(!produtos) return;
    PRODUCT produto;
    int i=0;
    unsigned long long ant = 0;
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
        if(comparar_strings(categoria,produto.category_alias) == 0)
        {
            printf("------------------------------------\n");
            printf("Produto %d\n",i++);
            printf("ID: %llu\n",produto.product_id);
            printf("Categoria: %llu\n",produto.category_id);
            printf("Categoria Alias: %s\n",produto.category_alias);
            printf("Brand ID: %d\n",produto.brand_id);
            printf("Preco: %.2f\n",produto.price);
            printf("Genero: %c\n",produto.product_gender);
            printf("Cor principal: %s\n",produto.main_color);
            printf("Metal principal: %s\n",produto.main_metal);
            printf("Gema principal: %s\n",produto.main_gem);
        }
        if (produto.prox != -1)
            fseek(produtos, produto.prox, SEEK_SET);
    }
    printf("------------------------------------\n");
    printf("Total de produtos: %d\n",i);
    printf("------------------------------------\n");
    fclose(produtos);
}

void pedidos_por_usuario()
{
    unsigned long long id;
    printf("Digite o ID do usuario: ");
    scanf("%llu",&id);
    FILE *pedidos = abrir(PATH_DADOS_ORDER,"rb");
    if(!pedidos) return;
    ORDER pedido;
    int i=0;
    unsigned long long ant = 0;
    while(fread(&pedido, sizeof(ORDER), 1, pedidos))
        if (pedido.ant != -1)
        {
            fseek(pedidos, pedido.ant, SEEK_SET);
        }
        else
        {
            fseek(pedidos, -sizeof(ORDER), SEEK_CUR);
            break;
        }
    while(fread(&pedido, sizeof(ORDER), 1, pedidos) && ant < pedido.order_id)
    {
        if(pedido.user_id == id)
        {
            printf("------------------------------------\n");
            printf("Pedido %d\n",i++);
            printf("ID: %llu\n",pedido.order_id);
            printf("Data: %02d/%02d/%d %02d:%02d:%02d\n",pedido.date_time.day,pedido.date_time.month,pedido.date_time.year,pedido.date_time.hour,pedido.date_time.min,pedido.date_time.sec);
            printf("User ID: %llu\n",pedido.user_id);
            printf("Quantidade de produtos: %d\n",pedido.products_amount);
            for(int i=0; i<pedido.products_amount; i++)
            {
                printf("\tProduto[%d]: %llu\n",i+1,pedido.products_id[i]);
                printf("\tSKU[%d]: %d\n",i+1,pedido.SKU_in_order[i]);
            }
        }
        if (pedido.prox != -1)
            fseek(pedidos, pedido.prox, SEEK_SET);
    }
    printf("------------------------------------\n");
    printf("Total de pedidos: %d\n",i);
    printf("------------------------------------\n");
    fclose(pedidos);
}

void pedidos_por_data()
{
    DATE_TIME data;
    printf("Digite a data do pedido: (dd/mm/aaaa hh:mm:ss) (preencha com 9 para nao filtrar)\n");
    scanf("%d/%d/%d %d:%d:%d",&data.day,&data.month,&data.year,&data.hour,&data.min,&data.sec);
    FILE *pedidos = abrir(PATH_DADOS_ORDER,"rb");
    if(!pedidos) return;
    ORDER pedido;
    int i=0;
    unsigned long long ant = 0;
    while(fread(&pedido, sizeof(ORDER), 1, pedidos))
        if (pedido.ant != -1)
        {
            fseek(pedidos, pedido.ant, SEEK_SET);
        }
        else
        {
            fseek(pedidos, -sizeof(ORDER), SEEK_CUR);
            break;
        }

    while(fread(&pedido, sizeof(ORDER), 1, pedidos) && ant < pedido.order_id)
    {
        if((data.year == 9999 || pedido.date_time.year == data.year) && (data.month == 99 || pedido.date_time.month == data.month) && 
           (data.day == 99 || pedido.date_time.day == data.day)    && (data.hour == 99 || pedido.date_time.hour == data.hour) && 
           (data.min == 99 || pedido.date_time.min == data.min)    && (data.sec == 99 || pedido.date_time.sec == data.sec))
        {
            printf("------------------------------------\n");
            printf("Pedido %d\n",i++);
            printf("ID: %llu\n",pedido.order_id);
            printf("Data: %02d/%02d/%d %02d:%02d:%02d\n",pedido.date_time.day,pedido.date_time.month,pedido.date_time.year,pedido.date_time.hour,pedido.date_time.min,pedido.date_time.sec);
            printf("User ID: %llu\n",pedido.user_id);
            printf("Quantidade de produtos: %d\n\t",pedido.products_amount);
            for(int i=0; i<pedido.products_amount; i++)
                printf("[%llu[%d]] ",pedido.products_id[i],pedido.SKU_in_order[i]);
            printf("\n");
        }
        if (pedido.prox != -1)
            fseek(pedidos, pedido.prox, SEEK_SET);
    }
    printf("------------------------------------\n");
    printf("Total de pedidos: %d\n",i);
    printf("------------------------------------\n");
    fclose(pedidos);
}
