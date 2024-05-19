/**
 * Trabalho 02 - Estrutura de Dados III
 *
 * Integrantes do grupo:
 *
 * Leonardo Hannas de Carvalho Santos  NUSP: 11800480
 * Lucas Carvalho Freiberger Stapf     NUSP: 11800559
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "../headers/binmanager.h"
#include "../headers/graph.h"
#include "../headers/sort.h"

typedef struct estruturaAux {
    int distancia;
    char *antecessor;
} estruturaAux;

enum COLOR {
    WHITE,
    GRAY,
    BLACK
};

/**
 * @brief Esta funcao compara duas strings
 * @param str1 Primeira string
 * @param str2 Segunda string
 * @return 0 caso as strings comparadas forem iguais,
 *         > 0 caso str1 > str2,
 *         < 0 caso str1 < str2
 */
int compareStrVoid(void *str1, void *str2) {
    char *STR1 = str1;
    char *STR2 = str2;
    return strcmp(STR1, STR2);
}

/**
 * @brief Compara os nomes de dois vertices de um grafo
 * @param v1 Ponteiro para o primeiro vertice
 * @param v2 Ponteiro para o segundo vertice
 * @return 0 caso os nomes das strings comparadas forem iguais,
 *         > 0 caso v1.nomeEstacao > v2.nomeEstacao,
 *         < 0 caso v1.nomeEstacao < v2.nomeEstacao
 */
int vertexCompareVoid(void *v1, void *v2) {
    vertex *V1 = v1;
    vertex *V2 = v2;
    return vertexCompare(*V1, *V2);
}

/**
 * @brief Compara os nomes de dois vertices de um grafo
 * @param v1 Vertice do grafo
 * @param v2 Vertice do grafo
 * @return 0 caso os nomes das strings comparadas forem iguais,
 *         > 0 caso v1.nomeEstacao > v2.nomeEstacao,
 *         < 0 caso v1.nomeEstacao < v2.nomeEstacao
 */
int vertexCompare(vertex v1, vertex v2) {
    return strcmp(v1.nomeEstacao, v2.nomeEstacao);
}

/**
 * @brief Compara os nomes de dois de uma lista de adjacencias em um grafo
 *
 * @param v1 Vertice de uma lista de adjacencia do grafo
 * @param v2 Vertice de uma lista de adjacencia do grafo
 *
 * @return 0 caso os nomes das strings comparadas forem iguais,
 *         > 0 caso v1.nomeEstacao > v2.nomeEstacao,
 *         < 0 caso v1.nomeEstacao < v2.nomeEstacao
 */
int adjVertexCompareVoid(void *v1, void *v2) {

    adjacentVertex *V1 = v1;
    adjacentVertex *V2 = v2;
    return adjVertexCompare(*V1, *V2);
}

/**
 * @brief Compara os nomes de dois de uma lista de adjacencias em um grafo
 * @param v1 Vertice de uma lista de adjacencia do grafo
 * @param v2 Vertice de uma lista de adjacencia do grafo
 * @return 0, caso os nomes das strings comparadas forem iguais
 *         > 0, caso v1.nomeEstacao > v2.nomeEstacao
 *         < 0, caso v1.nomeEstacao < v2.nomeEstacao
 */
int adjVertexCompare(adjacentVertex v1, adjacentVertex v2) {
    return strcmp(v1.nomeProxEstacao, v2.nomeProxEstacao);
}

/**
 * @brief Verfica se, dado um nome de estacao, tal estacao pertence ao grafo
 * @param g Grafo
 * @param nomeEstacao String contendo o nome da estacao de metro
 * @return Posicao "i" do vertice do grafo, caso a estacao procurada for encontrada. Caso contrario, retorna -1.
 */
int hasVertex(graph g, char *nomeEstacao) {
    for (int i = 0; i < g.size; i++) if (strcmp(g.vertices[i]->nomeEstacao, nomeEstacao) == 0) return i;
    return -1;
}

