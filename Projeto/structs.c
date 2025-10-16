#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"

typedef struct date_time {
    int year;
    int month;
    int day;
    int hour;
    int min;
    int sec;
} DATE_TIME;

typedef struct order {
    unsigned long long order_id;
    DATE_TIME date_time;
    unsigned long long products_id[AMOUNT_MAX];
    int SKU_in_order[AMOUNT_MAX];
    int products_amount;
    unsigned long long user_id;
    char exclusao;
    long prox;
    long ant;
} ORDER;

typedef struct product {
    unsigned long long product_id;
    unsigned long long category_id;
    char category_alias[SIZE_CATEGORY];
    int brand_id;
    float price;
    char product_gender;
    char main_color[SIZE_MAIN_CMG];
    char main_metal[SIZE_MAIN_CMG];
    char main_gem[SIZE_MAIN_CMG];
    char exclusao;
    long prox;
    long ant;
} PRODUCT;

typedef struct index
{
    unsigned long long chave;
    long endereco;
}INDEX;

DATE_TIME data_padrao() {
    DATE_TIME padrao = {0};
    padrao.year = 2000; 
    padrao.month  = 1;           
    padrao.day = 1;           
    return padrao; 
}
