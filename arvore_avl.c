/*
->arvore_avl.c
Implementação de uma árvore AVL para manipulação de amostras agrícolas.
Este arquivo contém as funções para inserir, remover, buscar, filtrar, imprimir 
e carregar dados em uma árvore AVL, além de funções auxiliares
e do conjunto de nove funções para benchmark.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <windows.h>
#include "arvore_avl.h"

/*
Calcula a altura de um nó da árvore AVL.
Parâmetro: no - ponteiro para o nó
Retorno: altura do nó (int)
*/
int altura(ItemAVL *no){
    if(no == NULL) return 0;
    return no->altura;
}

/*
Retorna o maior valor entre dois inteiros.
Parâmetros:
    a - inteiro
    b - inteiro
Retorno: maior valor entre a e b
*/
int maximo(int a, int b){
    if(a > b){
        return a;
    }
    return b;
}

/*
Realiza a rotação à direita em um nó da árvore AVL.
Parâmetro: y - ponteiro para o nó a ser rotacionado
Retorno: ponteiro para a nova raiz após a rotação
*/
ItemAVL* rotacao_direita(ItemAVL *y){
    ItemAVL *x = y->esq;
    ItemAVL *T2 = x->dir;

    x->dir = y;
    y->esq = T2;
    
    y->altura = maximo(altura(y->esq), altura(y->dir)) + 1;
    x->altura = maximo(altura(x->esq), altura(x->dir)) + 1;
    
    return x;
}

/*
Realiza a rotação à esquerda em um nó da árvore AVL.
Parâmetro: x - ponteiro para o nó a ser rotacionado
Retorno: ponteiro para a nova raiz após a rotação
*/
ItemAVL* rotacao_esquerda(ItemAVL *x){
    ItemAVL *y = x->dir;
    ItemAVL *T2 = y->esq;

    y->esq = x;
    x->dir = T2;
    
    x->altura = maximo(altura(x->esq), altura(x->dir)) + 1;
    y->altura = maximo(altura(y->esq), altura(y->dir)) + 1;
    
    return y;
}

/*
Calcula o fator de balanceamento de um nó da árvore AVL.
Parâmetro: no - ponteiro para o nó
Retorno: fator de balanceamento (int)
*/
int fator_balanceamento(ItemAVL *no){
    if(no == NULL){
        return 0;
    }

    int fator = altura(no->esq) - altura(no->dir);
    
    return fator;
}

/*
Cria um novo nó para a árvore AVL a partir de uma estrutura fornecida.
Parâmetro: novo - estrutura com os dados a serem inseridos
Retorno: ponteiro para o novo nó criado
*/
ItemAVL* novo_no(ItemAVL novo){
    ItemAVL *no = (ItemAVL*)malloc(sizeof(ItemAVL));
    if(!no){
        printf("Erro ao alocar memoria\n");
        exit(EXIT_FAILURE);
    }

    *no = novo;
    no->esq = NULL;
    no->dir = NULL;
    no->altura = 1;

    return no;
}

/*
Insere um novo elemento na árvore AVL.
Parâmetros:
    raiz - ponteiro para a raiz da árvore
    novo - estrutura com os dados a serem inseridos
Retorno: ponteiro para a nova raiz da árvore
*/
ItemAVL* inserir_avl(ItemAVL *raiz, ItemAVL novo){
    if(raiz == NULL) return novo_no(novo);

    if(novo.id < raiz->id){
        raiz->esq = inserir_avl(raiz->esq, novo);
    }else if(novo.id > raiz->id){
        raiz->dir = inserir_avl(raiz->dir, novo);
    }else{
        return raiz;
    }

    raiz->altura = 1 + maximo(altura(raiz->esq), altura(raiz->dir));
    int balanceamento = fator_balanceamento(raiz);

    if(balanceamento > 1 && novo.id < raiz->esq->id){
        return rotacao_direita(raiz);
    }
    if(balanceamento < -1 && novo.id > raiz->dir->id){
        return rotacao_esquerda(raiz);
    }
    if(balanceamento > 1 && novo.id > raiz->esq->id){
        raiz->esq = rotacao_esquerda(raiz->esq);
        return rotacao_direita(raiz);
    }
    if(balanceamento < -1 && novo.id < raiz->dir->id){
        raiz->dir = rotacao_direita(raiz->dir);
        return rotacao_esquerda(raiz);
    }

    return raiz;
}

