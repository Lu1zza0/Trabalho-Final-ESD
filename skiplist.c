/*
->skiplist.c
Implementação de uma skip list para manipulação de amostras agrícolas.
Este arquivo contém as funções para inserir, remover, buscar, filtrar, imprimir
e carregar dados em uma skip list, além de funções auxiliares
e do conjunto de nove funções para benchmark.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <windows.h>
#include "skiplist.h"

/*
Inicializa e retorna um ponteiro para uma nova skip list vazia.
Retorno: ponteiro para a estrutura Skiplist inicializada
*/
Skiplist* iniciar_skiplist(){
    Skiplist* lista = malloc(sizeof(Skiplist));
    if (!lista){
        printf("Erro ao alocar memória para a skiplist\n");
        exit(EXIT_FAILURE);
    }
    lista->nivel = 0;

    lista->cabeca = malloc(sizeof(ElementoSkiplist));
    if (!lista->cabeca){
        printf("Erro ao alocar memória para o header\n");
        free(lista);
        exit(EXIT_FAILURE);
    }

    int i = 0;
    for (i; i < NIVEL_MAX_SKIPLIST; i++){
        lista->cabeca->proximo[i] = NULL;
    }

    return lista;
}

/*
Gera de forma probabilística o nível de um novo nó na skip list.
Retorno: nível inteiro gerado aleatoriamente
*/
int nivel_aleatorio_skiplist(){
    int nvl = 1;
    while ((rand() % 100) < 50 && nvl < NIVEL_MAX_SKIPLIST){
        nvl++;
    }
    return nvl;
}

/*
Insere um novo elemento na skip list, mantendo a ordem por ID.
Parâmetros:
    lista - ponteiro para a skip list
    novo_dado - estrutura com os dados a serem inseridos
*/
void inserir_skiplist(Skiplist *lista, ElementoSkiplist novo_dado){
    ElementoSkiplist *atualizacao[NIVEL_MAX_SKIPLIST];
    ElementoSkiplist *x = lista->cabeca;
    
    int i = lista->nivel;
    for (i; i >= 0; i--){
        while (x->proximo[i] != NULL && x->proximo[i]->id < novo_dado.id){
            x = x->proximo[i];
        }
        atualizacao[i] = x;
    }
    
    int novo_nivel = nivel_aleatorio_skiplist();
    ElementoSkiplist *novo = malloc(sizeof(ElementoSkiplist));
    if (!novo){
        printf("Erro na alocação do novo nó\n");
        exit(EXIT_FAILURE);
    }
    
    *novo = novo_dado;
    int m = 0;
    for (m; m < NIVEL_MAX_SKIPLIST; m++) {
        novo->proximo[m] = NULL;
    }
    
    if (novo_nivel > lista->nivel){
        int j = lista->nivel + 1;
        for (j; j < novo_nivel; j++){
            atualizacao[j] = lista->cabeca;
        }
        lista->nivel = novo_nivel - 1;
    }
    
    int k = 0;
    for (k; k < novo_nivel; k++){
        novo->proximo[k] = atualizacao[k]->proximo[k];
        atualizacao[k]->proximo[k] = novo;
    }
}

/*
Busca um elemento pelo ID na skip list.
Parâmetros:
    lista - ponteiro para a skip list
    id - identificador a ser buscado
Retorno: ponteiro para o elemento encontrado ou NULL
*/
ElementoSkiplist* buscar_skiplist(Skiplist* lista, int id){
    ElementoSkiplist* atual = lista->cabeca;
    
    int i = lista->nivel;
    for (i; i >= 0; i--){
        while (atual->proximo[i] != NULL && atual->proximo[i]->id < id){
            atual = atual->proximo[i];
        }
    }
    
    atual = atual->proximo[0];
    if (atual != NULL && atual->id == id){
        return atual;
    }
    else
        return NULL;
}

