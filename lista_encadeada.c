/*
->lista_encadeada.c
Implementação de uma lista encadeada para manipulação de amostras agrícolas.
Este arquivo contém as funções para inserir, remover, buscar, filtrar, imprimir 
e carregar dados em uma lista encadeada simples, além de funções auxiliares 
e do conjunto de nove funções para benchamark.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include "lista_encadeada.h"

/*
Insere um novo elemento no início da lista encadeada.
Parâmetros:
    cabeca - ponteiro para a cabeça da lista
    novo - estrutura com os dados a serem inseridos
*/
void inserir_LE(ItemListaEncadeada **cabeca, ItemListaEncadeada novo){
    ItemListaEncadeada *novo_item = malloc(sizeof(ItemListaEncadeada));

    *novo_item = novo;
    novo_item->prox = *cabeca;
    *cabeca = novo_item;
}

/* 
Função auxiliar para a criação de amostras. Retorna o próximo ID disponível na lista encadeada. 
Parâmetro: cabeca - ponteiro para a cabeça da lista
Retorno: inteiro representando o próximo ID
*/
int criar_id_LE(ItemListaEncadeada *cabeca){
    int maior_id = 0;
    ItemListaEncadeada *atual = cabeca;
    while(atual != NULL){
        if(atual->id > maior_id){
            maior_id = atual->id;
        }
        atual = atual->prox;
    }
    return maior_id + 1;
}

/*
Cria uma nova amostra a partir da entrada do usuário e insere na lista.
Parâmetro: cabeca - ponteiro para o ponteiro da cabeça da lista
*/
void criar_amostra_LE(ItemListaEncadeada **cabeca){
    ItemListaEncadeada novo;
    char entrada[50];

    novo.id = criar_id_LE(*cabeca);

    printf("\n===Insira os dados da nova amostra===\n");

    printf("Ano: ");
    fgets(entrada, sizeof(entrada), stdin);
    sscanf(entrada, "%d", &novo.ano);

    printf("Estado: ");
    fgets(novo.estado, sizeof(novo.estado), stdin);
    novo.estado[strcspn(novo.estado, "\n")] = '\0';

    printf("Cultura: ");
    fgets(novo.cultura, sizeof(novo.cultura), stdin);
    novo.cultura[strcspn(novo.cultura, "\n")] = '\0';

    printf("Preco por tonelada: ");
    fgets(entrada, sizeof(entrada), stdin);
    sscanf(entrada, "%f", &novo.preco_ton);

    printf("Rendimento: ");
    fgets(entrada, sizeof(entrada), stdin);
    sscanf(entrada, "%f", &novo.rendimento);

    printf("Producao: ");
    fgets(entrada, sizeof(entrada), stdin);
    sscanf(entrada, "%f", &novo.producao);

    printf("Area plantada: ");
    fgets(entrada, sizeof(entrada), stdin);
    sscanf(entrada, "%f", &novo.area_plantada);

    printf("Valor total: ");
    fgets(entrada, sizeof(entrada), stdin);
    sscanf(entrada, "%f", &novo.valor_total);

    novo.prox = NULL;

    inserir_LE(cabeca, novo);

    printf("\nAmostra inserida com sucesso\n");
}

/*
Busca um elemento pelo ID na lista encadeada.
Parâmetros:
  cabeca - ponteiro para a cabeça da lista
  id - identificador a ser buscado
Retorno: ponteiro para o item encontrado ou NULL
*/
ItemListaEncadeada* buscar_LE(ItemListaEncadeada *cabeca, int id){
    while(cabeca != NULL){
        if(cabeca->id == id){
            return cabeca;
        }
        cabeca = cabeca->prox;
    }

    return NULL;
}

/*
Busca e imprime elementos que atendem aos filtros de ano, estado e cultura.
Parâmetros:
  cabeca - ponteiro para a cabeça da lista
  ano_min, ano_max - intervalo de anos
  cultura - filtro de cultura (string)
  estado - filtro de estado (string)
*/
void buscar_filtros_LE(ItemListaEncadeada *cabeca, int ano_min, int ano_max, const char *estado, const char *cultura){
    ItemListaEncadeada *atual = cabeca;
    int encontrados = 0;

    printf("\n===RESULTADOS DA BUSCA===\n");

    while(atual != NULL){
        int ano_certo = (atual->ano >= ano_min) && (atual->ano <= ano_max);

        int estado_certo = (estado == NULL || strlen(estado) == 0 || strcasecmp(atual->estado, estado) == 0);

        int cultura_certa = (cultura == NULL || strlen(cultura) == 0 || strcasecmp(atual->cultura, cultura) == 0);

        if(ano_certo && estado_certo && cultura_certa){
            printf("%d | %d | %s | %s | %.2f | %.2f | %.2f | %.2f | %.2f\n",
                atual->id, atual->ano, atual->estado, atual->cultura, atual->preco_ton,
                atual->rendimento, atual->producao, atual->area_plantada, atual->valor_total);
            encontrados++;
        }
        atual = atual->prox;
    }

    if(encontrados == 0){
        printf("Nenhum resultado encontrado.\n");
    }else{
        printf("\nTotal de resultados: %d\n", encontrados);
    }
}

