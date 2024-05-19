/**
* Trabalho 02 - Estrutura de Dados III
*
* Integrantes do grupo:
*
* Leonardo Hannas de Carvalho Santos  NUSP: 11800480
* Lucas Carvalho Freiberger Stapf     NUSP: 11800559
*/

#include <stdio.h>
#include "../headers/sort.h"
#include "../headers/constants.h"

/**
 * Esta função utiliza o algoritmo do bubble-sort otimizado para ordenar uma lista encadeada.
 * É necessário passar como argumento a função que será utilizada para comparar os elementos da lista.
 * @param l lista a ser ordenada.
 * @param comparator função que compara dois elementos da lista. Se o primeiro elemento for
 * maior que o segundo, a função deve retornar um valor maior que zero.
 * Se for igual ao segundo, a função deve retorna zero.
 * Se for menor que o segundo, a função deve retornar um valor menor que zero.
 */
void sortLinkedList(linkedlist *l, int(*comparator)()) {

    if (l == NULL || l->size == 0) return;

    int sorted = TRUE;

    node *prevNode, *node, *aux;

    for (int i = 0; i < l->size - 1; i++) {

        prevNode = NULL;
        node = l->head;

        for (int j = 0; j < l->size - 1 - i; j++) {

            if (comparator(node->data, node->next->data) > 0) {

                sorted = FALSE;

                aux = node->next;
                node->next = node->next->next;
                aux->next = node;

                if (j == 0) l->head = prevNode = aux;
                else prevNode->next = aux;

                prevNode = aux;

            } else {

                prevNode = node;
                node = node->next;

            }
        }

        if (sorted == TRUE) return;
    }
}

/**
 * Ordena um array utilizando o algoritmo de bubble-sort otimizado.
 * Utiliza uma função de comparação que deve ser passada como argumento para poder realizar as comparações
 * entre os elementos do array.
 * @param array vetor a ser ordenado.
 * @param size tamanho do vetor.
 * @param comparator função que compara dois elementos do array. Se o primeiro elemento for
 * maior que o segundo, a função deve retornar um valor maior que zero.
 * Se for igual ao segundo, a função deve retorna zero.
 * Se for menor que o segundo, a função deve retornar um valor menor que zero.
 */
void sortArray(void **array, unsigned int size, int(*comparator)()) {

    if (array == NULL || size == 0) return;
    int sorted = TRUE;

    void *aux;

    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - 1 - i; j++) {
            if (comparator(array[j], array[j + 1]) > 0) {

                sorted = FALSE;
                aux = array[j];
                array[j] = array[j + 1];
                array[j + 1] = aux;
            }
        }
        if (sorted == TRUE) return;
    }
}