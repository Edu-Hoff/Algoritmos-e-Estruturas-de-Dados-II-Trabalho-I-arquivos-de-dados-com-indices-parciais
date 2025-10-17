#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.c"

void limpar_tela(char *msg)
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    if(!strcmp("",msg))
        printf("%s",msg);
}

int existe(ORDER *pedidos, unsigned long long id_pedido, int tam)
{
    for(int i=0;i<tam;i++)
        if(pedidos[i].order_id == id_pedido)
            return i;
    return -1;
}

int existe_produto(PRODUCT *produtos, unsigned long long id_produto, int tam)
{
    for(int i=0;i<tam;i++)
        if(produtos[i].product_id == id_produto)
            return 1;
    return 0;
}

void copiar_e_preencher(char *destino, const char *origem, size_t tamanho)
{
    size_t len_origem = strlen(origem);
    size_t len_a_copiar = (len_origem < tamanho) ? len_origem : tamanho;

    memcpy(destino, origem, len_a_copiar);

    if (len_a_copiar < tamanho) {
        memset(destino + len_a_copiar, ' ', tamanho - len_a_copiar);
    }
    destino[tamanho-1]='\0';
}

char* tokenizacao(char *string, char separador, char **save)
{
    if (!string && !*save) return NULL; 
    
    char *start = string ? string : *save;
    if (!start || *start == '\0') {
        *save = NULL;
        return NULL;
    }

    static char temp[200];
    int i = 0;

    while (*start != '\0' && *start != separador && i < 199) {
        temp[i++] = *start++;
    }

    temp[i] = '\0';

    if (*start == separador) 
        *save = start + 1; 
    else 
        *save = start;

    return temp;
}

void desconstruir_linha(char string[], int *sku, ORDER *order, PRODUCT *product)
{
    char *token;
    char *saveptr;

    order->prox = order->ant = product->prox = product->ant = -1;
    order->exclusao = product->exclusao = order->products_amount = 0;
    
    token = tokenizacao(string, ',', &saveptr);
    DATE_TIME data = {0};

    if (token && token[0] != '\0' && sscanf(token, "%d-%d-%d %d:%d:%d", &data.year, &data.month, &data.day, &data.hour, &data.min, &data.sec) == 6) 
        order->date_time = data;
    else 
        order->date_time = data_padrao();
    
    memset(order->products_id, 0, sizeof(order->products_id));
    memset(order->SKU_in_order, 0, sizeof(order->SKU_in_order));

    token = tokenizacao(NULL, ',', &saveptr);
    order->order_id = token ? strtoull(token, NULL, 10) : 999999999;

    token = tokenizacao(NULL, ',', &saveptr);
    product->product_id = token ? strtoull(token, NULL, 10) : 999999999;

    token = tokenizacao(NULL, ',', &saveptr);
    *sku = token ? atoi(token) : 999999999;

    token = tokenizacao(NULL, ',', &saveptr);
    product->category_id = token && token[0] != '\0' ? strtoull(token, NULL, 10) : 0;

    token = tokenizacao(NULL, ',', &saveptr);
    copiar_e_preencher(product->category_alias, (token && token[0] != '\0' ? token : "other"), SIZE_CATEGORY);

    token = tokenizacao(NULL, ',', &saveptr);
    product->brand_id = token && token[0] != '\0' ? atoi(token) : 0;

    token = tokenizacao(NULL, ',', &saveptr);
    product->price = token && token[0] != '\0' ? atof(token) : 0.0f;

    token = tokenizacao(NULL, ',', &saveptr);
    order->user_id = token && token[0] != '\0' ? strtoull(token, NULL, 10) : 0;

    token = tokenizacao(NULL, ',', &saveptr);
    product->product_gender = token && token[0] != '\0' ? token[0] : 'x';

    token = tokenizacao(NULL, ',', &saveptr);
    copiar_e_preencher(product->main_color, (token && token[0] != '\0' ? token : "none"), SIZE_MAIN_CMG);

    token = tokenizacao(NULL, ',', &saveptr);
    copiar_e_preencher(product->main_metal, (token && token[0] != '\0' ? token : "none"), SIZE_MAIN_CMG);

    token = tokenizacao(NULL, ',', &saveptr);
    copiar_e_preencher(product->main_gem, (token && token[0] != '\0' ? token : "none"), SIZE_MAIN_CMG);
}