/*
Remove um elemento da lista pelo ID.
Parâmetros:
  cabeca - ponteiro para o ponteiro da cabeça da lista
  id - identificador do item a ser removido
Retorno: 1 se removido, 0 se não encontrado
*/
int remover_LE(ItemListaEncadeada **cabeca, int id){
    if(*cabeca == NULL){
        printf("ID nao encontrado\n");
    }
    
    ItemListaEncadeada *atual = *cabeca;
    ItemListaEncadeada *anterior = NULL;

    while(atual != NULL){
        if(atual->id == id){
            if(anterior == NULL){
                *cabeca = atual->prox;
            }else{
                anterior->prox = atual->prox;
            }
            free(atual);
            return 1;
        }
        anterior = atual;
        atual = atual->prox;
    }
    return 0;
}

/*
Imprime todos os elementos da lista encadeada.
Parâmetro: cabeca - ponteiro para a cabeça da lista
*/
void imprime_LE(ItemListaEncadeada *cabeca){
    while(cabeca != NULL){
        printf("%d | %d | %s | %s | %.2f | %.2f | %.2f | %.2f | %.2f\n", 
            cabeca->id, cabeca->ano, cabeca->estado, cabeca->cultura, cabeca->preco_ton, 
            cabeca->rendimento, cabeca->producao, cabeca->area_plantada, cabeca->valor_total);

        cabeca = cabeca->prox;
    }
}

/*
Libera toda a memória alocada pela lista encadeada.
Parâmetro: cabeca - ponteiro para a cabeça da lista
*/
void libera_LE(ItemListaEncadeada *cabeca){
    ItemListaEncadeada *temp;

    while(cabeca != NULL){
        temp = cabeca;
        cabeca = cabeca->prox;

        free(temp);
    }
}

/*
Carrega as amostras do dataset e insere na lista encadeada.
Parâmetros:
  cabeca - ponteiro para o ponteiro da cabeça da lista
  nome_arquivo - nome do arquivo de entrada (dataset)
*/
void carregar_dados_LE(ItemListaEncadeada **cabeca, const char *nome_arquivo){
    FILE *arq = fopen(nome_arquivo, "r");
    if(!arq){
        printf("Erro ao abrir o arquivo\n");
        exit(EXIT_FAILURE);
    }

    char linha[256];
    fgets(linha, sizeof(linha), arq);

    while(fgets(linha, sizeof(linha), arq)){
        ItemListaEncadeada novo;
        sscanf(linha, "%d;%d;%9[^;];%49[^;];%f;%f;%f;%f;%f", 
            &novo.id, &novo.ano, novo.estado, novo.cultura, &novo.preco_ton, 
            &novo.rendimento, &novo.producao, &novo.area_plantada, &novo.valor_total);
        inserir_LE(cabeca, novo);
    }

    fclose(arq);
}

/*
Salva todos os elementos da lista no dataset.
Parâmetros:
  cabeca - ponteiro para a cabeça da lista
  nome_arquivo - nome do arquivo de saída (dataset)
*/
void salvar_dados_LE(ItemListaEncadeada *cabeca, const char *nome_arquivo){
    FILE *arq = fopen(nome_arquivo, "w");
    if(!arq){
        printf("Erro ao abrir o arquivo\n");
        exit(EXIT_FAILURE);
    }

    fprintf(arq, "ID;Data;Localizacao;Tipo de plantio;Preco por tonelada (Dolares/tonelada);Rendimento (kilogramas por hectare);Producao (toneladas);Area plantada (hectares);Valor total da safra (Dolares)\n");
    ItemListaEncadeada *atual = cabeca;

    while(atual != NULL){
        fprintf(arq, "%d;%d;%s;%s;%.2f;%.2f;%.2f;%.2f;%.2f\n", atual->id, atual->ano, atual->estado, atual->cultura, atual->preco_ton, 
            atual->rendimento, atual->producao, atual->area_plantada, atual->valor_total);
        atual = atual->prox;
    }

    fclose(arq);
}

