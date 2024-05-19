/**
 * Trabalho 02 - Estrutura de Dados III
 *
 * Integrantes do grupo:
 *
 * Leonardo Hannas de Carvalho Santos  NUSP: 11800480
 * Lucas Carvalho Freiberger Stapf     NUSP: 11800559
 */

#ifndef GRAFO_H
#define GRAFO_H

#include "constants.h"
#include "linkedlist.h"

typedef struct VD {
    char *nomeVertice;
    int distancia;
} VD;

typedef struct adjacentVertex {
    char nomeProxEstacao[MAX_SIZE_STR];
    int distancia;
    linkedlist nomesLinhas;
} adjacentVertex;

typedef struct vertex {
    char nomeEstacao[MAX_SIZE_STR];
    linkedlist verticesAdjacentes;
} vertex;

typedef struct graph {
    unsigned int maxSize;
    unsigned int size;
    vertex **vertices;
} graph;

int vertexCompare(vertex v1, vertex v2);
int adjVertexCompare(adjacentVertex v1, adjacentVertex v2);

int hasVertex(graph g, char *nomeEstacao);
int hasAdjVertex(vertex v, char *nomeProxEstacao);

int createGraph(graph *g, int maxSize);
vertex *createVertex(char *nomeEstacao);
adjacentVertex *createAdjcentVertex(char *nomeEstacao, int distancia);

void printGraph(graph g);
void printVertex(vertex v);
void printAdjVertex(adjacentVertex adjV);

int insertEdge(graph *g, char *verticeOrigem, char *verticeDestino, int distancia, char *nomeLinha, int directedGraph);
void insertVertex(graph *g, vertex *v);
void insertAdjVertex(vertex *v, adjacentVertex *adjV);

void updateNomesLinhas(adjacentVertex *adjV, char *nomeLinha);
graph* createGraphFromBIN(char *filename, int directedGraph);

int dijkstraAlgorithm(graph g, char *nomeOrigem, char *nomeDestino, linkedlist *caminho, int *distanciaTotal);
void dfsAlgorithm(graph g, char *nomeOrigem, char *nomeDestino, linkedlist *caminho, int *distanciaTotal);
void primAlgorithm(graph g, char *nomeOrigem, graph *mst);

void printDFS(graph g, char *nomeOrigem);

int comparePaths(void *path1, void *path2);
void findAllPaths(graph g, char *nomeOrigem, char *nomeDestino, linkedlist *caminhos);

#endif