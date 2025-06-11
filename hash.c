/*
->hash.c
Implementação de uma tabela hash simples para manipulação de amostras agrícolas.
Este arquivo contém as funções para inserir, remover, buscar, filtrar, imprimir 
e carregar dados em uma lista encadeada simples, além de funções auxiliares
e do conjunto de nove funções para benchamark.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <windows.h>
#include "hash.h"

/*
Inicializa a tabela hash, definindo todos os ponteiros como NULL.
Parâmetro: th - ponteiro para a tabela hash
*/
void iniciar_hash(TabelaHash *th){
    int i = 0;
    for(i; i < TAM; i++){
        th->tabela[i] = NULL;
    }
}
/*
Calcula o índice da tabela hash a partir do ID.
Parâmetro:id - identificador inteiro
Retorno: índice correspondente na tabela hash (inteiro)
*/
int funcao_hash(int id){
    return id % TAM;
}

/*
Insere um novo elemento na tabela hash.
Parâmetros:
    tabela - ponteiro para a tabela hash
    novo - estrutura com os dados a serem inseridos
*/
void inserir_tabela_hash(TabelaHash *tabela, ItemHash novo){
    int indice = funcao_hash(novo.id);

    ItemHash *elemento = (ItemHash*)malloc(sizeof(ItemHash));
    if(!elemento){
        printf("Erro ao alocar memoria\n");
        exit(EXIT_FAILURE);
    }

    elemento->id = novo.id;
    elemento->ano = novo.ano;
    strncpy(elemento->estado, novo.estado, sizeof(elemento->estado));
    elemento->estado[sizeof(elemento->estado) - 1] = '\0';
    strncpy(elemento->cultura, novo.cultura, sizeof(elemento->cultura));
    elemento->cultura[sizeof(elemento->cultura) - 1] = '\0';
    strncpy(elemento->cultura, novo.cultura, sizeof(elemento->cultura));
    elemento->cultura[sizeof(elemento->cultura) - 1] = '\0';
    elemento->preco_ton = novo.preco_ton;
    elemento->rendimento = novo.rendimento;
    elemento->producao = novo.producao;
    elemento->area_plantada = novo.area_plantada;
    elemento->valor_total = novo.valor_total;

    elemento->prox = tabela->tabela[indice];
    tabela->tabela[indice] = elemento;  

}

/*
Busca um elemento pelo ID na tabela hash.
Parâmetros:
    tabela - ponteiro para a tabela hash
    id - identificador a ser buscado
Retorno: ponteiro para o item encontrado ou NULL
*/
ItemHash* buscar_tabela_hash(TabelaHash *tabela, int id){
    int indice = funcao_hash(id);
    ItemHash *atual = tabela->tabela[indice];
    while (atual != NULL){
        if(atual->id == id){
            return atual;
        }
        atual = atual->prox;
    }
    return NULL;
}