/*
Mede o tempo de inserção de n elementos do dataset na lista encadeada.
Parâmetros:
  nome_arquivo - nome do arquivo de entrada (dataset)
  n - número de elementos a serem inseridos
Retorno: tempo gasto em segundos (double)
*/
double bench_temp_insercao_LE(const char *nome_arquivo, int n){
    struct timespec inicio, fim;

    ItemListaEncadeada *cabeca = NULL;
    FILE *arq = fopen(nome_arquivo, "r");
    if(!arq){
        printf("Erro ao abrir o arquivo\n");
        return;
    }

    char linha[256];
    fgets(linha, sizeof(linha), arq);

    clock_gettime(CLOCK_MONOTONIC, &inicio);

    int i = 0;
    while(fgets(linha, sizeof(linha), arq) && i < n){
        ItemListaEncadeada novo;
        sscanf(linha, "%d,%d,%49[^,],%49[^,],%f,%f,%f,%f,%f", 
            &novo.id, &novo.ano, novo.estado, novo.cultura, &novo.preco_ton, 
            &novo.rendimento, &novo.producao, &novo.area_plantada, &novo.valor_total);
        inserir_LE(&cabeca, novo);
        i++;
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);

    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;
    
    libera_LE(cabeca);
    fclose(arq);
    return tempo;
}

/*
Mede o tempo de remoção de n elementos do dataset na lista encadeada.
Parâmetros:
  nome_arquivo - nome do arquivo de entrada (dataset)
  n - número de elementos a serem removidos
Retorno: tempo gasto em segundos (double)
*/
double bench_temp_remocao_LE(const char *nome_arquivo, int n){
    struct timespec inicio, fim;
    
    ItemListaEncadeada *cabeca = NULL;
    carregar_dados_LE(&cabeca, nome_arquivo);

    int *ids = (int*)malloc(n * sizeof(int));
    if(!ids){
        printf("Falha ao alocar memoria\n");
        libera_LE(cabeca);
        exit(EXIT_FAILURE);
    }

    ItemListaEncadeada *atual = cabeca;
    int total_ids = 0;

    while(atual != NULL && total_ids < n){
        ids[total_ids] = atual->id;
        atual = atual->prox;
        total_ids++;
    }

    srand(time(NULL));
    int i = total_ids - 1;
    for(i; i > 0; i--){
        int j = rand() % (i + 1);
        int temp = ids[i];
        ids[i] = ids[j];
        ids[j] = temp;
    }

    clock_gettime(CLOCK_MONOTONIC, &inicio);

    int k = 0;
    for(k = 0; k < total_ids; k++){
        remover_LE(&cabeca, ids[k]);
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);

    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    free(ids);
    libera_LE(cabeca);

    return tempo;
}

/*
Mede o tempo de busca de n elementos do dataset na lista encadeada.
Parâmetros:
  nome_arquivo - nome do arquivo de entrada (dataset)
  n - número de elementos a serem buscados
Retorno: tempo gasto em segundos (double)
*/
double bench_temp_busca_LE(const char *nome_arquivo, int n){
    struct timespec inicio, fim;
    ItemListaEncadeada *cabeca = NULL;
    carregar_dados_LE(&cabeca, nome_arquivo);

    int *ids = (int*)malloc(n * sizeof(int));
    if(!ids){
        printf("Falha ao alocar memoria\n");
        libera_LE(cabeca);
        exit(EXIT_FAILURE);
    }

    ItemListaEncadeada *atual = cabeca;
    int total_ids = 0;
    while(atual != NULL && total_ids < n){
        ids[total_ids] = atual->id;
        atual = atual->prox;
        total_ids++;
    }

    srand(time(NULL));
    int i = total_ids - 1;
    for(i; i > 0; i--){
        int j = rand() % (i + 1);
        int temp = ids[i];
        ids[i] = ids[j];
        ids[j] = temp;
    }

    clock_gettime(CLOCK_MONOTONIC, &inicio);

    int k = 0;
    for(k = 0; k < total_ids; k++){
        buscar_LE(cabeca, ids[k]);
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);

    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    free(ids);
    libera_LE(cabeca);

    return tempo;
}

