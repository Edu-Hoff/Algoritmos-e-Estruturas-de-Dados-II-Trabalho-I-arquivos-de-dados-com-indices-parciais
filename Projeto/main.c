#include <stdio.h>
#include "servicos.c"

int main ()
{
    //menu e chamada de servicoes
    //add teste se ja existe os arquivos

    //Opcao para o menu
        recriar_arquivos_base();
    //Opcao para o menu (da para mudar o nome)
        debug_txt();
        
    printf("\nFINALIZADO");
    getchar();
    getchar();
    return 0;
}
