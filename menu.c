/*
->menu.c
Implementação do menu principal do sistema de manipulação de amostras agrícolas.
Este arquivo contém as funções que controlam a interface de interação do usuário, 
permitindo acessar as operações CRUD (inserção, busca, remoção, listagem e filtros) 
e o modo de benchmark para comparar o desempenho das diferentes estruturas de dados 
(Lista Encadeada, Lista Ordenada, Árvore AVL, Tabela Hash, Skip List e Trie).
Também inclui funções de sincronização e liberação de todas as estruturas, 
garantindo que os dados estejam sempre atualizados entre as diferentes representações.
*/

#include <stdio.h>
#include <stdlib.h>
#include "lista_ordenada.h"
#include "hash.h"
#include "skiplist.h"
#include "arvore_avl.h"
#include "trie.h"
#include "lista_encadeada.h"

TabelaHash tabela;
ItemLista *cabeca = NULL;
Skiplist* skiplist = NULL;
ItemAVL *raiz = NULL;
Trie *trie_estado = NULL;
Trie *trie_cultura = NULL;


char nome_arquivo[256];

/*
Exibe o menu de benchmarks, permitindo ao usuário escolher qual métrica deseja avaliar
(tempo de inserção, remoção, busca, uso de memória, inserção com restrição, etc.). 
Executa os benchmarks e exibe os resultados.
*/
void menu_benchmark(){
    int opcao;
    int n;
    do{
        printf("\n=== MENU BENCHMARK ===\n");
        printf("1 - Tempo de insercao\n");
        printf("2 - Tempo de remocao\n");
        printf("3 - Tempo de busca\n");
        printf("4 - Uso de memoria\n");
        printf("5 - Insercao com memoria restrita\n");
        printf("6 - Insercao com delay\n");
        printf("7 - Busca com latencia\n");
        printf("8 - Insercao com perda\n");
        printf("9 - Busca com limite de acessos\n");
        printf("0 - Voltar\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        if(opcao == 0) break;

        printf("\nDigite o numero de amostras para o benchmark: ");
        scanf("%d", &n);
        getchar();

        printf("\nResultados para %d amostras:\n", n);

        switch(opcao){
            case 0:{
                printf("Retornando ao menu principal\n");
                break;
            }
            case 1:{
                printf("Tempo de isercao (Lista Encadeada): %.8f segundos\n", bench_temp_insercao_LE(nome_arquivo, n));
                printf("Tempo de isercao (Lista Ordenada): %.8f segundos\n", bench_temp_insercao_LO(nome_arquivo, n));
                printf("Tempo de isercao (Arvore AVL): %.8f segundos\n", bench_tempo_insercao_avl(nome_arquivo, n));
                printf("Tempo de isercao (Hash): %.8f segundos\n", bench_tempo_insercao_hash(nome_arquivo, n));
                printf("Tempo de isercao (Skiplist): %.8f segundos\n", bench_temp_insercao_skiplist(nome_arquivo, n));
                break;
            }
            case 2:{
                printf("Tempo de remocao (Lista Encadeada): %.8f segundos\n", bench_temp_remocao_LE(nome_arquivo, n));
                printf("Tempo de remocao (Lista Ordenada): %.8f segundos\n", bench_temp_remocao_LO(nome_arquivo, n));
                printf("Tempo de remocao (Arvore AVL): %.8f segundos\n", bench_tempo_remocao_avl(nome_arquivo, n));
                printf("Tempo de remocao (Hash): %.8f segundos\n", bench_tempo_remocao_hash(nome_arquivo, n));
                printf("Tempo de remocao (Skiplist): %.8f segundos\n", bench_temp_remocao_skiplist(nome_arquivo, n));
                break;
            }
            case 3:{
                printf("Tempo de busca por ID (Lista Encadeada): %.8f segundos\n", bench_temp_busca_LE(nome_arquivo, n));
                printf("Tempo de busca por ID (Lista Ordenada): %.8f segundos\n", bench_temp_busca_LO(nome_arquivo, n));
                printf("Tempo de busca por ID (Arvore AVL): %.8f segundos\n", bench_tempo_busca_avl(nome_arquivo, n));
                printf("Tempo de busca por ID (Hash): %.8f segundos\n", bench_tempo_busca_hash(nome_arquivo, n));
                printf("Tempo de busca por ID (Skiplist): %.8f segundos\n", bench_temp_busca_skiplist(nome_arquivo, n));
                break;
            }
            case 4:{
                printf("Uso de memoria (Lista Encadeada): %zu bytes\n", bench_uso_memoria_LE(nome_arquivo, n));
                printf("Uso de memoria (Lista Ordenada): %zu bytes\n", bench_uso_memoria_LO(nome_arquivo, n));
                printf("Uso de memoria (Arvore AVL): %zu bytes\n", bench_uso_memoria_avl(nome_arquivo, n));
                printf("Uso de memoria (Hash): %zu bytes\n", bench_uso_memoria_hash(nome_arquivo, n));
                printf("Uso de memoria (Skiplist): %zu bytes\n", bench_uso_memoria_skiplist(nome_arquivo, n));
                break;
            }
            case 5:{
                int lim_memoria = 0;
                printf("Digite o limite de memoria em MB para o benchmark: ");
                scanf("%d", &lim_memoria);
                getchar();

                printf("\nTempo de insercao com memoria restrita (Lista Encadeada): %.8f segundos\n", bench_insercao_mem_restrita_LE(nome_arquivo, lim_memoria));
                printf("Tempo de insercao com memoria restrita (Lista Ordenada): %.8f segundos\n", bench_insercao_mem_restrita_LO(nome_arquivo, lim_memoria));
                printf("Tempo de insercao com memoria restrita (Arvore AVL): %.8f segundos\n", bench_insercao_mem_restrita_avl(nome_arquivo, lim_memoria));
                printf("Tempo de insercao com memoria restrita (Hash): %.8f segundos\n", bench_insercao_mem_restrita_hash(nome_arquivo, lim_memoria));
                printf("Tempo de insercao com memoria restrita (Skiplist): %.8f segundos\n", bench_insercao_mem_restrita_skiplist(nome_arquivo, lim_memoria));
                break;
            }            
            case 6:{
                int delay = 0;
                printf("Digite o delay em MS para o benchmark: ");
                scanf("%d", &delay);
                getchar();

                printf("\nTempo de insercao com delay (Lista Encadeada): %.8f segundos\n", bench_insercao_com_delay_LE(nome_arquivo, n, delay));
                printf("Tempo de insercao com delay (Lista Ordenada): %.8f segundos\n", bench_insercao_com_delay_LO(nome_arquivo, n, delay));
                printf("Tempo de insercao com delay (Arvore AVL): %.8f segundos\n", bench_insercao_com_delay_avl(nome_arquivo, n, delay));
                printf("Tempo de insercao com delay (Hash): %.8f segundos\n", bench_insercao_com_delay_hash(nome_arquivo, n, delay));
                printf("Tempo de insercao com delay (Skiplist): %.8f segundos\n", bench_insercao_com_delay_hash(nome_arquivo, n, delay));
                break;
            }
            case 7:{
                printf("Tempo de busca com latencia (Lista Encadeada): %.8f segundos\n", bench_busca_latencia_LE(nome_arquivo, n));
                printf("Tempo de busca com latencia (Lista Ordenada): %.8f segundos\n", bench_busca_latencia_LO(nome_arquivo, n));
                printf("Tempo de busca com latencia (Arvore AVL): %.8f segundos\n", bench_busca_latencia_hash(nome_arquivo, n));
                printf("Tempo de busca com latencia (Hash): %.8f segundos\n", bench_busca_latencia_hash(nome_arquivo, n));
                printf("Tempo de busca com latencia (Skiplist): %.8f segundos\n", bench_busca_latencia_skiplist(nome_arquivo, n));
                break;
            }
            case 8:{
                printf("Tempo de insercao com perda (Lista Encadeada): %.8f segundos\n", bench_temp_insercao_perda_LE(nome_arquivo, n));
                printf("Tempo de insercao com perda (Lista Ordenada): %.8f segundos\n", bench_temp_insercao_perda_LO(nome_arquivo, n));
                printf("Tempo de insercao com perda (Arvore AVL): %.8f segundos\n", bench_tempo_insercao_perda_avl(nome_arquivo, n));
                printf("Tempo de insercao com perda (Hash): %.8f segundos\n", bench_tempo_insercao_perda_hash(nome_arquivo, n));
                printf("Tempo de insercao com perda (Skiplist): %.8f segundos\n", bench_temp_insercao_perda_skiplist(nome_arquivo, n));
                break;
            }
            case 9:{
                int lim = 0;
                printf("Digite o limite de acessos para o benchmark: ");
                scanf("%d", &lim);
                getchar();

                printf("Tempo de busca com limite de acessos (Lista Encadeada): %.8f segundos\n", bench_busca_lim_acessos_LE(nome_arquivo, n, lim));
                printf("Tempo de busca com limite de acessos (Lista Ordenada): %.8f segundos\n", bench_busca_lim_acessos_LO(nome_arquivo, n, lim));
                printf("Tempo de busca com limite de acessos (Arvore AVL): %.8f segundos\n", bench_busca_lim_acessos_avl(nome_arquivo, n, lim));
                printf("Tempo de busca com limite de acessos (Hash): %.8f segundos\n", bench_busca_lim_acessos_hash(nome_arquivo, n, lim));
                printf("Tempo de busca com limite de acessos (Skiplist): %.8f segundos\n", bench_busca_lim_acessos_skiplist(nome_arquivo, n, lim));
                break;
            }
            default:
                printf("Opcao invalida!\n");
        }
    }while(opcao != 0);
}

/*
Sincroniza todas as estruturas de dados (Skiplist, Hash, Lista Ordenada, AVL, Trie) 
com o conteúdo do arquivo informado, garantindo que todas estejam atualizadas 
com os mesmos dados após qualquer alteração.
Parâmetros:
    skiplist - ponteiro para a skiplist
    tabela - ponteiro para a tabela hash
    cabeca - ponteiro para a lista ordenada
    raiz_avl - ponteiro para a árvore AVL
    trie_estado - ponteiro para a trie de estados
    trie_cultura - ponteiro para a trie de culturas
    nome_arquivo - nome do arquivo de dados (dataset)
*/
void sincronizar_estruturas(Skiplist **skiplist, TabelaHash *tabela, ItemLista **cabeca, ItemAVL **raiz_avl, Trie **trie_estado, Trie **trie_cultura, const char *nome_arquivo){
    
    if(*skiplist != NULL){
        libera_skiplist(*skiplist);
        *skiplist = iniciar_skiplist();
        carregar_dados_skiplist(*skiplist, nome_arquivo);
    }

    liberar_tabela_hash(tabela);
    iniciar_hash(tabela);
    carregar_dados_hash(tabela, nome_arquivo);

    libera_LO(*cabeca);
    *cabeca = NULL;
    carregar_dados_LO(cabeca, nome_arquivo);

    if(*raiz_avl != NULL){
        liberar_avl(*raiz_avl);
        *raiz_avl = NULL;
    }
    carregar_dados_avl(raiz_avl, nome_arquivo);

    if(*trie_estado != NULL){
            liberar_trie(*trie_estado);
        }
        *trie_estado = criar_trie();
        carregar_dados_trie(*trie_estado, nome_arquivo, 2);

        if(*trie_cultura != NULL){
            liberar_trie(*trie_cultura);
        }
        *trie_cultura = criar_trie();
        carregar_dados_trie(*trie_cultura, nome_arquivo, 3);

}

/*
Libera toda a memória alocada por todas as estruturas de dados utilizadas no sistema,
evitando vazamentos de memória ao sair do programa ou ao retornar ao menu principal.
Parâmetros:
    skiplist - ponteiro para a skiplist
    tabela - ponteiro para a tabela hash
    cabeca - ponteiro para a lista ordenada
    raiz_avl - ponteiro para a árvore AVL
    trie_estado - ponteiro para a trie de estados
    trie_cultura - ponteiro para a trie de culturas
*/
void liberar_estruturas(Skiplist **skiplist, TabelaHash *tabela, ItemLista **cabeca, ItemAVL **raiz_avl, Trie **trie_estado, Trie **trie_cultura) {
    if(*skiplist != NULL){
        libera_skiplist(*skiplist);
        *skiplist = NULL;
    }

    liberar_tabela_hash(tabela);
    iniciar_hash(tabela);

    if(*cabeca != NULL){
        libera_LO(*cabeca);
        *cabeca = NULL;
    }

    if(*raiz_avl != NULL){
        liberar_avl(*raiz_avl);
        *raiz_avl = NULL;
    }

    if(*trie_estado != NULL){
        liberar_trie(*trie_estado);
        *trie_estado = NULL;
    }
    if(*trie_cultura != NULL){
        liberar_trie(*trie_cultura);
        *trie_cultura = NULL;
    }

}

/*
Exibe o menu de operações CRUD, permitindo inserir uma nova amostra, buscar por ID,
buscar estado/cultura por prefixo, remover uma amostra ou listar todas as amostras ordenadas.
Chama as funções correspondentes para cada operação.
*/
void menu_crud(){
    int opcao;
    do{
        printf("\n===MENU CRUD===\n");
        printf("1 - Inserir nova amostra (Arvore AVL)\n");
        printf("2 - Buscar amostra por id (Hash)\n");
        printf("3 - Busca estado/cultura por prefixo (Trie e Hash)\n");
        printf("4 - Remover uma amostra (Skiplist)\n");
        printf("5 - Listar todas as amostras ordenadas (Lista Ordenada)\n");
        printf("0 - Voltar ao menu principal\n");
        printf("Escolha uma opcao\n");
        scanf("%d", &opcao);
        getchar();

        switch(opcao){
            case 0:{
                liberar_estruturas(&skiplist, &tabela, &cabeca, &raiz, &trie_estado, &trie_cultura);
                printf("Retornando ao menu principal...\n");
                break;
            }

            case 1:{
                carregar_dados_avl(&raiz, nome_arquivo);

                criar_amostra_avl(&raiz);

                salvar_dados_avl(raiz, nome_arquivo);

                sincronizar_estruturas(&skiplist, &tabela, &cabeca, &raiz, &trie_estado, &trie_cultura, nome_arquivo);

                printf("Alteracoes salvas\n");

                break;
            }

            case 2:{
                sincronizar_estruturas(&skiplist, &tabela, &cabeca, &raiz, &trie_estado, &trie_cultura, nome_arquivo);
                int id;
                printf("Digite o ID a ser buscado: ");
                scanf("%d", &id);
                getchar();
                ItemHash *buscado = buscar_tabela_hash(&tabela, id);
                if (buscado){
                    printf("  ID: %d | Ano: %d | Estado: %s | Cultura: %s | Preco/Ton: %.2f | Rendimento: %.2f | Producao: %.2f | Area: %.2f | Valor Total: %.2f\n",
                       buscado->id, buscado->ano, buscado->estado, buscado->cultura,
                       buscado->preco_ton, buscado->rendimento, buscado->producao,
                       buscado->area_plantada, buscado->valor_total);
                }else{
                    printf("Amostra nao encontrada.\n");
                }
                break;
            }

            case 3:{
                sincronizar_estruturas(&skiplist, &tabela, &cabeca, &raiz, &trie_estado, &trie_cultura, nome_arquivo);

                Trie *trie_estado = criar_trie();
                Trie *trie_cultura = criar_trie();

                carregar_dados_trie(trie_estado, nome_arquivo, 2); 
                carregar_dados_trie(trie_cultura, nome_arquivo, 3); 

                char estado[50];
                char cultura[50];

                printf("Digite o prefixo ou palavra inteira do estado (deixe vazio para ignorar): ");
                fgets(estado, sizeof(estado), stdin);
                estado[strcspn(estado, "\n")] = '\0';

                printf("Digite o prefixo ou palavra inteira da cultura (deixe vazio para ignorar): ");
                fgets(cultura, sizeof(cultura), stdin);
                cultura[strcspn(cultura, "\n")] = '\0';

                char estados_encontrados[100][50];
                int total_estados = 0;
                char culturas_encontradas[100][50];
                int total_culturas = 0;

                if(strlen(estado) > 0){
                    NoTrie *no_estado = trie_estado->raiz;
                    int len = strlen(estado);
                    int i = 0;
                    for(i; i < len; i++){
                        char c = estado[i];

                        if(c >= 'A' && c <= 'Z'){
                            c = c - 'A' + 'a';
                        }

                        int idx = indice_trie(c);

                        if(idx == -1){
                            continue;
                        }

                        if(no_estado->filhos[idx] == NULL){
                            break;
                        }

                        no_estado = no_estado->filhos[idx];
                    }

                    if(i == len){
                        char buffer[50];
                        strcpy(buffer, estado);
                        coletar_palavras_trie(no_estado, buffer, strlen(estado), estados_encontrados, &total_estados);
                    }
                }

                if(strlen(cultura) > 0){
                    NoTrie *no_cultura = trie_cultura->raiz;
                    int len = strlen(cultura);
                    int j = 0;
                    for(j; j < len; j++){
                        char c = cultura[j];
                        if(c >= 'A' && c <= 'Z'){
                            c = c - 'A' + 'a';
                        }
                        int idx = indice_trie(c);
                        if(idx == -1){
                            continue;
                        }
                        if(no_cultura->filhos[idx] == NULL){
                            break;
                        }
                        no_cultura = no_cultura->filhos[idx];
                    }
                    if(j == len){
                        char buffer[50];
                        strcpy(buffer, cultura);
                        coletar_palavras_trie(no_cultura, buffer, strlen(cultura), culturas_encontradas, &total_culturas);
                    }
                }

                int encontrados = 0;
                int k = 0;
                for(k; k < TAM; k++){
                    ItemHash *atual = tabela.tabela[k];
                    while(atual != NULL){
                        int estado_ok = 1, cultura_ok = 1;
                        if(strlen(estado) > 0){
                            estado_ok = 0;
                            int l = 0;
                            for(l; l < total_estados; l++){
                                if(strcasecmp(atual->estado, estados_encontrados[l]) == 0){
                                    estado_ok = 1;
                                    break;
                                }
                            }
                        }
                        if(strlen(cultura) > 0){
                            cultura_ok = 0;
                            int n = 0;
                            for(n; n < total_culturas; n++){
                                if(strcasecmp(atual->cultura, culturas_encontradas[n]) == 0){
                                    cultura_ok = 1;
                                    break;
                                }
                            }
                        }
                        if(estado_ok && cultura_ok){
                            printf("%d | %d | %s | %s | %.2f | %.2f | %.2f | %.2f | %.2f\n",
                                atual->id, atual->ano, atual->estado, atual->cultura, atual->preco_ton,
                                atual->rendimento, atual->producao, atual->area_plantada, atual->valor_total);
                            encontrados++;
                        }
                        atual = atual->prox;
                    }
                }

                if(encontrados == 0){
                    printf("Nenhum resultado encontrado.\n");
                }else{
                    printf("Total de resultados: %d\n", encontrados);
                }

                liberar_trie(trie_estado);
                liberar_trie(trie_cultura);
                break;
            }

            case 4:{
                sincronizar_estruturas(&skiplist, &tabela, &cabeca, &raiz, &trie_estado, &trie_cultura, nome_arquivo);
                int id;
                printf("Digite o ID a ser removido: ");
                scanf("%d", &id);
                getchar();

                ElementoSkiplist* encontrado = buscar_skiplist(skiplist, id);

                if(encontrado){
                    ElementoSkiplist temp;
                    temp = *encontrado;

                    printf("\nAmostra encontrada:\n");
                    printf("ID: %d | Ano: %d | Estado: %s | Cultura: %s | Preco/Ton: %.2f | Rendimento: %.2f | Producao: %.2f | Area: %.2f | Valor Total: %.2f\n",
                        temp.id, temp.ano, temp.estado, temp.cultura, temp.preco_ton, temp.rendimento,
                        temp.producao, temp.area_plantada, temp.valor_total);

                    char resposta;
                    printf("\nTem certeza que deseja remover esta amostra e salvar as alteracoes? (s/n): ");
                    scanf(" %c", &resposta);
                    getchar();

                    if(resposta == 's' || resposta == 'S'){
                        remover_skiplist(skiplist, id);
                        remover_tabela_hash(&tabela, id);
                        remover_LO(&cabeca, id);

                        salvar_dados_skiplist(skiplist, nome_arquivo);
                        salvar_dados_hash(&tabela, nome_arquivo);
                        salvar_dados_LO(cabeca, nome_arquivo);
                        printf("Amostra removida e alteracoes salvas no arquivo CSV!\n");
                    }else{
                        printf("Remocao cancelada. Nenhuma alteracao foi feita.\n");
                    }
                }else{
                    printf("Amostra de ID %d nao encontrada na skip list.\n", id);
                }
                break;
            }

            case 5:{
                sincronizar_estruturas(&skiplist, &tabela, &cabeca, &raiz, &trie_estado, &trie_cultura, nome_arquivo);
                imprime_LO(cabeca);
                break;
                }

            default:
                printf("Opcao invalida!\n");
        }
    }while(opcao != 0);
}

/*
Exibe o menu principal do sistema, permitindo ao usuário escolher entre o modo CRUD,
o modo benchmark ou encerrar o programa.
Chama as funções de menu específicas conforme a opção escolhida.
*/
void menu_principal(){
    int opcao;
    do{
        printf("\n===MENU PRINCIPAL===\n");
        printf("1 - CRUD\n");
        printf("2 - Modo Benchmark\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
           scanf("%d", &opcao);
        getchar();

        switch (opcao){
            case 1:
                menu_crud();
                break;
            case 2:
                menu_benchmark();
                break;
            case 0:
                printf("Encerrando programa!\n");
            default:
                printf("Opcao invalida!\n");
        }
    } while(opcao != 0);
}

int main(){
    printf("Digite o nome do arquivo a ser lido: ");
    if (fgets(nome_arquivo, sizeof(nome_arquivo), stdin) != NULL) {
        nome_arquivo[strcspn(nome_arquivo, "\n")] = '\0';
    }

    iniciar_hash(&tabela);
    skiplist = iniciar_skiplist();
    cabeca = NULL;
    raiz = NULL;
    trie_estado = criar_trie();
    trie_cultura = criar_trie();

    sincronizar_estruturas(&skiplist, &tabela, &cabeca, &raiz, &trie_estado, &trie_cultura, nome_arquivo);

    menu_principal();

    liberar_estruturas(&skiplist, &tabela, &cabeca, &raiz, &trie_estado, &trie_cultura);

    return 0;
}