/*
Mede a memória utilizada por n elementos do dataset quando inseridos na lista encadeada.
Parâmetros:
  nome_arquivo - nome do arquivo de entrada (dataset)
  n - número de elementos a serem inseridos
Retorno: Memória utilizada (bytes)
*/
size_t bench_uso_memoria_LE(const char *nome_arquivo, int n){
    ItemListaEncadeada *cabeca = NULL;
    FILE *arq = fopen(nome_arquivo, "r");
    if(!arq){
        printf("Erro ao abrir o arquivo\n");
        exit(EXIT_FAILURE);
    }

    char linha[256];
    int total = 0;

    fgets(linha, sizeof(linha), arq);

    while(fgets(linha, sizeof(linha), arq) && total < n){
        ItemListaEncadeada novo;
        sscanf(linha, "%d,%d,%49[^,],%49[^,],%f,%f,%f,%f,%f", 
            &novo.id, &novo.ano, novo.estado, novo.cultura, &novo.preco_ton, 
            &novo.rendimento, &novo.producao, &novo.area_plantada, &novo.valor_total);
        inserir_LE(&cabeca, novo);
        total++;
    }

    fclose(arq);

    size_t cont = 0;
    ItemListaEncadeada *atual = cabeca;
    while(atual != NULL){
        cont++;
        atual = atual->prox;
    }

    size_t uso_memoria = cont * sizeof(ItemListaEncadeada);
    libera_LE(cabeca);
    return uso_memoria;
}

/*
Mede o tempo de inserção de n elementos do dataset na lista encadeada com restrição de memória.
Parâmetros:
  nome_arquivo - nome do arquivo de entrada (dataset)
  lim_memoria_mb - limite de memória a ser utilizada
Retorno: tempo gasto em segundos (double)
*/
double bench_insercao_mem_restrita_LE(const char *nome_arquivo, double lim_memoria_mb){
    struct timespec inicio, fim;

    size_t lim_bytes = (size_t)(lim_memoria_mb * 1024 * 1024);
    FILE *arq = fopen(nome_arquivo, "r");
    if(!arq){
        printf("Erro ao abrir o arquivo\n");
        exit(EXIT_FAILURE);
    }

    char linha[256];
    fgets(linha, sizeof(linha), arq);

    ItemListaEncadeada *cabeca = NULL;
    size_t memoria_usada = 0;
    int total = 0;

    clock_gettime(CLOCK_MONOTONIC, &inicio);

    while(fgets(linha, sizeof(linha), arq)){
        if(memoria_usada + sizeof(ItemListaEncadeada) > lim_bytes){
            printf("\nLimite de memoria atingido apos %d elementos\n", total);
            break;
        }

        ItemListaEncadeada novo;
        sscanf(linha, "%d,%d,%49[^,],%49[^,],%f,%f,%f,%f,%f",
            &novo.id, &novo.ano, novo.estado, novo.cultura, &novo.preco_ton,
            &novo.rendimento, &novo.producao, &novo.area_plantada, &novo.valor_total);

        novo.prox = NULL;
        inserir_LE(&cabeca, novo);
        memoria_usada += sizeof(ItemListaEncadeada);
        total++;
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);

    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    libera_LE(cabeca);
    fclose(arq);

    return tempo;
}

/*
Mede o tempo de inserção com delay de n elementos do dataset na lista encadeada.
Parâmetros:
  nome_arquivo - nome do arquivo de entrada (dataset)
  n - número de elementos a serem inseridos
  delay_ms - tempo em milisegundos a ser aplicado como delay
Retorno: tempo gasto em segundos (double)
*/
double bench_insercao_com_delay_LE(const char *nome_arquivo, int n, int delay_ms){
    struct timespec inicio, fim;

    FILE *arq = fopen(nome_arquivo, "r");
    if(!arq){
        printf("Erro ao abrir o arquivo\n");
        exit(EXIT_FAILURE);
    }

    char linha[256];
    fgets(linha, sizeof(linha), arq);

    ItemListaEncadeada *cabeca = NULL;
    int total = 0;

    clock_gettime(CLOCK_MONOTONIC, &inicio);

    while(fgets(linha, sizeof(linha), arq) && total < n){
        ItemListaEncadeada novo;
        sscanf(linha, "%d,%d,%49[^,],%49[^,],%f,%f,%f,%f,%f",
            &novo.id, &novo.ano, novo.estado, novo.cultura, &novo.preco_ton,
            &novo.rendimento, &novo.producao, &novo.area_plantada, &novo.valor_total);

        novo.prox = NULL;
        inserir_LE(&cabeca, novo);
        Sleep(delay_ms);
        total++;
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);

    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    libera_LE(cabeca);
    fclose(arq);
    return tempo;
}

