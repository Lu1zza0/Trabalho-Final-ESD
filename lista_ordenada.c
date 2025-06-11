/*
->lista_ordenada.c
Implementação de uma lista encadeada ordenada por ID para manipulação de amostras agrícolas.
Este arquivo contém as funções para inserir, remover, buscar, filtrar, imprimir 
e carregar dados em uma lista encadeada simples, além de funções auxiliares
e do conjunto de nove funções para benchamark.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include "lista_ordenada.h"
#include <windows.h>

/*
Insere um novo elemento na lista ordenada por ID.
Parâmetros:
    cabeca - ponteiro para a cabeça da lista
    novo - estrutura com os dados a serem inseridos
*/
void insereOrdenadoID_LO(ItemLista **cabeca, ItemLista novo){
    ItemLista *auxiliar = (ItemLista*)malloc(sizeof(ItemLista));
    *auxiliar = novo;
    auxiliar->prox = NULL;


    if(*cabeca==NULL || (*cabeca)->id > auxiliar->id){
        auxiliar->prox = NULL;
        *cabeca = auxiliar;
    }

    else{
        ItemLista *atual = *cabeca;
        while(atual->prox!=NULL && atual->prox->id < auxiliar->id){
            atual = atual->prox;
        }       

        auxiliar->prox = atual->prox;
        atual->prox = auxiliar;
    }
}

/*
Imprime todos os elementos na lista ordenada por ID.
Parâmetros:
    cabeca - ponteiro para a cabeça da lista
*/
void imprime_LO(ItemLista *cabeca){
    ItemLista *atual = cabeca;
    int i = 0;
    while(atual!=NULL){
        printf("%d | %d | %s | %s | %.2f | %.2f | %.2f | %.2f | %.2f\n",
             atual->id, atual->ano, atual->estado, atual->cultura, atual->preco_ton,
              atual->rendimento, atual->producao, atual->area_plantada, atual->valor_total);
        i++;
        atual = atual->prox;
    }
    printf("Total de itens da lista: %d\n", i);
}

/*
Libera toda a memoria alocada para lista ordenada.
Parâmetros:
    cabeca - ponteiro para a cabeça da lista
*/
void libera_LO(ItemLista *cabeca){
    ItemLista *atual = cabeca;

    ItemLista *liberado;
    while(atual!=NULL){
        liberado = atual;
        atual = atual->prox;
        free(liberado);
    }
}

