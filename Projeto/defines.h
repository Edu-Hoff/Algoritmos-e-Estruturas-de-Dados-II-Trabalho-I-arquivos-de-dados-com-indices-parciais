/*    Se alterados devem ser criados novos arquivos de dados apartir de um csv    */
/**/#define AMOUNT_MAX 50           //Maximo de produtos por pedido
/**/#define SIZE_CATEGORY 20        //Tamanho maximo da string do category_alias
/**/#define SIZE_MAIN_CMG 10        //Tamanho maximo da string da main color, metal, gem
/**/#define MAX_RECORDS 1000000     //Tamanho maximo de itens no arquivo (organizacao em memoria)
/**/
/**/#define DIR_CONFIG "../Arquivos/"               // Diretório do arquivo de configuracoes
/**/#define DIR_DADOS "../Arquivos/Dados/"          // Diretório dos arquivos de dados
/**/#define DIR_INDEX "../Arquivos/Indices/"        // Diretório dos arquivos de índices
/**/#define DIR_TXT "../Arquivos/Txt/"              // Diretório dos arquivos txt
/**/
/**/#define ARQ_CONFIG "config.bin"                 // Nome do arquivo de configuracoes
/**/
/**/#define ARQ_DADOS_ORIGEM "dados.csv"            // Nome do arquivo CSV onde estão os dados
/**/#define ARQ_DADOS_NOVO "novo_dados.csv"         // Nome do arquivo CSV gerado
/**/
/**/#define ARQ_DADOS_PROD   "products.bin"         // Nome do arquivo de dados dos produtos
/**/#define ARQ_DADOS_ORDER  "orders.bin"           // Nome do arquivo de dados dos pedidos
/**/
/**/#define ARQ_INDEX_PROD   "products_index.bin"   // Nome do arquivo de índice dos produtos
/**/#define ARQ_INDEX_ORDER  "orders_index.bin"     // Nome do arquivo de índice dos pedidos
/**/
/**/#define CHAVE_CRIPTOGRAFIA "chave_123"          // Chave para criptografar e descriptografar
/**/
/**//* ---- Caminhos completos ---- */
/**/#define PATH_DADOS_ORIGEM  DIR_DADOS ARQ_DADOS_ORIGEM
/**/#define PATH_DADOS_NOVO  DIR_DADOS ARQ_DADOS_NOVO
/**/#define PATH_DADOS_PROD    DIR_DADOS ARQ_DADOS_PROD
/**/#define PATH_DADOS_ORDER   DIR_DADOS ARQ_DADOS_ORDER
/**/#define PATH_INDEX_PROD    DIR_INDEX ARQ_INDEX_PROD
/**/#define PATH_INDEX_ORDER   DIR_INDEX ARQ_INDEX_ORDER
/**/#define PATH_CONFIG  DIR_CONFIG ARQ_CONFIG
/**/
/**/#define ARQ_INDEX_AMOUNT_PROD 500   //Quantidade de itens por grupo no indice dos produtos
/**/#define ARQ_INDEX_AMOUNT_ORDER 500  //Quantidade de itens por grupo no indice dos pedidos
/**********************************************************************************/