/*
Encontra o nó de menor valor em uma subárvore AVL.
Parâmetro: no - ponteiro para o nó raiz da subárvore
Retorno: ponteiro para o nó de menor valor
*/
ItemAVL* min_valor_no(ItemAVL* no){
    ItemAVL* atual = no;

    while(atual->esq != NULL){
        atual = atual->esq;
    }

    return atual;
}

/*
Remove um elemento da árvore AVL pelo ID.
Parâmetros:
    raiz - ponteiro para a raiz da árvore
    id - identificador do elemento a ser removido
Retorno: ponteiro para a nova raiz da árvore
*/
ItemAVL* remover_avl(ItemAVL* raiz, int id){
    if(raiz==NULL){
        return raiz;
    }
    if(id < raiz->id){
        raiz->esq = remover_avl(raiz->esq, id);
    }
    else if(id > raiz->id){
        raiz->dir = remover_avl(raiz->dir, id);
    }
    else{
        if((raiz->esq == NULL)||(raiz->dir == NULL)){
            ItemAVL *temp = raiz->esq ? raiz->esq : raiz->dir;
            if(temp == NULL){
                temp = raiz;
                raiz = NULL;
            }else
                *raiz = *temp;
            free(temp);
        }else{
            ItemAVL* temp = min_valor_no(raiz->dir);
            raiz->id = temp->id;
            raiz->ano = temp->ano;
            strcpy(raiz->estado, temp->estado);
            strcpy(raiz->cultura, temp->cultura);
            raiz->preco_ton = temp->preco_ton;
            raiz->rendimento = temp->rendimento;
            raiz->producao = temp->producao;
            raiz->area_plantada = temp->area_plantada;
            raiz->valor_total = temp->valor_total;
            raiz->dir = remover_avl(raiz->dir, temp->id);
        }
    }

    if(raiz==NULL){
        return raiz;
    }

    raiz->altura = 1 + maximo(altura(raiz->esq), altura(raiz->dir));
    int balanceamento = fator_balanceamento(raiz);

    if(balanceamento > 1 && fator_balanceamento(raiz->esq) >= 0)
        return rotacao_direita(raiz);
    if(balanceamento > 1 && fator_balanceamento(raiz->esq) < 0){
        raiz->esq = rotacao_esquerda(raiz->esq);
        return rotacao_direita(raiz);
    }

    if(balanceamento < -1 && fator_balanceamento(raiz->dir) <= 0)
        return rotacao_esquerda(raiz);
    if(balanceamento < -1 && fator_balanceamento(raiz->dir) > 0){
        raiz->dir = rotacao_direita(raiz->dir);
        return rotacao_esquerda(raiz);
    }

    return raiz;
}

/*
Busca um elemento pelo ID na árvore AVL.
Parâmetros:
    raiz - ponteiro para a raiz da árvore
    id - identificador a ser buscado
Retorno: ponteiro para o item encontrado ou NULL
*/
ItemAVL* buscar_avl(ItemAVL* raiz, int id){
    if(raiz == NULL || raiz->id == id){
        return raiz;
    }
    if(id < raiz->id){
        return buscar_avl(raiz->esq, id);
    }else{
        return buscar_avl(raiz->dir, id);
    }
}

/*
Libera toda a memória alocada pela árvore AVL.
Parâmetro: raiz - ponteiro para a raiz da árvore
*/
void liberar_avl(ItemAVL *raiz){
    if(raiz == NULL){
        return;
    }
    liberar_avl(raiz->esq);
    liberar_avl(raiz->dir);
    free(raiz);
}

