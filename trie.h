#ifndef TRIE_H
#define TRIE_H

#define TAM_ALFABETO 27

#include <stdio.h>

typedef struct NoTrie{
    char letra;
    struct NoTrie *filhos[TAM_ALFABETO];
    int eh_folha;
}NoTrie;

typedef struct{
    NoTrie *raiz;
}Trie;

NoTrie* criar_no_trie(char letra);
Trie* criar_trie();
void liberar_no_trie(NoTrie *no);
void liberar_trie(Trie *trie);

void inserir_trie(Trie *trie, const char *palavra);
int buscar_trie(Trie *trie, const char *palavra);
void listar_palavras_trie(NoTrie *no, char *prefixo, int nivel);
void buscar_prefixo_trie(Trie *trie, const char *prefixo);
void carregar_dados_trie(Trie *trie, const char *nome_arquivo, int coluna);
void coletar_palavras_trie(NoTrie *no, char *prefixo, int nivel, char lista[][50], int *total);

#endif