/**
 * @brief Verifica se, dado um nome de estacao, tal estacao pertence a lista de adjacencias do vertice "v" no grafo
 * @param v Vertice pertencente ao grafo
 * @param nomeProxEstacao Nome da estacao a ser procurada na lista de adjacencias do vertice "v"
 * @return Posicao "i" da estacao na lista de adjacencias do vertice "v" do grafo, caso a estacao procurada for
 *         encontrada. Caso contrario, retorna -1.
 */
int hasAdjVertex(vertex v, char *nomeProxEstacao) {
    node *aux = v.verticesAdjacentes.head;
    for (int i = 0; i < v.verticesAdjacentes.size; i++) {
        adjacentVertex *adjAux = aux->data;
        if (strcmp(nomeProxEstacao, adjAux->nomeProxEstacao) == 0) return i;
        else aux = aux->next;
    }
    return -1;
}

/**
 * @brief Aloca a memoria necessaria para a criacao do grafo
 * @param g Ponteiro para o grafo a ser criado
 * @param maxSize Numero maximo de vertices do grafo
 * @return ERROR caso nao tenha sido possivel alocar a memoria,
 *         SUCCESS caso a alocacao de memoria tenha sido sucedida
 */
int createGraph(graph *g, int maxSize) {

    g->vertices = malloc(sizeof(vertex*) * maxSize);
    if (g->vertices == NULL) return ERROR;
    g->maxSize = maxSize;
    g->size = 0;

    for (int i = 0; i < maxSize; i++) g->vertices[i] = NULL;

    return SUCCESS;
}

/**
 *
 * @param g Ponteiro para o grafo a ser criado
 * @param verticeOrigem Nome da estacao de origem
 * @param verticeDestino Nome da estacao de destino
 * @param distancia Distancia entre a estacao de origem e a estacao de destino
 * @param nomeLinha Nome da linha responsavel por ligar as estacoes de destino e origem
 * @param directedGraph Variavel booleana para indicar se a aresta a ser adicionada eh (ou nao) direcional
 * @return ERROR caso a insercao da aresta nao tenha sido sucedida,
 *         SUCCESS caso contrario
 */
int insertEdge(graph *g, char *verticeOrigem, char *verticeDestino, int distancia, char *nomeLinha, int directedGraph) {

    if (g == NULL) return ERROR;

    int indexV = hasVertex(*g, verticeOrigem);

    if (indexV == -1) { // Vértice de origem não encontrado.

        vertex *v = createVertex(verticeOrigem);
        adjacentVertex *adjV = createAdjcentVertex(verticeDestino, distancia);
        addStringLinkedList(&adjV->nomesLinhas, (void*) nomeLinha);
        insertAdjVertex(v, adjV);
        insertVertex(g, v);

    } else { // Vértice de origem encontrado.

        vertex *v = g->vertices[indexV];

        int indexAdjV = hasAdjVertex(*v, verticeDestino);

        if (indexAdjV == -1) { // Não existe aresta entre os vértices passados.

            adjacentVertex *adjV = createAdjcentVertex(verticeDestino, distancia);
            addStringLinkedList(&adjV->nomesLinhas, (void*) nomeLinha);
            insertAdjVertex(v, adjV);

        } else { // Existe aresta

            vertex *v = g->vertices[indexV];
            node *n = getNodeLinkedList(v->verticesAdjacentes, indexAdjV);
            adjacentVertex  *adjV = (adjacentVertex*) n->data;

            if (hasDataLinkedList(adjV->nomesLinhas, nomeLinha, strcmp) == -1) {
                updateNomesLinhas(adjV, nomeLinha);
            }
        }
    }

    if (directedGraph == FALSE) insertEdge(g, verticeDestino, verticeOrigem, distancia, nomeLinha, TRUE);

    return SUCCESS;
}

/**
 * @brief Imprime o grafo conforme especificação do trabalho.
 * @param g grafo a ser mostrado na tela.
 */
void printGraph(graph g) {
    for (int i = 0; i < g.size; i++) {
        printVertex(*g.vertices[i]);
        if (i < g.size - 1) printf("\n");
    }
}

/**
 * @brief Imprime um vértice conforme especificação do trabalho.
 * @param v vértice a ser mostrado na tela.
 */
