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
        printf("1 - Produtos Cadastrado\n");            //Feito todos_produtos                  - CORRETO
        printf("2 - Pedidos Cadastrados\n");            //Feito todos_pedidos                   - CORRETO
        printf("3 - Produto (Id - Binaria)\n");         //Feito busca_produto - BINARIA
        printf("4 - Pedido (Id - Binaria)\n");          //Feito busca_pedido - BINARIA
        printf("5 - Produtos (Categoria Alias)\n");     //Feito produtos_por_categoria          - CORRETO
        printf("6 - Pedidos (Data)\n");                 //Feito pedidos_por_data                - CORRETO
        printf("7 - Pedidos (Id Usuario)\n");           //Feito pedidos_por_usuario             - CORRETO
        printf("0 - Voltar\n"); 
        scanf("%d",&op);
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
                produtos_por_categoria();
                break;
            case 6:
                pedidos_por_data();
                break;
            case 7:
                pedidos_por_usuario();
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

        printf("1 - Inserir Produto\n");                //Feito - CORRETO
        printf("2 - Remover Produto\n");                //Feito - CORRETO
        printf("3 - Inserir Pedido\n");     
        printf("4 - Remover pedido\n");  
        printf("5 - Inserir Produto em um Pedido\n");     
        printf("6 - Remover Produto de um Pedido\n");     
        printf("0 - Voltar\n"); 
        scanf("%d",&op);
        switch (op)
        {
            case 1:
                adicionar_produto();
                break;
            case 2:
                remove_produto();
                break;
            case 3:
                adicionar_pedido();
                break;
            case 4:
                remove_pedido();
                break;
            case 5:
                adicionar_produto_pedido();
                break;
            case 6:
                remove_produto_pedido();
                break;
            case 0:
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

        printf("1 - Editar Limite Insercoes\n");        //Feito - CORRETO
        printf("2 - Editar Limite Remocoes\n");         //Feito - CORRETO
        printf("3 - Reordenar Produtos\n");             //Feito - CORRETO
        printf("4 - Reordenar Pedidos\n");              //Feito - CORRETO
        printf("5 - Reordenar Produtos e Pedidos\n");   //Feito - CORRETO
        printf("6 - Recriar Arquivos Bases\n");         //Feito - CORRETO
        printf("7 - Recriar Arquivos Txt\n");           //Feito - CORRETO
        printf("8 - Criar Novo CSV\n");
        printf("0 - Voltar\n"); 
        scanf("%d",&op);
        switch (op)
        {
            case 1:
                maximo_insercoes();
                break;
            case 2:
                maximo_remocoes();
                break;
            case 3:
                reordenar(0);
                break;
            case 4:
                reordenar(1);
                break;
            case 5:
                reordenar(2);
                break;
            case 6:
                criar_arquivos_base();
                break;
            case 7:
                debug_txt();
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
        printf("3 - Configurar Reordenacao\n");     //Feito
        printf("0 - Sair\n");
        scanf("%d",&op);
        switch (op)
        {
            case 1:
                exibir_menu();
                break;
            case 2:
                inserir_remover_menu();
                break;
            case 3:
                configurar_menu();
                break;
            case 0:
                break;
            default:
                opcao_invalida();
                break;
        }
    }while(op!=0);
}

void checar_arquivos()
{
    if(!existe(PATH_DADOS_ORDER,"rb") || !existe(PATH_DADOS_PROD,"rb") || !existe(PATH_INDEX_ORDER,"rb") 
    || !existe(PATH_INDEX_PROD,"rb")  || !existe(PATH_CONFIG,"rb")     || !existe(PATH_DADOS_ORIGEM,"w"))
        criar_arquivos_base();
}

int main ()
{
    checar_arquivos();
    menu();
    printf("\n--------------------------------------------------------------------------------\n");
    printf("PROGRAMA FINALIZADO");
    getchar(); getchar();
    return 0;
}
