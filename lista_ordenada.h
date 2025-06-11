#ifndef LISTA_ORDENADA_H
#define lista_ordenada_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <windows.h>

struct ElementoLista{
    int id;
    int ano;
    char estado[10];
    char cultura[50];
    float preco_ton;
    float rendimento;
    float producao;
    float area_plantada;
    float valor_total;
    struct ElementoLista *prox;
};

typedef struct ElementoLista ItemLista;

void insereOrdenadoID_LO(ItemLista **cabeca, ItemLista novo);
void imprime_LO(ItemLista *cabeca);
void libera_LO(ItemLista *cabeca);
void carregar_dados_LO(ItemLista **cabeca, const char *nome_arquivo);
void buscarFiltros_LO(ItemLista *cabeca, int ano_min, int ano_max, const char *estado, const char *cultura);
void criar_amostra_LO(ItemLista **cabeca);
void salvar_dados_LO(ItemLista *cabeca, const char *nome_arquivo);
int criar_id_LO(ItemLista *cabeca);
ItemLista *buscarId_LO(ItemLista *cabeca, int id);
void remover_LO(ItemLista **cabeca, int id);
double bench_temp_insercao_LO(const char *nome_arquivo, int n);
double bench_temp_remocao_LO(const char *nome_arquivo, int n);
double bench_temp_busca_LO(const char *nome_arquivo, int n);
size_t bench_uso_memoria_LO(const char *nome_arquivo, int n);
double bench_insercao_mem_restrita_LO(const char *nome_arquivo, double lim_memoria);
double bench_insercao_com_delay_LO(const char *nome_arquivo, int n, int delay_ms);
double bench_busca_latencia_LO(const char *nome_arquivo, int n);
double bench_temp_insercao_perda_LO(const char *nome_arquivo, int n);
double bench_busca_lim_acessos_LO(const char *nome_arquivo, int n, int lim);

#endif
