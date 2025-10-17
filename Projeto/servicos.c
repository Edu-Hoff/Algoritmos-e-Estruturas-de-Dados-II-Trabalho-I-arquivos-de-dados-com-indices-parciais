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
    printf("Tem certeza que deseja reorganizar todos os arquivos? (Y/N)\n");
    scanf("%c", &x);
    if(x=='Y' || x=='y')
        reorganizar();
    else
        limpar_tela("Reorganizacao Cancelada\n");
}

//consulta com exibicoes 
//insercoes e remocoes 

//pesquisa por data??? (possivel ideia se der tempo)