/*
Imprime todos os elementos da árvore AVL em ordem crescente de ID.
Parâmetro: raiz - ponteiro para a raiz da árvore
*/
void imprimir_avl(ItemAVL *raiz){
    if(raiz == NULL){
        return;
    }

    imprimir_avl(raiz->esq);
    printf("%d | %d | %s | %s | %.2f | %.2f | %.2f | %.2f | %.2f\n",
        raiz->id, raiz->ano, raiz->estado, raiz->cultura, raiz->preco_ton,
        raiz->rendimento, raiz->producao, raiz->area_plantada, raiz->valor_total);
    imprimir_avl(raiz->dir);
}

/*
Carrega dados de um arquivo e insere na árvore AVL.
Parâmetros:
    raiz - ponteiro para o ponteiro da raiz da árvore
    nome_arquivo - nome do arquivo de entrada (dataset)
*/
void carregar_dados_avl(ItemAVL **raiz, const char *nome_arquivo){
    FILE *arquivo = fopen(nome_arquivo, "r");
    if(!arquivo){
        printf("Erro ao abrir o arquivo\n");
        exit(EXIT_FAILURE);
    }

    char linha[256];
    fgets(linha, sizeof(linha), arquivo);

    while(fgets(linha, sizeof(linha), arquivo)){
        ItemAVL novo;
        sscanf(linha, "%d;%d;%9[^;];%49[^;];%f;%f;%f;%f;%f",
            &novo.id, &novo.ano, novo.estado, novo.cultura, &novo.preco_ton,
            &novo.rendimento, &novo.producao, &novo.area_plantada, &novo.valor_total);

        novo.esq = NULL;
        novo.dir = NULL;
        novo.altura = 1;
        *raiz = inserir_avl(*raiz, novo);
    }
    fclose(arquivo);
}

/*
Salva todos os elementos da árvore AVL em um arquivo (dataset). Usada internamente
para gravar os dados com o arquivo ja aberto.
Parâmetros:
    raiz - ponteiro para a raiz da árvore
    nome_arquivo - nome do arquivo de saída
*/
void salvar_aux_avl(ItemAVL *no, FILE *arquivo){
    if(no == NULL){
        return;
    }

    salvar_aux_avl(no->esq, arquivo);

    fprintf(arquivo, "%d;%d;%s;%s;%.2f;%.2f;%.2f;%.2f;%.2f\n",
        no->id, no->ano, no->estado, no->cultura, no->preco_ton,
        no->rendimento, no->producao, no->area_plantada, no->valor_total);

    salvar_aux_avl(no->dir, arquivo);
}

/*
Salva todos os elementos da árvore AVL em um arquivo (dataset). Abre e fecha o arquivo
utilizando a função auxiliar para gravar os dados.
Parâmetros:
    raiz - ponteiro para a raiz da árvore
    nome_arquivo - nome do arquivo de saída (dataset)
*/
void salvar_dados_avl(ItemAVL *raiz, const char *nome_arquivo){
    FILE *arquivo = fopen(nome_arquivo, "w");
    if(!arquivo){
        printf("Erro ao abrir o arquivo\n");
        exit(EXIT_FAILURE);
    }

    fprintf(arquivo, "ID;Data;Localizacao;Tipo de plantio;Preco por tonelada (Dolares/tonelada);Rendimento (kilogramas por hectare);Producao (toneladas);Area plantada (hectares);Valor total da safra (Dolares)\n");
    salvar_aux_avl(raiz, arquivo);
    fclose(arquivo);
}

/*
Busca o maior ID presente na árvore AVL.
Parâmetros:
    no - ponteiro para a raiz da árvore
    maior - ponteiro para a variável que armazena o maior ID encontrado
*/
void busca_maior_id_avl(ItemAVL *no, int *maior){
    if(no == NULL){
        return;
    }
    if(no->id > *maior){
        *maior = no->id;
    }

    busca_maior_id_avl(no->esq, maior);
    busca_maior_id_avl(no->dir, maior);
}

/*
Retorna o próximo ID disponível na árvore AVL.
Parâmetro: raiz - ponteiro para a raiz da árvore
Retorno: inteiro representando o próximo ID
*/
int proximo_id_avl(ItemAVL *raiz){
    int maior = 0;
    busca_maior_id_avl(raiz, &maior);
    return maior + 1;
}

