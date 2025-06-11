#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <stdlib.h>

#define TAM 2011

struct ElementoHash{
    int id;
    int ano;
    char estado[10];
    char cultura[50];
    float preco_ton;
    float rendimento;
    float producao;
    float area_plantada;
    float valor_total;
    struct ElementoHash *prox;
};

typedef struct ElementoHash ItemHash;

typedef struct {
    ItemHash* tabela[TAM];
} TabelaHash;

void iniciar_hash(TabelaHash *th);
int funcao_hash(int id);
void inserir_tabela_hash(TabelaHash *tabela, ItemHash novo);
ItemHash* buscar_tabela_hash(TabelaHash *tabela, int id);
int remover_tabela_hash(TabelaHash *tabela, int id);
void liberar_tabela_hash(TabelaHash *tabela);
void imprimir_tabela_hash(TabelaHash *tabela);
void carregar_dados_hash(TabelaHash *tabela, const char *nome_arquivo);
void salvar_dados_hash(TabelaHash *tabela, const char *nome_arquivo);
void criar_amostra_hash(TabelaHash *tabela);
int proximo_id_hash(TabelaHash *tabela);
double bench_tempo_insercao_hash(const char *nome_arquivo, int n);
double bench_tempo_remocao_hash(const char *nome_arquivo, int n);
double bench_tempo_busca_hash(const char *nome_arquivo, int n);
size_t bench_uso_memoria_hash(const char *nome_arquivo, int n);
double bench_insercao_mem_restrita_hash(const char *nome_arquivo, double lim_memoria_mb);
double bench_insercao_com_delay_hash(const char *nome_arquivo, int n, int delay_ms);
double bench_busca_latencia_hash(const char *nome_arquivo, int n);
double bench_tempo_insercao_perda_hash(const char *nome_arquivo, int n);
double bench_busca_lim_acessos_hash(const char *nome_arquivo, int n, int lim);



#endif