void printVertex(vertex v) {
    if (v.verticesAdjacentes.size == 0) printf("%s", v.nomeEstacao);
    else {
        printf("%s", v.nomeEstacao);
        node *n = v.verticesAdjacentes.head;
        for (int i = 0; i < v.verticesAdjacentes.size; i++) {
            printf(" ");
            adjacentVertex *adjV = n->data;
            printAdjVertex(*adjV);
            n = n->next;
        }
    }
}

/**
 * @brief Imprime um vértice adjacente conforme a especificação do trabalho.
 * @param adjV vértice adjcente a ser mostrado na tela.
 */
void printAdjVertex(adjacentVertex adjV) {
    printf("%s %d ", adjV.nomeProxEstacao, adjV.distancia);
    printStringLinkedList(adjV.nomesLinhas);
}

/**
 * @brief Cria um vertice e sua respectiva lista de adjacencia
 * @param nomeEstacao 
 * @return
 */
vertex *createVertex(char *nomeEstacao) {
    vertex *v = malloc(sizeof(vertex));
    strcpy(v->nomeEstacao, nomeEstacao);
    createLinkedList(&v->verticesAdjacentes);
    return v;
}
/**
 * @brief
 * @param nomeEstacao
 * @param distancia
 * @return
 */
adjacentVertex *createAdjcentVertex(char *nomeEstacao, int distancia) {
    adjacentVertex *adjV = malloc(sizeof(adjacentVertex));
    strcpy(adjV->nomeProxEstacao, nomeEstacao);
    adjV->distancia = distancia;
    createLinkedList(&adjV->nomesLinhas);
    return adjV;
}

/**
 * @brief Insere um novo vértice no grafo.
 * OBS: Não verifica se o vértice já se encontra no grafo ou não.
 * @param g grafo onde o vértice será inserido.
 * @param v vértice a ser inserido.
 */
void insertVertex(graph *g, vertex *v) {
    g->vertices[g->size] = v;
    g->size++;
    sortArray((void**) g->vertices, g->size, vertexCompareVoid);
}

/**
 * @brief Insere um vértice adjacente à lista de vértices adjacentes de um determinado vértice.
 * OBS: Não verifica se o vértice adjacente já está presente na lista ou não.
 * @param v vértice onde será inserido o vértice adjacente.
 * @param adjV vértice adjcente a ser inserido.
 */
void insertAdjVertex(vertex *v, adjacentVertex *adjV) {
    addElementLinkedList(&v->verticesAdjacentes, (void*) adjV);
    sortLinkedList(&v->verticesAdjacentes, adjVertexCompareVoid);
}

/**
 * @brief Adiciona a uma lista de nome de linhas de um novo elemento.
 * OBS: Não verifica se o nome da linha já está presente na lista ou não.
 * @param adjV vértice onde a nova linha será adicionada.
 * @param nomeLinha nome da linha a ser adicionada.
 */
void updateNomesLinhas(adjacentVertex *adjV, char *nomeLinha) {
    addStringLinkedList(&adjV->nomesLinhas, nomeLinha);
    sortLinkedList(&adjV->nomesLinhas, compareStrVoid);
}

/**
 * @brief Cria um grafo a partir do arquivo binário passado.
 * @param filename nome do arquivo binário de dados.
 * @param directedGraph diretriz para grafo ser direcional ou não.
 * @return retorna um ponteiro para um grafo caso tudo ocorra devidamente.
 * Retorna NULL caso ocorra alguma falha.
 */
graph* createGraphFromBIN(char *filename, int directedGraph) {

    FILE *f = fopen(filename, "rb");

    if (f == NULL) return NULL;

    HeaderRegister hr;
    readHeaderRegisterBIN(f, &hr);

    graph *g = malloc(sizeof(graph));
    createGraph(g, hr.nroEstacoes);

    DataRegister dr, dr_search, dr_return;

    while (!feof(f)) {

        if (readDataRegisterBIN(f, &dr) == NOT_REMOVED) {

            setEmptyDataRegister(&dr_search);
            dr_search.codEstacao = dr.codProxEstacao;

            if (findDataRegisterBIN(f, &dr_search, &dr_return) == REGISTER_FOUND) {
                insertEdge(g, dr.nomeEstacao, dr_return.nomeEstacao,
                           dr.distProxEstacao, dr.nomeLinha, directedGraph);
            } else if(hasVertex(*g, dr.nomeEstacao) == -1) {
                vertex *v = createVertex(dr.nomeEstacao);
                insertVertex(g, v);
            }

            dr_search.codEstacao = dr.codEstIntegra;

            if (findDataRegisterBIN(f, &dr_search, &dr_return) == REGISTER_FOUND) {
                if (strcmp(dr.nomeEstacao, dr_return.nomeEstacao) != 0) {
                    insertEdge(g, dr.nomeEstacao, dr_return.nomeEstacao,
                               0, "Integracao", directedGraph);
                }
            }
        }
    }

    return g;
}

