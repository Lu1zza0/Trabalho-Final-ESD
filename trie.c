/*
->trie.c
Implementação de uma Trie para manipulação e busca eficiente de textos (como estados e culturas) 
em amostras agrícolas.
Este arquivo contém as funções para criar, inserir, buscar, listar e liberar palavras na Trie, 
além de funções para busca por prefixo, carregamento de dados a partir de arquivo, e coleta de 
palavras para filtros.
Este arquivo não possui benchmarks devido a estrutura peculiar da trie.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <windows.h>
#include "trie.h"

/*
Retorna o índice correspondente ao caractere para uso na Trie.
Parâmetro: c - caractere a ser convertido
Retorno: índice inteiro no vetor de filhos da Trie
*/
int indice_trie(char c){
    if(c == ' '){
        return 26;
    }
    if(c >= 'a' && c <= 'z'){
        return c - 'a';
    }
    return -1;
}

/*
Cria e inicializa um novo nó da Trie para o caractere fornecido.
Parâmetro: letra - caractere a ser armazenado no nó
Retorno: ponteiro para o novo nó criado
*/
NoTrie* criar_no_trie(char letra){
    NoTrie *no = (NoTrie*)calloc(1, sizeof(NoTrie));
    no->letra = letra;
    no->eh_folha = 0;

    int i = 0;
    for(i; i < TAM_ALFABETO; i++){
        no->filhos[i] = NULL;
    }

    return no;
}

/*
Cria e inicializa uma nova Trie vazia.
Retorno: ponteiro para a Trie criada
*/
Trie* criar_trie(){
    Trie *trie = (Trie*)malloc(sizeof(Trie));
    trie->raiz = criar_no_trie('\0');
    return trie;
}

/*
Libera recursivamente toda a memória alocada para um nó da Trie e seus filhos.
Parâmetro: no - ponteiro para o nó a ser liberado
*/
void liberar_no_trie(NoTrie *no){
    if(no == NULL){
        return;
    }

    int i = 0;
    for(i; i < TAM_ALFABETO; i++){
        liberar_no_trie(no->filhos[i]);
    }
    free(no);
}

/*
Libera toda a memória alocada para a Trie.
Parâmetro: trie - ponteiro para a Trie a ser liberada
*/
void liberar_trie(Trie *trie){
    liberar_no_trie(trie->raiz);
    free(trie);
}

/*
Insere uma palavra na Trie.
Parâmetros:
    trie - ponteiro para a Trie
    palavra - string a ser inserida
*/
void inserir_trie(Trie *trie, const char *palavra){
    NoTrie *atual = trie->raiz;
    int i = 0;
    int len = strlen(palavra);

    for(i; i < len; i++){
        char c = palavra[i];
        if(c >= 'A' && c <= 'Z'){
            c = c - 'A' + 'a';
        }

        int idx = indice_trie(c);

        if(idx == -1){
            continue;
        }

        if(atual->filhos[idx] == NULL){
            atual->filhos[idx] = criar_no_trie(c);
        }

        atual = atual->filhos[idx];
    }

    atual->eh_folha = 1;
}

/*
Busca uma palavra completa na Trie.
Parâmetros:
    trie - ponteiro para a Trie
    palavra - string a ser buscada
Retorno: 1 se a palavra existe, 0 caso contrário
*/
int buscar_trie(Trie *trie, const char *palavra){
    NoTrie *atual = trie->raiz;
    int i = 0;
    int len = strlen(palavra);
    
    for(i; i < len; i++){
        char c = palavra[i];
        if(c >= 'A' && c <= 'Z'){
            c = c - 'A' + 'a';
        }
        int idx = indice_trie(c);
        if(idx == -1){
            continue;
        }
        if(atual->filhos[idx] == NULL){
            return 0;
        }
        atual = atual->filhos[idx];
    }

    int resultado = (atual != NULL) && (atual->eh_folha);
    return resultado;
}

