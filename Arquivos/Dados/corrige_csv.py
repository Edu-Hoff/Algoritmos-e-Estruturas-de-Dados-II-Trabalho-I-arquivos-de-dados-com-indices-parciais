import csv

arquivo = r"D:\EduFaculdade\Semestre4\AlgoritmoseEstruturasdeDados2\Algoritmos-e-Estruturas-de-Dados-II-Trabalho-I-arquivos-de-dados-com-indices-parciais\Arquivos\Dados\dados.csv"
COLUNAS_ALVO = 13
linhas_fora_do_padrao = {}

print(f"Verificando o arquivo: {arquivo}")
print(f"Colunas alvo: {COLUNAS_ALVO}\n")

try:
    with open(arquivo, 'r', newline='', encoding='utf-8') as infile:
        reader = csv.reader(infile)
        
        for num_linha, row in enumerate(reader, 1):
            
            if not row and num_linha > 1:
                continue

            num_colunas = len(row)
            
            if num_colunas != COLUNAS_ALVO:
                if num_colunas not in linhas_fora_do_padrao:
                    linhas_fora_do_padrao[num_colunas] = []
                
                linhas_fora_do_padrao[num_colunas].append(num_linha)

    
    if not linhas_fora_do_padrao:
        print(f"✅ Todas as linhas do arquivo têm exatamente {COLUNAS_ALVO} colunas.")
    else:
        print(f"🚨 Linhas que NÃO têm {COLUNAS_ALVO} colunas:")
        linhas = linhas_fora_do_padrao[11]
            
        print(f"ha {len(linhas)} linhas com {11} colunas (linhas: {linhas})")

except FileNotFoundError:
    print(f"ERRO: Arquivo não encontrado no caminho: {arquivo}")
except Exception as e:
    print(f"Ocorreu um erro durante a leitura do arquivo: {e}")
    
    
def encontrar_indice_n_ocorrencia(texto, sub_string, n):
    """
    Encontra o índice (posição) da n-ésima ocorrência de uma substring
    (que pode ser um único caractere) em uma string.

    :param texto: A string onde a busca será feita.
    :param sub_string: O caractere ou a substring a ser encontrada.
    :param n: A ocorrência desejada (1 para a primeira, 4 para a quarta, etc.).
    :return: O índice da n-ésima ocorrência, ou -1 se não for encontrada.
    """
    if n <= 0:
        return -1  # Ocorrência deve ser 1 ou mais

    indice = -1  # Começa fora do índice da string
    
    # Faz um loop para encontrar 'n' vezes
    for _ in range(n):
        # O método find() retorna o índice da próxima ocorrência
        # A busca começa a partir de 'indice + 1' para não encontrar a mesma ocorrência
        indice = texto.find(sub_string, indice + 1)
        
        # Se find() retornar -1, significa que a substring não foi encontrada
        if indice == -1:
            return -1  # Retorna -1 se não houver a n-ésima ocorrência

    return indice
def inserir_no_meio(string_original, string_a_inserir, indice_insercao):
    """
    Insere uma string em uma posição específica de outra.

    :param string_original: A string base (onde será inserido).
    :param string_a_inserir: O texto a ser inserido.
    :param indice_insercao: O índice (posição) onde a inserção deve ocorrer (base 0).
    :return: A nova string resultante.
    """
    
    # Parte 1: Tudo ANTES do índice de inserção
    parte_antes = string_original[:indice_insercao]
    
    # Parte 2: Tudo DEPOIS do índice de inserção
    parte_depois = string_original[indice_insercao:]
    
    # Concatena: Parte Antes + String a Inserir + Parte Depois
    string_final = parte_antes + string_a_inserir + parte_depois
    return string_final
    
linhas_indices = linhas_fora_do_padrao[11]
with open(r"D:\EduFaculdade\Semestre4\AlgoritmoseEstruturasdeDados2\Algoritmos-e-Estruturas-de-Dados-II-Trabalho-I-arquivos-de-dados-com-indices-parciais\Arquivos\Dados\dados.csv", 'r', encoding='utf-8') as arquivo:
    linhas = arquivo.readlines()
    for indice in linhas_indices:
        linha = linhas[indice-1]
        ind = encontrar_indice_n_ocorrencia(linha,",",5)
        linha_modificada = inserir_no_meio(linha, ",,", ind)
        linhas[indice - 1] = linha_modificada
    with open(r"D:\EduFaculdade\Semestre4\AlgoritmoseEstruturasdeDados2\Algoritmos-e-Estruturas-de-Dados-II-Trabalho-I-arquivos-de-dados-com-indices-parciais\Arquivos\Dados\dados_corretos.csv", 'w', encoding='utf-8') as arquivo:
            arquivo.writelines(linhas)
            print("\nArquivo  reescrito com sucesso.")
    