/**
 * @brief Calcula o caminho de menor distancia entre uma estacao de 
					origem e uma estacao de destino, segundo o Algortimo de Dijkstra
 * @param g Grafo
 * @param nomeOrigem Nome da estacao de origem
 * @param nomeDestino Nome da estacao de destino
 * @param caminho Lista encadeada que armazena o menor caminho
 * @param distanciaTotal Ponteiro para armazenar a distancia total a ser percorrida no caminho
 * @return PATH_NOT_FOUND caso nao haja um caminho entre os vertices passados,
					 SUCCESS caso tenha sido calculado o menor caminho
 */
int dijkstraAlgorithm(graph g, char *nomeOrigem, char *nomeDestino, linkedlist *caminho, int *distanciaTotal) {

    vertex **S = malloc(sizeof(vertex*) * g.size);
    int sizeOfS = 0;
    vertex **V = malloc(sizeof(vertex*) * g.size);
    int sizeOfV = g.size;


    estruturaAux *D = malloc(sizeof(estruturaAux) * g.size);

    for (int i = 0; i < g.size; i++) {
        D[i].distancia = INT_MAX / 2; // distancia "infinita"
        D[i].antecessor = NULL;
        V[i] = g.vertices[i];
    }


    int index = hasVertex(g, nomeOrigem);
    S[index] = g.vertices[index];
    sizeOfS++;
    D[index].distancia = 0;
    D[index].antecessor = NULL;

    while (sizeOfS < g.size - 1) {

        linkedlist adjList = V[index]->verticesAdjacentes;
        iterator iter;
        createIterator(adjList, &iter);

        while (hasNextNode(&iter) == TRUE) {
            node *n = getNextNode(&iter);
            adjacentVertex *adjV = n->data;
            int indexAux = hasVertex(g, adjV->nomeProxEstacao);
            if (adjV->distancia + D[index].distancia < D[indexAux].distancia) {
                D[indexAux].distancia = adjV->distancia + D[index].distancia;
                D[indexAux].antecessor = V[index]->nomeEstacao;
            }
        }

        int dMin = INT_MAX;
        int indexAux = -1;

        V[index] = NULL;
        sizeOfV--;

        for (int j = 0; j < g.size; j++) {
            if (D[j].distancia < dMin && V[j] != NULL) {
                dMin = D[j].distancia;
                indexAux = j;
            }
        }

        S[index] = g.vertices[index];
        sizeOfS++;

        index = indexAux;

    }

    createLinkedList(caminho);
    index = hasVertex(g, nomeDestino);

    if (D[index].antecessor == NULL) return PATH_NOT_FOUND;

    *distanciaTotal = D[index].distancia;

    while (strcmp(nomeOrigem, g.vertices[index]->nomeEstacao) != 0) {
        addElementFirstLinkedList(caminho, g.vertices[index]->nomeEstacao);
        index = hasVertex(g, D[index].antecessor);
    }

    addElementFirstLinkedList(caminho, g.vertices[hasVertex(g, nomeOrigem)]->nomeEstacao);

    return SUCCESS;
}

/**
 * @brief Auxilia na busca em profundidade do grafo, segundo o algortimo de Depth-First Search
 * @param g Grafo
 * @param vOrigem Vertice de origem da busca 
 * @param vDestino Vertice de destino da busca 
 * @param vertices Vetor de cor dos vertices para controlar a busca. Vertices nao 
 *                 vistados sao WHITE, os ja visitados sao GRAY e os que ja foram 
 *                 visitados, bem como todos os seus adjacentes sao BLACK
 * @param est Estrutura que armazena o vertice antecessor, bem como a distancia com relacao
 *            ao atual
 * @return TRUE caso o vertice de destino tenha sido encontrado,
 *         FALSE caso contrario
 */
