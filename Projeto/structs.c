#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#define timegm _mkgmtime
#endif

typedef struct order {
    time_t date_time;
    unsigned long long order_id;
    unsigned long long products_id[100];
    int SKU_in_order[100];
    int products_amount;
    unsigned long long user_id;
} ORDER;

typedef struct product {
    unsigned long long product_id;
    unsigned long long category_id;
    char category_alias[50];
    int brand_id;
    float price;
    char product_gender;
    char main_color[20];
    char main_metal[20];
    char main_gem[20];
} PRODUCT;

struct tm data_padrao() {
    struct tm tm_padrao = {0};
    tm_padrao.tm_year = 2000 - 1900; 
    tm_padrao.tm_mon  = 0;           
    tm_padrao.tm_mday = 1;           
    tm_padrao.tm_hour = 0;
    tm_padrao.tm_min  = 0;
    tm_padrao.tm_sec  = 0;
    return tm_padrao; 
}

void desconstruir_linha(char string[], int *sku, ORDER *order, PRODUCT *product)
{
    char *token;
    char *saveptr;

    token = strtok_r(string, ",", &saveptr);
    struct tm tm_data = {0};

    if (token && token[0] != '\0' && strptime(token, "%Y-%m-%d %H:%M:%S", &tm_data) != NULL) 
        order->date_time = timegm(&tm_data);
    else {
        struct tm tm_padrao = data_padrao();
        order->date_time = timegm(&tm_padrao);
    }
    

    token = strtok_r(NULL, ",", &saveptr);
    order->order_id = strtoull(token, NULL, 10);

    token = strtok_r(NULL, ",", &saveptr);
    product->product_id = strtoull(token, NULL, 10);

    token = strtok_r(NULL, ",", &saveptr);
    *sku = atoi(token);

    token = strtok_r(NULL, ",", &saveptr);
    product->category_id = token && token[0] != '\0' ? strtoull(token, NULL, 10) : 0;

    token = strtok_r(NULL, ",", &saveptr);
    strcpy(product->category_alias, token && token[0] != '\0' ? token : "other");

    token = strtok_r(NULL, ",", &saveptr);
    product->brand_id = token && token[0] != '\0' ? atoi(token) : 0;

    token = strtok_r(NULL, ",", &saveptr);
    product->price = token && token[0] != '\0' ? atof(token) : 0.0f;

    token = strtok_r(NULL, ",", &saveptr);
    order->user_id = token && token[0] != '\0' ? strtoull(token, NULL, 10) : 0;

    token = strtok_r(NULL, ",", &saveptr);
    product->product_gender = token && token[0] != '\0' ? token[0] : 'x';

    token = strtok_r(NULL, ",", &saveptr);
    strcpy(product->main_color, token && token[0] != '\0' ? token : "none");

    token = strtok_r(NULL, ",", &saveptr);
    strcpy(product->main_metal, token && token[0] != '\0' ? token : "none");

    token = strtok_r(NULL, ",", &saveptr);
    strcpy(product->main_gem, token && token[0] != '\0' ? token : "none");
}
