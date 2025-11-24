#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "structs.c"

void limpar_tela(const char *msg)
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    if(!strcmp("",msg))
        printf("%s",msg);
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
void bin2txt(FILE *f, const char *nome)
{
    FILE *f_txt = abrir(nome,"w");
    int i=0;
    while(fread(&i, sizeof(int), 1, f))
    {
        fprintf(f_txt,"%x\n",i);
    }
    fclose(f_txt);
}
void criptografar(FILE *f){
    unsigned char char_chave;
    unsigned char char_texto;
    int posChave = 0;
    int tamanho = sizeof(unsigned char);
    int tamanho_chave = strlen(CHAVE_CRIPTOGRAFIA);
    unsigned char deslocado;
    while(fread(&char_texto, tamanho, 1, f))
    {
        if (posChave % 10000 == 0)
            printf("\rPos: %d",posChave);
        char_chave = CHAVE_CRIPTOGRAFIA[posChave % tamanho_chave];

        deslocado = (char_texto + char_chave) % 256;
        
        char_texto = deslocado ^ char_chave;

        fseek(f, -tamanho, SEEK_CUR);
        fwrite(&char_texto, tamanho, 1, f);
        fseek(f, 0, SEEK_CUR);

        posChave++;
    }
}

FILE * descriptografar(const char * arquivo){
    FILE *base = abrir(arquivo,"rb");
    FILE *f = tmpfile();
    unsigned char char_chave;
    unsigned char char_texto;
    int posChave = 0;
    int tamanho = sizeof(unsigned char);
    int tamanho_chave = strlen(CHAVE_CRIPTOGRAFIA);
    unsigned char deslocado;
    while(fread(&char_texto, tamanho, 1, base))
    {
        if (posChave % 10000 == 0)
            printf("\rPos: %d",posChave);
        char_chave = CHAVE_CRIPTOGRAFIA[posChave % tamanho_chave];

        char_texto = char_texto ^ char_chave;

        int M = 256;
        char_texto = ((char_texto - char_chave) % M + M) % M;

        fwrite(&char_texto, tamanho, 1, f);

        posChave++;
    }
    fclose(base);
    rewind(f);
    return f;
}

void atualizar_arquivo(FILE *novo, const char *nome)
{
    FILE *arquivo = abrir(nome,"rb+");
    if(!arquivo) return;
    fseek(novo, 0, SEEK_END);
    long tam = ftell(novo);
    fseek(novo, 0, SEEK_SET);
    char * buffer = (char*) malloc(tam);
    fread(buffer, 1, tam, novo);
    fwrite(buffer, 1, tam, arquivo);
    free(buffer);
    fclose(novo);
    criptografar(arquivo);
    fclose(arquivo);
}

int busca_binaria_vet(PRODUCT vetor[], int tamanho, unsigned long long chave) {
    int inicio = 0;
    int fim = tamanho - 1;
    int meio;

    while (inicio <= fim) {
        meio = (inicio + fim) / 2;

        if (vetor[meio].product_id == chave) {
            return meio;
        } else if (vetor[meio].product_id < chave) {
            inicio = meio + 1;
        } else { 
            fim = meio - 1;
        }
    }

    return -1;
}

int existe_arquivo(const char *nome, const char *modo)
{
    FILE *f = fopen(nome,modo);
    if(!f) return 0;
    fclose(f);
    return 1;
}

int existe_pedido(ORDER *pedidos, unsigned long long id_pedido, int tam)
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

int checar_config(int indice)
{
    FILE *f = abrir(PATH_CONFIG,"rb");
    fseek(f, indice * sizeof(int),SEEK_SET);
    int num;
    fread (&num,sizeof(int),1,f);
    fclose(f);
    return num;
}