/*
Cria uma nova amostra a partir da entrada do usuário e insere na árvore AVL.
Parâmetro: raiz - ponteiro para o ponteiro da raiz da árvore
*/
void criar_amostra_avl(ItemAVL **raiz){
    ItemAVL novo;
    char entrada[50];
    novo.id = proximo_id_avl(*raiz);

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
    
    novo.esq = NULL;
    novo.dir = NULL;
    novo.altura = 1;
    *raiz = inserir_avl(*raiz, novo);
    printf("\nAmostra inserida com sucesso\n");
}

/*
Busca e imprime elementos que atendem aos filtros de ano, estado e cultura. Usada
internamente para comparar os parâmetros da busca com as amostras.
Parâmetros:
  cabeca - ponteiro para a cabeça da lista
  ano_min, ano_max - intervalo de anos
  cultura - filtro de cultura (string)
  estado - filtro de estado (string)
*/
void buscar_filtros_aux_avl(ItemAVL *no, int ano_min, int ano_max, const char *estado, const char *cultura, int *encontrados){
    if(no == NULL){
        return;
    }

    int ano_certo = (no->ano >= ano_min) && (no->ano <= ano_max);
    int estado_certo = (estado == NULL || strlen(estado) == 0 || strcasecmp(no->estado, estado) == 0);
    int cultura_certa = (cultura == NULL || strlen(cultura) == 0 || strcasecmp(no->cultura, cultura) == 0);

    if(ano_certo && estado_certo && cultura_certa){
        printf("%d | %d | %s | %s | %.2f | %.2f | %.2f | %.2f | %.2f\n",
            no->id, no->ano, no->estado, no->cultura, no->preco_ton,
            no->rendimento, no->producao, no->area_plantada, no->valor_total);
        (*encontrados)++;
    }
    buscar_filtros_aux_avl(no->esq, ano_min, ano_max, estado, cultura, encontrados);
    buscar_filtros_aux_avl(no->dir, ano_min, ano_max, estado, cultura, encontrados);
}

/*
Busca e imprime elementos que atendem aos filtros de ano, estado e cultura. Utiliza a 
função auxiliar para dar um panorama geral da busca.
Parâmetros:
  cabeca - ponteiro para a cabeça da lista
  ano_min, ano_max - intervalo de anos
  cultura - filtro de cultura (string)
  estado - filtro de estado (string)
*/
void buscar_filtros_avl(ItemAVL *raiz, int ano_min, int ano_max, const char *estado, const char *cultura){
    int encontrados = 0;

    printf("\n===RESULTADOS DA BUSCA===\n");

    buscar_filtros_aux_avl(raiz, ano_min, ano_max, estado, cultura, &encontrados);
    if(encontrados==0){
        printf("Nenhum resultado encontrado.\n");
    }else{
        printf("\nTotal de resultados: %d\n", encontrados);
    }
}

/*
Mede o tempo de inserção de n elementos na árvore AVL a partir de um arquivo.
Parâmetros:
    nome_arquivo - nome do arquivo de entrada (dataset)
    n - número de elementos a serem inseridos
Retorno: tempo gasto em segundos (double)
*/
double bench_tempo_insercao_avl(const char *nome_arquivo, int n){
    struct timespec inicio, fim;
    FILE *arquivo = fopen(nome_arquivo, "r");
    if(!arquivo){
        printf("Erro ao abrir o arquivo\n");
        exit(EXIT_FAILURE);
    }

    char linha[256];
    fgets(linha, sizeof(linha), arquivo);

    ItemAVL *raiz = NULL;
    int total = 0;
    
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    
    while(fgets(linha, sizeof(linha), arquivo) && total < n){
        ItemAVL novo;
        sscanf(linha, "%d;%d;%9[^;];%49[^;];%f;%f;%f;%f;%f",
            &novo.id, &novo.ano, novo.estado, novo.cultura, &novo.preco_ton,
            &novo.rendimento, &novo.producao, &novo.area_plantada, &novo.valor_total);

        novo.esq = NULL;
        novo.dir = NULL;
        novo.altura = 1;
        raiz = inserir_avl(raiz, novo);
        total++;
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);
    
    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;
    
    liberar_avl(raiz);
    fclose(arquivo);
    return tempo;
}