int dfsRecursion(graph g, vertex *vOrigem, vertex *vDestino, enum COLOR *vertices, estruturaAux *est) {

    int i = hasVertex(g, vOrigem->nomeEstacao);
    vertices[i] = GRAY;

    iterator iter;
    createIterator(vOrigem->verticesAdjacentes, &iter);

    while(hasNextNode(&iter) == TRUE) {

        node *nAux = getNextNode(&iter);
        adjacentVertex *adjVAux = nAux->data;
        int indexAux = hasVertex(g, adjVAux->nomeProxEstacao);

        if (vDestino != NULL && vertexCompare(*vDestino, *g.vertices[indexAux]) == TRUE) {

            est[indexAux].distancia = adjVAux->distancia;
            est[indexAux].antecessor = vOrigem->nomeEstacao;
            return TRUE;

        } else if (vertices[indexAux] == WHITE) {

            est[indexAux].distancia = adjVAux->distancia;
            est[indexAux].antecessor = vOrigem->nomeEstacao;
            if(dfsRecursion(g, g.vertices[indexAux], vDestino, vertices, est) == TRUE) return TRUE;
        }
    }

    vertices[i] = BLACK;
    return FALSE;
}

/**
 * @brief Faz a busca em profundidade do grafo, segundo o algortimo de Depth-First Search
 * @param g Grafo
 * @param nomeOrigem Nome do vertice de origem da busca 
 * @param nomeDestino Nome do vertice de destino da busca 
 * @param caminho Lista encadeada do caminho a ser percorrido
 * @param distanciaTotal Ponteiro para armazenar a distancia total a ser percorrida no caminho
 */
void dfsAlgorithm(graph g, char *nomeOrigem, char *nomeDestino, linkedlist *caminho, int *distanciaTotal) {

    createLinkedList(caminho);
    *distanciaTotal = 0;

    enum COLOR *vertices = malloc(sizeof(enum COLOR) * g.size);
    estruturaAux *est = malloc(sizeof(estruturaAux) * g.size);

    for (int i = 0; i < g.size; i++) {
        vertices[i] = WHITE;
        est[i].antecessor = NULL;
        est[i].distancia = -1;
    }

    int iOrigem = hasVertex(g, nomeOrigem);
    vertex *vOrigem = g.vertices[iOrigem];

    int iDestino = hasVertex(g, nomeDestino);
    vertex *vDestino = g.vertices[iDestino];

    dfsRecursion(g, vOrigem, vDestino, vertices, est);

    while (est[iDestino].antecessor != NULL) {

        addElementFirstLinkedList(caminho, g.vertices[iDestino]->nomeEstacao);
        int aux = iDestino;
        iDestino = hasVertex(g, est[iDestino].antecessor);
        *distanciaTotal += est[aux].distancia;
        est[aux].antecessor = NULL;
        est[aux].distancia = -1;
    }

    addElementFirstLinkedList(caminho, g.vertices[iOrigem]->nomeEstacao);
}


typedef struct { // vértice
    char *nome;
    enum COLOR cor;
}V;

typedef struct { // aresta
    V *v1;
    V *v2;
    int distancia;
}A;


/**
 * @brief Função que compara duas arestas conforme especificação do trabalho.
 * @param a1 primeira aresta.
 * @param a2 segunda aresta.
 * @return Retorna 1 caso a primeira aresta seja maior que a segunda, 0 caso ambas sejam iguais e -1
 * caso a primeira aresta seja menor que a primeira.
 */
int compareAresta(A a1, A a2) {
    if (a1.distancia == a2.distancia) {
        if (strcmp(a1.v1->nome, a2.v1->nome) == 0) {
            return strcmp(a1.v2->nome, a2.v2->nome);
        } else return strcmp(a1.v1->nome, a2.v1->nome);
    } else return a1.distancia - a2.distancia;
}

