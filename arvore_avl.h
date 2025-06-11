#ifndef ARVORE_AVL_H
#define ARVORE_AVL_H

#include <stdio.h>

typedef struct ItemAVL {
    int id;
    int ano;
    char estado[10];
    char cultura[50];
    float preco_ton;
    float rendimento;
    float producao;
    float area_plantada;
    float valor_total;
    struct ItemAVL *esq;
    struct ItemAVL *dir;
    int altura;
} ItemAVL;

int altura(ItemAVL *no);
int maximo(int a, int b);
ItemAVL* rotacao_direita(ItemAVL *y);
ItemAVL* rotacao_esquerda(ItemAVL *x);
int fator_balanceamento(ItemAVL *no);
ItemAVL* novo_no(ItemAVL novo);

ItemAVL* inserir_avl(ItemAVL *raiz, ItemAVL novo);
ItemAVL* min_valor_no(ItemAVL* no);
ItemAVL* remover_avl(ItemAVL *raiz, int id);
ItemAVL* buscar_avl(ItemAVL *raiz, int id);
void liberar_avl(ItemAVL *raiz);
void imprimir_avl(ItemAVL *raiz);
void carregar_dados_avl(ItemAVL **raiz, const char *nome_arquivo);
void salvar_aux_avl(ItemAVL *no, FILE *arquivo);
void salvar_dados_avl(ItemAVL *raiz, const char *nome_arquivo);
void busca_maior_id_avl(ItemAVL *no, int *maior);
void criar_amostra_avl(ItemAVL **raiz);
int proximo_id_avl(ItemAVL *raiz);
void buscar_filtros_aux_avl(ItemAVL *no, int ano_min, int ano_max, const char *estado, const char *cultura, int *encontrados);
void buscar_filtros_avl(ItemAVL *raiz, int ano_min, int ano_max, const char *estado, const char *cultura);

double bench_tempo_insercao_avl(const char *nome_arquivo, int n);
void coleta_ids(ItemAVL *no, int *ids, int n, int *coletados);
double bench_tempo_remocao_avl(const char *nome_arquivo, int n);
double bench_tempo_busca_avl(const char *nome_arquivo, int n);
size_t bench_uso_memoria_avl(const char *nome_arquivo, int n);
double bench_insercao_mem_restrita_avl(const char *nome_arquivo, double lim_memoria_mb);
double bench_insercao_com_delay_avl(const char *nome_arquivo, int n, int delay_ms);
void coleta_ids(ItemAVL *no, int *ids, int n, int *coletados);
double bench_busca_latencia_avl(const char *nome_arquivo, int n);
double bench_tempo_insercao_perda_avl(const char *nome_arquivo, int n);
double bench_busca_lim_acessos_avl(const char *nome_arquivo, int n, int lim);

#endif
