#ifndef LISTA_ENCADEADA_H
#define LISTA_ENCADEADA_H

typedef struct ItemListaEncadeada{
    int id;
    int ano;
    char estado[50];
    char cultura[50];
    float preco_ton;
    float rendimento;
    float producao;
    float area_plantada;
    float valor_total;
    struct ItemListaEncadeada *prox;
} ItemListaEncadeada;

void inserir_LE(ItemListaEncadeada **cabeca, ItemListaEncadeada novo);
int criar_id_LE(ItemListaEncadeada *cabeca);
void criar_amostra_LE(ItemListaEncadeada **cabeca);
ItemListaEncadeada* buscar_LE(ItemListaEncadeada *cabeca, int id);
void buscar_filtros_LE(ItemListaEncadeada *cabeca, int ano_min, int ano_max, const char *estado, const char *cultura);
int remover_LE(ItemListaEncadeada **cabeca, int id);
void imprime_LE(ItemListaEncadeada *cabeca);
void libera_LE(ItemListaEncadeada *cabeca);
void carregar_dados_LE(ItemListaEncadeada **cabeca, const char *nome_arquivo);
void salvar_dados_LE(ItemListaEncadeada *cabeca, const char *nome_arquivo);

double bench_temp_insercao_LE(const char *nome_arquivo, int n);
double bench_temp_remocao_LE(const char *nome_arquivo, int n);
double bench_temp_busca_LE(const char *nome_arquivo, int n);
size_t bench_uso_memoria_LE(const char *nome_arquivo, int n);
double bench_insercao_mem_restrita_LE(const char *nome_arquivo, double lim_memoria_mb);
double bench_insercao_com_delay_LE(const char *nome_arquivo, int n, int delay_ms);
double bench_busca_latencia_LE(const char *nome_arquivo, int n);
double bench_temp_insercao_perda_LE(const char *nome_arquivo, int n);
double bench_busca_lim_acessos_LE(const char *nome_arquivo, int n, int lim);

#endif