/**
 * @brief Função que compara duas arestas conforme especificação do trabalho.
 * @param a1 ponteiro do tipo (void*) que aponta para a primeira aresta.
 * @param a2 ponteiro do tipo (void*) que aponta para a segunda aresta.
 * @return
 */
int compareArestaVoid(void *a1, void *a2) {
    A *aresta1 = a1;
    A *aresta2 = a2;
    return compareAresta(*aresta1, *aresta2);
}

/**
 * @brief Função recursiva utilizada pela printDFS na impressão dos dados.
 * @param g grafo
 * @param nomeOrigem nome do vértice de origem.
 * @param vertices vetor de vértices que é utilizado para controlar se os vértices já foram 
 * analisados ou não.
 */
void printDFSRecursion(graph g, char *nomeOrigem, enum COLOR *vertices) {

    int index = hasVertex(g, nomeOrigem);
    vertices[index] = GRAY;

    vertex *v = g.vertices[index];
    iterator iter;
    createIterator(v->verticesAdjacentes, &iter);

    while (hasNextNode(&iter) == TRUE) {

        node *n = getNextNode(&iter);
        adjacentVertex *adjV = n->data;

        int indexAdj = hasVertex(g, adjV->nomeProxEstacao);

        if (vertices[indexAdj] == WHITE) {
            vertices[indexAdj] = GRAY;
            printf("%s, %s, %d\n", nomeOrigem, adjV->nomeProxEstacao, adjV->distancia);
            printDFSRecursion(g, adjV->nomeProxEstacao, vertices);
        }
    }

    vertices[index] = BLACK;
}

/**
 * @brief Função que utiliza a técnica de busca em profundidade para imprimir
 * os vértices do grafo.
 * @param g grafo.
 * @param nomeOrigem nome do vértice de origem.
 */
void printDFS(graph g, char *nomeOrigem) {

    enum COLOR *vertices = malloc(sizeof(enum COLOR) * g.size);
    for (int i = 0; i < g.size; i++) vertices[i] = WHITE;

    printDFSRecursion(g, nomeOrigem, vertices);
}

/**
 * @brief Função que utilizada o algortimo de PRIM para gerar uma árvore geradora mínima a partir de um
 * determinado vértice de origem.
 * @param g Grafo
 * @param nomeOrigem Nome da estacao de partida
 * @param mst árvore geradora mínima em formato de grafo.
 */
void primAlgorithm(graph g, char *nomeOrigem, graph *mst) {

    createGraph(mst, g.size);

    V vertices[g.size];

    linkedlist arestas;
    createLinkedList(&arestas);

    for (int i = 0; i < g.size; i++) { // criação dos vértices
        vertex *vertex1 = g.vertices[i];
        vertices[i].nome = vertex1->nomeEstacao;
        vertices[i].cor = WHITE;
    }

    for (int i = 0; i < g.size; i++) { // criação das arestas

        vertex *vertex1 = g.vertices[i];
        linkedlist adjV = vertex1->verticesAdjacentes;

        iterator iter;
        createIterator(adjV, &iter);

        while (hasNextNode(&iter) == TRUE) {

            node *n = getNextNode(&iter);
            adjacentVertex *adjacentVertex1 = n->data;

            int index = hasVertex(g, adjacentVertex1->nomeProxEstacao);

            A *aresta = malloc(sizeof(A));
            aresta->v1 = &vertices[i];
            aresta->v2 = &vertices[index];
            aresta->distancia = adjacentVertex1->distancia;

            addElementLinkedList(&arestas, aresta);
            sortLinkedList(&arestas, compareArestaVoid);
        }
    }

    int indexAtual = hasVertex(g, nomeOrigem);
    vertices[indexAtual].cor = BLACK;
    int percorridos = 1;

    while (percorridos < g.size) {

        iterator iter;
        createIterator(arestas, &iter);

        while (hasNextNode(&iter) == TRUE) {

            node *n = getNextNode(&iter);
            A *aresta = n->data;

            V *v1 = aresta->v1;
            V *v2 = aresta->v2;

            if (v1->cor == BLACK && v2->cor == BLACK) {
                removeNodeLinkedList(&arestas, n);
                continue;
            }

            if (v1->cor == WHITE && v2->cor == WHITE) continue;

            insertEdge(mst, v1->nome, v2->nome, aresta->distancia, NULL, FALSE);
            v1->cor = BLACK;
            v2->cor = BLACK;

            removeNodeLinkedList(&arestas, n);
            percorridos++;

            break;
        }
    }
}

