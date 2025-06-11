#ifndef SKIPLIST_H
#define SKIPLIST_H

#define  NIVEL_MAX_SKIPLIST 16

typedef struct ElementoSkiplist {
    int id;
    int ano;
    char estado[10];
    char cultura[50];
    float preco_ton;
    float rendimento;
    float producao;
    float area_plantada;
    float valor_total;
    struct ElementoSkiplist* proximo[NIVEL_MAX_SKIPLIST];
} ElementoSkiplist;

typedef struct {
    ElementoSkiplist* cabeca;
    int nivel;
} Skiplist;

Skiplist* iniciar_skiplist();
int nivel_aleatorio_skiplists();
void inserir_skiplist(Skiplist *lista, ElementoSkiplist novo_dado);
ElementoSkiplist* buscar_skiplist(Skiplist* lista, int id);
int remover_skiplist(Skiplist *lista, int id);
void imprime_skiplist(Skiplist* lista);
void libera_skiplist(Skiplist* lista);
void carregar_dados_skiplist(Skiplist* lista, const char* nome_arquivo);
void salvar_dados_skiplist(Skiplist* lista, const char* nome_arquivo);
int criar_id_skiplist(Skiplist* lista);
void criar_amostra_skiplist(Skiplist* lista);
void buscarFiltros_skiplist(Skiplist* lista, int ano_min, int ano_max, const char* estado, const char* cultura);
double bench_temp_insercao_skiplist(const char* novo_arquivo, int n);
double bench_temp_remocao_skiplist(const char* arquivo, int n);
double bench_temp_busca_skiplist(const char* arquivo, int n);
size_t bench_uso_memoria_skiplist(const char* nome_arquivo, int n);
double bench_insercao_mem_restrita_skiplist(const char* nome_arquivo, double lim_memoria_mb);
double bench_insercao_com_delay_skiplist(const char* nome_arquivo, int n, int delay_ms);
double bench_busca_latencia_skiplist(const char* nome_arquivo, int n);
double bench_temp_insercao_perda_skiplist(const char* nome_arquivo, int n);
double bench_busca_lim_acessos_skiplist(const char* nome_arquivo, int n, int lim);

#endif