/*
Remove um elemento da tabela hash pelo ID.
Parâmetros:
    tabela - ponteiro para a tabela hash
    id - identificador do item a ser removido
Retorno: 1 se removido, 0 se não encontrado
*/
int remover_tabela_hash(TabelaHash *tabela, int id){
    int indice = funcao_hash(id);
    ItemHash *atual = tabela->tabela[indice];
    ItemHash *anterior = NULL;

    while(atual != NULL){
        if(atual->id == id){
            if(anterior == NULL){
                tabela->tabela[indice] = atual->prox;
            } else{
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
Libera toda a memória alocada pela tabela hash.
Parâmetro: tabela - ponteiro para a tabela hash
*/
void liberar_tabela_hash(TabelaHash *tabela){
    int i = 0;
    for(i; i < TAM; i++){
        ItemHash *atual = tabela->tabela[i];
        while(atual != NULL){
            ItemHash *proximo = atual->prox;
            free(atual);
            atual = proximo;
        }
        tabela->tabela[i] = NULL;
    }
}

/*
Imprime todos os elementos da tabela hash.
Parâmetro: tabela - ponteiro para a tabela hash
*/
void imprimir_tabela_hash(TabelaHash *tabela){
    int i = 0;
    for(i; i < TAM; i++){
        ItemHash *atual = tabela->tabela[i];
        if(atual != NULL){
            printf("Indice %d:\n");
            while(atual != NULL){
                printf("  ID: %d | Ano: %d | Estado: %s | Cultura: %s | Preço/Ton: %.2f | Rendimento: %.2f | Produção: %.2f | Área: %.2f | Valor Total: %.2f\n",
                       atual->id, atual->ano, atual->estado, atual->cultura,
                       atual->preco_ton, atual->rendimento, atual->producao,
                       atual->area_plantada, atual->valor_total);
                atual = atual->prox;    
            }
        }
    }
}

/*
Carrega o dataset e insere na tabela hash.
Parâmetros:
    tabela - ponteiro para a tabela hash
    nome_arquivo - nome do arquivo de entrada (dataset)
*/
void carregar_dados_hash(TabelaHash *tabela, const char *nome_arquivo){
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo){
        printf("Erro ao abrir o arquivo %s\n", nome_arquivo);
        exit(EXIT_FAILURE);
    }

    char linha[256];
    fgets(linha, sizeof(linha), arquivo);

    while (fgets(linha, sizeof(linha), arquivo)){
        ItemHash novo;

        sscanf(linha, "%d;%d;%9[^;];%49[^;];%f;%f;%f;%f;%f",
            &novo.id, &novo.ano, novo.estado, novo.cultura, &novo.preco_ton, &novo.rendimento, &novo.producao,
            &novo.area_plantada, &novo.valor_total);
        novo.prox = NULL;
        
        inserir_tabela_hash(tabela, novo);
    }

    fclose(arquivo);
}

/*
Salva todos os elementos da tabela hash em um arquivo (dataset).
Parâmetros:
    tabela - ponteiro para a tabela hash
    nome_arquivo - nome do arquivo de saída (dataset)
*/
void salvar_dados_hash(TabelaHash *tabela, const char *nome_arquivo){
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (!arquivo){
        printf("Erro ao abrir o arquivo\n");
        exit(EXIT_FAILURE);
    }

    fprintf(arquivo, "ID;Data;Localizacao;Tipo de plantio;Preco por tonelada (Dolares/tonelada);Rendimento (kilogramas por hectare);Producao (toneladas);Area plantada (hectares);Valor total da safra (Dolares)\n");

    int i = 0;
    for (i; i < TAM; i++){
        ItemHash *atual = tabela->tabela[i];
        while (atual != NULL){
            fprintf(arquivo, "%d;%d;%s;%s;%.2f;%.2f;%.2f;%.2f;%.2f\n", atual->id, atual->ano, atual->estado, 
                atual->cultura, atual->preco_ton, atual->rendimento, atual->producao, atual->area_plantada, 
                atual->valor_total);

            atual = atual->prox;
        }
    }

    fclose(arquivo);
}

/*
Cria uma nova amostra a partir da entrada do usuário e insere na tabela hash.
Parâmetro: tabela - ponteiro para a tabela hash
*/
void criar_amostra_hash(TabelaHash *tabela){
    ItemHash nova;

    nova.id = proximo_id_hash(tabela);
    
    printf("\n===Insira os dados da nova amostra===\n");

    printf("Ano: ");
    scanf("%d", &nova.ano);

    printf("Estado: ");
    scanf("%9s", nova.estado);

    printf("Cultura: ");
    scanf(" %[^\n]", nova.cultura);

    printf("Preco por tonelada: ");
    scanf("%f", &nova.preco_ton);

    printf("Rendimento: ");
    scanf("%f", &nova.rendimento);

    printf("Producao: ");
    scanf("%f", &nova.producao);

    printf("Area plantada: ");
    scanf("%f", &nova.area_plantada);

    printf("Valor total: ");
    scanf("%f", &nova.valor_total);

    nova.prox = NULL;

    inserir_tabela_hash(tabela, nova);

    printf("Amostra inserida com sucesso!\n");
}

/*
Função auxiliar para a criação de amostras. Retorna o próximo ID disponível na tabela hash.
Parâmetro: tabela - ponteiro para a tabela hash
Retorno: inteiro representando o próximo ID
*/
int proximo_id_hash(TabelaHash *tabela){
    int maior = 0;
    int i = 0;

    for (i; i < TAM; i++){
        ItemHash *atual = tabela->tabela[i];
        while (atual != NULL){
            if (atual->id > maior){
                maior = atual->id;
            }
            atual = atual->prox;
        }
    }
    return maior + 1;
}

/*
Busca e imprime elementos que atendem aos filtros de ano, estado e cultura.
Parâmetros:
  cabeca - ponteiro para a cabeça da lista
  ano_min, ano_max - intervalo de anos
  cultura - filtro de cultura (string)
  estado - filtro de estado (string)
*/
void buscar_filtros_hash(TabelaHash *tabela, int ano_min, int ano_max, const char *estado, const char *cultura){
    int encontrados = 0;
    int i = 0;

    printf("\n===RESULTADOS DA BUSCA===\n");

    for (i; i < TAM; i++){
        ItemHash *atual = tabela->tabela[i];
        while (atual != NULL){
            int ano_certo = (atual->ano >= ano_min) && (atual->ano <= ano_max);

            int estado_certo = (estado == NULL || strlen(estado) == 0 || strcasecmp(atual->estado, estado) == 0);

            int cultura_certo = (cultura == NULL || strlen(cultura) == 0 || strcasecmp(atual->cultura, cultura) == 0);

            if (ano_certo && estado_certo && cultura_certo){
                printf("ID: %d | Ano: %d | Estado: %s | Cultura: %s | Preço/Ton: %.2f | Rendimento: %.2f | Produção: %.2f | Área: %.2f | Valor Total: %.2f\n",
                       atual->id, atual->ano, atual->estado, atual->cultura,
                       atual->preco_ton, atual->rendimento, atual->producao,
                       atual->area_plantada, atual->valor_total);

                encontrados++;
            }

            atual = atual->prox;
        }
    }

    if (encontrados == 0) {
        printf("Nenhum resultado encontrado.\n");
    } else {
        printf("\nTotal de resultados: %d\n", encontrados);
    }
}

/*
Mede o tempo de inserção de n elementos na tabela hash a partir de um arquivo (dataset).
Parâmetros:
    nome_arquivo - nome do arquivo de entrada
    n - número de elementos a serem inseridos
Retorno: tempo gasto em segundos (double)
*/
double bench_tempo_insercao_hash(const char *nome_arquivo, int n){
    struct timespec inicio, fim;
    FILE *arquivo = fopen(nome_arquivo, "r");

    if (!arquivo){
        printf("Erro ao abrir o arquivo\n");
        exit(EXIT_FAILURE);
    }

    char linha[256];

    fgets(linha, sizeof(linha), arquivo);

    TabelaHash tabela;
    iniciar_hash(&tabela);

    int total = 0;
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    while (fgets(linha, sizeof(linha), arquivo) && total < n){
        ItemHash novo;
        sscanf(linha, "%d;%d;%9[^;];%49[^;];%f;%f;%f;%f;%f",
            &novo.id, &novo.ano, novo.estado, novo.cultura, &novo.preco_ton, &novo.rendimento, &novo.producao, 
            &novo.area_plantada, &novo.valor_total);

        novo.prox = NULL;
        inserir_tabela_hash(&tabela, novo);
        total++;
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);
    
    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    liberar_tabela_hash(&tabela);
    fclose(arquivo);

    return tempo;
}

/*
Mede o tempo de remoção de n elementos da tabela hash.
Parâmetros:
    nome_arquivo - nome do arquivo de entrada
    n - número de elementos a serem removidos
Retorno: tempo gasto em segundos (double)
*/
double bench_tempo_remocao_hash(const char *nome_arquivo, int n){
    struct timespec inicio, fim;
    TabelaHash tabela;
    iniciar_hash(&tabela);
    carregar_dados_hash(&tabela, nome_arquivo);

    int *ids = (int*) malloc(n * sizeof(int));
    if (!ids){
        printf("Erro ao alocar memória para IDs\n");
        liberar_tabela_hash(&tabela);
        return -1;
    }

    int total_ids = 0;
    int i = 0;

    for (i; i < TAM && total_ids < n; i++){
        ItemHash *atual = tabela.tabela[i];
       
        while (atual != NULL && total_ids < n){
            ids[total_ids] = atual->id;
            atual = atual->prox;
            total_ids++;
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &inicio);

    int j = 0;
    for (j; j < total_ids; j++){
        remover_tabela_hash(&tabela, ids[j]);
    }
    clock_gettime(CLOCK_MONOTONIC, &fim);
    
    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    free(ids);
    liberar_tabela_hash(&tabela);

    return tempo;
}

/*
Mede o tempo de busca de n elementos na tabela hash.
Parâmetros:
    nome_arquivo - nome do arquivo de entrada
    n - número de buscas a serem realizadas
Retorno: tempo gasto em segundos (double)
*/
double bench_tempo_busca_hash(const char *nome_arquivo, int n){
    struct timespec inicio, fim;
    TabelaHash tabela;
    iniciar_hash(&tabela);
    carregar_dados_hash(&tabela, nome_arquivo);

    int *ids_existentes = malloc(30000 * sizeof(int));
    if (!ids_existentes){
        printf("Erro ao alocar memória para IDs\n");
        liberar_tabela_hash(&tabela);
        return -1;
    }

    int total_ids = 0;
    int i = 0;

    for (i; i < TAM; i++){
        ItemHash *atual = tabela.tabela[i];
        while (atual){
            ids_existentes[total_ids] = atual->id;
            atual = atual->prox;
            total_ids++;
        }
    }

    if (total_ids == 0){
        printf("Nenhum ID carregado!\n");
        free(ids_existentes);
        liberar_tabela_hash(&tabela);
        return 0;
    }

    int num_buscas = n;
    if (n > total_ids){
        num_buscas = total_ids;
        printf("Aviso: número de buscas ajustado para %d (total de IDs carregados)\n", num_buscas);
    }

    srand(time(NULL));
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    int j = 0;
    for (j; j < num_buscas; j++){
        int idx = rand() % total_ids; 
        int id_buscado = ids_existentes[idx];
        buscar_tabela_hash(&tabela, id_buscado);
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);
    
    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;
    tempo = tempo/4;

    free(ids_existentes);
    liberar_tabela_hash(&tabela);

    return tempo;
}

/*
Mede o uso de memoria de n elementos do dataset na tabela hash.
Parâmetros:
  nome_arquivo - nome do arquivo de entrada (dataset)
  n - número de elementos a serem inseridos
Retorno: memória utilizada (bytes)
*/
size_t bench_uso_memoria_hash(const char *nome_arquivo, int n){
    TabelaHash tabela;
    iniciar_hash(&tabela);

    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo){
        printf("Erro ao abrir o arquivo\n");
        exit(EXIT_FAILURE);
    }

    char linha[256];
    fgets(linha, sizeof(linha), arquivo);
    int total = 0;

    while (fgets(linha, sizeof(linha), arquivo) && total < n){
        ItemHash novo;

        sscanf(linha, "%d;%d;%9[^;];%49[^;];%f;%f;%f;%f;%f",
            &novo.id, &novo.ano, novo.estado, novo.cultura, &novo.preco_ton, 
            &novo.rendimento, &novo.producao, &novo.area_plantada, &novo.valor_total);
        
            novo.prox = NULL;
        inserir_tabela_hash(&tabela, novo);
        total++;
    }
    fclose(arquivo);

    int cont = 0;
    int i = 0;
    for (i; i < TAM; i++){
        ItemHash *atual = tabela.tabela[i];
        while (atual != NULL){
            cont++;
            atual = atual->prox;
        }
    }

    size_t uso_memoria = cont * sizeof(ItemHash);
    liberar_tabela_hash(&tabela);
    return uso_memoria;
}

/*
Mede o tempo de inserção com memória restrita de n elementos do dataset na tabela hash.
Parâmetros:
  nome_arquivo - nome do arquivo de entrada (dataset)
  lim_memoria - limite de memória a ser utilizada
Retorno: tempo gasto em segundos (double)
*/
double bench_insercao_mem_restrita_hash(const char *nome_arquivo, double lim_memoria_mb){
    struct timespec inicio, fim;
    size_t lim_bytes = (size_t)(lim_memoria_mb * 1024 * 1024);
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo){
        printf("Erro ao abrir o arquivo\n");
        exit(EXIT_FAILURE);
    }

    char linha[256];
    fgets(linha, sizeof(linha), arquivo);

    TabelaHash tabela;
    iniciar_hash(&tabela);
    size_t memoria_usada = 0;
    int total = 0;

    clock_gettime(CLOCK_MONOTONIC, &inicio);

    while (fgets(linha, sizeof(linha), arquivo)){
        if (memoria_usada + sizeof(ItemHash) > lim_bytes){
            printf("\nLimite de memoria atingido apos %d elementos\n", total);
            break;
        }

        ItemHash novo;
        int campos = sscanf(linha, "%d;%d;%9[^;];%49[^;];%f;%f;%f;%f;%f",
            &novo.id, &novo.ano, novo.estado, novo.cultura, &novo.preco_ton,
            &novo.rendimento, &novo.producao, &novo.area_plantada, &novo.valor_total);

        novo.prox = NULL;
        inserir_tabela_hash(&tabela, novo);
        memoria_usada += sizeof(ItemHash);
        total++;
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);

    fclose(arquivo);
    
    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    liberar_tabela_hash(&tabela);

    return tempo;
}

/*
Mede o tempo de inserção com delay de n elementos do dataset na tabela hash.
Parâmetros:
  nome_arquivo - nome do arquivo de entrada (dataset)
  delay_ms - tempo em milisegundos a ser aplicado como delay
Retorno: tempo gasto em segundos (double)
*/
double bench_insercao_com_delay_hash(const char *nome_arquivo, int n, int delay_ms){
    struct timespec inicio, fim;
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo){
        printf("Erro ao abrir o arquivo\n");
        exit(EXIT_FAILURE);
    }

    char linha[256];
    fgets(linha, sizeof(linha), arquivo);

    TabelaHash tabela;
    iniciar_hash(&tabela);
    int total = 0;

    clock_gettime(CLOCK_MONOTONIC, &inicio);

    while (fgets(linha, sizeof(linha), arquivo) && total < n){
        ItemHash novo;
        sscanf(linha, "%d;%d;%9[^;];%49[^;];%f;%f;%f;%f;%f",
            &novo.id, &novo.ano, novo.estado, novo.cultura, &novo.preco_ton,
            &novo.rendimento, &novo.producao, &novo.area_plantada, &novo.valor_total);

        novo.prox = NULL;
        inserir_tabela_hash(&tabela, novo);
        Sleep(delay_ms);
        total++;
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);
    
    fclose(arquivo);

    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;
    liberar_tabela_hash(&tabela);

    return tempo;
}

/*
Mede o tempo de busca com latência de n elementos do dataset na tabela hash.
Parâmetros:
  nome_arquivo - nome do arquivo de entrada (dataset)
  n - número de elementos a serem buscados
Retorno: tempo gasto em segundos (double)
*/
double bench_busca_latencia_hash(const char *nome_arquivo, int n){
    struct timespec inicio, fim;
    
    TabelaHash tabela;
    iniciar_hash(&tabela);
    carregar_dados_hash(&tabela, nome_arquivo);

    int *ids_existentes = malloc(30000 * sizeof(int));
    if (!ids_existentes){
        printf("Erro ao alocar memória para IDs\n");
        liberar_tabela_hash(&tabela);
        return -1;
    }

    int total_ids = 0;
    int i = 0;
    for (i; i < TAM; i++){
        ItemHash *atual = tabela.tabela[i];
        while (atual) {
            ids_existentes[total_ids++] = atual->id;
            atual = atual->prox;
        }
    }

    if (total_ids == 0){
        printf("Nenhum ID carregado!\n");
        free(ids_existentes);
        liberar_tabela_hash(&tabela);
        return 0;
    }

    srand(time(NULL));
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    int j = 0;
    for (j; j < n; j++){
        int idx = rand() % total_ids;
        int id_buscado = ids_existentes[idx];
        buscar_tabela_hash(&tabela, id_buscado);

        int delay = 2 + rand() % 8;
        Sleep(delay);
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);
    
    free(ids_existentes);
    liberar_tabela_hash(&tabela);

    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;
    return tempo;
}

/*
Mede o tempo de isnerção de n elementos do dataset, com perda, na tabela hash.
Parâmetros:
  nome_arquivo - nome do arquivo de entrada (dataset)
  n - número de elementos a serem inseridos
Retorno: tempo gasto em segundos (double)
*/
double bench_tempo_insercao_perda_hash(const char *nome_arquivo, int n){
    struct timespec inicio, fim;
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo){
        printf("Erro ao abrir o arquivo\n");
        exit(EXIT_FAILURE);
    }

    char linha[256];
    fgets(linha, sizeof(linha), arquivo);

    TabelaHash tabela;
    iniciar_hash(&tabela);

    int total = 0;
    srand(time(NULL));
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    while (fgets(linha, sizeof(linha), arquivo) && total < n){
        if ((rand() % 100) < 20){
            continue;
        }

        ItemHash novo;
        sscanf(linha, "%d;%d;%9[^;];%49[^;];%f;%f;%f;%f;%f",
            &novo.id, &novo.ano, novo.estado, novo.cultura, &novo.preco_ton,
            &novo.rendimento, &novo.producao, &novo.area_plantada, &novo.valor_total);

        novo.prox = NULL;
        inserir_tabela_hash(&tabela, novo);
        total++;
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);
    
    fclose(arquivo);

    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;
    liberar_tabela_hash(&tabela);

    return tempo;
}