/*
Função auxiliar utilizada para coletar todos os IDs presentes na estrutura
Parâmetros:
    no - ponteiro para o nó atual da árvore AVL
    ids - vetor de inteiros onde os IDs serão armazenados
    total - ponteiro para o contador de IDs já coletados (atualizado pela função)
    limite - número máximo de IDs a serem coletados
*/ 
void coletar_ids_avl(ItemAVL *no, int *ids, int *total, int limite){
    if(no == NULL || *total >= limite){
        return;
    }

    ids[*total] = no->id;
    (*total)++;

    coletar_ids_avl(no->esq, ids, total, limite);
    coletar_ids_avl(no->dir, ids, total, limite);
}

/*
Mede o tempo de remoção de n elementos da árvore AVL.
Parâmetros:
    nome_arquivo - nome do arquivo de entrada (daataset)
    n - número de elementos a serem removidos
Retorno: tempo gasto em segundos (double)
*/
double bench_tempo_remocao_avl(const char *nome_arquivo, int n){
    struct timespec inicio, fim;
    ItemAVL *raiz = NULL;
    carregar_dados_avl(&raiz, nome_arquivo);

    int *ids = (int*)malloc(n * sizeof(int));
    if(!ids){
        printf("Erro ao alocar memoria para ids\n");
        liberar_avl(raiz);
        return -1;
    }

    int total_ids = 0;
    coletar_ids_avl(raiz, ids, &total_ids, n);

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
    for(k; k < total_ids; k++){
        raiz = remover_avl(raiz, ids[k]);
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);

    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    free(ids);
    liberar_avl(raiz);

    return tempo;
}

/*
Mede o tempo de busca de n elementos na árvore AVL.
Parâmetros:
    nome_arquivo - nome do arquivo de entrada (dataset)
    n - número de buscas a serem realizadas
Retorno: tempo gasto em segundos (double)
*/
double bench_tempo_busca_avl(const char *nome_arquivo, int n){
    struct timespec inicio, fim;
    ItemAVL *raiz = NULL;
    carregar_dados_avl(&raiz, nome_arquivo);
    int *ids = (int*)malloc(n * sizeof(int));
    if(!ids){
        printf("Erro ao alocar memoria para ids\n");
        liberar_avl(raiz);
        return -1;
    }

    int total_ids = 0;
    coletar_ids_avl(raiz, ids, &total_ids, n);

    if(total_ids == 0){
        printf("Nenhum id encontrado\n");
        free(ids);
        liberar_avl(raiz);
        return 0;
    }

    srand((unsigned int)time(NULL));
    int i = total_ids - 1;
    for(i; i > 0; i--){
        int j = rand() % (i + 1);
        int temp = ids[i];
        ids[i] = ids[j];
        ids[j] = temp;
    }

    clock_gettime(CLOCK_MONOTONIC, &inicio);

    int k = 0;
    int num_buscas = n < total_ids ? n : total_ids;
    for(k; k < num_buscas; k++){
        buscar_avl(raiz, ids[k]);
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);

    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    free(ids);
    liberar_avl(raiz);

    return tempo;
}

/*
Percorre recursivamente a árvore AVL e conta o número total de nós.
Parâmetros:
    no - ponteiro para o nó atual da árvore AVL
    cont - ponteiro para o contador de nós (atualizado pela função)
*/
void conta_nos(ItemAVL *no, int *cont){
    if(no == NULL){
        return;
    }

    (*cont)++;
    conta_nos(no->esq, cont);
    conta_nos(no->dir, cont);
}