/*
Remove um elemento da skip list pelo ID.
Parâmetros:
    lista - ponteiro para a skip list
    id - identificador do elemento a ser removido
Retorno: 1 se removido, 0 se não encontrado
*/
int remover_skiplist(Skiplist *lista, int id){
    ElementoSkiplist *atual = lista->cabeca;
    ElementoSkiplist *atualizacao[NIVEL_MAX_SKIPLIST];
    
    int i = lista->nivel;
    for (i; i >= 0; i--){
        while (atual->proximo[i] != NULL && atual->proximo[i]->id < id){
            atual = atual->proximo[i];
        }
        atualizacao[i] = atual;
    }

    ElementoSkiplist *remover = atual->proximo[0];
    if (remover == NULL || remover->id != id){
        return 0;
    }

    int j = 0;
    for (j; j <= lista->nivel; j++){
        if (atualizacao[j]->proximo[j] != remover) break;
        atualizacao[j]->proximo[j] = remover->proximo[j];
    }

    free(remover);

    while (lista->nivel > 0 && lista->cabeca->proximo[lista->nivel] == NULL){
        lista->nivel--;
    }
    return 1;
}

/*
Imprime todos os elementos presentes na skip list.
Parâmetro:lista - ponteiro para a skip list
*/
void imprime_skiplist(Skiplist* lista){
    ElementoSkiplist* atual = lista->cabeca->proximo[0];
    int contador = 0;
    
    while (atual != NULL){
        printf("%d | %d | %s | %s | %.2f | %.2f | %.2f | %.2f | %.2f\n",
             atual->id, atual->ano, atual->estado, atual->cultura, atual->preco_ton,
              atual->rendimento, atual->producao, atual->area_plantada, atual->valor_total);
        contador++;
        atual = atual->proximo[0];
    }
    printf("Total de elementos: %d\n", contador);
}

/*
Libera toda a memória alocada pela skip list.
Parâmetro: lista - ponteiro para a skip list
*/
void libera_skiplist(Skiplist* lista){
    ElementoSkiplist* atual = lista->cabeca->proximo[0];
    ElementoSkiplist* proximo;

    while (atual != NULL){
        proximo = atual->proximo[0];
        free(atual);
        atual = proximo;
    }
    free(lista->cabeca);
    free(lista);
}