FILE *abrir(const char *nome, const char *modo)
{
    FILE *temp = fopen(nome,modo);
    if (!temp) {
        fprintf(stderr, "Erro abrindo '%s' com modo '%s': %s\n", nome, modo, strerror(errno));
        return NULL;
    }
    return temp;
}

void debug_txt()
{
    FILE *produtos_bin = abrir(PATH_DADOS_PROD, "rb");
    FILE *produtos_txt = fopen(DIR_TXT "produtos_completos.txt", "w"); 
    PRODUCT prod;

    if (!produtos_bin || !produtos_txt) 
        return;
    
    fprintf(produtos_txt, "PROD_ID - CAT_ID - CAT_ALIAS (SIZE) - BRAND_ID - PRICE - GENDER - MAIN_COLOR (SIZE) - MAIN_METAL (SIZE) - MAIN_GEM (SIZE) - EXC - PROX - ANT\n");
    while (fread(&prod, sizeof(PRODUCT), 1, produtos_bin))
    {
        fprintf(produtos_txt, 
            "%-20llu - %-20llu - %s - %04d - %07.2f - %c - %s - %s - %s - %1d - %ld - %ld\n",
            prod.product_id,
            prod.category_id,
            prod.category_alias, 
            prod.brand_id,
            prod.price,
            prod.product_gender,
            prod.main_color,     
            prod.main_metal,     
            prod.main_gem,       
            (int)prod.exclusao,  
            prod.prox,
            prod.ant
        );
    }
    
    fclose(produtos_bin);
    fclose(produtos_txt);

    FILE *pedidos_bin = abrir(PATH_DADOS_ORDER, "rb");
    FILE *pedidos_txt = fopen(DIR_TXT "pedidos_completos.txt", "w"); 
    ORDER ord;

    if (!pedidos_bin || !pedidos_txt) 
        return;

    fprintf(pedidos_txt, "ORDER_ID - DATA_UTC - USER_ID - QTD_PRODS - PRODUCTS_ID (SKU) - EXC - PROX - ANT\n");
    
    while (fread(&ord, sizeof(ORDER), 1, pedidos_bin))
    {
        fprintf(pedidos_txt, 
            "%-20llu - %04d/%02d/%02d %02d:%02d:%02d UTC - %-20llu - %03d - ",
            ord.order_id,
            ord.date_time.year,
            ord.date_time.month,
            ord.date_time.day,
            ord.date_time.hour,
            ord.date_time.min,
            ord.date_time.sec,
            ord.user_id,
            ord.products_amount 
        );

        for (int i = 0; i < ord.products_amount; i++) {
            fprintf(pedidos_txt, "[%20llu:%d]|", ord.products_id[i], ord.SKU_in_order[i]);
        }

        fprintf(pedidos_txt, 
            " - %1d - %ld - %ld\n",
            (int)ord.exclusao,
            ord.prox,
            ord.ant
        );
    }

    fclose(pedidos_bin);
    fclose(pedidos_txt);

    FILE *ind_prod_bin = abrir(PATH_INDEX_PROD, "rb");
    FILE *ind_prod_txt = fopen(DIR_TXT"indice_prod.txt", "w");
    INDEX ind_prod;

    if(ind_prod_bin && ind_prod_txt) {
        fprintf(ind_prod_txt, "Endereco - Chave (product_id)\n");
        while(fread(&ind_prod, sizeof(INDEX), 1, ind_prod_bin))
        {
            fprintf(ind_prod_txt, "%ld - %llu\n", ind_prod.endereco, ind_prod.chave);
        }
        fclose(ind_prod_bin);
        fclose(ind_prod_txt);
    }

    FILE *ind_order_bin = abrir(PATH_INDEX_ORDER, "rb");
    FILE *ind_order_txt = fopen(DIR_TXT"indice_order.txt", "w");
    INDEX ind_order;

    if(ind_order_bin && ind_order_txt) {
        fprintf(ind_order_txt, "Endereco - Chave (order_id)\n");
        while(fread(&ind_order, sizeof(INDEX), 1, ind_order_bin))
        {
            fprintf(ind_order_txt, "%ld - %llu\n", ind_order.endereco, ind_order.chave);
        }
        fclose(ind_order_bin);
        fclose(ind_order_txt);
    }
}