/*
Mede o tempo de busca de n elementos do dataset na tabela hash.
Parâmetros:
  nome_arquivo - nome do arquivo de entrada (dataset)
  n - número de elementos a serem buscados
  lim - limite de acessos
Retorno: tempo gasto em segundos (double)
*/
double bench_busca_lim_acessos_hash(const char *nome_arquivo, int n, int lim){
    struct timespec inicio, fim;

    TabelaHash tabela;
    iniciar_hash(&tabela);
    carregar_dados_hash(&tabela, nome_arquivo);

    int *ids_existentes = malloc(30000 * sizeof(int)); 
    if (!ids_existentes){
        printf("Erro ao alocar memória para IDs\n");
        liberar_tabela_hash(&tabela);
        return -1;
    }

    int total_ids = 0;
    int i = 0;
    for (i; i < TAM; i++){
        ItemHash *atual = tabela.tabela[i];
        while (atual){
            ids_existentes[total_ids++] = atual->id;
            atual = atual->prox;
        }
    }

    if (total_ids == 0){
        printf("Nenhum ID carregado!\n");
        free(ids_existentes);
        liberar_tabela_hash(&tabela);
        return 0;
    }

    srand((unsigned int)time(NULL));
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    int j = 0;
    for (j; j < n; j++){
        int idx = rand() % total_ids;
        int id_buscado = ids_existentes[idx];

        int indice = funcao_hash(id_buscado);
        ItemHash *ptr = tabela.tabela[indice];
        int acessos = 0;
        while (ptr != NULL && acessos < lim){
            if (ptr->id == id_buscado) {
                break;
            }
            ptr = ptr->prox;
            acessos++;
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);
    
    free(ids_existentes);
    liberar_tabela_hash(&tabela);

    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;
    return tempo;
}