/*
Calcula o uso de memória da árvore AVL após inserir n elementos.
Parâmetros:
    nome_arquivo - nome do arquivo de entrada (dataset)
    n - número de elementos a serem considerados
Retorno: memória utilizada (bytes)
*/
size_t bench_uso_memoria_avl(const char *nome_arquivo, int n){
    ItemAVL *raiz = NULL;
    FILE *arquivo = fopen(nome_arquivo, "r");
    if(!arquivo){
        printf("Erro ao abrir o arquivo\n");
        exit(EXIT_FAILURE);
    }

    char linha[256];
    fgets(linha, sizeof(linha), arquivo);
    int total = 0;

    while(fgets(linha, sizeof(linha), arquivo) && total < n){
        ItemAVL novo;

        sscanf(linha, "%d;%d;%9[^;];%49[^;];%f;%f;%f;%f;%f",
            &novo.id, &novo.ano, novo.estado, novo.cultura, &novo.preco_ton,
            &novo.rendimento, &novo.producao, &novo.area_plantada, &novo.valor_total);

        novo.esq = NULL;
        novo.dir = NULL;
        novo.altura = 1;
        raiz = inserir_avl(raiz, novo);
        total++;
    }

    fclose(arquivo);

    int cont = 0;
    conta_nos(raiz, &cont);
    size_t uso_memoria = cont * sizeof(ItemAVL);

    liberar_avl(raiz);
    return uso_memoria;
}

/*
Mede o tempo de inserção com memória restrita na árvore AVL.
Parâmetros:
    nome_arquivo - nome do arquivo de entrada (dataset)
    lim_memoria_mb - limite de memória em MB
Retorno: tempo gasto em segundos (double)
*/
double bench_insercao_mem_restrita_avl(const char *nome_arquivo, double lim_memoria_mb){
    struct timespec inicio, fim;
    size_t lim_bytes = (size_t)(lim_memoria_mb * 1024 * 1024);

    FILE *arquivo = fopen(nome_arquivo, "r");
    if(!arquivo){
        printf("Erro ao abrir o arquivo\n");
        exit(EXIT_FAILURE);
    }

    char linha[256];
    fgets(linha, sizeof(linha), arquivo);

    ItemAVL *raiz = NULL;
    size_t memoria_usada = 0;
    int total = 0;

    clock_gettime(CLOCK_MONOTONIC, &inicio);

    while(fgets(linha, sizeof(linha), arquivo)){
        if(memoria_usada + sizeof(ItemAVL) > lim_bytes){
            printf("\nLimite de memoria atingido apos %d elementos\n", total);
            break;
        }

        ItemAVL novo;
        sscanf(linha, "%d;%d;%9[^;];%49[^;];%f;%f;%f;%f;%f",
            &novo.id, &novo.ano, novo.estado, novo.cultura, &novo.preco_ton,
            &novo.rendimento, &novo.producao, &novo.area_plantada, &novo.valor_total);

        novo.esq = NULL;
        novo.dir = NULL;
        novo.altura = 1;
        raiz = inserir_avl(raiz, novo);
        memoria_usada += sizeof(ItemAVL);
        total++;
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);

    fclose(arquivo);

    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    liberar_avl(raiz);

    return tempo;
}

/*
Mede o tempo de inserção de n elementos na árvore AVL com delay entre inserções.
Parâmetros:
    nome_arquivo - nome do arquivo de entrada (dataset)
    n - número de elementos a serem inseridos
    delay_ms - tempo de delay em milissegundos
Retorno: tempo gasto em segundos (double)
*/
double bench_insercao_com_delay_avl(const char *nome_arquivo, int n, int delay_ms){
    struct timespec inicio, fim;

    FILE *arquivo = fopen(nome_arquivo, "r");
    if(!arquivo){
        printf("Erro ao abrir o arquivo\n");
        exit(EXIT_FAILURE);
    }

    char linha[256];
    fgets(linha, sizeof(linha), arquivo);

    ItemAVL *raiz = NULL;
    int total = 0;

    clock_gettime(CLOCK_MONOTONIC, &inicio);

    while(fgets(linha, sizeof(linha), arquivo) && total < n){
        ItemAVL novo;
        sscanf(linha, "%d;%d;%9[^;];%49[^;];%f;%f;%f;%f;%f",
            &novo.id, &novo.ano, novo.estado, novo.cultura, &novo.preco_ton,
            &novo.rendimento, &novo.producao, &novo.area_plantada, &novo.valor_total);

        novo.esq = NULL;
        novo.dir = NULL;
        novo.altura = 1;
        raiz = inserir_avl(raiz, novo);

        Sleep(delay_ms);
        total++;
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);

    fclose(arquivo);

    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    liberar_avl(raiz);

    return tempo;
}