void alterar_config(int indice, int novo_valor)
{
    FILE *f = abrir(PATH_CONFIG,"r+b");
    fseek(f, indice * sizeof(int),SEEK_SET);
    fwrite (&novo_valor,sizeof(int),1,f);
    fclose(f);
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

void debug_txt()
{
    FILE *produtos_bin = abrir(PATH_DADOS_PROD, "rb");
    FILE *produtos_txt = abrir(DIR_TXT "produtos_completos.txt", "w"); 
    PRODUCT prod;

    if (!produtos_bin || !produtos_txt) 
    {
        fclose(produtos_bin);
        fclose(produtos_txt);
        return;
    }
    
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
            prod.prox == -1? -1 : (prod.prox / sizeof(PRODUCT) + 2),
            prod.ant == -1? -1 : (prod.ant / sizeof(PRODUCT) + 2)
        );
    }
    
    fclose(produtos_bin);
    fclose(produtos_txt);

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    produtos_bin = abrir(PATH_DADOS_PROD, "rb");
    produtos_txt = abrir(DIR_TXT "produtos_completos_chars.txt", "w"); 
    unsigned char letra;

    if (!produtos_bin || !produtos_txt) 
    {
        fclose(produtos_bin);
        fclose(produtos_txt);
        return;
    }
    
    while (fread(&letra, sizeof(unsigned char), 1, produtos_bin))
    {
        fprintf(produtos_txt, 
            "%d",
            letra
        );
    }
    
    fclose(produtos_bin);
    fclose(produtos_txt);

    produtos_bin = descriptografar(PATH_DADOS_PROD);
    produtos_txt = abrir(DIR_TXT "produtos_completos_decripto.txt", "w"); 

    if (!produtos_bin || !produtos_txt) 
    {
        fclose(produtos_bin);
        fclose(produtos_txt);
        return;
    }
    
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
            prod.prox == -1? -1 : (prod.prox / sizeof(PRODUCT) + 2),
            prod.ant == -1? -1 : (prod.ant / sizeof(PRODUCT) + 2)
        );
    }
    
    fclose(produtos_bin);
    fclose(produtos_txt);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    FILE *pedidos_bin = abrir(PATH_DADOS_ORDER, "rb");
    FILE *pedidos_txt = abrir(DIR_TXT "pedidos_completos.txt", "w"); 
    ORDER ord;

    if (!pedidos_bin || !pedidos_txt) 
    {
        fclose(pedidos_bin);
        fclose(pedidos_txt);
        return;
    }

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
            ord.prox == -1? -1 : (ord.prox / sizeof(ORDER) + 2),
            ord.ant == -1? -1 : (ord.ant / sizeof(ORDER) + 2)
        );
    }

    fclose(pedidos_bin);
    fclose(pedidos_txt);

    FILE *ind_prod_bin = abrir(PATH_INDEX_PROD, "rb");
    FILE *ind_prod_txt = abrir(DIR_TXT"indice_prod.txt", "w");
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
    else
    {
        fclose(ind_prod_bin);
        fclose(ind_prod_txt);
    }

    FILE *ind_order_bin = abrir(PATH_INDEX_ORDER, "rb");
    FILE *ind_order_txt = abrir(DIR_TXT"indice_order.txt", "w");
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
    else{
        fclose(ind_order_bin);
        fclose(ind_order_txt);
    }

    FILE *conf_bin = abrir(PATH_CONFIG, "rb");
    FILE *conf_txt = abrir(DIR_TXT "config_completo.txt", "w");
    if (!conf_bin || !conf_txt) 
    {
        fclose(conf_bin);
        fclose(conf_txt);
        return;
    }

    int v[8];
    fread(v, sizeof(int), 8, conf_bin);

    fprintf(conf_txt, "CONFIGURAÇÃO DO SISTEMA\n\n");
    fprintf(conf_txt, "0 - MAX_INS_PROD: %d (Número máximo de inserções antes de reordenar)\n", v[0]);
    fprintf(conf_txt, "1 - MAX_REM_PROD: %d (Número máximo de remoções antes de reordenar)\n\n", v[1]);

    fprintf(conf_txt, "2 - CONT_INS_PROD: %d (Contagem atual de inserções de produtos)\n", v[2]);
    fprintf(conf_txt, "4 - CONT_REM_PROD: %d (Contagem atual de remoções de produtos)\n", v[4]);
    fprintf(conf_txt, "6 - FLAG_ORDEM_PROD: %d (Flag indicando se produtos estão ordenados)\n\n", v[6]);

    fprintf(conf_txt, "3 - CONT_INS_PED: %d (Contagem atual de inserções de pedidos)\n", v[3]);
    fprintf(conf_txt, "5 - CONT_REM_PED: %d (Contagem atual de remoções de pedidos)\n", v[5]);
    fprintf(conf_txt, "7 - FLAG_ORDEM_PED: %d (Flag indicando se pedidos estão ordenados)\n", v[7]);

    fclose(conf_bin);
    fclose(conf_txt);
}

int comparar_strings(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        char c1 = tolower((unsigned char)*s1);
        char c2 = tolower((unsigned char)*s2);
        if (c1 != c2)
            return (unsigned char)c1 - (unsigned char)c2;
        s1++;
        s2++;
    }
    return (unsigned char)tolower(*s1) - (unsigned char)tolower(*s2);
}
