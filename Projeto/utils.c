#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.c"

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