/*
Carrega as informações do dataset para a lista ordenada.
Parâmetros:
    cabeca - ponteiro para a cabeça da lista
    nome_arquivo - nome do arquivo de entrada (dataset)
*/
void carregar_dados_LO(ItemLista **cabeca, const char *nome_arquivo){
    FILE *arquivo = fopen(nome_arquivo, "r");
    if(!arquivo){
        printf("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    char linha[256];
    
    fgets(linha, sizeof(linha), arquivo);
    
    while(fgets(linha, sizeof(linha), arquivo)){
        ItemLista novo;

        sscanf(linha, "%d;%d;%9[^;];%49[^;];%f;%f;%f;%f;%f", 
            &novo.id, &novo.ano, novo.estado, novo.cultura, &novo.preco_ton, 
            &novo.rendimento, &novo.producao, &novo.area_plantada, &novo.valor_total);

        insereOrdenadoID_LO(cabeca, novo);
    }

    fclose(arquivo);
}

/*
Busca e imprime elementos que atendem aos filtros de ano, estado e cultura.
Parâmetros:
  cabeca - ponteiro para a cabeça da lista
  ano_min, ano_max - intervalo de anos
  cultura - filtro de cultura (string)
  estado - filtro de estado (string)
*/
void buscarFiltros_LO(ItemLista *cabeca, int ano_min, int ano_max, const char *estado, const char *cultura){
    ItemLista *atual = cabeca;
    int encontrados = 0;

    printf("\n===RESULTADOS DA BUSCA===\n");

    while (atual != NULL){
        int ano_certo = (atual->ano >= ano_min) && (atual->ano <= ano_max);

        int estado_certo = (estado == NULL || strcasecmp(atual->estado, estado) == 0);

        int cultura_certa = (cultura == NULL || strcasecmp(atual->cultura, cultura) == 0);

        if(ano_certo && estado_certo && cultura_certa){
            printf("%d | %s | %s | %.2f | %.2f | %.2f | %.2f | %.2f\n",
             atual->ano, atual->estado, atual->cultura, atual->preco_ton, 
             atual->rendimento, atual->producao, atual->area_plantada, atual->valor_total);
             
             encontrados++;
        }
        atual = atual->prox;
    }

    if(encontrados == 0){
        printf("\nNenhum resultado encontrado");
    }
    else{
        printf("\nTotal de resultados %d\n", encontrados);
    }
}

/*
Cria uma nova amostra a partir da entrada do usuário e insere na lista ordenada.
Parâmetro: cabeca - ponteiro para o ponteiro da cabeça da lista
*/
void criar_amostra_LO(ItemLista **cabeca){
    ItemLista novo;
    char entrada[50];

    printf("\n===Insira os dados da nova amostra===\n");

    novo.id = criar_id_LO(*cabeca);

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

    printf("Produção: ");
    fgets(entrada, sizeof(entrada), stdin);
    sscanf(entrada, "%f", &novo.producao);

    printf("Area plantada: ");
    fgets(entrada, sizeof(entrada), stdin);
    sscanf(entrada, "%f", &novo.area_plantada);

    printf("Valor total: ");
    fgets(entrada, sizeof(entrada), stdin);
    sscanf(entrada, "%f", &novo.valor_total);

    novo.prox = NULL;

    insereOrdenadoID_LO(cabeca, novo);

    printf("\nAmostra inserida com sucesso\n");
}

/*
Salva todos os elementos da lista ordenada no dataset.
Parâmetros:
  cabeca - ponteiro para a cabeça da lista
  nome_arquivo - nome do arquivo de saída (dataset)
*/
void salvar_dados_LO(ItemLista *cabeca, const char *nome_arquivo){
    FILE *arquivo = fopen(nome_arquivo, "w");
    
    if(!arquivo){
        printf("Erro ao abrir o arquivo\n");
        exit(EXIT_FAILURE);
    }

    fprintf(arquivo, "ID;Data;Localizacao;Tipo de plantio;Preco por tonelada (Dolares/tonelada);Rendimento (kilogramas por hectare);Producao (toneladas);Area plantada (hectares);Valor total da safra (Dolares)\n");
    ItemLista *atual = cabeca;
    while(atual != NULL){
        fprintf(arquivo, "%d;%d;%s;%s;%.2f;%.2f;%.2f;%.2f;%.2f\n",
                atual->id, atual->ano, atual->estado, atual->cultura, atual->preco_ton, 
                atual->rendimento, atual->producao, atual->area_plantada, 
                atual->valor_total);
            
            atual = atual->prox;
    }
    fclose(arquivo);
}

/*
Função auxiliar para a criação de amostras. Retorna o maior ID presente na lista ordenada.
Parâmetro: cabeca - ponteiro para a cabeça da lista
Retorno: inteiro representando o maior ID
*/
int criar_id_LO(ItemLista *cabeca){
    int ultimo_id = 0;
    ItemLista *atual = cabeca;
    while(atual != NULL){
        if(atual->id > ultimo_id){
            ultimo_id = atual->id;
        }
        atual = atual->prox;
    }
    return ultimo_id;
}

/*
Busca um elemento pelo ID na lista encadeada.
Parâmetros:
  cabeca - ponteiro para a cabeça da lista
  id - identificador a ser buscado
Retorno: ponteiro para o item encontrado ou NULL
*/
ItemLista *buscarId_LO(ItemLista *cabeca, int id){
    ItemLista *atual = cabeca;
    while(atual != NULL){
        if(atual->id == id){
            return atual;
        }
        atual = atual->prox;
    }
    return NULL;
}

/*
Remove um elemento da lista ordenada pelo ID.
Parâmetros:
    cabeca - ponteiro para a cabeça da lista
    id - identificador do item a ser removido
*/
void remover_LO(ItemLista **cabeca, int id){
    if(*cabeca == NULL){
        printf("ID nao encontrado\n");
    }

    ItemLista *atual = *cabeca;
    ItemLista *anterior = NULL;

    if(atual->id == id){
        *cabeca = atual->prox;
        free(atual);
        return;
    }

    while(atual !=NULL && atual->id != id){
        anterior = atual;
        atual = atual->prox;    
    }
    
    if(atual == NULL){
        printf("Amostra de ID %d não encontrada\n", id);
        return;
    }

    anterior->prox  = atual->prox;
    free(atual);
}

/*
Mede o tempo de inserção de n elementos do dataset na lista ordenada.
Parâmetros:
  nome_arquivo - nome do arquivo de entrada (dataset)
  n - número de elementos a serem inseridos
Retorno: tempo gasto em segundos (double)
*/
double bench_temp_insercao_LO(const char *nome_arquivo, int n){
    struct timespec inicio, fim;

    FILE *arquivo = fopen(nome_arquivo, "r");
    if(!arquivo){
        printf("Erro ao abrir o arquivo\n");
        exit(EXIT_FAILURE);
    }

    char linha[256];
    fgets(linha, sizeof(linha), arquivo);

    ItemLista *cabeca = NULL;
    int total = 0;

    clock_gettime(CLOCK_MONOTONIC, &inicio);

    while(fgets(linha, sizeof(linha), arquivo) && total < n){
        ItemLista novo;
        sscanf(linha, "%d;%d;%9[^;];%49[^;];%f;%f;%f;%f;%f", 
            &novo.id, &novo.ano, novo.estado, novo.cultura, &novo.preco_ton, 
            &novo.rendimento, &novo.producao, &novo.area_plantada, &novo.valor_total);
        
        novo.prox = NULL;
        insereOrdenadoID_LO(&cabeca, novo);
        total++;
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);
    
    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    libera_LO(cabeca);
    fclose(arquivo);
    return tempo;
}

/*
Mede o tempo de remoção de n elementos do dataset na lista ordenada.
Parâmetros:
  nome_arquivo - nome do arquivo de entrada (dataset)
  n - número de elementos a serem removidos
Retorno: tempo gasto em segundos (double)
*/
double bench_temp_remocao_LO(const char *nome_arquivo, int n){
    struct timespec inicio, fim;

    ItemLista *cabeca = NULL;
    carregar_dados_LO(&cabeca, nome_arquivo);

    int *ids = (int *)(malloc(n*sizeof(int)));
    if(!ids){
        printf("Falha ao alocar memoria\n");
        libera_LO(cabeca);
        exit(EXIT_FAILURE);
    }

    ItemLista *atual = cabeca;
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
    for(k; k<total_ids; k++){
        remover_LO(&cabeca, ids[k]);
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);
    
    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    free(ids);
    libera_LO(cabeca);

    return tempo;

}

/*
Mede o tempo de busca de n elementos do dataset na lista ordenada.
Parâmetros:
  nome_arquivo - nome do arquivo de entrada (dataset)
  n - número de elementos a serem removidos
Retorno: tempo gasto em segundos (double)
*/
double bench_temp_busca_LO(const char *nome_arquivo, int n){
    struct timespec inicio, fim;

    ItemLista *cabeca = NULL;
    carregar_dados_LO(&cabeca, nome_arquivo);

    int maior_id = 0;
    ItemLista *atual = cabeca;

    while(atual != NULL){
        if(atual->id > maior_id){
            maior_id = atual->id;
        }
        atual = atual->prox;
    }

    clock_gettime(CLOCK_MONOTONIC, &inicio);
    
    int i = 0;
    for(i; i<n; i++){
        int id_buscado = rand() % maior_id + 1;
        buscarId_LO(cabeca, id_buscado);
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);
    
    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;
    libera_LO(cabeca);
    return tempo;
}

/*
Mede o uso de memoria de n elementos do dataset na lista ordenada.
Parâmetros:
  nome_arquivo - nome do arquivo de entrada (dataset)
  n - número de elementos a serem inseridos
Retorno: memória utilizada (bytes)
*/
size_t bench_uso_memoria_LO(const char *nome_arquivo, int n){
    ItemLista *cabeca = NULL;
    FILE *arquivo = fopen(nome_arquivo, "r");
    if(!arquivo){
        printf("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    char linha[256];
    int total = 0;

    fgets(linha, sizeof(linha), arquivo);
    
    while(fgets(linha, sizeof(linha), arquivo) && total < n){
        ItemLista novo;

        sscanf(linha, "%d;%d;%9[^;];%49[^;];%f;%f;%f;%f;%f", 
            &novo.id, &novo.ano, novo.estado, novo.cultura, &novo.preco_ton, 
            &novo.rendimento, &novo.producao, &novo.area_plantada, &novo.valor_total);

        insereOrdenadoID_LO(&cabeca, novo);
    }

    fclose(arquivo);

    size_t cont = 0;
    ItemLista *atual = cabeca;
    while(atual != NULL){
        cont++;
        atual = atual->prox;
    }

    size_t uso_memoria = cont * sizeof(ItemLista);
    libera_LO(cabeca);
    return uso_memoria;
}

/*
Mede o tempo de inserção com memória restrita de n elementos do dataset na lista ordenada.
Parâmetros:
  nome_arquivo - nome do arquivo de entrada (dataset)
  lim_memoria - limite de memória a ser utilizada
Retorno: tempo gasto em segundos (double)
*/
double bench_insercao_mem_restrita_LO(const char *nome_arquivo, double lim_memoria){
    struct timespec inicio, fim;

    size_t lim_bytes = (size_t)(lim_memoria * 1024 * 1024);
    FILE *arquivo = fopen(nome_arquivo, "r");
    if(!arquivo){
        printf("Erro ao abrir o arquivo\n");
        exit(EXIT_FAILURE);
    }

    char linha[256];
    fgets(linha, sizeof(linha), arquivo);

    ItemLista *cabeca = NULL;
    int total = 0;
    size_t memoria_usada = 0;

    clock_gettime(CLOCK_MONOTONIC, &inicio);

    while(fgets(linha, sizeof(linha), arquivo)){
        if(memoria_usada + sizeof(ItemLista) > lim_bytes){
            printf("\nLimite de memoria atingido apos %d elementos\n", total);
            break;
        }
        ItemLista novo;
        sscanf(linha, "%d;%d;%9[^;];%49[^;];%f;%f;%f;%f;%f", 
            &novo.id, &novo.ano, novo.estado, novo.cultura, &novo.preco_ton, 
            &novo.rendimento, &novo.producao, &novo.area_plantada, &novo.valor_total);
        
        novo.prox = NULL;
        insereOrdenadoID_LO(&cabeca, novo);
        memoria_usada += sizeof(ItemLista);  
        total++;
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);
    
    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    libera_LO(cabeca);
    fclose(arquivo);
    return tempo;

}

/*
Mede o tempo de inserção com delay de n elementos do dataset na lista ordenada.
Parâmetros:
  nome_arquivo - nome do arquivo de entrada (dataset)
  delay_ms - tempo em milisegundos a ser aplicado como delay
Retorno: tempo gasto em segundos (double)
*/
double bench_insercao_com_delay_LO(const char *nome_arquivo, int n, int delay_ms){
    struct timespec inicio, fim;

    FILE *arquivo = fopen(nome_arquivo, "r");
    if(!arquivo){
        printf("Erro ao abrir o arquivo\n");
        exit(EXIT_FAILURE);
    }

    char linha[256];
    fgets(linha, sizeof(linha), arquivo);

    ItemLista *cabeca = NULL;
    int total = 0;

    clock_gettime(CLOCK_MONOTONIC, &inicio);

    while(fgets(linha, sizeof(linha), arquivo) && total < n){
        ItemLista novo;
        sscanf(linha, "%d;%d;%9[^;];%49[^;];%f;%f;%f;%f;%f", 
            &novo.id, &novo.ano, novo.estado, novo.cultura, &novo.preco_ton, 
            &novo.rendimento, &novo.producao, &novo.area_plantada, &novo.valor_total);
        
        novo.prox = NULL;
        insereOrdenadoID_LO(&cabeca, novo);
        Sleep(delay_ms);
        total++;
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);
    
    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    libera_LO(cabeca);
    fclose(arquivo);
    return tempo;
}

/*
Mede o tempo de busca com latência de n elementos do dataset na lista ordenada.
Parâmetros:
  nome_arquivo - nome do arquivo de entrada (dataset)
  n - número de elementos a serem buscados
Retorno: tempo gasto em segundos (double)
*/
double bench_busca_latencia_LO(const char *nome_arquivo, int n){
    struct timespec inicio, fim;

    ItemLista *cabeca = NULL;
    carregar_dados_LO(&cabeca, nome_arquivo);
    int maior_id = 0;
    ItemLista *atual = cabeca;
    while(atual != NULL){
        if(atual->id > maior_id){
            maior_id = atual->id;
        }
        atual = atual->prox;
    }

    srand(time(NULL));
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    int i = 0;
    for(i; i < n; i++){
        int id_buscado = rand() % maior_id + 1;
        buscarId_LO(cabeca, id_buscado);
        int delay = 2 + rand() % 8;
        Sleep(delay);
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);
    
    libera_LO(cabeca);
    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;
    return tempo;
}

/*
Mede o tempo de isnerção de n elementos do dataset, com perda, na lista ordenada.
Parâmetros:
  nome_arquivo - nome do arquivo de entrada (dataset)
  n - número de elementos a serem inseridos
Retorno: tempo gasto em segundos (double)
*/
double bench_temp_insercao_perda_LO(const char *nome_arquivo, int n){
    struct timespec inicio, fim;

    FILE *arquivo = fopen(nome_arquivo, "r");
    if(!arquivo){
        printf("Erro ao abrir o arquivo\n");
        exit(EXIT_FAILURE);
    }

    char linha[256];
    fgets(linha, sizeof(linha), arquivo);

    ItemLista *cabeca = NULL;
    int total = 0;

    srand(time(NULL));
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    while(fgets(linha, sizeof(linha), arquivo) && total < n){
        if((rand() % 100) < 20){
            continue;
        }
        
        ItemLista novo;
        sscanf(linha, "%d;%d;%9[^;];%49[^;];%f;%f;%f;%f;%f", 
            &novo.id, &novo.ano, novo.estado, novo.cultura, &novo.preco_ton, 
            &novo.rendimento, &novo.producao, &novo.area_plantada, &novo.valor_total);
        
        novo.prox = NULL;
        insereOrdenadoID_LO(&cabeca, novo);
        total++;
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);
    
    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    libera_LO(cabeca);
    fclose(arquivo);
    return tempo;
}

/*
Mede o tempo de busca de n elementos do dataset na lista ordenada.
Parâmetros:
  nome_arquivo - nome do arquivo de entrada (dataset)
  n - número de elementos a serem buscados
  lim - limite de acessos
Retorno: tempo gasto em segundos (double)
*/
double bench_busca_lim_acessos_LO(const char *nome_arquivo, int n, int lim){
    struct timespec inicio, fim;
    
    ItemLista *cabeca = NULL;
    carregar_dados_LO(&cabeca, nome_arquivo);

    int maior_id = 0;
    ItemLista *atual = cabeca;
    
    while(atual != NULL){
        if(atual->id > maior_id){
            maior_id = atual->id;
            atual = atual->prox;
        }
    }

    srand(time(NULL));
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    
    int i = 0;
    for(i; i < n; i++){
        int id_buscado = rand() % maior_id + 1;

        ItemLista *ptr = cabeca;
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
    
    libera_LO(cabeca);
    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;
    return tempo;
}
