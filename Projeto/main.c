#include <stdio.h>
#include "servicos.c"

void opcao_invalida()
{
    limpar_tela("Opcao Invalida\n");
}

void exibir_menu()
{
    int op=0;
    do
    {
        printf("==================================\n");
        printf("=====         Exbir          =====\n");
        printf("==================================\n\n");
        //Apenas ideia por enquanto
        printf("1 - Produtos Cadastrado\n");   //Feito todos_produtos
        printf("2 - Pedidos Cadastrados\n");   //Feito todos_pedidos
        printf("3 - Produto (Id - Binaria)\n"); //Feito busca_produto - BINARIA
        printf("4 - Pedido (Id - Binaria)\n"); //Feito busca_pedido - BINARIA
        printf("5 - Produtos (Categoria Alias)\n"); 
        printf("6 - Pedidos (Data)\n");
        printf("7 - Pedidos (Id Usuario)\n");
        printf("0 - Voltar\n"); 
        switch (op)
        {
            case 1:
                todos_produtos();
                break;
            case 2:
                todos_pedidos();
                break;
            case 3:
                busca_produto();
                break;
            case 4:
                busca_pedido();
                break;
            case 5:
                /* code */
                break;
            case 6:
                /* code */
                break;
            case 7:
                /* code */
                break;
            default:
                opcao_invalida();
                break;
        }
    } while (op != 0);   
}

void inserir_remover_menu()
{
        int op=0;
    do
    {
        printf("==================================\n");
        printf("=====    Inserir/Remover     =====\n");
        printf("==================================\n\n");

        printf("1 - Inserir Produto\n");     
        printf("2 - Remover Produto\n");   
        printf("3 - Inserir Pedido\n");     
        printf("4 - Remover pedido\n");  
        printf("5 - Inserir Produto em um Pedido\n");     
        printf("6 - Remover Produto de um Pedido\n");     
        printf("0 - Voltar\n"); 
        switch (op)
        {
            case 1:
                /* code */
                break;
            case 2:
                /* code */
                break;
            case 3:
                /* code */
                break;
            case 4:
                /* code */
                break;
            case 5:
                /* code */
                break;
            case 6:
                /* code */
                break;
            case 0:
                /* code */
                break;
            default:
                opcao_invalida();
                break;
        }
    } while (op != 0);   
}

void configurar_menu()
{
    int op=0;
    do
    {
        printf("==================================\n");
        printf("========    Configurar    ========\n");
        printf("==================================\n\n");

        printf("1 - Editar Limite Insercoes\n");     
        printf("2 - Editar Limite Remocoes\n");   
        printf("0 - Voltar\n"); 
        switch (op)
        {
            case 1:
                maximo_insercoes();
                break;
            case 2:
                maximo_remocoes();
                break;
            case 0:
                break;
            default:
                opcao_invalida();
                break;
        }
    } while (op != 0);   
}

void menu()
{
    int op=0;
    do{
        printf("==================================\n");
        printf("===== Gerenciador de Pedidos =====\n");
        printf("==================================\n\n");

        printf("1 - Exibir Dados\n");     
        printf("2 - Inserir/Remover Dados\n");   
        printf("3 - Configurar Reordenacao\n"); 
    }while(op!=0);
}

int main ()
{

    //Salva saida em um txt?
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