/*
Lista todas as palavras armazenadas a partir do nó atual da Trie.
Parâmetros:
    no - ponteiro para o nó atual
    prefixo - prefixo já montado até o nível atual
    nivel - profundidade atual na Trie
*/
void listar_palavras_trie(NoTrie *no, char *prefixo, int nivel){
    if(no == NULL){
        return;
    }

    if(no->eh_folha){
        prefixo[nivel] = '\0';
        printf("%s\n", prefixo);
    }

    int i = 0;
    for(i; i < TAM_ALFABETO; i++){
        if(no->filhos[i] != NULL){
            prefixo[nivel] = no->filhos[i]->letra;
            listar_palavras_trie(no->filhos[i], prefixo, nivel + 1);
        }
    }

}

/*
Busca e imprime todas as palavras que começam com o prefixo fornecido.
Parâmetros:
    trie - ponteiro para a Trie
    prefixo - string com o prefixo a ser buscado
*/
void buscar_prefixo_trie(Trie *trie, const char *prefixo){
    NoTrie *atual = trie->raiz;
    int len = strlen(prefixo);
    int i = 0;

    for(i; i < len; i++){
        char c = prefixo[i];
        if(c >= 'A' && c <= 'Z'){
            c = c - 'A' + 'a';
        }
        int idx = indice_trie(c);
        if(idx == -1){
            continue;
        }
        if(atual->filhos[idx] == NULL){
            printf("Nenhum resultado encontrado.\n");
            return;
        }
        atual = atual->filhos[idx];
    }


    char buffer[100];
    strcpy(buffer, prefixo);
    listar_palavras_trie(atual, buffer, strlen(prefixo));
}

/*
Carrega dados de um arquivo CSV e insere as palavras da coluna especificada na Trie.
Parâmetros:
    trie - ponteiro para a Trie
    nome_arquivo - nome do arquivo de entrada (dataset)
    coluna - índice da coluna a ser lida (ex: 2 para estado, 3 para cultura)
*/
void carregar_dados_trie(Trie *trie, const char *nome_arquivo, int coluna){
    FILE *arquivo = fopen(nome_arquivo, "r");
    if(!arquivo){
        printf("Erro ao abrir o arquivo\n");
        exit(EXIT_FAILURE);
    }

    char linha[256];
    fgets(linha, sizeof(linha), arquivo);

    while(fgets(linha, sizeof(linha), arquivo)){
        char *campos[10];
        char *token = strtok(linha, ";");
        int idx = 0;

        while(token != NULL && idx < 10){
            campos[idx++] = token;
            token = strtok(NULL, ";");
        }

        if(coluna < idx){
            char palavra[100];
            strncpy(palavra, campos[coluna], sizeof(palavra));
            palavra[sizeof(palavra)-1] = '\0';
            int j = 0;
            for(j; palavra[j]; j++){
                if(palavra[j] >= 'A' && palavra[j] <= 'Z'){
                    palavra[j] = palavra[j] - 'A' + 'a';
                }
            }

            inserir_trie(trie, palavra);
        }
    }

    fclose(arquivo);
}

/*
Coleta todas as palavras armazenadas a partir do nó atual da Trie e as armazena em uma lista.
Parâmetros:
    no - ponteiro para o nó atual
    prefixo - prefixo já montado até o nível atual
    nivel - profundidade atual na Trie
    lista - matriz de strings para armazenar as palavras encontradas
    total - ponteiro para o contador de palavras encontradas
*/ 
void coletar_palavras_trie(NoTrie *no, char *prefixo, int nivel, char lista[][50], int *total){
    if(no == NULL){
        return;
    }

    if(no->eh_folha){
        prefixo[nivel] = '\0';
        strcpy(lista[*total], prefixo);
        (*total)++;
    }
    
    int i = 0;
    for(i; i < TAM_ALFABETO; i++){
        if(no->filhos[i] != NULL){
            prefixo[nivel] = no->filhos[i]->letra;
            coletar_palavras_trie(no->filhos[i], prefixo, nivel + 1, lista, total);
        }
    }
}