/*
Mede o tempo de busca com latencia de n elementos do dataset na lista encadeada.
Parâmetros:
  nome_arquivo - nome do arquivo de entrada (dataset)
  n - número de elementos a serem buscados
Retorno: tempo gasto em segundos (double)
*/
double bench_busca_latencia_LE(const char *nome_arquivo, int n){
    struct timespec inicio, fim;

    ItemListaEncadeada *cabeca = NULL;
    carregar_dados_LE(&cabeca, nome_arquivo);

    int *ids = (int*)malloc(n * sizeof(int));
    if(!ids){
        printf("Erro ao alocar memoria para ids\n");
        libera_LE(cabeca);
        return -1;
    }

    ItemListaEncadeada *atual = cabeca;
    int total_ids = 0;
    while(atual != NULL && total_ids < n){
        ids[total_ids++] = atual->id;
        atual = atual->prox;
    }

    if(total_ids == 0){
        printf("Nenhum ID carregado!\n");
        free(ids);
        libera_LE(cabeca);
        return 0;
    }

    srand(time(NULL));
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    int i = 0;
    for(i; i < n; i++){
        int idx = rand() % total_ids;
        int id_buscado = ids[idx];
        buscar_LE(cabeca, id_buscado);

        int delay = 2 + rand() % 8;
        Sleep(delay);
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);

    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    free(ids);
    libera_LE(cabeca);

    return tempo;
}

/*
Mede o tempo de inserção, com perda, de n elementos do dataset na lista encadeada.
Parâmetros:
  nome_arquivo - nome do arquivo de entrada (dataset)
  n - número de elementos a serem inseridos
Retorno: tempo gasto em segundos (double)
*/
double bench_temp_insercao_perda_LE(const char *nome_arquivo, int n){
    struct timespec inicio, fim;

    FILE *arq = fopen(nome_arquivo, "r");
    if(!arq){
        printf("Erro ao abrir o arquivo\n");
        exit(EXIT_FAILURE);
    }

    char linha[256];
    fgets(linha, sizeof(linha), arq);

    ItemListaEncadeada *cabeca = NULL;
    int total = 0;

    srand(time(NULL));
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    while(fgets(linha, sizeof(linha), arq) && total < n){
        if((rand() % 100) < 20){
            continue;
        }

        ItemListaEncadeada novo;
        sscanf(linha, "%d,%d,%49[^,],%49[^,],%f,%f,%f,%f,%f", 
            &novo.id, &novo.ano, novo.estado, novo.cultura, &novo.preco_ton, 
            &novo.rendimento, &novo.producao, &novo.area_plantada, &novo.valor_total);

        novo.prox = NULL;
        inserir_LE(&cabeca, novo);
        total++;
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);

    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    libera_LE(cabeca);
    fclose(arq);
    return tempo;
}

/*
Mede o tempo de busca de n elementos, com limite de acessos, do dataset na lista encadeada.
Parâmetros:
  nome_arquivo - nome do arquivo de entrada (dataset)
  n - número de elementos a serem buscados
  lim - limite de acessos
Retorno: tempo gasto em segundos (double)
*/
double bench_busca_lim_acessos_LE(const char *nome_arquivo, int n, int lim){
    struct timespec inicio, fim;

    ItemListaEncadeada *cabeca = NULL;
    carregar_dados_LE(&cabeca, nome_arquivo);

    int *ids = (int*)malloc(n * sizeof(int));
    if(!ids){
        printf("Falha ao alocar memoria para ids\n");
        libera_LE(cabeca);
        return -1;
    }

    ItemListaEncadeada *atual = cabeca;
    int total_ids = 0;
    while(atual != NULL && total_ids < n){
        ids[total_ids] = atual->id;
        atual = atual->prox;
        total_ids++;
    }

    if(total_ids == 0){
        printf("Nenhum id carregado!\n");
        free(ids);
        libera_LE(cabeca);
        return 0;
    }

    srand(time(NULL));
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    int i = 0;
    for(i; i < n; i++){
        int idx = rand() % total_ids;
        int id_buscado = ids[idx];

        ItemListaEncadeada *ptr = cabeca;
        int acessos = 0;
        while(ptr != NULL && acessos < lim){
            if(ptr->id == id_buscado){
                break;
            }
            ptr = ptr->prox;
            acessos++;
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);

    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    free(ids);
    libera_LE(cabeca);

    return tempo;
}