/*
Carrega dados de um arquivo e insere na skip list.
Parâmetros:
    lista - ponteiro para a skip list
    nome_arquivo - nome do arquivo de entrada
*/
void carregar_dados_skiplist(Skiplist* lista, const char* nome_arquivo){
    FILE* arquivo = fopen(nome_arquivo, "r");
    if (!arquivo){
        printf("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    char linha[256];
    fgets(linha, sizeof(linha), arquivo);

    while (fgets(linha, sizeof(linha), arquivo)){
        ElementoSkiplist novo;
        sscanf(linha, "%d;%d;%9[^;];%49[^;];%f;%f;%f;%f;%f",
               &novo.id, &novo.ano, novo.estado, novo.cultura, &novo.preco_ton, &novo.rendimento, &novo.producao,
               &novo.area_plantada, &novo.valor_total);

        inserir_skiplist(lista, novo);
    }

    fclose(arquivo);
}

/*
Salva todos os elementos da skip list em um arquivo.
Parâmetros:
    lista - ponteiro para a skip list
    nome_arquivo - nome do arquivo de saída
*/
void salvar_dados_skiplist(Skiplist* lista, const char* nome_arquivo){
    FILE* arquivo = fopen(nome_arquivo, "w");
    if (!arquivo){
        printf("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    fprintf(arquivo, "ID;Data;Localizacao;Tipo de plantio;Preco por tonelada (Dolares/tonelada);Rendimento (kilogramas por hectare);Producao (toneladas);Area plantada (hectares);Valor total da safra (Dolares)\n");

    ElementoSkiplist* atual = lista->cabeca->proximo[0];
    while (atual != NULL){
        fprintf(arquivo, "%d;%d;%s;%s;%.2f;%.2f;%.2f;%.2f;%.2f\n",
                atual->id, atual->ano, atual->estado, atual->cultura,
                atual->preco_ton, atual->rendimento, atual->producao,
                atual->area_plantada, atual->valor_total);
        atual = atual->proximo[0];
    }

    fclose(arquivo);
}

/*
Função auxiliar para a criação de amostras. Retorna o próximo ID disponível na skip list.
Parâmetro: lista - ponteiro para a skip list
Retorno: inteiro representando o próximo ID
*/
int criar_id_skiplist(Skiplist* lista){
    int maior_id = 0;
    ElementoSkiplist* atual = lista->cabeca->proximo[0];
    
    while(atual != NULL){
        if(atual->id > maior_id){
            maior_id = atual->id;
        }
        atual = atual->proximo[0];
    }
    return maior_id + 1;
}

/*
Cria uma nova amostra a partir da entrada do usuário e insere na skip list.
Parâmetro: lista - ponteiro para a skip list
*/
void criar_amostra_skiplist(Skiplist* lista){
    ElementoSkiplist nova;
    char entrada[50];
     
    nova.id = criar_id_skiplist(lista);
    
    printf("\n===Insira os dados da nova amostra===\n");

    printf("Ano: ");
    fgets(entrada, sizeof(entrada), stdin);
    sscanf(entrada, "%d", &nova.ano);
    
    printf("Estado: ");
    fgets(nova.estado, sizeof(nova.estado), stdin);
    nova.estado[strcspn(nova.estado, "\n")] = '\0';
    
    printf("Cultura: ");
    fgets(nova.cultura, sizeof(nova.cultura), stdin);
    nova.cultura[strcspn(nova.cultura, "\n")] = '\0';
    
    printf("Preco por tonelada: ");
    fgets(entrada, sizeof(entrada), stdin);
    sscanf(entrada, "%f", &nova.preco_ton);
    
    printf("Rendimento: ");
    fgets(entrada, sizeof(entrada), stdin);
    sscanf(entrada, "%f", &nova.rendimento);
    
    printf("Producao: ");
    fgets(entrada, sizeof(entrada), stdin);
    sscanf(entrada, "%f", &nova.producao);
    
    printf("Area plantada: ");
    fgets(entrada, sizeof(entrada), stdin);
    sscanf(entrada, "%f", &nova.area_plantada);
    
    printf("Valor total: ");
    fgets(entrada, sizeof(entrada), stdin);
    sscanf(entrada, "%f", &nova.valor_total);
    
    inserir_skiplist(lista, nova);
    printf("\nAmostra inserida com sucesso! ID: %d\n", nova.id);
}

/*
Busca e imprime elementos que atendem aos filtros de ano, estado e cultura.
Parâmetros:
  cabeca - ponteiro para a cabeça da lista
  ano_min, ano_max - intervalo de anos
  cultura - filtro de cultura (string)
  estado - filtro de estado (string)
*/
void buscarFiltros_skiplist(Skiplist* lista, int ano_min, int ano_max, const char* estado, const char* cultura){
    ElementoSkiplist* atual = lista->cabeca->proximo[0];
    int encontrados = 0;

    printf("\n===RESULTADOS DA BUSCA===\n");
    while (atual != NULL){
        int ano_certo = (atual->ano >= ano_min) && (atual->ano <= ano_max);
        int estado_certo = (estado == NULL || strlen(estado) == 0 || strcasecmp(atual->estado, estado) == 0);
        int cultura_certa = (cultura == NULL || strlen(cultura) == 0 || strcasecmp(atual->cultura, cultura) == 0);

        if (ano_certo && estado_certo && cultura_certa){
            printf("%d | %d | %s | %s | %.2f | %.2f | %.2f | %.2f | %.2f\n",
                atual->id, atual->ano, atual->estado, atual->cultura,
                atual->preco_ton, atual->rendimento, atual->producao,
                atual->area_plantada, atual->valor_total);
            encontrados++;
        }
        atual = atual->proximo[0];
    }

    if (encontrados == 0){
        printf("\nNenhum resultado encontrado\n");
    } else{
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
double bench_temp_insercao_skiplist(const char* nome_arquivo, int n){
    struct timespec inicio, fim;
    Skiplist* lista = iniciar_skiplist();
    FILE* arquivo = fopen(nome_arquivo, "r");
    
    if(!arquivo){
        printf("Erro ao abrir arquivo");
        libera_skiplist(lista);
        exit(EXIT_FAILURE);
    }

    char linha[256];
    fgets(linha, sizeof(linha), arquivo);

    int total_inseridos = 0;
    
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    
    while(fgets(linha, sizeof(linha), arquivo) && total_inseridos < n){
        ElementoSkiplist novo;
        sscanf(linha, "%d;%d;%9[^;];%49[^;];%f;%f;%f;%f;%f",
               &novo.id, &novo.ano, novo.estado, novo.cultura, &novo.preco_ton, &novo.rendimento, &novo.producao,
               &novo.area_plantada, &novo.valor_total);
        
        inserir_skiplist(lista, novo);
        total_inseridos++;
    }
    
    clock_gettime(CLOCK_MONOTONIC, &fim);

    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;


    fclose(arquivo);
    libera_skiplist(lista);
    
    return tempo;
}

/*
Mede o tempo de remoção de n elementos da skip list.
Parâmetros:
    nome_arquivo - nome do arquivo de entrada
    n - número de elementos a serem removidos
Retorno: tempo gasto em segundos (double)
*/
double bench_temp_remocao_skiplist(const char* arquivo, int n){
    struct timespec inicio, fim;
    Skiplist* lista = iniciar_skiplist();
    carregar_dados_skiplist(lista, arquivo);

    int* ids = malloc(n * sizeof(int));
    ElementoSkiplist* atual = lista->cabeca->proximo[0];
    int total_ids = 0;
    
    while(atual != NULL && total_ids < n){
        ids[total_ids] = atual->id;
        atual = atual->proximo[0];
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

    int j = 0;
    for(j; j < total_ids; j++){
        remover_skiplist(lista, ids[j]);
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);

    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    free(ids);
    libera_skiplist(lista);

    return tempo;
}

/*
Mede o tempo de busca de n elementos na skip list.
Parâmetros:
    nome_arquivo - nome do arquivo de entrada
    n - número de buscas a serem realizadas
Retorno: tempo gasto em segundos (double)
*/
double bench_temp_busca_skiplist(const char* arquivo, int n){
    struct timespec inicio, fim;
    Skiplist* lista = iniciar_skiplist();
    carregar_dados_skiplist(lista, arquivo);

    int* ids = malloc(n * sizeof(int));
    ElementoSkiplist* atual = lista->cabeca->proximo[0];
    int total_ids = 0;
    
    while(atual != NULL && total_ids < n){
        ids[total_ids] = atual->id;
        atual = atual->proximo[0];
        total_ids++;
    }

    srand(time(NULL));
    int i = total_ids - 1;
    for(i; i > 0; i--){
        int j = rand() % (i+1);
        int temp = ids[i];
        ids[i] = ids[j];
        ids[j] = temp;
    }

    clock_gettime(CLOCK_MONOTONIC, &inicio);
    
    int j = 0;
    for(j; j < total_ids; j++){
        buscar_skiplist(lista, ids[j]);
    }
    
    clock_gettime(CLOCK_MONOTONIC, &fim);
    
    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;
 
    free(ids);
    libera_skiplist(lista);
    
    return tempo;
}

/*
Mede o uso de memoria de n elementos do dataset na skip list.
Parâmetros:
  nome_arquivo - nome do arquivo de entrada (dataset)
  n - número de elementos a serem inseridos
Retorno: memória utilizada (bytes)
*/
size_t bench_uso_memoria_skiplist(const char* nome_arquivo, int n){
    Skiplist* lista = iniciar_skiplist();
    FILE* arquivo = fopen(nome_arquivo, "r");
    
    if(!arquivo){
        printf("Erro ao abrir o arquivo\n");
        libera_skiplist(lista);
        exit(EXIT_FAILURE);
    }

    char linha[256];
    fgets(linha, sizeof(linha), arquivo); 

    int elementos_carregados = 0;
    while(elementos_carregados < n && fgets(linha, sizeof(linha), arquivo)){
        ElementoSkiplist novo;
        sscanf(linha, "%d;%d;%9[^;];%49[^;];%f;%f;%f;%f;%f",
            &novo.id, &novo.ano, novo.estado, novo.cultura, &novo.preco_ton, &novo.rendimento, &novo.producao,
            &novo.area_plantada, &novo.valor_total);
        
        inserir_skiplist(lista, novo);
        elementos_carregados++;
    }

    fclose(arquivo);
    
    ElementoSkiplist* atual = lista->cabeca->proximo[0];
    int cont = 0;
    while(atual != NULL){
        cont++;
        atual = atual->proximo[0];
    }

    size_t uso_memoria = cont * sizeof(ElementoSkiplist);
    libera_skiplist(lista);
    
    return uso_memoria;
}

/*
Mede o tempo de inserção com memória restrita de n elementos do dataset na skip list.
Parâmetros:
  nome_arquivo - nome do arquivo de entrada (dataset)
  lim_memoria - limite de memória a ser utilizada
Retorno: tempo gasto em segundos (double)
*/
double bench_insercao_mem_restrita_skiplist(const char* nome_arquivo, double lim_memoria_mb){
    struct timespec inicio, fim;
    size_t lim_bytes = (size_t)(lim_memoria_mb * 1024 * 1024);
    FILE* arquivo = fopen(nome_arquivo, "r");
    if(!arquivo){
        printf("Erro ao abrir o arquivo\n");
        exit(EXIT_FAILURE);
    }

    char linha[256];
    fgets(linha, sizeof(linha), arquivo); 

    Skiplist* lista = iniciar_skiplist();
    size_t memoria_usada = sizeof(Skiplist) + sizeof(ElementoSkiplist); 
    int total = 0;

    clock_gettime(CLOCK_MONOTONIC, &inicio);

    while(fgets(linha, sizeof(linha), arquivo)){
        if(memoria_usada + sizeof(ElementoSkiplist) > lim_bytes){
            printf("Limite de memoria atingido apos %d elementos\n", total);
            break;
        }
        ElementoSkiplist novo;
        sscanf(linha, "%d;%d;%9[^;];%49[^;];%f;%f;%f;%f;%f",
            &novo.id, &novo.ano, novo.estado, novo.cultura, &novo.preco_ton, &novo.rendimento, &novo.producao, 
            &novo.area_plantada, &novo.valor_total);

        inserir_skiplist(lista, novo);
        memoria_usada += sizeof(ElementoSkiplist);
        total++;
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);
    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    libera_skiplist(lista);
    fclose(arquivo);
    return tempo;
}

/*
Mede o tempo de inserção com delay de n elementos do dataset na skip list.
Parâmetros:
  nome_arquivo - nome do arquivo de entrada (dataset)
  delay_ms - tempo em milisegundos a ser aplicado como delay
Retorno: tempo gasto em segundos (double)
*/
double bench_insercao_com_delay_skiplist(const char* nome_arquivo, int n, int delay_ms){
    Skiplist* lista = iniciar_skiplist();
    FILE* arquivo = fopen(nome_arquivo, "r");
    if(!arquivo){
        printf("Erro ao abrir o arquivo\n");
        exit(EXIT_FAILURE);
    }

    char linha[256];
    fgets(linha, sizeof(linha), arquivo);

    int total = 0;

    struct timespec inicio, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    while(fgets(linha, sizeof(linha), arquivo) && total < n){
        ElementoSkiplist novo;
        sscanf(linha, "%d;%d;%9[^;];%49[^;];%f;%f;%f;%f;%f",
            &novo.id, &novo.ano, novo.estado, novo.cultura, &novo.preco_ton,
            &novo.rendimento, &novo.producao, &novo.area_plantada, &novo.valor_total);

        inserir_skiplist(lista, novo);
        printf("inserido");
        Sleep(delay_ms);
        total++;
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);

    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    libera_skiplist(lista);
    fclose(arquivo);

    return tempo;
}

/*
Mede o tempo de busca com latência de n elementos do dataset na skip list.
Parâmetros:
  nome_arquivo - nome do arquivo de entrada (dataset)
  n - número de elementos a serem buscados
Retorno: tempo gasto em segundos (double)
*/
double bench_busca_latencia_skiplist(const char* nome_arquivo, int n){
    struct timespec inicio, fim;
    Skiplist* lista = iniciar_skiplist();
    carregar_dados_skiplist(lista, nome_arquivo);

    int* ids = malloc(n * sizeof(int));
    ElementoSkiplist* atual = lista->cabeca->proximo[0];
    int total_ids = 0;
    while(atual != NULL && total_ids < n){
        ids[total_ids++] = atual->id;
        atual = atual->proximo[0];
    }

    if(total_ids == 0){
        printf("Nenhum ID carregado!\n");
        free(ids);
        libera_skiplist(lista);
        return 0;
    }

    srand((unsigned int)time(NULL));
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    int j = 0;
    for(j; j < n; j++){
        int idx = rand() % total_ids;
        int id_buscado = ids[idx];
        buscar_skiplist(lista, id_buscado);

        int delay = 2 + rand() % 8;
        Sleep(delay);
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);

    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;
    free(ids);
    libera_skiplist(lista);

    return tempo;
}

/*
Mede o tempo de isnerção de n elementos do dataset, com perda, na skip list.
Parâmetros:
  nome_arquivo - nome do arquivo de entrada (dataset)
  n - número de elementos a serem inseridos
Retorno: tempo gasto em segundos (double)
*/
double bench_temp_insercao_perda_skiplist(const char* nome_arquivo, int n){
    struct timespec inicio, fim;
    Skiplist* lista = iniciar_skiplist();
    FILE* arquivo = fopen(nome_arquivo, "r");
    
    if(!arquivo){
        printf("Erro ao abrir arquivo");
        libera_skiplist(lista);
        exit(EXIT_FAILURE);
    }

    char linha[256];
    fgets(linha, sizeof(linha), arquivo);

    int total_inseridos = 0;
    srand((unsigned int)time(NULL));
    
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    
    while(fgets(linha, sizeof(linha), arquivo) && total_inseridos < n){
        if((rand() % 100) < 20){ 
            continue;
        }
        ElementoSkiplist novo;
        sscanf(linha, "%d;%d;%9[^;];%49[^;];%f;%f;%f;%f;%f",
               &novo.id, &novo.ano, novo.estado, novo.cultura, &novo.preco_ton,
               &novo.rendimento, &novo.producao, &novo.area_plantada, &novo.valor_total);
        
        inserir_skiplist(lista, novo);
        total_inseridos++;
    }
    
    clock_gettime(CLOCK_MONOTONIC, &fim);

    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    fclose(arquivo);
    libera_skiplist(lista);
    
    return tempo;
}

/*
Mede o tempo de busca de n elementos do dataset na skip list.
Parâmetros:
  nome_arquivo - nome do arquivo de entrada (dataset)
  n - número de elementos a serem buscados
  lim - limite de acessos
Retorno: tempo gasto em segundos (double)
*/
double bench_busca_lim_acessos_skiplist(const char* nome_arquivo, int n, int lim){
    struct timespec inicio, fim;
    Skiplist* lista = iniciar_skiplist();
    carregar_dados_skiplist(lista, nome_arquivo);

    int* ids = malloc(n * sizeof(int));
    ElementoSkiplist* atual = lista->cabeca->proximo[0];
    int total_ids = 0;
    while(atual != NULL && total_ids < n){
        ids[total_ids++] = atual->id;
        atual = atual->proximo[0];
    }

    if(total_ids == 0){
        printf("Nenhum ID carregado!\n");
        free(ids);
        libera_skiplist(lista);
        return 0;
    }

    srand((unsigned int)time(NULL));
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    int j = 0;
    for(j; j < n; j++){
        int idx = rand() % total_ids;
        int id_buscado = ids[idx];

        ElementoSkiplist* ptr = lista->cabeca;
        int acessos = 0;
        int nivel = lista->nivel;
        while(nivel >= 0 && acessos < lim){
            while(ptr->proximo[nivel] != NULL && ptr->proximo[nivel]->id < id_buscado && acessos < lim){
                ptr = ptr->proximo[nivel];
                acessos++;
            }
            nivel--;
            acessos++;
        }
        if(ptr->proximo[0] != NULL && ptr->proximo[0]->id == id_buscado){
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);

    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;
    free(ids);
    libera_skiplist(lista);

    return tempo;
}