/*
Mede o tempo de busca de n elementos na árvore AVL com latência simulada.
Parâmetros:
    nome_arquivo - nome do arquivo de entrada (dataset)
    n - número de buscas a serem realizadas
Retorno: tempo gasto em segundos (double)
*/
double bench_busca_latencia_avl(const char *nome_arquivo, int n){
    struct timespec inicio, fim;

    ItemAVL *raiz = NULL;
    carregar_dados_avl(&raiz, nome_arquivo);

    int *ids = (int*)malloc(n * sizeof(int));
    if(!ids){
        printf("Erro ao alocar memoria para ids\n");
        liberar_avl(raiz);
        return -1;
    }

    int coletados = 0;

    coletar_ids_avl(raiz, ids, &coletados, n);

    srand(time(NULL));
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    int i = 0;
    for(i; i < n; i++){
        int indice = rand() % coletados;
        int id_buscado = ids[indice];
        buscar_avl(raiz, id_buscado);

        int delay = 2 + rand() % 8;
        Sleep(delay);
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);

    free(ids);
    liberar_avl(raiz);

    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    return tempo;
}

/*
Mede o tempo de inserção de n elementos na árvore AVL com perda simulada de dados.
Parâmetros:
    nome_arquivo - nome do arquivo de entrada
    n - número de elementos a serem inseridos
Retorno: tempo gasto em segundos (double)
*/
double bench_tempo_insercao_perda_avl(const char *nome_arquivo, int n){
    struct timespec inicio, fim;

    FILE *arquivo = fopen(nome_arquivo, "r");
    if(!arquivo){
        printf("Erro ao abrir o arquivo\n");
        exit(EXIT_FAILURE);
    }

    char linha[256];
    fgets(linha, sizeof(linha), arquivo);

    ItemAVL *raiz = NULL;
    int total = 0;

    srand(time(NULL));
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    while(fgets(linha, sizeof(linha), arquivo) && total < n){
        if((rand() % 100) < 20){
            continue;
        }

        ItemAVL novo;
        sscanf(linha, "%d;%d;%9[^;];%49[^;];%f;%f;%f;%f;%f",
            &novo.id, &novo.ano, novo.estado, novo.cultura, &novo.preco_ton,
            &novo.rendimento, &novo.producao, &novo.area_plantada, &novo.valor_total);

        novo.esq = NULL;
        novo.dir = NULL;
        novo.altura = 1;
        raiz = inserir_avl(raiz, novo);
        total++;
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);

    fclose(arquivo);

    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    liberar_avl(raiz);

    return tempo;
}

/*
Mede o tempo de busca de n elementos na árvore AVL com limite de acessos.
Parâmetros:
    nome_arquivo - nome do arquivo de entrada (dataset)
    n - número de buscas a serem realizadas
    lim - limite de acessos por busca
Retorno: tempo gasto em segundos (double)
*/
double bench_busca_lim_acessos_avl(const char *nome_arquivo, int n, int lim){
    struct timespec inicio, fim;

    ItemAVL *raiz = NULL;
    carregar_dados_avl(&raiz, nome_arquivo);

    int maior_id = 0;
    busca_maior_id_avl(raiz, &maior_id);

    srand(time(NULL));
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    int i = 0;
    for(i; i < n; i++){
        int id_buscado = rand() % maior_id + 1;

        ItemAVL *atual = raiz;
        int acessos = 0;
        while(atual != NULL && acessos < lim){
            if(atual->id == id_buscado){
                break;
            }
            if(id_buscado < atual->id){
                atual = atual->esq;
            }else{
                atual = atual->dir;
            }
            acessos++;
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);

    liberar_avl(raiz);

    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    return tempo;
}