/**
 * @brief Compara dois possiveis caminhos, vertice a vertice, segundo os\
 *        criterios de comparacao estabelecidos no Comando 11
 * @param path1 Ponteiro para o primeiro caminho a ser comparado
 * @param path2 Ponteiro para o primeiro caminho a ser comparado
 * @return 1 caso *path1 > *path2,
					 0 caso *path1 = *path2,
					 -1 caso *path1 < *path2
 */
int comparePaths(void *path1, void *path2) {

    linkedlist *p1 = path1;
    linkedlist *p2 = path2;

    if (p1->size > p2->size) return 1;
    else if (p1->size < p2->size) return -1;

    iterator i1, i2;
    createIterator(*p1, &i1);
    createIterator(*p2, &i2);

    while (hasNextNode(&i1) == TRUE) {

        node *n1 = getNextNode(&i1);
        node *n2 = getNextNode(&i2);

        VD *vd1 = n1->data;
        VD *vd2 = n2->data;

        int ret = strcmp(vd1->nomeVertice, vd2->nomeVertice);
        if (ret > 0) return 1;
        else if (ret < 0) return -1;
    }

    return 0;
}

/**
 * @brief Função recursiva utilizada pela findAllPaths para encontrar todos os caminhos entre dois vértices.
 * @param g grafo
 * @param nomeOrigem vértice de origem.
 * @param nomeDestino vértice de destino.
 * @param isVisited lista de vértices visitados
 * @param caminho lista encadeada que armazena o caminho atual.
 * @param caminhos lista encadeada que armazena todos os caminho encontrados.
 */
void findAllPathsRecursion(graph g, char *nomeOrigem, char *nomeDestino, int *isVisited, linkedlist *caminho, linkedlist *caminhos) {

    if (strcmp(nomeOrigem, nomeDestino) == 0) {
        linkedlist *l = malloc(sizeof(linkedlist));
        copyLinkedList(l, caminho);
        addElementLinkedList(caminhos, (void*) l);
        return;
    }

    int index = hasVertex(g, nomeOrigem);
    isVisited[index] = TRUE;

    vertex *vOrigem = g.vertices[index];

    iterator iter;
    createIterator(vOrigem->verticesAdjacentes, &iter);

    while (hasNextNode(&iter) == TRUE) {

        node *n = getNextNode(&iter);
        adjacentVertex *adjV = n->data;
        int indexAdj = hasVertex(g, adjV->nomeProxEstacao);

        if (isVisited[indexAdj] == FALSE) {

            VD *vd = malloc(sizeof(VD));
            vd->nomeVertice = adjV->nomeProxEstacao;
            vd->distancia = adjV->distancia;

            node *nAdd = addElementLinkedList(caminho, (void*) vd);
            findAllPathsRecursion(g, vd->nomeVertice, nomeDestino, isVisited, caminho, caminhos);
            removeNodeLinkedList(caminho, nAdd);
        }
    }

    isVisited[index] = FALSE;
}

/**
 * @brief Função que encontra todos os caminhos existentes a partir de um vértice de origem até um
 * vértice de destino.
 * @param g grafo.
 * @param nomeOrigem nome da estação de origem.
 * @param nomeDestino nome da estação de destino.
 * @param caminhos lista encadeada que armazena todos os caminhos encontrados.
 */
void findAllPaths(graph g, char *nomeOrigem, char *nomeDestino, linkedlist *caminhos) {

    int isVisited[g.size];
    for (int i = 0; i < g.size; i++) isVisited[i] = FALSE;

    linkedlist caminho;
    createLinkedList(&caminho);

    VD *vdOrigem = malloc(sizeof(VD));
    vdOrigem->nomeVertice = nomeOrigem;
    vdOrigem->distancia = 0;

    addElementLinkedList(&caminho, (void*) vdOrigem);

    findAllPathsRecursion(g, nomeOrigem, nomeDestino, isVisited, &caminho, caminhos);
}

